// SPDX-License-Identifier: GPL-2.0-only
/*
 * gs201 (Pixel 7a) bring-up helpers.  NOT FOR UPSTREAM.
 *
 * 1. A boot console that appends to the ramoops console zone, so the very
 *    first printk()s are readable (via /sys/fs/pstore on Android) after a
 *    reset, with no devicetree, clock or driver needed.
 * 2. Replaces the bootloader supplied devicetree with the built-in
 *    gs201-lynx one.  ABL hands the kernel the *Android* DTB (merged with
 *    dtbo) and there is no way to select another one with `fastboot boot`.
 *    The bootloader's /memory, /chosen and /reserved-memory are carried
 *    over, since they describe the real DRAM layout and the secure/firmware
 *    carve-outs that ABL patches in at runtime.
 */
#include <linux/console.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/libfdt.h>
#include <linux/sizes.h>
#include <linux/spinlock.h>
#include <linux/string.h>
#include <linux/arm-smccc.h>
#include <uapi/linux/psci.h>
#include <asm/fixmap.h>
#include <asm/memory.h>
#include <asm/sections.h>
#include <linux/of.h>
#include <linux/panic_notifier.h>
#include <linux/reboot.h>
#include <linux/regmap.h>
#include <linux/soc/samsung/exynos-pmu.h>

#include "gs201_bringup.h"

/* ---- ramoops console zone (see fs/pstore/ram_core.c) ---- */
#define RC_BASE 0xfd3ff000UL /* ramoops_mem in ABL's DT */
#define RC_SIZE 0x200000UL /* console-size */
#define RC_SIG 0x43474244 /* PERSISTENT_RAM_SIG ^ 0 */
#define RC_HDR 12 /* sig, start, size */
#define RC_DATA (RC_SIZE - RC_HDR)

static DEFINE_RAW_SPINLOCK(rc_lock);

static u8 __iomem *rc_map(unsigned long off)
{
	phys_addr_t pa = RC_BASE + off;

	set_fixmap_io(FIX_GS201_RAMCON, pa & PAGE_MASK);
	return (u8 __iomem *)__fix_to_virt(FIX_GS201_RAMCON) +
	       (pa & ~PAGE_MASK);
}

static void rc_write(struct console *con, const char *s, unsigned int n)
{
	unsigned long flags;
	u32 start, size;

	raw_spin_lock_irqsave(&rc_lock, flags);

	if (readl(rc_map(0)) != RC_SIG) {
		writel(RC_SIG, rc_map(0));
		writel(0, rc_map(4));
		writel(0, rc_map(8));
	}
	start = readl(rc_map(4));
	size = readl(rc_map(8));
	if (size > RC_DATA || start >= RC_DATA)
		start = 0;
		size = 0;

	while (n--) {
		writeb(*s++, rc_map(RC_HDR + start));
		if (++start >= RC_DATA)
			start = 0;
		if (size < RC_DATA)
			size++;
	}

	writel(start, rc_map(4));
	writel(size, rc_map(8));

	raw_spin_unlock_irqrestore(&rc_lock, flags);
}

static struct console rc_con = {
	.name = "gs201rc",
	.write = rc_write,
	.flags = CON_PRINTBUFFER | CON_BOOT | CON_ENABLED,
	.index = -1,
};

/*
 * ABL decides how to treat the previous boot (and whether to keep the ramoops
 * log) from a signature in the debug-snapshot header (header@d8100000, offset
 * DSS_OFFSET_EMERGENCY_REASON). Without one the reset is reported as
 * "APC Watchdog Early". 0xCAFE = DSS_SIGN_NORMAL_REBOOT, 0xBABA = PANIC.
 */
#define DSS_HEADER_PHYS 0xd8100000UL
#define DSS_OFFSET_EMERGENCY_REASON 0x300
#define DSS_SIGN_PANIC 0xBABA
#define DSS_SIGN_NORMAL_REBOOT 0xCAFE

