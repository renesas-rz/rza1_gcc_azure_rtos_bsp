                                                                          
                                                                           
                                                                           
                                                                           
                                                                           
                                                                           
                                                                           
                                                                           
                                                                           
                                                                           
                                                                           
 
/************************************************************************/
/*    File Version: V1.01                                               */
/*    Date Generated: 30/10/2014                                        */
/************************************************************************/

    /*reset_program.asm*/

    .list
    .section .text
    .global _PowerON_Reset    /*global Start routine */
    
    .extern HardwareSetup  /*external Sub-routine to initialise Hardware*/
    .extern _data
    .extern _mdata
    .extern __bss_start__
    .extern __bss_end__
    .extern _edata
    .extern main
    .extern .Lstack
    .extern _rvectors
    .extern _exit
    

/* bss initialisation labels  */
     .LC1:
     .word    __bss_start__
     .LC2:
     .word    __bss_end__

/* data initialisation labels  */
     .LD1:
     .word    _mdata
     .LD2:
     .word    _data
     .LD3:
     .word    _edata

     _PowerON_Reset:
     movs     a2, #0              /* Second arg: fill value */
     mov    fp, a2             /* Null frame pointer */
     mov    r7, a2             /* Null frame pointer for Thumb */
     ldr    a1, .LC1         /* First arg: start of memory block */
     ldr    a3, .LC2
     cmp    a3, a1
     beq     .next1
     .next:
     strb     a2, [a1]
     add    a1, a1, #1
     cmp    a3, a1
     bne    .next

vfp_init:
#ifndef __SOFTFP__
    /* Initialize VFP setting */
     ldr	r0,=(0xF <<20)
     mcr	p15, 0, r0, c1, c0, 2
     mov	r3,#0x40000000
     vmsr	fpexc, r3
#endif

/* load data section from ROM to RAM */

    .next1:
#ifndef SIMULATOR_DEBUG
    ldr    a3, .LD1
    ldr    a1,    .LD2
    ldr    a2, .LD3
    cmp    a2, a1
    beq    .next2
    .start:
    ldrb    a4, [a3]
    strb    a4,    [a1]
    add    a3, a3, #1
    add    a1, a1, #1
    cmp    a1,    a2
    bne    .start
    .next2:
#endif

/* initialise stack pointer */
     .Lstack:
     .word    _stack
     ldr    r11, .Lstack
     cmp    r11, #0
     beq    exit
     mov    sp, r11

#ifndef SIMULATOR_DEBUG
/* call the hardware initialiser */
    bl    HardwareSetup
#endif

/* start user program */
    bl    main
    bl    exit

/* call to exit*/
    exit:
    bl  _loop_here
    _loop_here:
    bl _loop_here

    .text
    .end
