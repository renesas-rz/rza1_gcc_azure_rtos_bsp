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
 * @headerfile     r_camera_if.h
 * @brief          Camera driver header
 * @version        1.00
 * @date           27.06.2018
 * H/W Platform    RZA1H
 *****************************************************************************/
 /*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 13.02.2017 1.00 First Release
 *****************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef R_CAMERA_IF_H
#define R_CAMERA_IF_H

/**************************************************************************//**
 * @ingroup R_SW_PKG_93_NONOS_MIDDLEWARE
 * @defgroup R_SW_PKG_93_CMOS_API CMOS
 * @brief CMOS Camera API
 *
 * @anchor R_SW_PKG_93_CAMERA_IF_API_SUMMARY
 * @par Summary
 *
 * This module contains the configuration settings for camera resolutions. 
 * 
 * @anchor R_SW_PKG_93_CAMERA_IF_API_INSTANCES
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
#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
#define CAMERA_OV7670    (0)
#define CAMERA_OV7740    (1)
#define CAMERA_SELECT    (CAMERA_OV7740)

#if ( CAMERA_SELECT == CAMERA_OV7670 )
#include "r_camera_ov7670.h"
#define CAMERA_I2C_ADDR    (OV7670_I2C_ADDR)
#elif ( CAMERA_SELECT == CAMERA_OV7740 )
#include "r_camera_ov7740.h"
#define CAMERA_I2C_ADDR    (OV7740_I2C_ADDR)
#else
#error ERROR: Invalid INPUT_SELECT.
#endif

/******************************************************************************
Typedef definitions
******************************************************************************/
/* Do not change following #define value */
#define CAMERA_RES_QVGA    (0)   /*!< Get QVGA size data from Camera */
#define CAMERA_RES_VGA     (1)   /*!< Get  VGA size data from Camera */

/* Select Imput Image Size from Camera */
#define CAMERA_RES_IMAGE_SIZE     (CAMERA_RES_QVGA)

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Functions Prototypes
******************************************************************************/
#else
#endif

#endif /* R_CAMERA_IF_H */
/**************************************************************************//**
 * @} (end addtogroup)
 *****************************************************************************/
