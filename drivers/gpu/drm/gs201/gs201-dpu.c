// SPDX-License-Identifier: GPL-2.0-only
/*
 * Google Tensor GS201 (Pixel 7/7a) display controller driver
 *
 * First stage: adopt the DECON/DPP/DSIM pipeline and the command-mode panel
 * that the bootloader leaves running. One RDMA (DPP) channel feeds DECON0,
 * which drives DSIM0 and the panel. The panel is in command mode and only
 * refreshes when DECON is triggered, so every plane update repoints the DMA
 * channel at the new buffer, latches the shadow registers and fires a
 * software trigger.
 */

#include <linux/bits.h>
#include <linux/clk.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/dma-mapping.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_graph.h>
#include <linux/platform_device.h>

#include <drm/clients/drm_client_setup.h>
#include <drm/drm_atomic_helper.h>
#include <drm/drm_bridge.h>
#include <drm/drm_connector.h>
#include <drm/drm_damage_helper.h>
#include <drm/drm_drv.h>
#include <drm/drm_fb_dma_helper.h>
#include <drm/drm_fbdev_dma.h>
#include <drm/drm_fourcc.h>
#include <drm/drm_framebuffer.h>
#include <drm/drm_gem_atomic_helper.h>
#include <drm/drm_gem_dma_helper.h>
#include <drm/drm_gem_framebuffer_helper.h>
#include <drm/drm_modes.h>
#include <drm/drm_vblank.h>
#include <drm/drm_probe_helper.h>
#include <drm/drm_simple_kms_helper.h>

#define GS201_DPU_WIDTH 1080
#define GS201_DPU_HEIGHT 2400

/* DPP (RDMA) channels: stride 0x1000, shadow copy of each register at +0x400 */
#define DPP_NUM_CHANNELS 6
#define DPP_CH_STRIDE 0x1000
#define DPP_SHADOW 0x400
#define DPP_ENABLE 0x00
#define DPP_ENABLE_SFR_UPDATE_FORCE BIT(4)
#define DPP_SRC_SIZE 0x10
#define DPP_IMG_SIZE 0x18
#define DPP_BASEADDR_Y8 0x40

/* DECON */
#define DECON_GLOBAL_CON		0x20
#define DECON_GLOBAL_CON_EN		BIT(0)
#define DECON_GLOBAL_CON_EN_F		BIT(1)
#define DECON_TRIG_CON			0x30
#define DECON_TRIG_CON_SECURE		0x3c
#define DECON_TRIG_HW_MASK		BIT(4)
#define DECON_TRIG_SW_EN		BIT(8)
#define DECON_TRIG_SW_DET_EN		BIT(1)
#define DECON_TRIG_HW_EN		BIT(0)
#define DECON_INT_EN			0x60
#define DECON_INT_EN_FRAME_START	BIT(12)
#define DECON_INT_EN_FRAME_DONE		BIT(13)
#define DECON_INT_EN_GLOBAL		BIT(0)
#define DECON_INT_PEND			0x70
#define DECON_INT_PEND_FRAME_START	BIT(12)
#define DECON_INT_PEND_FRAME_DONE	BIT(13)
#define DECON_SHD_REG_UP_REQ		0x50
#define DECON_SHD_REG_UP_REQ_ALL	(BIT(31) | 0x3f)

static const char *const gs201_dpu_clk_names[] = {
	"dma", "dpp", "dpu-apb", "decon", "disp-apb",
};

struct gs201_dpu {
	struct drm_device drm;
	struct drm_simple_display_pipe pipe;
	struct drm_connector connector;
	struct clk_bulk_data clks[ARRAY_SIZE(gs201_dpu_clk_names)];
	void __iomem *decon;
	void __iomem *dpp;
	unsigned int ch;

	/*
	 * The command-mode panel only refreshes when DECON is triggered. At
	 * most one frame is in flight; a commit that arrives meanwhile is
	 * remembered and sent once the frame-done interrupt fires. The flip
	 * event of the commit is completed by that interrupt too.
	 */
	spinlock_t lock;
	struct delayed_work timeout_work;
	struct drm_pending_vblank_event *event;
	bool busy;
	bool started;
	bool sw_fallback;
	bool kick_pending;
	u32 next_addr;
	u32 next_pitch;
	u32 next_height;
};

#define to_dpu(d) container_of(d, struct gs201_dpu, drm)

