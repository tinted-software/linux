/* SPDX-License-Identifier: GPL-2.0-only */
	.section .rodata, "a"
	.balign 8
	.globl gs201_bringup_dtb
gs201_bringup_dtb:
	.incbin "gs201-lynx.dtb"
	.globl gs201_bringup_dtb_end
gs201_bringup_dtb_end:
