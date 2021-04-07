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
   * http://www.renesas.com/disclaimer*
   * Copyright (C) 2016-2017 Renesas Electronics Corporation. All rights reserved.
   *******************************************************************************/

/**************************************************************************//**
 * @file     system_Renesas_RZ_A1.c
 * @brief    CMSIS Device System Source File for
 *           ARM Cortex-Ax Device Series
 * @version  V1.00
 * @date     01 December 2015
 *
 * @note
 *
 ******************************************************************************/
/* Copyright (c) 2011 - 2015 ARM LIMITED

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
   ---------------------------------------------------------------------------*/


#include <stdint.h>

#include "application_cfg.h"

#include "core_ca.h"
#include "Renesas_RZ_A1.h"
#include "mcu_board_select.h"
#include "system_Renesas_RZ_A1.h"

extern void PendSV_Handler(uint32_t);
extern void OS_Tick_Handler(uint32_t);

extern void $Super$$main(void);
void FPUEnable(void);

void CDAbtHandler(uint32_t DFSR, uint32_t DFAR, uint32_t LR);
void CPAbtHandler(uint32_t IFSR, uint32_t IFAR, uint32_t LR);
uint32_t CUndefHandler(uint32_t opcode, uint32_t state, uint32_t LR);
void $Sub$$main(void);
void __SVC_1(void);


/**
 * Initialize the memory subsystem.
 *
 * @param  none
 * @return none
 *
 * @brief Initialize the memory subsystem, including enabling the cache and BTAC. Requires PL1, so implemented as an SVC in case threads are USR mode.
 */

#define EnableCaches() __asm__("SVC #1")
void __SVC_1(void)
{

    /* Before enabling the caches, the instruction cache, the data cache, TLB, and BTAC must have been invalidated.
     * You are not required to invalidate the main TLB, even though it is recommended for safety
     * reasons. This ensures compatibility with future revisions of the processor. */

    unsigned int l2_id;

    /* After MMU is enabled and data has been invalidated, enable caches and BTAC */
    __enable_caches();
    __enable_btac();

    /* If present, you may also need to Invalidate and Enable L2 cache here */
    l2_id = PL310_GetID();
    if (l2_id)
    {
       PL310_InvAllByWay();
       PL310_Enable();
    }
}

IRQHandler IRQTable[Renesas_RZ_A1_IRQ_MAX+1];

uint32_t IRQCount = sizeof IRQTable / 4;

uint32_t InterruptHandlerRegister (IRQn_Type irq, IRQHandler handler)
{
    if (irq < IRQCount) {
        IRQTable[irq] = handler;
        return 0;
    }
    else {
        return 1;
    }
}

uint32_t InterruptHandlerUnregister (IRQn_Type irq)
{
    if (irq < IRQCount) {
        IRQTable[irq] = 0;
        return 0;
    }
    else {
        return 1;
    }
}

void $Sub$$main(void)
{
    //Add your power subsystem initialization here
    //Add your bus initialization here

    InterruptHandlerRegister(SGI0_IRQn    , PendSV_Handler);
    InterruptHandlerRegister(OSTMI0TINT_IRQn, OS_Tick_Handler);

    EnableCaches();
    extern int main(void);
    main(); //Call main
}

//Fault Status Register (IFSR/DFSR) definitions
#define FSR_ALIGNMENT_FAULT                  0x01   //DFSR only. Fault on first lookup
#define FSR_INSTRUCTION_CACHE_MAINTENANCE    0x04   //DFSR only - async/external
#define FSR_SYNC_EXT_TTB_WALK_FIRST          0x0c   //sync/external
#define FSR_SYNC_EXT_TTB_WALK_SECOND         0x0e   //sync/external
#define FSR_SYNC_PARITY_TTB_WALK_FIRST       0x1c   //sync/external
#define FSR_SYNC_PARITY_TTB_WALK_SECOND      0x1e   //sync/external
#define FSR_TRANSLATION_FAULT_FIRST          0x05   //MMU Fault - internal
#define FSR_TRANSLATION_FAULT_SECOND         0x07   //MMU Fault - internal
#define FSR_ACCESS_FLAG_FAULT_FIRST          0x03   //MMU Fault - internal
#define FSR_ACCESS_FLAG_FAULT_SECOND         0x06   //MMU Fault - internal
#define FSR_DOMAIN_FAULT_FIRST               0x09   //MMU Fault - internal
#define FSR_DOMAIN_FAULT_SECOND              0x0b   //MMU Fault - internal
#define FSR_PERMISSION_FAULT_FIRST           0x0f   //MMU Fault - internal
#define FSR_PERMISSION_FAULT_SECOND          0x0d   //MMU Fault - internal
#define FSR_DEBUG_EVENT                      0x02   //internal
#define FSR_SYNC_EXT_ABORT                   0x08   //sync/external
#define FSR_TLB_CONFLICT_ABORT               0x10   //sync/external
#define FSR_LOCKDOWN                         0x14   //internal
#define FSR_COPROCESSOR_ABORT                0x1a   //internal
#define FSR_SYNC_PARITY_ERROR                0x19   //sync/external
#define FSR_ASYNC_EXTERNAL_ABORT             0x16   //DFSR only - async/external
#define FSR_ASYNC_PARITY_ERROR               0x18   //DFSR only - async/external

