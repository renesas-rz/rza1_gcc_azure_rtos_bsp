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
* @file          tp.h
* Rev: 30 
* Date:: 2016-12-21 12:02:44 +0900#
* @brief         TouchPanel Driver internal hedder
******************************************************************************/
#ifndef     TP_H
#define     TP_H

/*******************************************************************************
Includes <System Includes>, "Project Includes"
*******************************************************************************/
#include "r_os_abstraction_api.h"
#include    "tp_if.h"

#if defined(__cplusplus)
extern "C" {
#endif


/******************************************************************************
Macro definitions
******************************************************************************/
#define DBG_LEVEL_OT                (-1)                /* onetime debug */
#define DBG_LEVEL_DEF               (0)                 /* default */
#define DBG_LEVEL_ERR               (1)                 /* error */
#define DBG_LEVEL_MSG               (2)                 /* message */
#define DBG_LEVEL_LOG               (3)                 /* log */
#define DBG_LEVEL_DBG               (4)                 /* debug */

#define DBG_LEVEL                   (DBG_LEVEL_ERR)
#if (DBG_LEVEL >= DBG_LEVEL_OT)
#define DBG_printf_OT                   printf
#else
#define DBG_printf_OT                   1 ? (int32_t) 0 : printf
#endif
#if (DBG_LEVEL >= DBG_LEVEL_DEF)
#define DBG_printf_DEF                  printf
#else
#define DBG_printf_DEF                  1 ? (int32_t) 0 : printf
#endif
#if (DBG_LEVEL >= DBG_LEVEL_ERR)
#define DBG_printf_ERR                  printf
#else
#define DBG_printf_ERR                  1 ? (int32_t) 0 : printf
#endif
#if (DBG_LEVEL >= DBG_LEVEL_MSG)
#define DBG_printf_MSG                  printf
#else
#define DBG_printf_MSG                  1 ? (int32_t) 0 : printf
#endif
#if (DBG_LEVEL >= DBG_LEVEL_LOG)
#define DBG_printf_LOG                  printf
#else
#define DBG_printf_LOG                  1 ? (int32_t) 0 : printf
#endif
#if (DBG_LEVEL >= DBG_LEVEL_DBG)
#define DBG_printf_DBG                  printf
#else
#define DBG_printf_DBG                  1 ? (int32_t) 0 : printf
#endif

//#define   __DEBUG
#ifdef  __DEBUG
#define SCOPE_STATIC
#else   /* __DEBUG */
#define SCOPE_STATIC    static
#endif  /* __DEBUG */

/*! The max number of event entry */
#define     TPEVT_ENTRY_MAX         (16)

#define        TP_EVTFLG_NONE             (0x00000000)
#define        TP_EVTFLG_PENIRQ           (0x00000001)      /*! Touch Panel event flag, pen interrupt */
#define        TP_EVTFLG_EXIT             (0x00000080)      /*! Touch Panel event flag, exit and delete task */
#define        TP_EVTFLG_ALL              (TP_EVTFLG_PENIRQ | TP_EVTFLG_EXIT)


/***********************************************************************************
 Enumerated Types
***********************************************************************************/
/*! Touch panel event lock state */
typedef enum {
    TP_EVT_UNLOCK = 0,  /*!< Unlocked */
    TP_EVT_LOCK         /*!< Locked */
} TpEvt_LockState ;


/***********************************************************************************
 Structures
***********************************************************************************/
/*! Coordinate structure */
typedef struct {
    int32_t x ;        /*!< x-coordinate [pixel] */
    int32_t y ;        /*!< y-coordinate [pixel] */
} TPEVT_COORDINATES ;

/*! Event entry struct */
typedef struct {
    TpEvt_EntryType mode ;      /*!< The type of touch panel event entry */
    TPEVT_COORDINATES st ;      /*!< The upper-left coordinates of the rectangular area in which touch event can be received. [pixel] */
    TPEVT_COORDINATES ed ;      /*!< The lower-right coordinates of the rectangular area in which touch event can be received. [pixel] */
    void (*function)( int_t, TP_TouchEvent_st* ) ;  /*!< Event notification callback function pointer */
    TpEvt_LockState evtlock ;   /*!< Event lock state */
} TPEVT_ENTRY ;


/***********************************************************************************
 Global Vaiables
***********************************************************************************/
extern os_task_t*  p_os_task;


/***********************************************************************************
 Function Prototypes
***********************************************************************************/

/**
 * @brief         Initializes internal variables of the touch panel driver.\n
  •    Securing of touch panel event entry area\n
  •    Setting of internal variable nEvtEntryId  to -1\n
  •    Setting of internal variable TpEvtLockInf to TP_EVT_UNLOCK\n
 * @retval        None.
*/
void            TP_Init( void );

/**
 * @brief         Opens the touch panel driver.\n
  •    Setting the LCD size in the driver’s variables ScreenWidth and ScreenHeight\n
  •    Generation of touch panel task synchronization semaphore\n
  •    Generation of touch panel task\n
  •    Setting of task priority of touch panel task\n
  •    Opening of communication environment with LCD board\n
  •    Registration of call-back event when touch panel interrupt occurs in LCD event
 * @param[in]     nWidth screen width
 * @param[in]     nHeight screen height
 * @param[in]     unIrqLv IRQ interrupt level
 * @param[in]     nTskPri task priority
 * @param[in]     unTskStk task stack size
 * @retval        0 Operation successfull.
 * @retval        -1 Error occured.
*/
int_t           TP_Open( const int_t nWidth, const int_t nHeight, const uint32_t unIrqLv, const int16_t nTskPri, const uint32_t unTskStk );

/**
 * @brief         Closes the touch panel driver.\n
  •    Removal of call-back event when touch panel interrupt occurs in LCD event\n
  •    Removal of all touch panel event registrations by the user\n
  •    Removal of touch panel task\n 
  •    Removal of semaphore for synchronization with the touch panel task
 * @retval        0 Operation successfull.
 * @retval       -1 Error occured.
*/
int_t           TP_Close( void );

/**
 * @brief         Registers in the event table a call-back function linked to a touch panel interrupt.\n
                  After registration finishes, the event ID is sent as a return value.\n

   •    Searching for a free area in the touch panel event table (Up to 16 touch panel events can be registered, and error processing occurs if no free area is available.)\n 
   •    Making “specified touch action,” “X coordinate of specified area,” “Y coordinate of specified area,” “width of specified area,”\n
   “height of specified area,” “specified call-back function” settings for the touch panel event table free area.\n
   Note:     When “X coordinate of specified area,” “Y coordinate of specified area,” “width of specified area,”\n
   and “height of specified area” are registered in the touch panel event table, the following processing is performed to register the result as a rectangular area:\n
   st.x (X coordinate of area start position) <- “X coordinate of specified area”\n
   st.y (Y coordinate of area start position) <- “Y coordinate of specified area”\n
   ed.x (X coordinate of area end position) <- (“X coordinate of specified area” - “width of specified area”)\n
   ed.y (Y coordinate of area end position) <- (“Y coordinate of specified area” - “height of specified area”)
 *
 * @param[in]       eMode event type
 * @param[in]       nPosX X-coordinate of LCD area
 * @param[in]       nPosY Y-coordinate of LCD area
 * @param[in]       nWidth width of LCD area
 * @param[in]       nHeight height of LCD area
 * @param[in]       function callback function
 * @retval          0 to (TPEVT_ENTRY_MAX-1)
 * @retval          -1 Error occured.
*/
int_t           TP_EventEntry( const TpEvt_EntryType eMode, const int32_t nPosX, const int32_t nPosY, const int32_t nWidth, const int32_t nHeight, const TpCBFunc function );

/**
 * @brief         Removes an event from the call-back event table of the touch panel driver.\n
   •    Event ID checking (within range of 0 to TPEVT_ENTRY_MAX)\n
   •    Disabling of event associated with event ID (TPEVT_ENTRY_NON)
 *
 * @param[in]     nId event ID
 * @retval        0 Operation successfull.
 * @retval        -1 Error occurred.
*/
int_t           TP_EventErase( const int_t nId );

/**
 * @brief         The rectangular area to which the event ID specified by the 1st argument (nId)\n
   is registered is changed to the rectangular area specified by the 2nd to 5th arguments.\n
   •    Event ID checking (within range of 0 to TPEVT_ENTRY_MAX)\n 
   •    Event ID checking (unregistered ID or removed ID)\n
   •    Registration of event in area of specified ID in touch panel event table 
 *
 * @param[in]     nId event ID
 * @param[in]     nPosX  X-coordinate of LCD area
 * @param[in]     nPosY  Y-coordinate of LCD area
 * @param[in]     nWidth width of LCD area
 * @param[in]     nHeight height of LCD area
 * @retval        0 Operation successfull.
 * @retval        -1 Error occurred.
*/
int_t           TP_ChangeEventEntry( const int_t nId, const int32_t nPosX, const int32_t nPosY, const int32_t nWidth, const int32_t nHeight );

/**
 * @brief         Locks all registered touch panel call-back events.\n
   Calls the function described in TP_EventLock, to set all events to the locked state (TP_EVT_LOCK).
 *
 * @retval        0 Operation successfull.
 * @retval        -1 Error occured.
*/
int_t           TP_EventLockAll( void );

/**
 * @brief        Unlocks all registered touch panel call-back events.\n
   Calls the function described in TP_EventUnlock, to set all events to the unlocked state (TP_EVT_UNLOCK).
 * @retval       0 Operation successfull.
 * @retval       -1 Error occured.
*/
int_t           TP_EventUnlockAll( void );

/**
 * @brief         Locks the touch panel call-back event specified by the 1st argument (nId).\n
   •    Event ID checking (within range of 0 to TPEVT_ENTRY_MAX)\n
   •    Setting the event specified by the event ID to the locked state (TP_EVT_LOCK) in the touch panel event table.
 *
 * @param[in]    nId event ID
 * @retval       0 Operation successfull.
 * @retval       -1 Error occured.
*/
int_t           TP_EventLock( const int_t nId );

/**
 * @brief        Unlocks the touch panel call-back event specified by the 1st argument (nId).\n
   •    Event ID checking (within range of 0 to TPEVT_ENTRY_MAX)\n
   •    Setting the event specified by the event ID to the unlocked state (TP_EVT_UNLOCK) in the touch panel event table.
 *
 * @param[in]    nId event ID
 * @retval       0 Operation successfull.
 * @retval       -1 Error occured.
*/
int_t           TP_EventUnlock( const int_t nId );

/**
 * @brief         Acquires from the touch panel driver call-back event table the\n
   pointer address at which the event ID event information is registered.
 * @param[in]     nId event ID
 * @retval        TPEVT_ENTRY pointer to event
*/
TPEVT_ENTRY     *TP_GetEventTable( const int_t nId );

/**
 * @brief         Acquires the lock state of the touch panel call-back event.
 * @retval        TP_EVT_LOCK In locked state
 * @retval        TP_EVT_UNLOCK In unlocked state.
*/
TpEvt_LockState TP_GetEventLockInf( void );

/**
 * @brief         Acquires the screen size of the LCD panel.
 * @param[out]    *pnWidth pointer to width value
 * @param[out]    *pnHeight pointer to height value
 * @retval        None
*/
void            TP_GetScreenSize( int_t *pnWidth, int_t *pnHeight );

/**************************************************************************//**
* @brief         Get the version number of the connected TFT App. board.
*
*                Description:<br>
*
* @retval        The version number of the board as an integer.
******************************************************************************/
int_t           TP_GetAppBoardVersion();

/**
 * @brief         Sends a synchronization event message.
 * @param[in]     unEvtFlg event flag
 * @retval        0 Operation successfull.
 * @retval        -1 Error occured.
*/
int32_t         TP_SendEvtMsg( const uint32_t unEvtFlg );

/**
 * @brief         Waits to receive a synchronization event message.
 * @retval        TP_EVTFLG_NONE No event flags
 * @retval        TP_EVTFLG_PENIRQ Interrupt pending
 * @retval        TP_EVTFLG_EXIT End task 
 * @retval        TP_EVTFLG_ALL Both Interrupt pending and exit flag. 
 * @retval        -1 Error occured.
*/
int32_t         TP_WaitEvtMsg( void );

/**
 * @brief         Clears the specified event flag.
 * @param[in]     unEvtFlg event flag
 * @retval        None.
*/
void            TP_ClearEvtMsg( const uint32_t unEvtFlg );

#if defined(__cplusplus)
}
#endif


#endif      /* TP_H */
