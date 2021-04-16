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
 * @headerfile     r_sitrza1h_led_lld.h
 * @brief          STDIO LED Low layer Driver API
 * @version        1.00
 * @date           27.06.2018
 * H/W Platform    RSK+RZA1H CPU Board
 *****************************************************************************/
 /*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 30.06.2018 1.00 First Release
 *****************************************************************************/

/* Multiple inclusion prevention macro */
#ifndef SRC_RENESAS_DRIVERS_R_LED_INC_R_RSKRZA1H_LED_LLD_H_
#define SRC_RENESAS_DRIVERS_R_LED_INC_R_RSKRZA1H_LED_LLD_H_

 /*************************************************************************//**
 * @ingroup R_SW_PKG_93_LED_API LED Driver
 * @defgroup RZA1H_LED_LLD_API LED low layer driver configuration
 * @brief This interface allows control of the LED interface on the RZA1H
 * Stream-IT Board.
 *
 * @anchor RZA1H_LED_LLD_API_Summary
 * @par Summary
 * @brief Low level, device specific driver for LEDs on the RZA1H
 * board.
 *
 * @anchor RZA1H_LED_LLD_API_LIMITATIONS
 * @par Known Limitations
 * NONE
 *
 * @anchor RZA1H_LED_LLD_API_INSTANCES
 * @par Known Implementations
 * @ref NONE_YET
 * @{
 *****************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include "r_devlink_wrapper.h"

/******************************************************************************
 Macro definitions
 ******************************************************************************/

/* Guard against failure to include driver support interface */
#ifndef DRIVER_CFG_PRV_65_DEFINED_R_LED_DRV_API_H_
#error Failure to include driver support interface in "r_sc_cfg.h"
#endif

/* Version Number of API. */

/** Driver Name */
#define STDIO_LED_RZ_LLD_DRV_NAME ("LLD SITRZA1H.LED")

/** Major Version Number of API. */
#define STDIO_LED_RZ_LLD_VERSION_MAJOR      (2)
/** Minor Version Number of API. */
#define STDIO_LED_RZ_LLD_VERSION_MINOR      (0)
/** Minor Version Number of API. */
#define STDIO_LED_RZ_LLD_BUILD_NUM          (5165)
/** Unique ID. */
#define STDIO_LED_RZ_LLD_UID                (68)

/******************************************************************************
 Constant Data
 ******************************************************************************/
/* Exposed Driver configuration data structure */

/******************************************************************************
 Interface
 ******************************************************************************/

/**
 * @brief LowLevel driver initialise interface function
 *
 * @return None.
 */
void    R_LED_InitialiseHwIf(void);

 /**
  * @brief LowLevel driver un-initialise interface function
  *
  * @return None.
  */
void    R_LED_UninitialiseHwIf(void);

/**
 * @brief Function to update the LEDs
 * @param[in] iLedMap - a bit map of the LEDs
 * @return DEVDRV_SUCCESS for success or DEVDRV_ERROR on error.
 */
int32_t R_LED_Update(int32_t iLedMap);

/**
 * @brief Get version of low level driver
 * @param[out] *pinfo - pointer to structure to report version to
 * @return DEVDRV_SUCCESS.
 */
int32_t R_LED_GetVersion(st_ver_info_t *pinfo);

#endif /* SRC_RENESAS_DRIVERS_R_LED_INC_R_RSKRZA1H_LED_LLD_H_ */
/**************************************************************************//**
 * @} (end addtogroup)
 *****************************************************************************/
/******************************************************************************
 End  Of File
 ******************************************************************************/

