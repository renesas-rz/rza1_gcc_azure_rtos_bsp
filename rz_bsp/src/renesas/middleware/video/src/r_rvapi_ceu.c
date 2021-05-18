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
 * Copyright (C) 2017 Renesas Electronics Corporation. All rights reserved.
 *******************************************************************************/
/**************************************************************************//**
 * File Name :   r_rvapi_ceu.c
 * @file         r_rvapi_ceu.c
 * @version      1.00
 * $Rev: 100 $
 * $Date:: 2017-04-10 21:02:16 +0900#$
 * @brief        RVAPI for CEU function
 ******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include    "r_rvapi_ceu.h"

/******************************************************************************
 Macro definitions
 ******************************************************************************/

/******************************************************************************
 Typedef definitions
 ******************************************************************************/

/******************************************************************************
 Imported global variables and functions (from other files)
 ******************************************************************************/

/******************************************************************************
 Exported global variables and functions (to be accessed by other files)
 ******************************************************************************/

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/
static void ceu_int_callback(const ceu_int_type_t interrupt_flag);
static volatile int32_t capture_flag;

/**************************************************************************//**
 * Function Name : R_RVAPI_InitializeCEU
 * @brief       CEU Initialize
 * @retval      none
 *****************************************************************************/
void R_RVAPI_InitializeCEU(void)
{
    R_CEU_Initialize (&R_CEU_OnInitialize, 0);
    capture_flag = 0;
} /* End of function R_RVAPI_InitializeCEU() */

/**************************************************************************//**
 * Function Name : R_RVAPI_TerminateCEU
 * @brief       CEU  Terminate
 * @retval      none
 *****************************************************************************/
void R_RVAPI_TerminateCEU(void)
{
    R_CEU_InterruptDisable ();
    R_CEU_Terminate (&R_CEU_OnFinalize, 0);
} /* End of function R_RVAPI_TerminateCEU() */

/**************************************************************************//**
 * Function Name : R_RVAPI_PortSettingCEU
 * @brief       CEU port setting
 * @retval      none
 *****************************************************************************/
void R_RVAPI_PortSettingCEU(void (* const port_func)(uint32_t))
{
    if( NULL != port_func )
    {
        port_func (0);
    }
} /* End of function R_RVAPI_PortSettingCEU() */

/**************************************************************************//**
 * Function Name : R_RVAPI_OpenCEU
 * @brief       CEU Setting main processing
 * @param[in]   config              : ceu config
 * @retval      CEU driver error code
 *****************************************************************************/
ceu_error_t R_RVAPI_OpenCEU(const ceu_config_t * const config)
{
    ceu_error_t ceu_error;

    ceu_error = R_CEU_Open (config);

    if (CEU_OK == ceu_error)
    {
        R_CEU_InterruptEnable (CEU_INT_CPEIE, ceu_int_callback);
    }
    return ceu_error;
} /* End of function R_RVAPI_OpenCEU() */

/**************************************************************************//**
 * Function Name : R_RVAPI_CaptureStartCEU
 * @brief       1 frame capture start
 * @param[in]   cayr       : Capture buffer (Y)
 * @param[in]   cacr       : Capture buffer (CbCr)
 * @param[in]   chdw       : stride
 * @retval      Error codes of the CEU driver
 ******************************************************************************/
ceu_error_t R_RVAPI_CaptureStartCEU(const void * cayr, const void * cacr, uint32_t chdw)
{
    ceu_error_t ceu_error;

    if (0 == capture_flag)
    {
        capture_flag = 1;
        ceu_error = R_CEU_Execute (cayr, cacr, chdw);
    }
    else
    {
        ceu_error = CEU_ERR_PARAM;
    }
    return ceu_error;
} /* End of function R_RVAPI_CaptureStartCEU() */

/**************************************************************************//**
 * Function Name : R_RVAPI_CaptureStatusCEU
 * @brief       1 frame capture status
 * @retval      cap status
 ******************************************************************************/
cap_status_t R_RVAPI_CaptureStatusCEU(void)
{
    cap_status_t status;

    if (1 == capture_flag)
    {
        status = CAP_BUSY;
    }
    else
    {
        status = CAP_END;
    }
    return status;
} /* End of function R_RVAPI_CaptureStatusCEU() */

/**************************************************************************//**
 * Function Name : ceu_int_callback
 * @brief       CEU Interrupt callback function
 * @param[in]   interrupt_flag    :
 * @retval      None
 ******************************************************************************/
static void ceu_int_callback(const ceu_int_type_t interrupt_flag)
{
    if ((CEU_INT_CPEIE & interrupt_flag) == 1u)
    {
        capture_flag = 0;
    }
    return;
} /* End of function ceu_int_callback() */
