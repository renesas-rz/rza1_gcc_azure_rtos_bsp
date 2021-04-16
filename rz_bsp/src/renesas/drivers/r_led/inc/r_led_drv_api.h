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
 * @headerfile     r_led_drv_api.h
 * @brief          Header file for LED Driver
 * @version        1.00
 * @date           27.06.2018
 * H/W Platform    RZ/RZA1
 *****************************************************************************/
 /*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 30.06.2018 1.00 First Release
 *****************************************************************************/

/* Multiple inclusion prevention macro */
#ifndef SRC_RENESAS_DRIVERS_R_LED_INC_R_LED_DRV_API_H_
#define SRC_RENESAS_DRIVERS_R_LED_INC_R_LED_DRV_API_H_
/**************************************************************************//**
 * @ingroup R_SW_PKG_93_LED_API
 * @defgroup LED_HLD_API LED High Level Driver
 * @brief This interface allows control of an LED interface of up to 16 LEDs.
 *
 * @anchor LED_HLD_API_Summary
 * @par Summary
 * @brief The driver supports High Level or direct implementation for controlling
 * up to 16 LEDs. This High Layer requires an associated low layer
 * implementation specific to the platform.
 * The HLD exposes a global for the LED driver, this must only be accessed
 * directly if used in an OS Free implementation.
 * @brief Before accessing the driver, call the open function to configure the
 * selected hardware interface, this must be implemented in the Lower Driver
 * layer. When open the control function can be used to control the status of
 * the LEDS.
 *
 * When finished with the driver close it using the close function.
 *
 * This driver does not allocate any heap memory.
 *
 * This driver does use locking primitives to manage access to the user LEDs.
 *
 * Driver Instance : @ref g_stdio_led_driver </b><BR>
 * @copydoc g_stdio_led_driver
 *
 * <b>Generic example of driver usage</b> ::st_def_stdio_device_t
 *
 * @anchor LED_HLD_API_LIMITATIONS
 * @par Known Limitations
 * Only LED 0 is supported by this driver on the RZA1 board for
 * the User LED.
 *
 * @anchor LED_HLD_API_INSTANCES
 * @par Known Implementations
 * This driver is used in the RZA1 Software Package.
 * @ref RENESAS_APPLICATION_SOFTWARE_PACKAGE
 * @{
 *****************************************************************************/

/*****************************************************************************
 Includes   <System Includes> , "Project Includes"
 *****************************************************************************/
#include "r_typedefs.h"
#include "r_devlink_wrapper.h"
#include "r_sc_cfg.h"
#include "r_led_drv_sc_cfg.h"

/******************************************************************************
 Macro definitions
 ******************************************************************************/

#define STDIO_LED_RZ_HLD_DRV_NAME ("HLD STDIO.LED")

/** Major Version Number of API.
 * Updated by product owner */
#define STDIO_LED_RZ_HLD_VERSION_MAJOR      (1)

/** Minor Version Number of API.
 * Updated by developer */
#define STDIO_LED_RZ_HLD_VERSION_MINOR      (9)

/** Build Number of API.
 * Generated during customer release */
#define STDIO_LED_RZ_HLD_BUILD_NUM          (5865)

/** Unique ID. Assigned by requirements */
#define STDIO_LED_RZ_HLD_UID                (65)

/** LED Control codes */
typedef enum
{
    CTL_SET_LED_ON = (STDIO_LED_RZ_HLD_UID << 8), /*!< Turn led on */
    CTL_SET_LED_OFF,                              /*!< Turn led off */
    CTL_SET_LED_TOGGLE,                           /*!< Toggle the current state of the led */
    CTL_GET_LED_MAP,                              /*!< Read the state of all 16 leds as a bitfield (1 = ON, 0 = OFF) */
    CTL_SET_LED_MAP,                              /*!< Write the state of all 16 leds as a bitfield (1 = ON, 0 = OFF) */
} e_ctrl_code_led_t;

/** Common id descriptors for specifying led's support in low layer drivers */
typedef enum
{
    LED0  = (0x01 <<  0),  /*!< Reference led 0 of associated driver */
    LED1  = (0x01 <<  1),  /*!< Reference led 1 of associated driver */
    LED2  = (0x01 <<  2),  /*!< Reference led 2 of associated driver */
    LED3  = (0x01 <<  3),  /*!< Reference led 3 of associated driver */
    LED4  = (0x01 <<  4),  /*!< Reference led 4 of associated driver */
    LED5  = (0x01 <<  5),  /*!< Reference led 5 of associated driver */
    LED6  = (0x01 <<  6),  /*!< Reference led 6 of associated driver */
    LED7  = (0x01 <<  7),  /*!< Reference led 7 of associated driver */
    LED8  = (0x01 <<  8),  /*!< Reference led 8 of associated driver */
    LED9  = (0x01 <<  9),  /*!< Reference led 9 of associated driver */
    LED10 = (0x01 << 10),  /*!< Reference led 10 of associated driver */
    LED11 = (0x01 << 11),  /*!< Reference led 11 of associated driver */
    LED12 = (0x01 << 12),  /*!< Reference led 12 of associated driver */
    LED13 = (0x01 << 13),  /*!< Reference led 13 of associated driver */
    LED14 = (0x01 << 14),  /*!< Reference led 14 of associated driver */
    LED15 = (0x01 << 15),  /*!< Reference led 15 of associated driver */
} e_led_id_t;


/******************************************************************************
 Constant Data
 ******************************************************************************/

/**
 * @var g_stdio_led_driver
 * <b>Driver Implementation:</b> <BR>
 * \par
 * \"LED Device Driver\" - Driver Name <BR>
 * \c led_open - Opens the LED Driver <BR>
 * \c led_close - Closes the LED Driver <BR>
 * \c led_read - Read LED status directly from pin <BR>
 * write is not supported <BR>
 * \c led_control - see @ref e_ctrl_code_led_t for commands <BR>
 * \c led_get_version - Get driver version <BR>
 */
extern const st_r_driver_t g_stdio_led_driver;

#endif /* SRC_RENESAS_DRIVERS_R_LED_INC_R_LED_DRV_API_H_ */
/**************************************************************************//**
 * @} (end addtogroup)
 *****************************************************************************/
/*******************************************************************************************************************//**
  End  Of File
 ***********************************************************************************************************************/
