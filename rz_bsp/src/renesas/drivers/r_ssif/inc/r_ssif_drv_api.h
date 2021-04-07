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
 * @headerfile     r_ssif_drv_api.h
 * @brief          Header file for R_SSID_DRV_API Driver
 * @version        1.00
 * @date           27.06.2018
 * H/W Platform    Platform Independent
 *****************************************************************************/
/*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 30.06.2018 1.00 First Release
 *****************************************************************************/

/* Multiple inclusion prevention macro */
#ifndef R_SSIF_DRV_API
    #define R_SSIF_DRV_API

/**************************************************************************//**
 * @ingroup R_SW_PKG_93_POSIX_Drivers
 * @defgroup R_SW_PKG_93_SSIF_API SSIF Driver
 * @brief This interface provides access to the Serial Sound Interface SSIF
 * peripheral.
 * @anchor R_SW_PKG_93_SSIF_API_SUMMARY
 * @par Summary
 * @brief The driver implements High Level access of the SSIF Peripheral
 * @brief Before accessing the driver, call the open function to configure the
 * selected hardware interface.
 *
 * When finished with the driver close it using the close function.
 *
 * This driver does not allocate any heap memory.
 *
 * This driver does use locking primitives for access control.<BR>
 *
 * Driver Instance : @ref g_ssif_driver </b><BR>
 * @copydoc g_ssif_driver
 *
 * <b>Generic example of driver usage</b> ::st_def_stdio_device_t
 *
 * @anchor R_SW_PKG_93_SSIF_API_LIMITATIONS
 * @par Known Limitations
 * @ref NONE_YET
 *
 * @anchor R_SW_PKG_93_SSIF_API_INSTANCES
 * @par Known Implementations:
 * This driver is used in the RZA1H Software Package.
 * @ref RENESAS_APPLICATION_SOFTWARE_PACKAGE
 *
 * @ref R_SW_PKG_93_SOUND_APP
 * @{
 *****************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

    #include "r_devlink_wrapper.h"

/******************************************************************************
 Constant Data
 ******************************************************************************/

/**
 * @var g_ssif_driver
 * <b>Driver Implementation:</b> <BR>
 * \par
 * \"SSIF Device Driver\" - Driver Name <BR>
 * \c ssif_open - Opens the SSIF Driver <BR>
 * \c ssif_close - Closes the SSIF Driver <BR>
 * \c ssif_read - Reads from the SSIF Peripheral <BR>
 * \c ssif_write - Writes to the SSIF Peripheral <BR>
 * \c ssif_control - see @ref e_control_codes_ssif_t for commands<BR>
 * Supported:<BR>
 * \arg \b R_SSIF_CONTROL_CONFIGURE Configure SSIF, uses parameter @ref st_r_ssif_drv_control_t <BR>
 * \arg \b R_SSIF_CONTROL_STATUS Report SSIF status, uses parameter @ref st_r_ssif_drv_control_t <BR>
 * \arg \b R_SSIF_AIO_READ_CONTROL Configure SSIF to read, uses parameter @ref aiocb <BR>
 * \arg \b R_SSIF_AIO_WRITE_CONTROL Configure SSIF to write, uses parameter @ref aiocb <BR>
 *
 * \c ssif_get_version - Get driver version<BR>
 */
extern const st_r_driver_t g_ssif_driver;

/******************************************************************************
 Macro definitions
 ******************************************************************************/

    #define R_SSIF_RZ_HLD_DRV_NAME             ("HLD SSIF")

/** Major Version Number of API.
 * Updated by product owner */
    #define R_SSIF_RZ_HLD_VERSION_MAJOR      (0)

/** Minor Version Number of API.
 * Updated by developer */
    #define R_SSIF_RZ_HLD_VERSION_MINOR      (0)

/** Build Number of API.
 * Generated during customer release */
    #define R_SSIF_RZ_HLD_BUILD_NUM          (5865)

/** Unique ID. Assigned by requirements */
    #define R_SSIF_RZ_HLD_UID                (00)

/* Control Codes */
/** Control codes for SSIF Driver */
typedef enum
{
    R_SSIF_CONTROL_CONFIGURE = 0,   /*!< Configure SSIF, uses parameter @ref st_r_ssif_drv_control_t */
    R_SSIF_CONTROL_CANCEL,
    R_SSIF_CONTROL_STATUS,          /*!< Report SSIF status, uses parameter @ref st_r_ssif_drv_control_t */
    R_SSIF_READ_CONTROL,
    R_SSIF_WRITE_CONTROL,
    R_SSIF_AIO_READ_CONTROL,        /*!< Configure SSIF to read, uses parameter @ref aiocb */
    R_SSIF_AIO_WRITE_CONTROL        /*!< Configure SSIF to write, uses parameter @ref aiocb */
} e_control_codes_ssif_t;

typedef struct st_r_ssif_drv_control_t
{
        void    *p_buf;
        int_t   request;
        int32_t *p_errno;

} st_r_ssif_drv_control_t;

    #define SSIF_CONTROL

#endif /* R_SSIF_DRV_API */
/**************************************************************************//**
 * @} (end addtogroup)
 *****************************************************************************/
