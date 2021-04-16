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
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name     : r_rskrza1h_rtc_lld.c
* Device(s)     : RZ/A1H (R7S721001)
* Tool-Chain    : GNUARM-NONEv14.02-EABI
* H/W Platform  : RSK+RZA1H CPU Board
* Description   : RTC low level driver
*******************************************************************************/
/*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 21.10.2014 1.00
*******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#if ((TARGET_RZA1 == TARGET_RZA1H) || (TARGET_RZA1 == TARGET_RZA1M))
#include <stdio.h>

#include "r_typedefs.h"             /* Default type definition header */
#include "iodefine_cfg.h"            /* I/O Register root header */
#include "dev_drv.h"                /* Device Driver common header */
#include "r_intc.h"                    /* INTC Driver Header */
#include "r_rskrza1h_rtc_lld.h"    /* RTC interface header */


/******************************************************************************
 Macro definitions
 ******************************************************************************/

/* Comment this line out to turn ON module trace in this file */
#undef _TRACE_ON_

#ifndef _TRACE_ON_
#undef TRACE
#define TRACE_PRV_(x)
#endif

static const st_drv_info_t gs_lld_info =
{
    {
        ((STDIO_RTC_RZ_LLD_VERSION_MAJOR << 16) + STDIO_RTC_RZ_LLD_VERSION_MINOR)
    },
    STDIO_RTC_RZ_LLD_BUILD_NUM,
    STDIO_RTC_RZ_LLD_DRV_NAME
};

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/

static uint32_t bcd_to_integer(uint32_t bcd, int8_t digits);
static uint32_t integer_to_bcd(uint32_t value, uint8_t digits);

/*******************************************************************************
 Function Name: bcd_to_integer
 Description  : convert BCD value to integer
 Arguments    : uint16_t - BCD value
                int8_t digits - the number of digits to convert (2 to 8)
 Return Value : uint32_t value - the integer value
 ******************************************************************************/
static uint32_t bcd_to_integer(uint32_t bcd, int8_t digits)
{
    uint32_t multiplier;
    uint32_t temp;
    uint32_t result;
    uint8_t digit;

    result = 0;
    multiplier = 1;

    for (digit = 0; digit < digits; digit++)
    {
        temp = bcd & 0xf;
        temp *= multiplier;
        result += temp;
        bcd >>= 4;
        multiplier *= 10;
    }

    return result;
}
/*******************************************************************************
 End of function bcd_to_integer
 ******************************************************************************/

/*******************************************************************************
 * Function Name: integer_to_bcd
 * Description  : convert integer to BCD
 * Arguments    : uint32_t value - value
 *                uint8_t digits - number of digits (2 to 8)
 * Return Value : uint32_t - BCD value
 ******************************************************************************/
static uint32_t integer_to_bcd(uint32_t value, uint8_t digits)
{
    uint32_t bcd;
    uint8_t digit;
    uint32_t temp;

    bcd = 0;

    for (digit = 0; digit < digits; digit++)
    {
        temp = value % 10;
        temp <<= (4 * digit);
        bcd |= temp;
        value /= 10;
    }

    return bcd;
}
/*******************************************************************************
 End of function integer_to_bcd
 ******************************************************************************/

/******************************************************************************
 Function Name: R_RTC_Init
 Description  : Initialises the RTC
 Arguments    : none
 Return Value : none
 ******************************************************************************/
void R_RTC_Init (void)
{
    uint8_t dummy_read;

    /* avoid unused variable warning */
    UNUSED_VARIABLE(dummy_read);

    /* Two dummy reads are added after writing certain registers to
       ensure the third read will return the correct register value */

    /* Stop the clock, select input clock, reset divider circuit */
    RTC.RCR2 &= 0x08;
    dummy_read = RTC.RCR2;
    dummy_read = RTC.RCR2;

    RTC.RCR2 |= 0xD8;
    dummy_read = RTC.RCR2;
    dummy_read = RTC.RCR2;

    /* RTC_X1 (32.768KHz) is selected by default */
    RTC.RCR5 = 0;
}
/*******************************************************************************
 End of function R_RTC_Init
 ******************************************************************************/

/******************************************************************************
 Function Name: R_RTC_SetDateTime
 Description  : Sets the date and time
 Arguments    : uint16_t year
                uint8_t month
                uint8_t date
                uint8_t hour
                uint8_t minute
                uint8_t second
                uint8_t day_of_week
 Return Value : none
 ******************************************************************************/
