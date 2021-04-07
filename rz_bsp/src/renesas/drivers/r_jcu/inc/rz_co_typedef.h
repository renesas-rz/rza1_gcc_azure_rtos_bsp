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
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/***********************************************************************
* File: rz_co_typedef.h
*    Common Types.
*
* - $Rev: $
* - $Date: 2018-02-07T11:44:56+09:00 $
************************************************************************/

#ifndef RZ_CO_TYPEDEF_H
#define RZ_CO_TYPEDEF_H


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/******************************************************************************
Macro definitions
******************************************************************************/

/* Section: Start_of_ErrNum */
/***********************************************************************
* Constants: Error code of global errnum_t type
************************************************************************/

/***********************************************************************
* Constant: E_OTHER
*    Error code that means parameter error or other error.
************************************************************************/
/* Constant: E_OTHER_MASK
*    Mask for checking E_OTHER. */
#define  E_OTHER_MASK              0x7F000000
#define  E_OTHER                   0x01000000


/***********************************************************************
* Constant: E_FEW_ARRAY
*    Error code that means error of few fixed length array.
************************************************************************/
/* Constant: E_FEW_ARRAY_MASK
*    Mask for checking E_FEW_ARRAY. */
#define  E_FEW_ARRAY_MASK          0x7F000000
#define  E_FEW_ARRAY               0x02000000


/***********************************************************************
* Constant: E_FEW_MEMORY
*    Error code that means few heap memory area.
************************************************************************/
/* Constant: E_FEW_MEMORY_MASK
*    Mask for checking E_FEW_MEMORY. */
#define  E_FEW_MEMORY_MASK         0x7F000000
#define  E_FEW_MEMORY              0x03000000


/***********************************************************************
* Constant: E_FIFO_OVER
*    Error code that means failed to enqueue.
************************************************************************/
/* Constant: E_FIFO_OVER_MASK
*    Mask for checking E_FIFO_OVER. */
#define  E_FIFO_OVER_MASK          0x7F000000
#define  E_FIFO_OVER               0x04000000


/***********************************************************************
* Constant: E_NOT_FOUND_SYMBOL
*    Error code that means not defined the symbol.
************************************************************************/
/* Constant: E_NOT_FOUND_SYMBOL_MASK
*    Mask for checking E_NOT_FOUND_SYMBOL. */
#define  E_NOT_FOUND_SYMBOL_MASK   0x7F000000
#define  E_NOT_FOUND_SYMBOL        0x05000000


/***********************************************************************
* Constant: E_NO_NEXT
*    Error code that means there is not next element of list.
************************************************************************/
/* Constant: E_NO_NEXT_MASK
*    Mask for checking E_NO_NEXT. */
#define  E_NO_NEXT_MASK            0x7F000000
#define  E_NO_NEXT                 0x06000000


/***********************************************************************
* Constant: E_ACCESS_DENIED
*    Error code that means error of denied read or write.
************************************************************************/
/* Constant: E_ACCESS_DENIED_MASK
*    Mask for checking E_ACCESS_DENIED. */
#define  E_ACCESS_DENIED_MASK      0x7F000000
#define  E_ACCESS_DENIED           0x07000000


/***********************************************************************
* Constant: E_NOT_IMPLEMENT_YET
*    Error code that means not implemented yet.
************************************************************************/
/* Constant: E_NOT_IMPLEMENT_YET_MASK
*    Mask for checking E_NOT_IMPLEMENT_YET. */
#define  E_NOT_IMPLEMENT_YET_MASK  0x7F000000
#define  E_NOT_IMPLEMENT_YET       0x09000000


/***********************************************************************
* Constant: E_OBLIVIOUS
*    Error code that means failure oblivious logged.
************************************************************************/
/* Constant: E_OBLIVIOUS_MASK
*    Mask for checking E_OBLIVIOUS. */
#define  E_OBLIVIOUS_MASK          0x7F000000
#define  E_OBLIVIOUS               0x0B000000