void gs201_report_reset_reason(u32 val)
{
	phys_addr_t pa = DSS_HEADER_PHYS + DSS_OFFSET_EMERGENCY_REASON;

	set_fixmap_io(FIX_GS201_RAMCON, pa & PAGE_MASK);
	writel(val, (void __iomem *)__fix_to_virt(FIX_GS201_RAMCON) +
			    (pa & ~PAGE_MASK));
}

/*
 * On panic, ask ABL for fastboot (PMU SYSIP_DAT0 = 0xfc, as reboot-mode
 * "bootloader" does) with a normal-reboot signature and a warm reset, so the
 * ramoops log of the failed boot survives and no Android boot overwrites it.
 */
#define PMU_SYSIP_DAT0 0x0810
#define SYSIP_MODE_BOOTLOADER 0xfc

static struct regmap *gs201_pmu;

static int gs201_panic_to_fastboot(struct notifier_block *nb,
				   unsigned long event, void *unused)
{
	/* PMU regmap: raw spinlock + SMC writes, safe in panic context */
	if (gs201_pmu)
		regmap_write(gs201_pmu, PMU_SYSIP_DAT0, SYSIP_MODE_BOOTLOADER);
	gs201_report_reset_reason(DSS_SIGN_PANIC);
	reboot_mode = REBOOT_WARM;
	return NOTIFY_DONE;
}

static struct notifier_block gs201_panic_nb = {
	.notifier_call = gs201_panic_to_fastboot,
	.priority = INT_MAX,
};

static int __init gs201_panic_hook_init(void)
{
	struct regmap *pmu;

	if (!of_machine_is_compatible("google,gs201"))
		return 0;
	pmu = exynos_get_pmu_regmap();
	if (IS_ERR(pmu))
		pr_warn("gs201: no PMU regmap (%pe), panic will not enter fastboot\n",
			pmu);
	else
		gs201_pmu = pmu;
	atomic_notifier_chain_register(&panic_notifier_list, &gs201_panic_nb);
	return 0;
}
late_initcall_sync(gs201_panic_hook_init);

/* ---- builtin devicetree with bootloader fixups ---- */

static u8 dtbuf[SZ_128K] __aligned(8);

static int __init copy_node(void *dst, int dparent, const void *src, int soff)
{
	const char *name = fdt_get_name(src, soff, NULL);
	int doff, p, c, ret;

	doff = fdt_add_subnode(dst, dparent, name);
	if (doff < 0)
		return doff;

	fdt_for_each_property_offset(p, src, soff) {
		const struct fdt_property *prop;
		const char *pname;

		prop = fdt_get_property_by_offset(src, p, NULL);
		pname = fdt_string(src, fdt32_to_cpu(prop->nameoff));
		/* phandles would collide with those of the builtin DT */
		if (!strcmp(pname, "phandle") ||
		    !strcmp(pname, "linux,phandle"))
			continue;
		ret = fdt_setprop(dst, doff, pname, prop->data,
				  fdt32_to_cpu(prop->len));
		if (ret)
			return ret;
	}

	fdt_for_each_subnode(c, src, soff) {
		ret = copy_node(dst, doff, src, c);
		if (ret)
			return ret;
	}
	return 0;
}

