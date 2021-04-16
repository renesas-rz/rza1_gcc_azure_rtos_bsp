/******************************************************************************
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
 * and to discontinue the availability of this software. By using this software,
 * you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *******************************************************************************
 * Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.
 *******************************************************************************
 * File Name    : drvLed.c
 * Version      : 1.00
 * Device(s)    : Renesas
 * Tool-Chain   : GNUARM-NONE-EABI v14.02
 * OS           : FreeRTOS
 * H/W Platform : RSK+
 * Description  : LED device driver
 *******************************************************************************
 * History      : DD.MM.YYYY Version Description

 *              : 04.02.2010 1.00    First Release
 ******************************************************************************/

/******************************************************************************
 WARNING!  IN ACCORDANCE WITH THE USER LICENCE THIS CODE MUST NOT BE CONVEYED
 OR REDISTRIBUTED IN COMBINATION WITH ANY SOFTWARE LICENSED UNDER TERMS THE
 SAME AS OR SIMILAR TO THE GNU GENERAL PUBLIC LICENCE
 ******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

#include "r_led_drv_api.h"
#include "r_os_abstraction_api.h"
#include "dev_drv.h"

/******************************************************************************
 Macro definitions
 ******************************************************************************/

/* Comment this line out to turn ON module trace in this file */
#undef _TRACE_ON_

#ifndef _TRACE_ON_
#undef DEBUG_PRV_TRACE
#define  DEBUG_PRV_TRACE(x)
#endif

/******************************************************************************
 Enumerated Types
 ******************************************************************************/

/******************************************************************************
 Typedefs
 ******************************************************************************/
typedef struct r_drvled
{
    bool_t   b_open;
    uint32_t instance;
    int32_t  i_led_map;
} st_r_drvled_t;

/*****************************************************************************
 Function Prototypes
 ******************************************************************************/

static int_t led_open(st_stream_ptr_t pStream);
static void  led_close(st_stream_ptr_t pStream);
static int_t led_read(st_stream_ptr_t pStream, uint8_t *pbyBuffer, uint32_t uiCount);
static int_t led_control(st_stream_ptr_t pStream, uint32_t ctrl_code, void *ctrl_ptr);
static int_t led_get_version(st_stream_ptr_t pStream, st_ver_info_t *pVerInfo);

/*****************************************************************************
 Constant Data
 ******************************************************************************/

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/

static st_r_drvled_t gs_r_drv_led =
{ false, 0, 0 };

static const st_drv_info_t gs_hld_info =
{
{ ((STDIO_LED_RZ_HLD_VERSION_MAJOR << 16) + STDIO_LED_RZ_HLD_VERSION_MINOR) },
  STDIO_LED_RZ_HLD_BUILD_NUM,
  STDIO_LED_RZ_HLD_DRV_NAME };

/* Define the driver function table for this implementation */
/* GSCE Warning (global variable 'g_r_led_fulldriver' not declared static [9.3a])
 * can be safely ignored */
const st_r_driver_t g_stdio_led_driver =
{
    "LED Device Driver",
    led_open,
    led_close,
    led_read,
    no_dev_io,
    led_control,
    led_get_version
};

/******************************************************************************
 Function Name: led_open
 Description:   Function to open the system timer device driver
 Arguments:     IN  pStream - Pointer to the file stream
 Return value:  0 for success otherwise -1
 ******************************************************************************/
static int_t led_open(st_stream_ptr_t pStream)
{
    /* unused variable */
    (void) pStream;

    if (!gs_r_drv_led.b_open)
    {
        gs_r_drv_led.b_open = true;
        gs_r_drv_led.instance++;
        R_LED_InitialiseHwIf ();
    }
    else
    {
        gs_r_drv_led.instance++;
    }


    /*  */
    return 0;
}
/******************************************************************************
 End of function  led_open
 ******************************************************************************/

/******************************************************************************
 Function Name: led_close
 Description:   Function to close the standard IN driver. All open devices
 connected to the led are closed.
 Arguments:     IN  pStream - Pointer to the file stream
 Return value:  none
 ******************************************************************************/
static void led_close(st_stream_ptr_t pStream)
{
    /* unused variable */
    (void) pStream;
    if ((gs_r_drv_led.b_open) && (0 == gs_r_drv_led.instance))
    {
        gs_r_drv_led.b_open = false;
    }
    else
    {
        gs_r_drv_led.instance--;
    }
}
/******************************************************************************
 End of function  led_close
 ******************************************************************************/

/******************************************************************************
 Function Name: led_read
 Description:   Function to read the current state of the single led
 If this driver supported multiple LED's either
 create new LLDriver (sharing the same HLDriver)
 OR
 use control function and pass the LED to read &
 the data store for the value in ctrl_ptr
 ie
 struct r_led_st
 {
 uint32_t led;            // LED to read
 uint32_t current_state;  // status of LED
 }

 Arguments:     IN  pStream - Pointer to the file stream
 IN  uiCount - Buffer size
 OUT *pbyBuffer - data
 Return value:  0 for success otherwise -1
 ******************************************************************************/
