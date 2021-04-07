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
*******************************************************************************/
/*******************************************************************************
* Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/******************************************************************************
* File Name     : resetprg.c
* Device(s)     : RZ/A1L
* Tool-Chain    : GNUARM-NONE-EABI-v16.01
* H/W Platform  : Stream it! v2
* Description   : C library entry point
*               : Variants of this file must be created for each compiler
******************************************************************************/
/*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 06.07.2016 1.00    Ported from RZA1H
*******************************************************************************/

#include "r_typedefs.h"

#include "compiler_settings.h"
#include "stb_init.h"
#include "main.h"

/* INTC Driver Header */
#include "r_intc.h"
#include "pl310.h"
#if defined(__thumb2__) || (defined(__thumb__) && defined(__ARM_ARCH_6M__))
# define THUMB_V7_V6M
#endif

/* Defined if this target supports the BLX Rm instruction.  */
#if  !defined(__ARM_ARCH_2__) && !defined(__ARM_ARCH_3__) && \
                !defined(__ARM_ARCH_3M__)    && !defined(__ARM_ARCH_4__) \
                                                   && !defined(__ARM_ARCH_4T__)
# define HAVE_CALL_INDIRECT
#endif

#ifdef HAVE_INITFINI_ARRAY
#define _init    __libc_init_array
#define _fini    __libc_fini_array
#endif

extern int L1CacheInit(void);
extern void __libc_init_array (void);

void resetprg (void);

/*******************************************************************************
 * Function Name: resetprg
 * Description  : Program entry point after basic peripheral initialisation
 * Arguments    : none
 * Return Value : none
 *******************************************************************************/
void resetprg (void)
{
	uint32_t l2_id = 0;

    stb_init();

    R_INTC_Init();

    /* If present, you may also need to Invalidate and Enable L2 cache here */
    l2_id = PL310_GetID();

    if (l2_id)
    {
       PL310_Disable();
       PL310_InvAllByWay();
       PL310_Enable();
    }

    /* Initial setting of the level 1 cache */
    L1CacheInit();

    /* Perform MemoryManager Initialisation before any use of dynamic RAM */
    R_OS_InitMemManager();

    __enable_irq();
    __enable_fiq();
    __libc_init_array();

    R_OS_InitKernel();


    /* Stops program from running off */
    while (1)
    {
        __asm__("nop");
    }
}
/*******************************************************************************
 End of function resetprg
 *******************************************************************************/
