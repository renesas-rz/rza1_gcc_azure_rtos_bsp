@/**************************************************************************/ 
@/*                                                                        */ 
@/*            Copyright (c) 1996-2014 by Express Logic Inc.               */ 
@/*                                                                        */ 
@/*  This software is copyrighted by and is the sole property of Express   */ 
@/*  Logic, Inc.  All rights, title, ownership, or other interests         */ 
@/*  in the software remain the property of Express Logic, Inc.  This      */ 
@/*  software may only be used in accordance with the corresponding        */ 
@/*  license agreement.  Any unauthorized use, duplication, transmission,  */ 
@/*  distribution, or disclosure of this software is expressly forbidden.  */ 
@/*                                                                        */
@/*  This Copyright notice may not be removed or modified without prior    */ 
@/*  written consent of Express Logic, Inc.                                */ 
@/*                                                                        */ 
@/*  Express Logic, Inc. reserves the right to modify this software        */ 
@/*  without notice.                                                       */ 
@/*                                                                        */ 
@/*  Express Logic, Inc.                     info@expresslogic.com         */ 
@/*  11423 West Bernardo Court               http://www.expresslogic.com   */ 
@/*  San Diego, CA  92127                                                  */ 
@/*                                                                        */
@/**************************************************************************/
@
@
@/**************************************************************************/
@/**************************************************************************/
@/**                                                                       */ 
@/** ThreadX Component                                                     */ 
@/**                                                                       */
@/**   Initialize                                                          */
@/**                                                                       */
@/**************************************************************************/
@/**************************************************************************/
@
@
@#define TX_SOURCE_CODE
@
@
@/* Include necessary system files.  */
@
@#include "tx_api.h"
@#include "tx_initialize.h"
@#include "tx_thread.h"
@#include "tx_timer.h"

    .arm

INTC_ICCIAR_ADDR  	= 0xE820200C
INTC_ICCEOIR_ADDR 	= 0xE8202010

SVC_MODE        =       0xD3                    @ Disable IRQ/FIQ SVC mode
IRQ_MODE        =       0xD2                    @ Disable IRQ/FIQ IRQ mode
FIQ_MODE        =       0xD1                    @ Disable IRQ/FIQ FIQ mode
SYS_MODE        =       0xDF                    @ Disable IRQ/FIQ SYS mode
FIQ_STACK_SIZE  =       512                     @ FIQ stack size
IRQ_STACK_SIZE  =       1024                    @ IRQ stack size
SYS_STACK_SIZE  =       1024                    @ System stack size
@
@
@ Add definitions from here. ---------- 2022.04.27 modify

#ifdef TX_ENABLE_FIQ_SUPPORT
DISABLE_INTS    =       0xC0                    @ Disable IRQ/FIQ interrupts
#else
DISABLE_INTS    =       0x80                    @ Disable IRQ interrupts
#endif

MODE_MASK       =       0x1F                    @ Mode mask
SYS_MODE_BITS   =       0x1F                    @ System mode bits
IRQ_MODE_BITS   =       0x12                    @ IRQ mode bits

@ Add definitions up to this point. ----------

    .global     _tx_thread_system_stack_ptr
    .global     _tx_initialize_unused_memory
    .global     _tx_thread_context_save
    .global     _tx_thread_context_restore
    .global     _tx_timer_interrupt
	.extern     vApplicationIRQHandler

    .equ    INTC_ICCIAR_MASK  , 0x3FF
    .equ    INTC_ICCIAR_ADDR  , 0xE820200C
    .equ    INTC_ICCEOIR_ADDR , 0xE8202010
    .equ    INTC_ICCHPIR_ADDR , 0xE8202018
    .equ    INTC_ICDIPR0_ADDR , 0xE8201400

.align 4
read_intc_icciar_addr:
	PUSH	{r1, r12}
	PUSH	{r3-r4, r12}

    /*; ++REE_SS Addressing ARM Errata 801120 */
    /*; Perform a dummy read to - ensure subsequent ICCIAR data */
    /*; will be correct */
    LDR     r2, =INTC_ICCHPIR_ADDR
    LDR     r2, [r2]
    LDR     r0, [r2]

    /*; Attempt to activate interrupt and get its ID */
    /*; Load in to R3 - if valid it will be used later as ICCIAR */
    LDR     r2, =INTC_ICCIAR_ADDR
    LDR     r3, [r2]

    /*; Extract the interrupt ID (removing the SGI source CPU ID) */
    LDR     r1, =INTC_ICCIAR_MASK
    AND     r3, r3, r1

    /*; Read data of 0x0 (SGI ID0 from CPU0) is possibly affected by 733075 */
    CMP     r3, #0
    BEQ     errata_733075_workaround

    /*; Interrupt IDs 0x3FE and 0x3FF are also possibly affected */
    LDR     r1, =1022
    CMP     r3, r1
    BGE     errata_733075_workaround

    B       post_733075_workaround

