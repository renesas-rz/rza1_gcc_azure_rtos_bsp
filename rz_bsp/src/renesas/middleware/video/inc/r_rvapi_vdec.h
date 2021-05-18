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
 * @headerfile     r_rvapi_vdec.h
 * @brief          RVAPI for VDEC function header
 * @version        1.00
 * @date           27.06.2018
 * H/W Platform    RZA1H
 *****************************************************************************/
 /*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 10.04.2017 1.00 First Release
 *****************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef R_RVAPI_VDEC_H
#define R_RVAPI_VDEC_H

/**************************************************************************//**
 * @ingroup R_SW_PKG_93_VIDEO_API 
 * @defgroup R_SW_PKG_93_VIDEO_VDEC Video VDEC
 * @brief VDEC Unit Module 
 *
 * @anchor R_SW_PKG_93_VIDEO_VDEC_API_SUMMARY
 * @par Summary
 *
 * This middleware module provides functionality for a user to utilise the
 * Video Decoder EnCoder (VDEC) unit, for detecting the colour format of
 * a video input. 
 * 
 * @anchor R_SW_PKG_93_VIDEO_VDEC_API_INSTANCES
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
#include "mcu_board_select.h"
#if ((TARGET_RZA1 == TARGET_RZA1H) || (TARGET_RZA1 == TARGET_RZA1M))

#include "r_typedefs.h"
#include "r_vdec.h"
#include "r_vdec_user.h"

/******************************************************************************
 Typedef definitions
 ******************************************************************************/
/*! @enum ana_video_col_sys_t
 *  @brief Video color system
 */
typedef enum
{
    GRPH_COL_SYS_UNKNOWN = -1,     /*!< Unknown          */
    GRPH_COL_SYS_NTSC_358 = 0,     /*!< NTSC-3.58        */
    GRPH_COL_SYS_NTSC_443 = 1,     /*!< NTSC-4.43        */
    GRPH_COL_SYS_PAL_443 = 2,      /*!< PAL-4.43         */
    GRPH_COL_SYS_PAL_M = 3,        /*!< PAL-M            */
    GRPH_COL_SYS_PAL_N = 4,        /*!< PAL-N            */
    GRPH_COL_SYS_SECAM = 5,        /*!< SECAM            */
    GRPH_COL_SYS_NTSC_443_60 = 6,  /*!< NTSC-4.43 (60Hz) */
    GRPH_COL_SYS_PAL_60 = 7,       /*!< PAL-60           */
    GRPH_COL_SYS_NUM = 8           
} ana_video_col_sys_t;

/******************************************************************************
 Exported global functions (to be accessed by other files)
 ******************************************************************************/

/**
 * @brief       Video decoder initialization
 * 
 * @param[in]   channel:         Channel for VDEC
 * @param[in]   adc_vinsel:      Input pin control
 * 
 * @retval      ana_video_col_sys_t:  Detected color system
 */
ana_video_col_sys_t R_RVAPI_AnalogVideDecodeDVDEC(const vdec_channel_t channel, const vdec_adc_vinsel_t adc_vinsel);

#endif /* ((TARGET_RZA1 == TARGET_RZA1H) || (TARGET_RZA1 == TARGET_RZA1M)) */

#endif  /* R_RVAPI_VDEC_H */
/**************************************************************************//**
 * @} (end addtogroup)
 *****************************************************************************/