static const struct drm_display_mode gs201_dpu_mode = {
	DRM_SIMPLE_MODE(GS201_DPU_WIDTH, GS201_DPU_HEIGHT, 65, 144),
};

static const u32 gs201_dpu_formats[] = { DRM_FORMAT_XRGB8888 };

static void dpp_write(struct gs201_dpu *dpu, unsigned int reg, u32 val)
{
	void __iomem *base = dpu->dpp + dpu->ch * DPP_CH_STRIDE;

	writel(val, base + reg);
	writel(val, base + reg + DPP_SHADOW);
}

/*
 * One-shot hardware trigger, as the vendor driver does it: unmask the
 * trigger after the shadow update and DECON sends exactly one frame on the
 * next TE edge, which keeps the transfer in step with the panel's own scan.
 * The bootloader leaves the trigger unmasked, which makes DECON free-run a
 * frame on every TE edge.
 */
static void gs201_dpu_trigger_mask(struct gs201_dpu *dpu)
{
	u32 v = readl(dpu->decon + DECON_TRIG_CON);

	v &= ~(DECON_TRIG_HW_EN | DECON_TRIG_SW_EN | DECON_TRIG_SW_DET_EN);
	writel(v | DECON_TRIG_HW_MASK, dpu->decon + DECON_TRIG_CON);
}

static void gs201_dpu_trigger_unmask(struct gs201_dpu *dpu)
{
	u32 v = readl(dpu->decon + DECON_TRIG_CON);

	v &= ~(DECON_TRIG_HW_MASK | DECON_TRIG_SW_EN | DECON_TRIG_SW_DET_EN);
	writel(v | DECON_TRIG_HW_EN, dpu->decon + DECON_TRIG_CON);
}

/* Program the next framebuffer, latch the shadow registers and refresh. */
static void gs201_dpu_kick(struct gs201_dpu *dpu)
{
	void __iomem *dpp = dpu->dpp + dpu->ch * DPP_CH_STRIDE;
	u32 v;

	/*
	 * SRC_SIZE is the frame size as laid out in memory; linear RGB has no
	 * stride register, so a padded pitch is expressed as a wider frame.
	 */
	dpp_write(dpu, DPP_SRC_SIZE,
		  (dpu->next_height << 16) | (dpu->next_pitch / 4));
	dpp_write(dpu, DPP_BASEADDR_Y8, dpu->next_addr);

	v = readl(dpu->decon + DECON_GLOBAL_CON);
	if (!(v & DECON_GLOBAL_CON_EN_F))
		writel(v | DECON_GLOBAL_CON_EN | DECON_GLOBAL_CON_EN_F,
		       dpu->decon + DECON_GLOBAL_CON);

	writel(readl(dpp + DPP_ENABLE) | DPP_ENABLE_SFR_UPDATE_FORCE,
	       dpp + DPP_ENABLE);
	writel(DECON_SHD_REG_UP_REQ_ALL, dpu->decon + DECON_SHD_REG_UP_REQ);

	v = readl(dpu->decon + DECON_TRIG_CON_SECURE);
	writel(v & ~DECON_TRIG_HW_MASK, dpu->decon + DECON_TRIG_CON_SECURE);

	dpu->busy = true;
	dpu->started = false;
	dpu->sw_fallback = false;
	dpu->kick_pending = false;
	schedule_delayed_work(&dpu->timeout_work, msecs_to_jiffies(100));

	gs201_dpu_trigger_unmask(dpu);
}

/* The panel has the frame: complete the flip and start a queued one. */
static void gs201_dpu_frame_done_locked(struct gs201_dpu *dpu)
{
	struct drm_crtc *crtc = &dpu->pipe.crtc;

	dpu->busy = false;

	if (dpu->event) {
		unsigned long flags;

		spin_lock_irqsave(&crtc->dev->event_lock, flags);
		drm_crtc_send_vblank_event(crtc, dpu->event);
		spin_unlock_irqrestore(&crtc->dev->event_lock, flags);
		dpu->event = NULL;
	}

	if (dpu->kick_pending)
		gs201_dpu_kick(dpu);
}

