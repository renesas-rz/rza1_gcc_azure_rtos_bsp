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
 *
 * Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.
 *******************************************************************************/
/**************************************************************************//**
 * File Name :   r_ceu_pl.c
 * @file         r_ceu_pl.c
 * @version      1.00
 * $Rev: 182 $
 * $Date:: 2017-05-11 07:35:51 +0900#$
 * @brief        CEU OS porting
 ******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include "iodefine_cfg.h"
#include "dev_drv.h"                /* Device driver header */
#include "r_intc.h"                   /* Interrupt Controller driver header */
#include "r_ceu_pl.h"

/******************************************************************************
 Macro definitions
 ******************************************************************************/
#define CEU_INT_PRIORITY  (5) /* level 5 (0-31) */

#define STP66_BIT      (0x40u)
#define STBRQ10_BIT    (0x01u)
#define STBAK10_BIT    (0x01u)

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/

/**************************************************************************//**
 * Function Name: R_CEU_OnInitialize
 * @brief       CEU User difinistion part Init
 * @param[in]   user_num          :
 * @retval      none
 *****************************************************************************/
void R_CEU_OnInitialize(const uint32_t user_num)
{
    uint32_t reg_data;
    volatile uint8_t dummy_read;

    /* Standby control register 6 (STBCR6)
     b6      -0------;  MSTP66 : 0 : Capture Engine Unit enable */
    reg_data = (uint32_t) CPG.STBCR6 & (uint32_t) ~STP66_BIT;
    CPG.STBCR6 = (uint8_t) reg_data;

    /* In order to reflect the change, a dummy read should be done. */
    reg_data = (uint32_t) CPG.STBCR6;
    dummy_read = (uint8_t)reg_data;

    /* Standby Request Register 1 (STBREQ1)
     b0      -------0;  STBRQ10 : The standby request to CEU is invalid. */
    reg_data = (uint32_t) CPG.STBREQ1 & (uint32_t) ~STBRQ10_BIT;
    CPG.STBREQ1 = (uint8_t) reg_data;

    /* Standby Acknowledge Register 1 (STBACK1)
     b0      -------*;  STBAK10 : Standby acknowledgement from CEU. */
    while (((uint32_t) CPG.STBACK1 & (uint32_t)STBAK10_BIT) != 0u)
    {
        /* Wait for the STBAK10 to be cleared to 0. */
    }

    R_CEU_OnEnableInterrupt (user_num);
    (void)dummy_read;
} /* End of function R_CEU_OnInitialize() */

/**************************************************************************//**
 * Function Name: R_CEU_OnFinalize
 * @brief       CEU User difinistion part final
 * @param[in]   user_num          :
 * @retval      none
 *****************************************************************************/
void R_CEU_OnFinalize(const uint32_t user_num)
{
    uint32_t reg_data;

    R_CEU_OnDisableInterrupt (user_num);

    /* Standby Request Register 1 (STBREQ1)
     b0      -------0;  STBRQ10 : The standby request to CEU is invalid. */
    reg_data = (uint32_t) CPG.STBREQ1 | (uint32_t)STBRQ10_BIT;
    CPG.STBREQ1 = (uint8_t) reg_data;

    /* Standby Acknowledge Register 1 (STBACK1)
     b0      -------*;  STBAK10 : Standby acknowledgement from CEU. */
    while (((uint32_t) CPG.STBACK1 & (uint32_t)STBAK10_BIT) == 0u)
    {
        /* Wait for the STBAK10 to be set to 1. */
    }

    /* Standby control register 6 (STBCR6)
     b6      -0------;  MSTP66 : 0 : Capture Engine Unit enable */
    reg_data = (uint32_t) CPG.STBCR6 | (uint32_t)STP66_BIT;
    CPG.STBCR6 = (uint8_t) reg_data;

} /* End of function R_CEU_OnFinalize() */

/**************************************************************************//**
 * Function Name: R_CEU_OnEnableInterrupt
 * @brief       CEU User difinistion part Enable Interrupt
 * @param[in]   user_num          :
 * @retval      Error code
 *****************************************************************************/
int32_t R_CEU_OnEnableInterrupt(const uint32_t user_num)
{
    UNUSED_PARAM(user_num);
    int32_t        int_err = DEVDRV_SUCCESS;
    int32_t        ret_code = 0;

    int_err = R_INTC_RegistIntFunc(INTC_ID_CEUI, R_CEU_Isr);
    if(DEVDRV_SUCCESS != int_err)
    {
        ret_code = -1;
    }
    else
    {
        ret_code = R_INTC_SetPriority(INTC_ID_CEUI, CEU_INT_PRIORITY);
        if(DEVDRV_SUCCESS != int_err)
        {
            ret_code = -1;
        }
        else
        {
            ret_code = R_INTC_Enable(INTC_ID_CEUI);
            if(DEVDRV_SUCCESS != int_err)
            {
                ret_code = -1;
            }
        }
    }
    return ret_code;
} /* End of function R_CEU_OnEnableInterrupt() */

/**************************************************************************//**
 * Function Name: R_CEU_OnDisableInterrupt
 * @brief       CEU User difinistion part Disable Interrupt
 * @param[in]   user_num          :
 * @retval      Error code
 *****************************************************************************/
int32_t R_CEU_OnDisableInterrupt(const uint32_t user_num)
{
    UNUSED_PARAM(user_num);

    int32_t        int_err = DEVDRV_SUCCESS;
    int32_t        ret_code = 0;

    int_err = R_INTC_Disable(INTC_ID_CEUI);
    if (DEVDRV_SUCCESS != int_err)
    {
        ret_code = -1;
    }
    return ret_code;
} /* End of function R_CEU_OnDisableInterrupt() */
