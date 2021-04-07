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
 * Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
 *****************************************************************************/
/******************************************************************************
 * File Name    : endian.c
 * Device(s)    : RZA
 * Tool-Chain   : GNUARM-NONE-EABI v14.02
 * OS           : Scheduler
 * H/W Platform : RSK
 * Description  : endian swapping
 *****************************************************************************/
/******************************************************************************
 * History     : DD.MM.YYYY Version Description
 *             : 18.06.2013 1.00
 *             : __#dd#__.__#mm#__.__#yyyy#__ __#ver_maj#__.__#ver_min#__ First Release
 *****************************************************************************/

/******************************************************************************
 WARNING!  IN ACCORDANCE WITH THE USER LICENCE THIS CODE MUST NOT BE CONVEYED
 OR REDISTRIBUTED IN COMBINATION WITH ANY SOFTWARE LICENSED UNDER TERMS THE
 SAME AS OR SIMILAR TO THE GNU GENERAL PUBLIC LICENCE
 ******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 *****************************************************************************/
#include "r_typedefs.h"
#include "endian.h"

/******************************************************************************
 Macro definitions
 *****************************************************************************/

/******************************************************************************
 Typedef definitions
 *****************************************************************************/

/******************************************************************************
 Exported global variables (to be accessed by other files)
 *****************************************************************************/

/******************************************************************************
 Private global variables and functions
 *****************************************************************************/
#if(0)
unsigned long swapDB(unsigned long val);
unsigned long swapLong(unsigned long val);

.global     swapLongBuffer
.func         swapLongBuffer
.global     swapLongBufferSafe
.func         swapLongBufferSafe
.global     swapToLongBufferSafe
.func         swapToLongBufferSafe
.global     swapShortBuffer
.func         swapShortBuffer

void         swapIndirectLong( unsigned long *, unsigned long * );
void         swapIndirectShort( unsigned short *, unsigned short * );
void         copyIndirectLong( unsigned long *, unsigned long * );
void         copyIndirectShort( unsigned short *, unsigned short * );
#endif
/******************************************************************************
 Imported global variables and functions (from other files)
 *****************************************************************************/

/******************************************************************************
 * Function Name: swapLong
 * Description  : The power on reset function
 * Arguments    : none
 * Return Value : none
 *****************************************************************************/

uint32_t swapLong(uint32_t val)
{
    __asm volatile (
        "eor     r3, %1, %1, ror #16\n\t"
        "bic     r3, r3, #0x00FF0000\n\t"
        "mov     %0, %1, ror #8\n\t"
        "eor     %0, %0, r3, lsr #8"
        : "=r" (val)
        : "0"(val)
        : "r3"
);
return val;
}

/******************************************************************************
;* Function Name: swapIndirectLong
;* Description  : Function to swap the byte order of a LONG
;* Arguments    : R4 - Pointer to the destination LONG
;                 R5 - Pointer to the source LONG
;* Return Value : none
;*****************************************************************************/
/*
_swapIndirectLong:
        ADD         #4,R4
        MOV.B       @R5+,R0
        MOV.B       R0,@-R4
        MOV.B       @R5+,R0
        MOV.B       R0,@-R4
        MOV.B       @R5+,R0
        MOV.B       R0,@-R4
        MOV.B       @R5+,R0
        RTS
        MOV.B       R0,@-R4
        .pool
*/
void swapIndirectLongXXX(uint32_t *pulDest, uint32_t *pulSrc);
void swapIndirectLongXXX(uint32_t *pulDest, uint32_t *pulSrc)
{
    //TDOO: incorrect code!!!!!
    __asm volatile
    (
            "add     %0, %0, #4\n\t" /*ADD         #4,R4*/
/*            "ldr*/
            : "=r"  (pulDest)    /*C variables modified by the instructions */
            : "0"  (pulSrc)     /*C expressions read by the instructions */
            : "r0"                    /*registers or other values changed*/
    );
}

void swapIndirectLong(uint32_t *pulDest, uint32_t *pulSrc)
{
    uint32_t work;
    work = *pulSrc;
    work = swapLong(work);
    *pulDest = work;
}


/******************************************************************************
;* Function Name: copyIndirectShort
;* Description  : Function to copy a short to a destination irrespective of
;                 alignment
;* Arguments    : pulDest - Pointer to the destination SHORT
;                 pulSrc - Pointer to the source SHORT
;* Return Value : none
;*****************************************************************************/
/*
_copyIndirectShort:
        MOV.B       @R5+,R0
        MOV.B       R0,@R4+
        MOV.B       @R5+,R0
        MOV.B       R0,@R4+
        RTS
        NOP
        .pool
*/
void copyIndirectShort(uint16_t *pulDest, uint16_t *pulSrc)
{
    uint8_t work;
    volatile uint8_t * psrc;
    volatile uint8_t * pdst;
    psrc = (uint8_t *) pulSrc;
    pdst = (uint8_t *) pulDest;
    work =  *psrc++;
    *pdst++ = work;
    work =  *psrc++;
    *pdst++ = work;
}

/******************************************************************************
;* Function Name: copyIndirectLong
;* Description  : Function to copy a short to a destination irrespective of
;                 alignment
;* Arguments    : pulDest - Pointer to the destination LONG
;                 pulSrc - Pointer to the source LONG
;* Return Value : none
;*****************************************************************************/
/*
_copyIndirectLong:
        MOV.B       @R5+,R0
        MOV.B       R0,@R4+
        MOV.B       @R5+,R0
        MOV.B       R0,@R4+
        MOV.B       @R5+,R0
        MOV.B       R0,@R4+
        MOV.B       @R5+,R0
        MOV.B       R0,@R4+
        RTS
        NOP
        .pool
*/
void copyIndirectLong(uint32_t *pulDest, uint32_t *pulSrc)
{
    uint8_t work;
    volatile uint8_t * psrc;
    volatile uint8_t * pdst;
    psrc = (uint8_t *) pulSrc;
    pdst = (uint8_t *) pulDest;
    work =  *psrc++;
    *pdst++ = work;
    work =  *psrc++;
    *pdst++ = work;
    work =  *psrc++;
    *pdst++ = work;
    work =  *psrc++;
    *pdst++ = work;
}
