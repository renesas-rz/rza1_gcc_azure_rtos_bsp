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
 * @headerfile     r_rskrza1lu_rtc_lld.h
 * @brief          RTC low level driver
 * @version        1.00
 * @date           27.06.2018
 * H/W Platform    RZA1LU Steam-IT
 *****************************************************************************/
/*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 30.06.2018 1.00 First Release
 *****************************************************************************/

/* Multiple inclusion prevention macro */
#ifndef R_RSKRZA1LU_RTC_LLD_H
    #define R_RSKRZA1LU_RTC_LLD_H

/**************************************************************************//**
 * @ingroup R_SW_PKG_93_RTC_API
 * @defgroup R_SW_PKG_93_RTC_LLD RTC Low Level Driver
 * @brief This interface provides low level device specific access to a
 * Real-Time Clock RTC device on the RZA1LU Stream-IT board.
 * @anchor R_SW_PKG_93_RTC_LLD_Summary
 * @par Summary
 * @brief This interface defines the functionality that the low level driver
 * must implement for compatibility with the high level driver.
 *
 * @anchor R_SW_PKG_93_RTC_LLD_LIMITATIONS
 * @par Known Limitations
 * @ref NONE_YET
 *
 * @anchor R_SW_PKG_93_RTC_LLD_INSTANCES
 * @par Known Implementations:
 * This driver is used in the RZA1LU Software Package.
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
    #define RTC_COUNTER_START   (1)
    #define RTC_COUNTER_STOP    (0)

/* Version Number of API */
/** Driver name */
    #define STDIO_RTC_RZ_LLD_DRV_NAME ("LLD SITRZA1LU.RTC")

/** Major Version Number of API */
    #define STDIO_RTC_RZ_LLD_VERSION_MAJOR      (1)
/** Minor Version Number of API */
    #define STDIO_RTC_RZ_LLD_VERSION_MINOR      (0)
/** Minor Version Number of API */
    #define STDIO_RTC_RZ_LLD_BUILD_NUM          (5165)
/** Unique ID */
    #define STDIO_RTC_RZ_LLD_UID                (68)

/******************************************************************************
 Function prototypes
 ******************************************************************************/

/**
 * @brief Initialises the RTC
 **/
void R_RTC_Init (void);

/**
 * @brief Starts or stops the RTC clock
 * @param[in] start - non-zero to start the clock, zero to stop it
 **/
void R_RTC_StartStop (uint8_t start);

/**
 * @brief Determine if bus is busy
 * @param[out] year
 * @param[out] month
 * @param[out] date
 * @param[out] hour
 * @param[out] minute
 * @param[out] second
 * @param[out] day_of_week
 **/
void R_RTC_SetDateTime (uint16_t year, uint8_t month, uint8_t date, uint8_t hour, uint8_t minute, uint8_t second,
        uint8_t day_of_week);

/**
 * @brief Sets the date and time
 * @param[out] year
 * @param[out] month
 * @param[out] date
 * @param[out] hour
 * @param[out] minute
 * @param[out] second
 * @param[out] day_of_week
 **/
void R_RTC_GetDateTime (uint16_t *year, uint8_t *month, uint8_t *date, uint8_t *hour, uint8_t *minute, uint8_t *second,
        uint8_t *day_of_week);

/**
 * @brief Return version of low level driver
 * @param[in] pinfo - pointer to detailed driver information structure
 * @return    DRV_SUCCESS
 **/
int32_t R_RTC_GetVersion (st_ver_info_t *pinfo);

#endif /* R_RSKRZA1LU_RTC_LLD_H */
/**************************************************************************//**
 * @} (end addtogroup)
 *****************************************************************************/