static irqreturn_t gs201_dpu_irq(int irq, void *data)
{
	struct gs201_dpu *dpu = data;
	u32 pend = readl(dpu->decon + DECON_INT_PEND);

	pend &= DECON_INT_PEND_FRAME_START | DECON_INT_PEND_FRAME_DONE;
	if (!pend)
		return IRQ_NONE;

	writel(pend, dpu->decon + DECON_INT_PEND);

	spin_lock(&dpu->lock);
	if (pend & DECON_INT_PEND_FRAME_START) {
		/* One frame per kick: stop DECON from sending another. */
		gs201_dpu_trigger_mask(dpu);
		dpu->started = true;
	}
	if ((pend & DECON_INT_PEND_FRAME_DONE) && dpu->busy) {
		cancel_delayed_work(&dpu->timeout_work);
		gs201_dpu_frame_done_locked(dpu);
	}
	spin_unlock(&dpu->lock);

	return IRQ_HANDLED;
}

/* Never let a missed interrupt stall the compositor. */
static void gs201_dpu_timeout_work(struct work_struct *work)
{
	struct gs201_dpu *dpu = container_of(to_delayed_work(work),
					     struct gs201_dpu, timeout_work);

	spin_lock_irq(&dpu->lock);
	if (dpu->busy && !dpu->started && !dpu->sw_fallback) {
		/* No TE edge arrived: push the frame out with a SW trigger. */
		u32 v = readl(dpu->decon + DECON_TRIG_CON);

		dev_warn_ratelimited(dpu->drm.dev, "no TE, SW trigger\n");
		dpu->sw_fallback = true;
		writel(v | DECON_TRIG_SW_EN | DECON_TRIG_SW_DET_EN,
		       dpu->decon + DECON_TRIG_CON);
		schedule_delayed_work(&dpu->timeout_work,
				      msecs_to_jiffies(100));
	} else if (dpu->busy) {
		dev_warn_ratelimited(dpu->drm.dev, "frame done timeout\n");
		gs201_dpu_trigger_mask(dpu);
		gs201_dpu_frame_done_locked(dpu);
	}
	spin_unlock_irq(&dpu->lock);
}

static void gs201_dpu_enable(struct drm_simple_display_pipe *pipe,
			     struct drm_crtc_state *crtc_state,
			     struct drm_plane_state *plane_state)
{
	struct gs201_dpu *dpu = to_dpu(pipe->crtc.dev);

	dpp_write(dpu, DPP_IMG_SIZE,
		  (GS201_DPU_HEIGHT << 16) | GS201_DPU_WIDTH);

	/* Stop the bootloader's free-running trigger; kick() sends frames. */
	gs201_dpu_trigger_mask(dpu);

	writel(DECON_INT_PEND_FRAME_START | DECON_INT_PEND_FRAME_DONE,
	       dpu->decon + DECON_INT_PEND);
	writel(readl(dpu->decon + DECON_INT_EN) |
	       DECON_INT_EN_FRAME_START | DECON_INT_EN_FRAME_DONE |
	       DECON_INT_EN_GLOBAL,
	       dpu->decon + DECON_INT_EN);
}

static void gs201_dpu_disable(struct drm_simple_display_pipe *pipe)
{
	struct gs201_dpu *dpu = to_dpu(pipe->crtc.dev);

	/* The panel stays powered; nothing to do until DSIM/panel are owned. */
	writel(readl(dpu->decon + DECON_INT_EN) & ~DECON_INT_EN_FRAME_DONE,
	       dpu->decon + DECON_INT_EN);
	cancel_delayed_work_sync(&dpu->timeout_work);

	spin_lock_irq(&dpu->lock);
	gs201_dpu_frame_done_locked(dpu);
	dpu->kick_pending = false;
	spin_unlock_irq(&dpu->lock);
}

static void gs201_dpu_update(struct drm_simple_display_pipe *pipe,
			     struct drm_plane_state *old_state)
{
	struct gs201_dpu *dpu = to_dpu(pipe->crtc.dev);
	struct drm_plane_state *state = pipe->plane.state;
	struct drm_crtc *crtc = &pipe->crtc;
	struct drm_pending_vblank_event *event;

	if (!state->fb)
		return;

	spin_lock_irq(&dpu->lock);

	/* Take over the flip event: it completes on frame done, not at once. */
	event = crtc->state->event;
	if (event) {
		crtc->state->event = NULL;
		if (dpu->event) {
			unsigned long flags;

			spin_lock_irqsave(&crtc->dev->event_lock, flags);
			drm_crtc_send_vblank_event(crtc, dpu->event);
			spin_unlock_irqrestore(&crtc->dev->event_lock, flags);
		}
		dpu->event = event;
	}

	dpu->next_addr =
		lower_32_bits(drm_fb_dma_get_gem_addr(state->fb, state, 0));
	dpu->next_pitch = state->fb->pitches[0];
	dpu->next_height = state->fb->height;

