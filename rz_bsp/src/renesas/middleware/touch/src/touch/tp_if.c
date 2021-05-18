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
* @file         tp_if.c
* $Rev: 30 $
* $Date:: 2016-12-21 12:02:44 +0900#$
* @brief        TouchPanel Driver API functions
******************************************************************************/

/*******************************************************************************
Includes <System Includes>, "Project Includes"
*******************************************************************************/
#include    "r_typedefs.h"

#include    "tp.h"


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


/**************************************************************************//**
* Function Name: TouchPanel_Init
* @brief         Initialize TouchPanel driver.
*
*                Description:<br>
*
* @param         None.
* @retval        None.
******************************************************************************/
void TouchPanel_Init( void )
{
    TP_Init();
}


/**************************************************************************//**
* Function Name: TouchPanel_Open
* @brief         Open TouchPanel driver.
*
*                Description:<br>
*
* @param         [in]int_t nWidth       : screen width
* @param         [in]int_t nHeight      : screen height
* @param         [in]uint32_t unIrqLv   : IRQ interrupt level
* @param         [in]int16_t nTskPri    : task priority
* @param         [in]uint32_t unTskStk  : task stack size
* @retval          0 : Operation successfull.
*                 -1 : Error occured.
******************************************************************************/
int_t TouchPanel_Open( const int_t nWidth, const int_t nHeight, const uint32_t unIrqLv, const int16_t nTskPri, const uint32_t unTskStk )
{
    int_t       nRet;

    nRet = TP_Open( nWidth, nHeight, unIrqLv, nTskPri, unTskStk );

    return nRet;
}


/**************************************************************************//**
* Function Name: TouchPanel_Close
* @brief         Close TouchPanel driver.
*
*                Description:<br>
*
* @param         None.
* @retval          0 : Operation successfull.
*                 -1 : Error occured.
******************************************************************************/
int_t TouchPanel_Close( void )
{
    int_t       nRet;

    nRet = TP_Close();

    return nRet;
}


/**************************************************************************//**
* Function Name: TouchPanel_EventEntry
* @brief         Enter callback event to event table.
*
*                Description:<br>
*
* @param         [in]TpEvt_EntryType eMode  : event type
* @param         [in]int32_t nPosX          : X-coordinate of LCD area
* @param         [in]int32_t nPosY          : Y-coordinate of LCD area
* @param         [in]int32_t nWidth         : width of LCD area
* @param         [in]int32_t nHeight        : height of LCD area
* @param         [in]void *function         : callback function
* @retval          0..(TPEVT_ENTRY_MAX-1) : event ID.
*                 -1                      : Error occured.
******************************************************************************/
int_t TouchPanel_EventEntry( const TpEvt_EntryType eMode, const int32_t nPosX, const int32_t nPosY, const int32_t nWidth, const int32_t nHeight, const TpCBFunc function )
{
    int_t       nRet;

    nRet = TP_EventEntry( eMode, nPosX, nPosY, nWidth, nHeight, function );

    return nRet;
}


/**************************************************************************//**
* Function Name: TouchPanel_EventErase
* @brief         Erase callback event from event table.
*
*                Description:<br>
*
* @param         [in]int_t nId              : event ID
* @retval          0 : Operation successfull.
*                 -1 : Error occured.
******************************************************************************/
int_t TouchPanel_EventErase( const int_t nId )
{
    int_t       nRet;

    nRet = TP_EventErase( nId );

    return nRet;
}


/**************************************************************************//**
* Function Name: TouchPanel_ChangeEventEntry
* @brief         Change Entered callback event.
*
*                Description:<br>
*
* @param         [in]int_t nId              : event ID
* @param         [in]int32_t nPosX          : X-coordinate of LCD area
* @param         [in]int32_t nPosY          : Y-coordinate of LCD area
* @param         [in]int32_t nWidth         : width of LCD area
* @param         [in]int32_t nHeight        : height of LCD area
* @retval          0 : Operation successfull.
*                 -1 : Error occured.
******************************************************************************/
int_t TouchPanel_ChangeEventEntry( const int_t nId, const int32_t nPosX, const int32_t nPosY, const int32_t nWidth, const int32_t nHeight )
{
    int_t       nRet;

    nRet = TP_ChangeEventEntry( nId, nPosX, nPosY, nWidth, nHeight );

    return nRet;
}


/**************************************************************************//**
* Function Name: TouchPanel_EventLockAll
* @brief         Lock all callback event.
*
*                Description:<br>
*
* @param         None.
* @retval          0 : Operation successfull.
*                 -1 : Error occured.
******************************************************************************/
int_t TouchPanel_EventLockAll( void )
{
    int_t       nRet;

    nRet = TP_EventLockAll();

    return nRet;
}


/**************************************************************************//**
* Function Name: TouchPanel_EventUnlockAll
* @brief         Unlock all callback event.
*
*                Description:<br>
*
* @param         None.
* @retval          0 : Operation successfull.
*                 -1 : Error occured.
******************************************************************************/
int_t TouchPanel_EventUnlockAll( void )
{
    int_t       nRet;

    nRet = TP_EventUnlockAll();

    return nRet;
}


/**************************************************************************//**
* Function Name: TouchPanel_EventLock
* @brief         Lock assigned callback event.
*
*                Description:<br>
*
* @param         [in]int_t nId              : event ID
* @retval          0 : Operation successfull.
*                 -1 : Error occured.
******************************************************************************/
int_t TouchPanel_EventLock( const int_t nId )
{
    int_t       nRet;

    nRet = TP_EventLock( nId );

    return nRet;
}


/**************************************************************************//**
* Function Name: TouchPanel_EventUnlock
* @brief         Unlock assigned callback event.
*
*                Description:<br>
*
* @param         [in]int_t nId              : event ID
* @retval          0 : Operation successfull.
*                 -1 : Error occured.
******************************************************************************/
int_t TouchPanel_EventUnlock( const int_t nId )
{
    int_t       nRet;

    nRet = TP_EventUnlock( nId );

    return nRet;
}


