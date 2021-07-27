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
*
* Copyright (C) 2017 Renesas Electronics Corporation. All rights reserved.
******************************************************************************/
/******************************************************************************
* File Name     : linker_settings.ld
* Device(s)     : RZ/A1H
* Tool-Chain    : GNUARM-NONE-EABI-v16.01
* H/W Platform  : RZ/A1H
* Description   : Linker file for projects that require to run from
*               : Internal device RAM
*               : Running code in place
                  In order to run the code in place, changes need to be made to
                  the project’s .ld linker file.
                  Firstly, change the setting of EXEC_BASE to EXEC_BASE_QSPI
                  Secondly, change all lines that tell the linker that a
                  section is in USER_RAM (.reset & .rodata).
******************************************************************************/
/******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 19.08.2016 1.00
******************************************************************************/
OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(start)


/* Size of each sector is 0x40000, first two sectors in single QSPI mode are reserved to the Bootloader */
/* Because of dual chip config the application is located @ 0x80000 */
/* Length is 64Mb - 512Kb (1 sector * 2), application located @ 18080000 */
/* !!!! Check the bootloader project for consistency !!!! */
QSPI_START  = 0x18080000;

VRAM_END    = 0x60A00000;
/* 128 * 2 is added to the length of VRAM_LENGHT because both frame buffers need to be aligned on either 128-byte
   or 32-byte boundary. Sections .VRAM1 and .VRAM2 are aligned on 128-byte boundaries. The addition of 128 * 2
   ensures that VRAM_LENGHT is large enough if the linker performs the automatic alignment at the 128-byte
   boundary for each frame buffer section.
VRAM_LENGTH = (480 * 272 * 2 * 2) + (128 * 2);
/* better use 0x80000 for alignment */
VRAM_LENGTH = 0x0;
VRAM_START  = VRAM_END - VRAM_LENGTH;

PROGRAM_STACK_SIZE = 0x8000;     /* Application stack                  */
IRQ_STACK_SIZE     = 0x2000;     /* IRQ mode stack                     */
FIQ_STACK_SIZE     = 0x2000;     /* FRQ mode stack                     */
SVC_STACK_SIZE     = 0x2000;     /* SVC mode stack                     */
ABT_STACK_SIZE     = 0x2000;     /* ABT mode stack                     */
TTB_SIZE           = 0x8000;     /* Level-1 Translation Table for MMU  */
NL_HEAP_SIZE       = 0xC800;     /* Newlib heap size 50k Bytes         */

STACK_END = VRAM_START - 0x40000000;/* DO NOT CHANGE THIS */
STACK_LENGTH = PROGRAM_STACK_SIZE + IRQ_STACK_SIZE + FIQ_STACK_SIZE + SVC_STACK_SIZE + ABT_STACK_SIZE + TTB_SIZE;
STACK_START  = STACK_END - STACK_LENGTH;

/* RTOS Heap size */
OS_RAM_HEAP_SIZE	= 1024 * 256;	/* OS Abstraction Layer Renesas Drivers Minimum size*/
TX_RAM_HEAP_SIZE = 1024 * 1600;	/* Microsoft Azure RTOS Threadx Heap Size */

OS_HEAP_END = STACK_START;
OS_HEAP_LENGTH = OS_RAM_HEAP_SIZE + TX_RAM_HEAP_SIZE;
OS_HEAP_START = OS_HEAP_END - OS_HEAP_LENGTH;

CACHED_DATA_END = OS_HEAP_START;
CACHED_DATA_LENGTH = 0x600000 + 0x10000;
CACHED_DATA_START = CACHED_DATA_END - CACHED_DATA_LENGTH;

/* The internal boot program of RZ/A1 uses the area at H'2002_0000 to H'2002_3FFF as work memory.
   After e2studio has downloaded the application to internal RAM, a reset is issued and the internal boot program
   start to boot from QSPI. The QSPI loader initializes the QSPI memory system and branches to RAM_START.
   During this process, the internal boot program overwrites the area H'2002_0000 to H'2002_3FFF.
   This area shall not be used by the application. */
