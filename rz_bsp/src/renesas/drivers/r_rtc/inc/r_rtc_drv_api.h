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
 * @headerfile     r_rtc_drv_api.h
 * @brief          Posix driver for RTC
 * @version        1.00
 * @date           27.06.2018
 * H/W Platform    Platform Independent
 *****************************************************************************/
 /*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 30.06.2018 1.00 First Release
 *****************************************************************************/

/* Multiple inclusion prevention macro */
#ifndef R_RTC_DRV_API
#define R_RTC_DRV_API

/**************************************************************************//**
 * @ingroup R_SW_PKG_93_POSIX_Drivers
 * @defgroup R_SW_PKG_93_RTC_API RTC Driver
 * @brief This interface provides access to an Real-Time Clock RTC device.
 * @anchor R_SW_PKG_93_RTC_API_SUMMARY
 * @par Summary
 * @brief The driver implements High Level access of the RTC Peripheral
 * @brief Before accessing the driver, call the open function to configure the
 * selected hardware interface.
 *
 * When finished with the driver close it using the close function.
 *
 * This driver does not allocate any heap memory.
 *
 * This driver does not use locking primitives.<BR>
 *
 * Driver Instance : @ref g_rtc_driver </b><BR>
 * @copydoc g_rtc_driver
 *
 * <b>Generic example of driver usage</b> ::st_def_stdio_device_t
 *
 * @anchor R_SW_PKG_93_RTC_API_LIMITATIONS
 * @par Known Limitations
 * @ref NONE_YET
 *
 * @anchor R_SW_PKG_93_RTC_API_INSTANCES
 * @par Known Implementations:
 * This driver is used in the RZA1 Software Package.
 * @ref RENESAS_APPLICATION_SOFTWARE_PACKAGE
 * @{
 *****************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_devlink_wrapper.h"



/******************************************************************************
 Macro definitions
 ******************************************************************************/
/** Driver Name */
#define STDIO_RTC_RZ_HLD_DRV_NAME             ("HLD STDIO.RTC")

/** Major Version Number of API.
 * Updated by product owner */
#define STDIO_RTC_RZ_HLD_VERSION_MAJOR      (1)

/** Minor Version Number of API.
 * Updated by developer */
#define STDIO_RTC_RZ_HLD_VERSION_MINOR      (9)

/** Build Number of API.
 * Generated during customer release */
#define STDIO_RTC_RZ_HLD_BUILD_NUM          (5865)

/** Unique ID. Assigned by requirements */
#define STDIO_RTC_RZ_HLD_UID                (65)

/******************************************************************************
Variable Externs
******************************************************************************/

/**
 * @var g_rtc_driver
 * <b>Driver Implementation:</b> <BR>
 * \par
 * \"RTC Device Driver\" - Driver Name <BR>
 * \c rtc_open - Opens the RTC Driver <BR>
 * \c rtc_close - Closes the RTC Driver <BR>
 * read and write are not supported <BR>
 * \c rtc_control - see @ref _CTLCODE for commands <BR>
 * Supported:<BR>
 * \arg \b CTL_GET_DATE See @ref _DATE for parameter details<BR>
 * \arg \b CTL_SET_DATE See @ref _DATE for parameter details<BR>
 *
 * \c rtc_get_version - Get driver version<BR>
 */
extern const st_r_driver_t g_rtc_driver;

#endif /* R_RTC_DRV_API */
 /**************************************************************************//**
  * @} (end addtogroup)
  *****************************************************************************/
/******************************************************************************
 End  Of File
 ******************************************************************************/