/***********************************************************************
* Constant: E_UNRECOVERABLE
*    Error code that means unrecoverable by self.
************************************************************************/
/* Constant: E_UNRECOVERABLE_MASK
*    Mask for checking E_UNRECOVERABLE. */
#define  E_UNRECOVERABLE_MASK      0x7F000000
#define  E_UNRECOVERABLE           0x0D000000


/***********************************************************************
* Constant: E_ERRNO
*    Error code that means Refer to "errno" of standard I/O.
************************************************************************/
/* Constant: E_ERRNO_MASK
*    Mask for checking E_ERRNO. */
#define  E_ERRNO_MASK              0x7F000000
#define  E_ERRNO                   0x0E000000


/***********************************************************************
* Constant: E_LIMITATION
*    Error code that means a limitation.
************************************************************************/
/* Constant: E_LIMITATION_MASK
*    Mask for checking E_LIMITATION. */
#define  E_LIMITATION_MASK         0x7F000000
#define  E_LIMITATION              0x0F000000


/***********************************************************************
* Constant: E_STATE
*    Error code that means it cannot do at this state.
************************************************************************/
/* Constant: E_STATE_MASK
*    Mask for checking E_STATE. */
#define  E_STATE_MASK              0x7F000000
#define  E_STATE                   0x10000000


/***********************************************************************
* Constant: E_NOT_THREAD
*    Error code that means not a thread, it cannot be called from interrupt context.
************************************************************************/
/* Constant: E_NOT_THREAD_MASK
*    Mask for checking E_NOT_THREAD. */
#define  E_NOT_THREAD_MASK         0x7F000000
#define  E_NOT_THREAD              0x11000000


/***********************************************************************
* Constant: E_THREAD_DENIED
*    Error code that means the object is locked by other thread.
************************************************************************/
/* Constant: E_THREAD_DENIED_MASK
*    Mask for checking E_THREAD_DENIED. */
#define  E_THREAD_DENIED_MASK      0x7F000000
#define  E_THREAD_DENIED           0x12000000


/***********************************************************************
* Constant: E_PATH_NOT_FOUND
*    Error code that means not found file or folder.
************************************************************************/
/* Constant: E_PATH_NOT_FOUND_MASK
*    Mask for checking E_PATH_NOT_FOUND. */
#define  E_PATH_NOT_FOUND_MASK     0x7F000000
#define  E_PATH_NOT_FOUND          0x13000000


/***********************************************************************
* Constant: E_BAD_COMMAND_ID
*    Error code that means out of number of command ID.
************************************************************************/
/* Constant: E_BAD_COMMAND_ID_MASK
*    Mask for checking E_BAD_COMMAND_ID. */
#define  E_BAD_COMMAND_ID_MASK     0x7F000000
#define  E_BAD_COMMAND_ID          0x16000000


/***********************************************************************
* Constant: E_TIME_OUT
*    Error code that means time out.
************************************************************************/
/* Constant: E_TIME_OUT_MASK
*    Mask for checking E_TIME_OUT. */
#define  E_TIME_OUT_MASK           0x7F000000
#define  E_TIME_OUT                0x17000000


/***********************************************************************
* Constant: E_STACK_OVERFLOW
*    Error code that means stack overflow.
************************************************************************/
/* Constant: E_STACK_OVERFLOW_MASK
*    Mask for checking E_STACK_OVERFLOW. */
#define  E_STACK_OVERFLOW_MASK     0x7F000000
#define  E_STACK_OVERFLOW          0x1C000000


/***********************************************************************
* Constant: E_NO_DEBUG_TLS
*    Error code that means not set debug work area.
************************************************************************/
/* Constant: E_NO_DEBUG_TLS_MASK
*    Mask for checking E_NO_DEBUG_TLS. */
#define  E_NO_DEBUG_TLS_MASK       0x7F000000
#define  E_NO_DEBUG_TLS            0x1D000000


/***********************************************************************
* Constant: E_EXIT_TEST
*    Error code that means a request of exit from the test.
************************************************************************/
/* Constant: E_EXIT_TEST_MASK
*    Mask for checking E_EXIT_TEST. */
#define  E_EXIT_TEST_MASK          0x7F000000
#define  E_EXIT_TEST               0x1E000000


