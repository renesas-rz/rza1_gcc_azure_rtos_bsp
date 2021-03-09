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
* Copyright (C) 2015-2017 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/**************************************************************************//**
* @file         exclusion_func.c
* $Rev: 6174 $
* $Date:: 2018-01-15 14:12:23 +0000#$
* @brief        The exclusion function for the compatibility with
                the ARM compiler built-in function.
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/

#include "r_typedefs.h"

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/
void __enable_irq(void);
uint32_t __disable_irq(void);
int __strex(unsigned int val, volatile char *ptr);
void __clrex(void);
unsigned int __ldrex(volatile char *ptr);

/******************************************************************************
* Function Name: __enable_irq
* @brief         Enables IRQ interrupts
*
*                Description:<br>
*                The following module refers to this.
*                  CMSIS-RTOS RTX
*                  IOIF
*                  Sample drivers
*                  Sample programs
*
* @param         none
* @retval        none
******************************************************************************/
void __enable_irq(void)
{
    __asm volatile ("cpsie i");
}
/******************************************************************************
End of function __enable_irq
******************************************************************************/

/******************************************************************************
* Function Name: __disable_irq
* @brief         Disables IRQ interrupts
*
*                Description:<br>
*                The following module refers to this.
*                  CMSIS-RTOS RTX
*                  IOIF
*                  Sample drivers
*                  Sample programs
*
* @param         none
* @retval        zero     : Interrupt was enabled before call.
*                non zero : Interrupt was disabled before call.
******************************************************************************/
uint32_t __disable_irq(void)
{
    uint32_t result;

    __asm volatile ("mrs %0, cpsr" : "=r" (result));
    __asm volatile ("cpsid i");
    return(result & 0x80);
}
/******************************************************************************
End of function __disable_irq
******************************************************************************/

/******************************************************************************
* Function Name: __strex
* @brief         Generate STREXB instruction.
*
*                Description:<br>
*                The following module refers to this.
*                  IOIF
*
* @param[in]     val   : value to be written to memory
* @param[in]     ptr   : address of the data (must be unsigned byte)
* @retval        0     : Write to ptr isn't locked and succeeds.
* @retval        1     : Write to ptr is locked and fails.
******************************************************************************/
int __strex(unsigned int val, volatile char *ptr)
{
    int ret;

    __asm volatile ("strexb %0, %2, [%1]" : "=&r" (ret) : "r" (ptr), "r" (val));

    return ret;
}
/******************************************************************************
End of function __strex
******************************************************************************/

/******************************************************************************
* Function Name: __clrex
* @brief         Generate CLREX instruction.
*
*                Description:<br>
*                The following module refers to this.
*                  IOIF
*                
* @param         none
* @retval        none
******************************************************************************/
void __clrex(void)
{
    __asm volatile ("clrex");
}
/******************************************************************************
End of function __clrex
******************************************************************************/

/******************************************************************************
* Function Name: __ldrex
* @brief         Generate LDREXB instruction.
*
*                Description:<br>
*                The following module refers to this.
*                  IOIF
*                
* @param[in]     ptr   : address of the data (must be unsigned byte)
* @retval        memory value of ptr address.
******************************************************************************/
unsigned int __ldrex(volatile char *ptr)
{
    uint8_t val;

    __asm volatile ("ldrexb %0, [%1]" : "=r" (val) : "r" (ptr));
    return (unsigned int)val;
}
/******************************************************************************
End of function __ldrex
******************************************************************************/

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/

