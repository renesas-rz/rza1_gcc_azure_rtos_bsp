/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_gpio_drv_sc_cfg.h
* Version      : 1.0.0
* Device(s)    : R7S921053
* Description  : Pin Configuration.
* Creation Date: 2019-04-04
***********************************************************************************************************************/

#ifndef _R_GPIO_RZA1H_CFG_H_
#define _R_GPIO_RZA1H_CFG_H_

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_port_if.h"

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Variable Externs
******************************************************************************/
static st_port_config_t GPIO_SC_TABLE_cpg[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_cpg = { 0,	0,	&GPIO_SC_TABLE_cpg[0]	};
static st_port_config_t GPIO_SC_TABLE_intc[] =
{
	{P1_8,	FUNCTION_MODE2,	PIN_INPUT},	// Pin Function : IRQ2
	{P4_9,	FUNCTION_MODE7,	PIN_INPUT},	// Pin Function : IRQ1
	{P4_14,	FUNCTION_MODE8,	PIN_INPUT},	// Pin Function : IRQ6
};
static const st_port_init_config_t GPIO_SC_INIT_intc = { 0,	3,	&GPIO_SC_TABLE_intc[0]	};
static st_port_config_t GPIO_SC_TABLE_bsc[] =
{
	{P7_9,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : A1
	{P7_10,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : A2
	{P7_11,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : A3
	{P7_12,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : A4
	{P7_13,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : A5
	{P7_14,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : A6
	{P7_15,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : A7
	{P8_0,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : A8
	{P8_1,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : A9
	{P7_8,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : RD#
	{P7_7,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : WE1#/DQMLU
	{P6_0,	FUNCTION_MODE1,	PIN_IO},	// Pin Function : D0
	{P6_1,	FUNCTION_MODE1,	PIN_IO},	// Pin Function : D1
	{P6_2,	FUNCTION_MODE1,	PIN_IO},	// Pin Function : D2
	{P6_3,	FUNCTION_MODE1,	PIN_IO},	// Pin Function : D3
	{P6_4,	FUNCTION_MODE1,	PIN_IO},	// Pin Function : D4
	{P6_5,	FUNCTION_MODE1,	PIN_IO},	// Pin Function : D5
	{P6_6,	FUNCTION_MODE1,	PIN_IO},	// Pin Function : D6
	{P6_7,	FUNCTION_MODE1,	PIN_IO},	// Pin Function : D7
	{P6_8,	FUNCTION_MODE1,	PIN_IO},	// Pin Function : D8
	{P6_9,	FUNCTION_MODE1,	PIN_IO},	// Pin Function : D9
	{P7_5,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : RD/WR#
	{P7_2,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : RAS#
	{P7_4,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : CKE
	{P6_10,	FUNCTION_MODE1,	PIN_IO},	// Pin Function : D10
	{P6_12,	FUNCTION_MODE1,	PIN_IO},	// Pin Function : D12
	{P6_11,	FUNCTION_MODE1,	PIN_IO},	// Pin Function : D11
	{P6_14,	FUNCTION_MODE1,	PIN_IO},	// Pin Function : D14
	{P6_13,	FUNCTION_MODE1,	PIN_IO},	// Pin Function : D13
	{P6_15,	FUNCTION_MODE1,	PIN_IO},	// Pin Function : D15
	{P7_6,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : WE0#/DQMLL
	{P8_2,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : A10
	{P8_3,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : A11
	{P8_4,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : A12
	{P8_5,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : A13
	{P8_6,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : A14
	{P8_7,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : A15
	{P5_8,	FUNCTION_MODE5,	PIN_OUTPUT},	// Pin Function : CS2#
};
static const st_port_init_config_t GPIO_SC_INIT_bsc = { 0,	38,	&GPIO_SC_TABLE_bsc[0]	};
static st_port_config_t GPIO_SC_TABLE_dmac[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_dmac = { 0,	0,	&GPIO_SC_TABLE_dmac[0]	};
static st_port_config_t GPIO_SC_TABLE_mtucommon[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_mtucommon = { 0,	0,	&GPIO_SC_TABLE_mtucommon[0]	};
static st_port_config_t GPIO_SC_TABLE_mtu0[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_mtu0 = { 0,	0,	&GPIO_SC_TABLE_mtu0[0]	};
static st_port_config_t GPIO_SC_TABLE_mtu1[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_mtu1 = { 0,	0,	&GPIO_SC_TABLE_mtu1[0]	};
static st_port_config_t GPIO_SC_TABLE_mtu2[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_mtu2 = { 0,	0,	&GPIO_SC_TABLE_mtu2[0]	};
static st_port_config_t GPIO_SC_TABLE_mtu3[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_mtu3 = { 0,	0,	&GPIO_SC_TABLE_mtu3[0]	};
static st_port_config_t GPIO_SC_TABLE_mtu4[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_mtu4 = { 0,	0,	&GPIO_SC_TABLE_mtu4[0]	};
static st_port_config_t GPIO_SC_TABLE_rtc[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_rtc = { 0,	0,	&GPIO_SC_TABLE_rtc[0]	};
static st_port_config_t GPIO_SC_TABLE_scif0[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_scif0 = { 0,	0,	&GPIO_SC_TABLE_scif0[0]	};
static st_port_config_t GPIO_SC_TABLE_scif1[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_scif1 = { 0,	0,	&GPIO_SC_TABLE_scif1[0]	};
static st_port_config_t GPIO_SC_TABLE_scif2[] =
{
#if 0 // 2022.04.27 modify
	{P3_2,	FUNCTION_MODE3,	PIN_INPUT},	    // Pin Function : RxD2
	{P3_0,	FUNCTION_MODE6,	PIN_OUTPUT},	// Pin Function : TxD2
#else
	{P3_2,	FUNCTION_MODE4,	PIN_INPUT},	    // Pin Function : RxD2
	{P3_0,	FUNCTION_MODE6,	PIN_OUTPUT},	// Pin Function : TxD2
#endif
};
static const st_port_init_config_t GPIO_SC_INIT_scif2 = { 0,	2,	&GPIO_SC_TABLE_scif2[0]	};
static st_port_config_t GPIO_SC_TABLE_scif3[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_scif3 = { 0,	0,	&GPIO_SC_TABLE_scif3[0]	};
static st_port_config_t GPIO_SC_TABLE_scif4[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_scif4 = { 0,	0,	&GPIO_SC_TABLE_scif4[0]	};
static st_port_config_t GPIO_SC_TABLE_scif5[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_scif5 = { 0,	0,	&GPIO_SC_TABLE_scif5[0]	};
static st_port_config_t GPIO_SC_TABLE_scif6[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_scif6 = { 0,	0,	&GPIO_SC_TABLE_scif6[0]	};
static st_port_config_t GPIO_SC_TABLE_scif7[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_scif7 = { 0,	0,	&GPIO_SC_TABLE_scif7[0]	};
static st_port_config_t GPIO_SC_TABLE_sci0[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_sci0 = { 0,	0,	&GPIO_SC_TABLE_sci0[0]	};
static st_port_config_t GPIO_SC_TABLE_sci1[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_sci1 = { 0,	0,	&GPIO_SC_TABLE_sci1[0]	};
static st_port_config_t GPIO_SC_TABLE_rspi0[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_rspi0 = { 0,	0,	&GPIO_SC_TABLE_rspi0[0]	};
static st_port_config_t GPIO_SC_TABLE_rspi1[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_rspi1 = { 0,	0,	&GPIO_SC_TABLE_rspi1[0]	};
static st_port_config_t GPIO_SC_TABLE_rspi2[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_rspi2 = { 0,	0,	&GPIO_SC_TABLE_rspi2[0]	};
static st_port_config_t GPIO_SC_TABLE_rspi3[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_rspi3 = { 0,	0,	&GPIO_SC_TABLE_rspi3[0]	};
static st_port_config_t GPIO_SC_TABLE_rspi4[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_rspi4 = { 0,	0,	&GPIO_SC_TABLE_rspi4[0]	};
static st_port_config_t GPIO_SC_TABLE_spibsc0[] =
{
	{P9_5,	FUNCTION_MODE2,	PIN_IO},	// Pin Function : SPBIO10_0
	{P2_13,	FUNCTION_MODE4,	PIN_IO},	// Pin Function : SPBIO11_0
	{P9_4,	FUNCTION_MODE2,	PIN_IO},	// Pin Function : SPBIO00_0
	{P2_12,	FUNCTION_MODE4,	PIN_IO},	// Pin Function : SPBIO01_0
	{P9_7,	FUNCTION_MODE2,	PIN_IO},	// Pin Function : SPBIO30_0
	{P2_15,	FUNCTION_MODE4,	PIN_IO},	// Pin Function : SPBIO31_0
	{P9_6,	FUNCTION_MODE2,	PIN_IO},	// Pin Function : SPBIO20_0
	{P2_14,	FUNCTION_MODE4,	PIN_IO},	// Pin Function : SPBIO21_0
	{P9_3,	FUNCTION_MODE2,	PIN_OUTPUT},	// Pin Function : SPBSSL_0
	{P9_2,	FUNCTION_MODE2,	PIN_OUTPUT},	// Pin Function : SPBCLK_0
};
static const st_port_init_config_t GPIO_SC_INIT_spibsc0 = { 0,	10,	&GPIO_SC_TABLE_spibsc0[0]	};
static st_port_config_t GPIO_SC_TABLE_spibsc1[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_spibsc1 = { 0,	0,	&GPIO_SC_TABLE_spibsc1[0]	};
static st_port_config_t GPIO_SC_TABLE_riic0[] =
{
#if 0 // 2022.04.27modify
	{P1_1,	FUNCTION_MODE1,	PIN_IO},	// Pin Function : RIIC0SDA
	{P1_0,	FUNCTION_MODE1,	PIN_IO},	// Pin Function : RIIC0SCL
#else
	{P1_1,	FUNCTION_MODE1,	PIN_BUF_ALT},	// Pin Function : RIIC0SDA
	{P1_0,	FUNCTION_MODE1,	PIN_BUF_ALT},	// Pin Function : RIIC0SCL
#endif
};
static const st_port_init_config_t GPIO_SC_INIT_riic0 = { 0,	2,	&GPIO_SC_TABLE_riic0[0]	};
static st_port_config_t GPIO_SC_TABLE_riic1[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_riic1 = { 0,	0,	&GPIO_SC_TABLE_riic1[0]	};
static st_port_config_t GPIO_SC_TABLE_riic2[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_riic2 = { 0,	0,	&GPIO_SC_TABLE_riic2[0]	};
static st_port_config_t GPIO_SC_TABLE_riic3[] =
{
	{P1_7,	FUNCTION_MODE1,	PIN_BUF_ALT},	// Pin Function : RIIC3SDA
	{P1_6,	FUNCTION_MODE1,	PIN_BUF_ALT},	// Pin Function : RIIC3SCL
};
static const st_port_init_config_t GPIO_SC_INIT_riic3 = { 0,	2,	&GPIO_SC_TABLE_riic3[0]	};
static st_port_config_t GPIO_SC_TABLE_ssi0[] =
{
	{P4_5,	FUNCTION_MODE5,	PIN_ALT},	// Pin Function : SSIWS0
	{P4_4,	FUNCTION_MODE5,	PIN_ALT},	// Pin Function : SSISCK0
	{P4_6,	FUNCTION_MODE5,	PIN_ALT},	// Pin Function : SSIRxD0
	{P4_7,	FUNCTION_MODE5,	PIN_ALT},	// Pin Function : SSITxD0
};
static const st_port_init_config_t GPIO_SC_INIT_ssi0 = { 0,	4,	&GPIO_SC_TABLE_ssi0[0]	};
static st_port_config_t GPIO_SC_TABLE_ssi1[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_ssi1 = { 0,	0,	&GPIO_SC_TABLE_ssi1[0]	};
static st_port_config_t GPIO_SC_TABLE_ssi2[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_ssi2 = { 0,	0,	&GPIO_SC_TABLE_ssi2[0]	};
static st_port_config_t GPIO_SC_TABLE_ssi3[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_ssi3 = { 0,	0,	&GPIO_SC_TABLE_ssi3[0]	};
static st_port_config_t GPIO_SC_TABLE_ssi4[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_ssi4 = { 0,	0,	&GPIO_SC_TABLE_ssi4[0]	};
static st_port_config_t GPIO_SC_TABLE_ssi5[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_ssi5 = { 0,	0,	&GPIO_SC_TABLE_ssi5[0]	};
static st_port_config_t GPIO_SC_TABLE_mlb[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_mlb = { 0,	0,	&GPIO_SC_TABLE_mlb[0]	};
static st_port_config_t GPIO_SC_TABLE_cancommon[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_cancommon = { 0,	0,	&GPIO_SC_TABLE_cancommon[0]	};
static st_port_config_t GPIO_SC_TABLE_can0[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_can0 = { 0,	0,	&GPIO_SC_TABLE_can0[0]	};
static st_port_config_t GPIO_SC_TABLE_can1[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_can1 = { 0,	0,	&GPIO_SC_TABLE_can1[0]	};
static st_port_config_t GPIO_SC_TABLE_can2[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_can2 = { 0,	0,	&GPIO_SC_TABLE_can2[0]	};
static st_port_config_t GPIO_SC_TABLE_can3[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_can3 = { 0,	0,	&GPIO_SC_TABLE_can3[0]	};
static st_port_config_t GPIO_SC_TABLE_can4[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_can4 = { 0,	0,	&GPIO_SC_TABLE_can4[0]	};
static st_port_config_t GPIO_SC_TABLE_iebus[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_iebus = { 0,	0,	&GPIO_SC_TABLE_iebus[0]	};
static st_port_config_t GPIO_SC_TABLE_spdif[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_spdif = { 0,	0,	&GPIO_SC_TABLE_spdif[0]	};
static st_port_config_t GPIO_SC_TABLE_rlin30[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_rlin30 = { 0,	0,	&GPIO_SC_TABLE_rlin30[0]	};
static st_port_config_t GPIO_SC_TABLE_rlin31[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_rlin31 = { 0,	0,	&GPIO_SC_TABLE_rlin31[0]	};
static st_port_config_t GPIO_SC_TABLE_ether0[] =
{
	{P2_10,	FUNCTION_MODE2,	PIN_INPUT},	// Pin Function : ET_RXD2
	{P2_11,	FUNCTION_MODE2,	PIN_INPUT},	// Pin Function : ET_RXD3
	{P2_8,	FUNCTION_MODE2,	PIN_INPUT},	// Pin Function : ET_RXD0
	{P2_9,	FUNCTION_MODE2,	PIN_INPUT},	// Pin Function : ET_RXD1
	{P3_6,	FUNCTION_MODE2,	PIN_INPUT},	// Pin Function : ET_RXDV
	{P3_3,	FUNCTION_MODE2,	PIN_IO},	// Pin Function : ET_MDIO
	{P2_3,	FUNCTION_MODE2,	PIN_INPUT},	// Pin Function : ET_CRS
	{P2_1,	FUNCTION_MODE2,	PIN_OUTPUT},	// Pin Function : ET_TXER
	{P2_7,	FUNCTION_MODE2,	PIN_OUTPUT},	// Pin Function : ET_TXD3
	{P2_6,	FUNCTION_MODE2,	PIN_OUTPUT},	// Pin Function : ET_TXD2
	{P2_5,	FUNCTION_MODE2,	PIN_OUTPUT},	// Pin Function : ET_TXD1
	{P2_4,	FUNCTION_MODE2,	PIN_OUTPUT},	// Pin Function : ET_TXD0
	{P2_2,	FUNCTION_MODE2,	PIN_OUTPUT},	// Pin Function : ET_TXEN
	{P3_5,	FUNCTION_MODE2,	PIN_INPUT},	// Pin Function : ET_RXER
	{P2_0,	FUNCTION_MODE2,	PIN_INPUT},	// Pin Function : ET_TXCLK
	{P3_4,	FUNCTION_MODE2,	PIN_INPUT},	// Pin Function : ET_RXCLK
};
static const st_port_init_config_t GPIO_SC_INIT_ether0 = { 0,	16,	&GPIO_SC_TABLE_ether0[0]	};
static st_port_config_t GPIO_SC_TABLE_nand[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_nand = { 0,	0,	&GPIO_SC_TABLE_nand[0]	};
static st_port_config_t GPIO_SC_TABLE_dvdcommon[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_dvdcommon = { 0,	0,	&GPIO_SC_TABLE_dvdcommon[0]	};
static st_port_config_t GPIO_SC_TABLE_dvd0[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_dvd0 = { 0,	0,	&GPIO_SC_TABLE_dvd0[0]	};
static st_port_config_t GPIO_SC_TABLE_dvd1[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_dvd1 = { 0,	0,	&GPIO_SC_TABLE_dvd1[0]	};
static st_port_config_t GPIO_SC_TABLE_vdc50[] =
{
	{P11_0,		FUNCTION_MODE5,	PIN_ALT},	// Pin Function : LCD0_DATA7
	{P11_1,		FUNCTION_MODE5,	PIN_ALT},	// Pin Function : LCD0_DATA6
	{P11_2,		FUNCTION_MODE5,	PIN_ALT},	// Pin Function : LCD0_DATA5
	{P11_3,		FUNCTION_MODE5,	PIN_ALT},	// Pin Function : LCD0_DATA4
	{P11_4,		FUNCTION_MODE5,	PIN_ALT},	// Pin Function : LCD0_DATA3
	{P11_5,		FUNCTION_MODE5,	PIN_ALT},	// Pin Function : LCD0_DATA2
	{P11_6,		FUNCTION_MODE5,	PIN_ALT},	// Pin Function : LCD0_DATA1
	{P11_7,		FUNCTION_MODE5,	PIN_ALT},	// Pin Function : LCD0_DATA0
	/* {P11_8,		FUNCTION_MODE5,	PIN_ALT},	// Pin Function : LCD0_TCON6 */
	/* {P11_9,		FUNCTION_MODE5,	PIN_ALT},	// Pin Function : LCD0_TCON5 */
	{P11_10,	FUNCTION_MODE5,	PIN_ALT},	// Pin Function : LCD0_TCON4
	{P11_11,	FUNCTION_MODE5,	PIN_ALT},	// Pin Function : LCD0_TCON3
	{P11_12,	FUNCTION_MODE5,	PIN_ALT},	// Pin Function : LCD0_TCON2
	/* {P11_13,	FUNCTION_MODE5,	PIN_ALT},	// Pin Function : LCD0_TCON1 */
	/* {P11_14,	FUNCTION_MODE5,	PIN_ALT},	// Pin Function : LCD0_TCON0 */
	{P11_15,	FUNCTION_MODE5,	PIN_ALT},	// Pin Function : LCD0_CLK

	{P10_15,	FUNCTION_MODE5,	PIN_ALT},	// Pin Function : LCD0_DATA8
	{P10_14,	FUNCTION_MODE5,	PIN_ALT},	// Pin Function : LCD0_DATA9
	{P10_13,	FUNCTION_MODE5,	PIN_ALT},	// Pin Function : LCD0_DATA10
	{P10_12,	FUNCTION_MODE5,	PIN_ALT},	// Pin Function : LCD0_DATA11
	{P10_11,	FUNCTION_MODE5,	PIN_ALT},	// Pin Function : LCD0_DATA12
	{P10_10,	FUNCTION_MODE5,	PIN_ALT},	// Pin Function : LCD0_DATA13
	{P10_9,		FUNCTION_MODE5,	PIN_ALT},	// Pin Function : LCD0_DATA14
	{P10_8,		FUNCTION_MODE5,	PIN_ALT},	// Pin Function : LCD0_DATA15
	{P10_7,		FUNCTION_MODE5,	PIN_ALT},	// Pin Function : LCD0_DATA16
	{P10_6,		FUNCTION_MODE5,	PIN_ALT},	// Pin Function : LCD0_DATA17
	{P10_5,		FUNCTION_MODE5,	PIN_ALT},	// Pin Function : LCD0_DATA18
	{P10_4,		FUNCTION_MODE5,	PIN_ALT},	// Pin Function : LCD0_DATA19
	{P10_3,		FUNCTION_MODE5,	PIN_ALT},	// Pin Function : LCD0_DATA20
	{P10_2,		FUNCTION_MODE5,	PIN_ALT},	// Pin Function : LCD0_DATA21
	{P10_1,		FUNCTION_MODE5,	PIN_ALT},	// Pin Function : LCD0_DATA22
	{P10_0,		FUNCTION_MODE5,	PIN_ALT},	// Pin Function : LCD0_DATA23

};
static const st_port_init_config_t GPIO_SC_INIT_vdc50 = { 0,	28,	&GPIO_SC_TABLE_vdc50[0]	};
static st_port_config_t GPIO_SC_TABLE_vdc51[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_vdc51 = { 0,	0,	&GPIO_SC_TABLE_vdc51[0]	};
static st_port_config_t GPIO_SC_TABLE_lvds[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_lvds = { 0,	0,	&GPIO_SC_TABLE_lvds[0]	};
static st_port_config_t GPIO_SC_TABLE_ceu[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_ceu = { 0,	0,	&GPIO_SC_TABLE_ceu[0]	};
static st_port_config_t GPIO_SC_TABLE_sg0[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_sg0 = { 0,	0,	&GPIO_SC_TABLE_sg0[0]	};
static st_port_config_t GPIO_SC_TABLE_sg1[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_sg1 = { 0,	0,	&GPIO_SC_TABLE_sg1[0]	};
static st_port_config_t GPIO_SC_TABLE_sg2[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_sg2 = { 0,	0,	&GPIO_SC_TABLE_sg2[0]	};
static st_port_config_t GPIO_SC_TABLE_sg3[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_sg3 = { 0,	0,	&GPIO_SC_TABLE_sg3[0]	};
static st_port_config_t GPIO_SC_TABLE_sdhi0[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_sdhi0 = { 0,	0,	&GPIO_SC_TABLE_sdhi0[0]	};
static st_port_config_t GPIO_SC_TABLE_sdhi1[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_sdhi1 = { 0,	0,	&GPIO_SC_TABLE_sdhi1[0]	};
static st_port_config_t GPIO_SC_TABLE_mmchi[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_mmchi = { 0,	0,	&GPIO_SC_TABLE_mmchi[0]	};
static st_port_config_t GPIO_SC_TABLE_pwm1[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_pwm1 = { 0,	0,	&GPIO_SC_TABLE_pwm1[0]	};
static st_port_config_t GPIO_SC_TABLE_pwm2[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_pwm2 = { 0,	0,	&GPIO_SC_TABLE_pwm2[0]	};
static st_port_config_t GPIO_SC_TABLE_adc[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_adc = { 0,	0,	&GPIO_SC_TABLE_adc[0]	};

#endif  /* _R_GPIO_RZA1H_CFG_H_ */
