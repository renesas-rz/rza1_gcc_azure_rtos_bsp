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
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************
* File Name    : r_rtc_drv_api.c
* Version      : 1.0
* Description  : Software emulation of a real time clock
*******************************************************************************
* History      : DD.MM.YYYY Version Description
*              : 23.02.2018 1.00    First Release
******************************************************************************/

/******************************************************************************
  WARNING!  IN ACCORDANCE WITH THE USER LICENCE THIS CODE MUST NOT BE CONVEYED
  OR REDISTRIBUTED IN COMBINATION WITH ANY SOFTWARE LICENSED UNDER TERMS THE
  SAME AS OR SIMILAR TO THE GNU GENERAL PUBLIC LICENCE
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/

#include <string.h>
#include <stdio.h>

#include "r_rtc_drv_api.h"
#include "r_devlink_wrapper.h"
#include "r_os_abstraction_api.h"
#include "trace.h"
#include "r_rskrza1h_rtc_lld.h"

/* Adding this dependency ensures that this file is built every time because of the build counter */
#include "version.h"

/******************************************************************************
Macro definitions
******************************************************************************/

/* Comment this line out to turn ON module trace in this file */
#undef _TRACE_ON_

#ifndef _TRACE_ON_
#undef TRACE
#define TRACE(x)
#endif

/******************************************************************************
Private global variables and functions
******************************************************************************/

static int_t rtc_open(st_stream_ptr_t pStream);
static void rtc_close(st_stream_ptr_t pStream);
static int_t rtc_control(st_stream_ptr_t pStream, uint32_t ctlCode, void *ctrl_ptr);
static int_t rtc_get_version(st_stream_ptr_t pStream, st_ver_info_t *pVerInfo);

static void rtc_get_date (PDATE date);
static bool_t rtc_set_date(DATE Date);

static const st_drv_info_t gs_hld_info =
{
    {
        ((STDIO_RTC_RZ_HLD_VERSION_MAJOR << 16) + STDIO_RTC_RZ_HLD_VERSION_MINOR)
    },

    STDIO_RTC_RZ_HLD_BUILD_NUM,
    STDIO_RTC_RZ_HLD_DRV_NAME
};

const st_r_driver_t g_rtc_driver =
{
    "RTC Device Driver",
    rtc_open,
    rtc_close,
    no_dev_io,
    no_dev_io,
    rtc_control,
    rtc_get_version
};

/******************************************************************************
 Public Functions
 ******************************************************************************/

/******************************************************************************
Private Functions
******************************************************************************/

/******************************************************************************
* Function Name: rtc_open
* Description  : Function to open the emulated RTC
* Arguments    : IN pStream - Pointer to the file stream
* Return Value : 0 for success otherwise -1
******************************************************************************/
static int_t rtc_open(st_stream_ptr_t pStream)
{
    /* Avoid unused parameter compiler warning */
    (void) pStream;
    static bool_t first_open = true;

    if (first_open)
    {
        R_RTC_Init();

        char_t month_string[16];
        char_t delim;
        static const char_t * const p_month[] =
        {
            "---", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
        };
        int8_t count = (sizeof(p_month)) / sizeof(char_t *);
        int_t hour;
        int_t minute;
        int_t second;
        int_t year;
        int_t month = 0;
        int_t date;

        /* Initialise the time from the build time of this file */
        sscanf(__TIME__, "%d%c%d%c%d", &hour, &delim, &minute, &delim, &second);
        sscanf(__DATE__, "%3s%d%d", month_string, &date, &year);

        /* Look up the month */
        while (count--)
        {
            if (strstr(month_string, p_month[count]))
            {
                month = count;
                break;
            }
        }

        R_RTC_SetDateTime((uint16_t) year, (uint8_t) month, (uint8_t) date, (uint8_t) hour, (uint8_t) minute, (uint8_t) second, 0);

        first_open = false;
    }

    return 0;
}
/******************************************************************************
 End of function rtc_open
 ******************************************************************************/

/******************************************************************************
 Function Name: rtc_close
 Description  : Function to close the SCIF
 Arguments    : IN pStream - Pointer to the file stream
 Return Value : none
 ******************************************************************************/
static void rtc_close(st_stream_ptr_t pStream)
{
    /* Avoid unused parameter compiler warning */
    (void) pStream;
}
/******************************************************************************
 End of function rtc_close
 ******************************************************************************/

