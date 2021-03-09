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
 * @headerfile     r_sci_drv_api.h
 * @brief          High Level Driver for SCIF
 * @version        1.00
 * @date           27.06.2018
 * H/W Platform    Platform Independent
 *****************************************************************************/
 /*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 30.06.2018 1.00 First Release
 *****************************************************************************/

/* Multiple inclusion prevention macro */
#ifndef SRC_RENESAS_DRIVERS_R_SCI_INC_R_SCI_DRV_API_H_INCLUDED
#define SRC_RENESAS_DRIVERS_R_SCI_INC_R_SCI_DRV_API_H_INCLUDED

/**************************************************************************//**
 * @ingroup R_SW_PKG_93_SCIF_API
 * @defgroup R_SW_PKG_93_SCIF_HLD_API SCIF High Level Driver
 * @brief This interface provides access to SCIF Peripheral
 * @anchor R_SW_PKG_93_SCIF_HLD_API_SUMMARY
 * @par Summary
 * @brief The driver implements High Level access to the SCIF Peripheral.
 * @brief Before accessing the driver, call the open function to configure the
 * selected hardware interface.
 *
 * When finished with the driver close it using the close function.
 *
 * This driver does not allocate any heap memory.
 *
 * This driver does not use locking primitives.<BR>
 *
 * Driver Instance : @ref g_scif_driver </b><BR>
 * @copydoc g_scif_driver
 *
 * <b>Generic example of driver usage</b> ::st_def_stdio_device_t
 *
 *
 * This SCIF High Level Driver is developed for the RZA1LU Stream-IT board.
 * @anchor R_SW_PKG_93_SCIF_HLD_API_INSTANCES
 * @par Known Implementations:
 * This driver is used in the RZA1LU Software Package.
 * @see RENESAS_APPLICATION_SOFTWARE_PACKAGE
 *
 * @see RENESAS_OS_ABSTRACTION  Renesas OS Abstraction interface
 * @{
 *****************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_devlink_wrapper.h"

/******************************************************************************
 Macro definitions
 *****************************************************************************/
/** Driver Name */
#define STDIO_SCIF_RZ_HLD_DRV_NAME ("HLD SCI")

/** Major Version Number of API.
 * Updated by product owner */
#define STDIO_SCIF_RZ_HLD_VERSION_MAJOR          (1)

/** Minor Version Number of API.
 * Updated by developer */
#define STDIO_SCIF_RZ_HLD_VERSION_MINOR          (1)

/** Build Number of API.
 * Generated during customer release */
#define STDIO_SCIF_RZ_HLD_BUILD_NUM              (1000)

/** Unique ID. Assigned by requirements */
#define STDIO_SCIF_RZ_HLD_UID                    (69)

/** Control structure for CTL_SCIF_CONFIGURE */
typedef struct _SCICFG
{
    unsigned long dwBaud;
    unsigned long dwConfig;
} SCICFG,
*PSCICFG;

/******************************************************************************
Constant Data
******************************************************************************/

/**
 * @var g_scif_driver
 * <b>Driver Implementation:</b> <BR>
 * \par
 * \"SCIF Device Driver\" - Driver Name <BR>
 * \c scif_open - Opens the RIIC Driver <BR>
 * \c scif_close - Closes the RIIC Driver <BR>
 * \c scif_read - Read from SCI<BR>
 * \c scif_write - Write to SCI<BR>
 * \c scif_control - see @ref _CTLCODE for commands<BR>
 * Supported:<BR>
 * \arg \b CTL_SCI_SET_CONFIGURATION See @ref _SCICFG for parameter details.<BR>
 * \arg \b CTL_GET_RX_BUFFER_COUNT<BR>
 *
 * \c scif_get_version - Get driver version<BR>
 */
extern const st_r_driver_t g_scif_driver;

#endif /* SRC_RENESAS_DRIVERS_R_SCI_INC_R_SCI_DRV_API_H_INCLUDED */
/**************************************************************************//**
 * @} (end addtogroup)
 *****************************************************************************/
/******************************************************************************
End  Of File
******************************************************************************/
