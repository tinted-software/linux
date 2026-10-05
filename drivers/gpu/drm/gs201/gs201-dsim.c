// SPDX-License-Identifier: GPL-2.0-only
/*
 * Google Tensor GS201 MIPI DSI host (DSIM)
 *
 * First stage: the bootloader leaves the link up (PLL, D-PHY, HS clock, and
 * command-mode configuration), so this driver adopts it and only implements
 * the host side of the command interface: DCS/generic writes through the
 * packet header and payload FIFOs, and the bridge plumbing that connects
 * the DECON to the panel.
 */

#include <linux/bits.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/iopoll.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/platform_device.h>

#include <drm/drm_atomic_state_helper.h>
#include <drm/drm_bridge.h>
#include <drm/drm_mipi_dsi.h>
#include <drm/drm_of.h>
#include <drm/drm_panel.h>

#define DSIM_STATUS 0x00
#define DSIM_ESCMODE 0x2c
#define DSIM_ESCMODE_CMD_LPDT BIT(7)
#define DSIM_PKTHDR 0x58
#define DSIM_PAYLOAD 0x5c
#define DSIM_FIFOCTRL 0x68
#define DSIM_FIFOCTRL_EMPTY_PL BIT(8)
#define DSIM_FIFOCTRL_EMPTY_PH BIT(10)

#define DSIM_FIFO_TIMEOUT_US 50000

struct gs201_dsim {
	struct device *dev;
	void __iomem *regs;
	struct mipi_dsi_host host;
	struct drm_bridge bridge;
	struct drm_bridge *panel_bridge;
	struct mutex cmd_lock; /* serialises command FIFO transfers */
	bool bridge_added;
};

#define host_to_dsim(h) container_of(h, struct gs201_dsim, host)
#define bridge_to_dsim(b) container_of(b, struct gs201_dsim, bridge)

static int gs201_dsim_fifo_wait_empty(struct gs201_dsim *dsim)
{
	const u32 empty = DSIM_FIFOCTRL_EMPTY_PH | DSIM_FIFOCTRL_EMPTY_PL;
	u32 val;

	return readl_poll_timeout(dsim->regs + DSIM_FIFOCTRL, val,
				  (val & empty) == empty, 10,
				  DSIM_FIFO_TIMEOUT_US);
}

static ssize_t gs201_dsim_host_transfer(struct mipi_dsi_host *host,
					const struct mipi_dsi_msg *msg)
{
	struct gs201_dsim *dsim = host_to_dsim(host);
	struct mipi_dsi_packet packet;
	u32 esc;
	int ret;
	size_t i;

	if (msg->rx_len)
		return -EOPNOTSUPP; /* TODO: RX FIFO */

	ret = mipi_dsi_create_packet(&packet, msg);
	if (ret)
		return ret;

	mutex_lock(&dsim->cmd_lock);

	ret = gs201_dsim_fifo_wait_empty(dsim);
	if (ret)
		goto out;

	esc = readl(dsim->regs + DSIM_ESCMODE);
	if (msg->flags & MIPI_DSI_MSG_USE_LPM)
		esc |= DSIM_ESCMODE_CMD_LPDT;
	else
		esc &= ~DSIM_ESCMODE_CMD_LPDT;
	writel(esc, dsim->regs + DSIM_ESCMODE);

	for (i = 0; i < packet.payload_length; i += 4) {
		u32 word = 0;
		size_t n = min_t(size_t, 4, packet.payload_length - i);

		memcpy(&word, packet.payload + i, n);
		writel(le32_to_cpu((__force __le32)word),
		       dsim->regs + DSIM_PAYLOAD);
	}

	writel(packet.header[0] | packet.header[1] << 8 |
		       packet.header[2] << 16,
	       dsim->regs + DSIM_PKTHDR);

	ret = gs201_dsim_fifo_wait_empty(dsim);
	if (ret)
		dev_err(dsim->dev, "command FIFO did not drain (type %#x)\n",
			msg->type);
out:
	mutex_unlock(&dsim->cmd_lock);
	return ret ? ret : packet.size;
}

