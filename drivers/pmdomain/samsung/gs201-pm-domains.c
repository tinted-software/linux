// SPDX-License-Identifier: GPL-2.0-only
/*
 * Google Tensor GS201 PMU power domains
 *
 * Each domain is controlled by a CONFIGURATION register in the PMU (bit 0
 * powers the domain) with a STATUS register right behind it. The PMU is
 * reached through its syscon regmap. Some domains need extra steps taken
 * from the vendor sequences: a CMU controller option bit that must be
 * cleared before power-down and a TrustZone monitor call that saves and
 * restores the domain's TZPC state around a power cycle.
 *
 * Copyright 2026 Theo Paris <theo@theoparis.com>
 */

#include <linux/arm-smccc.h>
#include <linux/bits.h>
#include <linux/io.h>
#include <linux/mfd/syscon.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/pm_domain.h>
#include <linux/regmap.h>

#define GS201_PD_CONFIG_ON BIT(0)
#define GS201_PD_STATUS_OFFSET 0x4
#define GS201_PD_STATUS_ON BIT(0)
#define GS201_PD_TIMEOUT_US 10000

/* CMU_<domain> CONTROLLER_OPTION: bit 24 is cleared before power-down */
#define GS201_CMU_OPTION_PD_EN BIT(24)

#define GS201_SMC_PREPARE_PD_ONOFF 0x82000410
#define GS201_SMC_PD_DOWN 0
#define GS201_SMC_PD_UP 1
#define GS201_SMC_TZPC_GROUP 2

struct gs201_pd {
	struct generic_pm_domain pd;
	struct regmap *pmu;
	void __iomem *cmu_option;
	u32 config_offset;
	u32 tzpc_smc;
};

#define to_gs201_pd(d) container_of(d, struct gs201_pd, pd)

static int gs201_pd_tzpc(struct gs201_pd *pd, unsigned long what)
{
	struct arm_smccc_res res;

	if (!pd->tzpc_smc)
		return 0;

	arm_smccc_smc(GS201_SMC_PREPARE_PD_ONOFF, what, pd->tzpc_smc,
		      GS201_SMC_TZPC_GROUP, 0, 0, 0, 0, &res);

	return res.a0 ? -EIO : 0;
}

static int gs201_pd_set(struct gs201_pd *pd, bool on)
{
	unsigned int val;
	int ret;

	ret = regmap_update_bits(pd->pmu, pd->config_offset, GS201_PD_CONFIG_ON,
				 on ? GS201_PD_CONFIG_ON : 0);
	if (ret)
		return ret;

	ret = regmap_read_poll_timeout(pd->pmu,
				       pd->config_offset + GS201_PD_STATUS_OFFSET,
				       val, !!(val & GS201_PD_STATUS_ON) == on,
				       100, GS201_PD_TIMEOUT_US);
	if (ret)
		pr_err("%s: power %s timed out\n", pd->pd.name,
		       on ? "on" : "off");

	return ret;
}

static int gs201_pd_power_on(struct generic_pm_domain *domain)
{
	struct gs201_pd *pd = to_gs201_pd(domain);
	int ret;

	ret = gs201_pd_set(pd, true);
	if (ret)
		return ret;

	return gs201_pd_tzpc(pd, GS201_SMC_PD_UP);
}

static int gs201_pd_power_off(struct generic_pm_domain *domain)
{
	struct gs201_pd *pd = to_gs201_pd(domain);
	int ret;

	ret = gs201_pd_tzpc(pd, GS201_SMC_PD_DOWN);
	if (ret)
		return ret;

	if (pd->cmu_option)
		writel(readl(pd->cmu_option) & ~GS201_CMU_OPTION_PD_EN,
		       pd->cmu_option);

	return gs201_pd_set(pd, false);
}

static int gs201_pd_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct device_node *np = dev->of_node;
	struct of_phandle_args parent, child;
	struct gs201_pd *pd;
	unsigned int status;
	int ret;

	pd = devm_kzalloc(dev, sizeof(*pd), GFP_KERNEL);
	if (!pd)
		return -ENOMEM;

	pd->pmu = syscon_regmap_lookup_by_phandle(np, "google,pmu-syscon");
	if (IS_ERR(pd->pmu))
		return dev_err_probe(dev, PTR_ERR(pd->pmu), "no PMU syscon\n");

	ret = of_property_read_u32(np, "google,pmu-offset", &pd->config_offset);
	if (ret)
		return dev_err_probe(dev, ret, "missing google,pmu-offset\n");

	of_property_read_u32(np, "google,tzpc-smc", &pd->tzpc_smc);

	if (of_property_present(np, "reg")) {
		pd->cmu_option = devm_platform_ioremap_resource(pdev, 0);
		if (IS_ERR(pd->cmu_option))
			return PTR_ERR(pd->cmu_option);
	}

	pd->pd.name = devm_kstrdup_const(dev, np->name, GFP_KERNEL);
	if (!pd->pd.name)
		return -ENOMEM;
	pd->pd.power_on = gs201_pd_power_on;
	pd->pd.power_off = gs201_pd_power_off;

	ret = regmap_read(pd->pmu, pd->config_offset + GS201_PD_STATUS_OFFSET,
			  &status);
	if (ret)
		return ret;

	ret = pm_genpd_init(&pd->pd, NULL, !(status & GS201_PD_STATUS_ON));
	if (ret)
		return ret;

	ret = of_genpd_add_provider_simple(np, &pd->pd);
	if (ret) {
		pm_genpd_remove(&pd->pd);
		return ret;
	}

	if (!of_parse_phandle_with_args(np, "power-domains",
					"#power-domain-cells", 0, &parent)) {
		child.np = np;
		child.args_count = 0;

		ret = of_genpd_add_subdomain(&parent, &child);
		of_node_put(parent.np);
		if (ret)
			dev_warn(dev, "failed to add subdomain of %pOF: %d\n",
				 parent.np, ret);
	}

	dev_info(dev, "power domain %s is %s\n", pd->pd.name,
		 status & GS201_PD_STATUS_ON ? "on" : "off");

	return 0;
}

static const struct of_device_id gs201_pd_of_match[] = {
	{ .compatible = "google,gs201-power-domain" },
	{}
};

static struct platform_driver gs201_pd_driver = {
	.probe = gs201_pd_probe,
	.driver = {
		.name = "gs201-pd",
		.of_match_table = gs201_pd_of_match,
		.suppress_bind_attrs = true,
	},
};
builtin_platform_driver(gs201_pd_driver);