	if (dpu->busy)
		dpu->kick_pending = true;
	else
		gs201_dpu_kick(dpu);

	spin_unlock_irq(&dpu->lock);
}

static const struct drm_simple_display_pipe_funcs gs201_dpu_pipe_funcs = {
	.enable = gs201_dpu_enable,
	.disable = gs201_dpu_disable,
	.update = gs201_dpu_update,
};

static int gs201_dpu_get_modes(struct drm_connector *connector)
{
	struct drm_display_mode *mode;

	mode = drm_mode_duplicate(connector->dev, &gs201_dpu_mode);
	if (!mode)
		return 0;
	drm_mode_probed_add(connector, mode);
	return 1;
}

static const struct drm_connector_helper_funcs
	gs201_dpu_connector_helper_funcs = {
		.get_modes = gs201_dpu_get_modes,
	};

static const struct drm_connector_funcs gs201_dpu_connector_funcs = {
	.reset = drm_atomic_helper_connector_reset,
	.fill_modes = drm_helper_probe_single_connector_modes,
	.destroy = drm_connector_cleanup,
	.atomic_duplicate_state = drm_atomic_helper_connector_duplicate_state,
	.atomic_destroy_state = drm_atomic_helper_connector_destroy_state,
};

static const struct drm_mode_config_funcs gs201_dpu_mode_config_funcs = {
	.fb_create = drm_gem_fb_create_with_dirty,
	.atomic_check = drm_atomic_helper_check,
	.atomic_commit = drm_atomic_helper_commit,
};

DEFINE_DRM_GEM_DMA_FOPS(gs201_dpu_fops);

static const struct drm_driver gs201_dpu_driver = {
	.driver_features = DRIVER_MODESET | DRIVER_GEM | DRIVER_ATOMIC,
	.fops = &gs201_dpu_fops,
	.name = "gs201-dpu",
	.desc = "Google Tensor GS201 DPU",
	.major = 1,
	.minor = 0,
	DRM_GEM_DMA_DRIVER_OPS,
	DRM_FBDEV_DMA_DRIVER_OPS,
};

/* Find the DPP channel the bootloader is scanning out of. */
static int gs201_dpu_find_channel(struct gs201_dpu *dpu)
{
	unsigned int ch;

	for (ch = 0; ch < DPP_NUM_CHANNELS; ch++) {
		void __iomem *base = dpu->dpp + ch * DPP_CH_STRIDE;
		u32 size = readl(base + DPP_IMG_SIZE);

		if ((size & 0x3fff) && ((size >> 16) & 0x3fff) &&
		    readl(base + DPP_BASEADDR_Y8)) {
			dev_info(dpu->drm.dev,
				 "adopting bootloader DPP ch%u: %ux%u @ %#x\n",
				 ch, size & 0x3fff, (size >> 16) & 0x3fff,
				 readl(base + DPP_BASEADDR_Y8));
			dpu->ch = ch;
			return 0;
		}
	}
	return -ENODEV;
}

static void gs201_dpu_clks_disable(void *data)
{
	struct gs201_dpu *dpu = data;

	clk_bulk_disable_unprepare(ARRAY_SIZE(dpu->clks), dpu->clks);
}