/***********************************************************************
* Constants: Error code of local errnum_t type
************************************************************************/

/* Sub class of E_ACCESS_DENIED */
#define  E_CO_ACCESS_DENIED_1    0x07000001

/* Section: End_of_ErrNum */

/******************************************************************************
Typedef definitions
******************************************************************************/

/***********************************************************************
* Type: bit_flags_fast32_t
*    Bit flags as "uint_fast32_t"
************************************************************************/
typedef uint_fast32_t  bit_flags_fast32_t;


/***********************************************************************
* Type: bit_flags32_t
*    Bit flags as "uint32_t"
************************************************************************/
typedef uint32_t  bit_flags32_t;


/***********************************************************************
* Type: bit_flags16_t
*    Bit flags as "uint16_t"
************************************************************************/
typedef uint16_t  bit_flags16_t;


/***********************************************************************
* Type: bit_flags8_t
*    Bit flags as "uint8_t"
************************************************************************/
typedef uint8_t  bit_flags8_t;


/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/

/***********************************************************************
* Function: r_co_function_t
*    The function type of general callback
*
* Arguments:
*    in_Argument - A parameter to be passed when the callback function is called
*
* Return Value:
*    None
*
* Example:
*    > errnum_t  R_DRIVER_TransferStart( r_co_function_t  in_CallbackFunction,
*    >     volatile void*  in_Argument )
*    > {
*    >         :
*    >     in_CallbackFunction( in_Argument );  // R_CO_SetTrue( &event_A );
*    > }
*    >
*    > e= R_DRIVER_TransferStart( (r_co_function_t) R_CO_SetTrue,  &event_A );
************************************************************************/
typedef void  (* r_co_function_t )( volatile void* in_Argument );


/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/

/***********************************************************************
* Function: r_co_intc_function_t
*    The function type of interrupt callback for type cast
*
* Arguments:
*    Arguments are depended on RTOS.
*
* Return Value:
*    Return value is depended on RTOS.
*
* Description:
*    Define an interrupt handler that has no arguments,
*    even if the interrupt handler caller has arguments.
*
* Example:
*    > void  R_DRIVER_OnInterrupt(void)
*    > {
*    >         :
*    > }
*    >
*    > e= R_INTC_RegistIntFunc( (r_co_intc_function_t) &( R_DRIVER_OnInterrupt ) );  // MISRA 16.9
************************************************************************/
typedef void  (* r_co_intc_function_t )( uint32_t  in_Ignored );


/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/

/***********************************************************************
* Constant: BSP_CFG_RTOS_USED
*    Whether it is supported to preemption of RTOS.
*
* Description:
*    If it is for OS less, BSP_CFG_RTOS_USED is 0.
*    Even if it is not round robin, BSP_CFG_RTOS_USED is 1, if interrupt and system call do preemption.
************************************************************************/
#define  BSP_CFG_RTOS_USED  0


/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/

/***********************************************************************
* Type: errnum_t
*    Error code. 0=NoError.
*
* Description:
*    Error code is defined in *_errnum.h file.
*    Bit field in the error node is:
*
*    > 3       2         1
*    > 1       4         6         8         0
*    > ---------------------------------------
*    > 0GGG GGGG MMMM MMMM SSSS SSSS IIII IIII
*    >
*    > 0 - 0 only
*    > G - Defined in global error code
*    > M - Defined in each module. ID of the module
*    > S - Defined in each module. Error code of the module
*    > I - Defined in each module. Internal ID
*
*    Error code must be compared with mask.
*    Because internal ID must be ignored.
*    > if ( ( e & E_FEW_MEMORY_MASK ) == E_FEW_MEMORY ) { }
************************************************************************/
typedef int  errnum_t;


/******************************************************************************
Variable Externs
******************************************************************************/

/***********************************************************************
* End of File:
************************************************************************/
#ifdef __cplusplus
}  /* extern "C" */
#endif /* __cplusplus */

#endif /* RZ_CO_TYPEDEF_H */
