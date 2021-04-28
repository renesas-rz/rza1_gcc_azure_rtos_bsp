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

#ifndef _R_GPIO_CFG_H_
#define _R_GPIO_CFG_H_

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
#endif  /* _R_GPIO_CFG_H_ */
static const st_port_config_t GPIO_SC_TABLE_scf0[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_scf0 = { 0,	0,	&GPIO_SC_TABLE_scf0[0]	};
static const st_port_config_t GPIO_SC_TABLE_scf1[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_scf1 = { 0,	0,	&GPIO_SC_TABLE_scf1[0]	};
static const st_port_config_t GPIO_SC_TABLE_scf2[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_scf2 = { 0,	0,	&GPIO_SC_TABLE_scf2[0]	};
static const st_port_config_t GPIO_SC_TABLE_scf3[] =
{
	{P7_10,	FUNCTION_MODE5,	PIN_INPUT},	// Pin Function : RxD3
	{P7_11,	FUNCTION_MODE5,	PIN_OUTPUT},	// Pin Function : TxD3
};
static const st_port_init_config_t GPIO_SC_INIT_scf3 = { 0,	2,	&GPIO_SC_TABLE_scf3[0]	};
static const st_port_config_t GPIO_SC_TABLE_scf4[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_scf4 = { 0,	0,	&GPIO_SC_TABLE_scf4[0]	};
static const st_port_config_t GPIO_SC_TABLE_sci0[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_sci0 = { 0,	0,	&GPIO_SC_TABLE_sci0[0]	};
static const st_port_config_t GPIO_SC_TABLE_sci1[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_sci1 = { 0,	0,	&GPIO_SC_TABLE_sci1[0]	};
static const st_port_config_t GPIO_SC_TABLE_rspi0[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_rspi0 = { 0,	0,	&GPIO_SC_TABLE_rspi0[0]	};
static const st_port_config_t GPIO_SC_TABLE_rspi1[] =
{
	{P6_12,	FUNCTION_MODE3,	PIN_IO},	// Pin Function : RSPCK1
	{P6_13,	FUNCTION_MODE3,	PIN_IO},	// Pin Function : SSL10
	{P6_15,	FUNCTION_MODE3,	PIN_IO},	// Pin Function : MISO1
	{P6_14,	FUNCTION_MODE3,	PIN_IO},	// Pin Function : MOSI1
};
static const st_port_init_config_t GPIO_SC_INIT_rspi1 = { 0,	4,	&GPIO_SC_TABLE_rspi1[0]	};
static const st_port_config_t GPIO_SC_TABLE_rspi2[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_rspi2 = { 0,	0,	&GPIO_SC_TABLE_rspi2[0]	};
static const st_port_config_t GPIO_SC_TABLE_rspb0[] =
{
	{P4_7,	FUNCTION_MODE2,	PIN_IO},	// Pin Function : SPBIO10_0
	{P3_11,	FUNCTION_MODE2,	PIN_IO},	// Pin Function : SPBIO11_0
	{P4_6,	FUNCTION_MODE2,	PIN_IO},	// Pin Function : SPBIO00_0
	{P3_10,	FUNCTION_MODE2,	PIN_IO},	// Pin Function : SPBIO01_0
	{P4_3,	FUNCTION_MODE2,	PIN_IO},	// Pin Function : SPBIO30_0
	{P3_13,	FUNCTION_MODE2,	PIN_IO},	// Pin Function : SPBIO31_0
	{P4_2,	FUNCTION_MODE2,	PIN_IO},	// Pin Function : SPBIO20_0
	{P3_12,	FUNCTION_MODE2,	PIN_IO},	// Pin Function : SPBIO21_0
	{P4_5,	FUNCTION_MODE2,	PIN_OUTPUT},	// Pin Function : SPBSSL_0
	{P4_4,	FUNCTION_MODE2,	PIN_OUTPUT},	// Pin Function : SPBCLK_0
};
static const st_port_init_config_t GPIO_SC_INIT_rspb0 = { 0,	10,	&GPIO_SC_TABLE_rspb0[0]	};
static const st_port_config_t GPIO_SC_TABLE_i2c0[] =
{
	{P1_1,	FUNCTION_MODE1,	PIN_IO},	// Pin Function : RIIC0SDA
	{P1_0,	FUNCTION_MODE1,	PIN_IO},	// Pin Function : RIIC0SCL
};
static const st_port_init_config_t GPIO_SC_INIT_i2c0 = { 0,	2,	&GPIO_SC_TABLE_i2c0[0]	};
static const st_port_config_t GPIO_SC_TABLE_i2c1[] =
{
	{P1_2,	FUNCTION_MODE1,	PIN_IO},	// Pin Function : RIIC1SCL
	{P1_3,	FUNCTION_MODE1,	PIN_IO},	// Pin Function : RIIC1SDA
};
static const st_port_init_config_t GPIO_SC_INIT_i2c1 = { 0,	2,	&GPIO_SC_TABLE_i2c1[0]	};
static const st_port_config_t GPIO_SC_TABLE_i2c2[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_i2c2 = { 0,	0,	&GPIO_SC_TABLE_i2c2[0]	};
static const st_port_config_t GPIO_SC_TABLE_i2c3[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_i2c3 = { 0,	0,	&GPIO_SC_TABLE_i2c3[0]	};
static const st_port_config_t GPIO_SC_TABLE_ssi0[] =
{
	{P6_9,	FUNCTION_MODE3,	PIN_IO},	// Pin Function : SSIWS0
	{P6_8,	FUNCTION_MODE3,	PIN_IO},	// Pin Function : SSISCK0
	{P6_11,	FUNCTION_MODE3,	PIN_INPUT},	// Pin Function : SSIRxD0
	{P6_10,	FUNCTION_MODE3,	PIN_OUTPUT},	// Pin Function : SSITxD0
};
static const st_port_init_config_t GPIO_SC_INIT_ssi0 = { 0,	4,	&GPIO_SC_TABLE_ssi0[0]	};
static const st_port_config_t GPIO_SC_TABLE_ssi1[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_ssi1 = { 0,	0,	&GPIO_SC_TABLE_ssi1[0]	};
static const st_port_config_t GPIO_SC_TABLE_ssi2[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_ssi2 = { 0,	0,	&GPIO_SC_TABLE_ssi2[0]	};
static const st_port_config_t GPIO_SC_TABLE_ssi3[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_ssi3 = { 0,	0,	&GPIO_SC_TABLE_ssi3[0]	};
static const st_port_config_t GPIO_SC_TABLE_irq[] =
{
	{P5_8,	FUNCTION_MODE4,	PIN_INPUT},	// Pin Function : IRQ3
};
static const st_port_init_config_t GPIO_SC_INIT_irq = { 0,	1,	&GPIO_SC_TABLE_irq[0]	};
static const st_port_config_t GPIO_SC_TABLE_mlb[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_mlb = { 0,	0,	&GPIO_SC_TABLE_mlb[0]	};
static const st_port_config_t GPIO_SC_TABLE_can0[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_can0 = { 0,	0,	&GPIO_SC_TABLE_can0[0]	};
static const st_port_config_t GPIO_SC_TABLE_iebus[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_iebus = { 0,	0,	&GPIO_SC_TABLE_iebus[0]	};
static const st_port_config_t GPIO_SC_TABLE_spdif[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_spdif = { 0,	0,	&GPIO_SC_TABLE_spdif[0]	};
static const st_port_config_t GPIO_SC_TABLE_lin[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_lin = { 0,	0,	&GPIO_SC_TABLE_lin[0]	};
static const st_port_config_t GPIO_SC_TABLE_et[] =
{
	{P8_9,	FUNCTION_MODE2,	PIN_INPUT},	// Pin Function : ET_RXD2
	{P8_10,	FUNCTION_MODE2,	PIN_INPUT},	// Pin Function : ET_RXD3
	{P8_7,	FUNCTION_MODE2,	PIN_INPUT},	// Pin Function : ET_RXD0
	{P8_8,	FUNCTION_MODE2,	PIN_INPUT},	// Pin Function : ET_RXD1
	{P3_5,	FUNCTION_MODE4,	PIN_INPUT},	// Pin Function : ET_RXDV
	{P9_1,	FUNCTION_MODE2,	PIN_IO},	// Pin Function : ET_MDIO
	{P8_15,	FUNCTION_MODE2,	PIN_INPUT},	// Pin Function : ET_CRS
	{P3_1,	FUNCTION_MODE4,	PIN_OUTPUT},	// Pin Function : ET_TXER
	{P5_3,	FUNCTION_MODE3,	PIN_OUTPUT},	// Pin Function : ET_TXD3
	{P8_2,	FUNCTION_MODE2,	PIN_OUTPUT},	// Pin Function : ET_TXD2
	{P8_1,	FUNCTION_MODE2,	PIN_OUTPUT},	// Pin Function : ET_TXD1
	{P8_0,	FUNCTION_MODE2,	PIN_OUTPUT},	// Pin Function : ET_TXD0
	{P8_6,	FUNCTION_MODE2,	PIN_OUTPUT},	// Pin Function : ET_TXEN
	{P3_4,	FUNCTION_MODE4,	PIN_INPUT},	// Pin Function : ET_RXER
	{P3_0,	FUNCTION_MODE4,	PIN_INPUT},	// Pin Function : ET_TXCLK
	{P8_14,	FUNCTION_MODE2,	PIN_INPUT},	// Pin Function : ET_COL
	{P3_3,	FUNCTION_MODE4,	PIN_INPUT},	// Pin Function : ET_RXCLK
	{P9_0,	FUNCTION_MODE2,	PIN_OUTPUT},	// Pin Function : ET_MDC
};
static const st_port_init_config_t GPIO_SC_INIT_et = { 0,	18,	&GPIO_SC_TABLE_et[0]	};
static const st_port_config_t GPIO_SC_TABLE_an[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_an = { 0,	0,	&GPIO_SC_TABLE_an[0]	};
static const st_port_config_t GPIO_SC_TABLE_usb0[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_usb0 = { 0,	0,	&GPIO_SC_TABLE_usb0[0]	};
static const st_port_config_t GPIO_SC_TABLE_usb1[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_usb1 = { 0,	0,	&GPIO_SC_TABLE_usb1[0]	};
static const st_port_config_t GPIO_SC_TABLE_vdc[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_vdc = { 0,	0,	&GPIO_SC_TABLE_vdc[0]	};
static const st_port_config_t GPIO_SC_TABLE_ceu[] =
{
	{P3_14,	FUNCTION_MODE2,	PIN_INPUT},	// Pin Function : VIO_CLK
	{P1_4,	FUNCTION_MODE4,	PIN_INPUT},	// Pin Function : VIO_D0
	{P1_7,	FUNCTION_MODE4,	PIN_INPUT},	// Pin Function : VIO_D3
	{P1_12,	FUNCTION_MODE4,	PIN_INPUT},	// Pin Function : VIO_D4
	{P1_5,	FUNCTION_MODE4,	PIN_INPUT},	// Pin Function : VIO_D1
	{P1_6,	FUNCTION_MODE4,	PIN_INPUT},	// Pin Function : VIO_D2
	{P1_15,	FUNCTION_MODE4,	PIN_INPUT},	// Pin Function : VIO_D7
	{P1_13,	FUNCTION_MODE4,	PIN_INPUT},	// Pin Function : VIO_D5
	{P1_14,	FUNCTION_MODE4,	PIN_INPUT},	// Pin Function : VIO_D6
	{P4_1,	FUNCTION_MODE2,	PIN_INPUT},	// Pin Function : VIO_HD
	{P4_0,	FUNCTION_MODE2,	PIN_INPUT},	// Pin Function : VIO_VD
};
static const st_port_init_config_t GPIO_SC_INIT_ceu = { 0,	11,	&GPIO_SC_TABLE_ceu[0]	};
static const st_port_config_t GPIO_SC_TABLE_sd0[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_sd0 = { 0,	0,	&GPIO_SC_TABLE_sd0[0]	};
static const st_port_config_t GPIO_SC_TABLE_sd1[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_sd1 = { 0,	0,	&GPIO_SC_TABLE_sd1[0]	};
static const st_port_config_t GPIO_SC_TABLE_mmc[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_mmc = { 0,	0,	&GPIO_SC_TABLE_mmc[0]	};
static const st_port_config_t GPIO_SC_TABLE_bsc[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_bsc = { 0,	0,	&GPIO_SC_TABLE_bsc[0]	};

/* End of File */