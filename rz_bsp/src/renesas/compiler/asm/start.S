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
* File Name     : start.s
* Device(s)     : RZ/A1L
* Tool-Chain    : GNUARM-NONE-EABI-v16.01
* H/W Platform  : Platform Independent
* Description   : This is the code to be executed on the target
                  The copyright string signifies the end of the Vector r_intc_enable
*                 Note boot strap sequence is as follows:
*
*                 start->reset_handler->resetprg()->main()
*
*                 start - first code to be executed on the target
                  start jumps to reset_handler the asm startup routine
*                 reset_handler jumps to resetprg() C entry point
*                 resetprg() calls main() C User code entry point
*******************************************************************************/
/*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 06.07.2016 1.00    Ported from RZA1H
*               : 15.10.2019 1.01    IRQ code is now in FreeRTOS so has been
*                                    removed from irqfiq_handler.S (now renamed
*                                    to fiq_handler.S).
*******************************************************************************/

    .text
    .code 32

    .global start
    .func   start

start:
    /* reset_handler /renesas/compiler/asm/reset_handler.S                     */
    LDR pc, =reset_handler

    /* undefined_handler /renesas/compiler/asm/reset_handler.S                 */
    LDR pc, =undefined_handler

    /* svc_handler */
    LDR pc, =svc_handler

    /* prefetch_handler /renesas/compiler/asm/reset_handler.S                  */
    LDR pc, =prefetch_handler

    /* abort_handler /renesas/compiler/asm/reset_handler.S                     */
    LDR pc, =abort_handler

    /* reserved_handler /renesas/compiler/asm/reset_handler.S                  */
    LDR pc, =reserved_handler

    /* FreeRTOS_IRQ_Handler /freertos/portable/gcc/arm_ca9_rza1lu/portasm.S    */
    LDR pc, =irq_handler

    /* fiq_handler /renesas/compiler/asm/fiq_handler.S                      */
    LDR pc, =fiq_handler
code_start:
    .word    start            /* pointer to the user application start address */
                              /* Used by NOR and SPI (RZ_A1H_xxxx_LOADER_RSK)  */
code_end:
    .word    end
code_execute:
    .word    execute                   /* execute address of first instruction */
    .string ".BootLoad_ValidProgramTest." /* bootloader validation signature   */
    .align 4
    .end
