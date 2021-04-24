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
 * @headerfile     r_touch_capacitive.h
 * @brief          Header file for r_touch_capacitive.c
 * @version        1.00
 * @date           27.06.2018
 * H/W Platform    RZA1H
 *****************************************************************************/
 /*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 30.06.2018 1.00 First Release
 *****************************************************************************/

/* Multiple inclusion prevention macro */
#ifndef TOUCH_CAPACITIVE_H_
#define TOUCH_CAPACITIVE_H_

/**************************************************************************//**
 * @ingroup RENESAS_APPLICATION_SOFTWARE_PACKAGE Software Package
 * @defgroup R_SW_PKG_93_TOUCH_APP Touchscreen Application
 * @brief Application that demonstrates the use of the touchscreen.
 * @anchor R_SW_PKG_93_TOUCH_APP_SUMMARY
 * @par Summary
 * @brief Application that demonstrates the use of the touchscreen.<br>
 * Application will place an arrow at the location on the screen where a
 * touch has been recorded and will report the touch style and co-ordinates
 * on the console<br>
 * The application is started by typing "tsdemo" on the console.
 * Pressing a key will terminate the application.<br>
 * Note that the command is disabled if the Ethernet module is enabled.
 * If Ethernet is enabled then the following message will be displayed on
 * the console:
 * <br>
 * <em><br>
 * REE> tsdemo<br>
 * Warning command disabled<br>
 * In configuration file /RZA1_Sample/src/renesas/configuration/application_cfg.h<br>
 * R_SELF_LOAD_MIDDLEWARE_ETHERNET_MODULES should be disabled when using this command<br>
 * <br>
 * REE></em>
 *
 * @anchor R_SW_PKG_93_TOUCH_APP_INSTANCES
 * @par Known Implementations:
 * This driver is used in the RZA1H Software Package.
 * @see RENESAS_APPLICATION_SOFTWARE_PACKAGE
 * @ref R_SW_PKG_93_TOUCH_API
 * @ref R_SW_PKG_93_RIIC_API
 * @{
 *****************************************************************************/

/******************************************************************************
User Includes
******************************************************************************/
#include "r_typedefs.h"

typedef struct _iostr
{
    FILE *p_in;
    FILE *p_out;
} st_iostr_t, * pst_iostr_t;

/******************************************************************************
 Functions Prototypes
 ******************************************************************************/

/**
 * @brief Creates touch screen task
 * @param pCom
 */
void R_TOUCH_ApplicationMain( pst_iostr_t pCom);

#endif // TOUCH_CAPACITIVE_H_
/**************************************************************************//**
 * @} (end addtogroup)
 *****************************************************************************/
