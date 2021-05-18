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
 * and to discontinue the availability of this software. By using this
 * software, you agree to the additional terms and conditions found by
 * accessing the following link:
 * http://www.renesas.com/disclaimer
*******************************************************************************
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
 *****************************************************************************/
/******************************************************************************
 * @headerfile     r_rvapi_ceu.h
 * @brief          RVAPI for CEU function header
 * @version        1.00
 * @date           27.06.2018
 * H/W Platform    RZA1H
 *****************************************************************************/
 /*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 10.04.2017 1.00 First Release
 *****************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef R_RVAPI_CEU_H
#define R_RVAPI_CEU_H

/**************************************************************************//**
 * @ingroup R_SW_PKG_93_VIDEO_API 
 * @defgroup R_SW_PKG_93_VIDEO_CEU Video CEU
 * @brief CEU Unit Module
 *
 * @anchor R_SW_PKG_93_VIDEO_CEU_API_SUMMARY
 * @par Summary
 *
 * This middleware module provides functionality for a user to utilise the
 * CEU unit, allowing configuration and capturing. 
 *
 * @anchor R_SW_PKG_93_VIDEO_CEU_API_INSTANCES
 * @par Known Implementations:
 * This driver is used in the RZA1H Software Package.
 * @see RENESAS_APPLICATION_SOFTWARE_PACKAGE
 *
 * @see RENESAS_OS_ABSTRACTION  Renesas OS Abstraction interface
 * @{
 *****************************************************************************/
/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include "r_typedefs.h"
#include "r_ceu.h"
#include "r_ceu_pl.h"

/******************************************************************************
 Macro definitions
 ******************************************************************************/

/******************************************************************************
 Typedef definitions
 ******************************************************************************/
typedef enum 
{
    CAP_END = 0,  /*!< Finished - Capture State */
    CAP_BUSY,     /*!< Busy - Capture State     */
} cap_status_t;

/******************************************************************************
 Exported global functions (to be accessed by other files)
 ******************************************************************************/

/**
 * @brief    CEU initialization processing
 * 
 * @return None. 
 */
void R_RVAPI_InitializeCEU(void);

/**
 * @brief    Closes CEU Operation
 * 
 * @return None. 
 */
void R_RVAPI_TerminateCEU(void);

/**
 * @brief    Calls the function passed as argument
 *          'port_func' 
 * 
 * @param[in] port_func: Pointer to a function relating
 *                       to CEU port settings.
 * 
 * @return None.  
 */
void R_RVAPI_PortSettingCEU(void (* const port_func)(uint32_t));

/**
 * @brief       CEU Setting main processing
 * 
 * @param[in]   config: ceu config
 * 
 * @retval      CEU_ER: driver error code
 */
ceu_error_t R_RVAPI_OpenCEU(const ceu_config_t * const config);

/**
 * @brief       1 frame capture start
 * 
 * @param[in]   cayr:      Capture buffer (Y)
 * @param[in]   cacr:      Capture buffer (CbCr)
 * @param[in]   chdw:      stride
 * 
 * @retval      CEU_ER: driver error code
 */
ceu_error_t R_RVAPI_CaptureStartCEU(const void * cayr, const void * cacr,
        uint32_t chdw);

/**
 * @brief       1 frame capture status
 * 
 * @retval      cap_status: Status of the Capture
 */
cap_status_t R_RVAPI_CaptureStatusCEU(void);
#endif  /* R_RVAPI_CEU_H */
/**************************************************************************//**
 * @} (end addtogroup)
 *****************************************************************************/
