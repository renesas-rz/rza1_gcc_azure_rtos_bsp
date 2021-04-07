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
 * @headerfile     r_camera_OV7740.h
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
#ifndef R_CAMERA_OV7740_H
#define R_CAMERA_OV7740_H

/**************************************************************************//**
 * @ingroup R_SW_PKG_93_CMOS_API
 * @defgroup R_SW_PKG_93_CMOS_OV7740 Camera OV7740 
 * @brief  API for the OV7740 Camera
 *
 * @anchor R_SW_PKG_93_CAMERA_OV7740_API_SUMMARY
 * @par Summary
 *
 * This module contains all the OV7740 configuration defintions.
 * So a user can set up and configure the camera for capture. 
 * 
 * @anchor R_SW_PKG_93_CAMERA_OV7740_API_INSTANCES
 * @par Known Implementations:
 * This driver is used in the RZA1H Software Package.
 * @see RENESAS_APPLICATION_SOFTWARE_PACKAGE
 * @see RENESAS_OS_ABSTRACTION  Renesas OS Abstraction interface
 * @{
 *****************************************************************************/
 /*****************************************************************************
 Macro definitions
 *****************************************************************************/
#define OV7740_I2C_ADDR (0x42) /* The 7740 sits on i2c with ID 0x42 */

 /*****************************************************************************
 Global functions
 *****************************************************************************/
void R_CAMERA_Ov7740Init (int videoResolution);

#endif  /* R_CAMERA_OV7740_H */
/**************************************************************************//**
 * @} (end addtogroup)
 *****************************************************************************/
/* End of File */