static int gs201_dsim_host_attach(struct mipi_dsi_host *host,
				  struct mipi_dsi_device *device)
{
	struct gs201_dsim *dsim = host_to_dsim(host);
	struct drm_panel *panel;

	if (device->lanes != 4)
		dev_warn(dsim->dev, "device wants %u lanes, link uses 4\n",
			 device->lanes);

	panel = of_drm_find_panel(device->dev.of_node);
	if (IS_ERR(panel))
		return PTR_ERR(panel);

	dsim->panel_bridge = devm_drm_panel_bridge_add_typed(dsim->dev, panel,
							     DRM_MODE_CONNECTOR_DSI);
	if (IS_ERR(dsim->panel_bridge))
		return PTR_ERR(dsim->panel_bridge);

	/* The DECON can only find us once a panel is there. */
	drm_bridge_add(&dsim->bridge);
	dsim->bridge_added = true;

	return 0;
}

static int gs201_dsim_host_detach(struct mipi_dsi_host *host,
				  struct mipi_dsi_device *device)
{
	struct gs201_dsim *dsim = host_to_dsim(host);

	if (dsim->bridge_added) {
		drm_bridge_remove(&dsim->bridge);
		dsim->bridge_added = false;
	}
	return 0;
}

static const struct mipi_dsi_host_ops gs201_dsim_host_ops = {
	.attach = gs201_dsim_host_attach,
	.detach = gs201_dsim_host_detach,
	.transfer = gs201_dsim_host_transfer,
};

static int gs201_dsim_bridge_attach(struct drm_bridge *bridge,
				    struct drm_encoder *encoder,
				    enum drm_bridge_attach_flags flags)
{
	struct gs201_dsim *dsim = bridge_to_dsim(bridge);

	return drm_bridge_attach(encoder, dsim->panel_bridge, bridge, flags);
}

static const struct drm_bridge_funcs gs201_dsim_bridge_funcs = {
	.attach = gs201_dsim_bridge_attach,
	.atomic_create_state = drm_atomic_helper_bridge_create_state,
	.atomic_duplicate_state = drm_atomic_helper_bridge_duplicate_state,
	.atomic_destroy_state = drm_atomic_helper_bridge_destroy_state,
};

static int gs201_dsim_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct gs201_dsim *dsim;
	struct clk *bus;

	dsim = devm_drm_bridge_alloc(dev, struct gs201_dsim, bridge,
				     &gs201_dsim_bridge_funcs);
	if (IS_ERR(dsim))
		return PTR_ERR(dsim);
	dsim->dev = dev;
	mutex_init(&dsim->cmd_lock);

	bus = devm_clk_get_enabled(dev, "bus");
	if (IS_ERR(bus))
		return dev_err_probe(dev, PTR_ERR(bus),
				     "failed to enable bus clock\n");

	dsim->regs = devm_platform_ioremap_resource(pdev, 0);
	if (IS_ERR(dsim->regs))
		return PTR_ERR(dsim->regs);

	dsim->bridge.of_node = dev->of_node;
	dsim->bridge.type = DRM_MODE_CONNECTOR_DSI;

	platform_set_drvdata(pdev, dsim);

	dsim->host.dev = dev;
	dsim->host.ops = &gs201_dsim_host_ops;
	return mipi_dsi_host_register(&dsim->host);
}

static void gs201_dsim_remove(struct platform_device *pdev)
{
	struct gs201_dsim *dsim = platform_get_drvdata(pdev);

	mipi_dsi_host_unregister(&dsim->host);
}

static const struct of_device_id gs201_dsim_of_match[] = {
	{ .compatible = "google,gs201-dsim" },
	{}
};
MODULE_DEVICE_TABLE(of, gs201_dsim_of_match);

static struct platform_driver gs201_dsim_driver = {
	.probe = gs201_dsim_probe,
	.remove = gs201_dsim_remove,
	.driver = {
		.name = "gs201-dsim",
		.of_match_table = gs201_dsim_of_match,
	},
};
module_platform_driver(gs201_dsim_driver);

MODULE_DESCRIPTION("Google Tensor GS201 MIPI DSI host");
MODULE_LICENSE("GPL");
