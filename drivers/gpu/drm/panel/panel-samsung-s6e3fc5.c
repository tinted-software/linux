// SPDX-License-Identifier: GPL-2.0-only
/*
 * Samsung S6E3FC5 AMOLED DSI command-mode panel (Google Pixel 7a)
 *
 * First stage: the bootloader already powered, reset and initialised the
 * panel (sleep-out, TE, DSC, gamma), and the display is showing its splash
 * screen. This driver adopts that state and only manages what is safe
 * without a full init sequence: brightness, display on/off.
 */

#include <linux/backlight.h>
#include <linux/module.h>
#include <linux/of.h>

#include <video/mipi_display.h>

#include <drm/drm_connector.h>
#include <drm/drm_mipi_dsi.h>
#include <drm/drm_probe_helper.h>
#include <drm/drm_modes.h>
#include <drm/drm_panel.h>

#define S6E3FC5_MAX_BRIGHTNESS 4095 /* 0-2047 normal, above is HBM */
#define S6E3FC5_DFT_BRIGHTNESS 1023

#define S6E3FC5_WRCTRLD_BCTRL 0x20
#define S6E3FC5_WRCTRLD_DD 0x08

struct s6e3fc5 {
	struct drm_panel panel;
	struct mipi_dsi_device *dsi;
	bool pixel_off;
};

static const struct drm_display_mode s6e3fc5_mode = {
	.clock = (1080 + 32 + 16 + 32) * (2400 + 12 + 4 + 12) * 60 / 1000,
	.hdisplay = 1080,
	.hsync_start = 1080 + 32,
	.hsync_end = 1080 + 32 + 16,
	.htotal = 1080 + 32 + 16 + 32,
	.vdisplay = 2400,
	.vsync_start = 2400 + 12,
	.vsync_end = 2400 + 12 + 4,
	.vtotal = 2400 + 12 + 4 + 12,
	.width_mm = 65,
	.height_mm = 144,
	.type = DRM_MODE_TYPE_DRIVER | DRM_MODE_TYPE_PREFERRED,
};

static inline struct s6e3fc5 *to_s6e3fc5(struct drm_panel *panel)
{
	return container_of(panel, struct s6e3fc5, panel);
}

static int s6e3fc5_prepare(struct drm_panel *panel)
{
	/* TODO: supplies, reset, and the full init sequence. */
	return 0;
}

static int s6e3fc5_enable(struct drm_panel *panel)
{
	struct mipi_dsi_device *dsi = to_s6e3fc5(panel)->dsi;
	struct mipi_dsi_multi_context ctx = { .dsi = dsi };

	/* brightness control + dimming */
	mipi_dsi_dcs_write_seq_multi(&ctx, MIPI_DCS_WRITE_CONTROL_DISPLAY,
				     S6E3FC5_WRCTRLD_BCTRL |
					     S6E3FC5_WRCTRLD_DD);
	mipi_dsi_dcs_set_display_on_multi(&ctx);
	return ctx.accum_err;
}

static int s6e3fc5_disable(struct drm_panel *panel)
{
	struct mipi_dsi_multi_context ctx = { .dsi = to_s6e3fc5(panel)->dsi };

	mipi_dsi_dcs_set_display_off_multi(&ctx);
	return ctx.accum_err;
}

static int s6e3fc5_get_modes(struct drm_panel *panel,
			     struct drm_connector *connector)
{
	return drm_connector_helper_get_modes_fixed(connector, &s6e3fc5_mode);
}

static const struct drm_panel_funcs s6e3fc5_panel_funcs = {
	.prepare = s6e3fc5_prepare,
	.enable = s6e3fc5_enable,
	.disable = s6e3fc5_disable,
	.get_modes = s6e3fc5_get_modes,
};

static int s6e3fc5_bl_update_status(struct backlight_device *bl)
{
	struct s6e3fc5 *ctx = bl_get_data(bl);
	struct mipi_dsi_device *dsi = ctx->dsi;
	u16 brightness = backlight_get_brightness(bl);
	struct mipi_dsi_multi_context mctx = { .dsi = dsi };
	static const u8 pixel_off[] = { 0x22 };
	static const u8 normal_on[] = { 0x13 };
	int ret;

	/* Like the vendor driver, use pixel-off rather than DBV 0 */
	if (!brightness) {
		if (!ctx->pixel_off)
			mipi_dsi_dcs_write_buffer_multi(&mctx, pixel_off,
							sizeof(pixel_off));
		ctx->pixel_off = true;
		return mctx.accum_err;
	}

	if (ctx->pixel_off) {
		mipi_dsi_dcs_write_buffer_multi(&mctx, normal_on,
						sizeof(normal_on));
		ctx->pixel_off = false;
		if (mctx.accum_err)
			return mctx.accum_err;
	}

	ret = mipi_dsi_dcs_set_display_brightness_large(dsi, brightness);
	return ret < 0 ? ret : 0;
}

static const struct backlight_ops s6e3fc5_bl_ops = {
	.update_status = s6e3fc5_bl_update_status,
};

static int s6e3fc5_probe(struct mipi_dsi_device *dsi)
{
	struct device *dev = &dsi->dev;
	struct backlight_properties props = {
		.type = BACKLIGHT_RAW,
		.brightness = S6E3FC5_DFT_BRIGHTNESS,
		.max_brightness = S6E3FC5_MAX_BRIGHTNESS,
	};
	struct s6e3fc5 *ctx;
	int ret;

	ctx = devm_drm_panel_alloc(dev, struct s6e3fc5, panel,
				   &s6e3fc5_panel_funcs,
				   DRM_MODE_CONNECTOR_DSI);
	if (IS_ERR(ctx))
		return PTR_ERR(ctx);

	ctx->dsi = dsi;
	mipi_dsi_set_drvdata(dsi, ctx);

	dsi->lanes = 4;
	dsi->format = MIPI_DSI_FMT_RGB888;
	dsi->mode_flags = MIPI_DSI_CLOCK_NON_CONTINUOUS;

	ctx->panel.backlight = devm_backlight_device_register(dev, "s6e3fc5",
							      dev, ctx,
							      &s6e3fc5_bl_ops,
							      &props);
	if (IS_ERR(ctx->panel.backlight))
		return dev_err_probe(dev, PTR_ERR(ctx->panel.backlight),
				     "failed to register backlight\n");

	drm_panel_add(&ctx->panel);

	ret = mipi_dsi_attach(dsi);
	if (ret < 0) {
		drm_panel_remove(&ctx->panel);
		return dev_err_probe(dev, ret,
				     "failed to attach to DSI host\n");
	}

	return 0;
}

static void s6e3fc5_remove(struct mipi_dsi_device *dsi)
{
	struct s6e3fc5 *ctx = mipi_dsi_get_drvdata(dsi);

	mipi_dsi_detach(dsi);
	drm_panel_remove(&ctx->panel);
}

static const struct of_device_id s6e3fc5_of_match[] = {
	{ .compatible = "samsung,s6e3fc5" },
	{}
};
MODULE_DEVICE_TABLE(of, s6e3fc5_of_match);

static struct mipi_dsi_driver s6e3fc5_driver = {
	.probe = s6e3fc5_probe,
	.remove = s6e3fc5_remove,
	.driver = {
		.name = "panel-samsung-s6e3fc5",
		.of_match_table = s6e3fc5_of_match,
	},
};
module_mipi_dsi_driver(s6e3fc5_driver);

MODULE_DESCRIPTION("Samsung S6E3FC5 DSI panel driver");
MODULE_LICENSE("GPL");