errata_733075_workaround:
    /*; Perform a read from ulICDIPR0 and write value back */
    /*; It is sufficient to write the value that is already in the register. */
    /*; You can obtain the value to be written to the ICDIPR0 register by */
    /*; reading from it, or through other means" */
    LDR     r2, =INTC_ICDIPR0_ADDR
    LDR     r0, [r2]
    STR     r0, [r2]
    DSB

    LDR     r2, =INTC_ICCHPIR_ADDR
    LDR     r2, [r2]
    LDR     r0, [r2]

    /*; Attempt to activate interrupt and get its ID */
    /*; Load in to R3 - if valid it will be used later as ICCIAR */
    LDR     r2, =INTC_ICCIAR_ADDR
    LDR     r3, [r2]

post_733075_workaround:
    MOV     r0, r3
	POP		{r3-r4, r12}
	POP		{r1, r12}
	B       icciar_read_complete

@
@
@/* Define the 16-bit Thumb mode veneer for _tx_initialize_low_level for
@   applications calling this function from to 16-bit Thumb mode.  */
@
    .text
    .align 2
    .thumb
    .global $_tx_initialize_low_level
    .type   $_tx_initialize_low_level,function
$_tx_initialize_low_level:
     BX        pc                               @ Switch to 32-bit mode
     NOP                                        @ 
    .arm
     STMFD     sp!, {lr}                        @ Save return address
     BL        _tx_initialize_low_level         @ Call _tx_initialize_low_level function
     LDMFD     sp!, {lr}                        @ Recover saved return address
     BX        lr                               @ Return to 16-bit caller
@
@
    .text
    .align 2