static phys_addr_t __init gs201_fixup_dtb(phys_addr_t abl_phys)
{
	void *dst = dtbuf;
	void *src;
	int size = 0, n = 0, c, off, p, ret;

	src = fixmap_remap_fdt(abl_phys, &size, PAGE_KERNEL);
	if (!src || fdt_check_header(src)) {
		pr_crit("gs201: bad bootloader FDT %pa\n", &abl_phys);
		return abl_phys;
	}
	pr_info("gs201: bootloader FDT at %pa, %d bytes\n", &abl_phys, size);

	ret = fdt_open_into(gs201_bringup_dtb, dst, sizeof(dtbuf));
	if (ret) {
		pr_crit("gs201: builtin FDT unusable (%d)\n", ret);
		return abl_phys;
	}

	/* /memory* nodes */
	fdt_for_each_subnode(c, src, 0) {
		const char *dt = fdt_getprop(src, c, "device_type", NULL);

		if (dt && !strcmp(dt, "memory")) {
			ret = copy_node(dst, 0, src, c);
			pr_info("gs201: copied %s: %d\n",
				fdt_get_name(src, c, NULL), ret);
			n++;
		}
	}
	if (!n)
		pr_warn("gs201: bootloader FDT has no /memory node!\n");

	/* /reserved-memory: take ABL's version wholesale */
	off = fdt_path_offset(dst, "/reserved-memory");
	if (off >= 0)
		fdt_del_node(dst, off);
	off = fdt_path_offset(src, "/reserved-memory");
	if (off >= 0) {
		ret = copy_node(dst, 0, src, off);
		pr_info("gs201: copied /reserved-memory: %d\n", ret);
	} else {
		pr_warn("gs201: bootloader FDT has no /reserved-memory\n");
	}

	/* /chosen: bootargs, initrd, rng seeds... (but keep our stdout-path) */
	off = fdt_path_offset(src, "/chosen");
	if (off >= 0) {
		int doff = fdt_path_offset(dst, "/chosen");

		if (doff < 0)
			doff = fdt_add_subnode(dst, 0, "chosen");
		fdt_for_each_property_offset(p, src, off) {
			const struct fdt_property *prop;
			const char *pname;

			prop = fdt_get_property_by_offset(src, p, NULL);
			pname = fdt_string(src, fdt32_to_cpu(prop->nameoff));
			if (!strcmp(pname, "stdout-path") ||
			    !strcmp(pname, "phandle"))
				continue;
			fdt_setprop(dst, doff, pname, prop->data,
				    fdt32_to_cpu(prop->len));
		}
		pr_info("gs201: bootargs: %s\n",
			(const char *)fdt_getprop(dst, doff, "bootargs", NULL));
	}

	fdt_pack(dst);
	/*
	 * The FDT fixmap window can't be re-pointed at another physical block,
	 * so write the merged tree over the bootloader's (much larger) one and
	 * keep the same address.
	 */
	memcpy(src, dst, fdt_totalsize(dst));
	pr_info("gs201: using builtin DT (%u bytes) in place at %pa\n",
		fdt_totalsize(dst), &abl_phys);
	return abl_phys;
}

/* ABL leaves both cluster watchdogs armed; WTCON (offset 0) = 0 disarms. */
static void __maybe_unused __init gs201_disarm_wdt(void)
{
	static const phys_addr_t wdt[] = { 0x10060000, 0x10070000 };
	int i;

	for (i = 0; i < ARRAY_SIZE(wdt); i++) {
		void __iomem *va;

		rc_write(NULL, "[gs201] wdt: map\n", 17);
		set_fixmap_io(FIX_GS201_RAMCON, wdt[i]);
		va = (void __iomem *)__fix_to_virt(FIX_GS201_RAMCON);
		/* write only: a read of WTCON here appears to hang the bus */
		writel(0, va);
		rc_write(NULL, "[gs201] wdt: write ok\n", 22);
	}
}

phys_addr_t __init gs201_bringup_early(phys_addr_t abl_fdt)
{
	rc_write(NULL, "[gs201] early entry\n", 20);
#ifdef CONFIG_GS201_BRINGUP_DISARM_WDT
	gs201_disarm_wdt();
#endif
	rc_write(NULL, "[gs201] wdt done\n", 17);
#ifdef CONFIG_GS201_BRINGUP_TEST_RESET
	rc_write(NULL,
		 "[gs201] test: hanging on purpose (long-press to reset)\n",
		 55);
	for (;;)
		cpu_relax();
#endif
	register_console(&rc_con);
	pr_info("gs201: bring-up ramcon online\n");

	return gs201_fixup_dtb(abl_fdt);
}
