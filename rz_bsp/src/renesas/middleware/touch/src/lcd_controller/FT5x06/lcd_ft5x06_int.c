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
* Copyright (C) 2017 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/

/**************************************************************************//**
* @file         lcd_ft5x06_int.c
* $Rev: 30 $
* $Date:: 2016-12-21 12:02:44 +0900#$
* @brief        LCD Driver interrupt functions
******************************************************************************/
/*******************************************************************************
Includes <System Includes>, "Project Includes"
*******************************************************************************/
#include    "mcu_board_select.h"
#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)

#include    <stdio.h>
#include    <string.h>

#include    "r_typedefs.h"
#include    "r_intc.h"
#include    "iodefine_cfg.h"

#include    "lcd_ft5x06.h"
#include    "lcd_ft5x06_int.h"


/******************************************************************************
 Macro definitions
******************************************************************************/


/******************************************************************************
 Enumerated Types
******************************************************************************/
/* Interrupt level */
typedef enum
{
    LCD_INT_LEVEL = 0,   /* Level interrupt */
    LCD_INT_EDGE  = 1    /* Edge interrupt */
} LCD_IntLv_et;


/******************************************************************************
 Structures
******************************************************************************/
typedef struct {
    uint32_t    unTotal;
    uint32_t    unTouch;
    uint32_t    unKey1;
    uint32_t    unKey2;
    uint32_t    unKey3;
    uint32_t    unErr;
} LCD_IntCnt_st;


/******************************************************************************
 Global Vaiables
******************************************************************************/


/*******************************************************************************
 Private global variables and functions
*******************************************************************************/
SCOPE_STATIC void LCD_Ft5x06_Int_Hdl(uint32_t int_sense);
SCOPE_STATIC void LCD_Ft5x06_Int_SetPort( void );

SCOPE_STATIC LCD_IntCnt_st gsIntCnt;


/**************************************************************************//**
* Function Name: LCD_Ft5x06_Int_Open
* @brief         Open LCD interrupt.
*
*                Description:<br>
*
* @param         [in]uint32_t unIrqLv   : IRQ interrupt level
* @retval          0 : Operation successfull.
*                 -1 : Error occured.
******************************************************************************/
int_t LCD_Ft5x06_Int_Open( const uint32_t unIrqLv )
{
    int_t       nRet;
    int32_t    unInhRet;

    DBG_printf_DBG( "[FUNC] LCD_Ft5x06_Int_Open()\n" );

    nRet = 0;

    memset( &gsIntCnt, 0, sizeof(LCD_IntCnt_st) );

    /* disable LCD interrupt */
    R_INTC_Disable( LCD_FT5x06_INT_NUM );

    LCD_Ft5x06_Int_SetPort();

    /** define interrupt handler */
    unInhRet = R_INTC_RegistIntFunc( LCD_FT5x06_INT_NUM, &LCD_Ft5x06_Int_Hdl );
    if( unInhRet != 0 )
    {
        DBG_printf_ERR( "[ERROR] interrupt handler is not registered\n" );
        nRet = -1;
    }
    else
    {
        /* set interrupt level */
        R_INTC_SetConfiguration( LCD_FT5x06_INT_NUM, INTC_LEVEL_SENSITIVE );
        /* set interrupt priority */
        R_INTC_SetPriority( LCD_FT5x06_INT_NUM, (uint8_t)unIrqLv );
    }

    return nRet;
}


/**************************************************************************//**
* Function Name: LCD_Ft5x06_Int_Close
* @brief         Close LCD interrupt.
*
*                Description:<br>
*
* @param         None.
* @retval          0 : Operation successfull.
*                 -1 : Error occured.
******************************************************************************/
int_t LCD_Ft5x06_Int_Close( void )
{
    int_t       nRet;

    nRet = 0;

    /* disable LCD interrupt */
    R_INTC_Disable( LCD_FT5x06_INT_NUM );

    return nRet;
}


/**************************************************************************//**
* Function Name: LCD_Ft5x06_Int_Start
* @brief         Enable interrupt of assigned type.
*
*                Description:<br>
*
* @param         None.
* @retval          0 : Operation successfull.
******************************************************************************/
int_t LCD_Ft5x06_Int_Start( void )
{
    /* enable LCD interrupt */
    R_INTC_Enable( LCD_FT5x06_INT_NUM );

    return 0;
}


/******************************************************************************
* Function Name: LCD_Ft5x06_Int_Hdl
* @brief         Interrupt handler function.
*
*                Description:<br>
*
* @param         uint32_t int_sense: not used.
* @retval        None.
******************************************************************************/
SCOPE_STATIC void LCD_Ft5x06_Int_Hdl(uint32_t int_sense)
{
    LCDEVT_ENTRY *psEvt;

    /* unused variable */
    (void)int_sense;

    /* disable LCD interrupt */
    R_INTC_Disable( LCD_FT5x06_INT_NUM );

    gsIntCnt.unTotal ++;

    /* send interrupt event to LCD_Task */
    psEvt = LCD_Ft5x06_GetEventTable( 0 );
    if( psEvt->mode == LCDEVT_ENTRY_TP )
    {
        psEvt->function( (void*)NULL );
    }
}


/**************************************************************************//**
* Function Name: LCD_Ft5x06_Int_SetPort
* @brief         Set port to use IRQ interrupt.
*
*                Description:<br>
*
* @param         None.
* @retval        None.
******************************************************************************/
SCOPE_STATIC void LCD_Ft5x06_Int_SetPort( void )
{
    int32_t was_masked;

#if defined (__ICCARM__)
    was_masked = __disable_irq_iar();
#else
    was_masked = (int32_t)__disable_irq();
#endif
    /* Initialise Touch Panel Interrupt. Set TP_INT (P1_11) as IRQ3 */
    /* P1_11=IRQ3(Alternative Mode 2,Input: PFC1) */

    /* Port initialize */
    GPIO.PIBC1  &= (uint16_t)~(GPIO_BIT_N11);   /* Input buffer disable */
    GPIO.PBDC1  &= (uint16_t)~(GPIO_BIT_N11);   /* Bidirectional mode disable */
    GPIO.PM1    |= (uint16_t) (GPIO_BIT_N11);   /* Input mode */
    GPIO.PMC1   &= (uint16_t)~(GPIO_BIT_N11);   /* Port mode */
    GPIO.PIPC1  &= (uint16_t)~(GPIO_BIT_N11);   /* Selected by PM bit */

    /* Port input function setting */
    GPIO.PBDC1  &= (uint16_t)~(GPIO_BIT_N11);   /* Bidirectional mode disable */
    GPIO.PFC1   |= (uint16_t) (GPIO_BIT_N11);   /* 2th convertible */
    GPIO.PFCE1  &= (uint16_t)~(GPIO_BIT_N11);   /* 2th convertible */
    GPIO.PFCAE1 &= (uint16_t)~(GPIO_BIT_N11);   /* 2th convertible */

    /* Port setting */
    GPIO.PIPC1  |= (uint16_t) (GPIO_BIT_N11);   /* Selected by peripheral */
    GPIO.PMC1   |= (uint16_t) (GPIO_BIT_N11);   /* Convertible mode */

    if (0 == was_masked)
    {
        __enable_irq();
    }
}
#endif /* TARGET_BOARD == TARGET_BOARD_STREAM_IT2 */
