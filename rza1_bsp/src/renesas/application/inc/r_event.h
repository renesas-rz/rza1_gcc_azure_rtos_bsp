/******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. 
* This software is owned by Renesas Electronics Corporation and is protected 
* under all applicable laws, including copyright laws.
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
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************
* File Name    : r_event.h
* Version      : 1.0
* Device(s)    : Renesas
* Tool-Chain   : GNUARM-NONE-EABI v14.02
* OS           : FreeRTOS
* H/W Platform : RSK+
* Description  : Task aware signals through "events"
******************************************************************************
* History      : DD.MM.YYYY Ver. Description
*              : 01.08.2009 1.00 First Release
******************************************************************************/

/******************************************************************************
  WARNING!  IN ACCORDANCE WITH THE USER LICENCE THIS CODE MUST NOT BE CONVEYED
  OR REDISTRIBUTED IN COMBINATION WITH ANY SOFTWARE LICENSED UNDER TERMS THE
  SAME AS OR SIMILAR TO THE GNU GENERAL PUBLIC LICENCE
******************************************************************************/

#ifndef EVENT_H_INCLUDED
#define EVENT_H_INCLUDED

/*****************************************************************************
User Includes
******************************************************************************/

#include <string.h>
#include "r_typedefs.h"
#include "compiler_settings.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "r_event.h"

/*****************************************************************************
Defines
******************************************************************************/

#define EV_WAIT_INFINITE            (R_OS_ABSTRACTION_PRV_EV_WAIT_INFINITE)

/*****************************************************************************
Enumerated Types
******************************************************************************/


/*****************************************************************************
Typedefs
******************************************************************************/

typedef xQueueHandle PEVENT, *PPEVENT;

/*****************************************************************************
Public Functions
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief      Function to create one or more events
 * @param[out] ppEventList - Pointer to the destination event pointer array
 * @param[in]  uiNumber - The number of events to create
 * @return     The number of events not created
 */
extern  uint32_t eventCreate(PPEVENT ppEventList, uint32_t uiNumber);

/**
 * @brief Function to destroy one or more events
 * @param ppEventList - Pointer to the event list to destroy
 * @param uiNumber - The number of events to destroy
 */
extern  void eventDestroy(PPEVENT ppEventList, uint32_t uiNumber);

/**
 * @brief  Function to set an event
 * @param  pEvent - Pointer to the event to set
 * @return true if the event was set
 */
extern  _Bool eventSet(PEVENT pEvent);

/**
 * @brief  Function to reset an event
 * @param  pEvent - pointer to the event to reset
 * @return true if the event was reset
 */
extern  _Bool eventReset(PEVENT pEvent);

/**
 * @brief Function to return the current state of an event
 * @param pEvent - pointer to the event
 * @return The state of the event
 */
extern  e_event_state_t eventState(PEVENT pEvent);

/**
 * @brief Function to return the state of an event and then reset it
 * @param pEvent - Pointer to the event
 * @return The state of the event
 */
extern  e_event_state_t eventStateEx(PEVENT pEvent);

/**
 * @brief Function to wait for one or more events to be set
 * @param ppEventList - Pointer to the list of events
 * @param uiNumber - The number of events to wait on
 * @param bfSingle - true if just one event being set is to wake
 *                   the task or false if all the events must be set
                     to wake the task
 * @return When bfSingle is true the index of the event in the list which
           was set. Always 0 when bfSingle is false
 */
extern  uint32_t eventWait(PPEVENT ppEventList,
                           uint32_t    uiNumber,
                           _Bool    bfSingle);

/**
 * @brief Function to wait for a mutex event
 * @param ppEvent[in,out] - Pointer to the mutex event pointer
 * @param dwTimeOut - The time out in mS or EV_WAIT_INFINITE
 * @return true for success, false if the event could not be created
           or the wait timed out
 */
extern  _Bool eventWaitMutex(PPEVENT ppEvent, uint32_t dwTimeOut);

/**
 * @brief Function to release a mutex event
 * @param ppEvent - Pointer to the mutex event pointer
 */
extern  void eventReleaseMutex(PPEVENT ppEvent);

/**
 * @brief Function to get the number of events used
 * @param pstEventsInUse - Pointer to the number of events in use
 * @param pstEventTotal - Pointer to the peak number of events used
 */
extern  void eventUsage(size_t *pstEventsInUse, size_t *pstEventTotal);

#ifdef __cplusplus
}
#endif

#endif /* EVENT_H_INCLUDED */

/******************************************************************************
End  Of File
******************************************************************************/
 
