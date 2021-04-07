/*******************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only
 * intended for use with Renesas products. No other uses are authorized. This
 * software is owned by Renesas Electronics Corporation and is protected under
 * all applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
 * LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
 * TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
 * ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
 * FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
 * ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
 * BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software
 * and to discontinue the availability of this software. By using this software,
 * you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 * Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
 *******************************************************************************/
/**************************************************************************//**
 * File Name :    vdc5_portsetting.c
 * @file          vdc5_portsetting.c
 * $Rev:          1.0
 * $Date:         24.04.2019
 * @brief         VDC5 port setting functions
 ******************************************************************************/

/*******************************************************************************
 Includes <System Includes>, "Project Includes"
 *******************************************************************************/
#include "mcu_board_select.h"
#include "r_typedefs.h"
#include "iodefine_cfg.h"
#include "cpg_iobitmask.h"
#include "r_vdc_portsetting.h"
#include "rza_io_regrw.h"

#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
/* nothing */
#elif (TARGET_BOARD == TARGET_BOARD_RSK)
#include "riic_cat9554_if.h"
#endif /* TARGET_BOARD */

/******************************************************************************
 Macro definitions
 ******************************************************************************/
/* LCD Port define */
#if (TARGET_BOARD == TARGET_BOARD_RSK)
#define LCD_PORT11_5TH (0x9CFFu)
#define LCD_PORT10_5TH (0xFFFFu)

#elif (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
#define LCD_PORT6_2TH (0x00FFu)
#define LCD_PORT7_6TH (0x0010u)
#define LCD_PORT8_1TH (0x1CFFu)
#endif

/* CMOS CAMERA Port define */
#if (TARGET_BOARD == TARGET_BOARD_RSK)

/* Not support */

#elif (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)

/* Not support */

#endif

/******************************************************************************
 Enumerated Types
 ******************************************************************************/

/******************************************************************************
 Structures
 ******************************************************************************/

/******************************************************************************
 Global Vaiables
 ******************************************************************************/

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/

/******************************************************************************
 Function prototypes
 ******************************************************************************/

/**************************************************************************//**
 * Function Name : VDC5_LcdPortSetting
 * @brief       LCD panel I/O port setup (VDC5 channel 0)
 * @param[in]   param     :
 * @retval      None
 ******************************************************************************/