void CDAbtHandler(uint32_t DFSR, uint32_t DFAR, uint32_t LR)
{
    (void) DFAR;
    (void) LR;

	uint32_t FS = (DFSR & (1 << 10)) >> 6 | (DFSR & 0x0f); //Store Fault Status

    switch(FS) {
        //Synchronous parity errors - retry
        case FSR_SYNC_PARITY_ERROR:
        case FSR_SYNC_PARITY_TTB_WALK_FIRST:
        case FSR_SYNC_PARITY_TTB_WALK_SECOND:
            return;

        //Your code here. Value in DFAR is invalid for some fault statuses.
        case FSR_ALIGNMENT_FAULT:
        case FSR_INSTRUCTION_CACHE_MAINTENANCE:
        case FSR_SYNC_EXT_TTB_WALK_FIRST:
        case FSR_SYNC_EXT_TTB_WALK_SECOND:
        case FSR_TRANSLATION_FAULT_FIRST:
        case FSR_TRANSLATION_FAULT_SECOND:
        case FSR_ACCESS_FLAG_FAULT_FIRST:
        case FSR_ACCESS_FLAG_FAULT_SECOND:
        case FSR_DOMAIN_FAULT_FIRST:
        case FSR_DOMAIN_FAULT_SECOND:
        case FSR_PERMISSION_FAULT_FIRST:
        case FSR_PERMISSION_FAULT_SECOND:
        case FSR_DEBUG_EVENT:
        case FSR_SYNC_EXT_ABORT:
        case FSR_TLB_CONFLICT_ABORT:
        case FSR_LOCKDOWN:
        case FSR_COPROCESSOR_ABORT:
        case FSR_ASYNC_EXTERNAL_ABORT: //DFAR invalid
        case FSR_ASYNC_PARITY_ERROR:   //DFAR invalid
        default:
            while(1);
    }
}

void CPAbtHandler(uint32_t IFSR, uint32_t IFAR, uint32_t LR)
{
	(void) IFAR;
    (void) LR;

	uint32_t FS = (IFSR & (1 << 10)) >> 6 | (IFSR & 0x0f); //Store Fault Status

    switch(FS) {
        //Synchronous parity errors - retry
        case FSR_SYNC_PARITY_ERROR:
        case FSR_SYNC_PARITY_TTB_WALK_FIRST:
        case FSR_SYNC_PARITY_TTB_WALK_SECOND:
            return;

        //Your code here. Value in IFAR is invalid for some fault statuses.
        case FSR_SYNC_EXT_TTB_WALK_FIRST:
        case FSR_SYNC_EXT_TTB_WALK_SECOND:
        case FSR_TRANSLATION_FAULT_FIRST:
        case FSR_TRANSLATION_FAULT_SECOND:
        case FSR_ACCESS_FLAG_FAULT_FIRST:
        case FSR_ACCESS_FLAG_FAULT_SECOND:
        case FSR_DOMAIN_FAULT_FIRST:
        case FSR_DOMAIN_FAULT_SECOND:
        case FSR_PERMISSION_FAULT_FIRST:
        case FSR_PERMISSION_FAULT_SECOND:
        case FSR_DEBUG_EVENT: //IFAR invalid
        case FSR_SYNC_EXT_ABORT:
        case FSR_TLB_CONFLICT_ABORT:
        case FSR_LOCKDOWN:
        case FSR_COPROCESSOR_ABORT:
        default:
            while(1);
    }
}

