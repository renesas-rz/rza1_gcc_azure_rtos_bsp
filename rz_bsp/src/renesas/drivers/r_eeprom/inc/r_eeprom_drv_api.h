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
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
 *****************************************************************************/
/******************************************************************************
 * @headerfile     r_eeprom_drv_api.h
 * @brief          Posix driver for EEPROM
 * @version        1.00
 * @date           24.04.2019
 * H/W Platform    Renesas Starter Kit+ for RZ/A1H
 *****************************************************************************/
 /*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 30.06.2018 1.00 First Release
 *              : 24.04.2019 2.00 Porting for RZA1H and RZ/A1LU
 *****************************************************************************/

/* Multiple inclusion prevention macro */
#ifndef R_EEPROM_DRV_API_H
    #define R_EEPROM_DRV_API_H
/**************************************************************************//**
 * @ingroup R_SW_PKG_93_POSIX_Drivers
 * @defgroup R_SW_PKG_93_EEPROM_API EEPROM Driver
 * @brief This interface provides access to an EEPROM memory device.
 * @anchor R_SW_PKG_93_EEPROM_API_SUMMARY
 * @par Summary
 * @brief The driver implements High Level access of an EEPROM memory device.
 * It in turn uses the High level RIIC driver to communicate to the EEPROM
 * device.
 *@brief Before accessing the driver, call the open function to configure the
 * selected hardware interface.
 *
 * When finished with the driver close it using the close function.
 *
 * This driver does not allocate any heap memory.
 *
 * This driver uses locking primitives to control access to the EEPROM and the
 * underlying I2C communications bus.
 *
 * Driver Instance : @ref gR1EX24016Driver </b><BR>
 * @copydoc gR1EX24016Driver
 *
 * <b>Generic example of driver usage</b> ::st_def_stdio_device_t
 *
 * @anchor RZA1H_EEPROM_DRIVER_API_LIMITATIONS
 * @par Known Limitations
 * @ref NONE_YET
 *
 * @anchor R_SW_PKG_93_EEPROM_API_INSTANCES
 * @par Known Implementations:
 * This driver is used in the RZA1H Software Package.
 * @ref RENESAS_APPLICATION_SOFTWARE_PACKAGE
 *
 * @ref RENESAS_OS_ABSTRACTION  Renesas OS Abstraction interface
 *
 * @ref SSGSTARC_56_RIIC_API RIIC Driver
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
    #define STDIO_EEPROM_DRIVER_RZ_HLD_DRV_NAME ("HLD EEPROM")

/** Major Version Number of API.
 * Updated by product owner */
    #define STDIO_EEPROM_DRIVER_RZ_HLD_VERSION_MAJOR      (1)

/** Minor Version Number of API.
 * Updated by developer */
    #define STDIO_EEPROM_DRIVER_RZ_HLD_VERSION_MINOR      (0)

/** Build Number of API.
 * Generated during customer release */
    #define STDIO_EEPROM_DRIVER_RZ_HLD_BUILD_NUM          (1000)

/******************************************************************************
 Type definitions
 ******************************************************************************/
/** EEPROM driver Error codes */
typedef enum
{
    EEPROM_NO_ERROR = 0,        /*!< Operation successful */
    EEPROM_ERROR,               /*!< An error occurred */
    EEPROM_ERROR_NO_RESOURCES   /*!< No resources available */
} e_eeprom_error_t;

/******************************************************************************
 Constant Data
 ******************************************************************************/

#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
/**
 * @var g24C04Driver
 * <b>Driver Implementation:</b> <BR>
 * \par
 * \"CAT24C04 Device Driver\" - Driver Name <BR>
 * \c romOpen - Opens the CDC Driver <BR>
 * \c romClose - Closes the CDC Driver <BR>
 * \c romRead - Read from EEPROM <BR>
 * \c romWrite - Write to EEPROM <BR>
 * \c romControl - see @ref _CTLCODE for commands <BR>
 * Supported:<BR>
 * \arg \b CTL_FILE_SEEK See @ref _FILESEEK for parameter details.<BR>
 * \arg \b CTL_FILE_SIZE Returns EEPROM size in bytes to uint32_t *<BR>
 *
 * \c romGetVersion - Get driver version<BR>
 */
extern const st_r_driver_t g24C04Driver;

#elif (TARGET_BOARD == TARGET_BOARD_RSK)

/**
 * @var gR1EX24016Driver
 * <b>Driver Implementation:</b> <BR>
 * \par
 * \"R1EX24016 Device Driver\" - Driver Name <BR>
 * \c romOpen - Opens the CDC Driver <BR>
 * \c romClose - Closes the CDC Driver <BR>
 * \c romRead - Read from EEPROM <BR>
 * \c romWrite - Write to EEPROM <BR>
 * \c romControl - see @ref _CTLCODE for commands <BR>
 * Supported:<BR>
 * \arg \b CTL_FILE_SEEK See @ref _FILESEEK for parameter details.<BR>
 * \arg \b CTL_FILE_SIZE Returns EEPROM size in bytes to uint32_t *<BR>
 *
 * \c romGetVersion - Get driver version<BR>
 */
extern const st_r_driver_t gR1EX24016Driver;
#endif /* TARGET_BOARD */


#endif /* R_EEPROM_DRV_API_H */
/**************************************************************************//**
 * @} (end addtogroup)
 *****************************************************************************/
/*****************************************************************************
 End Of File
 ******************************************************************************/
