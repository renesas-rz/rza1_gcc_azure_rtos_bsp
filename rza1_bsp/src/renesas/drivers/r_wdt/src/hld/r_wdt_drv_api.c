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
* File Name    : drvTimer.c
* Version      : 1.00
* Device(s)    : Renesas
* Tool-Chain   : GNUARM-NONE-EABI v14.02
* OS           : FreeRTOS
* H/W Platform : RSK+
* Description  : Timer device driver
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

#include <r_stream_it_wdt_lld.h>
#include <r_wdt_drv_api.h>
#include "r_typedefs.h"
#include "iodefine_cfg.h"
#include "r_devlink_wrapper.h"
#include "trace.h"

/* Adding this dependency ensures that this file is built every time because of the build counter */
#include "version.h"

/* Comment this line out to turn ON module trace in this file */
#undef _TRACE_ON_

#ifndef _TRACE_ON_
#undef TRACE
#define TRACE(x)
#endif

#define	PERIPHERAL_CLOCK0_HZ              ( 32000000UL )

static int_t wdt_open(st_stream_ptr_t pStream);
static void  wdt_close(st_stream_ptr_t pStream);
static int_t wdt_control(st_stream_ptr_t pStream, uint32_t ctlCode, void *pCtlStruct);
static int_t wdt_get_version(st_stream_ptr_t pStream, st_ver_info_t *pVerInfo);

static const st_drv_info_t gs_hld_info =
{
    {
        ((STDIO_WDT_RZ_HLD_VERSION_MAJOR << 16) + STDIO_WDT_RZ_HLD_VERSION_MINOR)
    },

    STDIO_WDT_RZ_HLD_BUILD_NUM,
    STDIO_WDT_RZ_HLD_DRV_NAME
};


/* Define the driver function table for this */
const st_r_driver_t g_wdt_driver =
{
    "Timer Device Driver",
    wdt_open,
    wdt_close,
    no_dev_io,
    no_dev_io,
    wdt_control,
    wdt_get_version
};

/*****************************************************************************
Private Functions
******************************************************************************/

/******************************************************************************
Function Name: wdt_open
Description:   Function to open the system timer device driver
Arguments:     IN  pStream - Pointer to the file stream
Return value:  0 for success otherwise -1
******************************************************************************/
static int_t wdt_open(st_stream_ptr_t pStream)
{
    /* No open function is required as this is done for the initialisation
       of FreeRTOS */
    (void)pStream;

    return 0;
}
/******************************************************************************
End of function  wdt_open
******************************************************************************/

/******************************************************************************
Function Name: wdt_close
Description:   Function to close the standard IN driver. All open devices
               connected to the tmr are closed.
Arguments:     IN  pStream - Pointer to the file stream
Return value:  none
******************************************************************************/
static void wdt_close(st_stream_ptr_t pStream)
{
    /* Casting to avoid compiler warning */
    (void)pStream;

}
/******************************************************************************
End of function  wdt_close
******************************************************************************/

/******************************************************************************
Function Name: wdt_control
Description:   Function to handle custom controls for the standard IN device
Arguments:     IN  pStream - Pointer to the file stream
               IN  ctlCode - The custom control code
               IN  pCtlStruct - Pointer to the custom control structure
Return value:  0 for success -1 on error
******************************************************************************/
static int_t wdt_control(st_stream_ptr_t pStream, uint32_t ctlCode, void *pCtlStruct)
{
    int8_t result = -1;

    /* Avoid unused parameter compiler warning */
    (void)pStream;
    if (pCtlStruct)
    {
        switch (ctlCode)
        {
            case CTL_GET_TIME_STAMP:
            {
                /* Casting the CtrlStructue for use of wdt_control structure */
                PTMSTMP     perf_timer = (PTMSTMP)pCtlStruct;

                /* Cast from the iodefine file */
                uint32_t    time_now = OSTM1.OSTMnCNT;
                perf_timer->ulMilisecond = time_now / (PERIPHERAL_CLOCK0_HZ / 1000);
                perf_timer->usSubMilisecond = (uint16_t)(time_now % (PERIPHERAL_CLOCK0_HZ / 1000));
                result = 0;
                break;
            }
            case CTL_WDT_RESET:
            {
                wdt_hardware_reset();
                break;
            }
            default:
            {
                TRACE(("drvTimer: Unknown control code\r\n"));
                break;
            }
        }
    }
    return result;
}
/******************************************************************************
End of function  wdt_control
******************************************************************************/

/*******************************************************************************
 Function Name: wdt_get_version
 Description  : Provides build information even if driver fails to open
                version information is updated by developer
 Arguments    : none
 Return Value : 0 (never fails)
 ******************************************************************************/
static int_t wdt_get_version(st_stream_ptr_t pStream, st_ver_info_t *pVerInfo)
{
    (void) pStream;

    pVerInfo->hld.version.sub.major = gs_hld_info.version.sub.major;
    pVerInfo->hld.version.sub.minor = gs_hld_info.version.sub.minor;
    pVerInfo->hld.build = gs_hld_info.build;
    pVerInfo->hld.p_szdriver_name = gs_hld_info.p_szdriver_name;

    /* Obtain version information from Low layer Driver */
    /* R_RTC_GetVersion(pVerInfo); */

    return 0;
}
/*******************************************************************************
 End of function wdt_get_version
 ******************************************************************************/

/******************************************************************************
End  Of File
******************************************************************************/