//returns amount to decrement lr by
//this will be 0 when we have emulated the instruction and want to execute the next instruction
//this will be 2 when we have performed some maintenance and want to retry the instruction in Thumb (state == 2)
//this will be 4 when we have performed some maintenance and want to retry the instruction in ARM   (state == 4)
uint32_t CUndefHandler(uint32_t opcode, uint32_t state, uint32_t LR)
{
	(void) LR;

    const uint32_t THUMB = 2;
    const uint32_t ARM = 4;
    //Lazy VFP/NEON initialisation and switching

    // (ARM ARM section A7.5) VFP data processing instruction?
    // (ARM ARM section A7.6) VFP/NEON register load/store instruction?
    // (ARM ARM section A7.8) VFP/NEON register data transfer instruction?
    // (ARM ARM section A7.9) VFP/NEON 64-bit register data transfer instruction?
    if ((state == ARM   && ((opcode & 0x0C000000) >> 26 == 0x03)) ||
        (state == THUMB && ((opcode & 0xEC000000) >> 26 == 0x3B)))
    {
        if (((opcode & 0x00000E00) >> 9) == 5)
        {
            FPUEnable();
            return state;
        }
    }

    // (ARM ARM section A7.4) NEON data processing instruction?
    if ((state == ARM   && ((opcode & 0xFE000000) >> 24 == 0xF2)) ||
        (state == THUMB && ((opcode & 0xEF000000) >> 24 == 0xEF)) ||
    // (ARM ARM section A7.7) NEON load/store instruction?
        (state == ARM   && ((opcode >> 24) == 0xF4)) ||
        (state == THUMB && ((opcode >> 24) == 0xF9))) {
            FPUEnable();
            return state;
    }

    //Add code here for other Undef cases
    while(1);
}

//Critical section, called from undef handler, so systick is disabled
void FPUEnable(void) {
    __asm__ (
        ".ARM;"

        //Permit access to VFP/NEON, registers by modifying CPACR
        "MRC     p15,0,R1,c1,c0,2;"
        "ORR     R1,R1,#0x00F00000;"
        "MCR     p15,0,R1,c1,c0,2;"

        //Ensure that subsequent instructions occur in the context of VFP/NEON access permitted
        "ISB;"

        //Enable VFP/NEON
        "VMRS    R1,FPEXC;"
        "ORR     R1,R1,#0x40000000;"
        "VMSR    FPEXC,R1;"

        //Initialise VFP/NEON registers to 0
        "MOV     R2,#0;"
#if 1   //{TARGET_FEATURE_EXTENSION_REGISTER_COUNT} >= 16
        //Initialise D16 registers to 0
        "VMOV    D0, R2,R2;"
        "VMOV    D1, R2,R2;"
        "VMOV    D2, R2,R2;"
        "VMOV    D3, R2,R2;"
        "VMOV    D4, R2,R2;"
        "VMOV    D5, R2,R2;"
        "VMOV    D6, R2,R2;"
        "VMOV    D7, R2,R2;"
        "VMOV    D8, R2,R2;"
        "VMOV    D9, R2,R2;"
        "VMOV    D10,R2,R2;"
        "VMOV    D11,R2,R2;"
        "VMOV    D12,R2,R2;"
        "VMOV    D13,R2,R2;"
        "VMOV    D14,R2,R2;"
        "VMOV    D15,R2,R2;"
#endif
#if 1   //{TARGET_FEATURE_EXTENSION_REGISTER_COUNT} == 32
        //Initialise D32 registers to 0
        "VMOV    D16,R2,R2;"
        "VMOV    D17,R2,R2;"
        "VMOV    D18,R2,R2;"
        "VMOV    D19,R2,R2;"
        "VMOV    D20,R2,R2;"
        "VMOV    D21,R2,R2;"
        "VMOV    D22,R2,R2;"
        "VMOV    D23,R2,R2;"
        "VMOV    D24,R2,R2;"
        "VMOV    D25,R2,R2;"
        "VMOV    D26,R2,R2;"
        "VMOV    D27,R2,R2;"
        "VMOV    D28,R2,R2;"
        "VMOV    D29,R2,R2;"
        "VMOV    D30,R2,R2;"
        "VMOV    D31,R2,R2;"
#endif

        //Initialise FPSCR to a known state
        "VMRS    R2,FPSCR;"
        "LDR     R3,=0x00086060;" //Mask off all bits that do not have to be preserved. Non-preserved bits can/should be zero.
        "AND     R2,R2,R3;"
        "VMSR    FPSCR,R2;"

        //"BX      LR;"
             :
             :
             :"r1", "r2", "r3");
    return;
}