static int_t led_read(st_stream_ptr_t pStream, uint8_t *pbyBuffer, uint32_t uiCount)
{
    (void) pStream;
    (void) pbyBuffer;
    (void) uiCount;

    return DEVDRV_ERROR;
}
/*******************************************************************************
 End of function led_read
 ******************************************************************************/

/******************************************************************************
 Function Name: led_control
 Description:   Function to handle custom controls for the standard IN device
 Arguments:     IN  pStream - Pointer to the file stream
 IN  ctrl_code - The custom control code
 IN  ctrl_ptr - Pointer to the custom control structure
 Return value:  0 for success -1 on error
 ******************************************************************************/
static int_t led_control(st_stream_ptr_t pStream, uint32_t ctrl_code, void *ctrl_ptr)
{
    /*  */
    static event_t led_mutex = NULL;
    int_t retval = -1;
    _Bool check_mutex_restricted_control = false;

    /* Avoid unused parameter compiler warning */
    (void) pStream;

    if (ctrl_ptr)
    {
        /* The following controls allow non-exclusive access */
        switch (ctrl_code)
        {
            case CTL_GET_VERSION:
            {
                retval = led_get_version (pStream, ctrl_ptr);
                break;
            }
            default:
            {
                check_mutex_restricted_control = true;
                break;
            }
        }

        /* The following controls require exclusive access to resource */
        if (check_mutex_restricted_control)
        {
            R_OS_EventWaitMutex (&led_mutex, R_OS_ABSTRACTION_PRV_EV_WAIT_INFINITE);
            switch (ctrl_code)
            {
                case CTL_SET_LED_ON:
                {
                    /* Casting the pointer to a uint32_t type is valid because even
                     * though the Renesas Compiler uses 4 bytes per pointer, this
                     * pointer will never reference memory over 0xFFFFFFFF. */
                    int32_t i_led_map = (*(int32_t*) ctrl_ptr);
                    gs_r_drv_led.i_led_map &= (~i_led_map);
                    retval = R_LED_Update (gs_r_drv_led.i_led_map);
                    break;
                }
                case CTL_SET_LED_OFF:
                {
                    /* Casting the pointer to a uint32_t type is valid because even
                     * though the Renesas Compiler uses 4 bytes per pointer, this
                     * pointer will never reference memory over 0xFFFFFFFF. */
                    int32_t i_led_map = (*(int32_t*) ctrl_ptr);
                    gs_r_drv_led.i_led_map |= i_led_map;
                    retval = R_LED_Update (gs_r_drv_led.i_led_map);
                    break;
                }
                case CTL_SET_LED_TOGGLE:
                {
                    /* Casting the pointer to a uint32_t type is valid because even
                     * though the Renesas Compiler uses 8 bytes per pointer, this
                     * pointer will never reference memory over 0xFFFFFFFF. */
                    int32_t i_led_map = (*(int32_t*) ctrl_ptr);
                    gs_r_drv_led.i_led_map ^= i_led_map;
                    retval = R_LED_Update (gs_r_drv_led.i_led_map);
                    break;
                }
                case CTL_SET_LED_MAP:
                {
                    /* Casting the pointer to a uint32_t type is valid because even
                     * though the Renesas Compiler uses 8 bytes per pointer, this
                     * pointer will never reference memory over 0xFFFFFFFF. */
                    gs_r_drv_led.i_led_map = (*(int32_t*) ctrl_ptr);
                    retval = R_LED_Update (gs_r_drv_led.i_led_map);
                    break;
                }
                case CTL_GET_LED_MAP:
                {
                    if (ctrl_ptr)
                    {
                        /* Casting the pointer to a uint32_t type is valid because even
                         * though the Renesas Compiler uses 8 bytes per pointer, this
                         * pointer will never reference memory over 0xFFFFFFFF. */
                        (*(int32_t*) ctrl_ptr) = gs_r_drv_led.i_led_map;
                        retval = 0;
                    }
                    break;
                }
                default:
                {
                    DEBUG_PRV_TRACE(("drvLed: Unknown control code\r\n"));
                    break;
                }
            }

            R_OS_EventReleaseMutex (&led_mutex);
        }
    }
    return retval;
}
/******************************************************************************
 End of function  led_control
 ******************************************************************************/

/*******************************************************************************
 * Function Name: led_get_version
 * Description  : Provides build information even if driver fails to open
 *                version information is updated by developer
 * Arguments    : none
 * Return Value : 0 (never fails)
 ******************************************************************************/
static int_t led_get_version(st_stream_ptr_t pStream, st_ver_info_t *pVerInfo)
{
    (void) pStream;

    pVerInfo->hld.version.sub.major = gs_hld_info.version.sub.major;
    pVerInfo->hld.version.sub.minor = gs_hld_info.version.sub.minor;
    pVerInfo->hld.build = gs_hld_info.build;
    pVerInfo->hld.p_szdriver_name = gs_hld_info.p_szdriver_name;

    /* Obtain version information from Low layer Driver */
    R_LED_GetVersion (pVerInfo);

    return 0;
}
/*******************************************************************************
 End of function led_get_version
 ******************************************************************************/

/******************************************************************************
 End  Of File
 ******************************************************************************/

