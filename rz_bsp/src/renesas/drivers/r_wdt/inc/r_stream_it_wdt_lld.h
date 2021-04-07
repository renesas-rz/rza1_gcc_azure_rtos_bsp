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
 * @headerfile     r_stream_it_wdt_lld.h
 * @brief          WDT low level driver
 * @version        1.00
 * @date           27.06.2018
 * H/W Platform    RZA1LU Steam-IT
 *****************************************************************************/
/*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 30.06.2018 1.00 First Release
 *****************************************************************************/

/* Multiple inclusion prevention macro */
#ifndef R_RSKRZA1LU_WDT_LLD_H
#define R_RSKRZA1LU_WDT_LLD_H

/**************************************************************************//**
 * @ingroup R_SW_PKG_93_WDT_API
 * @defgroup R_SW_PKG_93_WDT_LLD WDT Low Level Driver
 * @brief This interface provides low level device specific access to a
 * Watchdog timer WDT device on the RZA1LU Stream-IT board.
 * @anchor R_SW_PKG_93_WDT_LLD_Summary
 * @par Summary
 * @brief This interface defines the functionality that the low level driver
 * must implement for compatibility with the high level driver.
 *
 * @anchor R_SW_PKG_93_WDT_LLD_LIMITATIONS
 * @par Known Limitations
 * @ref NONE_YET
 *
 * @anchor R_SW_PKG_93_WDT_LLD_INSTANCES
 * @par Known Implementations:
 * This driver is used in the RZA1LU Software Package.
 * @ref RENESAS_APPLICATION_SOFTWARE_PACKAGE
 * @{
 *****************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "control.h"

/******************************************************************************
Constant Data
******************************************************************************/

/******************************************************************************
 Macro definitions
 ******************************************************************************/

/* Version Number of API */
/** Driver Name */
#define STDIO_WDT_RZ_LLD_DRV_NAME ("LLD SITRZA1LU.WDT")

/** Major Version Number of API */
#define STDIO_WDT_RZ_LLD_VERSION_MAJOR      (1)
/** Minor Version Number of API */
#define STDIO_WDT_RZ_LLD_VERSION_MINOR      (0)
/** Minor Version Number of API */
#define STDIO_WDT_RZ_LLD_BUILD_NUM          (5165)
/** Unique ID */
#define STDIO_WDT_RZ_LLD_UID                (68)

/******************************************************************************
Public Functions
******************************************************************************/

/******************************************************************************
 Function Name: wdt_hardware_reset
 Description:   Function to generate a system reset,
 Arguments:     None
 Return value:  0 for success -1 on error
 ******************************************************************************/
/**
 * @brief Function to generate a system reset
 * @return -1 on error, otherwise reset will occur
 **/
int_t wdt_hardware_reset (void);

/**
 * @brief Return version of low level driver
 * @param[in] pinfo - pointer to detailed driver information structure
 * @return    DRV_SUCCESS
 **/
int32_t R_WDT_GetVersion(st_ver_info_t *pinfo);

#endif /* R_RSKRZA1LU_WDT_LLD_H  */
/**************************************************************************//**
 * @} (end addtogroup)
 *****************************************************************************/
/******************************************************************************
End  Of File
******************************************************************************/
