// SPDX-License-Identifier: GPL-2.0-only
/*
 * Common Clock Framework support for Google Tensor GS201.
 *
 * Register and topology data derived from Samsung's GPL-2.0-only GS201
 * cmucal tables (Copyright (c) 2021 Samsung Electronics Co., Ltd.).
 * The tables were generated from that data.
 *
 * PLL rate tables are not provided: the PLLs keep the rates programmed by
 * the bootloader.
 */

#include <linux/clk-provider.h>
#include <linux/of.h>
#include <linux/platform_device.h>

#include <dt-bindings/clock/google,gs201.h>

#include "clk.h"
#include "clk-exynos-arm64.h"
#include "clk-pll.h"

/* ---- CMU_TOP ------------------------------------------------------------- */
/* Register offsets at 0x1e080000. */
#define TOP_PLL_LOCKTIME_PLL_LF_MIF 0x0000
#define TOP_PLL_LOCKTIME_PLL_SHARED0 0x0004
#define TOP_PLL_LOCKTIME_PLL_SHARED1 0x0008
#define TOP_PLL_LOCKTIME_PLL_SHARED2 0x000c
#define TOP_PLL_LOCKTIME_PLL_SHARED3 0x0010
#define TOP_PLL_LOCKTIME_PLL_SPARE 0x0014
#define TOP_PLL_CON0_PLL_LF_MIF 0x0100
#define TOP_PLL_CON1_PLL_LF_MIF 0x0104
#define TOP_PLL_CON2_PLL_LF_MIF 0x0108
#define TOP_PLL_CON3_PLL_LF_MIF 0x010c
#define TOP_PLL_CON4_PLL_LF_MIF 0x0110
#define TOP_PLL_CON0_PLL_SHARED0 0x0140
#define TOP_PLL_CON1_PLL_SHARED0 0x0144
#define TOP_PLL_CON2_PLL_SHARED0 0x0148
#define TOP_PLL_CON3_PLL_SHARED0 0x014c
#define TOP_PLL_CON4_PLL_SHARED0 0x0150
#define TOP_PLL_CON0_PLL_SHARED1 0x0180
#define TOP_PLL_CON1_PLL_SHARED1 0x0184
#define TOP_PLL_CON2_PLL_SHARED1 0x0188
#define TOP_PLL_CON3_PLL_SHARED1 0x018c
#define TOP_PLL_CON4_PLL_SHARED1 0x0190
#define TOP_PLL_CON0_PLL_SHARED2 0x01c0
#define TOP_PLL_CON1_PLL_SHARED2 0x01c4
#define TOP_PLL_CON2_PLL_SHARED2 0x01c8
#define TOP_PLL_CON3_PLL_SHARED2 0x01cc
#define TOP_PLL_CON4_PLL_SHARED2 0x01d0
#define TOP_PLL_CON0_PLL_SHARED3 0x0200
#define TOP_PLL_CON1_PLL_SHARED3 0x0204
#define TOP_PLL_CON2_PLL_SHARED3 0x0208
#define TOP_PLL_CON3_PLL_SHARED3 0x020c
#define TOP_PLL_CON4_PLL_SHARED3 0x0210
#define TOP_PLL_CON0_PLL_SPARE 0x0240
#define TOP_PLL_CON1_PLL_SPARE 0x0244
#define TOP_PLL_CON2_PLL_SPARE 0x0248
#define TOP_PLL_CON3_PLL_SPARE 0x024c
#define TOP_PLL_CON4_PLL_SPARE 0x0250
#define TOP_CLK_CON_MUX_MUX_CLKCMU_AUR_AUR 0x1000
#define TOP_CLK_CON_MUX_MUX_CLKCMU_AUR_AURCTL 0x1004
#define TOP_CLK_CON_MUX_MUX_CLKCMU_AUR_NOC 0x1008
#define TOP_CLK_CON_MUX_MUX_CLKCMU_BO_NOC 0x100c
#define TOP_CLK_CON_MUX_MUX_CLKCMU_CIS_CLK0 0x1010
#define TOP_CLK_CON_MUX_MUX_CLKCMU_CIS_CLK1 0x1014
#define TOP_CLK_CON_MUX_MUX_CLKCMU_CIS_CLK2 0x1018
#define TOP_CLK_CON_MUX_MUX_CLKCMU_CIS_CLK3 0x101c
#define TOP_CLK_CON_MUX_MUX_CLKCMU_CIS_CLK4 0x1020
#define TOP_CLK_CON_MUX_MUX_CLKCMU_CIS_CLK5 0x1024
#define TOP_CLK_CON_MUX_MUX_CLKCMU_CIS_CLK6 0x1028
#define TOP_CLK_CON_MUX_MUX_CLKCMU_CIS_CLK7 0x102c
#define TOP_CLK_CON_MUX_MUX_CLKCMU_CMU_BOOST 0x1030
#define TOP_CLK_CON_MUX_MUX_CLKCMU_CMU_BOOST_OPTION1 0x1034
#define TOP_CLK_CON_MUX_MUX_CLKCMU_CPUCL0_DBG 0x1038
#define TOP_CLK_CON_MUX_MUX_CLKCMU_CPUCL0_SWITCH 0x103c
#define TOP_CLK_CON_MUX_MUX_CLKCMU_CPUCL1_SWITCH 0x1040
#define TOP_CLK_CON_MUX_MUX_CLKCMU_CPUCL2_SWITCH 0x1044
#define TOP_CLK_CON_MUX_MUX_CLKCMU_CSIS_NOC 0x1048
#define TOP_CLK_CON_MUX_MUX_CLKCMU_DISP_NOC 0x104c
#define TOP_CLK_CON_MUX_MUX_CLKCMU_DNS_NOC 0x1050
#define TOP_CLK_CON_MUX_MUX_CLKCMU_DPU_NOC 0x1054
#define TOP_CLK_CON_MUX_MUX_CLKCMU_EH_NOC 0x1058
#define TOP_CLK_CON_MUX_MUX_CLKCMU_G2D_G2D 0x105c
#define TOP_CLK_CON_MUX_MUX_CLKCMU_G2D_MSCL 0x1060
#define TOP_CLK_CON_MUX_MUX_CLKCMU_G3AA_G3AA 0x1064
#define TOP_CLK_CON_MUX_MUX_CLKCMU_G3D_GLB 0x1068
#define TOP_CLK_CON_MUX_MUX_CLKCMU_G3D_NOCD 0x106c
#define TOP_CLK_CON_MUX_MUX_CLKCMU_G3D_SWITCH 0x1070
#define TOP_CLK_CON_MUX_MUX_CLKCMU_GDC_GDC0 0x1074
#define TOP_CLK_CON_MUX_MUX_CLKCMU_GDC_GDC1 0x1078
#define TOP_CLK_CON_MUX_MUX_CLKCMU_GDC_SCSC 0x107c
#define TOP_CLK_CON_MUX_MUX_CLKCMU_HPM 0x1080
#define TOP_CLK_CON_MUX_MUX_CLKCMU_HSI0_DPGTC 0x1084
#define TOP_CLK_CON_MUX_MUX_CLKCMU_HSI0_NOC 0x1088
#define TOP_CLK_CON_MUX_MUX_CLKCMU_HSI0_USB31DRD 0x108c
#define TOP_CLK_CON_MUX_MUX_CLKCMU_HSI0_USBDPDBG 0x1090
#define TOP_CLK_CON_MUX_MUX_CLKCMU_HSI1_NOC 0x1094
#define TOP_CLK_CON_MUX_MUX_CLKCMU_HSI1_PCIE 0x1098
#define TOP_CLK_CON_MUX_MUX_CLKCMU_HSI2_MMC_CARD 0x109c
#define TOP_CLK_CON_MUX_MUX_CLKCMU_HSI2_NOC 0x10a0
#define TOP_CLK_CON_MUX_MUX_CLKCMU_HSI2_PCIE 0x10a4
#define TOP_CLK_CON_MUX_MUX_CLKCMU_HSI2_UFS_EMBD 0x10a8
#define TOP_CLK_CON_MUX_MUX_CLKCMU_IPP_NOC 0x10ac
#define TOP_CLK_CON_MUX_MUX_CLKCMU_ITP_NOC 0x10b0
#define TOP_CLK_CON_MUX_MUX_CLKCMU_MCSC_ITSC 0x10b4
#define TOP_CLK_CON_MUX_MUX_CLKCMU_MCSC_MCSC 0x10b8
#define TOP_CLK_CON_MUX_MUX_CLKCMU_MFC_MFC 0x10bc
#define TOP_CLK_CON_MUX_MUX_CLKCMU_MIF_NOCP 0x10c0
#define TOP_CLK_CON_MUX_MUX_CLKCMU_MIF_SWITCH 0x10c4
#define TOP_CLK_CON_MUX_MUX_CLKCMU_MISC_NOC 0x10c8
#define TOP_CLK_CON_MUX_MUX_CLKCMU_MISC_SSS 0x10cc
#define TOP_CLK_CON_MUX_MUX_CLKCMU_NOCL0_NOC 0x10d0
#define TOP_CLK_CON_MUX_MUX_CLKCMU_NOCL1A_NOC 0x10d4
#define TOP_CLK_CON_MUX_MUX_CLKCMU_NOCL1B_NOC 0x10d8
#define TOP_CLK_CON_MUX_MUX_CLKCMU_NOCL2A_NOC 0x10dc
#define TOP_CLK_CON_MUX_MUX_CLKCMU_PDP_NOC 0x10e0
#define TOP_CLK_CON_MUX_MUX_CLKCMU_PDP_VRA 0x10e4
#define TOP_CLK_CON_MUX_MUX_CLKCMU_PERIC0_IP 0x10e8
#define TOP_CLK_CON_MUX_MUX_CLKCMU_PERIC0_NOC 0x10ec
#define TOP_CLK_CON_MUX_MUX_CLKCMU_PERIC1_IP 0x10f0
#define TOP_CLK_CON_MUX_MUX_CLKCMU_PERIC1_NOC 0x10f4
#define TOP_CLK_CON_MUX_MUX_CLKCMU_TNR_NOC 0x10f8
#define TOP_CLK_CON_MUX_MUX_CLKCMU_TOP_BOOST_OPTION1 0x10fc
#define TOP_CLK_CON_MUX_MUX_CLKCMU_TOP_CMUREF 0x1100
#define TOP_CLK_CON_MUX_MUX_CLKCMU_TPU_NOC 0x1104
#define TOP_CLK_CON_MUX_MUX_CLKCMU_TPU_TPU 0x1108
#define TOP_CLK_CON_MUX_MUX_CLKCMU_TPU_TPUCTL 0x110c
#define TOP_CLK_CON_MUX_MUX_CLKCMU_TPU_UART 0x1110
#define TOP_CLK_CON_MUX_MUX_CMU_CMUREF 0x1114
#define TOP_CLK_CON_DIV_CLKCMU_AUR_AUR 0x1804
#define TOP_CLK_CON_DIV_CLKCMU_AUR_AURCTL 0x1808
#define TOP_CLK_CON_DIV_CLKCMU_AUR_NOC 0x180c
#define TOP_CLK_CON_DIV_CLKCMU_BO_NOC 0x1810
#define TOP_CLK_CON_DIV_CLKCMU_CIS_CLK0 0x1814
#define TOP_CLK_CON_DIV_CLKCMU_CIS_CLK1 0x1818
#define TOP_CLK_CON_DIV_CLKCMU_CIS_CLK2 0x181c
#define TOP_CLK_CON_DIV_CLKCMU_CIS_CLK3 0x1820
#define TOP_CLK_CON_DIV_CLKCMU_CIS_CLK4 0x1824
#define TOP_CLK_CON_DIV_CLKCMU_CIS_CLK5 0x1828
#define TOP_CLK_CON_DIV_CLKCMU_CIS_CLK6 0x182c
#define TOP_CLK_CON_DIV_CLKCMU_CIS_CLK7 0x1830
#define TOP_CLK_CON_DIV_CLKCMU_CPUCL0_DBG 0x1834
#define TOP_CLK_CON_DIV_CLKCMU_CPUCL0_SWITCH 0x1838
#define TOP_CLK_CON_DIV_CLKCMU_CPUCL1_SWITCH 0x183c
#define TOP_CLK_CON_DIV_CLKCMU_CPUCL2_SWITCH 0x1840
#define TOP_CLK_CON_DIV_CLKCMU_CSIS_NOC 0x1844
#define TOP_CLK_CON_DIV_CLKCMU_DISP_NOC 0x1848
#define TOP_CLK_CON_DIV_CLKCMU_DNS_NOC 0x184c
#define TOP_CLK_CON_DIV_CLKCMU_DPU_NOC 0x1850
#define TOP_CLK_CON_DIV_CLKCMU_EH_NOC 0x1854
#define TOP_CLK_CON_DIV_CLKCMU_G2D_G2D 0x1858
#define TOP_CLK_CON_DIV_CLKCMU_G2D_MSCL 0x185c
#define TOP_CLK_CON_DIV_CLKCMU_G3AA_G3AA 0x1860
#define TOP_CLK_CON_DIV_CLKCMU_G3D_GLB 0x1864
#define TOP_CLK_CON_DIV_CLKCMU_G3D_NOCD 0x1868
#define TOP_CLK_CON_DIV_CLKCMU_G3D_SWITCH 0x186c
#define TOP_CLK_CON_DIV_CLKCMU_GDC_GDC0 0x1870
#define TOP_CLK_CON_DIV_CLKCMU_GDC_GDC1 0x1874
#define TOP_CLK_CON_DIV_CLKCMU_GDC_SCSC 0x1878
#define TOP_CLK_CON_DIV_CLKCMU_HPM 0x187c
#define TOP_CLK_CON_DIV_CLKCMU_HSI0_DPGTC 0x1880
#define TOP_CLK_CON_DIV_CLKCMU_HSI0_NOC 0x1884
#define TOP_CLK_CON_DIV_CLKCMU_HSI0_USB31DRD 0x1888
#define TOP_CLK_CON_DIV_CLKCMU_HSI0_USBDPDBG 0x188c
#define TOP_CLK_CON_DIV_CLKCMU_HSI1_NOC 0x1890
#define TOP_CLK_CON_DIV_CLKCMU_HSI1_PCIE 0x1894
#define TOP_CLK_CON_DIV_CLKCMU_HSI2_MMC_CARD 0x1898
#define TOP_CLK_CON_DIV_CLKCMU_HSI2_NOC 0x189c
#define TOP_CLK_CON_DIV_CLKCMU_HSI2_PCIE 0x18a0
#define TOP_CLK_CON_DIV_CLKCMU_HSI2_UFS_EMBD 0x18a4
#define TOP_CLK_CON_DIV_CLKCMU_IPP_NOC 0x18a8
#define TOP_CLK_CON_DIV_CLKCMU_ITP_NOC 0x18ac
#define TOP_CLK_CON_DIV_CLKCMU_MCSC_ITSC 0x18b0
#define TOP_CLK_CON_DIV_CLKCMU_MCSC_MCSC 0x18b4
#define TOP_CLK_CON_DIV_CLKCMU_MFC_MFC 0x18b8
#define TOP_CLK_CON_DIV_CLKCMU_MIF_NOCP 0x18bc
#define TOP_CLK_CON_DIV_CLKCMU_MISC_NOC 0x18c0
#define TOP_CLK_CON_DIV_CLKCMU_MISC_SSS 0x18c4
#define TOP_CLK_CON_DIV_CLKCMU_NOCL0_NOC 0x18c8
#define TOP_CLK_CON_DIV_CLKCMU_NOCL1A_NOC 0x18cc
#define TOP_CLK_CON_DIV_CLKCMU_NOCL1B_NOC 0x18d0
#define TOP_CLK_CON_DIV_CLKCMU_NOCL2A_NOC 0x18d4
#define TOP_CLK_CON_DIV_CLKCMU_OTP 0x18d8
#define TOP_CLK_CON_DIV_CLKCMU_PDP_NOC 0x18dc
#define TOP_CLK_CON_DIV_CLKCMU_PDP_VRA 0x18e0
#define TOP_CLK_CON_DIV_CLKCMU_PERIC0_IP 0x18e4
#define TOP_CLK_CON_DIV_CLKCMU_PERIC0_NOC 0x18e8
#define TOP_CLK_CON_DIV_CLKCMU_PERIC1_IP 0x18ec
#define TOP_CLK_CON_DIV_CLKCMU_PERIC1_NOC 0x18f0
#define TOP_CLK_CON_DIV_CLKCMU_TNR_NOC 0x18f4
#define TOP_CLK_CON_DIV_CLKCMU_TPU_NOC 0x18f8
#define TOP_CLK_CON_DIV_CLKCMU_TPU_TPU 0x18fc
#define TOP_CLK_CON_DIV_CLKCMU_TPU_TPUCTL 0x1900
#define TOP_CLK_CON_DIV_CLKCMU_TPU_UART 0x1904
#define TOP_CLK_CON_DIV_DIV_CLKCMU_CMU_BOOST 0x1920
#define TOP_CLK_CON_DIV_DIV_CLK_CMU_CMUREF 0x1924
#define TOP_CLK_CON_DIV_PLL_SHARED0_DIV2 0x1928
#define TOP_CLK_CON_DIV_PLL_SHARED0_DIV3 0x192c
#define TOP_CLK_CON_DIV_PLL_SHARED0_DIV4 0x1930
#define TOP_CLK_CON_DIV_PLL_SHARED0_DIV5 0x1934
#define TOP_CLK_CON_DIV_PLL_SHARED1_DIV2 0x1938
#define TOP_CLK_CON_DIV_PLL_SHARED1_DIV3 0x193c
#define TOP_CLK_CON_DIV_PLL_SHARED1_DIV4 0x1940
#define TOP_CLK_CON_DIV_PLL_SHARED2_DIV2 0x1944
#define TOP_CLK_CON_DIV_PLL_SHARED3_DIV2 0x1948
#define TOP_CLK_CON_GAT_CLKCMU_CPUCL0_BOOST 0x2000
#define TOP_CLK_CON_GAT_CLKCMU_CPUCL1_BOOST 0x2004
#define TOP_CLK_CON_GAT_CLKCMU_CPUCL2_BOOST 0x2008
#define TOP_CLK_CON_GAT_CLKCMU_MIF_BOOST 0x200c
#define TOP_CLK_CON_GAT_CLKCMU_MIF_SWITCH 0x2010
#define TOP_CLK_CON_GAT_CLKCMU_NOCL0_BOOST 0x2014
#define TOP_CLK_CON_GAT_CLKCMU_NOCL1A_BOOST 0x2018
#define TOP_CLK_CON_GAT_CLKCMU_NOCL1B_BOOST 0x201c
#define TOP_CLK_CON_GAT_CLKCMU_NOCL2A_BOOST 0x2020
#define TOP_CLK_CON_GAT_GATE_CLKCMU_AUR_AUR 0x2028
#define TOP_CLK_CON_GAT_GATE_CLKCMU_AUR_AURCTL 0x202c
#define TOP_CLK_CON_GAT_GATE_CLKCMU_AUR_NOC 0x2030
#define TOP_CLK_CON_GAT_GATE_CLKCMU_BO_NOC 0x2034
#define TOP_CLK_CON_GAT_GATE_CLKCMU_CIS_CLK0 0x2038
#define TOP_CLK_CON_GAT_GATE_CLKCMU_CIS_CLK1 0x203c
#define TOP_CLK_CON_GAT_GATE_CLKCMU_CIS_CLK2 0x2040
#define TOP_CLK_CON_GAT_GATE_CLKCMU_CIS_CLK3 0x2044
#define TOP_CLK_CON_GAT_GATE_CLKCMU_CIS_CLK4 0x2048
#define TOP_CLK_CON_GAT_GATE_CLKCMU_CIS_CLK5 0x204c
#define TOP_CLK_CON_GAT_GATE_CLKCMU_CIS_CLK6 0x2050
#define TOP_CLK_CON_GAT_GATE_CLKCMU_CIS_CLK7 0x2054
#define TOP_CLK_CON_GAT_GATE_CLKCMU_CMU_BOOST 0x2058
#define TOP_CLK_CON_GAT_GATE_CLKCMU_CPUCL0_DBG_NOC 0x205c
#define TOP_CLK_CON_GAT_GATE_CLKCMU_CPUCL0_SWITCH 0x2060
#define TOP_CLK_CON_GAT_GATE_CLKCMU_CPUCL1_SWITCH 0x2064
#define TOP_CLK_CON_GAT_GATE_CLKCMU_CPUCL2_SWITCH 0x2068
#define TOP_CLK_CON_GAT_GATE_CLKCMU_CSIS_NOC 0x206c
#define TOP_CLK_CON_GAT_GATE_CLKCMU_DISP_NOC 0x2070
#define TOP_CLK_CON_GAT_GATE_CLKCMU_DNS_NOC 0x2074
#define TOP_CLK_CON_GAT_GATE_CLKCMU_DPU_NOC 0x2078
#define TOP_CLK_CON_GAT_GATE_CLKCMU_EH_NOC 0x207c
#define TOP_CLK_CON_GAT_GATE_CLKCMU_G2D_G2D 0x2080
#define TOP_CLK_CON_GAT_GATE_CLKCMU_G2D_MSCL 0x2084
#define TOP_CLK_CON_GAT_GATE_CLKCMU_G3AA_G3AA 0x2088
#define TOP_CLK_CON_GAT_GATE_CLKCMU_G3D_GLB 0x208c
#define TOP_CLK_CON_GAT_GATE_CLKCMU_G3D_NOCD 0x2090
#define TOP_CLK_CON_GAT_GATE_CLKCMU_G3D_SWITCH 0x2094
#define TOP_CLK_CON_GAT_GATE_CLKCMU_GDC_GDC0 0x2098
#define TOP_CLK_CON_GAT_GATE_CLKCMU_GDC_GDC1 0x209c
#define TOP_CLK_CON_GAT_GATE_CLKCMU_GDC_SCSC 0x20a0
#define TOP_CLK_CON_GAT_GATE_CLKCMU_HPM 0x20a4
#define TOP_CLK_CON_GAT_GATE_CLKCMU_HSI0_DPGTC 0x20a8
#define TOP_CLK_CON_GAT_GATE_CLKCMU_HSI0_NOC 0x20ac
#define TOP_CLK_CON_GAT_GATE_CLKCMU_HSI0_USB31DRD 0x20b0
#define TOP_CLK_CON_GAT_GATE_CLKCMU_HSI0_USBDPDBG 0x20b4
#define TOP_CLK_CON_GAT_GATE_CLKCMU_HSI1_NOC 0x20b8
#define TOP_CLK_CON_GAT_GATE_CLKCMU_HSI1_PCIE 0x20bc
#define TOP_CLK_CON_GAT_GATE_CLKCMU_HSI2_MMCCARD 0x20c0
#define TOP_CLK_CON_GAT_GATE_CLKCMU_HSI2_NOC 0x20c4
#define TOP_CLK_CON_GAT_GATE_CLKCMU_HSI2_PCIE 0x20c8
#define TOP_CLK_CON_GAT_GATE_CLKCMU_HSI2_UFS_EMBD 0x20cc
#define TOP_CLK_CON_GAT_GATE_CLKCMU_IPP_NOC 0x20d0
#define TOP_CLK_CON_GAT_GATE_CLKCMU_ITP_NOC 0x20d4
#define TOP_CLK_CON_GAT_GATE_CLKCMU_MCSC_ITSC 0x20d8
#define TOP_CLK_CON_GAT_GATE_CLKCMU_MCSC_MCSC 0x20dc
#define TOP_CLK_CON_GAT_GATE_CLKCMU_MFC_MFC 0x20e0
#define TOP_CLK_CON_GAT_GATE_CLKCMU_MIF_NOCP 0x20e4
#define TOP_CLK_CON_GAT_GATE_CLKCMU_MISC_NOC 0x20e8
#define TOP_CLK_CON_GAT_GATE_CLKCMU_MISC_SSS 0x20ec
#define TOP_CLK_CON_GAT_GATE_CLKCMU_NOCL0_NOC 0x20f0
#define TOP_CLK_CON_GAT_GATE_CLKCMU_NOCL1A_NOC 0x20f4
#define TOP_CLK_CON_GAT_GATE_CLKCMU_NOCL1B_NOC 0x20f8
#define TOP_CLK_CON_GAT_GATE_CLKCMU_NOCL2A_NOC 0x20fc
#define TOP_CLK_CON_GAT_GATE_CLKCMU_PDP_NOC 0x2100
#define TOP_CLK_CON_GAT_GATE_CLKCMU_PDP_VRA 0x2104
#define TOP_CLK_CON_GAT_GATE_CLKCMU_PERIC0_IP 0x2108
#define TOP_CLK_CON_GAT_GATE_CLKCMU_PERIC0_NOC 0x210c
#define TOP_CLK_CON_GAT_GATE_CLKCMU_PERIC1_IP 0x2110
#define TOP_CLK_CON_GAT_GATE_CLKCMU_PERIC1_NOC 0x2114
#define TOP_CLK_CON_GAT_GATE_CLKCMU_TNR_NOC 0x2118
#define TOP_CLK_CON_GAT_GATE_CLKCMU_TOP_CMUREF 0x211c
#define TOP_CLK_CON_GAT_GATE_CLKCMU_TPU_NOC 0x2120
#define TOP_CLK_CON_GAT_GATE_CLKCMU_TPU_TPU 0x2124
#define TOP_CLK_CON_GAT_GATE_CLKCMU_TPU_TPUCTL 0x2128
#define TOP_CLK_CON_GAT_GATE_CLKCMU_TPU_UART 0x212c

static const unsigned long top_clk_regs[] __initconst = {
	TOP_PLL_LOCKTIME_PLL_LF_MIF,
	TOP_PLL_LOCKTIME_PLL_SHARED0,
	TOP_PLL_LOCKTIME_PLL_SHARED1,
	TOP_PLL_LOCKTIME_PLL_SHARED2,
	TOP_PLL_LOCKTIME_PLL_SHARED3,
	TOP_PLL_LOCKTIME_PLL_SPARE,
	TOP_PLL_CON0_PLL_LF_MIF,
	TOP_PLL_CON1_PLL_LF_MIF,
	TOP_PLL_CON2_PLL_LF_MIF,
	TOP_PLL_CON3_PLL_LF_MIF,
	TOP_PLL_CON4_PLL_LF_MIF,
	TOP_PLL_CON0_PLL_SHARED0,
	TOP_PLL_CON1_PLL_SHARED0,
	TOP_PLL_CON2_PLL_SHARED0,
	TOP_PLL_CON3_PLL_SHARED0,
	TOP_PLL_CON4_PLL_SHARED0,
	TOP_PLL_CON0_PLL_SHARED1,
	TOP_PLL_CON1_PLL_SHARED1,
	TOP_PLL_CON2_PLL_SHARED1,
	TOP_PLL_CON3_PLL_SHARED1,
	TOP_PLL_CON4_PLL_SHARED1,
	TOP_PLL_CON0_PLL_SHARED2,
	TOP_PLL_CON1_PLL_SHARED2,
	TOP_PLL_CON2_PLL_SHARED2,
	TOP_PLL_CON3_PLL_SHARED2,
	TOP_PLL_CON4_PLL_SHARED2,
	TOP_PLL_CON0_PLL_SHARED3,
	TOP_PLL_CON1_PLL_SHARED3,
	TOP_PLL_CON2_PLL_SHARED3,
	TOP_PLL_CON3_PLL_SHARED3,
	TOP_PLL_CON4_PLL_SHARED3,
	TOP_PLL_CON0_PLL_SPARE,
	TOP_PLL_CON1_PLL_SPARE,
	TOP_PLL_CON2_PLL_SPARE,
	TOP_PLL_CON3_PLL_SPARE,
	TOP_PLL_CON4_PLL_SPARE,
	TOP_CLK_CON_MUX_MUX_CLKCMU_AUR_AUR,
	TOP_CLK_CON_MUX_MUX_CLKCMU_AUR_AURCTL,
	TOP_CLK_CON_MUX_MUX_CLKCMU_AUR_NOC,
	TOP_CLK_CON_MUX_MUX_CLKCMU_BO_NOC,
	TOP_CLK_CON_MUX_MUX_CLKCMU_CIS_CLK0,
	TOP_CLK_CON_MUX_MUX_CLKCMU_CIS_CLK1,
	TOP_CLK_CON_MUX_MUX_CLKCMU_CIS_CLK2,
	TOP_CLK_CON_MUX_MUX_CLKCMU_CIS_CLK3,
	TOP_CLK_CON_MUX_MUX_CLKCMU_CIS_CLK4,
	TOP_CLK_CON_MUX_MUX_CLKCMU_CIS_CLK5,
	TOP_CLK_CON_MUX_MUX_CLKCMU_CIS_CLK6,
	TOP_CLK_CON_MUX_MUX_CLKCMU_CIS_CLK7,
	TOP_CLK_CON_MUX_MUX_CLKCMU_CMU_BOOST,
	TOP_CLK_CON_MUX_MUX_CLKCMU_CMU_BOOST_OPTION1,
	TOP_CLK_CON_MUX_MUX_CLKCMU_CPUCL0_DBG,
	TOP_CLK_CON_MUX_MUX_CLKCMU_CPUCL0_SWITCH,
	TOP_CLK_CON_MUX_MUX_CLKCMU_CPUCL1_SWITCH,
	TOP_CLK_CON_MUX_MUX_CLKCMU_CPUCL2_SWITCH,
	TOP_CLK_CON_MUX_MUX_CLKCMU_CSIS_NOC,
	TOP_CLK_CON_MUX_MUX_CLKCMU_DISP_NOC,
	TOP_CLK_CON_MUX_MUX_CLKCMU_DNS_NOC,
	TOP_CLK_CON_MUX_MUX_CLKCMU_DPU_NOC,
	TOP_CLK_CON_MUX_MUX_CLKCMU_EH_NOC,
	TOP_CLK_CON_MUX_MUX_CLKCMU_G2D_G2D,
	TOP_CLK_CON_MUX_MUX_CLKCMU_G2D_MSCL,
	TOP_CLK_CON_MUX_MUX_CLKCMU_G3AA_G3AA,
	TOP_CLK_CON_MUX_MUX_CLKCMU_G3D_GLB,
	TOP_CLK_CON_MUX_MUX_CLKCMU_G3D_NOCD,
	TOP_CLK_CON_MUX_MUX_CLKCMU_G3D_SWITCH,
	TOP_CLK_CON_MUX_MUX_CLKCMU_GDC_GDC0,
	TOP_CLK_CON_MUX_MUX_CLKCMU_GDC_GDC1,
	TOP_CLK_CON_MUX_MUX_CLKCMU_GDC_SCSC,
	TOP_CLK_CON_MUX_MUX_CLKCMU_HPM,
	TOP_CLK_CON_MUX_MUX_CLKCMU_HSI0_DPGTC,
	TOP_CLK_CON_MUX_MUX_CLKCMU_HSI0_NOC,
	TOP_CLK_CON_MUX_MUX_CLKCMU_HSI0_USB31DRD,
	TOP_CLK_CON_MUX_MUX_CLKCMU_HSI0_USBDPDBG,
	TOP_CLK_CON_MUX_MUX_CLKCMU_HSI1_NOC,
	TOP_CLK_CON_MUX_MUX_CLKCMU_HSI1_PCIE,
	TOP_CLK_CON_MUX_MUX_CLKCMU_HSI2_MMC_CARD,
	TOP_CLK_CON_MUX_MUX_CLKCMU_HSI2_NOC,
	TOP_CLK_CON_MUX_MUX_CLKCMU_HSI2_PCIE,
	TOP_CLK_CON_MUX_MUX_CLKCMU_HSI2_UFS_EMBD,
	TOP_CLK_CON_MUX_MUX_CLKCMU_IPP_NOC,
	TOP_CLK_CON_MUX_MUX_CLKCMU_ITP_NOC,
	TOP_CLK_CON_MUX_MUX_CLKCMU_MCSC_ITSC,
	TOP_CLK_CON_MUX_MUX_CLKCMU_MCSC_MCSC,
	TOP_CLK_CON_MUX_MUX_CLKCMU_MFC_MFC,
	TOP_CLK_CON_MUX_MUX_CLKCMU_MIF_NOCP,
	TOP_CLK_CON_MUX_MUX_CLKCMU_MIF_SWITCH,
	TOP_CLK_CON_MUX_MUX_CLKCMU_MISC_NOC,
	TOP_CLK_CON_MUX_MUX_CLKCMU_MISC_SSS,
	TOP_CLK_CON_MUX_MUX_CLKCMU_NOCL0_NOC,
	TOP_CLK_CON_MUX_MUX_CLKCMU_NOCL1A_NOC,
	TOP_CLK_CON_MUX_MUX_CLKCMU_NOCL1B_NOC,
	TOP_CLK_CON_MUX_MUX_CLKCMU_NOCL2A_NOC,
	TOP_CLK_CON_MUX_MUX_CLKCMU_PDP_NOC,
	TOP_CLK_CON_MUX_MUX_CLKCMU_PDP_VRA,
	TOP_CLK_CON_MUX_MUX_CLKCMU_PERIC0_IP,
	TOP_CLK_CON_MUX_MUX_CLKCMU_PERIC0_NOC,
	TOP_CLK_CON_MUX_MUX_CLKCMU_PERIC1_IP,
	TOP_CLK_CON_MUX_MUX_CLKCMU_PERIC1_NOC,
	TOP_CLK_CON_MUX_MUX_CLKCMU_TNR_NOC,
	TOP_CLK_CON_MUX_MUX_CLKCMU_TOP_BOOST_OPTION1,
	TOP_CLK_CON_MUX_MUX_CLKCMU_TOP_CMUREF,
	TOP_CLK_CON_MUX_MUX_CLKCMU_TPU_NOC,
	TOP_CLK_CON_MUX_MUX_CLKCMU_TPU_TPU,
	TOP_CLK_CON_MUX_MUX_CLKCMU_TPU_TPUCTL,
	TOP_CLK_CON_MUX_MUX_CLKCMU_TPU_UART,
	TOP_CLK_CON_MUX_MUX_CMU_CMUREF,
	TOP_CLK_CON_DIV_CLKCMU_AUR_AUR,
	TOP_CLK_CON_DIV_CLKCMU_AUR_AURCTL,
	TOP_CLK_CON_DIV_CLKCMU_AUR_NOC,
	TOP_CLK_CON_DIV_CLKCMU_BO_NOC,
	TOP_CLK_CON_DIV_CLKCMU_CIS_CLK0,
	TOP_CLK_CON_DIV_CLKCMU_CIS_CLK1,
	TOP_CLK_CON_DIV_CLKCMU_CIS_CLK2,
	TOP_CLK_CON_DIV_CLKCMU_CIS_CLK3,
	TOP_CLK_CON_DIV_CLKCMU_CIS_CLK4,
	TOP_CLK_CON_DIV_CLKCMU_CIS_CLK5,
	TOP_CLK_CON_DIV_CLKCMU_CIS_CLK6,
	TOP_CLK_CON_DIV_CLKCMU_CIS_CLK7,
	TOP_CLK_CON_DIV_CLKCMU_CPUCL0_DBG,
	TOP_CLK_CON_DIV_CLKCMU_CPUCL0_SWITCH,
	TOP_CLK_CON_DIV_CLKCMU_CPUCL1_SWITCH,
	TOP_CLK_CON_DIV_CLKCMU_CPUCL2_SWITCH,
	TOP_CLK_CON_DIV_CLKCMU_CSIS_NOC,
	TOP_CLK_CON_DIV_CLKCMU_DISP_NOC,
	TOP_CLK_CON_DIV_CLKCMU_DNS_NOC,
	TOP_CLK_CON_DIV_CLKCMU_DPU_NOC,
	TOP_CLK_CON_DIV_CLKCMU_EH_NOC,
	TOP_CLK_CON_DIV_CLKCMU_G2D_G2D,
	TOP_CLK_CON_DIV_CLKCMU_G2D_MSCL,
	TOP_CLK_CON_DIV_CLKCMU_G3AA_G3AA,
	TOP_CLK_CON_DIV_CLKCMU_G3D_GLB,
	TOP_CLK_CON_DIV_CLKCMU_G3D_NOCD,
	TOP_CLK_CON_DIV_CLKCMU_G3D_SWITCH,
	TOP_CLK_CON_DIV_CLKCMU_GDC_GDC0,
	TOP_CLK_CON_DIV_CLKCMU_GDC_GDC1,
	TOP_CLK_CON_DIV_CLKCMU_GDC_SCSC,
	TOP_CLK_CON_DIV_CLKCMU_HPM,
	TOP_CLK_CON_DIV_CLKCMU_HSI0_DPGTC,
	TOP_CLK_CON_DIV_CLKCMU_HSI0_NOC,
	TOP_CLK_CON_DIV_CLKCMU_HSI0_USB31DRD,
	TOP_CLK_CON_DIV_CLKCMU_HSI0_USBDPDBG,
	TOP_CLK_CON_DIV_CLKCMU_HSI1_NOC,
	TOP_CLK_CON_DIV_CLKCMU_HSI1_PCIE,
	TOP_CLK_CON_DIV_CLKCMU_HSI2_MMC_CARD,
	TOP_CLK_CON_DIV_CLKCMU_HSI2_NOC,
	TOP_CLK_CON_DIV_CLKCMU_HSI2_PCIE,
	TOP_CLK_CON_DIV_CLKCMU_HSI2_UFS_EMBD,
	TOP_CLK_CON_DIV_CLKCMU_IPP_NOC,
	TOP_CLK_CON_DIV_CLKCMU_ITP_NOC,
	TOP_CLK_CON_DIV_CLKCMU_MCSC_ITSC,
	TOP_CLK_CON_DIV_CLKCMU_MCSC_MCSC,
	TOP_CLK_CON_DIV_CLKCMU_MFC_MFC,
	TOP_CLK_CON_DIV_CLKCMU_MIF_NOCP,
	TOP_CLK_CON_DIV_CLKCMU_MISC_NOC,
	TOP_CLK_CON_DIV_CLKCMU_MISC_SSS,
	TOP_CLK_CON_DIV_CLKCMU_NOCL0_NOC,
	TOP_CLK_CON_DIV_CLKCMU_NOCL1A_NOC,
	TOP_CLK_CON_DIV_CLKCMU_NOCL1B_NOC,
	TOP_CLK_CON_DIV_CLKCMU_NOCL2A_NOC,
	TOP_CLK_CON_DIV_CLKCMU_OTP,
	TOP_CLK_CON_DIV_CLKCMU_PDP_NOC,
	TOP_CLK_CON_DIV_CLKCMU_PDP_VRA,
	TOP_CLK_CON_DIV_CLKCMU_PERIC0_IP,
	TOP_CLK_CON_DIV_CLKCMU_PERIC0_NOC,
	TOP_CLK_CON_DIV_CLKCMU_PERIC1_IP,
	TOP_CLK_CON_DIV_CLKCMU_PERIC1_NOC,
	TOP_CLK_CON_DIV_CLKCMU_TNR_NOC,
	TOP_CLK_CON_DIV_CLKCMU_TPU_NOC,
	TOP_CLK_CON_DIV_CLKCMU_TPU_TPU,
	TOP_CLK_CON_DIV_CLKCMU_TPU_TPUCTL,
	TOP_CLK_CON_DIV_CLKCMU_TPU_UART,
	TOP_CLK_CON_DIV_DIV_CLKCMU_CMU_BOOST,
	TOP_CLK_CON_DIV_DIV_CLK_CMU_CMUREF,
	TOP_CLK_CON_DIV_PLL_SHARED0_DIV2,
	TOP_CLK_CON_DIV_PLL_SHARED0_DIV3,
	TOP_CLK_CON_DIV_PLL_SHARED0_DIV4,
	TOP_CLK_CON_DIV_PLL_SHARED0_DIV5,
	TOP_CLK_CON_DIV_PLL_SHARED1_DIV2,
	TOP_CLK_CON_DIV_PLL_SHARED1_DIV3,
	TOP_CLK_CON_DIV_PLL_SHARED1_DIV4,
	TOP_CLK_CON_DIV_PLL_SHARED2_DIV2,
	TOP_CLK_CON_DIV_PLL_SHARED3_DIV2,
	TOP_CLK_CON_GAT_CLKCMU_CPUCL0_BOOST,
	TOP_CLK_CON_GAT_CLKCMU_CPUCL1_BOOST,
	TOP_CLK_CON_GAT_CLKCMU_CPUCL2_BOOST,
	TOP_CLK_CON_GAT_CLKCMU_MIF_BOOST,
	TOP_CLK_CON_GAT_CLKCMU_MIF_SWITCH,
	TOP_CLK_CON_GAT_CLKCMU_NOCL0_BOOST,
	TOP_CLK_CON_GAT_CLKCMU_NOCL1A_BOOST,
	TOP_CLK_CON_GAT_CLKCMU_NOCL1B_BOOST,
	TOP_CLK_CON_GAT_CLKCMU_NOCL2A_BOOST,
	TOP_CLK_CON_GAT_GATE_CLKCMU_AUR_AUR,
	TOP_CLK_CON_GAT_GATE_CLKCMU_AUR_AURCTL,
	TOP_CLK_CON_GAT_GATE_CLKCMU_AUR_NOC,
	TOP_CLK_CON_GAT_GATE_CLKCMU_BO_NOC,
	TOP_CLK_CON_GAT_GATE_CLKCMU_CIS_CLK0,
	TOP_CLK_CON_GAT_GATE_CLKCMU_CIS_CLK1,
	TOP_CLK_CON_GAT_GATE_CLKCMU_CIS_CLK2,
	TOP_CLK_CON_GAT_GATE_CLKCMU_CIS_CLK3,
	TOP_CLK_CON_GAT_GATE_CLKCMU_CIS_CLK4,
	TOP_CLK_CON_GAT_GATE_CLKCMU_CIS_CLK5,
	TOP_CLK_CON_GAT_GATE_CLKCMU_CIS_CLK6,
	TOP_CLK_CON_GAT_GATE_CLKCMU_CIS_CLK7,
	TOP_CLK_CON_GAT_GATE_CLKCMU_CMU_BOOST,
	TOP_CLK_CON_GAT_GATE_CLKCMU_CPUCL0_DBG_NOC,
	TOP_CLK_CON_GAT_GATE_CLKCMU_CPUCL0_SWITCH,
	TOP_CLK_CON_GAT_GATE_CLKCMU_CPUCL1_SWITCH,
	TOP_CLK_CON_GAT_GATE_CLKCMU_CPUCL2_SWITCH,
	TOP_CLK_CON_GAT_GATE_CLKCMU_CSIS_NOC,
	TOP_CLK_CON_GAT_GATE_CLKCMU_DISP_NOC,
	TOP_CLK_CON_GAT_GATE_CLKCMU_DNS_NOC,
	TOP_CLK_CON_GAT_GATE_CLKCMU_DPU_NOC,
	TOP_CLK_CON_GAT_GATE_CLKCMU_EH_NOC,
	TOP_CLK_CON_GAT_GATE_CLKCMU_G2D_G2D,
	TOP_CLK_CON_GAT_GATE_CLKCMU_G2D_MSCL,
	TOP_CLK_CON_GAT_GATE_CLKCMU_G3AA_G3AA,
	TOP_CLK_CON_GAT_GATE_CLKCMU_G3D_GLB,
	TOP_CLK_CON_GAT_GATE_CLKCMU_G3D_NOCD,
	TOP_CLK_CON_GAT_GATE_CLKCMU_G3D_SWITCH,
	TOP_CLK_CON_GAT_GATE_CLKCMU_GDC_GDC0,
	TOP_CLK_CON_GAT_GATE_CLKCMU_GDC_GDC1,
	TOP_CLK_CON_GAT_GATE_CLKCMU_GDC_SCSC,
	TOP_CLK_CON_GAT_GATE_CLKCMU_HPM,
	TOP_CLK_CON_GAT_GATE_CLKCMU_HSI0_DPGTC,
	TOP_CLK_CON_GAT_GATE_CLKCMU_HSI0_NOC,
	TOP_CLK_CON_GAT_GATE_CLKCMU_HSI0_USB31DRD,
	TOP_CLK_CON_GAT_GATE_CLKCMU_HSI0_USBDPDBG,
	TOP_CLK_CON_GAT_GATE_CLKCMU_HSI1_NOC,
	TOP_CLK_CON_GAT_GATE_CLKCMU_HSI1_PCIE,
	TOP_CLK_CON_GAT_GATE_CLKCMU_HSI2_MMCCARD,
	TOP_CLK_CON_GAT_GATE_CLKCMU_HSI2_NOC,
	TOP_CLK_CON_GAT_GATE_CLKCMU_HSI2_PCIE,
	TOP_CLK_CON_GAT_GATE_CLKCMU_HSI2_UFS_EMBD,
	TOP_CLK_CON_GAT_GATE_CLKCMU_IPP_NOC,
	TOP_CLK_CON_GAT_GATE_CLKCMU_ITP_NOC,
	TOP_CLK_CON_GAT_GATE_CLKCMU_MCSC_ITSC,
	TOP_CLK_CON_GAT_GATE_CLKCMU_MCSC_MCSC,
	TOP_CLK_CON_GAT_GATE_CLKCMU_MFC_MFC,
	TOP_CLK_CON_GAT_GATE_CLKCMU_MIF_NOCP,
	TOP_CLK_CON_GAT_GATE_CLKCMU_MISC_NOC,
	TOP_CLK_CON_GAT_GATE_CLKCMU_MISC_SSS,
	TOP_CLK_CON_GAT_GATE_CLKCMU_NOCL0_NOC,
	TOP_CLK_CON_GAT_GATE_CLKCMU_NOCL1A_NOC,
	TOP_CLK_CON_GAT_GATE_CLKCMU_NOCL1B_NOC,
	TOP_CLK_CON_GAT_GATE_CLKCMU_NOCL2A_NOC,
	TOP_CLK_CON_GAT_GATE_CLKCMU_PDP_NOC,
	TOP_CLK_CON_GAT_GATE_CLKCMU_PDP_VRA,
	TOP_CLK_CON_GAT_GATE_CLKCMU_PERIC0_IP,
	TOP_CLK_CON_GAT_GATE_CLKCMU_PERIC0_NOC,
	TOP_CLK_CON_GAT_GATE_CLKCMU_PERIC1_IP,
	TOP_CLK_CON_GAT_GATE_CLKCMU_PERIC1_NOC,
	TOP_CLK_CON_GAT_GATE_CLKCMU_TNR_NOC,
	TOP_CLK_CON_GAT_GATE_CLKCMU_TOP_CMUREF,
	TOP_CLK_CON_GAT_GATE_CLKCMU_TPU_NOC,
	TOP_CLK_CON_GAT_GATE_CLKCMU_TPU_TPU,
	TOP_CLK_CON_GAT_GATE_CLKCMU_TPU_TPUCTL,
	TOP_CLK_CON_GAT_GATE_CLKCMU_TPU_UART,
};

#define CLKS_NR_TOP 234

PNAME(mout_pll_lf_mif_p) = { "oscclk", "fout_lf_mif_pll" };
PNAME(mout_pll_shared0_p) = { "oscclk", "fout_shared0_pll" };
PNAME(mout_pll_shared1_p) = { "oscclk", "fout_shared1_pll" };
PNAME(mout_pll_shared2_p) = { "oscclk", "fout_shared2_pll" };
PNAME(mout_pll_shared3_p) = { "oscclk", "fout_shared3_pll" };
PNAME(mout_pll_spare_p) = { "oscclk", "fout_spare_pll" };
PNAME(mout_cmu_aur_aur_p) = { "dout_pll_shared0_div2", "dout_pll_shared1_div2",
			      "mout_pll_shared2",      "mout_pll_shared3",
			      "dout_pll_shared0_div3", "dout_pll_shared1_div3",
			      "dout_pll_shared0_div4", "mout_pll_spare" };
PNAME(mout_cmu_aur_aurctl_p) = {
	"mout_pll_shared2",	 "dout_pll_shared0_div3",
	"mout_pll_shared3",	 "dout_pll_shared1_div3",
	"dout_pll_shared0_div4", "dout_pll_shared1_div4",
	"dout_pll_shared2_div2", "mout_pll_spare"
};

PNAME(mout_cmu_aur_noc_p) = { "dout_pll_shared0_div2", "dout_pll_shared1_div2",
			      "mout_pll_shared2",      "mout_pll_shared3",
			      "dout_pll_shared0_div3", "dout_pll_shared1_div3",
			      "dout_pll_shared0_div5", "mout_pll_spare" };
PNAME(mout_cmu_bo_noc_p) = { "mout_pll_shared2",      "dout_pll_shared0_div3",
			     "mout_pll_shared3",      "dout_pll_shared1_div3",
			     "dout_pll_shared0_div4", "dout_pll_shared1_div4",
			     "mout_pll_spare",	      "oscclk" };
PNAME(mout_cmu_cis_clk0_p) = { "oscclk",
			       "dout_pll_shared0_div3",
			       "dout_pll_shared1_div3",
			       "dout_pll_shared2_div2",
			       "dout_pll_shared3_div2",
			       "mout_pll_spare",
			       "oscclk",
			       "oscclk" };
PNAME(mout_cmu_cis_clk1_p) = { "oscclk",
			       "dout_pll_shared0_div3",
			       "dout_pll_shared1_div3",
			       "dout_pll_shared2_div2",
			       "dout_pll_shared3_div2",
			       "mout_pll_spare",
			       "oscclk",
			       "oscclk" };
PNAME(mout_cmu_cis_clk2_p) = { "oscclk",
			       "dout_pll_shared0_div3",
			       "dout_pll_shared1_div3",
			       "dout_pll_shared2_div2",
			       "dout_pll_shared3_div2",
			       "mout_pll_spare",
			       "oscclk",
			       "oscclk" };
PNAME(mout_cmu_cis_clk3_p) = { "oscclk",
			       "dout_pll_shared0_div3",
			       "dout_pll_shared1_div3",
			       "dout_pll_shared2_div2",
			       "dout_pll_shared3_div2",
			       "mout_pll_spare",
			       "oscclk",
			       "oscclk" };
PNAME(mout_cmu_cis_clk4_p) = { "oscclk",
			       "dout_pll_shared0_div3",
			       "dout_pll_shared1_div3",
			       "dout_pll_shared2_div2",
			       "dout_pll_shared3_div2",
			       "mout_pll_spare",
			       "oscclk",
			       "oscclk" };
PNAME(mout_cmu_cis_clk5_p) = { "oscclk",
			       "dout_pll_shared0_div3",
			       "dout_pll_shared1_div3",
			       "dout_pll_shared2_div2",
			       "dout_pll_shared3_div2",
			       "mout_pll_spare",
			       "oscclk",
			       "oscclk" };
PNAME(mout_cmu_cis_clk6_p) = { "oscclk",
			       "dout_pll_shared0_div3",
			       "dout_pll_shared1_div3",
			       "dout_pll_shared2_div2",
			       "dout_pll_shared3_div2",
			       "mout_pll_spare",
			       "oscclk",
			       "oscclk" };
PNAME(mout_cmu_cis_clk7_p) = { "oscclk",
			       "dout_pll_shared0_div3",
			       "dout_pll_shared1_div3",
			       "dout_pll_shared2_div2",
			       "dout_pll_shared3_div2",
			       "mout_pll_spare",
			       "oscclk",
			       "oscclk" };
PNAME(mout_cmu_cmu_boost_p) = { "dout_pll_shared0_div4",
				"dout_pll_shared1_div4",
				"dout_pll_shared2_div2",
				"dout_pll_shared3_div2" };
PNAME(mout_cmu_cmu_boost_option1_p) = { "dout_cmu_cmu_boost",
					"gout_clk_cmu_boost_option1" };
PNAME(mout_cmu_cpucl0_dbg_p) = { "mout_pll_shared2",
				 "mout_pll_shared3",
				 "dout_pll_shared0_div4",
				 "dout_pll_shared1_div4",
				 "dout_pll_shared2_div2",
				 "mout_pll_spare",
				 "oscclk",
				 "oscclk" };
PNAME(mout_cmu_cpucl0_switch_p) = {
	"mout_pll_shared1",	 "dout_pll_shared0_div2",
	"dout_pll_shared1_div2", "mout_pll_shared2",
	"mout_pll_shared3",	 "dout_pll_shared0_div3",
	"dout_pll_shared1_div3", "mout_pll_spare"
};

PNAME(mout_cmu_cpucl1_switch_p) = {
	"mout_pll_shared1",	 "dout_pll_shared0_div2",
	"dout_pll_shared1_div2", "mout_pll_shared2",
	"mout_pll_shared3",	 "dout_pll_shared0_div3",
	"dout_pll_shared1_div3", "mout_pll_spare"
};

PNAME(mout_cmu_cpucl2_switch_p) = {
	"mout_pll_shared1",	 "dout_pll_shared0_div2",
	"dout_pll_shared1_div2", "mout_pll_shared2",
	"mout_pll_shared3",	 "dout_pll_shared0_div3",
	"dout_pll_shared1_div3", "mout_pll_spare"
};

PNAME(mout_cmu_csis_noc_p) = { "dout_pll_shared0_div3", "mout_pll_shared3",
			       "dout_pll_shared1_div3", "dout_pll_shared0_div4",
			       "dout_pll_shared1_div4", "dout_pll_shared2_div2",
			       "mout_pll_spare",	"oscclk" };
PNAME(mout_cmu_disp_noc_p) = { "dout_pll_shared0_div3", "mout_pll_shared3",
			       "dout_pll_shared1_div3", "dout_pll_shared0_div4",
			       "dout_pll_shared1_div4", "dout_pll_shared2_div2",
			       "mout_pll_spare",	"oscclk" };
PNAME(mout_cmu_dns_noc_p) = { "dout_pll_shared0_div3", "mout_pll_shared3",
			      "dout_pll_shared1_div3", "dout_pll_shared0_div4",
			      "dout_pll_shared1_div4", "dout_pll_shared2_div2",
			      "mout_pll_spare",	       "oscclk" };
PNAME(mout_cmu_dpu_noc_p) = { "dout_pll_shared0_div3", "mout_pll_shared3",
			      "dout_pll_shared1_div3", "dout_pll_shared0_div4",
			      "dout_pll_shared1_div4", "dout_pll_shared2_div2",
			      "mout_pll_spare",	       "oscclk" };
PNAME(mout_cmu_eh_noc_p) = { "dout_pll_shared0_div2", "dout_pll_shared1_div2",
			     "mout_pll_shared2",      "mout_pll_shared3",
			     "dout_pll_shared0_div3", "dout_pll_shared1_div3",
			     "dout_pll_shared0_div5", "mout_pll_spare" };
PNAME(mout_cmu_g2d_g2d_p) = { "dout_pll_shared0_div3", "mout_pll_shared3",
			      "dout_pll_shared1_div3", "dout_pll_shared0_div4",
			      "dout_pll_shared1_div4", "dout_pll_shared2_div2",
			      "mout_pll_spare",	       "oscclk" };
PNAME(mout_cmu_g2d_mscl_p) = { "dout_pll_shared0_div4",
			       "dout_pll_shared1_div4",
			       "dout_pll_shared2_div2",
			       "dout_pll_shared3_div2",
			       "mout_pll_spare",
			       "oscclk",
			       "oscclk",
			       "oscclk" };
PNAME(mout_cmu_g3aa_g3aa_p) = {
	"dout_pll_shared0_div3", "mout_pll_shared3",
	"dout_pll_shared1_div3", "dout_pll_shared0_div4",
	"dout_pll_shared1_div4", "dout_pll_shared2_div2",
	"mout_pll_spare",	 "oscclk"
};

PNAME(mout_cmu_g3d_glb_p) = { "dout_pll_shared0_div2", "dout_pll_shared1_div2",
			      "mout_pll_shared2",      "mout_pll_shared3",
			      "dout_pll_shared0_div3", "dout_pll_shared1_div3",
			      "dout_pll_shared0_div4", "mout_pll_spare" };
PNAME(mout_cmu_g3d_nocd_p) = { "dout_pll_shared0_div2", "dout_pll_shared1_div2",
			       "mout_pll_shared2",	"mout_pll_shared3",
			       "dout_pll_shared0_div3", "dout_pll_shared1_div3",
			       "dout_pll_shared0_div4", "mout_pll_spare" };
PNAME(mout_cmu_g3d_switch_p) = {
	"mout_pll_shared2",	 "dout_pll_shared0_div3",
	"mout_pll_shared3",	 "dout_pll_shared1_div3",
	"dout_pll_shared0_div4", "dout_pll_shared1_div4",
	"mout_pll_spare",	 "mout_pll_spare"
};

PNAME(mout_cmu_gdc_gdc0_p) = { "dout_pll_shared0_div3", "mout_pll_shared3",
			       "dout_pll_shared1_div3", "dout_pll_shared0_div4",
			       "dout_pll_shared1_div4", "dout_pll_shared2_div2",
			       "mout_pll_spare",	"oscclk" };
PNAME(mout_cmu_gdc_gdc1_p) = { "dout_pll_shared0_div3", "mout_pll_shared3",
			       "dout_pll_shared1_div3", "dout_pll_shared0_div4",
			       "dout_pll_shared1_div4", "dout_pll_shared2_div2",
			       "mout_pll_spare",	"oscclk" };
PNAME(mout_cmu_gdc_scsc_p) = { "dout_pll_shared0_div3", "mout_pll_shared3",
			       "dout_pll_shared1_div3", "dout_pll_shared0_div4",
			       "dout_pll_shared1_div4", "dout_pll_shared2_div2",
			       "mout_pll_spare",	"oscclk" };
PNAME(mout_cmu_hpm_p) = { "oscclk", "dout_pll_shared1_div3",
			  "dout_pll_shared0_div4", "dout_pll_shared2_div2" };
PNAME(mout_cmu_hsi0_dpgtc_p) = { "oscclk", "dout_pll_shared0_div4",
				 "dout_pll_shared2_div2", "mout_pll_spare" };
PNAME(mout_cmu_hsi0_noc_p) = { "dout_pll_shared0_div4",
			       "dout_pll_shared1_div4",
			       "dout_pll_shared2_div2",
			       "dout_pll_shared3_div2",
			       "mout_pll_spare",
			       "oscclk",
			       "oscclk",
			       "oscclk" };
PNAME(mout_cmu_hsi0_usb31drd_p) = { "oscclk", "dout_pll_shared2_div2" };
PNAME(mout_cmu_hsi0_usbdpdbg_p) = { "oscclk", "dout_pll_shared2_div2" };
PNAME(mout_cmu_hsi1_noc_p) = { "dout_pll_shared0_div4",
			       "dout_pll_shared1_div4",
			       "dout_pll_shared2_div2",
			       "dout_pll_shared3_div2",
			       "mout_pll_spare",
			       "oscclk",
			       "oscclk",
			       "oscclk" };
PNAME(mout_cmu_hsi1_pcie_p) = { "oscclk", "dout_pll_shared2_div2" };
PNAME(mout_cmu_hsi2_mmc_card_p) = { "mout_pll_shared2", "mout_pll_shared3",
				    "dout_pll_shared0_div4", "mout_pll_spare" };
PNAME(mout_cmu_hsi2_noc_p) = { "dout_pll_shared0_div4",
			       "dout_pll_shared1_div4",
			       "dout_pll_shared2_div2",
			       "dout_pll_shared3_div2",
			       "mout_pll_spare",
			       "oscclk",
			       "oscclk",
			       "oscclk" };
PNAME(mout_cmu_hsi2_pcie_p) = { "oscclk", "dout_pll_shared2_div2" };
PNAME(mout_cmu_hsi2_ufs_embd_p) = { "oscclk", "dout_pll_shared0_div4",
				    "dout_pll_shared2_div2", "mout_pll_spare" };
PNAME(mout_cmu_ipp_noc_p) = { "dout_pll_shared0_div3", "mout_pll_shared3",
			      "dout_pll_shared1_div3", "dout_pll_shared0_div4",
			      "dout_pll_shared1_div4", "dout_pll_shared2_div2",
			      "mout_pll_spare",	       "oscclk" };
PNAME(mout_cmu_itp_noc_p) = { "dout_pll_shared0_div3", "mout_pll_shared3",
			      "dout_pll_shared1_div3", "dout_pll_shared0_div4",
			      "dout_pll_shared1_div4", "dout_pll_shared2_div2",
			      "mout_pll_spare",	       "oscclk" };
PNAME(mout_cmu_mcsc_itsc_p) = {
	"dout_pll_shared0_div3", "mout_pll_shared3",
	"dout_pll_shared1_div3", "dout_pll_shared0_div4",
	"dout_pll_shared1_div4", "dout_pll_shared2_div2",
	"mout_pll_spare",	 "oscclk"
};

PNAME(mout_cmu_mcsc_mcsc_p) = {
	"dout_pll_shared0_div3", "mout_pll_shared3",
	"dout_pll_shared1_div3", "dout_pll_shared0_div4",
	"dout_pll_shared1_div4", "dout_pll_shared2_div2",
	"mout_pll_spare",	 "oscclk"
};

PNAME(mout_cmu_mfc_mfc_p) = { "dout_pll_shared0_div3",
			      "mout_pll_shared3",
			      "dout_pll_shared0_div4",
			      "dout_pll_shared1_div4",
			      "dout_pll_shared2_div2",
			      "mout_pll_spare",
			      "oscclk",
			      "oscclk" };
PNAME(mout_cmu_mif_nocp_p) = { "dout_pll_shared0_div4", "dout_pll_shared1_div4",
			       "dout_pll_shared0_div5", "mout_pll_spare" };
PNAME(mout_cmu_mif_switch_p) = {
	"mout_pll_shared0",	 "mout_pll_shared1", "dout_pll_shared0_div2",
	"dout_pll_shared1_div2", "mout_pll_shared2", "dout_pll_shared0_div3",
	"mout_pll_lf_mif",	 "mout_pll_spare"
};

PNAME(mout_cmu_misc_noc_p) = { "dout_pll_shared0_div4", "dout_pll_shared2_div2",
			       "dout_pll_shared3_div2", "mout_pll_spare" };
PNAME(mout_cmu_misc_sss_p) = { "dout_pll_shared0_div4", "dout_pll_shared2_div2",
			       "dout_pll_shared3_div2", "mout_pll_spare" };
PNAME(mout_cmu_nocl0_noc_p) = {
	"dout_pll_shared0_div2", "dout_pll_shared1_div2",
	"mout_pll_shared2",	 "mout_pll_shared3",
	"dout_pll_shared0_div3", "dout_pll_shared1_div3",
	"dout_pll_shared0_div5", "mout_pll_spare"
};

PNAME(mout_cmu_nocl1a_noc_p) = {
	"dout_pll_shared0_div2", "dout_pll_shared1_div2",
	"mout_pll_shared2",	 "mout_pll_shared3",
	"dout_pll_shared0_div3", "dout_pll_shared1_div3",
	"dout_pll_shared0_div5", "mout_pll_spare"
};

PNAME(mout_cmu_nocl1b_noc_p) = { "dout_pll_shared0_div4",
				 "dout_pll_shared1_div4",
				 "dout_pll_shared2_div2",
				 "dout_pll_shared3_div2",
				 "mout_pll_spare",
				 "oscclk",
				 "oscclk",
				 "oscclk" };
PNAME(mout_cmu_nocl2a_noc_p) = {
	"dout_pll_shared0_div3", "mout_pll_shared3",
	"dout_pll_shared1_div3", "dout_pll_shared0_div4",
	"dout_pll_shared1_div4", "dout_pll_shared2_div2",
	"mout_pll_spare",	 "oscclk"
};

PNAME(mout_cmu_pdp_noc_p) = { "dout_pll_shared0_div3", "mout_pll_shared3",
			      "dout_pll_shared1_div3", "dout_pll_shared0_div4",
			      "dout_pll_shared1_div4", "dout_pll_shared2_div2",
			      "mout_pll_spare",	       "oscclk" };
PNAME(mout_cmu_pdp_vra_p) = { "mout_pll_shared2",      "dout_pll_shared0_div3",
			      "mout_pll_shared3",      "dout_pll_shared1_div3",
			      "dout_pll_shared0_div4", "dout_pll_shared1_div4",
			      "mout_pll_spare",	       "oscclk" };
PNAME(mout_cmu_peric0_ip_p) = { "dout_pll_shared0_div4",
				"dout_pll_shared2_div2",
				"dout_pll_shared3_div2", "mout_pll_spare" };
PNAME(mout_cmu_peric0_noc_p) = { "dout_pll_shared0_div4",
				 "dout_pll_shared2_div2",
				 "dout_pll_shared3_div2", "mout_pll_spare" };
PNAME(mout_cmu_peric1_ip_p) = { "dout_pll_shared0_div4",
				"dout_pll_shared2_div2",
				"dout_pll_shared3_div2", "mout_pll_spare" };
PNAME(mout_cmu_peric1_noc_p) = { "dout_pll_shared0_div4",
				 "dout_pll_shared2_div2",
				 "dout_pll_shared3_div2", "mout_pll_spare" };
PNAME(mout_cmu_tnr_noc_p) = { "dout_pll_shared0_div3", "mout_pll_shared3",
			      "dout_pll_shared1_div3", "dout_pll_shared0_div4",
			      "dout_pll_shared1_div4", "dout_pll_shared2_div2",
			      "mout_pll_spare",	       "oscclk" };
PNAME(mout_cmu_top_boost_option1_p) = { "oscclk",
					"gout_clk_cmu_boost_option1" };
PNAME(mout_cmu_top_cmuref_p) = { "dout_pll_shared0_div4",
				 "dout_pll_shared1_div4",
				 "dout_pll_shared2_div2",
				 "dout_pll_shared3_div2" };
PNAME(mout_cmu_tpu_noc_p) = { "dout_pll_shared0_div2", "dout_pll_shared1_div2",
			      "mout_pll_shared2",      "mout_pll_shared3",
			      "dout_pll_shared0_div3", "dout_pll_shared1_div3",
			      "dout_pll_shared0_div4", "mout_pll_spare" };
PNAME(mout_cmu_tpu_tpu_p) = { "dout_pll_shared0_div2", "dout_pll_shared1_div2",
			      "mout_pll_shared2",      "mout_pll_shared3",
			      "dout_pll_shared0_div3", "dout_pll_shared1_div3",
			      "dout_pll_shared0_div4", "mout_pll_spare" };
PNAME(mout_cmu_tpu_tpuctl_p) = {
	"dout_pll_shared0_div2", "dout_pll_shared1_div2",
	"mout_pll_shared2",	 "mout_pll_shared3",
	"dout_pll_shared0_div3", "dout_pll_shared1_div3",
	"dout_pll_shared0_div4", "mout_pll_spare"
};

PNAME(mout_cmu_tpu_uart_p) = { "dout_pll_shared0_div4", "dout_pll_shared2_div2",
			       "dout_pll_shared3_div2", "mout_pll_spare" };
PNAME(mout_cmu_cmuref_p) = { "mout_cmu_top_boost_option1", "dout_cmu_cmuref" };

static const struct samsung_pll_clock top_pll_clks[] __initconst = {
	PLL(pll_0517x, CLK_FOUT_LF_MIF_PLL, "fout_lf_mif_pll", "oscclk",
	    TOP_PLL_LOCKTIME_PLL_LF_MIF, TOP_PLL_CON3_PLL_LF_MIF, NULL),
	PLL(pll_0517x, CLK_FOUT_SHARED0_PLL, "fout_shared0_pll", "oscclk",
	    TOP_PLL_LOCKTIME_PLL_SHARED0, TOP_PLL_CON3_PLL_SHARED0, NULL),
	PLL(pll_0517x, CLK_FOUT_SHARED1_PLL, "fout_shared1_pll", "oscclk",
	    TOP_PLL_LOCKTIME_PLL_SHARED1, TOP_PLL_CON3_PLL_SHARED1, NULL),
	PLL(pll_0518x, CLK_FOUT_SHARED2_PLL, "fout_shared2_pll", "oscclk",
	    TOP_PLL_LOCKTIME_PLL_SHARED2, TOP_PLL_CON3_PLL_SHARED2, NULL),
	PLL(pll_0518x, CLK_FOUT_SHARED3_PLL, "fout_shared3_pll", "oscclk",
	    TOP_PLL_LOCKTIME_PLL_SHARED3, TOP_PLL_CON3_PLL_SHARED3, NULL),
	PLL(pll_0518x, CLK_FOUT_SPARE_PLL, "fout_spare_pll", "oscclk",
	    TOP_PLL_LOCKTIME_PLL_SPARE, TOP_PLL_CON3_PLL_SPARE, NULL),
};

static const struct samsung_mux_clock top_mux_clks[] __initconst = {
	MUX(CLK_MOUT_PLL_LF_MIF, "mout_pll_lf_mif", mout_pll_lf_mif_p,
	    TOP_PLL_CON0_PLL_LF_MIF, 4, 1),
	MUX(CLK_MOUT_PLL_SHARED0, "mout_pll_shared0", mout_pll_shared0_p,
	    TOP_PLL_CON0_PLL_SHARED0, 4, 1),
	MUX(CLK_MOUT_PLL_SHARED1, "mout_pll_shared1", mout_pll_shared1_p,
	    TOP_PLL_CON0_PLL_SHARED1, 4, 1),
	MUX(CLK_MOUT_PLL_SHARED2, "mout_pll_shared2", mout_pll_shared2_p,
	    TOP_PLL_CON0_PLL_SHARED2, 4, 1),
	MUX(CLK_MOUT_PLL_SHARED3, "mout_pll_shared3", mout_pll_shared3_p,
	    TOP_PLL_CON0_PLL_SHARED3, 4, 1),
	MUX(CLK_MOUT_PLL_SPARE, "mout_pll_spare", mout_pll_spare_p,
	    TOP_PLL_CON0_PLL_SPARE, 4, 1),
	MUX(CLK_MOUT_CMU_AUR_AUR, "mout_cmu_aur_aur", mout_cmu_aur_aur_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_AUR_AUR, 0, 3),
	MUX(CLK_MOUT_CMU_AUR_AURCTL, "mout_cmu_aur_aurctl",
	    mout_cmu_aur_aurctl_p, TOP_CLK_CON_MUX_MUX_CLKCMU_AUR_AURCTL, 0, 3),
	MUX(CLK_MOUT_CMU_AUR_NOC, "mout_cmu_aur_noc", mout_cmu_aur_noc_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_AUR_NOC, 0, 3),
	MUX(CLK_MOUT_CMU_BO_NOC, "mout_cmu_bo_noc", mout_cmu_bo_noc_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_BO_NOC, 0, 3),
	MUX(CLK_MOUT_CMU_CIS_CLK0, "mout_cmu_cis_clk0", mout_cmu_cis_clk0_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_CIS_CLK0, 0, 3),
	MUX(CLK_MOUT_CMU_CIS_CLK1, "mout_cmu_cis_clk1", mout_cmu_cis_clk1_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_CIS_CLK1, 0, 3),
	MUX(CLK_MOUT_CMU_CIS_CLK2, "mout_cmu_cis_clk2", mout_cmu_cis_clk2_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_CIS_CLK2, 0, 3),
	MUX(CLK_MOUT_CMU_CIS_CLK3, "mout_cmu_cis_clk3", mout_cmu_cis_clk3_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_CIS_CLK3, 0, 3),
	MUX(CLK_MOUT_CMU_CIS_CLK4, "mout_cmu_cis_clk4", mout_cmu_cis_clk4_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_CIS_CLK4, 0, 3),
	MUX(CLK_MOUT_CMU_CIS_CLK5, "mout_cmu_cis_clk5", mout_cmu_cis_clk5_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_CIS_CLK5, 0, 3),
	MUX(CLK_MOUT_CMU_CIS_CLK6, "mout_cmu_cis_clk6", mout_cmu_cis_clk6_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_CIS_CLK6, 0, 3),
	MUX(CLK_MOUT_CMU_CIS_CLK7, "mout_cmu_cis_clk7", mout_cmu_cis_clk7_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_CIS_CLK7, 0, 3),
	MUX(CLK_MOUT_CMU_CMU_BOOST, "mout_cmu_cmu_boost", mout_cmu_cmu_boost_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_CMU_BOOST, 0, 2),
	MUX(CLK_MOUT_CMU_CMU_BOOST_OPTION1, "mout_cmu_cmu_boost_option1",
	    mout_cmu_cmu_boost_option1_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_CMU_BOOST_OPTION1, 0, 1),
	MUX(CLK_MOUT_CMU_CPUCL0_DBG, "mout_cmu_cpucl0_dbg",
	    mout_cmu_cpucl0_dbg_p, TOP_CLK_CON_MUX_MUX_CLKCMU_CPUCL0_DBG, 0, 3),
	MUX(CLK_MOUT_CMU_CPUCL0_SWITCH, "mout_cmu_cpucl0_switch",
	    mout_cmu_cpucl0_switch_p, TOP_CLK_CON_MUX_MUX_CLKCMU_CPUCL0_SWITCH,
	    0, 3),
	MUX(CLK_MOUT_CMU_CPUCL1_SWITCH, "mout_cmu_cpucl1_switch",
	    mout_cmu_cpucl1_switch_p, TOP_CLK_CON_MUX_MUX_CLKCMU_CPUCL1_SWITCH,
	    0, 3),
	MUX(CLK_MOUT_CMU_CPUCL2_SWITCH, "mout_cmu_cpucl2_switch",
	    mout_cmu_cpucl2_switch_p, TOP_CLK_CON_MUX_MUX_CLKCMU_CPUCL2_SWITCH,
	    0, 3),
	MUX(CLK_MOUT_CMU_CSIS_NOC, "mout_cmu_csis_noc", mout_cmu_csis_noc_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_CSIS_NOC, 0, 3),
	MUX(CLK_MOUT_CMU_DISP_NOC, "mout_cmu_disp_noc", mout_cmu_disp_noc_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_DISP_NOC, 0, 3),
	MUX(CLK_MOUT_CMU_DNS_NOC, "mout_cmu_dns_noc", mout_cmu_dns_noc_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_DNS_NOC, 0, 3),
	MUX(CLK_MOUT_CMU_DPU_NOC, "mout_cmu_dpu_noc", mout_cmu_dpu_noc_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_DPU_NOC, 0, 3),
	MUX(CLK_MOUT_CMU_EH_NOC, "mout_cmu_eh_noc", mout_cmu_eh_noc_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_EH_NOC, 0, 3),
	MUX(CLK_MOUT_CMU_G2D_G2D, "mout_cmu_g2d_g2d", mout_cmu_g2d_g2d_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_G2D_G2D, 0, 3),
	MUX(CLK_MOUT_CMU_G2D_MSCL, "mout_cmu_g2d_mscl", mout_cmu_g2d_mscl_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_G2D_MSCL, 0, 3),
	MUX(CLK_MOUT_CMU_G3AA_G3AA, "mout_cmu_g3aa_g3aa", mout_cmu_g3aa_g3aa_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_G3AA_G3AA, 0, 3),
	MUX(CLK_MOUT_CMU_G3D_GLB, "mout_cmu_g3d_glb", mout_cmu_g3d_glb_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_G3D_GLB, 0, 3),
	MUX(CLK_MOUT_CMU_G3D_NOCD, "mout_cmu_g3d_nocd", mout_cmu_g3d_nocd_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_G3D_NOCD, 0, 3),
	MUX(CLK_MOUT_CMU_G3D_SWITCH, "mout_cmu_g3d_switch",
	    mout_cmu_g3d_switch_p, TOP_CLK_CON_MUX_MUX_CLKCMU_G3D_SWITCH, 0, 3),
	MUX(CLK_MOUT_CMU_GDC_GDC0, "mout_cmu_gdc_gdc0", mout_cmu_gdc_gdc0_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_GDC_GDC0, 0, 3),
	MUX(CLK_MOUT_CMU_GDC_GDC1, "mout_cmu_gdc_gdc1", mout_cmu_gdc_gdc1_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_GDC_GDC1, 0, 3),
	MUX(CLK_MOUT_CMU_GDC_SCSC, "mout_cmu_gdc_scsc", mout_cmu_gdc_scsc_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_GDC_SCSC, 0, 3),
	MUX(CLK_MOUT_CMU_HPM, "mout_cmu_hpm", mout_cmu_hpm_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_HPM, 0, 2),
	MUX(CLK_MOUT_CMU_HSI0_DPGTC, "mout_cmu_hsi0_dpgtc",
	    mout_cmu_hsi0_dpgtc_p, TOP_CLK_CON_MUX_MUX_CLKCMU_HSI0_DPGTC, 0, 2),
	MUX(CLK_MOUT_CMU_HSI0_NOC, "mout_cmu_hsi0_noc", mout_cmu_hsi0_noc_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_HSI0_NOC, 0, 3),
	MUX(CLK_MOUT_CMU_HSI0_USB31DRD, "mout_cmu_hsi0_usb31drd",
	    mout_cmu_hsi0_usb31drd_p, TOP_CLK_CON_MUX_MUX_CLKCMU_HSI0_USB31DRD,
	    0, 1),
	MUX(CLK_MOUT_CMU_HSI0_USBDPDBG, "mout_cmu_hsi0_usbdpdbg",
	    mout_cmu_hsi0_usbdpdbg_p, TOP_CLK_CON_MUX_MUX_CLKCMU_HSI0_USBDPDBG,
	    0, 1),
	MUX(CLK_MOUT_CMU_HSI1_NOC, "mout_cmu_hsi1_noc", mout_cmu_hsi1_noc_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_HSI1_NOC, 0, 3),
	MUX(CLK_MOUT_CMU_HSI1_PCIE, "mout_cmu_hsi1_pcie", mout_cmu_hsi1_pcie_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_HSI1_PCIE, 0, 1),
	MUX(CLK_MOUT_CMU_HSI2_MMC_CARD, "mout_cmu_hsi2_mmc_card",
	    mout_cmu_hsi2_mmc_card_p, TOP_CLK_CON_MUX_MUX_CLKCMU_HSI2_MMC_CARD,
	    0, 2),
	MUX(CLK_MOUT_CMU_HSI2_NOC, "mout_cmu_hsi2_noc", mout_cmu_hsi2_noc_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_HSI2_NOC, 0, 3),
	MUX(CLK_MOUT_CMU_HSI2_PCIE, "mout_cmu_hsi2_pcie", mout_cmu_hsi2_pcie_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_HSI2_PCIE, 0, 1),
	MUX(CLK_MOUT_CMU_HSI2_UFS_EMBD, "mout_cmu_hsi2_ufs_embd",
	    mout_cmu_hsi2_ufs_embd_p, TOP_CLK_CON_MUX_MUX_CLKCMU_HSI2_UFS_EMBD,
	    0, 2),
	MUX(CLK_MOUT_CMU_IPP_NOC, "mout_cmu_ipp_noc", mout_cmu_ipp_noc_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_IPP_NOC, 0, 3),
	MUX(CLK_MOUT_CMU_ITP_NOC, "mout_cmu_itp_noc", mout_cmu_itp_noc_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_ITP_NOC, 0, 3),
	MUX(CLK_MOUT_CMU_MCSC_ITSC, "mout_cmu_mcsc_itsc", mout_cmu_mcsc_itsc_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_MCSC_ITSC, 0, 3),
	MUX(CLK_MOUT_CMU_MCSC_MCSC, "mout_cmu_mcsc_mcsc", mout_cmu_mcsc_mcsc_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_MCSC_MCSC, 0, 3),
	MUX(CLK_MOUT_CMU_MFC_MFC, "mout_cmu_mfc_mfc", mout_cmu_mfc_mfc_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_MFC_MFC, 0, 3),
	MUX(CLK_MOUT_CMU_MIF_NOCP, "mout_cmu_mif_nocp", mout_cmu_mif_nocp_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_MIF_NOCP, 0, 2),
	MUX(CLK_MOUT_CMU_MIF_SWITCH, "mout_cmu_mif_switch",
	    mout_cmu_mif_switch_p, TOP_CLK_CON_MUX_MUX_CLKCMU_MIF_SWITCH, 0, 3),
	MUX(CLK_MOUT_CMU_MISC_NOC, "mout_cmu_misc_noc", mout_cmu_misc_noc_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_MISC_NOC, 0, 2),
	MUX(CLK_MOUT_CMU_MISC_SSS, "mout_cmu_misc_sss", mout_cmu_misc_sss_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_MISC_SSS, 0, 2),
	MUX(CLK_MOUT_CMU_NOCL0_NOC, "mout_cmu_nocl0_noc", mout_cmu_nocl0_noc_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_NOCL0_NOC, 0, 3),
	MUX(CLK_MOUT_CMU_NOCL1A_NOC, "mout_cmu_nocl1a_noc",
	    mout_cmu_nocl1a_noc_p, TOP_CLK_CON_MUX_MUX_CLKCMU_NOCL1A_NOC, 0, 3),
	MUX(CLK_MOUT_CMU_NOCL1B_NOC, "mout_cmu_nocl1b_noc",
	    mout_cmu_nocl1b_noc_p, TOP_CLK_CON_MUX_MUX_CLKCMU_NOCL1B_NOC, 0, 3),
	MUX(CLK_MOUT_CMU_NOCL2A_NOC, "mout_cmu_nocl2a_noc",
	    mout_cmu_nocl2a_noc_p, TOP_CLK_CON_MUX_MUX_CLKCMU_NOCL2A_NOC, 0, 3),
	MUX(CLK_MOUT_CMU_PDP_NOC, "mout_cmu_pdp_noc", mout_cmu_pdp_noc_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_PDP_NOC, 0, 3),
	MUX(CLK_MOUT_CMU_PDP_VRA, "mout_cmu_pdp_vra", mout_cmu_pdp_vra_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_PDP_VRA, 0, 3),
	MUX(CLK_MOUT_CMU_PERIC0_IP, "mout_cmu_peric0_ip", mout_cmu_peric0_ip_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_PERIC0_IP, 0, 2),
	MUX(CLK_MOUT_CMU_PERIC0_NOC, "mout_cmu_peric0_noc",
	    mout_cmu_peric0_noc_p, TOP_CLK_CON_MUX_MUX_CLKCMU_PERIC0_NOC, 0, 2),
	MUX(CLK_MOUT_CMU_PERIC1_IP, "mout_cmu_peric1_ip", mout_cmu_peric1_ip_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_PERIC1_IP, 0, 2),
	MUX(CLK_MOUT_CMU_PERIC1_NOC, "mout_cmu_peric1_noc",
	    mout_cmu_peric1_noc_p, TOP_CLK_CON_MUX_MUX_CLKCMU_PERIC1_NOC, 0, 2),
	MUX(CLK_MOUT_CMU_TNR_NOC, "mout_cmu_tnr_noc", mout_cmu_tnr_noc_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_TNR_NOC, 0, 3),
	MUX(CLK_MOUT_CMU_TOP_BOOST_OPTION1, "mout_cmu_top_boost_option1",
	    mout_cmu_top_boost_option1_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_TOP_BOOST_OPTION1, 0, 1),
	MUX(CLK_MOUT_CMU_TOP_CMUREF, "mout_cmu_top_cmuref",
	    mout_cmu_top_cmuref_p, TOP_CLK_CON_MUX_MUX_CLKCMU_TOP_CMUREF, 0, 2),
	MUX(CLK_MOUT_CMU_TPU_NOC, "mout_cmu_tpu_noc", mout_cmu_tpu_noc_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_TPU_NOC, 0, 3),
	MUX(CLK_MOUT_CMU_TPU_TPU, "mout_cmu_tpu_tpu", mout_cmu_tpu_tpu_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_TPU_TPU, 0, 3),
	MUX(CLK_MOUT_CMU_TPU_TPUCTL, "mout_cmu_tpu_tpuctl",
	    mout_cmu_tpu_tpuctl_p, TOP_CLK_CON_MUX_MUX_CLKCMU_TPU_TPUCTL, 0, 3),
	MUX(CLK_MOUT_CMU_TPU_UART, "mout_cmu_tpu_uart", mout_cmu_tpu_uart_p,
	    TOP_CLK_CON_MUX_MUX_CLKCMU_TPU_UART, 0, 2),
	MUX(CLK_MOUT_CMU_CMUREF, "mout_cmu_cmuref", mout_cmu_cmuref_p,
	    TOP_CLK_CON_MUX_MUX_CMU_CMUREF, 0, 1),
};

static const struct samsung_div_clock top_div_clks[] __initconst = {
	DIV(CLK_DOUT_CMU_AUR_AUR, "dout_cmu_aur_aur", "gout_cmu_aur_aur",
	    TOP_CLK_CON_DIV_CLKCMU_AUR_AUR, 0, 4),
	DIV(CLK_DOUT_CMU_AUR_AURCTL, "dout_cmu_aur_aurctl",
	    "gout_cmu_aur_aurctl", TOP_CLK_CON_DIV_CLKCMU_AUR_AURCTL, 0, 4),
	DIV(CLK_DOUT_CMU_AUR_NOC, "dout_cmu_aur_noc", "gout_cmu_aur_noc",
	    TOP_CLK_CON_DIV_CLKCMU_AUR_NOC, 0, 4),
	DIV(CLK_DOUT_CMU_BO_NOC, "dout_cmu_bo_noc", "gout_cmu_bo_noc",
	    TOP_CLK_CON_DIV_CLKCMU_BO_NOC, 0, 4),
	DIV(CLK_DOUT_CMU_CIS_CLK0, "dout_cmu_cis_clk0", "gout_cmu_cis_clk0",
	    TOP_CLK_CON_DIV_CLKCMU_CIS_CLK0, 0, 5),
	DIV(CLK_DOUT_CMU_CIS_CLK1, "dout_cmu_cis_clk1", "gout_cmu_cis_clk1",
	    TOP_CLK_CON_DIV_CLKCMU_CIS_CLK1, 0, 5),
	DIV(CLK_DOUT_CMU_CIS_CLK2, "dout_cmu_cis_clk2", "gout_cmu_cis_clk2",
	    TOP_CLK_CON_DIV_CLKCMU_CIS_CLK2, 0, 5),
	DIV(CLK_DOUT_CMU_CIS_CLK3, "dout_cmu_cis_clk3", "gout_cmu_cis_clk3",
	    TOP_CLK_CON_DIV_CLKCMU_CIS_CLK3, 0, 5),
	DIV(CLK_DOUT_CMU_CIS_CLK4, "dout_cmu_cis_clk4", "gout_cmu_cis_clk4",
	    TOP_CLK_CON_DIV_CLKCMU_CIS_CLK4, 0, 5),
	DIV(CLK_DOUT_CMU_CIS_CLK5, "dout_cmu_cis_clk5", "gout_cmu_cis_clk5",
	    TOP_CLK_CON_DIV_CLKCMU_CIS_CLK5, 0, 5),
	DIV(CLK_DOUT_CMU_CIS_CLK6, "dout_cmu_cis_clk6", "gout_cmu_cis_clk6",
	    TOP_CLK_CON_DIV_CLKCMU_CIS_CLK6, 0, 5),
	DIV(CLK_DOUT_CMU_CIS_CLK7, "dout_cmu_cis_clk7", "gout_cmu_cis_clk7",
	    TOP_CLK_CON_DIV_CLKCMU_CIS_CLK7, 0, 5),
	DIV(CLK_DOUT_CMU_CPUCL0_DBG, "dout_cmu_cpucl0_dbg",
	    "gout_cmu_cpucl0_dbg_noc", TOP_CLK_CON_DIV_CLKCMU_CPUCL0_DBG, 0, 4),
	DIV(CLK_DOUT_CMU_CPUCL0_SWITCH, "dout_cmu_cpucl0_switch",
	    "gout_cmu_cpucl0_switch", TOP_CLK_CON_DIV_CLKCMU_CPUCL0_SWITCH, 0,
	    3),
	DIV(CLK_DOUT_CMU_CPUCL1_SWITCH, "dout_cmu_cpucl1_switch",
	    "gout_cmu_cpucl1_switch", TOP_CLK_CON_DIV_CLKCMU_CPUCL1_SWITCH, 0,
	    3),
	DIV(CLK_DOUT_CMU_CPUCL2_SWITCH, "dout_cmu_cpucl2_switch",
	    "gout_cmu_cpucl2_switch", TOP_CLK_CON_DIV_CLKCMU_CPUCL2_SWITCH, 0,
	    3),
	DIV(CLK_DOUT_CMU_CSIS_NOC, "dout_cmu_csis_noc", "gout_cmu_csis_noc",
	    TOP_CLK_CON_DIV_CLKCMU_CSIS_NOC, 0, 4),
	DIV(CLK_DOUT_CMU_DISP_NOC, "dout_cmu_disp_noc", "gout_cmu_disp_noc",
	    TOP_CLK_CON_DIV_CLKCMU_DISP_NOC, 0, 4),
	DIV(CLK_DOUT_CMU_DNS_NOC, "dout_cmu_dns_noc", "gout_cmu_dns_noc",
	    TOP_CLK_CON_DIV_CLKCMU_DNS_NOC, 0, 4),
	DIV(CLK_DOUT_CMU_DPU_NOC, "dout_cmu_dpu_noc", "gout_cmu_dpu_noc",
	    TOP_CLK_CON_DIV_CLKCMU_DPU_NOC, 0, 4),
	DIV(CLK_DOUT_CMU_EH_NOC, "dout_cmu_eh_noc", "gout_cmu_eh_noc",
	    TOP_CLK_CON_DIV_CLKCMU_EH_NOC, 0, 4),
	DIV(CLK_DOUT_CMU_G2D_G2D, "dout_cmu_g2d_g2d", "gout_cmu_g2d_g2d",
	    TOP_CLK_CON_DIV_CLKCMU_G2D_G2D, 0, 4),
	DIV(CLK_DOUT_CMU_G2D_MSCL, "dout_cmu_g2d_mscl", "gout_cmu_g2d_mscl",
	    TOP_CLK_CON_DIV_CLKCMU_G2D_MSCL, 0, 4),
	DIV(CLK_DOUT_CMU_G3AA_G3AA, "dout_cmu_g3aa_g3aa", "gout_cmu_g3aa_g3aa",
	    TOP_CLK_CON_DIV_CLKCMU_G3AA_G3AA, 0, 4),
	DIV(CLK_DOUT_CMU_G3D_GLB, "dout_cmu_g3d_glb", "gout_cmu_g3d_glb",
	    TOP_CLK_CON_DIV_CLKCMU_G3D_GLB, 0, 4),
	DIV(CLK_DOUT_CMU_G3D_NOCD, "dout_cmu_g3d_nocd", "gout_cmu_g3d_nocd",
	    TOP_CLK_CON_DIV_CLKCMU_G3D_NOCD, 0, 4),
	DIV(CLK_DOUT_CMU_G3D_SWITCH, "dout_cmu_g3d_switch",
	    "gout_cmu_g3d_switch", TOP_CLK_CON_DIV_CLKCMU_G3D_SWITCH, 0, 3),
	DIV(CLK_DOUT_CMU_GDC_GDC0, "dout_cmu_gdc_gdc0", "gout_cmu_gdc_gdc0",
	    TOP_CLK_CON_DIV_CLKCMU_GDC_GDC0, 0, 4),
	DIV(CLK_DOUT_CMU_GDC_GDC1, "dout_cmu_gdc_gdc1", "gout_cmu_gdc_gdc1",
	    TOP_CLK_CON_DIV_CLKCMU_GDC_GDC1, 0, 4),
	DIV(CLK_DOUT_CMU_GDC_SCSC, "dout_cmu_gdc_scsc", "gout_cmu_gdc_scsc",
	    TOP_CLK_CON_DIV_CLKCMU_GDC_SCSC, 0, 4),
	DIV(CLK_DOUT_CMU_HPM, "dout_cmu_hpm", "gout_cmu_hpm",
	    TOP_CLK_CON_DIV_CLKCMU_HPM, 0, 2),
	DIV(CLK_DOUT_CMU_HSI0_DPGTC, "dout_cmu_hsi0_dpgtc",
	    "gout_cmu_hsi0_dpgtc", TOP_CLK_CON_DIV_CLKCMU_HSI0_DPGTC, 0, 4),
	DIV(CLK_DOUT_CMU_HSI0_NOC, "dout_cmu_hsi0_noc", "gout_cmu_hsi0_noc",
	    TOP_CLK_CON_DIV_CLKCMU_HSI0_NOC, 0, 4),
	DIV(CLK_DOUT_CMU_HSI0_USB31DRD, "dout_cmu_hsi0_usb31drd",
	    "gout_cmu_hsi0_usb31drd", TOP_CLK_CON_DIV_CLKCMU_HSI0_USB31DRD, 0,
	    5),
	DIV(CLK_DOUT_CMU_HSI1_NOC, "dout_cmu_hsi1_noc", "gout_cmu_hsi1_noc",
	    TOP_CLK_CON_DIV_CLKCMU_HSI1_NOC, 0, 4),
	DIV(CLK_DOUT_CMU_HSI1_PCIE, "dout_cmu_hsi1_pcie", "gout_cmu_hsi1_pcie",
	    TOP_CLK_CON_DIV_CLKCMU_HSI1_PCIE, 0, 3),
	DIV(CLK_DOUT_CMU_HSI2_MMC_CARD, "dout_cmu_hsi2_mmc_card",
	    "gout_cmu_hsi2_mmccard", TOP_CLK_CON_DIV_CLKCMU_HSI2_MMC_CARD, 0,
	    9),
	DIV(CLK_DOUT_CMU_HSI2_NOC, "dout_cmu_hsi2_noc", "gout_cmu_hsi2_noc",
	    TOP_CLK_CON_DIV_CLKCMU_HSI2_NOC, 0, 4),
	DIV(CLK_DOUT_CMU_HSI2_PCIE, "dout_cmu_hsi2_pcie", "gout_cmu_hsi2_pcie",
	    TOP_CLK_CON_DIV_CLKCMU_HSI2_PCIE, 0, 3),
	DIV(CLK_DOUT_CMU_HSI2_UFS_EMBD, "dout_cmu_hsi2_ufs_embd",
	    "gout_cmu_hsi2_ufs_embd", TOP_CLK_CON_DIV_CLKCMU_HSI2_UFS_EMBD, 0,
	    4),
	DIV(CLK_DOUT_CMU_IPP_NOC, "dout_cmu_ipp_noc", "gout_cmu_ipp_noc",
	    TOP_CLK_CON_DIV_CLKCMU_IPP_NOC, 0, 4),
	DIV(CLK_DOUT_CMU_ITP_NOC, "dout_cmu_itp_noc", "gout_cmu_itp_noc",
	    TOP_CLK_CON_DIV_CLKCMU_ITP_NOC, 0, 4),
	DIV(CLK_DOUT_CMU_MCSC_ITSC, "dout_cmu_mcsc_itsc", "gout_cmu_mcsc_itsc",
	    TOP_CLK_CON_DIV_CLKCMU_MCSC_ITSC, 0, 4),
	DIV(CLK_DOUT_CMU_MCSC_MCSC, "dout_cmu_mcsc_mcsc", "gout_cmu_mcsc_mcsc",
	    TOP_CLK_CON_DIV_CLKCMU_MCSC_MCSC, 0, 4),
	DIV(CLK_DOUT_CMU_MFC_MFC, "dout_cmu_mfc_mfc", "gout_cmu_mfc_mfc",
	    TOP_CLK_CON_DIV_CLKCMU_MFC_MFC, 0, 4),
	DIV(CLK_DOUT_CMU_MIF_NOCP, "dout_cmu_mif_nocp", "gout_cmu_mif_nocp",
	    TOP_CLK_CON_DIV_CLKCMU_MIF_NOCP, 0, 4),
	DIV(CLK_DOUT_CMU_MISC_NOC, "dout_cmu_misc_noc", "gout_cmu_misc_noc",
	    TOP_CLK_CON_DIV_CLKCMU_MISC_NOC, 0, 4),
	DIV(CLK_DOUT_CMU_MISC_SSS, "dout_cmu_misc_sss", "gout_cmu_misc_sss",
	    TOP_CLK_CON_DIV_CLKCMU_MISC_SSS, 0, 4),
	DIV(CLK_DOUT_CMU_NOCL0_NOC, "dout_cmu_nocl0_noc", "gout_cmu_nocl0_noc",
	    TOP_CLK_CON_DIV_CLKCMU_NOCL0_NOC, 0, 4),
	DIV(CLK_DOUT_CMU_NOCL1A_NOC, "dout_cmu_nocl1a_noc",
	    "gout_cmu_nocl1a_noc", TOP_CLK_CON_DIV_CLKCMU_NOCL1A_NOC, 0, 4),
	DIV(CLK_DOUT_CMU_NOCL1B_NOC, "dout_cmu_nocl1b_noc",
	    "gout_cmu_nocl1b_noc", TOP_CLK_CON_DIV_CLKCMU_NOCL1B_NOC, 0, 4),
	DIV(CLK_DOUT_CMU_NOCL2A_NOC, "dout_cmu_nocl2a_noc",
	    "gout_cmu_nocl2a_noc", TOP_CLK_CON_DIV_CLKCMU_NOCL2A_NOC, 0, 4),
	DIV(CLK_DOUT_CMU_PDP_NOC, "dout_cmu_pdp_noc", "gout_cmu_pdp_noc",
	    TOP_CLK_CON_DIV_CLKCMU_PDP_NOC, 0, 4),
	DIV(CLK_DOUT_CMU_PDP_VRA, "dout_cmu_pdp_vra", "gout_cmu_pdp_vra",
	    TOP_CLK_CON_DIV_CLKCMU_PDP_VRA, 0, 4),
	DIV(CLK_DOUT_CMU_PERIC0_IP, "dout_cmu_peric0_ip", "gout_cmu_peric0_ip",
	    TOP_CLK_CON_DIV_CLKCMU_PERIC0_IP, 0, 4),
	DIV(CLK_DOUT_CMU_PERIC0_NOC, "dout_cmu_peric0_noc",
	    "gout_cmu_peric0_noc", TOP_CLK_CON_DIV_CLKCMU_PERIC0_NOC, 0, 4),
	DIV(CLK_DOUT_CMU_PERIC1_IP, "dout_cmu_peric1_ip", "gout_cmu_peric1_ip",
	    TOP_CLK_CON_DIV_CLKCMU_PERIC1_IP, 0, 4),
	DIV(CLK_DOUT_CMU_PERIC1_NOC, "dout_cmu_peric1_noc",
	    "gout_cmu_peric1_noc", TOP_CLK_CON_DIV_CLKCMU_PERIC1_NOC, 0, 4),
	DIV(CLK_DOUT_CMU_TNR_NOC, "dout_cmu_tnr_noc", "gout_cmu_tnr_noc",
	    TOP_CLK_CON_DIV_CLKCMU_TNR_NOC, 0, 4),
	DIV(CLK_DOUT_CMU_TPU_NOC, "dout_cmu_tpu_noc", "gout_cmu_tpu_noc",
	    TOP_CLK_CON_DIV_CLKCMU_TPU_NOC, 0, 4),
	DIV(CLK_DOUT_CMU_TPU_TPU, "dout_cmu_tpu_tpu", "gout_cmu_tpu_tpu",
	    TOP_CLK_CON_DIV_CLKCMU_TPU_TPU, 0, 4),
	DIV(CLK_DOUT_CMU_TPU_TPUCTL, "dout_cmu_tpu_tpuctl",
	    "gout_cmu_tpu_tpuctl", TOP_CLK_CON_DIV_CLKCMU_TPU_TPUCTL, 0, 4),
	DIV(CLK_DOUT_CMU_TPU_UART, "dout_cmu_tpu_uart", "gout_cmu_tpu_uart",
	    TOP_CLK_CON_DIV_CLKCMU_TPU_UART, 0, 4),
	DIV(CLK_DOUT_CMU_CMU_BOOST, "dout_cmu_cmu_boost", "gout_cmu_cmu_boost",
	    TOP_CLK_CON_DIV_DIV_CLKCMU_CMU_BOOST, 0, 2),
	DIV(CLK_DOUT_CMU_CMUREF, "dout_cmu_cmuref", "gout_cmu_top_cmuref",
	    TOP_CLK_CON_DIV_DIV_CLK_CMU_CMUREF, 0, 2),
	DIV(CLK_DOUT_PLL_SHARED0_DIV2, "dout_pll_shared0_div2",
	    "mout_pll_shared0", TOP_CLK_CON_DIV_PLL_SHARED0_DIV2, 0, 1),
	DIV(CLK_DOUT_PLL_SHARED0_DIV3, "dout_pll_shared0_div3",
	    "mout_pll_shared0", TOP_CLK_CON_DIV_PLL_SHARED0_DIV3, 0, 2),
	DIV(CLK_DOUT_PLL_SHARED0_DIV4, "dout_pll_shared0_div4",
	    "dout_pll_shared0_div2", TOP_CLK_CON_DIV_PLL_SHARED0_DIV4, 0, 1),
	DIV(CLK_DOUT_PLL_SHARED0_DIV5, "dout_pll_shared0_div5",
	    "mout_pll_shared0", TOP_CLK_CON_DIV_PLL_SHARED0_DIV5, 0, 3),
	DIV(CLK_DOUT_PLL_SHARED1_DIV2, "dout_pll_shared1_div2",
	    "mout_pll_shared1", TOP_CLK_CON_DIV_PLL_SHARED1_DIV2, 0, 1),
	DIV(CLK_DOUT_PLL_SHARED1_DIV3, "dout_pll_shared1_div3",
	    "mout_pll_shared1", TOP_CLK_CON_DIV_PLL_SHARED1_DIV3, 0, 2),
	DIV(CLK_DOUT_PLL_SHARED1_DIV4, "dout_pll_shared1_div4",
	    "dout_pll_shared1_div2", TOP_CLK_CON_DIV_PLL_SHARED1_DIV4, 0, 1),
	DIV(CLK_DOUT_PLL_SHARED2_DIV2, "dout_pll_shared2_div2",
	    "mout_pll_shared2", TOP_CLK_CON_DIV_PLL_SHARED2_DIV2, 0, 1),
	DIV(CLK_DOUT_PLL_SHARED3_DIV2, "dout_pll_shared3_div2",
	    "mout_pll_shared3", TOP_CLK_CON_DIV_PLL_SHARED3_DIV2, 0, 1),
};

static const struct samsung_fixed_factor_clock top_ffactor_clks[] __initconst = {
	FFACTOR(CLK_DOUT_CMU_HSI0_USBDPDBG, "dout_cmu_hsi0_usbdpdbg",
		"gout_cmu_hsi0_usbdpdbg", 1, 4, 0),
	FFACTOR(CLK_DOUT_CMU_OTP, "dout_cmu_otp", "oscclk", 1, 8, 0),
};

static const struct samsung_gate_clock top_gate_clks[] __initconst = {
	GATE(CLK_GOUT_CMU_CPUCL0_BOOST, "gout_cmu_cpucl0_boost",
	     "mout_cmu_cmu_boost_option1", TOP_CLK_CON_GAT_CLKCMU_CPUCL0_BOOST,
	     21, 0, 0),
	GATE(CLK_GOUT_CMU_CPUCL1_BOOST, "gout_cmu_cpucl1_boost",
	     "mout_cmu_cmu_boost_option1", TOP_CLK_CON_GAT_CLKCMU_CPUCL1_BOOST,
	     21, 0, 0),
	GATE(CLK_GOUT_CMU_CPUCL2_BOOST, "gout_cmu_cpucl2_boost",
	     "mout_cmu_cmu_boost_option1", TOP_CLK_CON_GAT_CLKCMU_CPUCL2_BOOST,
	     21, 0, 0),
	GATE(CLK_GOUT_CMU_MIF_BOOST, "gout_cmu_mif_boost",
	     "mout_cmu_cmu_boost_option1", TOP_CLK_CON_GAT_CLKCMU_MIF_BOOST, 21,
	     0, 0),
	GATE(CLK_GOUT_CMU_MIF_SWITCH, "gout_cmu_mif_switch",
	     "mout_cmu_mif_switch", TOP_CLK_CON_GAT_CLKCMU_MIF_SWITCH, 21, 0,
	     0),
	GATE(CLK_GOUT_CMU_NOCL0_BOOST, "gout_cmu_nocl0_boost",
	     "mout_cmu_cmu_boost_option1", TOP_CLK_CON_GAT_CLKCMU_NOCL0_BOOST,
	     21, 0, 0),
	GATE(CLK_GOUT_CMU_NOCL1A_BOOST, "gout_cmu_nocl1a_boost",
	     "mout_cmu_cmu_boost_option1", TOP_CLK_CON_GAT_CLKCMU_NOCL1A_BOOST,
	     21, 0, 0),
	GATE(CLK_GOUT_CMU_NOCL1B_BOOST, "gout_cmu_nocl1b_boost",
	     "mout_cmu_cmu_boost_option1", TOP_CLK_CON_GAT_CLKCMU_NOCL1B_BOOST,
	     21, 0, 0),
	GATE(CLK_GOUT_CMU_NOCL2A_BOOST, "gout_cmu_nocl2a_boost",
	     "mout_cmu_cmu_boost_option1", TOP_CLK_CON_GAT_CLKCMU_NOCL2A_BOOST,
	     21, 0, 0),
	GATE(CLK_GOUT_CMU_AUR_AUR, "gout_cmu_aur_aur", "mout_cmu_aur_aur",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_AUR_AUR, 21, 0, 0),
	GATE(CLK_GOUT_CMU_AUR_AURCTL, "gout_cmu_aur_aurctl",
	     "mout_cmu_aur_aurctl", TOP_CLK_CON_GAT_GATE_CLKCMU_AUR_AURCTL, 21,
	     0, 0),
	GATE(CLK_GOUT_CMU_AUR_NOC, "gout_cmu_aur_noc", "mout_cmu_aur_noc",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_AUR_NOC, 21, 0, 0),
	GATE(CLK_GOUT_CMU_BO_NOC, "gout_cmu_bo_noc", "mout_cmu_bo_noc",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_BO_NOC, 21, 0, 0),
	GATE(CLK_GOUT_CMU_CIS_CLK0, "gout_cmu_cis_clk0", "mout_cmu_cis_clk0",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_CIS_CLK0, 21, 0, 0),
	GATE(CLK_GOUT_CMU_CIS_CLK1, "gout_cmu_cis_clk1", "mout_cmu_cis_clk1",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_CIS_CLK1, 21, 0, 0),
	GATE(CLK_GOUT_CMU_CIS_CLK2, "gout_cmu_cis_clk2", "mout_cmu_cis_clk2",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_CIS_CLK2, 21, 0, 0),
	GATE(CLK_GOUT_CMU_CIS_CLK3, "gout_cmu_cis_clk3", "mout_cmu_cis_clk3",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_CIS_CLK3, 21, 0, 0),
	GATE(CLK_GOUT_CMU_CIS_CLK4, "gout_cmu_cis_clk4", "mout_cmu_cis_clk4",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_CIS_CLK4, 21, 0, 0),
	GATE(CLK_GOUT_CMU_CIS_CLK5, "gout_cmu_cis_clk5", "mout_cmu_cis_clk5",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_CIS_CLK5, 21, 0, 0),
	GATE(CLK_GOUT_CMU_CIS_CLK6, "gout_cmu_cis_clk6", "mout_cmu_cis_clk6",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_CIS_CLK6, 21, 0, 0),
	GATE(CLK_GOUT_CMU_CIS_CLK7, "gout_cmu_cis_clk7", "mout_cmu_cis_clk7",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_CIS_CLK7, 21, 0, 0),
	GATE(CLK_GOUT_CMU_CMU_BOOST, "gout_cmu_cmu_boost", "mout_cmu_cmu_boost",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_CMU_BOOST, 21, 0, 0),
	GATE(CLK_GOUT_CMU_CPUCL0_DBG_NOC, "gout_cmu_cpucl0_dbg_noc",
	     "mout_cmu_cpucl0_dbg", TOP_CLK_CON_GAT_GATE_CLKCMU_CPUCL0_DBG_NOC,
	     21, 0, 0),
	GATE(CLK_GOUT_CMU_CPUCL0_SWITCH, "gout_cmu_cpucl0_switch",
	     "mout_cmu_cpucl0_switch",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_CPUCL0_SWITCH, 21, 0, 0),
	GATE(CLK_GOUT_CMU_CPUCL1_SWITCH, "gout_cmu_cpucl1_switch",
	     "mout_cmu_cpucl1_switch",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_CPUCL1_SWITCH, 21, 0, 0),
	GATE(CLK_GOUT_CMU_CPUCL2_SWITCH, "gout_cmu_cpucl2_switch",
	     "mout_cmu_cpucl2_switch",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_CPUCL2_SWITCH, 21, 0, 0),
	GATE(CLK_GOUT_CMU_CSIS_NOC, "gout_cmu_csis_noc", "mout_cmu_csis_noc",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_CSIS_NOC, 21, 0, 0),
	GATE(CLK_GOUT_CMU_DISP_NOC, "gout_cmu_disp_noc", "mout_cmu_disp_noc",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_DISP_NOC, 21, 0, 0),
	GATE(CLK_GOUT_CMU_DNS_NOC, "gout_cmu_dns_noc", "mout_cmu_dns_noc",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_DNS_NOC, 21, 0, 0),
	GATE(CLK_GOUT_CMU_DPU_NOC, "gout_cmu_dpu_noc", "mout_cmu_dpu_noc",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_DPU_NOC, 21, 0, 0),
	GATE(CLK_GOUT_CMU_EH_NOC, "gout_cmu_eh_noc", "mout_cmu_eh_noc",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_EH_NOC, 21, 0, 0),
	GATE(CLK_GOUT_CMU_G2D_G2D, "gout_cmu_g2d_g2d", "mout_cmu_g2d_g2d",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_G2D_G2D, 21, 0, 0),
	GATE(CLK_GOUT_CMU_G2D_MSCL, "gout_cmu_g2d_mscl", "mout_cmu_g2d_mscl",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_G2D_MSCL, 21, 0, 0),
	GATE(CLK_GOUT_CMU_G3AA_G3AA, "gout_cmu_g3aa_g3aa", "mout_cmu_g3aa_g3aa",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_G3AA_G3AA, 21, 0, 0),
	GATE(CLK_GOUT_CMU_G3D_GLB, "gout_cmu_g3d_glb", "mout_cmu_g3d_glb",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_G3D_GLB, 21, 0, 0),
	GATE(CLK_GOUT_CMU_G3D_NOCD, "gout_cmu_g3d_nocd", "mout_cmu_g3d_nocd",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_G3D_NOCD, 21, 0, 0),
	GATE(CLK_GOUT_CMU_G3D_SWITCH, "gout_cmu_g3d_switch",
	     "mout_cmu_g3d_switch", TOP_CLK_CON_GAT_GATE_CLKCMU_G3D_SWITCH, 21,
	     0, 0),
	GATE(CLK_GOUT_CMU_GDC_GDC0, "gout_cmu_gdc_gdc0", "mout_cmu_gdc_gdc0",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_GDC_GDC0, 21, 0, 0),
	GATE(CLK_GOUT_CMU_GDC_GDC1, "gout_cmu_gdc_gdc1", "mout_cmu_gdc_gdc1",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_GDC_GDC1, 21, 0, 0),
	GATE(CLK_GOUT_CMU_GDC_SCSC, "gout_cmu_gdc_scsc", "mout_cmu_gdc_scsc",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_GDC_SCSC, 21, 0, 0),
	GATE(CLK_GOUT_CMU_HPM, "gout_cmu_hpm", "mout_cmu_hpm",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_HPM, 21, 0, 0),
	GATE(CLK_GOUT_CMU_HSI0_DPGTC, "gout_cmu_hsi0_dpgtc",
	     "mout_cmu_hsi0_dpgtc", TOP_CLK_CON_GAT_GATE_CLKCMU_HSI0_DPGTC, 21,
	     0, 0),
	GATE(CLK_GOUT_CMU_HSI0_NOC, "gout_cmu_hsi0_noc", "mout_cmu_hsi0_noc",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_HSI0_NOC, 21, 0, 0),
	GATE(CLK_GOUT_CMU_HSI0_USB31DRD, "gout_cmu_hsi0_usb31drd",
	     "mout_cmu_hsi0_usb31drd",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_HSI0_USB31DRD, 21, 0, 0),
	GATE(CLK_GOUT_CMU_HSI0_USBDPDBG, "gout_cmu_hsi0_usbdpdbg",
	     "mout_cmu_hsi0_usbdpdbg",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_HSI0_USBDPDBG, 21, 0, 0),
	GATE(CLK_GOUT_CMU_HSI1_NOC, "gout_cmu_hsi1_noc", "mout_cmu_hsi1_noc",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_HSI1_NOC, 21, 0, 0),
	GATE(CLK_GOUT_CMU_HSI1_PCIE, "gout_cmu_hsi1_pcie", "mout_cmu_hsi1_pcie",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_HSI1_PCIE, 21, 0, 0),
	GATE(CLK_GOUT_CMU_HSI2_MMCCARD, "gout_cmu_hsi2_mmccard",
	     "mout_cmu_hsi2_mmc_card", TOP_CLK_CON_GAT_GATE_CLKCMU_HSI2_MMCCARD,
	     21, 0, 0),
	GATE(CLK_GOUT_CMU_HSI2_NOC, "gout_cmu_hsi2_noc", "mout_cmu_hsi2_noc",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_HSI2_NOC, 21, 0, 0),
	GATE(CLK_GOUT_CMU_HSI2_PCIE, "gout_cmu_hsi2_pcie", "mout_cmu_hsi2_pcie",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_HSI2_PCIE, 21, 0, 0),
	GATE(CLK_GOUT_CMU_HSI2_UFS_EMBD, "gout_cmu_hsi2_ufs_embd",
	     "mout_cmu_hsi2_ufs_embd",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_HSI2_UFS_EMBD, 21, 0, 0),
	GATE(CLK_GOUT_CMU_IPP_NOC, "gout_cmu_ipp_noc", "mout_cmu_ipp_noc",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_IPP_NOC, 21, 0, 0),
	GATE(CLK_GOUT_CMU_ITP_NOC, "gout_cmu_itp_noc", "mout_cmu_itp_noc",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_ITP_NOC, 21, 0, 0),
	GATE(CLK_GOUT_CMU_MCSC_ITSC, "gout_cmu_mcsc_itsc", "mout_cmu_mcsc_itsc",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_MCSC_ITSC, 21, 0, 0),
	GATE(CLK_GOUT_CMU_MCSC_MCSC, "gout_cmu_mcsc_mcsc", "mout_cmu_mcsc_mcsc",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_MCSC_MCSC, 21, 0, 0),
	GATE(CLK_GOUT_CMU_MFC_MFC, "gout_cmu_mfc_mfc", "mout_cmu_mfc_mfc",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_MFC_MFC, 21, 0, 0),
	GATE(CLK_GOUT_CMU_MIF_NOCP, "gout_cmu_mif_nocp", "mout_cmu_mif_nocp",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_MIF_NOCP, 21, 0, 0),
	GATE(CLK_GOUT_CMU_MISC_NOC, "gout_cmu_misc_noc", "mout_cmu_misc_noc",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_MISC_NOC, 21, 0, 0),
	GATE(CLK_GOUT_CMU_MISC_SSS, "gout_cmu_misc_sss", "mout_cmu_misc_sss",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_MISC_SSS, 21, 0, 0),
	GATE(CLK_GOUT_CMU_NOCL0_NOC, "gout_cmu_nocl0_noc", "mout_cmu_nocl0_noc",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_NOCL0_NOC, 21, 0, 0),
	GATE(CLK_GOUT_CMU_NOCL1A_NOC, "gout_cmu_nocl1a_noc",
	     "mout_cmu_nocl1a_noc", TOP_CLK_CON_GAT_GATE_CLKCMU_NOCL1A_NOC, 21,
	     0, 0),
	GATE(CLK_GOUT_CMU_NOCL1B_NOC, "gout_cmu_nocl1b_noc",
	     "mout_cmu_nocl1b_noc", TOP_CLK_CON_GAT_GATE_CLKCMU_NOCL1B_NOC, 21,
	     0, 0),
	GATE(CLK_GOUT_CMU_NOCL2A_NOC, "gout_cmu_nocl2a_noc",
	     "mout_cmu_nocl2a_noc", TOP_CLK_CON_GAT_GATE_CLKCMU_NOCL2A_NOC, 21,
	     0, 0),
	GATE(CLK_GOUT_CMU_PDP_NOC, "gout_cmu_pdp_noc", "mout_cmu_pdp_noc",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_PDP_NOC, 21, 0, 0),
	GATE(CLK_GOUT_CMU_PDP_VRA, "gout_cmu_pdp_vra", "mout_cmu_pdp_vra",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_PDP_VRA, 21, 0, 0),
	GATE(CLK_GOUT_CMU_PERIC0_IP, "gout_cmu_peric0_ip", "mout_cmu_peric0_ip",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_PERIC0_IP, 21, 0, 0),
	GATE(CLK_GOUT_CMU_PERIC0_NOC, "gout_cmu_peric0_noc",
	     "mout_cmu_peric0_noc", TOP_CLK_CON_GAT_GATE_CLKCMU_PERIC0_NOC, 21,
	     0, 0),
	GATE(CLK_GOUT_CMU_PERIC1_IP, "gout_cmu_peric1_ip", "mout_cmu_peric1_ip",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_PERIC1_IP, 21, 0, 0),
	GATE(CLK_GOUT_CMU_PERIC1_NOC, "gout_cmu_peric1_noc",
	     "mout_cmu_peric1_noc", TOP_CLK_CON_GAT_GATE_CLKCMU_PERIC1_NOC, 21,
	     0, 0),
	GATE(CLK_GOUT_CMU_TNR_NOC, "gout_cmu_tnr_noc", "mout_cmu_tnr_noc",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_TNR_NOC, 21, 0, 0),
	GATE(CLK_GOUT_CMU_TOP_CMUREF, "gout_cmu_top_cmuref",
	     "mout_cmu_top_cmuref", TOP_CLK_CON_GAT_GATE_CLKCMU_TOP_CMUREF, 21,
	     0, 0),
	GATE(CLK_GOUT_CMU_TPU_NOC, "gout_cmu_tpu_noc", "mout_cmu_tpu_noc",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_TPU_NOC, 21, 0, 0),
	GATE(CLK_GOUT_CMU_TPU_TPU, "gout_cmu_tpu_tpu", "mout_cmu_tpu_tpu",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_TPU_TPU, 21, 0, 0),
	GATE(CLK_GOUT_CMU_TPU_TPUCTL, "gout_cmu_tpu_tpuctl",
	     "mout_cmu_tpu_tpuctl", TOP_CLK_CON_GAT_GATE_CLKCMU_TPU_TPUCTL, 21,
	     0, 0),
	GATE(CLK_GOUT_CMU_TPU_UART, "gout_cmu_tpu_uart", "mout_cmu_tpu_uart",
	     TOP_CLK_CON_GAT_GATE_CLKCMU_TPU_UART, 21, 0, 0),
};

static const struct samsung_cmu_info top_cmu_info __initconst = {
	.pll_clks = top_pll_clks,
	.nr_pll_clks = ARRAY_SIZE(top_pll_clks),
	.mux_clks = top_mux_clks,
	.nr_mux_clks = ARRAY_SIZE(top_mux_clks),
	.div_clks = top_div_clks,
	.nr_div_clks = ARRAY_SIZE(top_div_clks),
	.fixed_factor_clks = top_ffactor_clks,
	.nr_fixed_factor_clks = ARRAY_SIZE(top_ffactor_clks),
	.gate_clks = top_gate_clks,
	.nr_gate_clks = ARRAY_SIZE(top_gate_clks),
	.nr_clk_ids = CLKS_NR_TOP,
	.clk_regs = top_clk_regs,
	.nr_clk_regs = ARRAY_SIZE(top_clk_regs),
};

/* ---- CMU_MISC ------------------------------------------------------------- */
/* Register offsets at 0x10010000. */
#define MISC_PLL_CON0_MUX_CLKCMU_MISC_NOC_USER 0x0600
#define MISC_PLL_CON0_MUX_CLKCMU_MISC_SSS_USER 0x0610
#define MISC_CLK_CON_DIV_DIV_CLK_MISC_GIC 0x1800
#define MISC_CLK_CON_DIV_DIV_CLK_MISC_GIC_LH 0x1804
#define MISC_CLK_CON_DIV_DIV_CLK_MISC_NOCP 0x1808
#define MISC_CLK_CON_DIV_DIV_CLK_MISC_NOCP_LH 0x180c
#define MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_LH_AST_MI_L_ICC_CLUSTER0_GIC_IPCLKPORT_I_CLK \
	0x2000
#define MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_LH_AST_MI_L_IRI_GIC_CLUSTER0_CD_IPCLKPORT_I_CLK \
	0x2004
#define MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_LH_AST_SI_L_ICC_CLUSTER0_GIC_CU_IPCLKPORT_I_CLK \
	0x2008
#define MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_LH_AST_SI_L_IRI_GIC_CLUSTER0_IPCLKPORT_I_CLK \
	0x200c
#define MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_LH_AXI_SI_P_GIC_CU_IPCLKPORT_I_CLK \
	0x2010
#define MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_LH_AXI_SI_P_MISC_CU_IPCLKPORT_I_CLK \
	0x2014
#define MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_MISC_CMU_MISC_IPCLKPORT_PCLK 0x2018
#define MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_OTP_CON_BIRA_IPCLKPORT_I_OSCCLK 0x201c
#define MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_OTP_CON_BISR_IPCLKPORT_I_OSCCLK 0x2020
#define MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_OTP_CON_TOP_IPCLKPORT_I_OSCCLK 0x2024
#define MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_PDMA1_IPCLKPORT_ACLK 0x2028
#define MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_QE_PDMA1_IPCLKPORT_ACLK 0x202c
#define MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_QE_PDMA1_IPCLKPORT_PCLK 0x2030
#define MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_QE_SPDMA1_IPCLKPORT_ACLK 0x2034
#define MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_QE_SPDMA1_IPCLKPORT_PCLK 0x2038
#define MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_RSTNSYNC_CLK_MISC_GIC_LH_IPCLKPORT_CLK \
	0x203c
#define MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_RSTNSYNC_CLK_MISC_NOCP_LH_IPCLKPORT_CLK \
	0x2040
#define MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_RSTNSYNC_CLK_MISC_OSCCLK_IPCLKPORT_CLK \
	0x2044
#define MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_SLH_AXI_MI_P_GIC_IPCLKPORT_I_CLK \
	0x2048
#define MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_SLH_AXI_MI_P_MISC_IPCLKPORT_I_CLK \
	0x204c
#define MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_SPDMA1_IPCLKPORT_ACLK 0x2050
#define MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_SSMT_PDMA1_IPCLKPORT_ACLK 0x2054
#define MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_SSMT_PDMA1_IPCLKPORT_PCLK 0x2058
#define MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_SSMT_SPDMA1_IPCLKPORT_ACLK 0x205c
#define MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_SSMT_SPDMA1_IPCLKPORT_PCLK 0x2060
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_ADM_AHB_G_SSS_IPCLKPORT_HCLKM 0x2064
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_AD_APB_DIT_IPCLKPORT_PCLKM 0x2068
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_AD_APB_PUF_IPCLKPORT_PCLKM 0x206c
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_DIT_IPCLKPORT_ICLKL2A 0x2070
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_D_TZPC_MISC_IPCLKPORT_PCLK 0x2074
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_GIC_IPCLKPORT_GICCLK 0x2078
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_GPC_MISC_IPCLKPORT_PCLK 0x207c
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_LH_ACEL_SI_D_MISC_IPCLKPORT_I_CLK \
	0x2080
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_LH_AST_MI_L_ICC_CLUSTER0_GIC_CU_IPCLKPORT_I_CLK \
	0x2084
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_LH_AST_SI_L_IRI_GIC_CLUSTER0_CD_IPCLKPORT_I_CLK \
	0x2088
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_LH_AXI_MI_ID_SSS_IPCLKPORT_I_CLK \
	0x208c
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_LH_AXI_MI_P_GIC_CU_IPCLKPORT_I_CLK \
	0x2090
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_LH_AXI_MI_P_MISC_CU_IPCLKPORT_I_CLK \
	0x2094
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_LH_AXI_SI_ID_SSS_IPCLKPORT_I_CLK \
	0x2098
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_MCT_IPCLKPORT_PCLK 0x209c
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_OTP_CON_BIRA_IPCLKPORT_PCLK 0x20a0
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_OTP_CON_BISR_IPCLKPORT_PCLK 0x20a4
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_OTP_CON_TOP_IPCLKPORT_PCLK 0x20a8
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_PDMA0_IPCLKPORT_ACLK 0x20ac
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_PPMU_MISC_IPCLKPORT_ACLK 0x20b0
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_PPMU_MISC_IPCLKPORT_PCLK 0x20b4
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_PUF_IPCLKPORT_I_CLK 0x20b8
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_QE_DIT_IPCLKPORT_ACLK 0x20bc
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_QE_DIT_IPCLKPORT_PCLK 0x20c0
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_QE_PDMA0_IPCLKPORT_ACLK 0x20c4
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_QE_PDMA0_IPCLKPORT_PCLK 0x20c8
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_QE_RTIC_IPCLKPORT_ACLK 0x20cc
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_QE_RTIC_IPCLKPORT_PCLK 0x20d0
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_QE_SPDMA0_IPCLKPORT_ACLK 0x20d4
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_QE_SPDMA0_IPCLKPORT_PCLK 0x20d8
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_QE_SSS_IPCLKPORT_ACLK 0x20dc
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_QE_SSS_IPCLKPORT_PCLK 0x20e0
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_RSTNSYNC_CLK_MISC_GIC_IPCLKPORT_CLK \
	0x20e4
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_RSTNSYNC_CLK_MISC_NOCD_IPCLKPORT_CLK \
	0x20e8
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_RSTNSYNC_CLK_MISC_NOCP_IPCLKPORT_CLK \
	0x20ec
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_RSTNSYNC_CLK_MISC_SSS_IPCLKPORT_CLK \
	0x20f0
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_RTIC_IPCLKPORT_I_ACLK 0x20f4
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_RTIC_IPCLKPORT_I_PCLK 0x20f8
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SPDMA0_IPCLKPORT_ACLK 0x20fc
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SSMT_DIT_IPCLKPORT_ACLK 0x2100
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SSMT_DIT_IPCLKPORT_PCLK 0x2104
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SSMT_PDMA0_IPCLKPORT_ACLK 0x2108
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SSMT_PDMA0_IPCLKPORT_PCLK 0x210c
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SSMT_RTIC_IPCLKPORT_ACLK 0x2110
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SSMT_RTIC_IPCLKPORT_PCLK 0x2114
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SSMT_SPDMA0_IPCLKPORT_ACLK 0x2118
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SSMT_SPDMA0_IPCLKPORT_PCLK 0x211c
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SSMT_SSS_IPCLKPORT_ACLK 0x2120
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SSMT_SSS_IPCLKPORT_PCLK 0x2124
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SSS_IPCLKPORT_I_ACLK 0x2128
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SSS_IPCLKPORT_I_PCLK 0x212c
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SYSMMU_MISC_IPCLKPORT_CLK_S2 0x2130
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SYSMMU_SSS_IPCLKPORT_CLK_S1 0x2134
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SYSREG_MISC_IPCLKPORT_PCLK 0x2138
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_TMU_SUB_IPCLKPORT_PCLK 0x213c
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_TMU_TOP_IPCLKPORT_PCLK 0x2140
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_WDT_CLUSTER0_IPCLKPORT_PCLK 0x2144
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_WDT_CLUSTER1_IPCLKPORT_PCLK 0x2148
#define MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_XIU_D_MISC_IPCLKPORT_ACLK 0x214c

static const unsigned long misc_clk_regs[] __initconst = {
	MISC_PLL_CON0_MUX_CLKCMU_MISC_NOC_USER,
	MISC_PLL_CON0_MUX_CLKCMU_MISC_SSS_USER,
	MISC_CLK_CON_DIV_DIV_CLK_MISC_GIC,
	MISC_CLK_CON_DIV_DIV_CLK_MISC_GIC_LH,
	MISC_CLK_CON_DIV_DIV_CLK_MISC_NOCP,
	MISC_CLK_CON_DIV_DIV_CLK_MISC_NOCP_LH,
	MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_LH_AST_MI_L_ICC_CLUSTER0_GIC_IPCLKPORT_I_CLK,
	MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_LH_AST_MI_L_IRI_GIC_CLUSTER0_CD_IPCLKPORT_I_CLK,
	MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_LH_AST_SI_L_ICC_CLUSTER0_GIC_CU_IPCLKPORT_I_CLK,
	MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_LH_AST_SI_L_IRI_GIC_CLUSTER0_IPCLKPORT_I_CLK,
	MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_LH_AXI_SI_P_GIC_CU_IPCLKPORT_I_CLK,
	MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_LH_AXI_SI_P_MISC_CU_IPCLKPORT_I_CLK,
	MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_MISC_CMU_MISC_IPCLKPORT_PCLK,
	MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_OTP_CON_BIRA_IPCLKPORT_I_OSCCLK,
	MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_OTP_CON_BISR_IPCLKPORT_I_OSCCLK,
	MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_OTP_CON_TOP_IPCLKPORT_I_OSCCLK,
	MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_PDMA1_IPCLKPORT_ACLK,
	MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_QE_PDMA1_IPCLKPORT_ACLK,
	MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_QE_PDMA1_IPCLKPORT_PCLK,
	MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_QE_SPDMA1_IPCLKPORT_ACLK,
	MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_QE_SPDMA1_IPCLKPORT_PCLK,
	MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_RSTNSYNC_CLK_MISC_GIC_LH_IPCLKPORT_CLK,
	MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_RSTNSYNC_CLK_MISC_NOCP_LH_IPCLKPORT_CLK,
	MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_RSTNSYNC_CLK_MISC_OSCCLK_IPCLKPORT_CLK,
	MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_SLH_AXI_MI_P_GIC_IPCLKPORT_I_CLK,
	MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_SLH_AXI_MI_P_MISC_IPCLKPORT_I_CLK,
	MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_SPDMA1_IPCLKPORT_ACLK,
	MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_SSMT_PDMA1_IPCLKPORT_ACLK,
	MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_SSMT_PDMA1_IPCLKPORT_PCLK,
	MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_SSMT_SPDMA1_IPCLKPORT_ACLK,
	MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_SSMT_SPDMA1_IPCLKPORT_PCLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_ADM_AHB_G_SSS_IPCLKPORT_HCLKM,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_AD_APB_DIT_IPCLKPORT_PCLKM,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_AD_APB_PUF_IPCLKPORT_PCLKM,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_DIT_IPCLKPORT_ICLKL2A,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_D_TZPC_MISC_IPCLKPORT_PCLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_GIC_IPCLKPORT_GICCLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_GPC_MISC_IPCLKPORT_PCLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_LH_ACEL_SI_D_MISC_IPCLKPORT_I_CLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_LH_AST_MI_L_ICC_CLUSTER0_GIC_CU_IPCLKPORT_I_CLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_LH_AST_SI_L_IRI_GIC_CLUSTER0_CD_IPCLKPORT_I_CLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_LH_AXI_MI_ID_SSS_IPCLKPORT_I_CLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_LH_AXI_MI_P_GIC_CU_IPCLKPORT_I_CLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_LH_AXI_MI_P_MISC_CU_IPCLKPORT_I_CLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_LH_AXI_SI_ID_SSS_IPCLKPORT_I_CLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_MCT_IPCLKPORT_PCLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_OTP_CON_BIRA_IPCLKPORT_PCLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_OTP_CON_BISR_IPCLKPORT_PCLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_OTP_CON_TOP_IPCLKPORT_PCLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_PDMA0_IPCLKPORT_ACLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_PPMU_MISC_IPCLKPORT_ACLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_PPMU_MISC_IPCLKPORT_PCLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_PUF_IPCLKPORT_I_CLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_QE_DIT_IPCLKPORT_ACLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_QE_DIT_IPCLKPORT_PCLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_QE_PDMA0_IPCLKPORT_ACLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_QE_PDMA0_IPCLKPORT_PCLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_QE_RTIC_IPCLKPORT_ACLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_QE_RTIC_IPCLKPORT_PCLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_QE_SPDMA0_IPCLKPORT_ACLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_QE_SPDMA0_IPCLKPORT_PCLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_QE_SSS_IPCLKPORT_ACLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_QE_SSS_IPCLKPORT_PCLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_RSTNSYNC_CLK_MISC_GIC_IPCLKPORT_CLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_RSTNSYNC_CLK_MISC_NOCD_IPCLKPORT_CLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_RSTNSYNC_CLK_MISC_NOCP_IPCLKPORT_CLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_RSTNSYNC_CLK_MISC_SSS_IPCLKPORT_CLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_RTIC_IPCLKPORT_I_ACLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_RTIC_IPCLKPORT_I_PCLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SPDMA0_IPCLKPORT_ACLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SSMT_DIT_IPCLKPORT_ACLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SSMT_DIT_IPCLKPORT_PCLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SSMT_PDMA0_IPCLKPORT_ACLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SSMT_PDMA0_IPCLKPORT_PCLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SSMT_RTIC_IPCLKPORT_ACLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SSMT_RTIC_IPCLKPORT_PCLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SSMT_SPDMA0_IPCLKPORT_ACLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SSMT_SPDMA0_IPCLKPORT_PCLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SSMT_SSS_IPCLKPORT_ACLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SSMT_SSS_IPCLKPORT_PCLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SSS_IPCLKPORT_I_ACLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SSS_IPCLKPORT_I_PCLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SYSMMU_MISC_IPCLKPORT_CLK_S2,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SYSMMU_SSS_IPCLKPORT_CLK_S1,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SYSREG_MISC_IPCLKPORT_PCLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_TMU_SUB_IPCLKPORT_PCLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_TMU_TOP_IPCLKPORT_PCLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_WDT_CLUSTER0_IPCLKPORT_PCLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_WDT_CLUSTER1_IPCLKPORT_PCLK,
	MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_XIU_D_MISC_IPCLKPORT_ACLK,
};

#define CLKS_NR_MISC 91

PNAME(mout_misc_noc_user_p) = { "oscclk", "dout_cmu_misc_noc" };
PNAME(mout_misc_sss_user_p) = { "oscclk", "dout_cmu_misc_sss" };

static const struct samsung_mux_clock misc_mux_clks[] __initconst = {
	MUX(CLK_MOUT_MISC_NOC_USER, "mout_misc_noc_user", mout_misc_noc_user_p,
	    MISC_PLL_CON0_MUX_CLKCMU_MISC_NOC_USER, 4, 1),
	MUX(CLK_MOUT_MISC_SSS_USER, "mout_misc_sss_user", mout_misc_sss_user_p,
	    MISC_PLL_CON0_MUX_CLKCMU_MISC_SSS_USER, 4, 1),
};

static const struct samsung_div_clock misc_div_clks[] __initconst = {
	DIV(CLK_DOUT_MISC_GIC, "dout_misc_gic", "mout_misc_noc_user",
	    MISC_CLK_CON_DIV_DIV_CLK_MISC_GIC, 0, 3),
	DIV(CLK_DOUT_MISC_GIC_LH, "dout_misc_gic_lh", "dout_misc_gic",
	    MISC_CLK_CON_DIV_DIV_CLK_MISC_GIC_LH, 0, 3),
	DIV(CLK_DOUT_MISC_NOCP, "dout_misc_nocp", "mout_misc_noc_user",
	    MISC_CLK_CON_DIV_DIV_CLK_MISC_NOCP, 0, 3),
	DIV(CLK_DOUT_MISC_NOCP_LH, "dout_misc_nocp_lh", "dout_misc_nocp",
	    MISC_CLK_CON_DIV_DIV_CLK_MISC_NOCP_LH, 0, 3),
};

static const struct samsung_gate_clock misc_gate_clks[] __initconst = {
	GATE(CLK_GOUT_MISC_LH_AST_MI_L_ICC_CLUSTER0_GIC_I_CLK,
	     "gout_misc_lh_ast_mi_l_icc_cluster0_gic_i_clk", "dout_misc_gic_lh",
	     MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_LH_AST_MI_L_ICC_CLUSTER0_GIC_IPCLKPORT_I_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_MISC_LH_AST_MI_L_IRI_GIC_CLUSTER0_CD_I_CLK,
	     "gout_misc_lh_ast_mi_l_iri_gic_cluster0_cd_i_clk",
	     "dout_misc_gic_lh",
	     MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_LH_AST_MI_L_IRI_GIC_CLUSTER0_CD_IPCLKPORT_I_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_MISC_LH_AST_SI_L_ICC_CLUSTER0_GIC_CU_I_CLK,
	     "gout_misc_lh_ast_si_l_icc_cluster0_gic_cu_i_clk",
	     "dout_misc_gic_lh",
	     MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_LH_AST_SI_L_ICC_CLUSTER0_GIC_CU_IPCLKPORT_I_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_MISC_LH_AST_SI_L_IRI_GIC_CLUSTER0_I_CLK,
	     "gout_misc_lh_ast_si_l_iri_gic_cluster0_i_clk", "dout_misc_gic_lh",
	     MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_LH_AST_SI_L_IRI_GIC_CLUSTER0_IPCLKPORT_I_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_MISC_LH_AXI_SI_P_GIC_CU_I_CLK,
	     "gout_misc_lh_axi_si_p_gic_cu_i_clk", "dout_misc_gic_lh",
	     MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_LH_AXI_SI_P_GIC_CU_IPCLKPORT_I_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_MISC_LH_AXI_SI_P_MISC_CU_I_CLK,
	     "gout_misc_lh_axi_si_p_misc_cu_i_clk", "dout_misc_nocp_lh",
	     MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_LH_AXI_SI_P_MISC_CU_IPCLKPORT_I_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_MISC_MISC_CMU_MISC_PCLK, "gout_misc_misc_cmu_misc_pclk",
	     "dout_misc_nocp",
	     MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_MISC_CMU_MISC_IPCLKPORT_PCLK, 21,
	     0, 0),
	GATE(CLK_GOUT_MISC_OTP_CON_BIRA_I_OSCCLK,
	     "gout_misc_otp_con_bira_i_oscclk", "oscclk",
	     MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_OTP_CON_BIRA_IPCLKPORT_I_OSCCLK,
	     21, 0, 0),
	GATE(CLK_GOUT_MISC_OTP_CON_BISR_I_OSCCLK,
	     "gout_misc_otp_con_bisr_i_oscclk", "oscclk",
	     MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_OTP_CON_BISR_IPCLKPORT_I_OSCCLK,
	     21, 0, 0),
	GATE(CLK_GOUT_MISC_OTP_CON_TOP_I_OSCCLK,
	     "gout_misc_otp_con_top_i_oscclk", "oscclk",
	     MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_OTP_CON_TOP_IPCLKPORT_I_OSCCLK,
	     21, 0, 0),
	GATE(CLK_GOUT_MISC_PDMA1_ACLK, "gout_misc_pdma1_aclk",
	     "mout_misc_noc_user",
	     MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_PDMA1_IPCLKPORT_ACLK, 21, 0, 0),
	GATE(CLK_GOUT_MISC_QE_PDMA1_ACLK, "gout_misc_qe_pdma1_aclk",
	     "mout_misc_noc_user",
	     MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_QE_PDMA1_IPCLKPORT_ACLK, 21, 0,
	     0),
	GATE(CLK_GOUT_MISC_QE_PDMA1_PCLK, "gout_misc_qe_pdma1_pclk",
	     "dout_misc_nocp",
	     MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_QE_PDMA1_IPCLKPORT_PCLK, 21, 0,
	     0),
	GATE(CLK_GOUT_MISC_QE_SPDMA1_ACLK, "gout_misc_qe_spdma1_aclk",
	     "mout_misc_noc_user",
	     MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_QE_SPDMA1_IPCLKPORT_ACLK, 21, 0,
	     0),
	GATE(CLK_GOUT_MISC_QE_SPDMA1_PCLK, "gout_misc_qe_spdma1_pclk",
	     "dout_misc_nocp",
	     MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_QE_SPDMA1_IPCLKPORT_PCLK, 21, 0,
	     0),
	GATE(CLK_GOUT_MISC_RSTNSYNC_CLK_MISC_GIC_LH_CLK,
	     "gout_misc_rstnsync_clk_misc_gic_lh_clk", "dout_misc_gic_lh",
	     MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_RSTNSYNC_CLK_MISC_GIC_LH_IPCLKPORT_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_MISC_RSTNSYNC_CLK_MISC_NOCP_LH_CLK,
	     "gout_misc_rstnsync_clk_misc_nocp_lh_clk", "dout_misc_nocp_lh",
	     MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_RSTNSYNC_CLK_MISC_NOCP_LH_IPCLKPORT_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_MISC_RSTNSYNC_CLK_MISC_OSCCLK_CLK,
	     "gout_misc_rstnsync_clk_misc_oscclk_clk", "oscclk",
	     MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_RSTNSYNC_CLK_MISC_OSCCLK_IPCLKPORT_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_MISC_SLH_AXI_MI_P_GIC_I_CLK,
	     "gout_misc_slh_axi_mi_p_gic_i_clk", "dout_misc_gic_lh",
	     MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_SLH_AXI_MI_P_GIC_IPCLKPORT_I_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_MISC_SLH_AXI_MI_P_MISC_I_CLK,
	     "gout_misc_slh_axi_mi_p_misc_i_clk", "dout_misc_nocp_lh",
	     MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_SLH_AXI_MI_P_MISC_IPCLKPORT_I_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_MISC_SPDMA1_ACLK, "gout_misc_spdma1_aclk",
	     "mout_misc_noc_user",
	     MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_SPDMA1_IPCLKPORT_ACLK, 21, 0, 0),
	GATE(CLK_GOUT_MISC_SSMT_PDMA1_ACLK, "gout_misc_ssmt_pdma1_aclk",
	     "mout_misc_noc_user",
	     MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_SSMT_PDMA1_IPCLKPORT_ACLK, 21, 0,
	     0),
	GATE(CLK_GOUT_MISC_SSMT_PDMA1_PCLK, "gout_misc_ssmt_pdma1_pclk",
	     "dout_misc_nocp",
	     MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_SSMT_PDMA1_IPCLKPORT_PCLK, 21, 0,
	     0),
	GATE(CLK_GOUT_MISC_SSMT_SPDMA1_ACLK, "gout_misc_ssmt_spdma1_aclk",
	     "mout_misc_noc_user",
	     MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_SSMT_SPDMA1_IPCLKPORT_ACLK, 21,
	     0, 0),
	GATE(CLK_GOUT_MISC_SSMT_SPDMA1_PCLK, "gout_misc_ssmt_spdma1_pclk",
	     "dout_misc_nocp",
	     MISC_CLK_CON_GAT_CLK_BLK_MISC_UID_SSMT_SPDMA1_IPCLKPORT_PCLK, 21,
	     0, 0),
	GATE(CLK_GOUT_MISC_ADM_AHB_G_SSS_HCLKM, "gout_misc_adm_ahb_g_sss_hclkm",
	     "mout_misc_sss_user",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_ADM_AHB_G_SSS_IPCLKPORT_HCLKM,
	     21, 0, 0),
	GATE(CLK_GOUT_MISC_AD_APB_DIT_PCLKM, "gout_misc_ad_apb_dit_pclkm",
	     "mout_misc_noc_user",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_AD_APB_DIT_IPCLKPORT_PCLKM, 21,
	     0, 0),
	GATE(CLK_GOUT_MISC_AD_APB_PUF_PCLKM, "gout_misc_ad_apb_puf_pclkm",
	     "mout_misc_sss_user",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_AD_APB_PUF_IPCLKPORT_PCLKM, 21,
	     0, 0),
	GATE(CLK_GOUT_MISC_DIT_ICLKL2A, "gout_misc_dit_iclkl2a",
	     "mout_misc_noc_user",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_DIT_IPCLKPORT_ICLKL2A, 21, 0,
	     0),
	GATE(CLK_GOUT_MISC_D_TZPC_MISC_PCLK, "gout_misc_d_tzpc_misc_pclk",
	     "dout_misc_nocp",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_D_TZPC_MISC_IPCLKPORT_PCLK, 21,
	     0, 0),
	GATE(CLK_GOUT_MISC_GIC_GICCLK, "gout_misc_gic_gicclk", "dout_misc_gic",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_GIC_IPCLKPORT_GICCLK, 21, 0, 0),
	GATE(CLK_GOUT_MISC_GPC_MISC_PCLK, "gout_misc_gpc_misc_pclk",
	     "dout_misc_nocp",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_GPC_MISC_IPCLKPORT_PCLK, 21, 0,
	     0),
	GATE(CLK_GOUT_MISC_LH_ACEL_SI_D_MISC_I_CLK,
	     "gout_misc_lh_acel_si_d_misc_i_clk", "mout_misc_noc_user",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_LH_ACEL_SI_D_MISC_IPCLKPORT_I_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_MISC_LH_AST_MI_L_ICC_CLUSTER0_GIC_CU_I_CLK,
	     "gout_misc_lh_ast_mi_l_icc_cluster0_gic_cu_i_clk", "dout_misc_gic",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_LH_AST_MI_L_ICC_CLUSTER0_GIC_CU_IPCLKPORT_I_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_MISC_LH_AST_SI_L_IRI_GIC_CLUSTER0_CD_I_CLK,
	     "gout_misc_lh_ast_si_l_iri_gic_cluster0_cd_i_clk", "dout_misc_gic",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_LH_AST_SI_L_IRI_GIC_CLUSTER0_CD_IPCLKPORT_I_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_MISC_LH_AXI_MI_ID_SSS_I_CLK,
	     "gout_misc_lh_axi_mi_id_sss_i_clk", "mout_misc_noc_user",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_LH_AXI_MI_ID_SSS_IPCLKPORT_I_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_MISC_LH_AXI_MI_P_GIC_CU_I_CLK,
	     "gout_misc_lh_axi_mi_p_gic_cu_i_clk", "dout_misc_gic",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_LH_AXI_MI_P_GIC_CU_IPCLKPORT_I_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_MISC_LH_AXI_MI_P_MISC_CU_I_CLK,
	     "gout_misc_lh_axi_mi_p_misc_cu_i_clk", "dout_misc_nocp",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_LH_AXI_MI_P_MISC_CU_IPCLKPORT_I_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_MISC_LH_AXI_SI_ID_SSS_I_CLK,
	     "gout_misc_lh_axi_si_id_sss_i_clk", "mout_misc_sss_user",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_LH_AXI_SI_ID_SSS_IPCLKPORT_I_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_MISC_MCT_PCLK, "gout_misc_mct_pclk", "dout_misc_nocp",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_MCT_IPCLKPORT_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_MISC_OTP_CON_BIRA_PCLK, "gout_misc_otp_con_bira_pclk",
	     "dout_misc_nocp",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_OTP_CON_BIRA_IPCLKPORT_PCLK, 21,
	     0, 0),
	GATE(CLK_GOUT_MISC_OTP_CON_BISR_PCLK, "gout_misc_otp_con_bisr_pclk",
	     "dout_misc_nocp",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_OTP_CON_BISR_IPCLKPORT_PCLK, 21,
	     0, 0),
	GATE(CLK_GOUT_MISC_OTP_CON_TOP_PCLK, "gout_misc_otp_con_top_pclk",
	     "dout_misc_nocp",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_OTP_CON_TOP_IPCLKPORT_PCLK, 21,
	     0, 0),
	GATE(CLK_GOUT_MISC_PDMA0_ACLK, "gout_misc_pdma0_aclk",
	     "mout_misc_noc_user",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_PDMA0_IPCLKPORT_ACLK, 21, 0, 0),
	GATE(CLK_GOUT_MISC_PPMU_MISC_ACLK, "gout_misc_ppmu_misc_aclk",
	     "mout_misc_noc_user",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_PPMU_MISC_IPCLKPORT_ACLK, 21, 0,
	     0),
	GATE(CLK_GOUT_MISC_PPMU_MISC_PCLK, "gout_misc_ppmu_misc_pclk",
	     "dout_misc_nocp",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_PPMU_MISC_IPCLKPORT_PCLK, 21, 0,
	     0),
	GATE(CLK_GOUT_MISC_PUF_I_CLK, "gout_misc_puf_i_clk",
	     "mout_misc_sss_user",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_PUF_IPCLKPORT_I_CLK, 21, 0, 0),
	GATE(CLK_GOUT_MISC_QE_DIT_ACLK, "gout_misc_qe_dit_aclk",
	     "mout_misc_noc_user",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_QE_DIT_IPCLKPORT_ACLK, 21, 0,
	     0),
	GATE(CLK_GOUT_MISC_QE_DIT_PCLK, "gout_misc_qe_dit_pclk",
	     "dout_misc_nocp",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_QE_DIT_IPCLKPORT_PCLK, 21, 0,
	     0),
	GATE(CLK_GOUT_MISC_QE_PDMA0_ACLK, "gout_misc_qe_pdma0_aclk",
	     "mout_misc_noc_user",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_QE_PDMA0_IPCLKPORT_ACLK, 21, 0,
	     0),
	GATE(CLK_GOUT_MISC_QE_PDMA0_PCLK, "gout_misc_qe_pdma0_pclk",
	     "dout_misc_nocp",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_QE_PDMA0_IPCLKPORT_PCLK, 21, 0,
	     0),
	GATE(CLK_GOUT_MISC_QE_RTIC_ACLK, "gout_misc_qe_rtic_aclk",
	     "mout_misc_noc_user",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_QE_RTIC_IPCLKPORT_ACLK, 21, 0,
	     0),
	GATE(CLK_GOUT_MISC_QE_RTIC_PCLK, "gout_misc_qe_rtic_pclk",
	     "dout_misc_nocp",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_QE_RTIC_IPCLKPORT_PCLK, 21, 0,
	     0),
	GATE(CLK_GOUT_MISC_QE_SPDMA0_ACLK, "gout_misc_qe_spdma0_aclk",
	     "mout_misc_noc_user",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_QE_SPDMA0_IPCLKPORT_ACLK, 21, 0,
	     0),
	GATE(CLK_GOUT_MISC_QE_SPDMA0_PCLK, "gout_misc_qe_spdma0_pclk",
	     "dout_misc_nocp",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_QE_SPDMA0_IPCLKPORT_PCLK, 21, 0,
	     0),
	GATE(CLK_GOUT_MISC_QE_SSS_ACLK, "gout_misc_qe_sss_aclk",
	     "mout_misc_sss_user",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_QE_SSS_IPCLKPORT_ACLK, 21, 0,
	     0),
	GATE(CLK_GOUT_MISC_QE_SSS_PCLK, "gout_misc_qe_sss_pclk",
	     "dout_misc_nocp",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_QE_SSS_IPCLKPORT_PCLK, 21, 0,
	     0),
	GATE(CLK_GOUT_MISC_RSTNSYNC_CLK_MISC_GIC_CLK,
	     "gout_misc_rstnsync_clk_misc_gic_clk", "dout_misc_gic",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_RSTNSYNC_CLK_MISC_GIC_IPCLKPORT_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_MISC_RSTNSYNC_CLK_MISC_NOCD_CLK,
	     "gout_misc_rstnsync_clk_misc_nocd_clk", "mout_misc_noc_user",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_RSTNSYNC_CLK_MISC_NOCD_IPCLKPORT_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_MISC_RSTNSYNC_CLK_MISC_NOCP_CLK,
	     "gout_misc_rstnsync_clk_misc_nocp_clk", "dout_misc_nocp",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_RSTNSYNC_CLK_MISC_NOCP_IPCLKPORT_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_MISC_RSTNSYNC_CLK_MISC_SSS_CLK,
	     "gout_misc_rstnsync_clk_misc_sss_clk", "mout_misc_sss_user",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_RSTNSYNC_CLK_MISC_SSS_IPCLKPORT_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_MISC_RTIC_I_ACLK, "gout_misc_rtic_i_aclk",
	     "mout_misc_noc_user",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_RTIC_IPCLKPORT_I_ACLK, 21, 0,
	     0),
	GATE(CLK_GOUT_MISC_RTIC_I_PCLK, "gout_misc_rtic_i_pclk",
	     "dout_misc_nocp",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_RTIC_IPCLKPORT_I_PCLK, 21, 0,
	     0),
	GATE(CLK_GOUT_MISC_SPDMA0_ACLK, "gout_misc_spdma0_aclk",
	     "mout_misc_noc_user",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SPDMA0_IPCLKPORT_ACLK, 21, 0,
	     0),
	GATE(CLK_GOUT_MISC_SSMT_DIT_ACLK, "gout_misc_ssmt_dit_aclk",
	     "mout_misc_noc_user",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SSMT_DIT_IPCLKPORT_ACLK, 21, 0,
	     0),
	GATE(CLK_GOUT_MISC_SSMT_DIT_PCLK, "gout_misc_ssmt_dit_pclk",
	     "dout_misc_nocp",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SSMT_DIT_IPCLKPORT_PCLK, 21, 0,
	     0),
	GATE(CLK_GOUT_MISC_SSMT_PDMA0_ACLK, "gout_misc_ssmt_pdma0_aclk",
	     "mout_misc_noc_user",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SSMT_PDMA0_IPCLKPORT_ACLK, 21,
	     0, 0),
	GATE(CLK_GOUT_MISC_SSMT_PDMA0_PCLK, "gout_misc_ssmt_pdma0_pclk",
	     "dout_misc_nocp",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SSMT_PDMA0_IPCLKPORT_PCLK, 21,
	     0, 0),
	GATE(CLK_GOUT_MISC_SSMT_RTIC_ACLK, "gout_misc_ssmt_rtic_aclk",
	     "mout_misc_noc_user",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SSMT_RTIC_IPCLKPORT_ACLK, 21, 0,
	     0),
	GATE(CLK_GOUT_MISC_SSMT_RTIC_PCLK, "gout_misc_ssmt_rtic_pclk",
	     "dout_misc_nocp",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SSMT_RTIC_IPCLKPORT_PCLK, 21, 0,
	     0),
	GATE(CLK_GOUT_MISC_SSMT_SPDMA0_ACLK, "gout_misc_ssmt_spdma0_aclk",
	     "mout_misc_noc_user",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SSMT_SPDMA0_IPCLKPORT_ACLK, 21,
	     0, 0),
	GATE(CLK_GOUT_MISC_SSMT_SPDMA0_PCLK, "gout_misc_ssmt_spdma0_pclk",
	     "dout_misc_nocp",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SSMT_SPDMA0_IPCLKPORT_PCLK, 21,
	     0, 0),
	GATE(CLK_GOUT_MISC_SSMT_SSS_ACLK, "gout_misc_ssmt_sss_aclk",
	     "mout_misc_sss_user",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SSMT_SSS_IPCLKPORT_ACLK, 21, 0,
	     0),
	GATE(CLK_GOUT_MISC_SSMT_SSS_PCLK, "gout_misc_ssmt_sss_pclk",
	     "dout_misc_nocp",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SSMT_SSS_IPCLKPORT_PCLK, 21, 0,
	     0),
	GATE(CLK_GOUT_MISC_SSS_I_ACLK, "gout_misc_sss_i_aclk",
	     "mout_misc_sss_user",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SSS_IPCLKPORT_I_ACLK, 21, 0, 0),
	GATE(CLK_GOUT_MISC_SSS_I_PCLK, "gout_misc_sss_i_pclk", "dout_misc_nocp",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SSS_IPCLKPORT_I_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_MISC_SYSMMU_MISC_CLK_S2, "gout_misc_sysmmu_misc_clk_s2",
	     "mout_misc_noc_user",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SYSMMU_MISC_IPCLKPORT_CLK_S2,
	     21, 0, 0),
	GATE(CLK_GOUT_MISC_SYSMMU_SSS_CLK_S1, "gout_misc_sysmmu_sss_clk_s1",
	     "mout_misc_sss_user",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SYSMMU_SSS_IPCLKPORT_CLK_S1, 21,
	     0, 0),
	GATE(CLK_GOUT_MISC_SYSREG_MISC_PCLK, "gout_misc_sysreg_misc_pclk",
	     "dout_misc_nocp",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_SYSREG_MISC_IPCLKPORT_PCLK, 21,
	     0, 0),
	GATE(CLK_GOUT_MISC_TMU_SUB_PCLK, "gout_misc_tmu_sub_pclk",
	     "dout_misc_nocp",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_TMU_SUB_IPCLKPORT_PCLK, 21, 0,
	     0),
	GATE(CLK_GOUT_MISC_TMU_TOP_PCLK, "gout_misc_tmu_top_pclk",
	     "dout_misc_nocp",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_TMU_TOP_IPCLKPORT_PCLK, 21, 0,
	     0),
	GATE(CLK_GOUT_MISC_WDT_CLUSTER0_PCLK, "gout_misc_wdt_cluster0_pclk",
	     "dout_misc_nocp",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_WDT_CLUSTER0_IPCLKPORT_PCLK, 21,
	     0, 0),
	GATE(CLK_GOUT_MISC_WDT_CLUSTER1_PCLK, "gout_misc_wdt_cluster1_pclk",
	     "dout_misc_nocp",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_WDT_CLUSTER1_IPCLKPORT_PCLK, 21,
	     0, 0),
	GATE(CLK_GOUT_MISC_XIU_D_MISC_ACLK, "gout_misc_xiu_d_misc_aclk",
	     "mout_misc_noc_user",
	     MISC_CLK_CON_GAT_GOUT_BLK_MISC_UID_XIU_D_MISC_IPCLKPORT_ACLK, 21,
	     0, 0),
};

static const struct samsung_cmu_info misc_cmu_info __initconst = {
	.mux_clks = misc_mux_clks,
	.nr_mux_clks = ARRAY_SIZE(misc_mux_clks),
	.div_clks = misc_div_clks,
	.nr_div_clks = ARRAY_SIZE(misc_div_clks),
	.gate_clks = misc_gate_clks,
	.nr_gate_clks = ARRAY_SIZE(misc_gate_clks),
	.nr_clk_ids = CLKS_NR_MISC,
	.clk_regs = misc_clk_regs,
	.nr_clk_regs = ARRAY_SIZE(misc_clk_regs),
	.clk_name = "bus",
};

/* ---- CMU_PERIC0 ------------------------------------------------------------- */
/* Register offsets at 0x10800000. */
#define PERIC0_PLL_CON0_MUX_CLKCMU_PERIC0_NOC_USER 0x0600
#define PERIC0_PLL_CON0_MUX_CLKCMU_PERIC0_I3C_USER 0x0610
#define PERIC0_PLL_CON0_MUX_CLKCMU_PERIC0_USI0_UART_USER 0x0620
#define PERIC0_PLL_CON0_MUX_CLKCMU_PERIC0_USI14_USI_USER 0x0640
#define PERIC0_PLL_CON0_MUX_CLKCMU_PERIC0_USI1_USI_USER 0x0650
#define PERIC0_PLL_CON0_MUX_CLKCMU_PERIC0_USI2_USI_USER 0x0660
#define PERIC0_PLL_CON0_MUX_CLKCMU_PERIC0_USI3_USI_USER 0x0670
#define PERIC0_PLL_CON0_MUX_CLKCMU_PERIC0_USI4_USI_USER 0x0680
#define PERIC0_PLL_CON0_MUX_CLKCMU_PERIC0_USI5_USI_USER 0x0690
#define PERIC0_PLL_CON0_MUX_CLKCMU_PERIC0_USI6_USI_USER 0x06a0
#define PERIC0_PLL_CON0_MUX_CLKCMU_PERIC0_USI7_USI_USER 0x06b0
#define PERIC0_PLL_CON0_MUX_CLKCMU_PERIC0_USI8_USI_USER 0x06c0
#define PERIC0_CLK_CON_DIV_DIV_CLK_PERIC0_I3C 0x1800
#define PERIC0_CLK_CON_DIV_DIV_CLK_PERIC0_NOCP_LH 0x1804
#define PERIC0_CLK_CON_DIV_DIV_CLK_PERIC0_USI0_UART 0x1808
#define PERIC0_CLK_CON_DIV_DIV_CLK_PERIC0_USI14_USI 0x180c
#define PERIC0_CLK_CON_DIV_DIV_CLK_PERIC0_USI1_USI 0x1810
#define PERIC0_CLK_CON_DIV_DIV_CLK_PERIC0_USI2_USI 0x1814
#define PERIC0_CLK_CON_DIV_DIV_CLK_PERIC0_USI3_USI 0x1818
#define PERIC0_CLK_CON_DIV_DIV_CLK_PERIC0_USI4_USI 0x181c
#define PERIC0_CLK_CON_DIV_DIV_CLK_PERIC0_USI5_USI 0x1820
#define PERIC0_CLK_CON_DIV_DIV_CLK_PERIC0_USI6_USI 0x1824
#define PERIC0_CLK_CON_DIV_DIV_CLK_PERIC0_USI7_USI 0x1828
#define PERIC0_CLK_CON_DIV_DIV_CLK_PERIC0_USI8_USI 0x182c
#define PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C1_IPCLKPORT_I_PCLK 0x2004
#define PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C1_IPCLKPORT_I_SCLK 0x2008
#define PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C2_IPCLKPORT_I_PCLK 0x200c
#define PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C2_IPCLKPORT_I_SCLK 0x2010
#define PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C3_IPCLKPORT_I_PCLK 0x2014
#define PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C3_IPCLKPORT_I_SCLK 0x2018
#define PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C4_IPCLKPORT_I_PCLK 0x201c
#define PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C4_IPCLKPORT_I_SCLK 0x2020
#define PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C5_IPCLKPORT_I_PCLK 0x2024
#define PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C5_IPCLKPORT_I_SCLK 0x2028
#define PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C6_IPCLKPORT_I_PCLK 0x202c
#define PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C6_IPCLKPORT_I_SCLK 0x2030
#define PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C7_IPCLKPORT_I_PCLK 0x2034
#define PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C7_IPCLKPORT_I_SCLK 0x2038
#define PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C8_IPCLKPORT_I_PCLK 0x203c
#define PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C8_IPCLKPORT_I_SCLK 0x2040
#define PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_LH_AXI_SI_P_PERIC0_CU_IPCLKPORT_I_CLK \
	0x2044
#define PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_PERIC0_CMU_PERIC0_IPCLKPORT_PCLK \
	0x2048
#define PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_RSTNSYNC_CLK_PERIC0_NOCP_LH_IPCLKPORT_CLK \
	0x204c
#define PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_RSTNSYNC_CLK_PERIC0_OSCCLK_IPCLKPORT_CLK \
	0x2050
#define PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_SLH_AXI_MI_P_PERIC0_IPCLKPORT_I_CLK \
	0x2054
#define PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI0_UART_IPCLKPORT_IPCLK 0x2058
#define PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI0_UART_IPCLKPORT_PCLK 0x205c
#define PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI14_USI_IPCLKPORT_IPCLK 0x2060
#define PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI14_USI_IPCLKPORT_PCLK 0x2064
#define PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI1_USI_IPCLKPORT_IPCLK 0x2068
#define PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI1_USI_IPCLKPORT_PCLK 0x206c
#define PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI2_USI_IPCLKPORT_IPCLK 0x2070
#define PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI2_USI_IPCLKPORT_PCLK 0x2074
#define PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI3_USI_IPCLKPORT_IPCLK 0x2078
#define PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI3_USI_IPCLKPORT_PCLK 0x207c
#define PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI4_USI_IPCLKPORT_IPCLK 0x2080
#define PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI4_USI_IPCLKPORT_PCLK 0x2084
#define PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI5_USI_IPCLKPORT_IPCLK 0x2088
#define PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI5_USI_IPCLKPORT_PCLK 0x208c
#define PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI6_USI_IPCLKPORT_IPCLK 0x2090
#define PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI6_USI_IPCLKPORT_PCLK 0x2094
#define PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI7_USI_IPCLKPORT_IPCLK 0x2098
#define PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI7_USI_IPCLKPORT_PCLK 0x209c
#define PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI8_USI_IPCLKPORT_IPCLK 0x20a0
#define PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI8_USI_IPCLKPORT_PCLK 0x20a4
#define PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_D_TZPC_PERIC0_IPCLKPORT_PCLK \
	0x20a8
#define PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_GPC_PERIC0_IPCLKPORT_PCLK 0x20ac
#define PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_GPIO_PERIC0_IPCLKPORT_PCLK 0x20b0
#define PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_LH_AXI_MI_P_PERIC0_CU_IPCLKPORT_I_CLK \
	0x20b4
#define PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_RSTNSYNC_CLK_PERIC0_I3C_IPCLKPORT_CLK \
	0x20b8
#define PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_RSTNSYNC_CLK_PERIC0_NOCP_IPCLKPORT_CLK \
	0x20bc
#define PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_RSTNSYNC_CLK_PERIC0_USI0_UART_IPCLKPORT_CLK \
	0x20c0
#define PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_RSTNSYNC_CLK_PERIC0_USI14_USI_IPCLKPORT_CLK \
	0x20c4
#define PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_RSTNSYNC_CLK_PERIC0_USI1_USI_IPCLKPORT_CLK \
	0x20c8
#define PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_RSTNSYNC_CLK_PERIC0_USI2_USI_IPCLKPORT_CLK \
	0x20cc
#define PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_RSTNSYNC_CLK_PERIC0_USI3_USI_IPCLKPORT_CLK \
	0x20d0
#define PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_RSTNSYNC_CLK_PERIC0_USI4_USI_IPCLKPORT_CLK \
	0x20d4
#define PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_RSTNSYNC_CLK_PERIC0_USI5_USI_IPCLKPORT_CLK \
	0x20d8
#define PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_RSTNSYNC_CLK_PERIC0_USI6_USI_IPCLKPORT_CLK \
	0x20dc
#define PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_RSTNSYNC_CLK_PERIC0_USI7_USI_IPCLKPORT_CLK \
	0x20e0
#define PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_RSTNSYNC_CLK_PERIC0_USI8_USI_IPCLKPORT_CLK \
	0x20e4
#define PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_SYSREG_PERIC0_IPCLKPORT_PCLK \
	0x20e8

static const unsigned long peric0_clk_regs[] __initconst = {
	PERIC0_PLL_CON0_MUX_CLKCMU_PERIC0_NOC_USER,
	PERIC0_PLL_CON0_MUX_CLKCMU_PERIC0_I3C_USER,
	PERIC0_PLL_CON0_MUX_CLKCMU_PERIC0_USI0_UART_USER,
	PERIC0_PLL_CON0_MUX_CLKCMU_PERIC0_USI14_USI_USER,
	PERIC0_PLL_CON0_MUX_CLKCMU_PERIC0_USI1_USI_USER,
	PERIC0_PLL_CON0_MUX_CLKCMU_PERIC0_USI2_USI_USER,
	PERIC0_PLL_CON0_MUX_CLKCMU_PERIC0_USI3_USI_USER,
	PERIC0_PLL_CON0_MUX_CLKCMU_PERIC0_USI4_USI_USER,
	PERIC0_PLL_CON0_MUX_CLKCMU_PERIC0_USI5_USI_USER,
	PERIC0_PLL_CON0_MUX_CLKCMU_PERIC0_USI6_USI_USER,
	PERIC0_PLL_CON0_MUX_CLKCMU_PERIC0_USI7_USI_USER,
	PERIC0_PLL_CON0_MUX_CLKCMU_PERIC0_USI8_USI_USER,
	PERIC0_CLK_CON_DIV_DIV_CLK_PERIC0_I3C,
	PERIC0_CLK_CON_DIV_DIV_CLK_PERIC0_NOCP_LH,
	PERIC0_CLK_CON_DIV_DIV_CLK_PERIC0_USI0_UART,
	PERIC0_CLK_CON_DIV_DIV_CLK_PERIC0_USI14_USI,
	PERIC0_CLK_CON_DIV_DIV_CLK_PERIC0_USI1_USI,
	PERIC0_CLK_CON_DIV_DIV_CLK_PERIC0_USI2_USI,
	PERIC0_CLK_CON_DIV_DIV_CLK_PERIC0_USI3_USI,
	PERIC0_CLK_CON_DIV_DIV_CLK_PERIC0_USI4_USI,
	PERIC0_CLK_CON_DIV_DIV_CLK_PERIC0_USI5_USI,
	PERIC0_CLK_CON_DIV_DIV_CLK_PERIC0_USI6_USI,
	PERIC0_CLK_CON_DIV_DIV_CLK_PERIC0_USI7_USI,
	PERIC0_CLK_CON_DIV_DIV_CLK_PERIC0_USI8_USI,
	PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C1_IPCLKPORT_I_PCLK,
	PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C1_IPCLKPORT_I_SCLK,
	PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C2_IPCLKPORT_I_PCLK,
	PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C2_IPCLKPORT_I_SCLK,
	PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C3_IPCLKPORT_I_PCLK,
	PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C3_IPCLKPORT_I_SCLK,
	PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C4_IPCLKPORT_I_PCLK,
	PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C4_IPCLKPORT_I_SCLK,
	PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C5_IPCLKPORT_I_PCLK,
	PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C5_IPCLKPORT_I_SCLK,
	PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C6_IPCLKPORT_I_PCLK,
	PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C6_IPCLKPORT_I_SCLK,
	PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C7_IPCLKPORT_I_PCLK,
	PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C7_IPCLKPORT_I_SCLK,
	PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C8_IPCLKPORT_I_PCLK,
	PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C8_IPCLKPORT_I_SCLK,
	PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_LH_AXI_SI_P_PERIC0_CU_IPCLKPORT_I_CLK,
	PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_PERIC0_CMU_PERIC0_IPCLKPORT_PCLK,
	PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_RSTNSYNC_CLK_PERIC0_NOCP_LH_IPCLKPORT_CLK,
	PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_RSTNSYNC_CLK_PERIC0_OSCCLK_IPCLKPORT_CLK,
	PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_SLH_AXI_MI_P_PERIC0_IPCLKPORT_I_CLK,
	PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI0_UART_IPCLKPORT_IPCLK,
	PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI0_UART_IPCLKPORT_PCLK,
	PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI14_USI_IPCLKPORT_IPCLK,
	PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI14_USI_IPCLKPORT_PCLK,
	PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI1_USI_IPCLKPORT_IPCLK,
	PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI1_USI_IPCLKPORT_PCLK,
	PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI2_USI_IPCLKPORT_IPCLK,
	PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI2_USI_IPCLKPORT_PCLK,
	PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI3_USI_IPCLKPORT_IPCLK,
	PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI3_USI_IPCLKPORT_PCLK,
	PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI4_USI_IPCLKPORT_IPCLK,
	PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI4_USI_IPCLKPORT_PCLK,
	PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI5_USI_IPCLKPORT_IPCLK,
	PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI5_USI_IPCLKPORT_PCLK,
	PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI6_USI_IPCLKPORT_IPCLK,
	PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI6_USI_IPCLKPORT_PCLK,
	PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI7_USI_IPCLKPORT_IPCLK,
	PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI7_USI_IPCLKPORT_PCLK,
	PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI8_USI_IPCLKPORT_IPCLK,
	PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI8_USI_IPCLKPORT_PCLK,
	PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_D_TZPC_PERIC0_IPCLKPORT_PCLK,
	PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_GPC_PERIC0_IPCLKPORT_PCLK,
	PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_GPIO_PERIC0_IPCLKPORT_PCLK,
	PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_LH_AXI_MI_P_PERIC0_CU_IPCLKPORT_I_CLK,
	PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_RSTNSYNC_CLK_PERIC0_I3C_IPCLKPORT_CLK,
	PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_RSTNSYNC_CLK_PERIC0_NOCP_IPCLKPORT_CLK,
	PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_RSTNSYNC_CLK_PERIC0_USI0_UART_IPCLKPORT_CLK,
	PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_RSTNSYNC_CLK_PERIC0_USI14_USI_IPCLKPORT_CLK,
	PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_RSTNSYNC_CLK_PERIC0_USI1_USI_IPCLKPORT_CLK,
	PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_RSTNSYNC_CLK_PERIC0_USI2_USI_IPCLKPORT_CLK,
	PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_RSTNSYNC_CLK_PERIC0_USI3_USI_IPCLKPORT_CLK,
	PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_RSTNSYNC_CLK_PERIC0_USI4_USI_IPCLKPORT_CLK,
	PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_RSTNSYNC_CLK_PERIC0_USI5_USI_IPCLKPORT_CLK,
	PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_RSTNSYNC_CLK_PERIC0_USI6_USI_IPCLKPORT_CLK,
	PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_RSTNSYNC_CLK_PERIC0_USI7_USI_IPCLKPORT_CLK,
	PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_RSTNSYNC_CLK_PERIC0_USI8_USI_IPCLKPORT_CLK,
	PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_SYSREG_PERIC0_IPCLKPORT_PCLK,
};

#define CLKS_NR_PERIC0 83

PNAME(mout_peric0_noc_user_p) = { "oscclk", "dout_cmu_peric0_noc" };
PNAME(mout_peric0_i3c_user_p) = { "oscclk", "dout_cmu_peric0_ip" };
PNAME(mout_peric0_usi0_uart_user_p) = { "oscclk", "dout_cmu_peric0_ip" };
PNAME(mout_peric0_usi14_usi_user_p) = { "oscclk", "dout_cmu_peric0_ip" };
PNAME(mout_peric0_usi1_usi_user_p) = { "oscclk", "dout_cmu_peric0_ip" };
PNAME(mout_peric0_usi2_usi_user_p) = { "oscclk", "dout_cmu_peric0_ip" };
PNAME(mout_peric0_usi3_usi_user_p) = { "oscclk", "dout_cmu_peric0_ip" };
PNAME(mout_peric0_usi4_usi_user_p) = { "oscclk", "dout_cmu_peric0_ip" };
PNAME(mout_peric0_usi5_usi_user_p) = { "oscclk", "dout_cmu_peric0_ip" };
PNAME(mout_peric0_usi6_usi_user_p) = { "oscclk", "dout_cmu_peric0_ip" };
PNAME(mout_peric0_usi7_usi_user_p) = { "oscclk", "dout_cmu_peric0_ip" };
PNAME(mout_peric0_usi8_usi_user_p) = { "oscclk", "dout_cmu_peric0_ip" };

static const struct samsung_mux_clock peric0_mux_clks[] __initconst = {
	MUX(CLK_MOUT_PERIC0_NOC_USER, "mout_peric0_noc_user",
	    mout_peric0_noc_user_p, PERIC0_PLL_CON0_MUX_CLKCMU_PERIC0_NOC_USER,
	    4, 1),
	MUX(CLK_MOUT_PERIC0_I3C_USER, "mout_peric0_i3c_user",
	    mout_peric0_i3c_user_p, PERIC0_PLL_CON0_MUX_CLKCMU_PERIC0_I3C_USER,
	    4, 1),
	MUX(CLK_MOUT_PERIC0_USI0_UART_USER, "mout_peric0_usi0_uart_user",
	    mout_peric0_usi0_uart_user_p,
	    PERIC0_PLL_CON0_MUX_CLKCMU_PERIC0_USI0_UART_USER, 4, 1),
	MUX(CLK_MOUT_PERIC0_USI14_USI_USER, "mout_peric0_usi14_usi_user",
	    mout_peric0_usi14_usi_user_p,
	    PERIC0_PLL_CON0_MUX_CLKCMU_PERIC0_USI14_USI_USER, 4, 1),
	MUX(CLK_MOUT_PERIC0_USI1_USI_USER, "mout_peric0_usi1_usi_user",
	    mout_peric0_usi1_usi_user_p,
	    PERIC0_PLL_CON0_MUX_CLKCMU_PERIC0_USI1_USI_USER, 4, 1),
	MUX(CLK_MOUT_PERIC0_USI2_USI_USER, "mout_peric0_usi2_usi_user",
	    mout_peric0_usi2_usi_user_p,
	    PERIC0_PLL_CON0_MUX_CLKCMU_PERIC0_USI2_USI_USER, 4, 1),
	MUX(CLK_MOUT_PERIC0_USI3_USI_USER, "mout_peric0_usi3_usi_user",
	    mout_peric0_usi3_usi_user_p,
	    PERIC0_PLL_CON0_MUX_CLKCMU_PERIC0_USI3_USI_USER, 4, 1),
	MUX(CLK_MOUT_PERIC0_USI4_USI_USER, "mout_peric0_usi4_usi_user",
	    mout_peric0_usi4_usi_user_p,
	    PERIC0_PLL_CON0_MUX_CLKCMU_PERIC0_USI4_USI_USER, 4, 1),
	MUX(CLK_MOUT_PERIC0_USI5_USI_USER, "mout_peric0_usi5_usi_user",
	    mout_peric0_usi5_usi_user_p,
	    PERIC0_PLL_CON0_MUX_CLKCMU_PERIC0_USI5_USI_USER, 4, 1),
	MUX(CLK_MOUT_PERIC0_USI6_USI_USER, "mout_peric0_usi6_usi_user",
	    mout_peric0_usi6_usi_user_p,
	    PERIC0_PLL_CON0_MUX_CLKCMU_PERIC0_USI6_USI_USER, 4, 1),
	MUX(CLK_MOUT_PERIC0_USI7_USI_USER, "mout_peric0_usi7_usi_user",
	    mout_peric0_usi7_usi_user_p,
	    PERIC0_PLL_CON0_MUX_CLKCMU_PERIC0_USI7_USI_USER, 4, 1),
	MUX(CLK_MOUT_PERIC0_USI8_USI_USER, "mout_peric0_usi8_usi_user",
	    mout_peric0_usi8_usi_user_p,
	    PERIC0_PLL_CON0_MUX_CLKCMU_PERIC0_USI8_USI_USER, 4, 1),
};

static const struct samsung_div_clock peric0_div_clks[] __initconst = {
	DIV(CLK_DOUT_PERIC0_I3C, "dout_peric0_i3c", "mout_peric0_i3c_user",
	    PERIC0_CLK_CON_DIV_DIV_CLK_PERIC0_I3C, 0, 4),
	DIV(CLK_DOUT_PERIC0_NOCP_LH, "dout_peric0_nocp_lh",
	    "mout_peric0_noc_user", PERIC0_CLK_CON_DIV_DIV_CLK_PERIC0_NOCP_LH,
	    0, 3),
	DIV(CLK_DOUT_PERIC0_USI0_UART, "dout_peric0_usi0_uart",
	    "mout_peric0_usi0_uart_user",
	    PERIC0_CLK_CON_DIV_DIV_CLK_PERIC0_USI0_UART, 0, 4),
	DIV(CLK_DOUT_PERIC0_USI14_USI, "dout_peric0_usi14_usi",
	    "mout_peric0_usi14_usi_user",
	    PERIC0_CLK_CON_DIV_DIV_CLK_PERIC0_USI14_USI, 0, 4),
	DIV(CLK_DOUT_PERIC0_USI1_USI, "dout_peric0_usi1_usi",
	    "mout_peric0_usi1_usi_user",
	    PERIC0_CLK_CON_DIV_DIV_CLK_PERIC0_USI1_USI, 0, 4),
	DIV(CLK_DOUT_PERIC0_USI2_USI, "dout_peric0_usi2_usi",
	    "mout_peric0_usi2_usi_user",
	    PERIC0_CLK_CON_DIV_DIV_CLK_PERIC0_USI2_USI, 0, 4),
	DIV(CLK_DOUT_PERIC0_USI3_USI, "dout_peric0_usi3_usi",
	    "mout_peric0_usi3_usi_user",
	    PERIC0_CLK_CON_DIV_DIV_CLK_PERIC0_USI3_USI, 0, 4),
	DIV(CLK_DOUT_PERIC0_USI4_USI, "dout_peric0_usi4_usi",
	    "mout_peric0_usi4_usi_user",
	    PERIC0_CLK_CON_DIV_DIV_CLK_PERIC0_USI4_USI, 0, 4),
	DIV(CLK_DOUT_PERIC0_USI5_USI, "dout_peric0_usi5_usi",
	    "mout_peric0_usi5_usi_user",
	    PERIC0_CLK_CON_DIV_DIV_CLK_PERIC0_USI5_USI, 0, 4),
	DIV(CLK_DOUT_PERIC0_USI6_USI, "dout_peric0_usi6_usi",
	    "mout_peric0_usi6_usi_user",
	    PERIC0_CLK_CON_DIV_DIV_CLK_PERIC0_USI6_USI, 0, 4),
	DIV(CLK_DOUT_PERIC0_USI7_USI, "dout_peric0_usi7_usi",
	    "mout_peric0_usi7_usi_user",
	    PERIC0_CLK_CON_DIV_DIV_CLK_PERIC0_USI7_USI, 0, 4),
	DIV(CLK_DOUT_PERIC0_USI8_USI, "dout_peric0_usi8_usi",
	    "mout_peric0_usi8_usi_user",
	    PERIC0_CLK_CON_DIV_DIV_CLK_PERIC0_USI8_USI, 0, 4),
};

static const struct samsung_gate_clock peric0_gate_clks[] __initconst = {
	GATE(CLK_GOUT_PERIC0_I3C1_I_PCLK, "gout_peric0_i3c1_i_pclk", "mout_peric0_noc_user",
	     PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C1_IPCLKPORT_I_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC0_I3C1_I_SCLK, "gout_peric0_i3c1_i_sclk", "dout_peric0_i3c",
	     PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C1_IPCLKPORT_I_SCLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC0_I3C2_I_PCLK, "gout_peric0_i3c2_i_pclk", "mout_peric0_noc_user",
	     PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C2_IPCLKPORT_I_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC0_I3C2_I_SCLK, "gout_peric0_i3c2_i_sclk", "dout_peric0_i3c",
	     PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C2_IPCLKPORT_I_SCLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC0_I3C3_I_PCLK, "gout_peric0_i3c3_i_pclk", "mout_peric0_noc_user",
	     PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C3_IPCLKPORT_I_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC0_I3C3_I_SCLK, "gout_peric0_i3c3_i_sclk", "dout_peric0_i3c",
	     PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C3_IPCLKPORT_I_SCLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC0_I3C4_I_PCLK, "gout_peric0_i3c4_i_pclk", "mout_peric0_noc_user",
	     PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C4_IPCLKPORT_I_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC0_I3C4_I_SCLK, "gout_peric0_i3c4_i_sclk", "dout_peric0_i3c",
	     PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C4_IPCLKPORT_I_SCLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC0_I3C5_I_PCLK, "gout_peric0_i3c5_i_pclk", "mout_peric0_noc_user",
	     PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C5_IPCLKPORT_I_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC0_I3C5_I_SCLK, "gout_peric0_i3c5_i_sclk", "dout_peric0_i3c",
	     PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C5_IPCLKPORT_I_SCLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC0_I3C6_I_PCLK, "gout_peric0_i3c6_i_pclk", "mout_peric0_noc_user",
	     PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C6_IPCLKPORT_I_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC0_I3C6_I_SCLK, "gout_peric0_i3c6_i_sclk", "dout_peric0_i3c",
	     PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C6_IPCLKPORT_I_SCLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC0_I3C7_I_PCLK, "gout_peric0_i3c7_i_pclk", "mout_peric0_noc_user",
	     PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C7_IPCLKPORT_I_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC0_I3C7_I_SCLK, "gout_peric0_i3c7_i_sclk", "dout_peric0_i3c",
	     PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C7_IPCLKPORT_I_SCLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC0_I3C8_I_PCLK, "gout_peric0_i3c8_i_pclk", "mout_peric0_noc_user",
	     PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C8_IPCLKPORT_I_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC0_I3C8_I_SCLK, "gout_peric0_i3c8_i_sclk", "dout_peric0_i3c",
	     PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_I3C8_IPCLKPORT_I_SCLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC0_LH_AXI_SI_P_PERIC0_CU_I_CLK, "gout_peric0_lh_axi_si_p_peric0_cu_i_clk",
	     "dout_peric0_nocp_lh",
	     PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_LH_AXI_SI_P_PERIC0_CU_IPCLKPORT_I_CLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC0_PERIC0_CMU_PERIC0_PCLK, "gout_peric0_peric0_cmu_peric0_pclk",
	     "mout_peric0_noc_user",
	     PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_PERIC0_CMU_PERIC0_IPCLKPORT_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC0_RSTNSYNC_CLK_PERIC0_NOCP_LH_CLK,
	     "gout_peric0_rstnsync_clk_peric0_nocp_lh_clk", "dout_peric0_nocp_lh",
	     PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_RSTNSYNC_CLK_PERIC0_NOCP_LH_IPCLKPORT_CLK, 21, 0,
	     0),
	GATE(CLK_GOUT_PERIC0_RSTNSYNC_CLK_PERIC0_OSCCLK_CLK,
	     "gout_peric0_rstnsync_clk_peric0_oscclk_clk", "oscclk",
	     PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_RSTNSYNC_CLK_PERIC0_OSCCLK_IPCLKPORT_CLK, 21, 0,
	     0),
	GATE(CLK_GOUT_PERIC0_SLH_AXI_MI_P_PERIC0_I_CLK, "gout_peric0_slh_axi_mi_p_peric0_i_clk",
	     "dout_peric0_nocp_lh",
	     PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_SLH_AXI_MI_P_PERIC0_IPCLKPORT_I_CLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC0_USI0_UART_IPCLK, "gout_peric0_usi0_uart_ipclk",
	     "dout_peric0_usi0_uart",
	     PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI0_UART_IPCLKPORT_IPCLK, 21,
	     CLK_SET_RATE_PARENT, 0),
	GATE(CLK_GOUT_PERIC0_USI0_UART_PCLK, "gout_peric0_usi0_uart_pclk", "mout_peric0_noc_user",
	     PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI0_UART_IPCLKPORT_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC0_USI14_USI_IPCLK, "gout_peric0_usi14_usi_ipclk",
	     "dout_peric0_usi14_usi",
	     PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI14_USI_IPCLKPORT_IPCLK, 21,
	     CLK_SET_RATE_PARENT, 0),
	GATE(CLK_GOUT_PERIC0_USI14_USI_PCLK, "gout_peric0_usi14_usi_pclk", "mout_peric0_noc_user",
	     PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI14_USI_IPCLKPORT_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC0_USI1_USI_IPCLK, "gout_peric0_usi1_usi_ipclk", "dout_peric0_usi1_usi",
	     PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI1_USI_IPCLKPORT_IPCLK, 21,
	     CLK_SET_RATE_PARENT, 0),
	GATE(CLK_GOUT_PERIC0_USI1_USI_PCLK, "gout_peric0_usi1_usi_pclk", "mout_peric0_noc_user",
	     PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI1_USI_IPCLKPORT_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC0_USI2_USI_IPCLK, "gout_peric0_usi2_usi_ipclk", "dout_peric0_usi2_usi",
	     PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI2_USI_IPCLKPORT_IPCLK, 21,
	     CLK_SET_RATE_PARENT, 0),
	GATE(CLK_GOUT_PERIC0_USI2_USI_PCLK, "gout_peric0_usi2_usi_pclk", "mout_peric0_noc_user",
	     PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI2_USI_IPCLKPORT_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC0_USI3_USI_IPCLK, "gout_peric0_usi3_usi_ipclk", "dout_peric0_usi3_usi",
	     PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI3_USI_IPCLKPORT_IPCLK, 21,
	     CLK_SET_RATE_PARENT, 0),
	GATE(CLK_GOUT_PERIC0_USI3_USI_PCLK, "gout_peric0_usi3_usi_pclk", "mout_peric0_noc_user",
	     PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI3_USI_IPCLKPORT_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC0_USI4_USI_IPCLK, "gout_peric0_usi4_usi_ipclk", "dout_peric0_usi4_usi",
	     PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI4_USI_IPCLKPORT_IPCLK, 21,
	     CLK_SET_RATE_PARENT, 0),
	GATE(CLK_GOUT_PERIC0_USI4_USI_PCLK, "gout_peric0_usi4_usi_pclk", "mout_peric0_noc_user",
	     PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI4_USI_IPCLKPORT_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC0_USI5_USI_IPCLK, "gout_peric0_usi5_usi_ipclk", "dout_peric0_usi5_usi",
	     PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI5_USI_IPCLKPORT_IPCLK, 21,
	     CLK_SET_RATE_PARENT, 0),
	GATE(CLK_GOUT_PERIC0_USI5_USI_PCLK, "gout_peric0_usi5_usi_pclk", "mout_peric0_noc_user",
	     PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI5_USI_IPCLKPORT_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC0_USI6_USI_IPCLK, "gout_peric0_usi6_usi_ipclk", "dout_peric0_usi6_usi",
	     PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI6_USI_IPCLKPORT_IPCLK, 21,
	     CLK_SET_RATE_PARENT, 0),
	GATE(CLK_GOUT_PERIC0_USI6_USI_PCLK, "gout_peric0_usi6_usi_pclk", "mout_peric0_noc_user",
	     PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI6_USI_IPCLKPORT_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC0_USI7_USI_IPCLK, "gout_peric0_usi7_usi_ipclk", "dout_peric0_usi7_usi",
	     PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI7_USI_IPCLKPORT_IPCLK, 21,
	     CLK_SET_RATE_PARENT, 0),
	GATE(CLK_GOUT_PERIC0_USI7_USI_PCLK, "gout_peric0_usi7_usi_pclk", "mout_peric0_noc_user",
	     PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI7_USI_IPCLKPORT_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC0_USI8_USI_IPCLK, "gout_peric0_usi8_usi_ipclk", "dout_peric0_usi8_usi",
	     PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI8_USI_IPCLKPORT_IPCLK, 21,
	     CLK_SET_RATE_PARENT, 0),
	GATE(CLK_GOUT_PERIC0_USI8_USI_PCLK, "gout_peric0_usi8_usi_pclk", "mout_peric0_noc_user",
	     PERIC0_CLK_CON_GAT_CLK_BLK_PERIC0_UID_USI8_USI_IPCLKPORT_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC0_D_TZPC_PERIC0_PCLK, "gout_peric0_d_tzpc_peric0_pclk",
	     "mout_peric0_noc_user",
	     PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_D_TZPC_PERIC0_IPCLKPORT_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC0_GPC_PERIC0_PCLK, "gout_peric0_gpc_peric0_pclk", "mout_peric0_noc_user",
	     PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_GPC_PERIC0_IPCLKPORT_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC0_GPIO_PERIC0_PCLK, "gout_peric0_gpio_peric0_pclk",
	     "mout_peric0_noc_user",
	     PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_GPIO_PERIC0_IPCLKPORT_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC0_LH_AXI_MI_P_PERIC0_CU_I_CLK, "gout_peric0_lh_axi_mi_p_peric0_cu_i_clk",
	     "mout_peric0_noc_user",
	     PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_LH_AXI_MI_P_PERIC0_CU_IPCLKPORT_I_CLK, 21, 0,
	     0),
	GATE(CLK_GOUT_PERIC0_RSTNSYNC_CLK_PERIC0_I3C_CLK, "gout_peric0_rstnsync_clk_peric0_i3c_clk",
	     "dout_peric0_i3c",
	     PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_RSTNSYNC_CLK_PERIC0_I3C_IPCLKPORT_CLK, 21, 0,
	     0),
	GATE(CLK_GOUT_PERIC0_RSTNSYNC_CLK_PERIC0_NOCP_CLK,
	     "gout_peric0_rstnsync_clk_peric0_nocp_clk", "mout_peric0_noc_user",
	     PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_RSTNSYNC_CLK_PERIC0_NOCP_IPCLKPORT_CLK, 21, 0,
	     0),
	GATE(CLK_GOUT_PERIC0_RSTNSYNC_CLK_PERIC0_USI0_UART_CLK,
	     "gout_peric0_rstnsync_clk_peric0_usi0_uart_clk", "dout_peric0_usi0_uart",
	     PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_RSTNSYNC_CLK_PERIC0_USI0_UART_IPCLKPORT_CLK, 21,
	     0, 0),
	GATE(CLK_GOUT_PERIC0_RSTNSYNC_CLK_PERIC0_USI14_USI_CLK,
	     "gout_peric0_rstnsync_clk_peric0_usi14_usi_clk", "dout_peric0_usi14_usi",
	     PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_RSTNSYNC_CLK_PERIC0_USI14_USI_IPCLKPORT_CLK, 21,
	     0, 0),
	GATE(CLK_GOUT_PERIC0_RSTNSYNC_CLK_PERIC0_USI1_USI_CLK,
	     "gout_peric0_rstnsync_clk_peric0_usi1_usi_clk", "dout_peric0_usi1_usi",
	     PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_RSTNSYNC_CLK_PERIC0_USI1_USI_IPCLKPORT_CLK, 21,
	     0, 0),
	GATE(CLK_GOUT_PERIC0_RSTNSYNC_CLK_PERIC0_USI2_USI_CLK,
	     "gout_peric0_rstnsync_clk_peric0_usi2_usi_clk", "dout_peric0_usi2_usi",
	     PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_RSTNSYNC_CLK_PERIC0_USI2_USI_IPCLKPORT_CLK, 21,
	     0, 0),
	GATE(CLK_GOUT_PERIC0_RSTNSYNC_CLK_PERIC0_USI3_USI_CLK,
	     "gout_peric0_rstnsync_clk_peric0_usi3_usi_clk", "dout_peric0_usi3_usi",
	     PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_RSTNSYNC_CLK_PERIC0_USI3_USI_IPCLKPORT_CLK, 21,
	     0, 0),
	GATE(CLK_GOUT_PERIC0_RSTNSYNC_CLK_PERIC0_USI4_USI_CLK,
	     "gout_peric0_rstnsync_clk_peric0_usi4_usi_clk", "dout_peric0_usi4_usi",
	     PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_RSTNSYNC_CLK_PERIC0_USI4_USI_IPCLKPORT_CLK, 21,
	     0, 0),
	GATE(CLK_GOUT_PERIC0_RSTNSYNC_CLK_PERIC0_USI5_USI_CLK,
	     "gout_peric0_rstnsync_clk_peric0_usi5_usi_clk", "dout_peric0_usi5_usi",
	     PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_RSTNSYNC_CLK_PERIC0_USI5_USI_IPCLKPORT_CLK, 21,
	     0, 0),
	GATE(CLK_GOUT_PERIC0_RSTNSYNC_CLK_PERIC0_USI6_USI_CLK,
	     "gout_peric0_rstnsync_clk_peric0_usi6_usi_clk", "dout_peric0_usi6_usi",
	     PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_RSTNSYNC_CLK_PERIC0_USI6_USI_IPCLKPORT_CLK, 21,
	     0, 0),
	GATE(CLK_GOUT_PERIC0_RSTNSYNC_CLK_PERIC0_USI7_USI_CLK,
	     "gout_peric0_rstnsync_clk_peric0_usi7_usi_clk", "dout_peric0_usi7_usi",
	     PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_RSTNSYNC_CLK_PERIC0_USI7_USI_IPCLKPORT_CLK, 21,
	     0, 0),
	GATE(CLK_GOUT_PERIC0_RSTNSYNC_CLK_PERIC0_USI8_USI_CLK,
	     "gout_peric0_rstnsync_clk_peric0_usi8_usi_clk", "dout_peric0_usi8_usi",
	     PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_RSTNSYNC_CLK_PERIC0_USI8_USI_IPCLKPORT_CLK, 21,
	     0, 0),
	GATE(CLK_GOUT_PERIC0_SYSREG_PERIC0_PCLK, "gout_peric0_sysreg_peric0_pclk",
	     "mout_peric0_noc_user",
	     PERIC0_CLK_CON_GAT_GOUT_BLK_PERIC0_UID_SYSREG_PERIC0_IPCLKPORT_PCLK, 21, 0, 0),
};

static const struct samsung_cmu_info peric0_cmu_info __initconst = {
	.mux_clks = peric0_mux_clks,
	.nr_mux_clks = ARRAY_SIZE(peric0_mux_clks),
	.div_clks = peric0_div_clks,
	.nr_div_clks = ARRAY_SIZE(peric0_div_clks),
	.gate_clks = peric0_gate_clks,
	.nr_gate_clks = ARRAY_SIZE(peric0_gate_clks),
	.nr_clk_ids = CLKS_NR_PERIC0,
	.clk_regs = peric0_clk_regs,
	.nr_clk_regs = ARRAY_SIZE(peric0_clk_regs),
	.clk_name = "bus",
};

/* ---- CMU_PERIC1 ------------------------------------------------------------- */
/* Register offsets at 0x10c00000. */
#define PERIC1_PLL_CON0_MUX_CLKCMU_PERIC1_I3C_USER 0x0600
#define PERIC1_PLL_CON0_MUX_CLKCMU_PERIC1_NOC_USER 0x0610
#define PERIC1_PLL_CON0_MUX_CLKCMU_PERIC1_USI0_USI_USER 0x0620
#define PERIC1_PLL_CON0_MUX_CLKCMU_PERIC1_USI10_USI_USER 0x0630
#define PERIC1_PLL_CON0_MUX_CLKCMU_PERIC1_USI11_USI_USER 0x0640
#define PERIC1_PLL_CON0_MUX_CLKCMU_PERIC1_USI12_USI_USER 0x0650
#define PERIC1_PLL_CON0_MUX_CLKCMU_PERIC1_USI13_USI_USER 0x0660
#define PERIC1_PLL_CON0_MUX_CLKCMU_PERIC1_USI15_USI_USER 0x0670
#define PERIC1_PLL_CON0_MUX_CLKCMU_PERIC1_USI16_USI_USER 0x0680
#define PERIC1_PLL_CON0_MUX_CLKCMU_PERIC1_USI9_USI_USER 0x0690
#define PERIC1_CLK_CON_DIV_DIV_CLK_PERIC1_I3C 0x1800
#define PERIC1_CLK_CON_DIV_DIV_CLK_PERIC1_NOCP_LH 0x1804
#define PERIC1_CLK_CON_DIV_DIV_CLK_PERIC1_USI0_USI 0x1808
#define PERIC1_CLK_CON_DIV_DIV_CLK_PERIC1_USI10_USI 0x180c
#define PERIC1_CLK_CON_DIV_DIV_CLK_PERIC1_USI11_USI 0x1810
#define PERIC1_CLK_CON_DIV_DIV_CLK_PERIC1_USI12_USI 0x1814
#define PERIC1_CLK_CON_DIV_DIV_CLK_PERIC1_USI13_USI 0x1818
#define PERIC1_CLK_CON_DIV_DIV_CLK_PERIC1_USI15_USI 0x181c
#define PERIC1_CLK_CON_DIV_DIV_CLK_PERIC1_USI16_USI 0x1820
#define PERIC1_CLK_CON_DIV_DIV_CLK_PERIC1_USI9_USI 0x1824
#define PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_I3C0_IPCLKPORT_I_PCLK 0x2004
#define PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_I3C0_IPCLKPORT_I_SCLK 0x2008
#define PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_LH_AXI_SI_P_PERIC1_CU_IPCLKPORT_I_CLK \
	0x200c
#define PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_PERIC1_CMU_PERIC1_IPCLKPORT_PCLK \
	0x2010
#define PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_PWM_IPCLKPORT_I_PCLK_S0 0x2014
#define PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_RSTNSYNC_CLK_PERIC1_I3C_IPCLKPORT_CLK \
	0x2018
#define PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_RSTNSYNC_CLK_PERIC1_NOCP_LH_IPCLKPORT_CLK \
	0x201c
#define PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_RSTNSYNC_CLK_PERIC1_OSCCLK_IPCLKPORT_CLK \
	0x2020
#define PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_RSTNSYNC_CLK_PERIC1_USI15_USI_IPCLKPORT_CLK \
	0x2024
#define PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_RSTNSYNC_CLK_PERIC1_USI16_USI_IPCLKPORT_CLK \
	0x2028
#define PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_SLH_AXI_MI_P_PERIC1_IPCLKPORT_I_CLK \
	0x202c
#define PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI0_USI_IPCLKPORT_IPCLK 0x2030
#define PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI0_USI_IPCLKPORT_PCLK 0x2034
#define PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI10_USI_IPCLKPORT_IPCLK 0x2038
#define PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI10_USI_IPCLKPORT_PCLK 0x203c
#define PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI11_USI_IPCLKPORT_IPCLK 0x2040
#define PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI11_USI_IPCLKPORT_PCLK 0x2044
#define PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI12_USI_IPCLKPORT_IPCLK 0x2048
#define PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI12_USI_IPCLKPORT_PCLK 0x204c
#define PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI13_USI_IPCLKPORT_IPCLK 0x2050
#define PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI13_USI_IPCLKPORT_PCLK 0x2054
#define PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI15_USI_IPCLKPORT_IPCLK 0x2058
#define PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI15_USI_IPCLKPORT_PCLK 0x205c
#define PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI16_USI_IPCLKPORT_IPCLK 0x2060
#define PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI16_USI_IPCLKPORT_PCLK 0x2064
#define PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI9_USI_IPCLKPORT_IPCLK 0x2068
#define PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI9_USI_IPCLKPORT_PCLK 0x206c
#define PERIC1_CLK_CON_GAT_GOUT_BLK_PERIC1_UID_D_TZPC_PERIC1_IPCLKPORT_PCLK \
	0x2070
#define PERIC1_CLK_CON_GAT_GOUT_BLK_PERIC1_UID_GPC_PERIC1_IPCLKPORT_PCLK 0x2074
#define PERIC1_CLK_CON_GAT_GOUT_BLK_PERIC1_UID_GPIO_PERIC1_IPCLKPORT_PCLK 0x2078
#define PERIC1_CLK_CON_GAT_GOUT_BLK_PERIC1_UID_LH_AXI_MI_P_PERIC1_CU_IPCLKPORT_I_CLK \
	0x207c
#define PERIC1_CLK_CON_GAT_GOUT_BLK_PERIC1_UID_RSTNSYNC_CLK_PERIC1_NOCP_IPCLKPORT_CLK \
	0x2080
#define PERIC1_CLK_CON_GAT_GOUT_BLK_PERIC1_UID_RSTNSYNC_CLK_PERIC1_USI0_USI_IPCLKPORT_CLK \
	0x2084
#define PERIC1_CLK_CON_GAT_GOUT_BLK_PERIC1_UID_RSTNSYNC_CLK_PERIC1_USI10_USI_IPCLKPORT_CLK \
	0x2088
#define PERIC1_CLK_CON_GAT_GOUT_BLK_PERIC1_UID_RSTNSYNC_CLK_PERIC1_USI11_USI_IPCLKPORT_CLK \
	0x208c
#define PERIC1_CLK_CON_GAT_GOUT_BLK_PERIC1_UID_RSTNSYNC_CLK_PERIC1_USI12_USI_IPCLKPORT_CLK \
	0x2090
#define PERIC1_CLK_CON_GAT_GOUT_BLK_PERIC1_UID_RSTNSYNC_CLK_PERIC1_USI13_USI_IPCLKPORT_CLK \
	0x2094
#define PERIC1_CLK_CON_GAT_GOUT_BLK_PERIC1_UID_RSTNSYNC_CLK_PERIC1_USI9_USI_IPCLKPORT_CLK \
	0x2098
#define PERIC1_CLK_CON_GAT_GOUT_BLK_PERIC1_UID_SYSREG_PERIC1_IPCLKPORT_PCLK \
	0x209c

static const unsigned long peric1_clk_regs[] __initconst = {
	PERIC1_PLL_CON0_MUX_CLKCMU_PERIC1_I3C_USER,
	PERIC1_PLL_CON0_MUX_CLKCMU_PERIC1_NOC_USER,
	PERIC1_PLL_CON0_MUX_CLKCMU_PERIC1_USI0_USI_USER,
	PERIC1_PLL_CON0_MUX_CLKCMU_PERIC1_USI10_USI_USER,
	PERIC1_PLL_CON0_MUX_CLKCMU_PERIC1_USI11_USI_USER,
	PERIC1_PLL_CON0_MUX_CLKCMU_PERIC1_USI12_USI_USER,
	PERIC1_PLL_CON0_MUX_CLKCMU_PERIC1_USI13_USI_USER,
	PERIC1_PLL_CON0_MUX_CLKCMU_PERIC1_USI15_USI_USER,
	PERIC1_PLL_CON0_MUX_CLKCMU_PERIC1_USI16_USI_USER,
	PERIC1_PLL_CON0_MUX_CLKCMU_PERIC1_USI9_USI_USER,
	PERIC1_CLK_CON_DIV_DIV_CLK_PERIC1_I3C,
	PERIC1_CLK_CON_DIV_DIV_CLK_PERIC1_NOCP_LH,
	PERIC1_CLK_CON_DIV_DIV_CLK_PERIC1_USI0_USI,
	PERIC1_CLK_CON_DIV_DIV_CLK_PERIC1_USI10_USI,
	PERIC1_CLK_CON_DIV_DIV_CLK_PERIC1_USI11_USI,
	PERIC1_CLK_CON_DIV_DIV_CLK_PERIC1_USI12_USI,
	PERIC1_CLK_CON_DIV_DIV_CLK_PERIC1_USI13_USI,
	PERIC1_CLK_CON_DIV_DIV_CLK_PERIC1_USI15_USI,
	PERIC1_CLK_CON_DIV_DIV_CLK_PERIC1_USI16_USI,
	PERIC1_CLK_CON_DIV_DIV_CLK_PERIC1_USI9_USI,
	PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_I3C0_IPCLKPORT_I_PCLK,
	PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_I3C0_IPCLKPORT_I_SCLK,
	PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_LH_AXI_SI_P_PERIC1_CU_IPCLKPORT_I_CLK,
	PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_PERIC1_CMU_PERIC1_IPCLKPORT_PCLK,
	PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_PWM_IPCLKPORT_I_PCLK_S0,
	PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_RSTNSYNC_CLK_PERIC1_I3C_IPCLKPORT_CLK,
	PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_RSTNSYNC_CLK_PERIC1_NOCP_LH_IPCLKPORT_CLK,
	PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_RSTNSYNC_CLK_PERIC1_OSCCLK_IPCLKPORT_CLK,
	PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_RSTNSYNC_CLK_PERIC1_USI15_USI_IPCLKPORT_CLK,
	PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_RSTNSYNC_CLK_PERIC1_USI16_USI_IPCLKPORT_CLK,
	PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_SLH_AXI_MI_P_PERIC1_IPCLKPORT_I_CLK,
	PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI0_USI_IPCLKPORT_IPCLK,
	PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI0_USI_IPCLKPORT_PCLK,
	PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI10_USI_IPCLKPORT_IPCLK,
	PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI10_USI_IPCLKPORT_PCLK,
	PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI11_USI_IPCLKPORT_IPCLK,
	PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI11_USI_IPCLKPORT_PCLK,
	PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI12_USI_IPCLKPORT_IPCLK,
	PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI12_USI_IPCLKPORT_PCLK,
	PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI13_USI_IPCLKPORT_IPCLK,
	PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI13_USI_IPCLKPORT_PCLK,
	PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI15_USI_IPCLKPORT_IPCLK,
	PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI15_USI_IPCLKPORT_PCLK,
	PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI16_USI_IPCLKPORT_IPCLK,
	PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI16_USI_IPCLKPORT_PCLK,
	PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI9_USI_IPCLKPORT_IPCLK,
	PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI9_USI_IPCLKPORT_PCLK,
	PERIC1_CLK_CON_GAT_GOUT_BLK_PERIC1_UID_D_TZPC_PERIC1_IPCLKPORT_PCLK,
	PERIC1_CLK_CON_GAT_GOUT_BLK_PERIC1_UID_GPC_PERIC1_IPCLKPORT_PCLK,
	PERIC1_CLK_CON_GAT_GOUT_BLK_PERIC1_UID_GPIO_PERIC1_IPCLKPORT_PCLK,
	PERIC1_CLK_CON_GAT_GOUT_BLK_PERIC1_UID_LH_AXI_MI_P_PERIC1_CU_IPCLKPORT_I_CLK,
	PERIC1_CLK_CON_GAT_GOUT_BLK_PERIC1_UID_RSTNSYNC_CLK_PERIC1_NOCP_IPCLKPORT_CLK,
	PERIC1_CLK_CON_GAT_GOUT_BLK_PERIC1_UID_RSTNSYNC_CLK_PERIC1_USI0_USI_IPCLKPORT_CLK,
	PERIC1_CLK_CON_GAT_GOUT_BLK_PERIC1_UID_RSTNSYNC_CLK_PERIC1_USI10_USI_IPCLKPORT_CLK,
	PERIC1_CLK_CON_GAT_GOUT_BLK_PERIC1_UID_RSTNSYNC_CLK_PERIC1_USI11_USI_IPCLKPORT_CLK,
	PERIC1_CLK_CON_GAT_GOUT_BLK_PERIC1_UID_RSTNSYNC_CLK_PERIC1_USI12_USI_IPCLKPORT_CLK,
	PERIC1_CLK_CON_GAT_GOUT_BLK_PERIC1_UID_RSTNSYNC_CLK_PERIC1_USI13_USI_IPCLKPORT_CLK,
	PERIC1_CLK_CON_GAT_GOUT_BLK_PERIC1_UID_RSTNSYNC_CLK_PERIC1_USI9_USI_IPCLKPORT_CLK,
	PERIC1_CLK_CON_GAT_GOUT_BLK_PERIC1_UID_SYSREG_PERIC1_IPCLKPORT_PCLK,
};

#define CLKS_NR_PERIC1 60

PNAME(mout_peric1_i3c_user_p) = { "oscclk", "dout_cmu_peric1_ip" };
PNAME(mout_peric1_noc_user_p) = { "oscclk", "dout_cmu_peric1_noc" };
PNAME(mout_peric1_usi0_usi_user_p) = { "oscclk", "dout_cmu_peric1_ip" };
PNAME(mout_peric1_usi10_usi_user_p) = { "oscclk", "dout_cmu_peric1_ip" };
PNAME(mout_peric1_usi11_usi_user_p) = { "oscclk", "dout_cmu_peric1_ip" };
PNAME(mout_peric1_usi12_usi_user_p) = { "oscclk", "dout_cmu_peric1_ip" };
PNAME(mout_peric1_usi13_usi_user_p) = { "oscclk", "dout_cmu_peric1_ip" };
PNAME(mout_peric1_usi15_usi_user_p) = { "oscclk", "dout_cmu_peric1_ip" };
PNAME(mout_peric1_usi16_usi_user_p) = { "oscclk", "dout_cmu_peric1_ip" };
PNAME(mout_peric1_usi9_usi_user_p) = { "oscclk", "dout_cmu_peric1_ip" };

static const struct samsung_mux_clock peric1_mux_clks[] __initconst = {
	MUX(CLK_MOUT_PERIC1_I3C_USER, "mout_peric1_i3c_user",
	    mout_peric1_i3c_user_p, PERIC1_PLL_CON0_MUX_CLKCMU_PERIC1_I3C_USER,
	    4, 1),
	MUX(CLK_MOUT_PERIC1_NOC_USER, "mout_peric1_noc_user",
	    mout_peric1_noc_user_p, PERIC1_PLL_CON0_MUX_CLKCMU_PERIC1_NOC_USER,
	    4, 1),
	MUX(CLK_MOUT_PERIC1_USI0_USI_USER, "mout_peric1_usi0_usi_user",
	    mout_peric1_usi0_usi_user_p,
	    PERIC1_PLL_CON0_MUX_CLKCMU_PERIC1_USI0_USI_USER, 4, 1),
	MUX(CLK_MOUT_PERIC1_USI10_USI_USER, "mout_peric1_usi10_usi_user",
	    mout_peric1_usi10_usi_user_p,
	    PERIC1_PLL_CON0_MUX_CLKCMU_PERIC1_USI10_USI_USER, 4, 1),
	MUX(CLK_MOUT_PERIC1_USI11_USI_USER, "mout_peric1_usi11_usi_user",
	    mout_peric1_usi11_usi_user_p,
	    PERIC1_PLL_CON0_MUX_CLKCMU_PERIC1_USI11_USI_USER, 4, 1),
	MUX(CLK_MOUT_PERIC1_USI12_USI_USER, "mout_peric1_usi12_usi_user",
	    mout_peric1_usi12_usi_user_p,
	    PERIC1_PLL_CON0_MUX_CLKCMU_PERIC1_USI12_USI_USER, 4, 1),
	MUX(CLK_MOUT_PERIC1_USI13_USI_USER, "mout_peric1_usi13_usi_user",
	    mout_peric1_usi13_usi_user_p,
	    PERIC1_PLL_CON0_MUX_CLKCMU_PERIC1_USI13_USI_USER, 4, 1),
	MUX(CLK_MOUT_PERIC1_USI15_USI_USER, "mout_peric1_usi15_usi_user",
	    mout_peric1_usi15_usi_user_p,
	    PERIC1_PLL_CON0_MUX_CLKCMU_PERIC1_USI15_USI_USER, 4, 1),
	MUX(CLK_MOUT_PERIC1_USI16_USI_USER, "mout_peric1_usi16_usi_user",
	    mout_peric1_usi16_usi_user_p,
	    PERIC1_PLL_CON0_MUX_CLKCMU_PERIC1_USI16_USI_USER, 4, 1),
	MUX(CLK_MOUT_PERIC1_USI9_USI_USER, "mout_peric1_usi9_usi_user",
	    mout_peric1_usi9_usi_user_p,
	    PERIC1_PLL_CON0_MUX_CLKCMU_PERIC1_USI9_USI_USER, 4, 1),
};

static const struct samsung_div_clock peric1_div_clks[] __initconst = {
	DIV(CLK_DOUT_PERIC1_I3C, "dout_peric1_i3c", "mout_peric1_i3c_user",
	    PERIC1_CLK_CON_DIV_DIV_CLK_PERIC1_I3C, 0, 4),
	DIV(CLK_DOUT_PERIC1_NOCP_LH, "dout_peric1_nocp_lh",
	    "mout_peric1_noc_user", PERIC1_CLK_CON_DIV_DIV_CLK_PERIC1_NOCP_LH,
	    0, 3),
	DIV(CLK_DOUT_PERIC1_USI0_USI, "dout_peric1_usi0_usi",
	    "mout_peric1_usi0_usi_user",
	    PERIC1_CLK_CON_DIV_DIV_CLK_PERIC1_USI0_USI, 0, 4),
	DIV(CLK_DOUT_PERIC1_USI10_USI, "dout_peric1_usi10_usi",
	    "mout_peric1_usi10_usi_user",
	    PERIC1_CLK_CON_DIV_DIV_CLK_PERIC1_USI10_USI, 0, 4),
	DIV(CLK_DOUT_PERIC1_USI11_USI, "dout_peric1_usi11_usi",
	    "mout_peric1_usi11_usi_user",
	    PERIC1_CLK_CON_DIV_DIV_CLK_PERIC1_USI11_USI, 0, 4),
	DIV(CLK_DOUT_PERIC1_USI12_USI, "dout_peric1_usi12_usi",
	    "mout_peric1_usi12_usi_user",
	    PERIC1_CLK_CON_DIV_DIV_CLK_PERIC1_USI12_USI, 0, 4),
	DIV(CLK_DOUT_PERIC1_USI13_USI, "dout_peric1_usi13_usi",
	    "mout_peric1_usi13_usi_user",
	    PERIC1_CLK_CON_DIV_DIV_CLK_PERIC1_USI13_USI, 0, 4),
	DIV(CLK_DOUT_PERIC1_USI15_USI, "dout_peric1_usi15_usi",
	    "mout_peric1_usi15_usi_user",
	    PERIC1_CLK_CON_DIV_DIV_CLK_PERIC1_USI15_USI, 0, 4),
	DIV(CLK_DOUT_PERIC1_USI16_USI, "dout_peric1_usi16_usi",
	    "mout_peric1_usi16_usi_user",
	    PERIC1_CLK_CON_DIV_DIV_CLK_PERIC1_USI16_USI, 0, 4),
	DIV(CLK_DOUT_PERIC1_USI9_USI, "dout_peric1_usi9_usi",
	    "mout_peric1_usi9_usi_user",
	    PERIC1_CLK_CON_DIV_DIV_CLK_PERIC1_USI9_USI, 0, 4),
};

static const struct samsung_gate_clock peric1_gate_clks[] __initconst = {
	GATE(CLK_GOUT_PERIC1_I3C0_I_PCLK, "gout_peric1_i3c0_i_pclk", "mout_peric1_noc_user",
	     PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_I3C0_IPCLKPORT_I_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC1_I3C0_I_SCLK, "gout_peric1_i3c0_i_sclk", "dout_peric1_i3c",
	     PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_I3C0_IPCLKPORT_I_SCLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC1_LH_AXI_SI_P_PERIC1_CU_I_CLK, "gout_peric1_lh_axi_si_p_peric1_cu_i_clk",
	     "dout_peric1_nocp_lh",
	     PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_LH_AXI_SI_P_PERIC1_CU_IPCLKPORT_I_CLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC1_PERIC1_CMU_PERIC1_PCLK, "gout_peric1_peric1_cmu_peric1_pclk",
	     "mout_peric1_noc_user",
	     PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_PERIC1_CMU_PERIC1_IPCLKPORT_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC1_PWM_I_PCLK_S0, "gout_peric1_pwm_i_pclk_s0", "mout_peric1_noc_user",
	     PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_PWM_IPCLKPORT_I_PCLK_S0, 21, 0, 0),
	GATE(CLK_GOUT_PERIC1_RSTNSYNC_CLK_PERIC1_I3C_CLK, "gout_peric1_rstnsync_clk_peric1_i3c_clk",
	     "dout_peric1_i3c",
	     PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_RSTNSYNC_CLK_PERIC1_I3C_IPCLKPORT_CLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC1_RSTNSYNC_CLK_PERIC1_NOCP_LH_CLK,
	     "gout_peric1_rstnsync_clk_peric1_nocp_lh_clk", "dout_peric1_nocp_lh",
	     PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_RSTNSYNC_CLK_PERIC1_NOCP_LH_IPCLKPORT_CLK, 21, 0,
	     0),
	GATE(CLK_GOUT_PERIC1_RSTNSYNC_CLK_PERIC1_OSCCLK_CLK,
	     "gout_peric1_rstnsync_clk_peric1_oscclk_clk", "oscclk",
	     PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_RSTNSYNC_CLK_PERIC1_OSCCLK_IPCLKPORT_CLK, 21, 0,
	     0),
	GATE(CLK_GOUT_PERIC1_RSTNSYNC_CLK_PERIC1_USI15_USI_CLK,
	     "gout_peric1_rstnsync_clk_peric1_usi15_usi_clk", "dout_peric1_usi15_usi",
	     PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_RSTNSYNC_CLK_PERIC1_USI15_USI_IPCLKPORT_CLK, 21,
	     0, 0),
	GATE(CLK_GOUT_PERIC1_RSTNSYNC_CLK_PERIC1_USI16_USI_CLK,
	     "gout_peric1_rstnsync_clk_peric1_usi16_usi_clk", "dout_peric1_usi16_usi",
	     PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_RSTNSYNC_CLK_PERIC1_USI16_USI_IPCLKPORT_CLK, 21,
	     0, 0),
	GATE(CLK_GOUT_PERIC1_SLH_AXI_MI_P_PERIC1_I_CLK, "gout_peric1_slh_axi_mi_p_peric1_i_clk",
	     "dout_peric1_nocp_lh",
	     PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_SLH_AXI_MI_P_PERIC1_IPCLKPORT_I_CLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC1_USI0_USI_IPCLK, "gout_peric1_usi0_usi_ipclk", "dout_peric1_usi0_usi",
	     PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI0_USI_IPCLKPORT_IPCLK, 21,
	     CLK_SET_RATE_PARENT, 0),
	GATE(CLK_GOUT_PERIC1_USI0_USI_PCLK, "gout_peric1_usi0_usi_pclk", "mout_peric1_noc_user",
	     PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI0_USI_IPCLKPORT_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC1_USI10_USI_IPCLK, "gout_peric1_usi10_usi_ipclk",
	     "dout_peric1_usi10_usi",
	     PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI10_USI_IPCLKPORT_IPCLK, 21,
	     CLK_SET_RATE_PARENT, 0),
	GATE(CLK_GOUT_PERIC1_USI10_USI_PCLK, "gout_peric1_usi10_usi_pclk", "mout_peric1_noc_user",
	     PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI10_USI_IPCLKPORT_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC1_USI11_USI_IPCLK, "gout_peric1_usi11_usi_ipclk",
	     "dout_peric1_usi11_usi",
	     PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI11_USI_IPCLKPORT_IPCLK, 21,
	     CLK_SET_RATE_PARENT, 0),
	GATE(CLK_GOUT_PERIC1_USI11_USI_PCLK, "gout_peric1_usi11_usi_pclk", "mout_peric1_noc_user",
	     PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI11_USI_IPCLKPORT_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC1_USI12_USI_IPCLK, "gout_peric1_usi12_usi_ipclk",
	     "dout_peric1_usi12_usi",
	     PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI12_USI_IPCLKPORT_IPCLK, 21,
	     CLK_SET_RATE_PARENT, 0),
	GATE(CLK_GOUT_PERIC1_USI12_USI_PCLK, "gout_peric1_usi12_usi_pclk", "mout_peric1_noc_user",
	     PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI12_USI_IPCLKPORT_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC1_USI13_USI_IPCLK, "gout_peric1_usi13_usi_ipclk",
	     "dout_peric1_usi13_usi",
	     PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI13_USI_IPCLKPORT_IPCLK, 21,
	     CLK_SET_RATE_PARENT, 0),
	GATE(CLK_GOUT_PERIC1_USI13_USI_PCLK, "gout_peric1_usi13_usi_pclk", "mout_peric1_noc_user",
	     PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI13_USI_IPCLKPORT_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC1_USI15_USI_IPCLK, "gout_peric1_usi15_usi_ipclk",
	     "dout_peric1_usi15_usi",
	     PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI15_USI_IPCLKPORT_IPCLK, 21,
	     CLK_SET_RATE_PARENT, 0),
	GATE(CLK_GOUT_PERIC1_USI15_USI_PCLK, "gout_peric1_usi15_usi_pclk", "mout_peric1_noc_user",
	     PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI15_USI_IPCLKPORT_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC1_USI16_USI_IPCLK, "gout_peric1_usi16_usi_ipclk",
	     "dout_peric1_usi16_usi",
	     PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI16_USI_IPCLKPORT_IPCLK, 21,
	     CLK_SET_RATE_PARENT, 0),
	GATE(CLK_GOUT_PERIC1_USI16_USI_PCLK, "gout_peric1_usi16_usi_pclk", "mout_peric1_noc_user",
	     PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI16_USI_IPCLKPORT_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC1_USI9_USI_IPCLK, "gout_peric1_usi9_usi_ipclk", "dout_peric1_usi9_usi",
	     PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI9_USI_IPCLKPORT_IPCLK, 21,
	     CLK_SET_RATE_PARENT, 0),
	GATE(CLK_GOUT_PERIC1_USI9_USI_PCLK, "gout_peric1_usi9_usi_pclk", "mout_peric1_noc_user",
	     PERIC1_CLK_CON_GAT_CLK_BLK_PERIC1_UID_USI9_USI_IPCLKPORT_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC1_D_TZPC_PERIC1_PCLK, "gout_peric1_d_tzpc_peric1_pclk",
	     "mout_peric1_noc_user",
	     PERIC1_CLK_CON_GAT_GOUT_BLK_PERIC1_UID_D_TZPC_PERIC1_IPCLKPORT_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC1_GPC_PERIC1_PCLK, "gout_peric1_gpc_peric1_pclk", "mout_peric1_noc_user",
	     PERIC1_CLK_CON_GAT_GOUT_BLK_PERIC1_UID_GPC_PERIC1_IPCLKPORT_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC1_GPIO_PERIC1_PCLK, "gout_peric1_gpio_peric1_pclk",
	     "mout_peric1_noc_user",
	     PERIC1_CLK_CON_GAT_GOUT_BLK_PERIC1_UID_GPIO_PERIC1_IPCLKPORT_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_PERIC1_LH_AXI_MI_P_PERIC1_CU_I_CLK, "gout_peric1_lh_axi_mi_p_peric1_cu_i_clk",
	     "mout_peric1_noc_user",
	     PERIC1_CLK_CON_GAT_GOUT_BLK_PERIC1_UID_LH_AXI_MI_P_PERIC1_CU_IPCLKPORT_I_CLK, 21, 0,
	     0),
	GATE(CLK_GOUT_PERIC1_RSTNSYNC_CLK_PERIC1_NOCP_CLK,
	     "gout_peric1_rstnsync_clk_peric1_nocp_clk", "mout_peric1_noc_user",
	     PERIC1_CLK_CON_GAT_GOUT_BLK_PERIC1_UID_RSTNSYNC_CLK_PERIC1_NOCP_IPCLKPORT_CLK, 21, 0,
	     0),
	GATE(CLK_GOUT_PERIC1_RSTNSYNC_CLK_PERIC1_USI0_USI_CLK,
	     "gout_peric1_rstnsync_clk_peric1_usi0_usi_clk", "dout_peric1_usi0_usi",
	     PERIC1_CLK_CON_GAT_GOUT_BLK_PERIC1_UID_RSTNSYNC_CLK_PERIC1_USI0_USI_IPCLKPORT_CLK, 21,
	     0, 0),
	GATE(CLK_GOUT_PERIC1_RSTNSYNC_CLK_PERIC1_USI10_USI_CLK,
	     "gout_peric1_rstnsync_clk_peric1_usi10_usi_clk", "dout_peric1_usi10_usi",
	     PERIC1_CLK_CON_GAT_GOUT_BLK_PERIC1_UID_RSTNSYNC_CLK_PERIC1_USI10_USI_IPCLKPORT_CLK, 21,
	     0, 0),
	GATE(CLK_GOUT_PERIC1_RSTNSYNC_CLK_PERIC1_USI11_USI_CLK,
	     "gout_peric1_rstnsync_clk_peric1_usi11_usi_clk", "dout_peric1_usi11_usi",
	     PERIC1_CLK_CON_GAT_GOUT_BLK_PERIC1_UID_RSTNSYNC_CLK_PERIC1_USI11_USI_IPCLKPORT_CLK, 21,
	     0, 0),
	GATE(CLK_GOUT_PERIC1_RSTNSYNC_CLK_PERIC1_USI12_USI_CLK,
	     "gout_peric1_rstnsync_clk_peric1_usi12_usi_clk", "dout_peric1_usi12_usi",
	     PERIC1_CLK_CON_GAT_GOUT_BLK_PERIC1_UID_RSTNSYNC_CLK_PERIC1_USI12_USI_IPCLKPORT_CLK, 21,
	     0, 0),
	GATE(CLK_GOUT_PERIC1_RSTNSYNC_CLK_PERIC1_USI13_USI_CLK,
	     "gout_peric1_rstnsync_clk_peric1_usi13_usi_clk", "dout_peric1_usi13_usi",
	     PERIC1_CLK_CON_GAT_GOUT_BLK_PERIC1_UID_RSTNSYNC_CLK_PERIC1_USI13_USI_IPCLKPORT_CLK, 21,
	     0, 0),
	GATE(CLK_GOUT_PERIC1_RSTNSYNC_CLK_PERIC1_USI9_USI_CLK,
	     "gout_peric1_rstnsync_clk_peric1_usi9_usi_clk", "dout_peric1_usi9_usi",
	     PERIC1_CLK_CON_GAT_GOUT_BLK_PERIC1_UID_RSTNSYNC_CLK_PERIC1_USI9_USI_IPCLKPORT_CLK, 21,
	     0, 0),
	GATE(CLK_GOUT_PERIC1_SYSREG_PERIC1_PCLK, "gout_peric1_sysreg_peric1_pclk",
	     "mout_peric1_noc_user",
	     PERIC1_CLK_CON_GAT_GOUT_BLK_PERIC1_UID_SYSREG_PERIC1_IPCLKPORT_PCLK, 21, 0, 0),
};

static const struct samsung_cmu_info peric1_cmu_info __initconst = {
	.mux_clks = peric1_mux_clks,
	.nr_mux_clks = ARRAY_SIZE(peric1_mux_clks),
	.div_clks = peric1_div_clks,
	.nr_div_clks = ARRAY_SIZE(peric1_div_clks),
	.gate_clks = peric1_gate_clks,
	.nr_gate_clks = ARRAY_SIZE(peric1_gate_clks),
	.nr_clk_ids = CLKS_NR_PERIC1,
	.clk_regs = peric1_clk_regs,
	.nr_clk_regs = ARRAY_SIZE(peric1_clk_regs),
	.clk_name = "bus",
};

/* ---- CMU_HSI0 ------------------------------------------------------------- */
/* Register offsets at 0x11000000. */
#define HSI0_PLL_LOCKTIME_PLL_USB 0x0004
#define HSI0_PLL_CON0_PLL_USB 0x0140
#define HSI0_PLL_CON1_PLL_USB 0x0144
#define HSI0_PLL_CON2_PLL_USB 0x0148
#define HSI0_PLL_CON3_PLL_USB 0x014c
#define HSI0_PLL_CON4_PLL_USB 0x0150
#define HSI0_PLL_CON0_MUX_CLKCMU_HSI0_ALT_USER 0x0600
#define HSI0_PLL_CON0_MUX_CLKCMU_HSI0_NOC_USER 0x0610
#define HSI0_PLL_CON0_MUX_CLKCMU_HSI0_DPGTC_USER 0x0620
#define HSI0_PLL_CON0_MUX_CLKCMU_HSI0_TCXO_USER 0x0630
#define HSI0_PLL_CON0_MUX_CLKCMU_HSI0_USB20_USER 0x0640
#define HSI0_PLL_CON0_MUX_CLKCMU_HSI0_USB31DRD_USER 0x0650
#define HSI0_PLL_CON0_MUX_CLKCMU_HSI0_USPDPDBG_USER 0x0660
#define HSI0_CLK_CON_MUX_MUX_CLK_HSI0_NOC 0x1000
#define HSI0_CLK_CON_MUX_MUX_CLK_HSI0_USB20_REF 0x1004
#define HSI0_CLK_CON_MUX_MUX_CLK_HSI0_USB31DRD 0x1008
#define HSI0_CLK_CON_DIV_DIV_CLK_HSI0_NOC_LH 0x1800
#define HSI0_CLK_CON_DIV_DIV_CLK_HSI0_USB 0x1804
#define HSI0_CLK_CON_DIV_DIV_CLK_HSI0_USB31DRD 0x1808
#define HSI0_CLK_CON_GAT_CLK_BLK_HSI0_UID_HSI0_CMU_HSI0_IPCLKPORT_PCLK 0x2000
#define HSI0_CLK_CON_GAT_CLK_BLK_HSI0_UID_LH_AXI_MI_LG_ETR_HSI0_CU_IPCLKPORT_I_CLK \
	0x2004
#define HSI0_CLK_CON_GAT_CLK_BLK_HSI0_UID_LH_AXI_MI_LP1_AOC_CU_IPCLKPORT_I_CLK \
	0x2008
#define HSI0_CLK_CON_GAT_CLK_BLK_HSI0_UID_LH_AXI_MI_P_HSI0_CU_IPCLKPORT_I_CLK \
	0x200c
#define HSI0_CLK_CON_GAT_CLK_BLK_HSI0_UID_LH_AXI_SI_LG_ETR_HSI0_CU_IPCLKPORT_I_CLK \
	0x2010
#define HSI0_CLK_CON_GAT_CLK_BLK_HSI0_UID_LH_AXI_SI_LP1_AOC_CU_IPCLKPORT_I_CLK \
	0x2014
#define HSI0_CLK_CON_GAT_CLK_BLK_HSI0_UID_LH_AXI_SI_P_HSI0_CU_IPCLKPORT_I_CLK \
	0x2018
#define HSI0_CLK_CON_GAT_CLK_BLK_HSI0_UID_RSTNSYNC_CLK_HSI0_NOC_LH_IPCLKPORT_CLK \
	0x201c
#define HSI0_CLK_CON_GAT_CLK_BLK_HSI0_UID_SLH_AXI_MI_LG_ETR_HSI0_IPCLKPORT_I_CLK \
	0x2020
#define HSI0_CLK_CON_GAT_CLK_BLK_HSI0_UID_SLH_AXI_MI_LP1_AOC_IPCLKPORT_I_CLK \
	0x2024
#define HSI0_CLK_CON_GAT_CLK_BLK_HSI0_UID_SLH_AXI_MI_P_HSI0_IPCLKPORT_I_CLK \
	0x2028
#define HSI0_CLK_CON_GAT_CLK_BLK_HSI0_UID_SYSMMU_USB_IPCLKPORT_CLK_S1 0x202c
#define HSI0_CLK_CON_GAT_CLK_BLK_HSI0_UID_USB31DRD_IPCLKPORT_I_USB31DRD_SUSPEND_CLK_26 \
	0x2030
#define HSI0_CLK_CON_GAT_CLK_HSI0_ALT 0x2034
#define HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_DP_LINK_IPCLKPORT_I_DP_GTC_CLK 0x2038
#define HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_DP_LINK_IPCLKPORT_I_PCLK 0x203c
#define HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_D_TZPC_HSI0_IPCLKPORT_PCLK 0x2040
#define HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_ETR_MIU_IPCLKPORT_I_ACLK 0x2044
#define HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_ETR_MIU_IPCLKPORT_I_PCLK 0x2048
#define HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_GPC_HSI0_IPCLKPORT_PCLK 0x204c
#define HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_LH_ACEL_SI_D_HSI0_IPCLKPORT_I_CLK \
	0x2050
#define HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_LH_AXI_SI_LD_HSI0_AOC_IPCLKPORT_I_CLK \
	0x2054
#define HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_PPMU_HSI0_AOC_IPCLKPORT_ACLK 0x2058
#define HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_PPMU_HSI0_AOC_IPCLKPORT_PCLK 0x205c
#define HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_PPMU_HSI0_NOCL1B_IPCLKPORT_ACLK \
	0x2060
#define HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_PPMU_HSI0_NOCL1B_IPCLKPORT_PCLK \
	0x2064
#define HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_RSTNSYNC_CLK_HSI0_NOC_IPCLKPORT_CLK \
	0x2068
#define HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_SSMT_USB_IPCLKPORT_ACLK 0x206c
#define HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_SSMT_USB_IPCLKPORT_PCLK 0x2070
#define HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_SYSMMU_USB_IPCLKPORT_CLK_S2 0x2074
#define HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_SYSREG_HSI0_IPCLKPORT_PCLK 0x2078
#define HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_UASC_HSI0_CTRL_IPCLKPORT_ACLK 0x207c
#define HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_UASC_HSI0_CTRL_IPCLKPORT_PCLK 0x2080
#define HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_UASC_HSI0_LINK_IPCLKPORT_ACLK 0x2084
#define HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_UASC_HSI0_LINK_IPCLKPORT_PCLK 0x2088
#define HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_USB31DRD_IPCLKPORT_ACLK_PHYCTRL \
	0x208c
#define HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_USB31DRD_IPCLKPORT_BUS_CLK_EARLY \
	0x2090
#define HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_USB31DRD_IPCLKPORT_I_USB20_PHY_REFCLK_26 \
	0x2094
#define HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_USB31DRD_IPCLKPORT_I_USB31DRD_REF_CLK_40 \
	0x2098
#define HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_USB31DRD_IPCLKPORT_I_USBDPPHY_REF_SOC_PLL \
	0x209c
#define HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_USB31DRD_IPCLKPORT_I_USBDPPHY_SCL_APB_PCLK \
	0x20a0
#define HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_USB31DRD_IPCLKPORT_I_USBPCS_APB_CLK \
	0x20a4
#define HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_USB31DRD_IPCLKPORT_USBDPPHY_I_ACLK \
	0x20a8
#define HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_USB31DRD_IPCLKPORT_USBDPPHY_UDBG_I_APB_PCLK \
	0x20ac
#define HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_XIU_D0_HSI0_IPCLKPORT_ACLK 0x20b0
#define HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_XIU_D1_HSI0_IPCLKPORT_ACLK 0x20b4
#define HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_XIU_P_HSI0_IPCLKPORT_ACLK 0x20b8

static const unsigned long hsi0_clk_regs[] __initconst = {
	HSI0_PLL_LOCKTIME_PLL_USB,
	HSI0_PLL_CON0_PLL_USB,
	HSI0_PLL_CON1_PLL_USB,
	HSI0_PLL_CON2_PLL_USB,
	HSI0_PLL_CON3_PLL_USB,
	HSI0_PLL_CON4_PLL_USB,
	HSI0_PLL_CON0_MUX_CLKCMU_HSI0_ALT_USER,
	HSI0_PLL_CON0_MUX_CLKCMU_HSI0_NOC_USER,
	HSI0_PLL_CON0_MUX_CLKCMU_HSI0_DPGTC_USER,
	HSI0_PLL_CON0_MUX_CLKCMU_HSI0_TCXO_USER,
	HSI0_PLL_CON0_MUX_CLKCMU_HSI0_USB20_USER,
	HSI0_PLL_CON0_MUX_CLKCMU_HSI0_USB31DRD_USER,
	HSI0_PLL_CON0_MUX_CLKCMU_HSI0_USPDPDBG_USER,
	HSI0_CLK_CON_MUX_MUX_CLK_HSI0_NOC,
	HSI0_CLK_CON_MUX_MUX_CLK_HSI0_USB20_REF,
	HSI0_CLK_CON_MUX_MUX_CLK_HSI0_USB31DRD,
	HSI0_CLK_CON_DIV_DIV_CLK_HSI0_NOC_LH,
	HSI0_CLK_CON_DIV_DIV_CLK_HSI0_USB,
	HSI0_CLK_CON_DIV_DIV_CLK_HSI0_USB31DRD,
	HSI0_CLK_CON_GAT_CLK_BLK_HSI0_UID_HSI0_CMU_HSI0_IPCLKPORT_PCLK,
	HSI0_CLK_CON_GAT_CLK_BLK_HSI0_UID_LH_AXI_MI_LG_ETR_HSI0_CU_IPCLKPORT_I_CLK,
	HSI0_CLK_CON_GAT_CLK_BLK_HSI0_UID_LH_AXI_MI_LP1_AOC_CU_IPCLKPORT_I_CLK,
	HSI0_CLK_CON_GAT_CLK_BLK_HSI0_UID_LH_AXI_MI_P_HSI0_CU_IPCLKPORT_I_CLK,
	HSI0_CLK_CON_GAT_CLK_BLK_HSI0_UID_LH_AXI_SI_LG_ETR_HSI0_CU_IPCLKPORT_I_CLK,
	HSI0_CLK_CON_GAT_CLK_BLK_HSI0_UID_LH_AXI_SI_LP1_AOC_CU_IPCLKPORT_I_CLK,
	HSI0_CLK_CON_GAT_CLK_BLK_HSI0_UID_LH_AXI_SI_P_HSI0_CU_IPCLKPORT_I_CLK,
	HSI0_CLK_CON_GAT_CLK_BLK_HSI0_UID_RSTNSYNC_CLK_HSI0_NOC_LH_IPCLKPORT_CLK,
	HSI0_CLK_CON_GAT_CLK_BLK_HSI0_UID_SLH_AXI_MI_LG_ETR_HSI0_IPCLKPORT_I_CLK,
	HSI0_CLK_CON_GAT_CLK_BLK_HSI0_UID_SLH_AXI_MI_LP1_AOC_IPCLKPORT_I_CLK,
	HSI0_CLK_CON_GAT_CLK_BLK_HSI0_UID_SLH_AXI_MI_P_HSI0_IPCLKPORT_I_CLK,
	HSI0_CLK_CON_GAT_CLK_BLK_HSI0_UID_SYSMMU_USB_IPCLKPORT_CLK_S1,
	HSI0_CLK_CON_GAT_CLK_BLK_HSI0_UID_USB31DRD_IPCLKPORT_I_USB31DRD_SUSPEND_CLK_26,
	HSI0_CLK_CON_GAT_CLK_HSI0_ALT,
	HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_DP_LINK_IPCLKPORT_I_DP_GTC_CLK,
	HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_DP_LINK_IPCLKPORT_I_PCLK,
	HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_D_TZPC_HSI0_IPCLKPORT_PCLK,
	HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_ETR_MIU_IPCLKPORT_I_ACLK,
	HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_ETR_MIU_IPCLKPORT_I_PCLK,
	HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_GPC_HSI0_IPCLKPORT_PCLK,
	HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_LH_ACEL_SI_D_HSI0_IPCLKPORT_I_CLK,
	HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_LH_AXI_SI_LD_HSI0_AOC_IPCLKPORT_I_CLK,
	HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_PPMU_HSI0_AOC_IPCLKPORT_ACLK,
	HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_PPMU_HSI0_AOC_IPCLKPORT_PCLK,
	HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_PPMU_HSI0_NOCL1B_IPCLKPORT_ACLK,
	HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_PPMU_HSI0_NOCL1B_IPCLKPORT_PCLK,
	HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_RSTNSYNC_CLK_HSI0_NOC_IPCLKPORT_CLK,
	HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_SSMT_USB_IPCLKPORT_ACLK,
	HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_SSMT_USB_IPCLKPORT_PCLK,
	HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_SYSMMU_USB_IPCLKPORT_CLK_S2,
	HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_SYSREG_HSI0_IPCLKPORT_PCLK,
	HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_UASC_HSI0_CTRL_IPCLKPORT_ACLK,
	HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_UASC_HSI0_CTRL_IPCLKPORT_PCLK,
	HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_UASC_HSI0_LINK_IPCLKPORT_ACLK,
	HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_UASC_HSI0_LINK_IPCLKPORT_PCLK,
	HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_USB31DRD_IPCLKPORT_ACLK_PHYCTRL,
	HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_USB31DRD_IPCLKPORT_BUS_CLK_EARLY,
	HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_USB31DRD_IPCLKPORT_I_USB20_PHY_REFCLK_26,
	HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_USB31DRD_IPCLKPORT_I_USB31DRD_REF_CLK_40,
	HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_USB31DRD_IPCLKPORT_I_USBDPPHY_REF_SOC_PLL,
	HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_USB31DRD_IPCLKPORT_I_USBDPPHY_SCL_APB_PCLK,
	HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_USB31DRD_IPCLKPORT_I_USBPCS_APB_CLK,
	HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_USB31DRD_IPCLKPORT_USBDPPHY_I_ACLK,
	HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_USB31DRD_IPCLKPORT_USBDPPHY_UDBG_I_APB_PCLK,
	HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_XIU_D0_HSI0_IPCLKPORT_ACLK,
	HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_XIU_D1_HSI0_IPCLKPORT_ACLK,
	HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_XIU_P_HSI0_IPCLKPORT_ACLK,
};

#define CLKS_NR_HSI0 63

PNAME(mout_pll_usb_p) = { "oscclk", "fout_usb_pll" };
PNAME(mout_hsi0_alt_user_p) = { "oscclk", "gout_clk_hsi0_alt" };
PNAME(mout_hsi0_noc_user_p) = { "oscclk", "dout_cmu_hsi0_noc" };
PNAME(mout_hsi0_dpgtc_user_p) = { "oscclk", "dout_cmu_hsi0_dpgtc" };
PNAME(mout_hsi0_tcxo_user_p) = { "oscclk", "tcxo_hsi1_hsi0" };
PNAME(mout_hsi0_usb20_user_p) = { "oscclk", "usb20phy_phy_clock" };
PNAME(mout_hsi0_usb31drd_user_p) = { "oscclk", "dout_cmu_hsi0_usb31drd" };
PNAME(mout_hsi0_uspdpdbg_user_p) = { "oscclk", "dout_cmu_hsi0_usbdpdbg" };
PNAME(mout_hsi0_noc_p) = { "mout_hsi0_noc_user", "mout_hsi0_alt_user" };
PNAME(mout_hsi0_usb20_ref_p) = { "dout_hsi0_usb", "mout_hsi0_tcxo_user" };
PNAME(mout_hsi0_usb31drd_p) = { "dout_hsi0_usb", "mout_hsi0_usb31drd_user",
				"dout_hsi0_usb31drd", "oscclk" };

static const struct samsung_pll_clock hsi0_pll_clks[] __initconst = {
	PLL(pll_0518x, CLK_FOUT_USB_PLL, "fout_usb_pll", "oscclk",
	    HSI0_PLL_LOCKTIME_PLL_USB, HSI0_PLL_CON3_PLL_USB, NULL),
};

static const struct samsung_mux_clock hsi0_mux_clks[] __initconst = {
	MUX(CLK_MOUT_PLL_USB, "mout_pll_usb", mout_pll_usb_p,
	    HSI0_PLL_CON0_PLL_USB, 4, 1),
	MUX(CLK_MOUT_HSI0_ALT_USER, "mout_hsi0_alt_user", mout_hsi0_alt_user_p,
	    HSI0_PLL_CON0_MUX_CLKCMU_HSI0_ALT_USER, 4, 1),
	MUX(CLK_MOUT_HSI0_NOC_USER, "mout_hsi0_noc_user", mout_hsi0_noc_user_p,
	    HSI0_PLL_CON0_MUX_CLKCMU_HSI0_NOC_USER, 4, 1),
	MUX(CLK_MOUT_HSI0_DPGTC_USER, "mout_hsi0_dpgtc_user",
	    mout_hsi0_dpgtc_user_p, HSI0_PLL_CON0_MUX_CLKCMU_HSI0_DPGTC_USER, 4,
	    1),
	MUX(CLK_MOUT_HSI0_TCXO_USER, "mout_hsi0_tcxo_user",
	    mout_hsi0_tcxo_user_p, HSI0_PLL_CON0_MUX_CLKCMU_HSI0_TCXO_USER, 4,
	    1),
	MUX(CLK_MOUT_HSI0_USB20_USER, "mout_hsi0_usb20_user",
	    mout_hsi0_usb20_user_p, HSI0_PLL_CON0_MUX_CLKCMU_HSI0_USB20_USER, 4,
	    1),
	MUX(CLK_MOUT_HSI0_USB31DRD_USER, "mout_hsi0_usb31drd_user",
	    mout_hsi0_usb31drd_user_p,
	    HSI0_PLL_CON0_MUX_CLKCMU_HSI0_USB31DRD_USER, 4, 1),
	MUX(CLK_MOUT_HSI0_USPDPDBG_USER, "mout_hsi0_uspdpdbg_user",
	    mout_hsi0_uspdpdbg_user_p,
	    HSI0_PLL_CON0_MUX_CLKCMU_HSI0_USPDPDBG_USER, 4, 1),
	MUX(CLK_MOUT_HSI0_NOC, "mout_hsi0_noc", mout_hsi0_noc_p,
	    HSI0_CLK_CON_MUX_MUX_CLK_HSI0_NOC, 0, 1),
	MUX(CLK_MOUT_HSI0_USB20_REF, "mout_hsi0_usb20_ref",
	    mout_hsi0_usb20_ref_p, HSI0_CLK_CON_MUX_MUX_CLK_HSI0_USB20_REF, 0,
	    1),
	MUX(CLK_MOUT_HSI0_USB31DRD, "mout_hsi0_usb31drd", mout_hsi0_usb31drd_p,
	    HSI0_CLK_CON_MUX_MUX_CLK_HSI0_USB31DRD, 0, 2),
};

static const struct samsung_div_clock hsi0_div_clks[] __initconst = {
	DIV(CLK_DOUT_HSI0_NOC_LH, "dout_hsi0_noc_lh", "mout_hsi0_noc",
	    HSI0_CLK_CON_DIV_DIV_CLK_HSI0_NOC_LH, 0, 2),
	DIV(CLK_DOUT_HSI0_USB, "dout_hsi0_usb", "mout_pll_usb",
	    HSI0_CLK_CON_DIV_DIV_CLK_HSI0_USB, 0, 6),
	DIV(CLK_DOUT_HSI0_USB31DRD, "dout_hsi0_usb31drd",
	    "mout_hsi0_usb20_user", HSI0_CLK_CON_DIV_DIV_CLK_HSI0_USB31DRD, 0,
	    3),
};

static const struct samsung_gate_clock hsi0_gate_clks[] __initconst = {
	GATE(CLK_GOUT_HSI0_HSI0_CMU_HSI0_PCLK, "gout_hsi0_hsi0_cmu_hsi0_pclk",
	     "mout_hsi0_noc",
	     HSI0_CLK_CON_GAT_CLK_BLK_HSI0_UID_HSI0_CMU_HSI0_IPCLKPORT_PCLK, 21,
	     0, 0),
	GATE(CLK_GOUT_HSI0_LH_AXI_MI_LG_ETR_HSI0_CU_I_CLK,
	     "gout_hsi0_lh_axi_mi_lg_etr_hsi0_cu_i_clk", "mout_hsi0_noc",
	     HSI0_CLK_CON_GAT_CLK_BLK_HSI0_UID_LH_AXI_MI_LG_ETR_HSI0_CU_IPCLKPORT_I_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI0_LH_AXI_MI_LP1_AOC_CU_I_CLK,
	     "gout_hsi0_lh_axi_mi_lp1_aoc_cu_i_clk", "mout_hsi0_noc",
	     HSI0_CLK_CON_GAT_CLK_BLK_HSI0_UID_LH_AXI_MI_LP1_AOC_CU_IPCLKPORT_I_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI0_LH_AXI_MI_P_HSI0_CU_I_CLK,
	     "gout_hsi0_lh_axi_mi_p_hsi0_cu_i_clk", "mout_hsi0_noc",
	     HSI0_CLK_CON_GAT_CLK_BLK_HSI0_UID_LH_AXI_MI_P_HSI0_CU_IPCLKPORT_I_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI0_LH_AXI_SI_LG_ETR_HSI0_CU_I_CLK,
	     "gout_hsi0_lh_axi_si_lg_etr_hsi0_cu_i_clk", "dout_hsi0_noc_lh",
	     HSI0_CLK_CON_GAT_CLK_BLK_HSI0_UID_LH_AXI_SI_LG_ETR_HSI0_CU_IPCLKPORT_I_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI0_LH_AXI_SI_LP1_AOC_CU_I_CLK,
	     "gout_hsi0_lh_axi_si_lp1_aoc_cu_i_clk", "dout_hsi0_noc_lh",
	     HSI0_CLK_CON_GAT_CLK_BLK_HSI0_UID_LH_AXI_SI_LP1_AOC_CU_IPCLKPORT_I_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI0_LH_AXI_SI_P_HSI0_CU_I_CLK,
	     "gout_hsi0_lh_axi_si_p_hsi0_cu_i_clk", "dout_hsi0_noc_lh",
	     HSI0_CLK_CON_GAT_CLK_BLK_HSI0_UID_LH_AXI_SI_P_HSI0_CU_IPCLKPORT_I_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI0_RSTNSYNC_CLK_HSI0_NOC_LH_CLK,
	     "gout_hsi0_rstnsync_clk_hsi0_noc_lh_clk", "dout_hsi0_noc_lh",
	     HSI0_CLK_CON_GAT_CLK_BLK_HSI0_UID_RSTNSYNC_CLK_HSI0_NOC_LH_IPCLKPORT_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI0_SLH_AXI_MI_LG_ETR_HSI0_I_CLK,
	     "gout_hsi0_slh_axi_mi_lg_etr_hsi0_i_clk", "dout_hsi0_noc_lh",
	     HSI0_CLK_CON_GAT_CLK_BLK_HSI0_UID_SLH_AXI_MI_LG_ETR_HSI0_IPCLKPORT_I_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI0_SLH_AXI_MI_LP1_AOC_I_CLK,
	     "gout_hsi0_slh_axi_mi_lp1_aoc_i_clk", "dout_hsi0_noc_lh",
	     HSI0_CLK_CON_GAT_CLK_BLK_HSI0_UID_SLH_AXI_MI_LP1_AOC_IPCLKPORT_I_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI0_SLH_AXI_MI_P_HSI0_I_CLK,
	     "gout_hsi0_slh_axi_mi_p_hsi0_i_clk", "dout_hsi0_noc_lh",
	     HSI0_CLK_CON_GAT_CLK_BLK_HSI0_UID_SLH_AXI_MI_P_HSI0_IPCLKPORT_I_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI0_SYSMMU_USB_CLK_S1, "gout_hsi0_sysmmu_usb_clk_s1",
	     "mout_hsi0_noc",
	     HSI0_CLK_CON_GAT_CLK_BLK_HSI0_UID_SYSMMU_USB_IPCLKPORT_CLK_S1, 21,
	     0, 0),
	GATE(CLK_GOUT_HSI0_USB31DRD_I_USB31DRD_SUSPEND_CLK_26,
	     "gout_hsi0_usb31drd_i_usb31drd_suspend_clk_26",
	     "mout_hsi0_usb20_ref",
	     HSI0_CLK_CON_GAT_CLK_BLK_HSI0_UID_USB31DRD_IPCLKPORT_I_USB31DRD_SUSPEND_CLK_26,
	     21, 0, 0),
	GATE(CLK_GOUT_CLK_HSI0_ALT, "gout_clk_hsi0_alt", "i_clk_hsi0_alt",
	     HSI0_CLK_CON_GAT_CLK_HSI0_ALT, 21, 0, 0),
	GATE(CLK_GOUT_HSI0_DP_LINK_I_DP_GTC_CLK,
	     "gout_hsi0_dp_link_i_dp_gtc_clk", "mout_hsi0_dpgtc_user",
	     HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_DP_LINK_IPCLKPORT_I_DP_GTC_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI0_DP_LINK_I_PCLK, "gout_hsi0_dp_link_i_pclk",
	     "mout_hsi0_noc",
	     HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_DP_LINK_IPCLKPORT_I_PCLK, 21, 0,
	     0),
	GATE(CLK_GOUT_HSI0_D_TZPC_HSI0_PCLK, "gout_hsi0_d_tzpc_hsi0_pclk",
	     "mout_hsi0_noc",
	     HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_D_TZPC_HSI0_IPCLKPORT_PCLK, 21,
	     0, 0),
	GATE(CLK_GOUT_HSI0_ETR_MIU_I_ACLK, "gout_hsi0_etr_miu_i_aclk",
	     "mout_hsi0_noc",
	     HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_ETR_MIU_IPCLKPORT_I_ACLK, 21, 0,
	     0),
	GATE(CLK_GOUT_HSI0_ETR_MIU_I_PCLK, "gout_hsi0_etr_miu_i_pclk",
	     "mout_hsi0_noc",
	     HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_ETR_MIU_IPCLKPORT_I_PCLK, 21, 0,
	     0),
	GATE(CLK_GOUT_HSI0_GPC_HSI0_PCLK, "gout_hsi0_gpc_hsi0_pclk",
	     "mout_hsi0_noc",
	     HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_GPC_HSI0_IPCLKPORT_PCLK, 21, 0,
	     0),
	GATE(CLK_GOUT_HSI0_LH_ACEL_SI_D_HSI0_I_CLK,
	     "gout_hsi0_lh_acel_si_d_hsi0_i_clk", "mout_hsi0_noc",
	     HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_LH_ACEL_SI_D_HSI0_IPCLKPORT_I_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI0_LH_AXI_SI_LD_HSI0_AOC_I_CLK,
	     "gout_hsi0_lh_axi_si_ld_hsi0_aoc_i_clk", "mout_hsi0_noc",
	     HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_LH_AXI_SI_LD_HSI0_AOC_IPCLKPORT_I_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI0_PPMU_HSI0_AOC_ACLK, "gout_hsi0_ppmu_hsi0_aoc_aclk",
	     "mout_hsi0_noc",
	     HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_PPMU_HSI0_AOC_IPCLKPORT_ACLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI0_PPMU_HSI0_AOC_PCLK, "gout_hsi0_ppmu_hsi0_aoc_pclk",
	     "mout_hsi0_noc",
	     HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_PPMU_HSI0_AOC_IPCLKPORT_PCLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI0_PPMU_HSI0_NOCL1B_ACLK,
	     "gout_hsi0_ppmu_hsi0_nocl1b_aclk", "mout_hsi0_noc",
	     HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_PPMU_HSI0_NOCL1B_IPCLKPORT_ACLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI0_PPMU_HSI0_NOCL1B_PCLK,
	     "gout_hsi0_ppmu_hsi0_nocl1b_pclk", "mout_hsi0_noc",
	     HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_PPMU_HSI0_NOCL1B_IPCLKPORT_PCLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI0_RSTNSYNC_CLK_HSI0_NOC_CLK,
	     "gout_hsi0_rstnsync_clk_hsi0_noc_clk", "mout_hsi0_noc",
	     HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_RSTNSYNC_CLK_HSI0_NOC_IPCLKPORT_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI0_SSMT_USB_ACLK, "gout_hsi0_ssmt_usb_aclk",
	     "mout_hsi0_noc",
	     HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_SSMT_USB_IPCLKPORT_ACLK, 21, 0,
	     0),
	GATE(CLK_GOUT_HSI0_SSMT_USB_PCLK, "gout_hsi0_ssmt_usb_pclk",
	     "mout_hsi0_noc",
	     HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_SSMT_USB_IPCLKPORT_PCLK, 21, 0,
	     0),
	GATE(CLK_GOUT_HSI0_SYSMMU_USB_CLK_S2, "gout_hsi0_sysmmu_usb_clk_s2",
	     "mout_hsi0_noc",
	     HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_SYSMMU_USB_IPCLKPORT_CLK_S2, 21,
	     0, 0),
	GATE(CLK_GOUT_HSI0_SYSREG_HSI0_PCLK, "gout_hsi0_sysreg_hsi0_pclk",
	     "mout_hsi0_noc",
	     HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_SYSREG_HSI0_IPCLKPORT_PCLK, 21,
	     0, 0),
	GATE(CLK_GOUT_HSI0_UASC_HSI0_CTRL_ACLK, "gout_hsi0_uasc_hsi0_ctrl_aclk",
	     "mout_hsi0_noc",
	     HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_UASC_HSI0_CTRL_IPCLKPORT_ACLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI0_UASC_HSI0_CTRL_PCLK, "gout_hsi0_uasc_hsi0_ctrl_pclk",
	     "mout_hsi0_noc",
	     HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_UASC_HSI0_CTRL_IPCLKPORT_PCLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI0_UASC_HSI0_LINK_ACLK, "gout_hsi0_uasc_hsi0_link_aclk",
	     "mout_hsi0_noc",
	     HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_UASC_HSI0_LINK_IPCLKPORT_ACLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI0_UASC_HSI0_LINK_PCLK, "gout_hsi0_uasc_hsi0_link_pclk",
	     "mout_hsi0_noc",
	     HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_UASC_HSI0_LINK_IPCLKPORT_PCLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI0_USB31DRD_ACLK_PHYCTRL,
	     "gout_hsi0_usb31drd_aclk_phyctrl", "mout_hsi0_noc",
	     HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_USB31DRD_IPCLKPORT_ACLK_PHYCTRL,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI0_USB31DRD_BUS_CLK_EARLY,
	     "gout_hsi0_usb31drd_bus_clk_early", "mout_hsi0_noc",
	     HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_USB31DRD_IPCLKPORT_BUS_CLK_EARLY,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI0_USB31DRD_I_USB20_PHY_REFCLK_26,
	     "gout_hsi0_usb31drd_i_usb20_phy_refclk_26", "mout_hsi0_usb20_ref",
	     HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_USB31DRD_IPCLKPORT_I_USB20_PHY_REFCLK_26,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI0_USB31DRD_I_USB31DRD_REF_CLK_40,
	     "gout_hsi0_usb31drd_i_usb31drd_ref_clk_40", "mout_hsi0_usb31drd",
	     HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_USB31DRD_IPCLKPORT_I_USB31DRD_REF_CLK_40,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI0_USB31DRD_I_USBDPPHY_REF_SOC_PLL,
	     "gout_hsi0_usb31drd_i_usbdpphy_ref_soc_pll",
	     "mout_hsi0_uspdpdbg_user",
	     HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_USB31DRD_IPCLKPORT_I_USBDPPHY_REF_SOC_PLL,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI0_USB31DRD_I_USBDPPHY_SCL_APB_PCLK,
	     "gout_hsi0_usb31drd_i_usbdpphy_scl_apb_pclk", "mout_hsi0_noc",
	     HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_USB31DRD_IPCLKPORT_I_USBDPPHY_SCL_APB_PCLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI0_USB31DRD_I_USBPCS_APB_CLK,
	     "gout_hsi0_usb31drd_i_usbpcs_apb_clk", "mout_hsi0_noc",
	     HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_USB31DRD_IPCLKPORT_I_USBPCS_APB_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI0_USB31DRD_USBDPPHY_I_ACLK,
	     "gout_hsi0_usb31drd_usbdpphy_i_aclk", "mout_hsi0_noc",
	     HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_USB31DRD_IPCLKPORT_USBDPPHY_I_ACLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI0_USB31DRD_USBDPPHY_UDBG_I_APB_PCLK,
	     "gout_hsi0_usb31drd_usbdpphy_udbg_i_apb_pclk", "mout_hsi0_noc",
	     HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_USB31DRD_IPCLKPORT_USBDPPHY_UDBG_I_APB_PCLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI0_XIU_D0_HSI0_ACLK, "gout_hsi0_xiu_d0_hsi0_aclk",
	     "mout_hsi0_noc",
	     HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_XIU_D0_HSI0_IPCLKPORT_ACLK, 21,
	     0, 0),
	GATE(CLK_GOUT_HSI0_XIU_D1_HSI0_ACLK, "gout_hsi0_xiu_d1_hsi0_aclk",
	     "mout_hsi0_noc",
	     HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_XIU_D1_HSI0_IPCLKPORT_ACLK, 21,
	     0, 0),
	GATE(CLK_GOUT_HSI0_XIU_P_HSI0_ACLK, "gout_hsi0_xiu_p_hsi0_aclk",
	     "mout_hsi0_noc",
	     HSI0_CLK_CON_GAT_GOUT_BLK_HSI0_UID_XIU_P_HSI0_IPCLKPORT_ACLK, 21,
	     0, 0),
};

static const struct samsung_cmu_info hsi0_cmu_info __initconst = {
	.pll_clks = hsi0_pll_clks,
	.nr_pll_clks = ARRAY_SIZE(hsi0_pll_clks),
	.mux_clks = hsi0_mux_clks,
	.nr_mux_clks = ARRAY_SIZE(hsi0_mux_clks),
	.div_clks = hsi0_div_clks,
	.nr_div_clks = ARRAY_SIZE(hsi0_div_clks),
	.gate_clks = hsi0_gate_clks,
	.nr_gate_clks = ARRAY_SIZE(hsi0_gate_clks),
	.nr_clk_ids = CLKS_NR_HSI0,
	.clk_regs = hsi0_clk_regs,
	.nr_clk_regs = ARRAY_SIZE(hsi0_clk_regs),
	.clk_name = "bus",
};

/* ---- CMU_HSI2 ------------------------------------------------------------- */
/* Register offsets at 0x14400000. */
#define HSI2_PLL_CON0_MUX_CLKCMU_HSI2_NOC_USER 0x0600
#define HSI2_PLL_CON0_MUX_CLKCMU_HSI2_MMC_CARD_USER 0x0610
#define HSI2_PLL_CON0_MUX_CLKCMU_HSI2_PCIE_USER 0x0620
#define HSI2_PLL_CON0_MUX_CLKCMU_HSI2_UFS_EMBD_USER 0x0630
#define HSI2_CLK_CON_DIV_DIV_CLK_HSI2_NOCP 0x1800
#define HSI2_CLK_CON_DIV_DIV_CLK_HSI2_NOC_LH 0x1804
#define HSI2_CLK_CON_GAT_CLK_BLK_HSI2_UID_GPIO_HSI2UFS_IPCLKPORT_PCLK 0x2000
#define HSI2_CLK_CON_GAT_CLK_BLK_HSI2_UID_LH_AXI_SI_P_HSI2_CU_IPCLKPORT_I_CLK \
	0x2004
#define HSI2_CLK_CON_GAT_CLK_BLK_HSI2_UID_PCIE_GEN4_1_IPCLKPORT_PCIE_003_PCIE_SUB_CTRL_INST_0_PHY_REFCLK_IN \
	0x2008
#define HSI2_CLK_CON_GAT_CLK_BLK_HSI2_UID_PCIE_GEN4_1_IPCLKPORT_PCIE_004_PCIE_SUB_CTRL_INST_0_PHY_REFCLK_IN \
	0x200c
#define HSI2_CLK_CON_GAT_CLK_BLK_HSI2_UID_RSTNSYNC_CLK_HSI2_NOC_LH_IPCLKPORT_CLK \
	0x2010
#define HSI2_CLK_CON_GAT_CLK_BLK_HSI2_UID_SLH_AXI_MI_P_HSI2_IPCLKPORT_I_CLK \
	0x2014
#define HSI2_CLK_CON_GAT_CLK_BLK_HSI2_UID_SSMT_PCIE_IA_GEN4A_1_IPCLKPORT_ACLK \
	0x2018
#define HSI2_CLK_CON_GAT_CLK_BLK_HSI2_UID_SSMT_PCIE_IA_GEN4A_1_IPCLKPORT_PCLK \
	0x201c
#define HSI2_CLK_CON_GAT_CLK_BLK_HSI2_UID_SSMT_PCIE_IA_GEN4B_1_IPCLKPORT_ACLK \
	0x2020
#define HSI2_CLK_CON_GAT_CLK_BLK_HSI2_UID_SSMT_PCIE_IA_GEN4B_1_IPCLKPORT_PCLK \
	0x2024
#define HSI2_CLK_CON_GAT_CLK_BLK_HSI2_UID_SYSMMU_HSI2_IPCLKPORT_CLK_S1 0x2028
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_AS_APB_PCIEPHY_HSI2_IPCLKPORT_PCLKM \
	0x202c
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_D_TZPC_HSI2_IPCLKPORT_PCLK 0x2030
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_GPC_HSI2_IPCLKPORT_PCLK 0x2034
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_GPIO_HSI2_IPCLKPORT_PCLK 0x2038
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_HSI2_CMU_HSI2_IPCLKPORT_PCLK 0x203c
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_LH_ACEL_SI_D_HSI2_IPCLKPORT_I_CLK \
	0x2040
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_LH_AXI_MI_P_HSI2_CU_IPCLKPORT_I_CLK \
	0x2044
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_MMC_CARD_IPCLKPORT_I_ACLK 0x2048
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_MMC_CARD_IPCLKPORT_SDCLKIN 0x204c
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_PCIE_GEN4_1_IPCLKPORT_PCIE_003_G4X2_DWC_PCIE_CTL_INST_0_DBI_ACLK_UG \
	0x2050
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_PCIE_GEN4_1_IPCLKPORT_PCIE_003_G4X2_DWC_PCIE_CTL_INST_0_MSTR_ACLK_UG \
	0x2054
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_PCIE_GEN4_1_IPCLKPORT_PCIE_003_G4X2_DWC_PCIE_CTL_INST_0_SLV_ACLK_UG \
	0x2058
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_PCIE_GEN4_1_IPCLKPORT_PCIE_003_PCIE_SUB_CTRL_INST_0_I_DRIVER_APB_CLK \
	0x205c
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_PCIE_GEN4_1_IPCLKPORT_PCIE_004_G4X1_DWC_PCIE_CTL_INST_0_DBI_ACLK_UG \
	0x2060
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_PCIE_GEN4_1_IPCLKPORT_PCIE_004_G4X1_DWC_PCIE_CTL_INST_0_MSTR_ACLK_UG \
	0x2064
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_PCIE_GEN4_1_IPCLKPORT_PCIE_004_G4X1_DWC_PCIE_CTL_INST_0_SLV_ACLK_UG \
	0x2068
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_PCIE_GEN4_1_IPCLKPORT_PCIE_004_PCIE_SUB_CTRL_INST_0_I_DRIVER_APB_CLK \
	0x206c
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_PCIE_GEN4_1_IPCLKPORT_PCS_PMA_INST_0_PHY_UDBG_I_APB_PCLK \
	0x2070
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_PCIE_GEN4_1_IPCLKPORT_PCS_PMA_INST_0_PIPE_PAL_PCIE_INST_0_I_APB_PCLK \
	0x2074
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_PCIE_GEN4_1_IPCLKPORT_PCS_PMA_INST_0_SF_PCIEPHY210X2_LN05LPE_QCH_TM_WRAPPER_INST_0_I_APB_PCLK \
	0x2078
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_PCIE_IA_GEN4A_1_IPCLKPORT_I_CLK \
	0x207c
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_PCIE_IA_GEN4B_1_IPCLKPORT_I_CLK \
	0x2080
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_PPMU_HSI2_IPCLKPORT_ACLK 0x2084
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_PPMU_HSI2_IPCLKPORT_PCLK 0x2088
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_QE_MMC_CARD_HSI2_IPCLKPORT_ACLK \
	0x208c
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_QE_MMC_CARD_HSI2_IPCLKPORT_PCLK \
	0x2090
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_QE_PCIE_GEN4A_HSI2_IPCLKPORT_ACLK \
	0x2094
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_QE_PCIE_GEN4A_HSI2_IPCLKPORT_PCLK \
	0x2098
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_QE_PCIE_GEN4B_HSI2_IPCLKPORT_ACLK \
	0x209c
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_QE_PCIE_GEN4B_HSI2_IPCLKPORT_PCLK \
	0x20a0
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_QE_UFS_EMBD_HSI2_IPCLKPORT_ACLK \
	0x20a4
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_QE_UFS_EMBD_HSI2_IPCLKPORT_PCLK \
	0x20a8
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_RSTNSYNC_CLK_HSI2_NOCP_IPCLKPORT_CLK \
	0x20ac
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_RSTNSYNC_CLK_HSI2_NOC_IPCLKPORT_CLK \
	0x20b0
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_RSTNSYNC_CLK_HSI2_OSCCLK_IPCLKPORT_CLK \
	0x20b4
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_SSMT_HSI2_IPCLKPORT_ACLK 0x20b8
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_SSMT_HSI2_IPCLKPORT_PCLK 0x20bc
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_SYSMMU_HSI2_IPCLKPORT_CLK_S2 0x20c0
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_SYSREG_HSI2_IPCLKPORT_PCLK 0x20c4
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_UASC_PCIE_GEN4A_DBI_1_IPCLKPORT_ACLK \
	0x20c8
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_UASC_PCIE_GEN4A_DBI_1_IPCLKPORT_PCLK \
	0x20cc
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_UASC_PCIE_GEN4A_SLV_1_IPCLKPORT_ACLK \
	0x20d0
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_UASC_PCIE_GEN4A_SLV_1_IPCLKPORT_PCLK \
	0x20d4
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_UASC_PCIE_GEN4B_DBI_1_IPCLKPORT_ACLK \
	0x20d8
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_UASC_PCIE_GEN4B_DBI_1_IPCLKPORT_PCLK \
	0x20dc
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_UASC_PCIE_GEN4B_SLV_1_IPCLKPORT_ACLK \
	0x20e0
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_UASC_PCIE_GEN4B_SLV_1_IPCLKPORT_PCLK \
	0x20e4
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_UFS_EMBD_IPCLKPORT_I_ACLK 0x20e8
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_UFS_EMBD_IPCLKPORT_I_CLK_UNIPRO \
	0x20ec
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_UFS_EMBD_IPCLKPORT_I_FMP_CLK 0x20f0
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_XIU_D_HSI2_IPCLKPORT_ACLK 0x20f4
#define HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_XIU_P_HSI2_IPCLKPORT_ACLK 0x20f8

static const unsigned long hsi2_clk_regs[] __initconst = {
	HSI2_PLL_CON0_MUX_CLKCMU_HSI2_NOC_USER,
	HSI2_PLL_CON0_MUX_CLKCMU_HSI2_MMC_CARD_USER,
	HSI2_PLL_CON0_MUX_CLKCMU_HSI2_PCIE_USER,
	HSI2_PLL_CON0_MUX_CLKCMU_HSI2_UFS_EMBD_USER,
	HSI2_CLK_CON_DIV_DIV_CLK_HSI2_NOCP,
	HSI2_CLK_CON_DIV_DIV_CLK_HSI2_NOC_LH,
	HSI2_CLK_CON_GAT_CLK_BLK_HSI2_UID_GPIO_HSI2UFS_IPCLKPORT_PCLK,
	HSI2_CLK_CON_GAT_CLK_BLK_HSI2_UID_LH_AXI_SI_P_HSI2_CU_IPCLKPORT_I_CLK,
	HSI2_CLK_CON_GAT_CLK_BLK_HSI2_UID_PCIE_GEN4_1_IPCLKPORT_PCIE_003_PCIE_SUB_CTRL_INST_0_PHY_REFCLK_IN,
	HSI2_CLK_CON_GAT_CLK_BLK_HSI2_UID_PCIE_GEN4_1_IPCLKPORT_PCIE_004_PCIE_SUB_CTRL_INST_0_PHY_REFCLK_IN,
	HSI2_CLK_CON_GAT_CLK_BLK_HSI2_UID_RSTNSYNC_CLK_HSI2_NOC_LH_IPCLKPORT_CLK,
	HSI2_CLK_CON_GAT_CLK_BLK_HSI2_UID_SLH_AXI_MI_P_HSI2_IPCLKPORT_I_CLK,
	HSI2_CLK_CON_GAT_CLK_BLK_HSI2_UID_SSMT_PCIE_IA_GEN4A_1_IPCLKPORT_ACLK,
	HSI2_CLK_CON_GAT_CLK_BLK_HSI2_UID_SSMT_PCIE_IA_GEN4A_1_IPCLKPORT_PCLK,
	HSI2_CLK_CON_GAT_CLK_BLK_HSI2_UID_SSMT_PCIE_IA_GEN4B_1_IPCLKPORT_ACLK,
	HSI2_CLK_CON_GAT_CLK_BLK_HSI2_UID_SSMT_PCIE_IA_GEN4B_1_IPCLKPORT_PCLK,
	HSI2_CLK_CON_GAT_CLK_BLK_HSI2_UID_SYSMMU_HSI2_IPCLKPORT_CLK_S1,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_AS_APB_PCIEPHY_HSI2_IPCLKPORT_PCLKM,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_D_TZPC_HSI2_IPCLKPORT_PCLK,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_GPC_HSI2_IPCLKPORT_PCLK,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_GPIO_HSI2_IPCLKPORT_PCLK,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_HSI2_CMU_HSI2_IPCLKPORT_PCLK,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_LH_ACEL_SI_D_HSI2_IPCLKPORT_I_CLK,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_LH_AXI_MI_P_HSI2_CU_IPCLKPORT_I_CLK,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_MMC_CARD_IPCLKPORT_I_ACLK,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_MMC_CARD_IPCLKPORT_SDCLKIN,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_PCIE_GEN4_1_IPCLKPORT_PCIE_003_G4X2_DWC_PCIE_CTL_INST_0_DBI_ACLK_UG,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_PCIE_GEN4_1_IPCLKPORT_PCIE_003_G4X2_DWC_PCIE_CTL_INST_0_MSTR_ACLK_UG,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_PCIE_GEN4_1_IPCLKPORT_PCIE_003_G4X2_DWC_PCIE_CTL_INST_0_SLV_ACLK_UG,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_PCIE_GEN4_1_IPCLKPORT_PCIE_003_PCIE_SUB_CTRL_INST_0_I_DRIVER_APB_CLK,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_PCIE_GEN4_1_IPCLKPORT_PCIE_004_G4X1_DWC_PCIE_CTL_INST_0_DBI_ACLK_UG,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_PCIE_GEN4_1_IPCLKPORT_PCIE_004_G4X1_DWC_PCIE_CTL_INST_0_MSTR_ACLK_UG,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_PCIE_GEN4_1_IPCLKPORT_PCIE_004_G4X1_DWC_PCIE_CTL_INST_0_SLV_ACLK_UG,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_PCIE_GEN4_1_IPCLKPORT_PCIE_004_PCIE_SUB_CTRL_INST_0_I_DRIVER_APB_CLK,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_PCIE_GEN4_1_IPCLKPORT_PCS_PMA_INST_0_PHY_UDBG_I_APB_PCLK,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_PCIE_GEN4_1_IPCLKPORT_PCS_PMA_INST_0_PIPE_PAL_PCIE_INST_0_I_APB_PCLK,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_PCIE_GEN4_1_IPCLKPORT_PCS_PMA_INST_0_SF_PCIEPHY210X2_LN05LPE_QCH_TM_WRAPPER_INST_0_I_APB_PCLK,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_PCIE_IA_GEN4A_1_IPCLKPORT_I_CLK,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_PCIE_IA_GEN4B_1_IPCLKPORT_I_CLK,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_PPMU_HSI2_IPCLKPORT_ACLK,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_PPMU_HSI2_IPCLKPORT_PCLK,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_QE_MMC_CARD_HSI2_IPCLKPORT_ACLK,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_QE_MMC_CARD_HSI2_IPCLKPORT_PCLK,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_QE_PCIE_GEN4A_HSI2_IPCLKPORT_ACLK,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_QE_PCIE_GEN4A_HSI2_IPCLKPORT_PCLK,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_QE_PCIE_GEN4B_HSI2_IPCLKPORT_ACLK,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_QE_PCIE_GEN4B_HSI2_IPCLKPORT_PCLK,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_QE_UFS_EMBD_HSI2_IPCLKPORT_ACLK,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_QE_UFS_EMBD_HSI2_IPCLKPORT_PCLK,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_RSTNSYNC_CLK_HSI2_NOCP_IPCLKPORT_CLK,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_RSTNSYNC_CLK_HSI2_NOC_IPCLKPORT_CLK,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_RSTNSYNC_CLK_HSI2_OSCCLK_IPCLKPORT_CLK,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_SSMT_HSI2_IPCLKPORT_ACLK,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_SSMT_HSI2_IPCLKPORT_PCLK,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_SYSMMU_HSI2_IPCLKPORT_CLK_S2,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_SYSREG_HSI2_IPCLKPORT_PCLK,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_UASC_PCIE_GEN4A_DBI_1_IPCLKPORT_ACLK,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_UASC_PCIE_GEN4A_DBI_1_IPCLKPORT_PCLK,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_UASC_PCIE_GEN4A_SLV_1_IPCLKPORT_ACLK,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_UASC_PCIE_GEN4A_SLV_1_IPCLKPORT_PCLK,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_UASC_PCIE_GEN4B_DBI_1_IPCLKPORT_ACLK,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_UASC_PCIE_GEN4B_DBI_1_IPCLKPORT_PCLK,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_UASC_PCIE_GEN4B_SLV_1_IPCLKPORT_ACLK,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_UASC_PCIE_GEN4B_SLV_1_IPCLKPORT_PCLK,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_UFS_EMBD_IPCLKPORT_I_ACLK,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_UFS_EMBD_IPCLKPORT_I_CLK_UNIPRO,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_UFS_EMBD_IPCLKPORT_I_FMP_CLK,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_XIU_D_HSI2_IPCLKPORT_ACLK,
	HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_XIU_P_HSI2_IPCLKPORT_ACLK,
};

#define CLKS_NR_HSI2 70

PNAME(mout_hsi2_noc_user_p) = { "oscclk", "dout_cmu_hsi2_noc" };
PNAME(mout_hsi2_mmc_card_user_p) = { "oscclk", "dout_cmu_hsi2_mmc_card" };
PNAME(mout_hsi2_pcie_user_p) = { "oscclk", "dout_cmu_hsi2_pcie" };
PNAME(mout_hsi2_ufs_embd_user_p) = { "oscclk", "dout_cmu_hsi2_ufs_embd" };

static const struct samsung_mux_clock hsi2_mux_clks[] __initconst = {
	MUX(CLK_MOUT_HSI2_NOC_USER, "mout_hsi2_noc_user", mout_hsi2_noc_user_p,
	    HSI2_PLL_CON0_MUX_CLKCMU_HSI2_NOC_USER, 4, 1),
	MUX(CLK_MOUT_HSI2_MMC_CARD_USER, "mout_hsi2_mmc_card_user",
	    mout_hsi2_mmc_card_user_p,
	    HSI2_PLL_CON0_MUX_CLKCMU_HSI2_MMC_CARD_USER, 4, 1),
	MUX(CLK_MOUT_HSI2_PCIE_USER, "mout_hsi2_pcie_user",
	    mout_hsi2_pcie_user_p, HSI2_PLL_CON0_MUX_CLKCMU_HSI2_PCIE_USER, 4,
	    1),
	MUX(CLK_MOUT_HSI2_UFS_EMBD_USER, "mout_hsi2_ufs_embd_user",
	    mout_hsi2_ufs_embd_user_p,
	    HSI2_PLL_CON0_MUX_CLKCMU_HSI2_UFS_EMBD_USER, 4, 1),
};

static const struct samsung_div_clock hsi2_div_clks[] __initconst = {
	DIV(CLK_DOUT_HSI2_NOCP, "dout_hsi2_nocp", "mout_hsi2_noc_user",
	    HSI2_CLK_CON_DIV_DIV_CLK_HSI2_NOCP, 0, 3),
	DIV(CLK_DOUT_HSI2_NOC_LH, "dout_hsi2_noc_lh", "mout_hsi2_noc_user",
	    HSI2_CLK_CON_DIV_DIV_CLK_HSI2_NOC_LH, 0, 3),
};

static const struct samsung_gate_clock hsi2_gate_clks[] __initconst = {
	GATE(CLK_GOUT_HSI2_GPIO_HSI2UFS_PCLK, "gout_hsi2_gpio_hsi2ufs_pclk",
	     "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_CLK_BLK_HSI2_UID_GPIO_HSI2UFS_IPCLKPORT_PCLK, 21,
	     0, 0),
	GATE(CLK_GOUT_HSI2_LH_AXI_SI_P_HSI2_CU_I_CLK,
	     "gout_hsi2_lh_axi_si_p_hsi2_cu_i_clk", "dout_hsi2_noc_lh",
	     HSI2_CLK_CON_GAT_CLK_BLK_HSI2_UID_LH_AXI_SI_P_HSI2_CU_IPCLKPORT_I_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_PCIE_GEN4_1_PCIE_003_PCIE_SUB_CTRL_INST_0_PHY_REFCLK_IN,
	     "gout_hsi2_pcie_gen4_1_pcie_003_pcie_sub_ctrl_inst_0_phy_refclk_in",
	     "mout_hsi2_pcie_user",
	     HSI2_CLK_CON_GAT_CLK_BLK_HSI2_UID_PCIE_GEN4_1_IPCLKPORT_PCIE_003_PCIE_SUB_CTRL_INST_0_PHY_REFCLK_IN,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_PCIE_GEN4_1_PCIE_004_PCIE_SUB_CTRL_INST_0_PHY_REFCLK_IN,
	     "gout_hsi2_pcie_gen4_1_pcie_004_pcie_sub_ctrl_inst_0_phy_refclk_in",
	     "mout_hsi2_pcie_user",
	     HSI2_CLK_CON_GAT_CLK_BLK_HSI2_UID_PCIE_GEN4_1_IPCLKPORT_PCIE_004_PCIE_SUB_CTRL_INST_0_PHY_REFCLK_IN,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_RSTNSYNC_CLK_HSI2_NOC_LH_CLK,
	     "gout_hsi2_rstnsync_clk_hsi2_noc_lh_clk", "dout_hsi2_noc_lh",
	     HSI2_CLK_CON_GAT_CLK_BLK_HSI2_UID_RSTNSYNC_CLK_HSI2_NOC_LH_IPCLKPORT_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_SLH_AXI_MI_P_HSI2_I_CLK,
	     "gout_hsi2_slh_axi_mi_p_hsi2_i_clk", "dout_hsi2_noc_lh",
	     HSI2_CLK_CON_GAT_CLK_BLK_HSI2_UID_SLH_AXI_MI_P_HSI2_IPCLKPORT_I_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_SSMT_PCIE_IA_GEN4A_1_ACLK,
	     "gout_hsi2_ssmt_pcie_ia_gen4a_1_aclk", "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_CLK_BLK_HSI2_UID_SSMT_PCIE_IA_GEN4A_1_IPCLKPORT_ACLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_SSMT_PCIE_IA_GEN4A_1_PCLK,
	     "gout_hsi2_ssmt_pcie_ia_gen4a_1_pclk", "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_CLK_BLK_HSI2_UID_SSMT_PCIE_IA_GEN4A_1_IPCLKPORT_PCLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_SSMT_PCIE_IA_GEN4B_1_ACLK,
	     "gout_hsi2_ssmt_pcie_ia_gen4b_1_aclk", "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_CLK_BLK_HSI2_UID_SSMT_PCIE_IA_GEN4B_1_IPCLKPORT_ACLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_SSMT_PCIE_IA_GEN4B_1_PCLK,
	     "gout_hsi2_ssmt_pcie_ia_gen4b_1_pclk", "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_CLK_BLK_HSI2_UID_SSMT_PCIE_IA_GEN4B_1_IPCLKPORT_PCLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_SYSMMU_HSI2_CLK_S1, "gout_hsi2_sysmmu_hsi2_clk_s1",
	     "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_CLK_BLK_HSI2_UID_SYSMMU_HSI2_IPCLKPORT_CLK_S1, 21,
	     0, 0),
	GATE(CLK_GOUT_HSI2_AS_APB_PCIEPHY_HSI2_PCLKM,
	     "gout_hsi2_as_apb_pciephy_hsi2_pclkm", "dout_hsi2_nocp",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_AS_APB_PCIEPHY_HSI2_IPCLKPORT_PCLKM,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_D_TZPC_HSI2_PCLK, "gout_hsi2_d_tzpc_hsi2_pclk",
	     "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_D_TZPC_HSI2_IPCLKPORT_PCLK, 21,
	     0, 0),
	GATE(CLK_GOUT_HSI2_GPC_HSI2_PCLK, "gout_hsi2_gpc_hsi2_pclk",
	     "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_GPC_HSI2_IPCLKPORT_PCLK, 21, 0,
	     0),
	GATE(CLK_GOUT_HSI2_GPIO_HSI2_PCLK, "gout_hsi2_gpio_hsi2_pclk",
	     "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_GPIO_HSI2_IPCLKPORT_PCLK, 21, 0,
	     0),
	GATE(CLK_GOUT_HSI2_HSI2_CMU_HSI2_PCLK, "gout_hsi2_hsi2_cmu_hsi2_pclk",
	     "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_HSI2_CMU_HSI2_IPCLKPORT_PCLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_LH_ACEL_SI_D_HSI2_I_CLK,
	     "gout_hsi2_lh_acel_si_d_hsi2_i_clk", "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_LH_ACEL_SI_D_HSI2_IPCLKPORT_I_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_LH_AXI_MI_P_HSI2_CU_I_CLK,
	     "gout_hsi2_lh_axi_mi_p_hsi2_cu_i_clk", "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_LH_AXI_MI_P_HSI2_CU_IPCLKPORT_I_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_MMC_CARD_I_ACLK, "gout_hsi2_mmc_card_i_aclk",
	     "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_MMC_CARD_IPCLKPORT_I_ACLK, 21,
	     0, 0),
	GATE(CLK_GOUT_HSI2_MMC_CARD_SDCLKIN, "gout_hsi2_mmc_card_sdclkin",
	     "mout_hsi2_mmc_card_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_MMC_CARD_IPCLKPORT_SDCLKIN, 21,
	     0, 0),
	GATE(CLK_GOUT_HSI2_PCIE_GEN4_1_PCIE_003_G4X2_DWC_PCIE_CTL_INST_0_DBI_ACLK_UG,
	     "gout_hsi2_pcie_gen4_1_pcie_003_g4x2_dwc_pcie_ctl_inst_0_dbi_aclk_ug",
	     "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_PCIE_GEN4_1_IPCLKPORT_PCIE_003_G4X2_DWC_PCIE_CTL_INST_0_DBI_ACLK_UG,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_PCIE_GEN4_1_PCIE_003_G4X2_DWC_PCIE_CTL_INST_0_MSTR_ACLK_UG,
	     "gout_hsi2_pcie_gen4_1_pcie_003_g4x2_dwc_pcie_ctl_inst_0_mstr_aclk_ug",
	     "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_PCIE_GEN4_1_IPCLKPORT_PCIE_003_G4X2_DWC_PCIE_CTL_INST_0_MSTR_ACLK_UG,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_PCIE_GEN4_1_PCIE_003_G4X2_DWC_PCIE_CTL_INST_0_SLV_ACLK_UG,
	     "gout_hsi2_pcie_gen4_1_pcie_003_g4x2_dwc_pcie_ctl_inst_0_slv_aclk_ug",
	     "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_PCIE_GEN4_1_IPCLKPORT_PCIE_003_G4X2_DWC_PCIE_CTL_INST_0_SLV_ACLK_UG,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_PCIE_GEN4_1_PCIE_003_PCIE_SUB_CTRL_INST_0_I_DRIVER_APB_CLK,
	     "gout_hsi2_pcie_gen4_1_pcie_003_pcie_sub_ctrl_inst_0_i_driver_apb_clk",
	     "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_PCIE_GEN4_1_IPCLKPORT_PCIE_003_PCIE_SUB_CTRL_INST_0_I_DRIVER_APB_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_PCIE_GEN4_1_PCIE_004_G4X1_DWC_PCIE_CTL_INST_0_DBI_ACLK_UG,
	     "gout_hsi2_pcie_gen4_1_pcie_004_g4x1_dwc_pcie_ctl_inst_0_dbi_aclk_ug",
	     "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_PCIE_GEN4_1_IPCLKPORT_PCIE_004_G4X1_DWC_PCIE_CTL_INST_0_DBI_ACLK_UG,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_PCIE_GEN4_1_PCIE_004_G4X1_DWC_PCIE_CTL_INST_0_MSTR_ACLK_UG,
	     "gout_hsi2_pcie_gen4_1_pcie_004_g4x1_dwc_pcie_ctl_inst_0_mstr_aclk_ug",
	     "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_PCIE_GEN4_1_IPCLKPORT_PCIE_004_G4X1_DWC_PCIE_CTL_INST_0_MSTR_ACLK_UG,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_PCIE_GEN4_1_PCIE_004_G4X1_DWC_PCIE_CTL_INST_0_SLV_ACLK_UG,
	     "gout_hsi2_pcie_gen4_1_pcie_004_g4x1_dwc_pcie_ctl_inst_0_slv_aclk_ug",
	     "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_PCIE_GEN4_1_IPCLKPORT_PCIE_004_G4X1_DWC_PCIE_CTL_INST_0_SLV_ACLK_UG,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_PCIE_GEN4_1_PCIE_004_PCIE_SUB_CTRL_INST_0_I_DRIVER_APB_CLK,
	     "gout_hsi2_pcie_gen4_1_pcie_004_pcie_sub_ctrl_inst_0_i_driver_apb_clk",
	     "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_PCIE_GEN4_1_IPCLKPORT_PCIE_004_PCIE_SUB_CTRL_INST_0_I_DRIVER_APB_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_PCIE_GEN4_1_PCS_PMA_INST_0_PHY_UDBG_I_APB_PCLK,
	     "gout_hsi2_pcie_gen4_1_pcs_pma_inst_0_phy_udbg_i_apb_pclk", "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_PCIE_GEN4_1_IPCLKPORT_PCS_PMA_INST_0_PHY_UDBG_I_APB_PCLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_PCIE_GEN4_1_PCS_PMA_INST_0_PIPE_PAL_PCIE_INST_0_I_APB_PCLK,
	     "gout_hsi2_pcie_gen4_1_pcs_pma_inst_0_pipe_pal_pcie_inst_0_i_apb_pclk",
	     "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_PCIE_GEN4_1_IPCLKPORT_PCS_PMA_INST_0_PIPE_PAL_PCIE_INST_0_I_APB_PCLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_PCIE_GEN4_1_PCS_PMA_INST_0_SF_PCIEPHY210X2_LN05LPE_QCH_TM_WRAPPER_INST_0_I_APB_PCLK,
	     "gout_hsi2_pcie_gen4_1_pcs_pma_inst_0_sf_pciephy210x2_ln05lpe_qch_tm_wrapper_inst_0_i_apb_pclk",
	     "dout_hsi2_nocp",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_PCIE_GEN4_1_IPCLKPORT_PCS_PMA_INST_0_SF_PCIEPHY210X2_LN05LPE_QCH_TM_WRAPPER_INST_0_I_APB_PCLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_PCIE_IA_GEN4A_1_I_CLK,
	     "gout_hsi2_pcie_ia_gen4a_1_i_clk", "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_PCIE_IA_GEN4A_1_IPCLKPORT_I_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_PCIE_IA_GEN4B_1_I_CLK,
	     "gout_hsi2_pcie_ia_gen4b_1_i_clk", "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_PCIE_IA_GEN4B_1_IPCLKPORT_I_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_PPMU_HSI2_ACLK, "gout_hsi2_ppmu_hsi2_aclk",
	     "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_PPMU_HSI2_IPCLKPORT_ACLK, 21, 0,
	     0),
	GATE(CLK_GOUT_HSI2_PPMU_HSI2_PCLK, "gout_hsi2_ppmu_hsi2_pclk",
	     "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_PPMU_HSI2_IPCLKPORT_PCLK, 21, 0,
	     0),
	GATE(CLK_GOUT_HSI2_QE_MMC_CARD_HSI2_ACLK,
	     "gout_hsi2_qe_mmc_card_hsi2_aclk", "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_QE_MMC_CARD_HSI2_IPCLKPORT_ACLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_QE_MMC_CARD_HSI2_PCLK,
	     "gout_hsi2_qe_mmc_card_hsi2_pclk", "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_QE_MMC_CARD_HSI2_IPCLKPORT_PCLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_QE_PCIE_GEN4A_HSI2_ACLK,
	     "gout_hsi2_qe_pcie_gen4a_hsi2_aclk", "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_QE_PCIE_GEN4A_HSI2_IPCLKPORT_ACLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_QE_PCIE_GEN4A_HSI2_PCLK,
	     "gout_hsi2_qe_pcie_gen4a_hsi2_pclk", "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_QE_PCIE_GEN4A_HSI2_IPCLKPORT_PCLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_QE_PCIE_GEN4B_HSI2_ACLK,
	     "gout_hsi2_qe_pcie_gen4b_hsi2_aclk", "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_QE_PCIE_GEN4B_HSI2_IPCLKPORT_ACLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_QE_PCIE_GEN4B_HSI2_PCLK,
	     "gout_hsi2_qe_pcie_gen4b_hsi2_pclk", "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_QE_PCIE_GEN4B_HSI2_IPCLKPORT_PCLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_QE_UFS_EMBD_HSI2_ACLK,
	     "gout_hsi2_qe_ufs_embd_hsi2_aclk", "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_QE_UFS_EMBD_HSI2_IPCLKPORT_ACLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_QE_UFS_EMBD_HSI2_PCLK,
	     "gout_hsi2_qe_ufs_embd_hsi2_pclk", "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_QE_UFS_EMBD_HSI2_IPCLKPORT_PCLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_RSTNSYNC_CLK_HSI2_NOCP_CLK,
	     "gout_hsi2_rstnsync_clk_hsi2_nocp_clk", "dout_hsi2_nocp",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_RSTNSYNC_CLK_HSI2_NOCP_IPCLKPORT_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_RSTNSYNC_CLK_HSI2_NOC_CLK,
	     "gout_hsi2_rstnsync_clk_hsi2_noc_clk", "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_RSTNSYNC_CLK_HSI2_NOC_IPCLKPORT_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_RSTNSYNC_CLK_HSI2_OSCCLK_CLK,
	     "gout_hsi2_rstnsync_clk_hsi2_oscclk_clk", "oscclk",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_RSTNSYNC_CLK_HSI2_OSCCLK_IPCLKPORT_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_SSMT_HSI2_ACLK, "gout_hsi2_ssmt_hsi2_aclk",
	     "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_SSMT_HSI2_IPCLKPORT_ACLK, 21, 0,
	     0),
	GATE(CLK_GOUT_HSI2_SSMT_HSI2_PCLK, "gout_hsi2_ssmt_hsi2_pclk",
	     "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_SSMT_HSI2_IPCLKPORT_PCLK, 21, 0,
	     0),
	GATE(CLK_GOUT_HSI2_SYSMMU_HSI2_CLK_S2, "gout_hsi2_sysmmu_hsi2_clk_s2",
	     "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_SYSMMU_HSI2_IPCLKPORT_CLK_S2,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_SYSREG_HSI2_PCLK, "gout_hsi2_sysreg_hsi2_pclk",
	     "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_SYSREG_HSI2_IPCLKPORT_PCLK, 21,
	     0, 0),
	GATE(CLK_GOUT_HSI2_UASC_PCIE_GEN4A_DBI_1_ACLK,
	     "gout_hsi2_uasc_pcie_gen4a_dbi_1_aclk", "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_UASC_PCIE_GEN4A_DBI_1_IPCLKPORT_ACLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_UASC_PCIE_GEN4A_DBI_1_PCLK,
	     "gout_hsi2_uasc_pcie_gen4a_dbi_1_pclk", "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_UASC_PCIE_GEN4A_DBI_1_IPCLKPORT_PCLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_UASC_PCIE_GEN4A_SLV_1_ACLK,
	     "gout_hsi2_uasc_pcie_gen4a_slv_1_aclk", "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_UASC_PCIE_GEN4A_SLV_1_IPCLKPORT_ACLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_UASC_PCIE_GEN4A_SLV_1_PCLK,
	     "gout_hsi2_uasc_pcie_gen4a_slv_1_pclk", "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_UASC_PCIE_GEN4A_SLV_1_IPCLKPORT_PCLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_UASC_PCIE_GEN4B_DBI_1_ACLK,
	     "gout_hsi2_uasc_pcie_gen4b_dbi_1_aclk", "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_UASC_PCIE_GEN4B_DBI_1_IPCLKPORT_ACLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_UASC_PCIE_GEN4B_DBI_1_PCLK,
	     "gout_hsi2_uasc_pcie_gen4b_dbi_1_pclk", "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_UASC_PCIE_GEN4B_DBI_1_IPCLKPORT_PCLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_UASC_PCIE_GEN4B_SLV_1_ACLK,
	     "gout_hsi2_uasc_pcie_gen4b_slv_1_aclk", "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_UASC_PCIE_GEN4B_SLV_1_IPCLKPORT_ACLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_UASC_PCIE_GEN4B_SLV_1_PCLK,
	     "gout_hsi2_uasc_pcie_gen4b_slv_1_pclk", "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_UASC_PCIE_GEN4B_SLV_1_IPCLKPORT_PCLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_UFS_EMBD_I_ACLK, "gout_hsi2_ufs_embd_i_aclk",
	     "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_UFS_EMBD_IPCLKPORT_I_ACLK, 21,
	     0, 0),
	GATE(CLK_GOUT_HSI2_UFS_EMBD_I_CLK_UNIPRO,
	     "gout_hsi2_ufs_embd_i_clk_unipro", "mout_hsi2_ufs_embd_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_UFS_EMBD_IPCLKPORT_I_CLK_UNIPRO,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_UFS_EMBD_I_FMP_CLK, "gout_hsi2_ufs_embd_i_fmp_clk",
	     "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_UFS_EMBD_IPCLKPORT_I_FMP_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_HSI2_XIU_D_HSI2_ACLK, "gout_hsi2_xiu_d_hsi2_aclk",
	     "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_XIU_D_HSI2_IPCLKPORT_ACLK, 21,
	     0, 0),
	GATE(CLK_GOUT_HSI2_XIU_P_HSI2_ACLK, "gout_hsi2_xiu_p_hsi2_aclk",
	     "mout_hsi2_noc_user",
	     HSI2_CLK_CON_GAT_GOUT_BLK_HSI2_UID_XIU_P_HSI2_IPCLKPORT_ACLK, 21,
	     0, 0),
};

static const struct samsung_cmu_info hsi2_cmu_info __initconst = {
	.mux_clks = hsi2_mux_clks,
	.nr_mux_clks = ARRAY_SIZE(hsi2_mux_clks),
	.div_clks = hsi2_div_clks,
	.nr_div_clks = ARRAY_SIZE(hsi2_div_clks),
	.gate_clks = hsi2_gate_clks,
	.nr_gate_clks = ARRAY_SIZE(hsi2_gate_clks),
	.nr_clk_ids = CLKS_NR_HSI2,
	.clk_regs = hsi2_clk_regs,
	.nr_clk_regs = ARRAY_SIZE(hsi2_clk_regs),
	.clk_name = "bus",
};

/* ---- CMU_DPU ------------------------------------------------------------- */
/* Register offsets at 0x1c000000. */
#define DPU_PLL_CON0_MUX_CLKCMU_DPU_NOC_USER 0x0600
#define DPU_CLK_CON_DIV_DIV_CLK_DPU_NOCP 0x1800
#define DPU_CLK_CON_GAT_CLK_BLK_DPU_UID_DPU_CMU_DPU_IPCLKPORT_PCLK 0x2000
#define DPU_CLK_CON_GAT_CLK_BLK_DPU_UID_RSTNSYNC_CLK_DPU_OSCCLK_IPCLKPORT_CLK \
	0x200c
#define DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_AD_APB_DPU_DMA_IPCLKPORT_PCLKM 0x2010
#define DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_DPUF_IPCLKPORT_ACLK_DMA 0x2014
#define DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_DPUF_IPCLKPORT_ACLK_DPP 0x2018
#define DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_D_TZPC_DPU_IPCLKPORT_PCLK 0x201c
#define DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_GPC_DPU_IPCLKPORT_PCLK 0x2020
#define DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_LH_AXI_SI_D0_DPU_IPCLKPORT_I_CLK 0x2024
#define DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_LH_AXI_SI_D1_DPU_IPCLKPORT_I_CLK 0x2028
#define DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_LH_AXI_SI_D2_DPU_IPCLKPORT_I_CLK 0x202c
#define DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_PPMU_DPUD0_IPCLKPORT_ACLK 0x2030
#define DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_PPMU_DPUD0_IPCLKPORT_PCLK 0x2034
#define DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_PPMU_DPUD1_IPCLKPORT_ACLK 0x2038
#define DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_PPMU_DPUD1_IPCLKPORT_PCLK 0x203c
#define DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_PPMU_DPUD2_IPCLKPORT_ACLK 0x2040
#define DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_PPMU_DPUD2_IPCLKPORT_PCLK 0x2044
#define DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_RSTNSYNC_CLK_DPU_NOCD_IPCLKPORT_CLK \
	0x2048
#define DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_RSTNSYNC_CLK_DPU_NOCP_IPCLKPORT_CLK \
	0x204c
#define DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_SLH_AXI_MI_P_DPU_IPCLKPORT_I_CLK 0x2050
#define DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_SSMT_DPU0_IPCLKPORT_ACLK 0x2054
#define DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_SSMT_DPU0_IPCLKPORT_PCLK 0x2058
#define DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_SSMT_DPU1_IPCLKPORT_ACLK 0x205c
#define DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_SSMT_DPU1_IPCLKPORT_PCLK 0x2060
#define DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_SSMT_DPU2_IPCLKPORT_ACLK 0x2064
#define DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_SSMT_DPU2_IPCLKPORT_PCLK 0x2068
#define DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_SYSMMU_DPUD0_IPCLKPORT_CLK_S1 0x206c
#define DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_SYSMMU_DPUD0_IPCLKPORT_CLK_S2 0x2070
#define DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_SYSMMU_DPUD1_IPCLKPORT_CLK_S1 0x2074
#define DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_SYSMMU_DPUD1_IPCLKPORT_CLK_S2 0x2078
#define DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_SYSMMU_DPUD2_IPCLKPORT_CLK_S1 0x207c
#define DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_SYSMMU_DPUD2_IPCLKPORT_CLK_S2 0x2080
#define DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_SYSREG_DPU_IPCLKPORT_PCLK 0x2084

static const unsigned long dpu_clk_regs[] __initconst = {
	DPU_PLL_CON0_MUX_CLKCMU_DPU_NOC_USER,
	DPU_CLK_CON_DIV_DIV_CLK_DPU_NOCP,
	DPU_CLK_CON_GAT_CLK_BLK_DPU_UID_DPU_CMU_DPU_IPCLKPORT_PCLK,
	DPU_CLK_CON_GAT_CLK_BLK_DPU_UID_RSTNSYNC_CLK_DPU_OSCCLK_IPCLKPORT_CLK,
	DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_AD_APB_DPU_DMA_IPCLKPORT_PCLKM,
	DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_DPUF_IPCLKPORT_ACLK_DMA,
	DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_DPUF_IPCLKPORT_ACLK_DPP,
	DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_D_TZPC_DPU_IPCLKPORT_PCLK,
	DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_GPC_DPU_IPCLKPORT_PCLK,
	DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_LH_AXI_SI_D0_DPU_IPCLKPORT_I_CLK,
	DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_LH_AXI_SI_D1_DPU_IPCLKPORT_I_CLK,
	DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_LH_AXI_SI_D2_DPU_IPCLKPORT_I_CLK,
	DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_PPMU_DPUD0_IPCLKPORT_ACLK,
	DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_PPMU_DPUD0_IPCLKPORT_PCLK,
	DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_PPMU_DPUD1_IPCLKPORT_ACLK,
	DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_PPMU_DPUD1_IPCLKPORT_PCLK,
	DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_PPMU_DPUD2_IPCLKPORT_ACLK,
	DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_PPMU_DPUD2_IPCLKPORT_PCLK,
	DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_RSTNSYNC_CLK_DPU_NOCD_IPCLKPORT_CLK,
	DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_RSTNSYNC_CLK_DPU_NOCP_IPCLKPORT_CLK,
	DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_SLH_AXI_MI_P_DPU_IPCLKPORT_I_CLK,
	DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_SSMT_DPU0_IPCLKPORT_ACLK,
	DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_SSMT_DPU0_IPCLKPORT_PCLK,
	DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_SSMT_DPU1_IPCLKPORT_ACLK,
	DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_SSMT_DPU1_IPCLKPORT_PCLK,
	DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_SSMT_DPU2_IPCLKPORT_ACLK,
	DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_SSMT_DPU2_IPCLKPORT_PCLK,
	DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_SYSMMU_DPUD0_IPCLKPORT_CLK_S1,
	DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_SYSMMU_DPUD0_IPCLKPORT_CLK_S2,
	DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_SYSMMU_DPUD1_IPCLKPORT_CLK_S1,
	DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_SYSMMU_DPUD1_IPCLKPORT_CLK_S2,
	DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_SYSMMU_DPUD2_IPCLKPORT_CLK_S1,
	DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_SYSMMU_DPUD2_IPCLKPORT_CLK_S2,
	DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_SYSREG_DPU_IPCLKPORT_PCLK,
};

#define CLKS_NR_DPU 35

PNAME(mout_dpu_noc_user_p) = { "oscclk", "dout_cmu_dpu_noc" };

static const struct samsung_mux_clock dpu_mux_clks[] __initconst = {
	MUX(CLK_MOUT_DPU_NOC_USER, "mout_dpu_noc_user", mout_dpu_noc_user_p,
	    DPU_PLL_CON0_MUX_CLKCMU_DPU_NOC_USER, 4, 1),
};

static const struct samsung_div_clock dpu_div_clks[] __initconst = {
	DIV(CLK_DOUT_DPU_NOCP, "dout_dpu_nocp", "mout_dpu_noc_user",
	    DPU_CLK_CON_DIV_DIV_CLK_DPU_NOCP, 0, 3),
};

static const struct samsung_gate_clock dpu_gate_clks[] __initconst = {
	GATE(CLK_GOUT_DPU_DPU_CMU_DPU_PCLK, "gout_dpu_dpu_cmu_dpu_pclk",
	     "dout_dpu_nocp",
	     DPU_CLK_CON_GAT_CLK_BLK_DPU_UID_DPU_CMU_DPU_IPCLKPORT_PCLK, 21, 0,
	     0),
	GATE(CLK_GOUT_DPU_RSTNSYNC_CLK_DPU_OSCCLK_CLK,
	     "gout_dpu_rstnsync_clk_dpu_oscclk_clk", "oscclk",
	     DPU_CLK_CON_GAT_CLK_BLK_DPU_UID_RSTNSYNC_CLK_DPU_OSCCLK_IPCLKPORT_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_DPU_AD_APB_DPU_DMA_PCLKM, "gout_dpu_ad_apb_dpu_dma_pclkm",
	     "mout_dpu_noc_user",
	     DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_AD_APB_DPU_DMA_IPCLKPORT_PCLKM,
	     21, 0, 0),
	GATE(CLK_GOUT_DPU_DPUF_ACLK_DMA, "gout_dpu_dpuf_aclk_dma",
	     "mout_dpu_noc_user",
	     DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_DPUF_IPCLKPORT_ACLK_DMA, 21, 0,
	     0),
	GATE(CLK_GOUT_DPU_DPUF_ACLK_DPP, "gout_dpu_dpuf_aclk_dpp",
	     "mout_dpu_noc_user",
	     DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_DPUF_IPCLKPORT_ACLK_DPP, 21, 0,
	     0),
	GATE(CLK_GOUT_DPU_D_TZPC_DPU_PCLK, "gout_dpu_d_tzpc_dpu_pclk",
	     "dout_dpu_nocp",
	     DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_D_TZPC_DPU_IPCLKPORT_PCLK, 21, 0,
	     0),
	GATE(CLK_GOUT_DPU_GPC_DPU_PCLK, "gout_dpu_gpc_dpu_pclk",
	     "dout_dpu_nocp",
	     DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_GPC_DPU_IPCLKPORT_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_DPU_LH_AXI_SI_D0_DPU_I_CLK,
	     "gout_dpu_lh_axi_si_d0_dpu_i_clk", "mout_dpu_noc_user",
	     DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_LH_AXI_SI_D0_DPU_IPCLKPORT_I_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_DPU_LH_AXI_SI_D1_DPU_I_CLK,
	     "gout_dpu_lh_axi_si_d1_dpu_i_clk", "mout_dpu_noc_user",
	     DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_LH_AXI_SI_D1_DPU_IPCLKPORT_I_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_DPU_LH_AXI_SI_D2_DPU_I_CLK,
	     "gout_dpu_lh_axi_si_d2_dpu_i_clk", "mout_dpu_noc_user",
	     DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_LH_AXI_SI_D2_DPU_IPCLKPORT_I_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_DPU_PPMU_DPUD0_ACLK, "gout_dpu_ppmu_dpud0_aclk",
	     "mout_dpu_noc_user",
	     DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_PPMU_DPUD0_IPCLKPORT_ACLK, 21, 0,
	     0),
	GATE(CLK_GOUT_DPU_PPMU_DPUD0_PCLK, "gout_dpu_ppmu_dpud0_pclk",
	     "dout_dpu_nocp",
	     DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_PPMU_DPUD0_IPCLKPORT_PCLK, 21, 0,
	     0),
	GATE(CLK_GOUT_DPU_PPMU_DPUD1_ACLK, "gout_dpu_ppmu_dpud1_aclk",
	     "mout_dpu_noc_user",
	     DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_PPMU_DPUD1_IPCLKPORT_ACLK, 21, 0,
	     0),
	GATE(CLK_GOUT_DPU_PPMU_DPUD1_PCLK, "gout_dpu_ppmu_dpud1_pclk",
	     "dout_dpu_nocp",
	     DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_PPMU_DPUD1_IPCLKPORT_PCLK, 21, 0,
	     0),
	GATE(CLK_GOUT_DPU_PPMU_DPUD2_ACLK, "gout_dpu_ppmu_dpud2_aclk",
	     "mout_dpu_noc_user",
	     DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_PPMU_DPUD2_IPCLKPORT_ACLK, 21, 0,
	     0),
	GATE(CLK_GOUT_DPU_PPMU_DPUD2_PCLK, "gout_dpu_ppmu_dpud2_pclk",
	     "dout_dpu_nocp",
	     DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_PPMU_DPUD2_IPCLKPORT_PCLK, 21, 0,
	     0),
	GATE(CLK_GOUT_DPU_RSTNSYNC_CLK_DPU_NOCD_CLK,
	     "gout_dpu_rstnsync_clk_dpu_nocd_clk", "mout_dpu_noc_user",
	     DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_RSTNSYNC_CLK_DPU_NOCD_IPCLKPORT_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_DPU_RSTNSYNC_CLK_DPU_NOCP_CLK,
	     "gout_dpu_rstnsync_clk_dpu_nocp_clk", "dout_dpu_nocp",
	     DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_RSTNSYNC_CLK_DPU_NOCP_IPCLKPORT_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_DPU_SLH_AXI_MI_P_DPU_I_CLK,
	     "gout_dpu_slh_axi_mi_p_dpu_i_clk", "dout_dpu_nocp",
	     DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_SLH_AXI_MI_P_DPU_IPCLKPORT_I_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_DPU_SSMT_DPU0_ACLK, "gout_dpu_ssmt_dpu0_aclk",
	     "mout_dpu_noc_user",
	     DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_SSMT_DPU0_IPCLKPORT_ACLK, 21, 0,
	     0),
	GATE(CLK_GOUT_DPU_SSMT_DPU0_PCLK, "gout_dpu_ssmt_dpu0_pclk",
	     "dout_dpu_nocp",
	     DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_SSMT_DPU0_IPCLKPORT_PCLK, 21, 0,
	     0),
	GATE(CLK_GOUT_DPU_SSMT_DPU1_ACLK, "gout_dpu_ssmt_dpu1_aclk",
	     "mout_dpu_noc_user",
	     DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_SSMT_DPU1_IPCLKPORT_ACLK, 21, 0,
	     0),
	GATE(CLK_GOUT_DPU_SSMT_DPU1_PCLK, "gout_dpu_ssmt_dpu1_pclk",
	     "dout_dpu_nocp",
	     DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_SSMT_DPU1_IPCLKPORT_PCLK, 21, 0,
	     0),
	GATE(CLK_GOUT_DPU_SSMT_DPU2_ACLK, "gout_dpu_ssmt_dpu2_aclk",
	     "mout_dpu_noc_user",
	     DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_SSMT_DPU2_IPCLKPORT_ACLK, 21, 0,
	     0),
	GATE(CLK_GOUT_DPU_SSMT_DPU2_PCLK, "gout_dpu_ssmt_dpu2_pclk",
	     "dout_dpu_nocp",
	     DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_SSMT_DPU2_IPCLKPORT_PCLK, 21, 0,
	     0),
	GATE(CLK_GOUT_DPU_SYSMMU_DPUD0_CLK_S1, "gout_dpu_sysmmu_dpud0_clk_s1",
	     "mout_dpu_noc_user",
	     DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_SYSMMU_DPUD0_IPCLKPORT_CLK_S1, 21,
	     0, 0),
	GATE(CLK_GOUT_DPU_SYSMMU_DPUD0_CLK_S2, "gout_dpu_sysmmu_dpud0_clk_s2",
	     "mout_dpu_noc_user",
	     DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_SYSMMU_DPUD0_IPCLKPORT_CLK_S2, 21,
	     0, 0),
	GATE(CLK_GOUT_DPU_SYSMMU_DPUD1_CLK_S1, "gout_dpu_sysmmu_dpud1_clk_s1",
	     "mout_dpu_noc_user",
	     DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_SYSMMU_DPUD1_IPCLKPORT_CLK_S1, 21,
	     0, 0),
	GATE(CLK_GOUT_DPU_SYSMMU_DPUD1_CLK_S2, "gout_dpu_sysmmu_dpud1_clk_s2",
	     "mout_dpu_noc_user",
	     DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_SYSMMU_DPUD1_IPCLKPORT_CLK_S2, 21,
	     0, 0),
	GATE(CLK_GOUT_DPU_SYSMMU_DPUD2_CLK_S1, "gout_dpu_sysmmu_dpud2_clk_s1",
	     "mout_dpu_noc_user",
	     DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_SYSMMU_DPUD2_IPCLKPORT_CLK_S1, 21,
	     0, 0),
	GATE(CLK_GOUT_DPU_SYSMMU_DPUD2_CLK_S2, "gout_dpu_sysmmu_dpud2_clk_s2",
	     "mout_dpu_noc_user",
	     DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_SYSMMU_DPUD2_IPCLKPORT_CLK_S2, 21,
	     0, 0),
	GATE(CLK_GOUT_DPU_SYSREG_DPU_PCLK, "gout_dpu_sysreg_dpu_pclk",
	     "dout_dpu_nocp",
	     DPU_CLK_CON_GAT_GOUT_BLK_DPU_UID_SYSREG_DPU_IPCLKPORT_PCLK, 21, 0,
	     0),
};

static const struct samsung_cmu_info dpu_cmu_info __initconst = {
	.mux_clks = dpu_mux_clks,
	.nr_mux_clks = ARRAY_SIZE(dpu_mux_clks),
	.div_clks = dpu_div_clks,
	.nr_div_clks = ARRAY_SIZE(dpu_div_clks),
	.gate_clks = dpu_gate_clks,
	.nr_gate_clks = ARRAY_SIZE(dpu_gate_clks),
	.nr_clk_ids = CLKS_NR_DPU,
	.clk_regs = dpu_clk_regs,
	.nr_clk_regs = ARRAY_SIZE(dpu_clk_regs),
	.clk_name = "bus",
};

/* ---- CMU_DISP ------------------------------------------------------------- */
/* Register offsets at 0x1c200000. */
#define DISP_PLL_CON0_MUX_CLKCMU_DISP_NOC_USER 0x0600
#define DISP_CLK_CON_DIV_DIV_CLK_DISP_NOCP 0x1804
#define DISP_CLK_CON_GAT_CLK_BLK_DISP_UID_DISP_CMU_DISP_IPCLKPORT_PCLK 0x2000
#define DISP_CLK_CON_GAT_CLK_BLK_DISP_UID_SLH_AXI_MI_P_DISP_IPCLKPORT_I_CLK \
	0x200c
#define DISP_CLK_CON_GAT_GOUT_BLK_DISP_UID_AD_APB_DECON_MAIN_IPCLKPORT_PCLKM \
	0x2010
#define DISP_CLK_CON_GAT_GOUT_BLK_DISP_UID_DPUB_IPCLKPORT_ACLK_DECON 0x2014
#define DISP_CLK_CON_GAT_GOUT_BLK_DISP_UID_D_TZPC_DISP_IPCLKPORT_PCLK 0x2018
#define DISP_CLK_CON_GAT_GOUT_BLK_DISP_UID_GPC_DISP_IPCLKPORT_PCLK 0x201c
#define DISP_CLK_CON_GAT_GOUT_BLK_DISP_UID_RSTNSYNC_CLK_DISP_NOCD_IPCLKPORT_CLK \
	0x2020
#define DISP_CLK_CON_GAT_GOUT_BLK_DISP_UID_RSTNSYNC_CLK_DISP_NOCP_IPCLKPORT_CLK \
	0x2024
#define DISP_CLK_CON_GAT_GOUT_BLK_DISP_UID_RSTNSYNC_CLK_DISP_OSCCLK_IPCLKPORT_CLK \
	0x2028
#define DISP_CLK_CON_GAT_GOUT_BLK_DISP_UID_SYSREG_DISP_IPCLKPORT_PCLK 0x202c

static const unsigned long disp_clk_regs[] __initconst = {
	DISP_PLL_CON0_MUX_CLKCMU_DISP_NOC_USER,
	DISP_CLK_CON_DIV_DIV_CLK_DISP_NOCP,
	DISP_CLK_CON_GAT_CLK_BLK_DISP_UID_DISP_CMU_DISP_IPCLKPORT_PCLK,
	DISP_CLK_CON_GAT_CLK_BLK_DISP_UID_SLH_AXI_MI_P_DISP_IPCLKPORT_I_CLK,
	DISP_CLK_CON_GAT_GOUT_BLK_DISP_UID_AD_APB_DECON_MAIN_IPCLKPORT_PCLKM,
	DISP_CLK_CON_GAT_GOUT_BLK_DISP_UID_DPUB_IPCLKPORT_ACLK_DECON,
	DISP_CLK_CON_GAT_GOUT_BLK_DISP_UID_D_TZPC_DISP_IPCLKPORT_PCLK,
	DISP_CLK_CON_GAT_GOUT_BLK_DISP_UID_GPC_DISP_IPCLKPORT_PCLK,
	DISP_CLK_CON_GAT_GOUT_BLK_DISP_UID_RSTNSYNC_CLK_DISP_NOCD_IPCLKPORT_CLK,
	DISP_CLK_CON_GAT_GOUT_BLK_DISP_UID_RSTNSYNC_CLK_DISP_NOCP_IPCLKPORT_CLK,
	DISP_CLK_CON_GAT_GOUT_BLK_DISP_UID_RSTNSYNC_CLK_DISP_OSCCLK_IPCLKPORT_CLK,
	DISP_CLK_CON_GAT_GOUT_BLK_DISP_UID_SYSREG_DISP_IPCLKPORT_PCLK,
};

#define CLKS_NR_DISP 13

PNAME(mout_disp_noc_user_p) = { "oscclk", "dout_cmu_disp_noc" };

static const struct samsung_mux_clock disp_mux_clks[] __initconst = {
	MUX(CLK_MOUT_DISP_NOC_USER, "mout_disp_noc_user", mout_disp_noc_user_p,
	    DISP_PLL_CON0_MUX_CLKCMU_DISP_NOC_USER, 4, 1),
};

static const struct samsung_div_clock disp_div_clks[] __initconst = {
	DIV(CLK_DOUT_DISP_NOCP, "dout_disp_nocp", "mout_disp_noc_user",
	    DISP_CLK_CON_DIV_DIV_CLK_DISP_NOCP, 0, 3),
};

static const struct samsung_gate_clock disp_gate_clks[] __initconst = {
	GATE(CLK_GOUT_DISP_DISP_CMU_DISP_PCLK, "gout_disp_disp_cmu_disp_pclk",
	     "dout_disp_nocp",
	     DISP_CLK_CON_GAT_CLK_BLK_DISP_UID_DISP_CMU_DISP_IPCLKPORT_PCLK, 21,
	     0, 0),
	GATE(CLK_GOUT_DISP_SLH_AXI_MI_P_DISP_I_CLK,
	     "gout_disp_slh_axi_mi_p_disp_i_clk", "dout_disp_nocp",
	     DISP_CLK_CON_GAT_CLK_BLK_DISP_UID_SLH_AXI_MI_P_DISP_IPCLKPORT_I_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_DISP_AD_APB_DECON_MAIN_PCLKM,
	     "gout_disp_ad_apb_decon_main_pclkm", "mout_disp_noc_user",
	     DISP_CLK_CON_GAT_GOUT_BLK_DISP_UID_AD_APB_DECON_MAIN_IPCLKPORT_PCLKM,
	     21, 0, 0),
	GATE(CLK_GOUT_DISP_DPUB_ACLK_DECON, "gout_disp_dpub_aclk_decon",
	     "mout_disp_noc_user",
	     DISP_CLK_CON_GAT_GOUT_BLK_DISP_UID_DPUB_IPCLKPORT_ACLK_DECON, 21,
	     0, 0),
	GATE(CLK_GOUT_DISP_D_TZPC_DISP_PCLK, "gout_disp_d_tzpc_disp_pclk",
	     "dout_disp_nocp",
	     DISP_CLK_CON_GAT_GOUT_BLK_DISP_UID_D_TZPC_DISP_IPCLKPORT_PCLK, 21,
	     0, 0),
	GATE(CLK_GOUT_DISP_GPC_DISP_PCLK, "gout_disp_gpc_disp_pclk",
	     "dout_disp_nocp",
	     DISP_CLK_CON_GAT_GOUT_BLK_DISP_UID_GPC_DISP_IPCLKPORT_PCLK, 21, 0,
	     0),
	GATE(CLK_GOUT_DISP_RSTNSYNC_CLK_DISP_NOCD_CLK,
	     "gout_disp_rstnsync_clk_disp_nocd_clk", "mout_disp_noc_user",
	     DISP_CLK_CON_GAT_GOUT_BLK_DISP_UID_RSTNSYNC_CLK_DISP_NOCD_IPCLKPORT_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_DISP_RSTNSYNC_CLK_DISP_NOCP_CLK,
	     "gout_disp_rstnsync_clk_disp_nocp_clk", "dout_disp_nocp",
	     DISP_CLK_CON_GAT_GOUT_BLK_DISP_UID_RSTNSYNC_CLK_DISP_NOCP_IPCLKPORT_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_DISP_RSTNSYNC_CLK_DISP_OSCCLK_CLK,
	     "gout_disp_rstnsync_clk_disp_oscclk_clk", "oscclk",
	     DISP_CLK_CON_GAT_GOUT_BLK_DISP_UID_RSTNSYNC_CLK_DISP_OSCCLK_IPCLKPORT_CLK,
	     21, 0, 0),
	GATE(CLK_GOUT_DISP_SYSREG_DISP_PCLK, "gout_disp_sysreg_disp_pclk",
	     "dout_disp_nocp",
	     DISP_CLK_CON_GAT_GOUT_BLK_DISP_UID_SYSREG_DISP_IPCLKPORT_PCLK, 21,
	     0, 0),
};

static const struct samsung_cmu_info disp_cmu_info __initconst = {
	.mux_clks = disp_mux_clks,
	.nr_mux_clks = ARRAY_SIZE(disp_mux_clks),
	.div_clks = disp_div_clks,
	.nr_div_clks = ARRAY_SIZE(disp_div_clks),
	.gate_clks = disp_gate_clks,
	.nr_gate_clks = ARRAY_SIZE(disp_gate_clks),
	.nr_clk_ids = CLKS_NR_DISP,
	.clk_regs = disp_clk_regs,
	.nr_clk_regs = ARRAY_SIZE(disp_clk_regs),
	.clk_name = "bus",
};

static void __init gs201_cmu_top_init(struct device_node *np)
{
	exynos_arm64_register_cmu(NULL, np, &top_cmu_info);
}

CLK_OF_DECLARE(gs201_cmu_top, "google,gs201-cmu-top", gs201_cmu_top_init);

static void __init gs201_cmu_misc_init(struct device_node *np)
{
	exynos_arm64_register_cmu(NULL, np, &misc_cmu_info);
}

CLK_OF_DECLARE(gs201_cmu_misc, "google,gs201-cmu-misc", gs201_cmu_misc_init);

static int __init gs201_cmu_probe(struct platform_device *pdev)
{
	const struct samsung_cmu_info *cmu = device_get_match_data(&pdev->dev);

	exynos_arm64_register_cmu(&pdev->dev, pdev->dev.of_node, cmu);
	return 0;
}

static const struct of_device_id gs201_cmu_of_match[] = {
	{ .compatible = "google,gs201-cmu-peric0", .data = &peric0_cmu_info },
	{ .compatible = "google,gs201-cmu-peric1", .data = &peric1_cmu_info },
	{ .compatible = "google,gs201-cmu-hsi0", .data = &hsi0_cmu_info },
	{ .compatible = "google,gs201-cmu-hsi2", .data = &hsi2_cmu_info },
	{ .compatible = "google,gs201-cmu-dpu", .data = &dpu_cmu_info },
	{ .compatible = "google,gs201-cmu-disp", .data = &disp_cmu_info },
	{},
};

static struct platform_driver gs201_cmu_driver = {
	.driver = {
		.name = "gs201-cmu",
		.of_match_table = gs201_cmu_of_match,
		.suppress_bind_attrs = true,
	},
};

builtin_platform_driver_probe(gs201_cmu_driver, gs201_cmu_probe);
