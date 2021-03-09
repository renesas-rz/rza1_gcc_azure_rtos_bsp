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
* File Name     : intc_handler.c
* Device(s)     : RZ/A1L
* Tool-Chain    : GNUARM-NONE-EABI-v16.01
* H/W Platform  : Platform Independent
* Description   : Sample Program - Handler process
*******************************************************************************/
/*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 06.07.2016 1.00    Ported from RZA1H
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
/* Default  type definition header */
#include "r_typedefs.h"

#include "iodefine_cfg.h"
/* Interchangeable compiler specific header */
#include "compiler_settings.h"

/* INTC Driver Header */
#include "r_intc.h"

/*******************************************************************************
* Function Name: R_INTC_InterruptHandler
* Description  : This function is the INTC interrupt handler processing called 
*              : by the irq_handler. Executes the handler processing which 
*              : corresponds to the INTC interrupt source ID specified by the
*              : icciar by calling the Userdef_INTC_HandlerExe function. The
*              : IRQ multiple interrupts are enabled. The processing for
*              : unsupported interrupt ID is executed by calling
*              : Userdef_INTC_UndefId function.
*              : In the interrupt handler processing, when the int_sense shows 
*              : "INTC_LEVEL_SENSITIVE", clear the interrupt source because it
*              : means a level sense interrupt. 
*              : Stacks are restored by ASM with the top level to correspond
*              : to multiple interrupts
* Arguments    : uint32_t icciar : Interrupt ID (value of ICCIAR register)
* Return Value : none
*******************************************************************************/
void R_INTC_InterruptHandler (uint32_t icciar)
{
    uint32_t mask;
    uint32_t int_sense;
    uint16_t int_id;

    uint32_t volatile * paddr = NULL;




    int_id = (uint16_t)(icciar & 0x000003FFuL); /* Obtain interrupt ID */

    /*
     * If an interrupt ID value read from the interrupt acknowledge register
     * (ICCIAR) is 1022 or 1023, return from interrupt processing after
     * writing the same value as the setting value to the interrupt priority
     * register 0 (ICDIPR0).
     */
    if (int_id >= 0x3fe)    /* In case of unsupported interrupt ID */
    {
        paddr = (volatile uint32_t *)&INTC.ICDIPR0;

        *paddr = icciar;
        return;
    }

    if (int_id >= INTC_ID_TOTAL)    /* In case of unsupported interrupt ID */
    {
        userdef_intc_undef_id(int_id);
    }

    /* ==== Interrupt handler call ==== */
    __enable_irq();         /* IRQ interrupt enabled */

    /* ICDICFRn has 16 sources in the 32 bits                                  */
    /* The n can be calculated by int_id / 16                                  */
    /* The upper 1 bit out of 2 bits for the bit field width is the target bit */
    /* The target bit can be calculated by ((int_id % 16) * 2) + 1             */
    paddr = (volatile uint32_t *)&INTC.ICDICFR0;
    mask = (uint32_t)(1 << (((int_id % 16) * 2) + 1));
    if (0 == ((*(paddr + (int_id / 16))) & mask))  /* In the case of level sense */
    {
        int_sense = INTC_LEVEL_SENSITIVE;
    }

    /* In the case of edge trigger */
    else
    {
        int_sense = INTC_EDGE_TRIGGER;
    }

    /* Call interrupt handler */
    userdef_intc_handler_exe(int_id, int_sense);

    /* IRQ interrupt disabled */
    __disable_irq();
}
/*******************************************************************************
 End of function R_INTC_InterruptHandler
 *******************************************************************************/

/*******************************************************************************
* Function Name: R_INTC_FiqHandler
* Description  : This function is the INTC interrupt handler processing called
*              : by the fiq_handler.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void R_INTC_FiqHandler (void)
{
    userdef_fiq_handler_exe();
}
/*******************************************************************************
 End of function R_INTC_FiqHandler
 *******************************************************************************/

/* END of File */

