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
 * @headerfile     r_ostm_drv_api.h
 * @brief          OS timer driver API header
 * @version        1.00
 * @date           27.06.2018
 * H/W Platform    RSK+RZA1H CPU Board
 *****************************************************************************/
 /*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 30.06.2018 1.00 First Release
 *****************************************************************************/

/* Multiple inclusion prevention macro */
#ifndef SRC_RENESAS_DRIVERS_R_OSTM_INC_R_OSTM_DRV_API_H_
#define SRC_RENESAS_DRIVERS_R_OSTM_INC_R_OSTM_DRV_API_H_

/***************************************************************************//**
 * @ingroup SSGSTARC_37_OSTM_DRIVER
 * @defgroup OSTM_HLD_DRIVER OSTM High Level Driver API
 * @brief This interface allows high level control of the OSTM module
 *
 * @anchor OSTM_HLD_DRIVER_SUMMARY
 * @par Summary
 * @brief The driver supports STDIO or direct implementation for controlling the
 * Operating System Timer Module (OSTM).
 * This High Layer driver (HLD) requires an associated low layer implementation
 * specific to the platform.
 * The HLD exposes a global for the OSTM driver, this must only be accessed
 * directly if used in an OS Free implementation.
 * @brief Before accessing the driver, call the open function to configure the
 * selected hardware interface, this must be implemented in the Lower Driver
 * layer. When open the control function can be used to control the operation
 * of the OSTM.
 *
 * When finished with the driver close it using the close function.
 *
 * This driver does not allocate any heap memory.
 *
 * This driver does not have any locking primitives.
 *
 * Driver Instance : @ref g_ostm_driver </b><BR>
 * @copydoc g_ostm_driver
 *
 * <b>Generic example of driver usage</b> ::st_def_stdio_device_t
 *
 * @anchor OSTM_HLD_DRIVER_LIMITATIONS
 * @par Known Limitations
 * @ref NONE_YET
 *
 * @anchor OSTM_HLD_DRIVER_INSTANCES
 * @par Known Implementations
 * This driver is used in the RZA1H Software Package.
 * @ref RENESAS_APPLICATION_SOFTWARE_PACKAGE
 *
 * @anchor OSTM_HLD_DRIVER_RELATED
 * @par Related modules
 * @ref RENESAS_OS_ABSTRACTION  Renesas OS Abstraction interface
 * @{
 *****************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 *****************************************************************************/
#include "r_typedefs.h"
#include "r_devlink_wrapper.h"
#include "r_sc_cfg.h"
#include "r_ostm_drv_sc_cfg.h"
#include "r_ostm_drv_link.h"

/******************************************************************************
 Macro definitions
 *****************************************************************************/

/** Driver name */
#define STDIO_OSTM_RZ_HLD_DRV_NAME ("HLD STDIO.OSTM")

/** Major Version Number of API.
 * Updated by product owner */
#define STDIO_OSTM_RZ_HLD_VERSION_MAJOR      (1)

/** Minor Version Number of API.
 * Updated by developer */
#define STDIO_OSTM_RZ_HLD_VERSION_MINOR      (1)

/** Build Number of API.
 * Generated during customer release */
#define STDIO_OSTM_RZ_HLD_BUILD_NUM          (5928)

/** Unique ID. Assigned by requirements */
#define STDIO_OSTM_RZ_HLD_UID                (63)

/* Total channels supported channels supported */
#define R_CFG_OSTM_DRV_CHANNELS_AVAIL_PRV (2)

/**********************************************************************************************************************
 Typedef definitions
 *********************************************************************************************************************/
/******************************************************************************
 Variable Externs
 ******************************************************************************/

/**
 * @var g_ostm_driver
 * <b>Driver Implementation:</b> <BR>
 * \par
 * \"OS Timer Driver\" - Driver Name <BR>
 * \c ostm_open - Opens the OSTM Driver <BR>
 * \c ostm_close - Closes the OSTM Driver <BR>
 * \c ostm_read - Read LED status directly from pin <BR>
 * write is not supported <BR>
 * \c ostm_control - see @ref e_ctrl_code_ostm_t for commands <BR>
 * \c ostm_get_version - Get driver version <BR>
 */
extern const st_r_driver_t g_ostm_driver;

/** array of available channels for OSTM */
extern bool_t gs_r_drv_ostm_open[R_CFG_OSTM_DRV_CHANNELS_AVAIL_PRV];

/******************************************************************************
 Functions Prototypes
 ******************************************************************************/
/* ==== API functions ==== */

#endif  /* SRC_RENESAS_DRIVERS_R_OSTM_INC_R_OSTM_DRV_API_H_ */
/**************************************************************************//**
 * @} (end addtogroup)
 *****************************************************************************/
/***************************************************************************//**
  End  Of File
 *****************************************************************************/