RAM_END      = CACHED_DATA_START;
RAM_LENGTH   = 0x00200000;
/* RAM_START may *NOT* be lower than 0x20000000!!!!! */
RAM_START    = RAM_END - RAM_LENGTH;

MEMORY
{
    QSPI    (rx)             : ORIGIN = 0x18080000,  LENGTH = 0x00F80000
    QSPI_FLASH_RES (r)       : ORIGIN = 0x19000000,  LENGTH = 0x03000000
    OS_HEAP (rwx)            : ORIGIN = OS_HEAP_START,		LENGTH = OS_HEAP_LENGTH
    CACHED_DATA (rwx)        : ORIGIN = CACHED_DATA_START,  LENGTH = CACHED_DATA_LENGTH
    USER_RAM (rwx)           : ORIGIN = RAM_START,   LENGTH = RAM_LENGTH
    STACK_RAM (rwx)          : ORIGIN = STACK_START, LENGTH = STACK_LENGTH
    VRAM_RESERVED (r)        : ORIGIN = VRAM_START,  LENGTH = VRAM_LENGTH

    /* Mirror area for cache off operation */
    VRAM_MIRROR (rw)         : ORIGIN = VRAM_START,  LENGTH = VRAM_LENGTH
}

/* SPI ROM attached to H'18000000 to H'1807FFFF Reserved for boot loader */
/* SPI ROM attached to H'18000000 to H'19FFFFFF User Code and data*/
EXEC_BASE = 0x18080000;


