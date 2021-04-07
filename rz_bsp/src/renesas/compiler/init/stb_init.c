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
* Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/******************************************************************************
* File Name     : stb_init.c
* Device(s)     : RZ/A1L
* Tool-Chain    : GNUARM-NONE-EABI-v16.01
* H/W Platform  : Stream it! v2
* Description   : Initialise RZ/A1L Standby Control Registers.
******************************************************************************/
/*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 06.07.2016 1.00    Stream it! v2 first release
*******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
/* Default  type definition header */
#include "r_typedefs.h"

/* I/O Register root header */
#include "iodefine_cfg.h"

/* System CPG register configuration header */
#include "stb_init.h"

/* Interchangeable compiler specific header */
#include "compiler_settings.h"

/******************************************************************************
 * Function Name: stb_init
* Description  : Standby Control Register Initialisation
*                Takes peripherals out of standby mode.
* Arguments    : none
* Return Value : none
******************************************************************************/
void stb_init (void)
{
    volatile uint8_t dummy_buf = 0u;

    UNUSED_VARIABLE(dummy_buf);

    /* ---- Disable all module clocks ---- */
    CPG.STBCR10 = 0xFFu;
    /* (Dummy read)                                                */
    dummy_buf = CPG.STBCR10;

    /* ---- Enable all module clocks ---- */

    /* Port level is keep in standby mode, [1], [1], [0],          */
    /* [1], [0], [1], CoreSight                                    */
    CPG.STBCR2  = 0x6Au;

    /* (Dummy read)                                                */
    dummy_buf = CPG.STBCR2;

    /* IEBus, IrDA, LIN, [1], MTU2, RSCAN, ADC, [1]                */
    CPG.STBCR3  = 0x11u;

    /* (Dummy read)                                                */
    dummy_buf = CPG.STBCR3;

    /* SCIF0, SCIF1, SCIF2, SCIF3, SCIF4, [1], [1], [1]            */
    CPG.STBCR4  = 0x07u;

    /* (Dummy read)                                                */
    dummy_buf = CPG.STBCR4;

    /* SCIM0, SCIM1, [1], [1], [1], [1], OSTM0, OSTM1              */
    CPG.STBCR5  = 0x3Cu;

    /* (Dummy read)                                                */
    dummy_buf = CPG.STBCR5;

    /* A/D, CEU, [1], [1], [1], [1], JCU, RTClock                  */
    CPG.STBCR6  = 0x3Cu;

    /* (Dummy read)                                                */
    dummy_buf = CPG.STBCR6;

    /* [0], [0], [1], ETHER, [1], [1], USB0, USB1                  */
    CPG.STBCR7  = 0x2Fu;

    /* (Dummy read)                                                */
    dummy_buf = CPG.STBCR7;

    /* [1], [1], [1], MMCIF, MOST50, [1], SCUX, [1]                */
    CPG.STBCR8  = 0xE5u;

    /* (Dummy read)                                                */
    dummy_buf = CPG.STBCR8;

    /* I2C0, I2C1, I2C2, I2C3, SPIBSC0, [1], VDC50, [1]            */
    CPG.STBCR9  = 0x05u;

    /* (Dummy read)                                                */
    dummy_buf = CPG.STBCR9;

    /* RSPI0, RSPI1, RSPI2, [1], [1], CD-ROMDEC, RSPDIF, [1]       */
    CPG.STBCR10 = 0x19u;

    /* (Dummy read)                                                */
    dummy_buf = CPG.STBCR10;

    /* [1], [1], SSIF0, SSIF1, SSIF2, SSIF3, [1], [1]              */
    CPG.STBCR11 = 0xC3u;

    /* (Dummy read)                                                */
    dummy_buf = CPG.STBCR11;

    /* [1], [1], [1], [1], SDHI00, SDHI01, SDHI10, SDHI11          */
    CPG.STBCR12 = 0xF0u;

    /* (Dummy read)                                                */
    dummy_buf = CPG.STBCR12;
}
/*******************************************************************************
 End of function R_STB_Init
 *******************************************************************************/

/* End of File */

