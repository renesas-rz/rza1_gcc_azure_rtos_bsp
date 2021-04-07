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
 * @headerfile     application_cfg.h
 * @brief          Overall control of system state
 * @version        1.00
 * @date           24.04.2019
 * H/W Platform    Renesas Starter Kit+ for RZ/A1H
 *****************************************************************************/
/*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 30.06.2018 1.00 First Release
 *              : 24.04.2019 2.00 Porting for RZ/A1H
 *****************************************************************************/

/* Multiple inclusion prevention macro */
#ifndef RENESAS_CONFIGURATION_APPLICATION_CFG_H_
#define RENESAS_CONFIGURATION_APPLICATION_CFG_H_

/******************************************************************************
  WARNING!  IN ACCORDANCE WITH THE USER LICENCE THIS CODE MUST NOT BE CONVEYED
  OR REDISTRIBUTED IN COMBINATION WITH ANY SOFTWARE LICENSED UNDER TERMS THE
  SAME AS OR SIMILAR TO THE GNU GENERAL PUBLIC LICENCE
******************************************************************************/

/* Common function controls */
/** Define to show an option is enabled */
#define R_OPTION_ENABLE  (1)

/** Text to show an option is enabled */
#define R_MSG_ENABLED  ("ENABLED")

/** Define to show an option is disabled */
#define R_OPTION_DISABLE (0)

/** Text to show an option is disabled */
#define R_MSG_DISABLED ("DISABLED")

/** OS or NON-OS Memory Pool (in bytes) Used in GCC variant only  */
#define R_TOTAL_MEMORY_AVAILABLE (1600 * 1024)

/** CMSIS OPTIONS Required by ARM SMSYS module */
#define __FPU_PRESENT (R_OPTION_ENABLE)

/** Enable Trace Macro here allows individual source files to enable Trace macro.
 * Removing this define disables trace macro for all files regardless of file setting */
#define _TRACE_ON_

/** Enable support for stdio.h in application  */
#define R_USE_ANSI_STDIO_MODE_CFG (R_OPTION_ENABLE)

/** Enable blink LED task in main.c */
#define R_SELF_BLINK_TASK_CREATION (R_OPTION_ENABLE)

/** Enable Ethernet drivers, WebServer Support  */
#define R_SELF_LOAD_MIDDLEWARE_ETHERNET_MODULES (R_OPTION_DISABLE)

/** Enable control for src/application/app_adc sample application */
#define R_SELF_INSERT_APP_ADC (R_OPTION_DISABLE)

/** Enable PMOD RPSI src/application/app_pmod sample application */
#define R_SELF_INSERT_APP_PMOD (R_OPTION_DISABLE)

/** Enable control for src/application/app_sound sample application */
#define R_SELF_INSERT_APP_SOUND (R_OPTION_DISABLE)

/** Enable control for src/application/app_touchscreen sample application */
#define R_SELF_INSERT_APP_TOUCH_SCREEN (R_OPTION_DISABLE)

/** Enable control for src/application/app_sdk_camera sample application */
#define R_SELF_INSERT_APP_SDK_CAMERA (R_OPTION_DISABLE)

/** Enable control for src/application/app_gui sample application */
#define R_SELF_INSERT_APP_GUI (R_OPTION_DISABLE)

/** Enable control to load /src/renesas/middleware/usb_host_controller */
#define R_SELF_LOAD_MIDDLEWARE_USB_HOST_CONTROLLER (R_OPTION_DISABLE)

/** Enable file system support when R_SELF_LOAD_MIDDLEWARE_USB_HOST_CONTROLLER is enabled */
#if R_SELF_LOAD_MIDDLEWARE_USB_HOST_CONTROLLER

/** Enable file system support if R_SELF_LOAD_MIDDLEWARE_USB_HOST_CONTROLLER is enabled */
#define INCLUDE_FILE_SYSTEM (R_OPTION_ENABLE)

/** Enable USB CDC Control via console if R_SELF_LOAD_MIDDLEWARE_USB_HOST_CONTROLLER is enabled */
#define R_SELF_INSERT_APP_HOST_CDC_CONSOLE (R_OPTION_ENABLE)
#endif /* R_SELF_LOAD_MIDDLEWARE_USB_HOST_CONTROLLER */

/** Enable control for src/application/app_hid_mouse application */
#define R_SELF_INSERT_APP_HID_MOUSE (R_OPTION_DISABLE)

/** Enable control for src/application/app_cdc_serial_port application */
#define R_SELF_INSERT_APP_CDC_SERIAL_PORT (R_OPTION_DISABLE)

#if R_SELF_INSERT_APP_CDC_SERIAL_PORT

/** Configure driver mode when R_SELF_INSERT_APP_CDC_SERIAL_PORT is enabled
 * R_SELF_APP_CDC_ASYNC_MODE (R_OPTION_ENABLE)  = I/O in this mode is non-blocking
 * R_SELF_APP_CDC_ASYNC_MODE (R_OPTION_DISABLE) = I/O in this mode is blocking */
    #define R_SELF_APP_CDC_ASYNC_ENABLE (R_OPTION_ENABLE)
#endif

#endif /* RENESAS_CONFIGURATION_APPLICATION_CFG_H_ */
