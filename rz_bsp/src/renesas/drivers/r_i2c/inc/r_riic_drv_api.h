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
 * @headerfile     r_riic_drv_api.h
 * @brief          High Level Driver for RIIC
 * @version        1.00
 * @date           27.06.2018
 * H/W Platform    RZ/A1
 *****************************************************************************/
 /*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 30.06.2018 1.00 First Release
 *****************************************************************************/

/* Multiple inclusion prevention macro */
#ifndef RENESAS_DRIVERS_R_RIIC_INC_R_RIIC_DRV_API_H_
#define RENESAS_DRIVERS_R_RIIC_INC_R_RIIC_DRV_API_H_

/**************************************************************************//**
 * @ingroup SSGSTARC_56_RIIC_API
 * @defgroup SSGSTARC_56_RIIC_HLD_API RIIC High Level Driver
 * @brief This interface provides access to RIIC Peripheral.
 * @anchor SSGSTARC_56_RIIC_HLD_API_SUMMARY
 * @par Summary
 * @brief The driver implements High Level access to the RIIC Peripheral.
 * @brief Before accessing the driver, call the open function to configure the
 * selected hardware interface.
 *
 * When finished with the driver close it using the close function.
 *
 * This driver does not allocate any heap memory.
 *
 * This driver uses locking primitives to control access to the I2C
 * communications bus.<BR>
 *
 * Driver Instance : @ref g_riic_driver </b><BR>
 * @copydoc g_riic_driver
 *
 * <b>Generic example of driver usage</b> ::st_def_stdio_device_t
 *
 * @anchor RZA1H_RIIC_DRIVER_HLD_API_LIMITATIONS
 * @par Known Limitations
 * @ref NONE_YET
 *
 * @anchor SSGSTARC_56_RIIC_API_INSTANCES
 * @par Known Implementations:
 * @ref RENESAS_APPLICATION_SOFTWARE_PACKAGE
 *
 * @ref RENESAS_OS_ABSTRACTION  Renesas OS Abstraction interface
 *
 * @ref R_SW_PKG_93_EEPROM_API
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
#define STDIO_RIIC_RZ_HLD_DRV_NAME ("HLD RIIC")

/** Major Version Number of API.
 * Updated by product owner */
#define STDIO_RIIC_RZ_HLD_VERSION_MAJOR      (1)

/** Minor Version Number of API.
 * Updated by developer */
#define STDIO_RIIC_RZ_HLD_VERSION_MINOR      (0)

/** Build Number of API.
 * Generated during customer release */
#define STDIO_RIIC_RZ_HLD_BUILD_NUM          (1000)

/** Unique ID. Assigned by requirements */
#define STDIO_RIIC_RZ_HLD_UID                (56)


/******************************************************************************
Constant Data
******************************************************************************/

/**
 * @var g_riic_driver
 * <b>Driver Implementation:</b> <BR>
 * \par
 * \"I2C Device Driver\" - Driver Name <BR>
 * iic_hld_open - Opens the RIIC Driver <BR>
 * iic_hld_close - Closes the RIIC Driver <BR>
 * read and write are not supported <BR>
 * iic_hld_control - see @ref e_ctrl_code_riic_t for commands <BR>
 * iic_hld_get_version - Get driver version<BR>
 */
extern const st_r_driver_t g_riic_driver;
 /**************************************************************************//**
  * @} (end addtogroup)
  *****************************************************************************/
#endif /* RENESAS_DRIVERS_R_RIIC_INC_R_RIIC_DRV_API_H_ */

