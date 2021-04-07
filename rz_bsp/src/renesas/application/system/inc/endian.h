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
*******************************************************************************
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************
* File Name    : endian.h
* Version      : 1.0
* Device(s)    : Renesas
* Tool-Chain   : GNUARM-NONE-EABI v14.02
* OS           : None
* H/W Platform : RSK+
* Description  : Functions to swap the endian of shorts and longs.
*******************************************************************************
* History      : DD.MM.YYYY Version Description
*              : 05.08.2010 1.00    First Release
******************************************************************************/

/******************************************************************************
  WARNING!  IN ACCORDANCE WITH THE USER LICENCE THIS CODE MUST NOT BE CONVEYED
  OR REDISTRIBUTED IN COMBINATION WITH ANY SOFTWARE LICENSED UNDER TERMS THE
  SAME AS OR SIMILAR TO THE GNU GENERAL PUBLIC LICENCE
******************************************************************************/

#ifndef ENDIAN_H_INCLUDED
#define ENDIAN_H_INCLUDED

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"

/******************************************************************************
Macro definitions
******************************************************************************/

/* Note: The definition of _BIG is a specific compiler definition depending
   upon the compiler configuration settings */

#ifdef _BIG
#define SWAP_ENDIAN_LONG(dw)        swapLong(dw)
#define SWAP_ENDIAN_LONG_AT(d,s)    swapIndirectLong(d,s)
#define SWAPDB(db)                  swapDB(db)
/* Renesas C compiler provides swapb as an intrinsic function through
   machine.h */ 
#define SWAP_ENDIAN_SHORT(w)        swapb((uint16_t)w)
#define SWAP_ENDIAN_SHORT_AT(d,s)   swapIndirectShort(d,s)

#else
/* Below macros may not work with RISC machines due to alignment. This is
   outside the scope of this sample code */

#define SWAP_ENDIAN_LONG(dw)        dw
#define SWAP_ENDIAN_LONG_AT(d,s)    (*((uint32_t *)d) = (*((uint32_t *)s)))
#define SWAP_ENDIAN_SHORT(w)        w
#define SWAP_ENDIAN_SHORT_AT(d,s)   (*((uint16_t *)d) = (*((uint16_t *)s)))
#endif

/******************************************************************************
Function Prototypes
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
* Function Name: swapDB
* Description  : Function to swap the bytes in the words of a double word
* Parameters   : ulValue - The LONG to swap
* Return value : The swapped LONG
******************************************************************************/

extern  uint32_t swapDB(uint32_t ulValue);

/******************************************************************************
* Function Name: swapLong
* Description  : Function to swap the endian of a long
* Parameters   : ulValue - The long to swap
* Return value : The endian swapped long
******************************************************************************/

extern  uint32_t swapLong(uint32_t ulValue);

/******************************************************************************
* Function Name: swapLongBuffer
* Description  : Function to swap the endian of a long buffer
* Parameters   : IN  pulBuffer - Pointer to the buffer of longs to swap
*              : IN  stLength 
* Return value : The endian swapped long
******************************************************************************/

extern  void swapLongBuffer(uint32_t *pulBuffer, size_t stLength);

/******************************************************************************
* Function Name: swapLongBufferSafe
* Description  : Function to swap the endian of a long buffer irrispective of
*                alignment
* Parameters   : IN  pbyBuffer - Pointer to the buffer of longs to swap
*              : IN  stLength 
* Return value : The endian swapped long
******************************************************************************/

extern  void swapLongBufferSafe(uint8_t *pbyBuffer, size_t stLength);

/******************************************************************************
* Function Name: swapToLongBufferSafe
* Description  : Function to swap the endian of a long buffer irrispective of
*                the aligment to a destination buffer
* Arguments    : OUT pulDest - Pointer to the destination buffer of longs
*              : IN  pulSrc - Pointer to the buffer of longs to swap
*              : IN  stLength - The number of longs to swap
* Return Value : none
******************************************************************************/

extern  void swapToLongBufferSafe(uint32_t *pulDest,
                                  uint32_t *pulSrc,
                                  size_t    stLength);

/******************************************************************************
* Function Name: swapIndirectLong
* Description  : Function to swap the endian of a long via a pointer to avoid
*                alignment restrictions of a RISC machine
* Parameters   : OUT pulDest - Pointer to the destination long
*                IN  pulSrc - Pointer to the source long
* Return value : none
******************************************************************************/

extern void swapIndirectLong(uint32_t *pulDest, uint32_t *pulSrc);

/******************************************************************************
* Function Name: swapShort
* Description  : Function to swap the endian of a short
* Parameters   : usValue - The short to swap
* Return value : The endian swapped short
******************************************************************************/

extern  uint16_t swapShort(uint16_t usValue);

/******************************************************************************
* Function Name: swapIndirectShort
* Description  : Function to swap the endian of a short via a pointer to avoid
*                alignment restrictions of a RISC machine
* Parameters   : OUT pusDest - Pointer to the destination short
*                IN  pusSrc - Pointer to the source short
* Return value : none
******************************************************************************/

extern void swapIndirectShort(uint16_t *pusDest, uint16_t *pusSrc);

/******************************************************************************
* Function Name: copyIndirectLong
* Description  : Function to copy an indirect long to a destination
*                irrespective of the alignment of the destination or source
* Parameters   : OUT pulDest - Pointer to the destination
*                IN  pulSrc - Pointer to the source
* Return value : none
******************************************************************************/

extern void copyIndirectLong(uint32_t *pulDest, uint32_t *pulSrc);

/******************************************************************************
* Function Name: copyIndirectShort
* Description  : Function to copy an indirect shot to a destination
*                irrespective of the alignment of the destination or source
* Parameters   : OUT pulDest - Pointer to the destination
*                IN  pulSource - Pointer to the source
* Return value : none
******************************************************************************/

extern void copyIndirectShort(uint16_t *pulDest, uint16_t *pulSrc);

#ifdef __cplusplus
}
#endif

#endif                              /* ENDIAN_H_INCLUDED */

/******************************************************************************
End  Of File
******************************************************************************/