@/**************************************************************************/ 
@/*                                                                        */ 
@/*  FUNCTION                                               RELEASE        */ 
@/*                                                                        */ 
@/*    _tx_initialize_low_level                          Cortex-A9/GNU     */ 
@/*                                                           5.1          */ 
@/*  AUTHOR                                                                */ 
@/*                                                                        */ 
@/*    William E. Lamie, Express Logic, Inc.                               */ 
@/*                                                                        */ 
@/*  DESCRIPTION                                                           */ 
@/*                                                                        */ 
@/*    This function is responsible for any low-level processor            */ 
@/*    initialization, including setting up interrupt vectors, setting     */ 
@/*    up a periodic timer interrupt source, saving the system stack       */ 
@/*    pointer for use in ISR processing later, and finding the first      */ 
@/*    available RAM memory address for tx_application_define.             */ 
@/*                                                                        */ 
@/*  INPUT                                                                 */ 
@/*                                                                        */ 
@/*    None                                                                */ 
@/*                                                                        */ 
@/*  OUTPUT                                                                */ 
@/*                                                                        */ 
@/*    None                                                                */ 
@/*                                                                        */ 
@/*  CALLS                                                                 */ 
@/*                                                                        */ 
@/*    None                                                                */ 
@/*                                                                        */ 
@/*  CALLED BY                                                             */ 
@/*                                                                        */ 
@/*    _tx_initialize_kernel_enter           ThreadX entry function        */ 
@/*                                                                        */ 
@/*  RELEASE HISTORY                                                       */ 
@/*                                                                        */ 
@/*    DATE              NAME                      DESCRIPTION             */ 
@/*                                                                        */ 
@/*  03-11-2012     William E. Lamie         Initial Version 5.0           */ 
@/*  01-01-2014     William E. Lamie         Modified comment(s),          */ 
@/*                                            resulting in version 5.1    */ 
@/*                                                                        */ 
@/**************************************************************************/ 
@VOID   _tx_initialize_low_level(VOID)
@{
    .global _tx_initialize_low_level
    .type   _tx_initialize_low_level,function
_tx_initialize_low_level:
@   /* Remember the stack pointer, link register, and switch to SVC mode. */

    MOV     r0, sp                              /* Remember the SP */
    MOV     r1, lr                              /* Remember the LR */
    MOV     r3, #SVC_MODE                       /* Build SVC mode CPSR */
    MSR     CPSR_cxsf, r3                       /* Switch to SVC mode */
    MOV     sp, r0                              /* Inherit the stack pointer setup by cstartup */
    MOV     lr, r1                              /* Inherit the link register */

    /* Pickup the start of free memory. */

    LDR     r0, =tx_free_memory_start         	/* Get end of non-initialized RAM area */

    LDR     r2, =_tx_initialize_unused_memory   /* Pickup unused memory ptr address */
    STR     r0, [r2, #0]                        /* Save first free memory address */

    /* Done, return to caller.  */

    BX      lr                                  /* Return to caller */
@
@    /* Setup Timer for periodic interrupts.  */
@
@    /* Done, return to caller.  */
@
#ifdef __THUMB_INTERWORK
    BX      lr                                  @ Return to caller
#else
    MOV     pc, lr                              @ Return to caller
#endif
@}
@
@
@/* Define shells for each of the interrupt vectors.  */
@
    .global __tx_undefined
__tx_undefined:
    B       __tx_undefined                      @ Undefined handler
@
    .global __tx_swi_interrupt
__tx_swi_interrupt:
    B       __tx_swi_interrupt                  @ Software interrupt handler
@
    .global __tx_prefetch_handler
__tx_prefetch_handler:
    B       __tx_prefetch_handler               @ Prefetch exception handler
@
    .global __tx_abort_handler
__tx_abort_handler:
    B       __tx_abort_handler                  @ Abort exception handler
@
    .global __tx_reserved_handler
__tx_reserved_handler:
    B       __tx_reserved_handler               @ Reserved exception handler
@
	.global irq_handler
    .global __tx_irq_handler
    .global __tx_irq_processing_return      
__tx_irq_handler:
irq_handler:
@
@    /* Jump to context save to save system context.  */
    B       _tx_thread_context_save
__tx_irq_processing_return:
@
@    /* At this point execution is still in the IRQ mode.  The CPSR, point of
@       interrupt, and all C scratch registers are available for use.  In 
@       addition, IRQ interrupts may be re-enabled - with certain restrictions -
@       if nested IRQ interrupts are desired.  Interrupts may be re-enabled over
@       small code sequences where lr is saved before enabling interrupts and 
@       restored after interrupts are again disabled.  */
@
@    /* Interrupt nesting is allowed after calling _tx_thread_irq_nesting_start 
@       from IRQ mode with interrupts disabled.  This routine switches to the
@       system mode and returns with IRQ interrupts enabled.  
@       
@       NOTE:  It is very important to ensure all IRQ interrupts are cleared 
@       prior to enabling nested IRQ interrupts.  */
#ifdef TX_ENABLE_IRQ_NESTING
    BL      _tx_thread_irq_nesting_start
#endif
@

@ Add processing from here.  ---------- 2022.04.27 modify

@   /* _tx_thread_irq_nesting_start (IRQ interrupt disable) */
    MRS     r0, CPSR                            @ Pickup the CPSR
    BIC     r0, r0, #MODE_MASK                  @ Clear the mode bits
    ORR     r0, r0, #SYS_MODE_BITS              @ Build system mode CPSR
    MSR     CPSR_c, r0                          @ Enter system mode
    STMDB   sp!, {r1, lr}                       @ Push the system mode lr on the system mode stack
                                                @   and push r1 just to keep 8-byte alignment

@ Add the process up to this point.  ----------


@   /* Call the Renesas RZ IRQ processing routine. */

	//LDR		r2, =INTC_ICCIAR_ADDR
    //LDR     r0, [r2]
  	B	read_intc_icciar_addr
icciar_read_complete:
    PUSH    {r0}

	/* Ensure bit 2 of the stack pointer is clear.  r2 holds the bit 2 value for
	future use.  Guard against the start of the stack not being 8-byte aligned */
    MOV     r1, sp
    AND     r1, r1, #4
    SUB     sp, sp, r1
    PUSH    {r1, lr}
    BL      R_INTC_InterruptHandler
    POP     {r1, lr}
    ADD     sp, sp, r1
    POP     {r0}
    LDR     r2, =INTC_ICCEOIR_ADDR
    STR     r0, [r2]

@ Add processing from here.  ---------- 2022.04.27 modify

@   /* _tx_thread_irq_nesting_end */
    MRS     r0, CPSR                            @ Pickup the CPSR
    ORR     r0, r0, #DISABLE_INTS               @ Build disable interrupt value
    MSR     CPSR_c, r0                          @ Disable interrupts
    LDMIA   sp!, {r1, lr}                       @ Pickup saved lr (and r1 throw-away for 
                                                @   8-byte alignment logic)
    BIC     r0, r0, #MODE_MASK                  @ Clear mode bits
    ORR     r0, r0, #IRQ_MODE_BITS              @ Build IRQ mode CPSR
    MSR     CPSR_c, r0                          @ Reenter IRQ mode

@ Add the process up to this point.  ----------

@
@
@    /* If interrupt nesting was started earlier, the end of interrupt nesting
@       service must be called before returning to _tx_thread_context_restore.  
@       This routine returns in processing in IRQ mode with interrupts disabled.  */
#ifdef TX_ENABLE_IRQ_NESTING
    BL      _tx_thread_irq_nesting_end
#endif
@
@    /* Jump to context restore to restore system context.  */
    B       _tx_thread_context_restore
@
@
@    /* This is an example of a vectored IRQ handler.  */
@
@    .global __tx_example_vectored_irq_handler
@__tx_example_vectored_irq_handler:
@
@
@    /* Save initial context and call context save to prepare for 
@       vectored ISR execution.  */
@
@    STMDB   sp!, {r0-r3}                        @ Save some scratch registers
@    MRS     r0, SPSR                            @ Pickup saved SPSR
@    SUB     lr, lr, #4                          @ Adjust point of interrupt 
@    STMDB   sp!, {r0, r10, r12, lr}             @ Store other scratch registers
@    BL      _tx_thread_vectored_context_save    @ Vectored context save
@
@    /* At this point execution is still in the IRQ mode.  The CPSR, point of
@       interrupt, and all C scratch registers are available for use.  In 
@       addition, IRQ interrupts may be re-enabled - with certain restrictions -
@       if nested IRQ interrupts are desired.  Interrupts may be re-enabled over
@       small code sequences where lr is saved before enabling interrupts and 
@       restored after interrupts are again disabled.  */
@
@
@    /* Interrupt nesting is allowed after calling _tx_thread_irq_nesting_start 
@       from IRQ mode with interrupts disabled.  This routine switches to the
@       system mode and returns with IRQ interrupts enabled.  
@       
@       NOTE:  It is very important to ensure all IRQ interrupts are cleared 
@       prior to enabling nested IRQ interrupts.  */
@#ifdef TX_ENABLE_IRQ_NESTING
@    BL      _tx_thread_irq_nesting_start
@#endif
@
@    /* Application IRQ handlers can be called here!  */
@
@    /* If interrupt nesting was started earlier, the end of interrupt nesting
@       service must be called before returning to _tx_thread_context_restore.  
@       This routine returns in processing in IRQ mode with interrupts disabled.  */
@#ifdef TX_ENABLE_IRQ_NESTING
@    BL      _tx_thread_irq_nesting_end
@#endif
@
@    /* Jump to context restore to restore system context.  */
@    B       _tx_thread_context_restore
@
@
#ifdef TX_ENABLE_FIQ_SUPPORT
    .global  __tx_fiq_handler
    .global  __tx_fiq_processing_return
__tx_fiq_handler:
@
@    /* Jump to fiq context save to save system context.  */
    B       _tx_thread_fiq_context_save
__tx_fiq_processing_return:
@
@    /* At this point execution is still in the FIQ mode.  The CPSR, point of
@       interrupt, and all C scratch registers are available for use.  */
@
@    /* Interrupt nesting is allowed after calling _tx_thread_fiq_nesting_start 
@       from FIQ mode with interrupts disabled.  This routine switches to the
@       system mode and returns with FIQ interrupts enabled. 
@
@       NOTE:  It is very important to ensure all FIQ interrupts are cleared 
@       prior to enabling nested FIQ interrupts.  */
#ifdef TX_ENABLE_FIQ_NESTING
    BL      _tx_thread_fiq_nesting_start
#endif
@
@    /* Application FIQ handlers can be called here!  */
@
@    /* If interrupt nesting was started earlier, the end of interrupt nesting
@       service must be called before returning to _tx_thread_fiq_context_restore.  */
#ifdef TX_ENABLE_FIQ_NESTING
    BL      _tx_thread_fiq_nesting_end
#endif
@
@    /* Jump to fiq context restore to restore system context.  */
    B       _tx_thread_fiq_context_restore
@
@
#else
    .global  __tx_fiq_handler
__tx_fiq_handler:
    B       __tx_fiq_handler                    @ FIQ interrupt handler
#endif


BUILD_OPTIONS:
    .word  _tx_build_options                    @ Reference to bring in
VERSION_ID:
    .word  _tx_version_id                       @ Reference to bring in



