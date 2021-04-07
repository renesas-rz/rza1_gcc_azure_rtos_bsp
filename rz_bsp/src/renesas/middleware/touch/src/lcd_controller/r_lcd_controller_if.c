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
* Copyright (C) 2013-2017 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/

/**************************************************************************//**
* @file         lcd_conroller_if.c
* $Rev: 30 $
* $Date:: 2016-12-21 12:02:44 +0900#$
* @brief        LCD Driver API functions
******************************************************************************/
/*******************************************************************************
Includes <System Includes>, "Project Includes"
*******************************************************************************/
#include    <stdio.h>
#include    <string.h>

#include    "r_os_abstraction_api.h"
#include    "tx_api.h"
#include    "r_typedefs.h"
#include    "mcu_board_select.h"
#include    "lcd_controller_if.h"

#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
#include    "FT5x06/lcd_ft5x06.h"
#include    "FT5x06/lcd_ft5x06_int.h"
#elif (TARGET_BOARD == TARGET_BOARD_RSK)
#include    "FT5216/lcd_ft5216.h"
#include    "FT5216/lcd_ft5216_int.h"
#else
#error Unsupported BOARD.
#endif


/******************************************************************************
Macro definitions
******************************************************************************/


/******************************************************************************
 Enumerated Types
******************************************************************************/


/******************************************************************************
 Structures
******************************************************************************/


/******************************************************************************
 Global Vaiables
******************************************************************************/


/*******************************************************************************
Private global variables and functions
*******************************************************************************/
static int_t   lcd_init_cnt;


/**************************************************************************//**
* Function Name: R_LCD_Init
* @brief         Initialize LCD Driver.
*
*                Description:<br>
*
* @param         None.
* @retval        None.
******************************************************************************/
void R_LCD_Init( void )
{
    lcd_init_cnt = 0;
}


/**************************************************************************//**
* Function Name: R_LCD_Open
* @brief         Open LCD driver.
*
*                Description:<br>
*
* @param         [in]uint32_t unIrqLv   : IRQ interrupt level
* @retval          0 : Operation successfull.
*                 -1 : Error occured.
******************************************************************************/
int_t R_LCD_Open( const uint32_t unIrqLv, const int16_t nTskPri, const uint32_t unTskStk )
{
    int_t   n_ret;

    n_ret    = 0;

    if( lcd_init_cnt == 0 )
    {
        DBG_printf_MSG( "LCD board TI-FT5216 is opened\n" );
#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
        n_ret = LCD_Ft5x06_Open( unIrqLv, nTskPri, unTskStk );
#elif (TARGET_BOARD == TARGET_BOARD_RSK)
        n_ret = LCD_Ft5216_Open( unIrqLv, nTskPri, unTskStk );
#endif
        if( n_ret >= 0 )
        {
            R_OS_WaitForSemaphore( &sLcdSemIdAcc, R_OS_ABSTRACTION_PRV_EV_WAIT_INFINITE );
            lcd_init_cnt ++;
            R_OS_ReleaseSemaphore( &sLcdSemIdAcc );
        }
    }
    else if( lcd_init_cnt < 0 )
    {
        DBG_printf_ERR( "[ERROR] initialize count is underflow\n" );
        n_ret = -1;
    }
    else
    {
        R_OS_WaitForSemaphore( &sLcdSemIdAcc, R_OS_ABSTRACTION_PRV_EV_WAIT_INFINITE );
        lcd_init_cnt ++;
        R_OS_ReleaseSemaphore( &sLcdSemIdAcc );
    }

    return n_ret;
}


/**************************************************************************//**
* Function Name: R_LCD_Close
* @brief         Close LCD driver.
*
*                Description:<br>
*
* @param         None.
* @retval          0 : Operation successfull.
*                 -1 : Error occured.
******************************************************************************/
int_t R_LCD_Close( void )
{
    int_t   n_ret;

    n_ret    = 0;

    R_OS_WaitForSemaphore( &sLcdSemIdAcc, R_OS_ABSTRACTION_PRV_EV_WAIT_INFINITE );
    lcd_init_cnt --;
    R_OS_ReleaseSemaphore( &sLcdSemIdAcc );

    if( lcd_init_cnt == 0 )
    {
#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
        n_ret = LCD_Ft5x06_Close();
#elif (TARGET_BOARD == TARGET_BOARD_RSK)
        n_ret = LCD_Ft5216_Close();
#endif
        if( n_ret < 0 )
        {
            R_OS_WaitForSemaphore( &sLcdSemIdAcc, R_OS_ABSTRACTION_PRV_EV_WAIT_INFINITE );
            lcd_init_cnt ++;
            R_OS_ReleaseSemaphore( &sLcdSemIdAcc );
        }
    }
    else if( lcd_init_cnt < 0 )
    {
        DBG_printf_ERR( "[ERROR] initialize count is underflow\n" );
        n_ret = -1;
    }
    else
    {
        /* do nothing */
    }

    return n_ret;
}