void R_RTC_SetDateTime (uint16_t year, uint8_t month, uint8_t date, uint8_t hour,
                        uint8_t minute, uint8_t second, uint8_t day_of_week)
{
    uint8_t dummy_read;

    /* avoid unused variable warning */
    UNUSED_VARIABLE(dummy_read);

    R_RTC_StartStop(0);

    /* cast to protect hardware data width RHRCNT */
    RTC.RHRCNT  = (uint8_t)integer_to_bcd(hour, 2);

    /* cast to protect hardware data width RMINCNT */
    RTC.RMINCNT = (uint8_t)integer_to_bcd(minute, 2);

    /* cast to protect hardware data width RSECCNT */
    RTC.RSECCNT = (uint8_t)integer_to_bcd(second, 2);

    /* cast to protect hardware data width RMONCNT */
    RTC.RMONCNT = (uint8_t)integer_to_bcd(month, 2);

    /* cast to protect hardware data width RDAYCNT */
    RTC.RDAYCNT = (uint8_t)integer_to_bcd(date, 2);

    /* cast to protect hardware data width RYRCNT */
    RTC.RYRCNT  = (uint16_t)integer_to_bcd(year, 4);

    RTC.RWKCNT = day_of_week;

    /* two dummy reads to ensure that the correct value is read next time */
    dummy_read = RTC.RHRCNT;
    dummy_read = RTC.RHRCNT;
    dummy_read = RTC.RMINCNT;
    dummy_read = RTC.RMINCNT;
    dummy_read = RTC.RSECCNT;
    dummy_read = RTC.RSECCNT;
    dummy_read = RTC.RYRCNT;
    dummy_read = RTC.RYRCNT;
    dummy_read = RTC.RMONCNT;
    dummy_read = RTC.RMONCNT;
    dummy_read = RTC.RDAYCNT;
    dummy_read = RTC.RDAYCNT;
    dummy_read = RTC.RWKCNT;
    dummy_read = RTC.RWKCNT;

    R_RTC_StartStop(1);
}
/******************************************************************************
 End of function R_RTC_SetDateTime
 ******************************************************************************/

/******************************************************************************
 Function Name: R_RTC_GetDateTime
 Description  : Gets the date and time
 Arguments    : uint16_t year
                uint8_t month
                uint8_t date
                uint8_t hour
                uint8_t minute
                uint8_t second
                uint8_t day_of_week
 Return Value : none
 ******************************************************************************/
void R_RTC_GetDateTime (uint16_t *year, uint8_t *month, uint8_t *date, uint8_t *hour, uint8_t *minute, uint8_t *second, uint8_t *day_of_week)
{
    do
    {
        RTC.RCR1 &= 0x7f;                            /* clear the carry flag */

        *hour = (uint8_t) bcd_to_integer(RTC.RHRCNT, 2);
        *minute = (uint8_t) bcd_to_integer(RTC.RMINCNT, 2);
        *second = (uint8_t) bcd_to_integer(RTC.RSECCNT, 2);
        uint16_t bcd_year = RTC.RYRCNT;
        *year = (uint16_t) bcd_to_integer((uint32_t) bcd_year, 4);
        *month = (uint8_t) bcd_to_integer(RTC.RMONCNT, 2);
        *date = (uint8_t) bcd_to_integer(RTC.RDAYCNT, 2);

        *day_of_week = RTC.RWKCNT;
    }
    while ((RTC.RCR1 & 0x80) != 0);                    /* if carry flag is set, then read again */
}
/******************************************************************************
 End of function R_RTC_GetDateTime
 ******************************************************************************/

/******************************************************************************
 Function Name: R_RTC_StartStop
 Description  : Starts or stops the RTC clock
 Arguments    : uint8_t start - non-zero to start the clock, zero to stop it
 Return Value : none
 ******************************************************************************/
void R_RTC_StartStop(uint8_t start)
{
    uint8_t dummy_read;

    /* dummy read the register twice to ensure that the correct value is read */
    dummy_read =  RTC.RCR2;
    dummy_read =  RTC.RCR2;
    (void) dummy_read;

    if (start)
    {
        /* start the clock */
        RTC.RCR2 |= 1;
    }
    else
    {
        /* stop the clock */
        RTC.RCR2 &= 0xfe;
    }
}
/******************************************************************************
 End of function R_RTC_StartStop
 ******************************************************************************/

/*******************************************************************************
 * Function Name: R_RTC_GetVersion
 * Description  :
 * Arguments    : *pinfo - pointer to version information structure
 * Return Value : 0
  ******************************************************************************/
int32_t R_RTC_GetVersion(st_ver_info_t *pinfo)
{
    pinfo->lld.version.sub.major = gs_lld_info.version.sub.major;
    pinfo->lld.version.sub.minor = gs_lld_info.version.sub.minor;
    pinfo->lld.build = gs_lld_info.build;
    pinfo->lld.p_szdriver_name = gs_lld_info.p_szdriver_name;

    return (0);
}
/*******************************************************************************
 End of function R_RTC_GetVersion
 ******************************************************************************/
#endif
/* End of File */