/******************************************************************************
 Function Name: rtc_control
 Description  : Function to handle custom control functions for SCIF
 Arguments    : IN  pStream - Pointer to the file stream
                IN  ctlCode - The custom control code
                IN  pCtlStruct - Pointer to the custom control structure
 Return Value : 0 for success -1 on error
 ******************************************************************************/
static int_t rtc_control(st_stream_ptr_t pStream, uint32_t ctlCode, void *pCtlStruct)
{
    /* Avoid unused parameter compiler warning */
    (void) pStream;

    switch (ctlCode)
    {
        case CTL_GET_DATE:
        {
            if (pCtlStruct)
            {
                /* Return the current date */
                rtc_get_date((PDATE)pCtlStruct);
                return 0;
            }

            return -1;
        }

        case CTL_SET_DATE:
        {
            if (pCtlStruct)
            {
                /* Set the date */
                if (rtc_set_date(*((PDATE) pCtlStruct)))
                {
                    return 0;
                }
            }

            return -1;
        }

        default:
        {
            TRACE(("drvRtc: Unknown control code\r\n"));
            break;
        }
    }

    return -1;
}
/******************************************************************************
 End of function rtc_control
 ******************************************************************************/

/*******************************************************************************
 Function Name: rtc_get_version
 Description  : Provides build information even if driver fails to open
                version information is updated by developer
 Arguments    : none
 Return Value : 0 (never fails)
 ******************************************************************************/
static int_t rtc_get_version(st_stream_ptr_t pStream, st_ver_info_t *pVerInfo)
{
    (void) pStream;

    pVerInfo->hld.version.sub.major = gs_hld_info.version.sub.major;
    pVerInfo->hld.version.sub.minor = gs_hld_info.version.sub.minor;
    pVerInfo->hld.build = gs_hld_info.build;
    pVerInfo->hld.p_szdriver_name = gs_hld_info.p_szdriver_name;

    /* Obtain version information from Low layer Driver */
    R_RTC_GetVersion(pVerInfo);

    return 0;
}
/*******************************************************************************
 End of function rtc_get_version
 ******************************************************************************/

/******************************************************************************
 Function Name: rtc_calc_week_day
 Description  : Function to calculate the week day for a given date from 1753
                onwards based on Tomohiko Sakamoto's algorithm
 Arguments    : IN  iDay - The day 1..31
                IN  iMonth - The month 1..12
                IN  iYear - The year (>= 1753)
 Return Value : The weekday 0 = Monday ... 6 = Sunday
 ******************************************************************************/
static uint8_t rtc_calc_week_day(uint8_t day, uint8_t month, uint16_t year)
{
    static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
	uint8_t day_of_week;

	if (month < 3)
    {
        year--;
    }

    day_of_week = (year + (year / 4) - (year / 100) + (year / 400) + t[month - 1] + day + 6) % 7;

    return (day_of_week);
}
/******************************************************************************
 End of function rtc_calc_week_day
 ******************************************************************************/

/******************************************************************************
 Function Name: rtc_get_date
 Description  : Function to get the current date
 Arguments    : DATE structure, provided by caller
 Return Value : The current date
 ******************************************************************************/
static void rtc_get_date (PDATE date)
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t week_day;

    R_RTC_GetDateTime(&year, &month, &day, &hour, &minute, &second, &week_day);

    date->Field.Year = year;
    date->Field.Month = (unsigned) (month & 0xf);
    date->Field.Day = (unsigned) (day & 0x1f);
    date->Field.Hour = (unsigned) (hour & 0x1f);
    date->Field.Minute = (unsigned) (minute & 0x3f);
    date->Field.Second = (unsigned) (second & 0x3f);
    date->Field.WeekDay = (unsigned) (rtc_calc_week_day(day, month, year) & 0x7);

    return;
}
/******************************************************************************
 End of function rtc_get_date
 ******************************************************************************/

/******************************************************************************
 Function Name: rtc_set_date
 Description  : Function to set the date
 Arguments    : IN  Date - The date to set
 Return Value : TRUE if the date was set
 ******************************************************************************/
static bool_t rtc_set_date(DATE date)
{
    R_RTC_SetDateTime((uint16_t) date.Field.Year, date.Field.Month, date.Field.Day, date.Field.Hour,
					  date.Field.Minute, date.Field.Second, date.Field.WeekDay);

    return true;
}
/******************************************************************************
 End of function rtc_set_date
 ******************************************************************************/

/******************************************************************************
 End  Of File
 ******************************************************************************/
