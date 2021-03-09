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
/*******************************************************************************
* File Name     : fiq_handler.s
******************************************************************************
* Device(s)     : RZ/A1L
* Tool-Chain    : GNUARM-NONE-EABI-v16.01
* H/W Platform  : Platform Independent
* Description   : This is the code provides access to the co-processors
********************************************************************************/
/*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 06.07.2016 1.00    Ported from RZA1H v1.01 (dated 3.7.15)
*               : 15.10.2019 1.01    Removed errata and IRQ code due to IRQ code
*                                    now being located in FreeRTOS.
*******************************************************************************/

/* Standard definitions of mode bits and interrupt (I & F) flags in PSRs */
    .equ    SYS_MODE          , 0x1F
    .equ    INTC_ICCIAR_ADDR  , 0xE820200C
    .equ    INTC_ICCEOIR_ADDR , 0xE8202010

/* ================================================================== */
/* Entry point for the FIQ handler */
/* ================================================================== */
    .global  fiq_handler

    .func fiq_handler
fiq_handler:
    SUB     lr, lr, #4
    SRSDB   sp!, #SYS_MODE
    CPS     #SYS_MODE
    PUSH    {r0-r4, r12}

    /*; Dummy read to activate interrupt */
    LDR     r0, =INTC_ICCIAR_ADDR

    MOV     r0, sp
    AND     r0, r0, #4
    SUB     sp, sp, r0
    PUSH    {r0, lr}

    BL      R_INTC_FiqHandler

    POP     {r0, lr}
    ADD     sp, sp, r0

    /*; Dummy read of end-of-interrupt register*/
    LDR     r0, =INTC_ICCEOIR_ADDR

    POP     {r0-r4, r12}
    RFEIA   sp!
    .endfunc

    .end

