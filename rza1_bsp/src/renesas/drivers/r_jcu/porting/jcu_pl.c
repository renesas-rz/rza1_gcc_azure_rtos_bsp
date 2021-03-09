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
* Copyright (C) 2012 - 2017 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File: jcu_pl.c
*    JPEG Codec Unit (JCU) Sample Driver. OS Porting Layer. For RZ/A1.
*
* - $Version: $ (=JCU_VERSION)
* - $ModuleRevision: {Masters}\JCU\RZ2\Single\04_RZA1 $
* - $Rev: $
* - $Date: 2018-03-06T18:08:22+09:00 $
******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include  "r_typedefs.h"
#include  "jcu_iodefine.h"
#include  "jcu_iobitmask.h"
#include  "cpg_iodefine.h"
#include  "cpg_iobitmask.h"
#include  "r_intc.h"
#include  "r_jcu.h"
#include  "../src/r_jcu_pl.h"
#include  "rz_co.h"

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/

/***********************************************************************
* Constant: JCU_INT_PRIORITY
************************************************************************/
enum { JCU_INT_PRIORITY = 30 };


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
void  JCU_IRQ_JEDI_Handler(void);
void  JCU_IRQ_JDTI_Handler(void);
static bool_t  gs_IsEnabledAllInterrupt;


/***********************************************************************
* Function: R_JCU_OnInitialize
*    OnInitialize
*
* Arguments:
*    None.
*
* Return Value:
*    Error code, 0=No error.
************************************************************************/
errnum_t  R_JCU_OnInitialize(void)
{
    int32_t   er;
    errnum_t  e;


    gs_IsEnabledAllInterrupt = true;


    /* Register "JEDI" */
    er= R_INTC_RegistIntFunc( INTC_ID_JEDI,  (r_co_intc_function_t) &( JCU_IRQ_JEDI_Handler ) );  /* MISRA 16.9 */
    if ( er != DEVDRV_SUCCESS )
    {
        e=JCU_ERROR_PARAM_61;    /* Error: RegistIntFunc */
        goto fin;
    }

    er= R_INTC_SetPriority( INTC_ID_JEDI,  JCU_INT_PRIORITY );
    if ( er != DEVDRV_SUCCESS )
    {
        e=JCU_ERROR_PARAM_62;    /* Error: IntPriority */
        goto fin;
    }


    /* Register "JDTI" */
    er= R_INTC_RegistIntFunc( INTC_ID_JDTI,  (r_co_intc_function_t) &( JCU_IRQ_JDTI_Handler ) );  /* MISRA 16.9 */
    if ( er != DEVDRV_SUCCESS )
    {
        e=JCU_ERROR_PARAM_63;    /* Error: RegistIntFunc */
        goto fin;
    }

    er= R_INTC_SetPriority( INTC_ID_JDTI,  JCU_INT_PRIORITY );
    if ( er != DEVDRV_SUCCESS )
    {
        e=JCU_ERROR_PARAM_64;    /* Error: IntPriority */
        goto fin;
    }


    /* start to suuply the clock for JCU */
    {
        /* ->QAC 0306 */
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif
        volatile uint8_t  dummy;
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

        /* TARGET_RZA1 */
        R_CO_SET_BIT_IN_REGISTER8( CPG,STBCR6, MSTP61,  0 );  /* 0 = Supply the clock of JCU */
        dummy = R_CO_REGISTER8( CPG.STBCR6 );  /* Dummy read for operation completely */
    } /* <-QAC 0306 */

    e=0;
fin:
    R_CO_SetErrNum( e );
    return  e;
}


/***********************************************************************
* Function: R_JCU_OnFinalize
*    OnFinalize
*
* Arguments:
*    None.
*
* Return Value:
*    Error code or e, 0 = successful and input e=0.
************************************************************************/
/* ->QAC 3227 : "e" is usually changed in finalize function. */
errnum_t  R_JCU_OnFinalize(void)
/* <-QAC 3227 */
{
    /* stop to suuply the clock for JCU */
    {
        /* ->QAC 0306 */
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif
        volatile uint8_t  dummy;
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

        /* TARGET_RZA1 */
        R_CO_SET_BIT_IN_REGISTER8( CPG,STBCR6, MSTP61,  1u );  /* 1u = Stop the clock of JCU */
        dummy = R_CO_REGISTER8( CPG.STBCR6 );  /* Dummy read for operation completely */
    } /* <-QAC 0306 */

    return  0;
}


/***********************************************************************
* Function: R_JCU_DisableInterrupt
*    Start I-Lock
*
* Arguments:
*    None.
*
* Return Value:
*    Whether interrupts of this function was enabled.
*
* Example:
*    > void  Func()
*    > {
*    >     bool_t  was_all_enabled = false;
*    >
*    >     was_all_enabled = R_..._DisableAllInterrupt();
*    >
*    >     // All interrupt disabled
*    >
*    >     if ( was_all_enabled )
*    >         { R_..._EnableAllInterrupt(); }
*    > }
************************************************************************/
bool_t  R_JCU_DisableInterrupt(void)
{
    int32_t   er;
    errnum_t  e;
    int32_t   oblivious = 0;
    bool_t    is_enabled_all_interrupt = gs_IsEnabledAllInterrupt;

    gs_IsEnabledAllInterrupt = false;

    er= R_INTC_Disable( (uint16_t) INTC_ID_JEDI );
    if ( er != DEVDRV_SUCCESS )
    {
        oblivious = __LINE__;
    }

    er= R_INTC_Disable( (uint16_t) INTC_ID_JDTI );
    if ( er != DEVDRV_SUCCESS )
    {
        oblivious = __LINE__;
    }

    if ( oblivious != 0 )
    {
        e=E_JCU_OBLIVIOUS_1;    /* Error: */
        goto fin;
    }
    e=0;
fin:
    R_CO_SetErrNum( e );
    return  is_enabled_all_interrupt;
}


/***********************************************************************
* Function: R_JCU_EnableInterrupt
*    End I-Lock
*
* Arguments:
*    None.
*
* Return Value:
*    None.
************************************************************************/
void  R_JCU_EnableInterrupt(void)
{
    int32_t   er;
    errnum_t  e;
    int32_t   oblivious = 0;

    gs_IsEnabledAllInterrupt = true;

    er= R_INTC_Enable( (uint16_t) INTC_ID_JEDI );
    if ( er != DEVDRV_SUCCESS )
    {
        oblivious = __LINE__;
    }

    er= R_INTC_Enable( (uint16_t) INTC_ID_JDTI );
    if ( er != DEVDRV_SUCCESS )
    {
        oblivious = __LINE__;
    }

    if ( oblivious != 0 )
    {
        e=E_JCU_OBLIVIOUS_2;    /* Error: */
        goto fin;
    }
    e=0;
fin:
    R_CO_SetErrNum( e );
}


/***********************************************************************
* Function: JCU_IRQ_JEDI_Handler
*    JEDI (JCU Encode Decode Interrupt) interrupt handler.
*
* Arguments:
*    None.
*
* Return Value:
*    None.
************************************************************************/
void  JCU_IRQ_JEDI_Handler()
{
    R_JCU_OnInterrupting();
}


/***********************************************************************
* Function: JCU_IRQ_JDTI_Handler
*    JDTI (JCU Data Transfer Interrupt) interrupt handler.
*
* Arguments:
*    None.
*
* Return Value:
*    None.
************************************************************************/
void  JCU_IRQ_JDTI_Handler()
{
    R_JCU_OnInterrupting();
}