/*

WARNING!!!!!

when using cmake for compilation each *.S-file will be compiled to a file named *.S.obj
this linker-script uses filenames named *.o as the compilation-result from the assembler, like it is done in e2studio

thus, this liker-script needs to be enhanced for each filename (*.o) and its appropriate section with the same entry using a the cmake'ed-filename

example:

this entry needs to be duplicated
        *start.o (.text)
        *start.o (.rodata)
        *start.o (.data)

with this each reference to an *.o-file changed to the correct filename INCLUDING the archive-filename!!!!!
        *libBSP.a:start.S.obj (.text)
        *libBSP.a:start.S.obj (.rodata)
        *libBSP.a:start.S.obj (.data)

*/
SECTIONS
{
    .reset EXEC_BASE :
    {
        execute = .;
        *start.o (.text)
        *start.o (.rodata)
        *start.o (.data)

        *access.o (.text)
        *access.o (.rodata)
        *access.o (.data)

        *initsct.o (.text)
        *initsct.o (.rodata)
        *initsct.o (.data)

        *reset_handler.o (.text)
        *reset_handler.o (.rodata)
        *reset_handler.o (.data)

        *ttb_init.o (.text)
        *ttb_init.o (.rodata)
        *ttb_init.o (.data)

        *vbar_init.o (.text)
        *vbar_init.o (.rodata)
        *vbar_init.o (.data)

        .   = ALIGN(0x4);
        *(.text.startup)
        address_end_reset = .;
  } > QSPI

  .rodata EXEC_BASE + SIZEOF(.reset) :
  {
    KEEP("*crtbegin.o"(.ctors))
    KEEP(*(EXCLUDE_FILE(* crtend.o) .ctors))
    KEEP(*(SORT(.ctors.*)))
    KEEP("*crtend.o"(.ctors))
    KEEP("*crtbegin.o"(.dtors))
    KEEP(*(EXCLUDE_FILE(* crtend.o) .dtors))
    KEEP(*(SORT(.dtors.*)))
    KEEP("*crtend.o"(.dtors))

    *(.rodata)
    *(.rodata.*)
    . = ALIGN(0x8);
    *(.text)
    *(.text.*)
    *(.glue_7) /* stubs generated by gcc to glue ARM7 code  calling Thumb code */
    *(.glue_7t) /* stubs generated by gcc to glue Thumb code calling ARM7  code */
    *(.gnu.linkonce.t.*)
    *(.*plt) /* ./common/src/StreamRuntimeConfig.cpp */
    *(.igot.plt) /* ./common/src/StreamRuntimeConfig.cpp */
    *(.rel.iplt) /* ./common/src/StreamRuntimeConfig.cpp */
    *(.rel.dyn)
    KEEP(*(.init))
    *(.fini)
    . = ALIGN(0x4);

        /* stable code and data */
        *webData.o (.text)
        *webSite.o (.text)
    } >QSPI

    .execute_in_ram : AT (address_start_data_ROM)
    {
        address_start_data_RAM = .;
        *(.data)
        *(.data.*)
        *(.igot.plt)
        *(.got.plt)
        *(.got)
        . = ALIGN(0x8);
        address_end_data_RAM = .;
    } > USER_RAM

    .preinit_array :
    {
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);
    }> QSPI

    .init_array :
    {
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP(*(.init_array))
        PROVIDE_HIDDEN (__init_array_end = .);
    } > QSPI

    .ARM.exidx : {
        PROVIDE_HIDDEN(__exidx_start = .);
        *(.ARM.exidx*)
        *(.gnu.linkonce.armexidx.*)
        PROVIDE_HIDDEN(__exidx_end = .);
        address_start_data_ROM = .;
  } > QSPI

    .tors :
    {
        __CTOR_LIST__ = .;
        . = ALIGN(2);
        __ctors = .;
        *(.ctors)
        __ctors_end = .;
        __CTOR_END__ = .;
        __DTOR_LIST__ = .;
        ___dtors = .;
        *(.dtors)
        ___dtors_end = .;
        __DTOR_END__ = .;
        . = ALIGN(2);
        _mdata = .;
    } > USER_RAM AT >QSPI

    .bss (NOLOAD) :
    {
        PROVIDE(g_ld_bss_start = .);
        *(.bss)
        *(.bss.**)
        *(COMMON)
        . = ALIGN(0x4);
         _nl_heap_start = .;
         .   += NL_HEAP_SIZE;
         _nl_heap_end = .;
         . = ALIGN(0x4);
        . = ALIGN(0x4);
        PROVIDE(g_ld_bss_end = .);
        PROVIDE(end = .);
    } > USER_RAM
    
	.UNCACHED_SECTION (NOLOAD) : ALIGN(0x4000)
    {
    	*(.RAM_regionCache)
    } > CACHED_DATA
    
    .OS_RAM_SECTION (NOLOAD) : ALIGN(0x4000)
    {
    	PROVIDE(_ld_mirrored_heap_start = .);
    	. += OS_RAM_HEAP_SIZE;
		PROVIDE(_ld_mirrored_heap_end = .);
		PROVIDE(tx_free_memory_start = .);
		. += TX_RAM_HEAP_SIZE;
		PROVIDE(tx_free_memory_end = .);

    } > OS_HEAP

    .irq_stack (NOLOAD) : ALIGN(0x4000)
    {
        irq_stack_start = .;
        .  += IRQ_STACK_SIZE;
        .   = ALIGN(0x4);
        irq_stack_end = .;
        fiq_stack_start = .;
        .  += FIQ_STACK_SIZE;
        .   = ALIGN(0x4);
        fiq_stack_end = .;
        .  += SVC_STACK_SIZE;
        .   = ALIGN(0x4);
        svc_stack_end = .;
        abt_stack_start = .;
        .  += ABT_STACK_SIZE;
        .   = ALIGN(0x4);
        abt_stack_end = .;
        program_stack_start = .;
        .  += PROGRAM_STACK_SIZE;
        .   = ALIGN(0x4);
        program_stack_end = .;
    } > STACK_RAM

    /* L1 translation table must be aligned to 16KB Boundary!           */
    /* Please refer to Cortex-A Series Version: 4.0 Programmer�s Guide, */
    /* section 9.4 First level address translation                      */
    .ttb_mmu1 (NOLOAD) : ALIGN(0x4000)
    {
        ttb_mmu1_base = .;
        .  += TTB_SIZE;
        .   = ALIGN(0x4);
        ttb_mmu1_end = .;
    } > STACK_RAM

    /* NOLOAD directs linker NOT to fill VRAMx_SECTION with 0.          */
    /* Usage of NOLOAD increases speed of linker and download to target */
    .VRAM_SECTION0 (NOLOAD) :
    {
        vram_start = .;
        *(.VRAM_SECTION0)
        *(.ETHER_DESCRIPTOR)
       
        vram_end = .;
    } > VRAM_MIRROR

}