static int gs201_dpu_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct gs201_dpu *dpu;
	struct device_node *endpoint;
	struct drm_device *drm;
	int ret, i, irq;

	dpu = devm_drm_dev_alloc(dev, &gs201_dpu_driver, struct gs201_dpu, drm);
	if (IS_ERR(dpu))
		return PTR_ERR(dpu);
	drm = &dpu->drm;

	spin_lock_init(&dpu->lock);
	INIT_DELAYED_WORK(&dpu->timeout_work, gs201_dpu_timeout_work);

	for (i = 0; i < ARRAY_SIZE(dpu->clks); i++)
		dpu->clks[i].id = gs201_dpu_clk_names[i];
	ret = devm_clk_bulk_get(dev, ARRAY_SIZE(dpu->clks), dpu->clks);
	if (ret)
		return dev_err_probe(dev, ret, "failed to get clocks\n");

	ret = clk_bulk_prepare_enable(ARRAY_SIZE(dpu->clks), dpu->clks);
	if (ret)
		return dev_err_probe(dev, ret, "failed to enable clocks\n");

	ret = devm_add_action_or_reset(dev, gs201_dpu_clks_disable, dpu);
	if (ret)
		return ret;

	dpu->decon = devm_platform_ioremap_resource_byname(pdev, "decon");
	if (IS_ERR(dpu->decon))
		return PTR_ERR(dpu->decon);
	dpu->dpp = devm_platform_ioremap_resource_byname(pdev, "dpp");
	if (IS_ERR(dpu->dpp))
		return PTR_ERR(dpu->dpp);

	ret = gs201_dpu_find_channel(dpu);
	if (ret)
		return dev_err_probe(dev, ret,
				     "no active DPP channel left by bootloader\n");

	ret = dma_set_mask_and_coherent(dev, DMA_BIT_MASK(32));
	if (ret)
		return ret;

	ret = drmm_mode_config_init(drm);
	if (ret)
		return ret;

	drm->mode_config.min_width = GS201_DPU_WIDTH;
	drm->mode_config.max_width = GS201_DPU_WIDTH;
	drm->mode_config.min_height = GS201_DPU_HEIGHT;
	drm->mode_config.max_height = GS201_DPU_HEIGHT;
	drm->mode_config.funcs = &gs201_dpu_mode_config_funcs;

	endpoint = of_graph_get_endpoint_by_regs(dev->of_node, 0, -1);
	if (endpoint) {
		struct device_node *remote __free(device_node) =
			of_graph_get_remote_port_parent(endpoint);
		struct drm_bridge *bridge __free(drm_bridge_put) = NULL;

		of_node_put(endpoint);
		if (remote)
			bridge = of_drm_find_and_get_bridge(remote);
		if (!bridge)
			return -EPROBE_DEFER;

		ret = drm_simple_display_pipe_init(drm, &dpu->pipe,
						   &gs201_dpu_pipe_funcs,
						   gs201_dpu_formats,
						   ARRAY_SIZE(gs201_dpu_formats),
						   NULL, NULL);
		if (ret)
			return ret;

		ret = drm_simple_display_pipe_attach_bridge(&dpu->pipe, bridge);
		if (ret)
			return ret;
	} else {
		/* No DSI bridge described: fixed connector, panel untouched */
		drm_connector_helper_add(&dpu->connector,
					 &gs201_dpu_connector_helper_funcs);
		ret = drm_connector_init(drm, &dpu->connector,
					 &gs201_dpu_connector_funcs,
					 DRM_MODE_CONNECTOR_DSI);
		if (ret)
			return ret;

		ret = drm_simple_display_pipe_init(drm, &dpu->pipe,
						   &gs201_dpu_pipe_funcs,
						   gs201_dpu_formats,
						   ARRAY_SIZE(gs201_dpu_formats),
						   NULL, &dpu->connector);
		if (ret)
			return ret;
	}

	irq = platform_get_irq_byname(pdev, "frame_done");
	if (irq < 0)
		return irq;
	ret = devm_request_irq(dev, irq, gs201_dpu_irq, 0, dev_name(dev), dpu);
	if (ret)
		return dev_err_probe(dev, ret,
				     "failed to request frame_done irq\n");

	drm_plane_enable_fb_damage_clips(&dpu->pipe.plane);
	drm_mode_config_reset(drm);

	ret = drm_dev_register(drm, 0);
	if (ret)
		return ret;

	platform_set_drvdata(pdev, drm);
	drm_client_setup_with_fourcc(drm, DRM_FORMAT_XRGB8888);
	return 0;
}

static void gs201_dpu_remove(struct platform_device *pdev)
{
	struct drm_device *drm = platform_get_drvdata(pdev);

	drm_dev_unplug(drm);
	drm_atomic_helper_shutdown(drm);
}

static void gs201_dpu_shutdown(struct platform_device *pdev)
{
	drm_atomic_helper_shutdown(platform_get_drvdata(pdev));
}

static const struct of_device_id gs201_dpu_of_match[] = {
	{ .compatible = "google,gs201-dpu" },
	{}
};
MODULE_DEVICE_TABLE(of, gs201_dpu_of_match);

static struct platform_driver gs201_dpu_platform_driver = {
	.probe = gs201_dpu_probe,
	.remove = gs201_dpu_remove,
	.shutdown = gs201_dpu_shutdown,
	.driver = {
		.name = "gs201-dpu",
		.of_match_table = gs201_dpu_of_match,
	},
};
module_platform_driver(gs201_dpu_platform_driver);

MODULE_DESCRIPTION("Google Tensor GS201 display controller driver");
MODULE_LICENSE("GPL");
