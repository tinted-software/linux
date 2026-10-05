/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __ARM64_KERNEL_GS201_BRINGUP_H
#define __ARM64_KERNEL_GS201_BRINGUP_H

#include <linux/init.h>
#include <linux/types.h>

extern const u8 gs201_bringup_dtb[];
extern const u8 gs201_bringup_dtb_end[];

phys_addr_t __init gs201_bringup_early(phys_addr_t abl_fdt);

#endif
