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
 * Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
 *******************************************************************************
 * File Name    : hwwdt.c
 * Version      : 1.00
 * Device(s)    : Renesas
 * Tool-Chain   : GNUARM-NONE-EABI v14.02
 * OS           : FreeRTOS
 * H/W Platform : RSK+
 * Description  : Simple sample device driver for the Watch Dog Timer.
 *                 : Currently limited to a direct call function that
 *                 : provides a reset.
 ******************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 18.06.2015 1.00 First Release
 ******************************************************************************/

/******************************************************************************
 WARNING!  IN ACCORDANCE WITH THE USER LICENCE THIS CODE MUST NOT BE CONVEYED
 OR REDISTRIBUTED IN COMBINATION WITH ANY SOFTWARE LICENSED UNDER TERMS THE
 SAME AS OR SIMILAR TO THE GNU GENERAL PUBLIC LICENCE
 ******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include <r_wdt_drv_api.h>
#include <stdio.h>
#include <string.h>
#include "r_typedefs.h"
#include "iodefine_cfg.h"
#include "r_stream_it_wdt_lld.h"
#include "r_compiler_abstraction_api.h"

/******************************************************************************
 Defines
 ******************************************************************************/
#define  WDT_PRV_WDT_WRCSR_BIT_WOVF    (1u << 7)


/******************************************************************************
 Function Prototypes
 ******************************************************************************/

static const st_drv_info_t gs_lld_info =
{
    {
        ((STDIO_WDT_RZ_LLD_VERSION_MAJOR << 16) + STDIO_WDT_RZ_LLD_VERSION_MINOR)
    },
    STDIO_WDT_RZ_LLD_BUILD_NUM,
    STDIO_WDT_RZ_LLD_DRV_NAME
};


/******************************************************************************
 Constant Data
 ******************************************************************************/


/******************************************************************************
 Global Variables
 ******************************************************************************/


/******************************************************************************
 Public Functions
 ******************************************************************************/

/******************************************************************************
 Function Name: wdt_hardware_reset
 Description:   Function to generate a reset.
 Arguments:     None
 Return value:  0 for success -1 on error
 ******************************************************************************/
int_t wdt_hardware_reset (void)
{
     volatile  uint8_t wdt_wrcsr;

    /* Stop the counter */
    WDT.WTCSR = 0xA518;

    /* Reset count to 0 */
    WDT.WTCNT = 0x5A00;

    do
    {

        /* Reset the WOVF Flag */
        WDT.WRCSR = 0xA500;

        /* Must read it */
        wdt_wrcsr = *((uint8_t*)(&WDT.WRCSR));

    } while ( WDT_PRV_WDT_WRCSR_BIT_WOVF == (wdt_wrcsr & WDT_PRV_WDT_WRCSR_BIT_WOVF) );


    /* Lower bits 5-0 = 1's */
    WDT.WRCSR = 0x5A5F;

    /* Set WT mode and start count */
    WDT.WTCSR = 0xA578;

    while (true)
    {
        R_COMPILER_Nop();
    }

    /* Reset should have been generated so never reach this return */
    return -1;
}
/******************************************************************************
 End of function  wdt_hardware_reset
 ******************************************************************************/

/*******************************************************************************
 * Function Name: R_WDT_GetVersion
 * Description  :
 * Arguments    : *pinfo - pointer to version information structure
 * Return Value : 0
  ******************************************************************************/
int32_t R_WDT_GetVersion(st_ver_info_t *pinfo)
{
    pinfo->lld.version.sub.major = gs_lld_info.version.sub.major;
    pinfo->lld.version.sub.minor = gs_lld_info.version.sub.minor;
    pinfo->lld.build = gs_lld_info.build;
    pinfo->lld.p_szdriver_name = gs_lld_info.p_szdriver_name;

    return (0);
}
/*******************************************************************************
 End of function R_WDT_GetVersion
 ******************************************************************************/

/******************************************************************************
 Private Functions
 ******************************************************************************/



/******************************************************************************
 End  Of File
 ******************************************************************************/