void VDC_LcdPortSetting(uint32_t param)
{
    UNUSED_PARAM(param);
    volatile uint32_t dummy_read;
    uint32_t reg_data;

#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
/* nothing */
#elif (TARGET_BOARD == TARGET_BOARD_RSK)
	uint8_t px_addr, px_data, px_config;
#endif /* TARGET_BOARD */

#if (TARGET_BOARD == TARGET_BOARD_RSK)
    /* RSK TFT (RGB888)
     Panel clock : LCD0_CLK              ... P11_15, 5th alternative function
     HSYNC       : LCD0_TCON2            ... P11_12, 5th alternative function
     VSYNC       : LCD0_TCON3            ... P11_11, 5th alternative function
     DE          : LCD0_TCON4            ... P11_10, 5th alternative function
     LCD0_DATA
     R[7:0]      : LCD0_DATA[23:16]      ... P10_0 ~ P10_7,  5th alternative function
     G[7:0]      : LCD0_DATA[15:8]       ... P10_8 ~ P10_15, 5th alternative function
     B[7:0]      : LCD0_DATA[7:0]        ... P11_0 ~ P11_7,  5th alternative function
     */

    /* Port 11 */
    reg_data = ((uint32_t) GPIO.PMC11 & (uint32_t) ~LCD_PORT11_5TH);
    GPIO.PMC11 = (uint16_t) reg_data;
    reg_data = (uint32_t) GPIO.PMC11;
    dummy_read = reg_data;

	/* Port Expander 2 setting */
    /* PX1 EN0: Low = LCD */
	R_RIIC_CAT9554_Open();
    px_addr = CAT9554_I2C_PX2;
	px_data = (~PX2_PX1_EN0);
    px_config = PX2_PX1_EN0;
	R_RIIC_CAT9554_Write(px_addr, px_data, px_config);
	R_RIIC_CAT9554_Close();

    /* PFCAE11, PFCE11, PFC11 ... 5th alternative function
     PIPC11, PMC11
     b15:b13  : P11_15 ~ P11_13
     b7:b0    : P11_7 ~ P11_0 */
    reg_data = ((uint32_t) GPIO.PFCAE11 | (uint32_t)LCD_PORT11_5TH);
    GPIO.PFCAE11 = (uint16_t) reg_data;
    reg_data = ((uint32_t) GPIO.PFCE11 & (uint32_t) ~LCD_PORT11_5TH);
    GPIO.PFCE11 = (uint16_t) reg_data;
    reg_data = ((uint32_t) GPIO.PFC11 & (uint32_t) ~LCD_PORT11_5TH);
    GPIO.PFC11 = (uint16_t) reg_data;
    reg_data = ((uint32_t) GPIO.PIPC11 | (uint32_t)LCD_PORT11_5TH);
    GPIO.PIPC11 = (uint16_t) reg_data;
    reg_data = ((uint32_t) GPIO.PMC11 | (uint32_t)LCD_PORT11_5TH);
    GPIO.PMC11 = (uint16_t) reg_data;

    /* Port 10 */
    reg_data = ((uint32_t) GPIO.PMC10 & (uint32_t) ~LCD_PORT10_5TH);
    GPIO.PMC10 = (uint16_t) reg_data;
    reg_data = (uint32_t) GPIO.PMC10;
    dummy_read = reg_data;

    /* PFCAE10, PFCE10, PFC10 ... 5th alternative function
     PIPC10, PMC10
     b15:b0   : P10_15 ~ P10_0 */
    reg_data = ((uint32_t) GPIO.PFCAE10 | (uint32_t)LCD_PORT10_5TH);
    GPIO.PFCAE10 = (uint16_t) reg_data;
    reg_data = ((uint32_t) GPIO.PFCE10 & (uint32_t) ~LCD_PORT10_5TH);
    GPIO.PFCE10 = (uint16_t) reg_data;
    reg_data = ((uint32_t) GPIO.PFC10 & (uint32_t) ~LCD_PORT10_5TH);
    GPIO.PFC10 = (uint16_t) reg_data;
    reg_data = ((uint32_t) GPIO.PIPC10 | (uint32_t)LCD_PORT10_5TH);
    GPIO.PIPC10 = (uint16_t) reg_data;
    reg_data = ((uint32_t) GPIO.PMC10 | (uint32_t)LCD_PORT10_5TH);
    GPIO.PMC10 = (uint16_t) reg_data;
    reg_data = GPIO.PMC10;

#elif (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
    /* Stream it! TFT 4.3 (RGB565)
     Panel clock : LCD0_CLK              ... P7_4,   6th alternative function
     DE          : LCD0_TCON2            ... P8_10,  1th alternative function
     HSYNC       : LCD0_TCON3            ... P8_12,  1th alternative function
     VSYNC       : LCD0_TCON4            ... P8_11,  1th alternative function

     LCD0_DATA   : LCD0_DATA[15:8]       ... P6_7 ~ P6_0,    2th alternative function
                 : LCD0_DATA[7:0]        ... P8_7 ~ P8_0,    1th alternative function
     */

    /* Port 6 */
    reg_data = ((uint32_t) GPIO.PMC6 & (uint32_t) ~LCD_PORT6_2TH);
    GPIO.PMC6 = (uint16_t) reg_data;
    reg_data = (uint32_t) GPIO.PMC6;
    dummy_read = reg_data;

    /* PFCAE6, PFCE6, PFC6 ... 2th alternative function
     PIPC6, PMC6 */
    reg_data = ((uint32_t) GPIO.PFCAE6 & (uint32_t)~LCD_PORT6_2TH);
    GPIO.PFCAE6 = (uint16_t) reg_data;
    reg_data = ((uint32_t) GPIO.PFCE6 & (uint32_t) ~LCD_PORT6_2TH);
    GPIO.PFCE6 = (uint16_t) reg_data;
    reg_data = ((uint32_t) GPIO.PFC6 | (uint32_t) LCD_PORT6_2TH);
    GPIO.PFC6 = (uint16_t) reg_data;
    reg_data = ((uint32_t) GPIO.PIPC6 | (uint32_t)LCD_PORT6_2TH);
    GPIO.PIPC6 = (uint16_t) reg_data;
    reg_data = ((uint32_t) GPIO.PMC6 | (uint32_t)LCD_PORT6_2TH);
    GPIO.PMC6 = (uint16_t) reg_data;

    /* Port 7 */
    reg_data = ((uint32_t) GPIO.PMC7 & (uint32_t) ~LCD_PORT7_6TH);
    GPIO.PMC7 = (uint16_t) reg_data;
    reg_data = (uint32_t) GPIO.PMC7;
    dummy_read = reg_data;

    /* PFCAE7, PFCE7, PFC7 ... 6th alternative function
     PIPC7, PMC7 */
    reg_data = ((uint32_t) GPIO.PFCAE7 | (uint32_t)LCD_PORT7_6TH);
    GPIO.PFCAE7 = (uint16_t) reg_data;
    reg_data = ((uint32_t) GPIO.PFCE7 & (uint32_t) ~LCD_PORT7_6TH);
    GPIO.PFCE7 = (uint16_t) reg_data;
    reg_data = ((uint32_t) GPIO.PFC7 | (uint32_t) LCD_PORT7_6TH);
    GPIO.PFC7 = (uint16_t) reg_data;
    reg_data = ((uint32_t) GPIO.PIPC7 | (uint32_t)LCD_PORT7_6TH);
    GPIO.PIPC7 = (uint16_t) reg_data;
    reg_data = ((uint32_t) GPIO.PMC7 | (uint32_t)LCD_PORT7_6TH);
    GPIO.PMC7 = (uint16_t) reg_data;
    reg_data = GPIO.PMC7;

    /* Port 8 */
    reg_data = ((uint32_t) GPIO.PMC8 & (uint32_t) ~LCD_PORT8_1TH);
    GPIO.PMC8 = (uint16_t) reg_data;
    reg_data = (uint32_t) GPIO.PMC8;
    dummy_read = reg_data;

    /* PFCAE8, PFCE8, PFC8 ... 1th alternative function
     PIPC8, PMC8 */
    reg_data = ((uint32_t) GPIO.PFCAE8 & (uint32_t) ~LCD_PORT8_1TH);
    GPIO.PFCAE8 = (uint16_t) reg_data;
    reg_data = ((uint32_t) GPIO.PFCE8 & (uint32_t) ~LCD_PORT8_1TH);
    GPIO.PFCE8 = (uint16_t) reg_data;
    reg_data = ((uint32_t) GPIO.PFC8 & (uint32_t) ~LCD_PORT8_1TH);
    GPIO.PFC8 = (uint16_t) reg_data;
    reg_data = ((uint32_t) GPIO.PIPC8 | (uint32_t)LCD_PORT8_1TH);
    GPIO.PIPC8 = (uint16_t) reg_data;
    reg_data = ((uint32_t) GPIO.PMC8 | (uint32_t)LCD_PORT8_1TH);
    GPIO.PMC8 = (uint16_t) reg_data;
    reg_data = GPIO.PMC8;

    /* Standby out I2C ch1 for QE coworking */
    /* ==== Module standby clear ==== */
    /* ---- Supply clock to the RIIC(channel 1) ---- */
    rza_io_reg_write_8((volatile uint8_t *) &CPG.STBCR9, 0,
    CPG_STBCR9_MSTP96_SHIFT, CPG_STBCR9_MSTP96);

    /* cast register address to volatile uint8_t pointer */
    dummy_read = rza_io_reg_read_8((volatile uint8_t *) &CPG.STBCR9,
    CPG_STBCR9_MSTP96_SHIFT, CPG_STBCR9_MSTP96);

    (void) dummy_read;

#endif
} /* End of function VDC5_LcdPortSetting() */

/**************************************************************************//**
 * Function Name : VDC5_CameraPortSetting
 * @brief       CMOS CAMERA I/O port setup (VDC5 channel 0)
 * @param[in]   param     :
 * @retval      None
 ******************************************************************************/
void VDC_CameraPortSetting(uint32_t param)
{
#if (TARGET_BOARD == TARGET_BOARD_RSK)
    uint32_t reg_data;
    volatile uint32_t dummy_read;
#else
    UNUSED_PARAM(param);

    /* Not support */
#endif
} /* End of function VDC5_CameraPortSetting() */

