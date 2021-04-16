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
 * Copyright (C) 2017 Renesas Electronics Corporation. All rights reserved.
 *******************************************************************************/
/**************************************************************************//**
 * File Name :    ceu_portsetting.c
 * @file          ceu_portsetting.c
 * $Rev:  $
 * $Date::  #$
 * @brief        CEU port setting functions
 ******************************************************************************/

/*******************************************************************************
 Includes <System Includes>, "Project Includes"
 *******************************************************************************/
#include     "mcu_board_select.h"
#include     "r_typedefs.h"
#include     "iodefine_cfg.h"

/******************************************************************************
 Macro definitions
 ******************************************************************************/
/* CEU Port define */
#if (TARGET_BOARD == TARGET_BOARD_RSK)

#elif (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
#define LCD_PORT3_2TH (0x4000u)
#define LCD_PORT4_2TH (0x0003u)
#define LCD_PORT1_4TH (0xF0F0u)

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
void CEU_PortSetting(uint32_t param);

/**************************************************************************//**
 * Function Name : CEU_PortSetting
 * @brief       LCD panel I/O port setup (VDC5 channel 0)
 * @param[in]   param     :
 * @retval      None
 ******************************************************************************/
void CEU_PortSetting(uint32_t param)
{
    UNUSED_PARAM(param);
    volatile uint32_t dummy_read;
    uint32_t reg_data;

#if (TARGET_BOARD == TARGET_BOARD_RSK)
    /* Not support */
#elif (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
    /* RSK TFT (RGB888)
     clock       : VIO_CLK     ... P3_14, 2th alternative function
     HSYNC       : VIO_HD      ... P4_1,  2th alternative function
     VSYNC       : VIO_VD      ... P4.0,  2th alternative function

     DATA7       : VIO_D7      ... P1_15, 4th alternative function
     DATA6       : VIO_D6      ... P1_14, 4th alternative function
     DATA5       : VIO_D5      ... P1_13, 4th alternative function
     DATA4       : VIO_D4      ... P1_12, 4th alternative function
     DATA3       : VIO_D3      ... P1_7,  4th alternative function
     DATA2       : VIO_D2      ... P1_6,  4th alternative function
     DATA1       : VIO_D1      ... P1_5,  4th alternative function
     DATA0       : VIO_D0      ... P1_4,  4th alternative function
     */

    /* Port 3 */
    reg_data = ((uint32_t) GPIO.PMC3 & (uint32_t) ~LCD_PORT3_2TH);
    GPIO.PMC3 = (uint16_t) reg_data;
    reg_data = (uint32_t) GPIO.PMC3;
    dummy_read = reg_data;

    /* PFCAE3, PFCE3, PFC3 ... 2th alternative function
     PIPC3, PMC3 */
    reg_data = ((uint32_t) GPIO.PFCAE3 & (uint32_t) ~LCD_PORT3_2TH);
    GPIO.PFCAE3 = (uint16_t) reg_data;
    reg_data = ((uint32_t) GPIO.PFCE3 & (uint32_t) ~LCD_PORT3_2TH);
    GPIO.PFCE3 = (uint16_t) reg_data;
    reg_data = ((uint32_t) GPIO.PFC3 | (uint32_t) LCD_PORT3_2TH);
    GPIO.PFC3 = (uint16_t) reg_data;
    reg_data = ((uint32_t) GPIO.PIPC3 | (uint32_t)LCD_PORT3_2TH);
    GPIO.PIPC3 = (uint16_t) reg_data;
    reg_data = ((uint32_t) GPIO.PMC3 | (uint32_t)LCD_PORT3_2TH);
    GPIO.PMC3 = (uint16_t) reg_data;

    /* Port 4 */
    reg_data = ((uint32_t) GPIO.PMC4 & (uint32_t) ~LCD_PORT4_2TH);
    GPIO.PMC4 = (uint16_t) reg_data;
    reg_data = (uint32_t) GPIO.PMC4;
    dummy_read = reg_data;

    /* PFCAE4, PFCE4, PFC4 ... 2th alternative function
     PIPC4, PMC4 */
    reg_data = ((uint32_t) GPIO.PFCAE4 & (uint32_t) ~LCD_PORT4_2TH);
    GPIO.PFCAE4 = (uint16_t) reg_data;
    reg_data = ((uint32_t) GPIO.PFCE4 & (uint32_t) ~LCD_PORT4_2TH);
    GPIO.PFCE4 = (uint16_t) reg_data;
    reg_data = ((uint32_t) GPIO.PFC4 | (uint32_t) LCD_PORT4_2TH);
    GPIO.PFC4 = (uint16_t) reg_data;
    reg_data = ((uint32_t) GPIO.PIPC4 | (uint32_t)LCD_PORT4_2TH);
    GPIO.PIPC4 = (uint16_t) reg_data;
    reg_data = ((uint32_t) GPIO.PMC4 | (uint32_t)LCD_PORT4_2TH);
    GPIO.PMC4 = (uint16_t) reg_data;

    /* Port 1 */
    reg_data = ((uint32_t) GPIO.PMC1 & (uint32_t) ~LCD_PORT1_4TH);
    GPIO.PMC1 = (uint16_t) reg_data;
    reg_data = (uint32_t) GPIO.PMC1;
    dummy_read = reg_data;

    /* PFCAE1, PFCE1, PFC1 ... 4th alternative function
     PIPC1, PMC1 */
    reg_data = ((uint32_t) GPIO.PFCAE1 & (uint32_t) ~LCD_PORT1_4TH);
    GPIO.PFCAE1 = (uint16_t) reg_data;
    reg_data = ((uint32_t) GPIO.PFCE1 | (uint32_t) LCD_PORT1_4TH);
    GPIO.PFCE1 = (uint16_t) reg_data;
    reg_data = ((uint32_t) GPIO.PFC1 | (uint32_t) LCD_PORT1_4TH);
    GPIO.PFC1 = (uint16_t) reg_data;
    reg_data = ((uint32_t) GPIO.PIPC1 | (uint32_t)LCD_PORT1_4TH);
    GPIO.PIPC1 = (uint16_t) reg_data;
    reg_data = ((uint32_t) GPIO.PMC1 | (uint32_t)LCD_PORT1_4TH);
    GPIO.PMC1 = (uint16_t) reg_data;

    (void)dummy_read;

#endif
} /* End of function CEU_PortSetting() */