/**************************************************************************//**
* Function Name: R_LCD_WriteCmd
* @brief         Write command to LCD device via RIIC.
*
*                Description:<br>
*
* @param         [in]uint16_t unDevAddr : device address
* @param         [in]uint8_t uData      : data
* @param         [in]uint32_t unSize    : size of data
* @retval          0 : Operation successfull.
*                 -1 : Error occured.
******************************************************************************/
uint8_t R_LCD_WriteCmd( const uint16_t unDevAddr, const uint8_t uCmd, const uint8_t uData, const uint32_t unSize )
{
    uint8_t   n_ret;
    UNUSED_PARAM(uCmd);
#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
    n_ret = LCD_Ft5x06_WriteCmd( unDevAddr, uData, unSize );
#elif (TARGET_BOARD == TARGET_BOARD_RSK)
    n_ret = LCD_Ft5216_WriteCmd( unDevAddr, uData, unSize );
#endif

    return n_ret;
}


/**************************************************************************//**
* Function Name: R_LCD_ReadCmd
* @brief         Write command to LCD device and read data via RIIC.
*
*                Description:<br>
*
* @param         [in]uint16_t unDevAddr : device address
* @param         [out]uint8_t *puData   : pointer to receive buffer
* @param         [in]uint32_t unSize    : size of data
* @retval          0 : Operation successfull.
*                 -1 : Error occured.
******************************************************************************/
uint8_t R_LCD_ReadCmd( const uint16_t unDevAddr, const uint8_t uCmd, uint8_t *puData, const uint32_t unSize )
{
    uint8_t   n_ret;
    UNUSED_PARAM(uCmd);

#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
    n_ret = LCD_Ft5x06_ReadCmd(unDevAddr, puData, unSize);
#elif (TARGET_BOARD == TARGET_BOARD_RSK)
    n_ret = LCD_Ft5216_ReadCmd(unDevAddr, puData, unSize);
#endif

    return n_ret;
}


/**************************************************************************//**
* Function Name: R_LCD_EventEntry
* @brief         Enter callback event to event table.
*
*                Description:<br>
*
* @param         [in]LcdEvt_EntryType eType : event type
* @param         [in]void *function         : callback function
* @retval          0..(LCDEVT_ENTRY_MAX-1) : event ID.
*                 -1                       : Error occured.
******************************************************************************/
int_t R_LCD_EventEntry( const LcdEvt_EntryType eType, const LcdCBFunc function )
{
    int_t   n_ret;

#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
    n_ret = LCD_Ft5x06_EventEntry( eType, function );
#elif (TARGET_BOARD == TARGET_BOARD_RSK)
    n_ret = LCD_Ft5216_EventEntry( eType, function );
#endif

    return n_ret;
}


/**************************************************************************//**
* Function Name: R_LCD_EventErase
* @brief         Erase callback event from event table.
*
*                Description:<br>
*
* @param         [in]int_t nId              : event ID
* @retval          0 : Operation successfull.
*                 -1 : Error occured.
******************************************************************************/
int_t R_LCD_EventErase( const int_t nId )
{
    int_t   n_ret;

#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
    n_ret = LCD_Ft5x06_EventErase( nId );
#elif (TARGET_BOARD == TARGET_BOARD_RSK)
    n_ret = LCD_Ft5216_EventErase( nId );
#endif

    return n_ret;
}


/**************************************************************************//**
* Function Name: R_LCD_StartInt
* @brief         Enable interrupt of assigned type.
*
*                Description:<br>
*
* @param         [in]LcdEvt_EntryType eType : interrupt type
* @retval          0 : Operation successfull.
*                 -1 : Error occured.
******************************************************************************/
int_t R_LCD_StartInt( const LcdEvt_EntryType eType )
{
    UNUSED_PARAM(eType);

    int_t   n_ret;

#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
    n_ret = LCD_Ft5x06_Int_Start( );
#elif (TARGET_BOARD == TARGET_BOARD_RSK)
    n_ret = LCD_Ft5216_Int_Start( );
#endif

    return n_ret;
}


/**************************************************************************//**
* Function Name: R_LCD_Restart
* @brief         Reset LCD board.
*
*                Description:<br>
*
* @param         none.
* @retval          0 : Operation successfull.
******************************************************************************/
int_t R_LCD_Restart( void )
{
    DBG_printf_ERR( "[WARN] this function is not supported\n" );

    return 0;
}


/**************************************************************************//**
* Function Name: R_LCD_ReadVersion
* @brief         Read LCD FW version.
*
*                Description:<br>
*
* @param         [out]uint8_t *puData   : pointer to receive buffer
* @retval        None.
******************************************************************************/
void R_LCD_ReadVersion( uint8_t *puData )
{
    UNUSED_PARAM(puData);
    DBG_printf_ERR( "[WARN] this function is not supported\n" );
}


/**************************************************************************//**
* Function Name: R_LCD_SetBacklight
* @brief         Set bright level of backlight.
*
*                Description:<br>
*
* @param         [in]uint8_t uLevel     : bright level
* @retval        None.
******************************************************************************/
void R_LCD_SetBacklight( uint8_t uLevel )
{
    UNUSED_PARAM(uLevel);
    DBG_printf_ERR( "[WARN] this function is not supported\n" );
}


/**************************************************************************//**
* Function Name: R_LCD_SetBuzzer
* @brief         Set scale of buzzer.
*
*                Description:<br>
*
* @param         [in]uint8_t uScale     : scale
* @retval        None.
******************************************************************************/
void R_LCD_SetBuzzer( uint8_t uScale )
{
    UNUSED_PARAM(uScale);
    DBG_printf_ERR( "[WARN] this function is not supported\n" );
}

