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
* File Name    : eventControl.c
* Version      : 1.0
* Device(s)    : Renesas
* Tool-Chain   : GNUARM-NONE-EABI v14.02
* OS           : FreeRTOS
* H/W Platform : RSK+
* Description  : Task aware signals through "events"
******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 04.02.2010 1.00    First Release
******************************************************************************/

/******************************************************************************
  WARNING!  IN ACCORDANCE WITH THE USER LICENCE THIS CODE MUST NOT BE CONVEYED
  OR REDISTRIBUTED IN COMBINATION WITH ANY SOFTWARE LICENSED UNDER TERMS THE
  SAME AS OR SIMILAR TO THE GNU GENERAL PUBLIC LICENCE
******************************************************************************/

/*****************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/

#include "r_event.h"


/*****************************************************************************
Constant Data
******************************************************************************/

/*****************************************************************************
Function Prototypes
******************************************************************************/


/*****************************************************************************
External Variables
******************************************************************************/

extern unsigned long ulPortInterruptNesting;

/*****************************************************************************
Global Variables
******************************************************************************/

size_t    gstEventCount = 0UL;
size_t    gstEventMax = 0UL;

/*****************************************************************************
Public Functions
******************************************************************************/

/*****************************************************************************
Function Name: eventCreate
Description:   Function to create one or more events
Arguments:     OUT ppEventList - Pointer to the destination event pointer array
               IN  uiNumber - The number of events to create
Return value:  The number of events not created
*****************************************************************************/
uint32_t eventCreate(ppevent_t ppEventList, uint32_t uiNumber)
{
    bool_t no_more_objects = false;
    if (ppEventList)
    {
        /* For each of the events required */
        while ((uiNumber) && (false == no_more_objects))
        {
            /* Allocate a queue */
        	os_msg_queue_handle_t pEvent;
            if ( R_OS_CreateMessageQueue( sizeof(e_event_state_t) * 1U, &pEvent))
            if (pEvent)
            {
                /* Reset the event */
                xQueueReset(pEvent);

                /* return the event */
                *ppEventList++ = pEvent;
                uiNumber--;
                gstEventCount++;
                if (gstEventCount > gstEventMax)
                {
                    gstEventMax = gstEventCount;
                }
            }
            else
            {
                TRACE(("eventCreate: Out of event objects\r\n"));
                no_more_objects = true;
            }
        }
    }
    return uiNumber;
}
/*****************************************************************************
End of function  eventCreate
******************************************************************************/

/*****************************************************************************
Function Name: eventDestroy
Description:   Function to destroy one or more events
Arguments:     IN  ppEventList - Pointer to the event list to destroy
               IN  uiNumber The number of events to destroy
Return value:  none
*****************************************************************************/
void eventDestroy(ppevent_t ppEventList, uint32_t uiNumber)
{
    while (uiNumber--)
    {
        pevent_t pEvent = *ppEventList++;
        gstEventCount--;
        vQueueDelete(pEvent);
    }
}
/*****************************************************************************
End of function  eventDestroy
******************************************************************************/

/*****************************************************************************
Function Name: eventSet
Description:   Function to set an event
Arguments:     IN  pEvent - Pointer to the event to set
Return value:  true if the event was set
*****************************************************************************/
_Bool eventSet(pevent_t pEvent)
{
    e_event_state_t eventState = EV_SET;

    /* Check if we are in an ISR */
    if (ulPortInterruptNesting)
    {
        if (xQueueSendFromISR(pEvent, &eventState, NULL) == pdTRUE)
        {
            return true;
        }
    }
    else
    {
        if (xQueueSend(pEvent, &eventState, 0UL) == pdTRUE)
        {
            return true;
        }
    }
    return false;
}
/*****************************************************************************
End of function  eventSet
******************************************************************************/

/*****************************************************************************
Function Name: eventReset
Description:   Function to reset an event
Arguments:     IN  Pointer to the event to reset
Return value:  true if the event was reset
*****************************************************************************/
_Bool eventReset(pevent_t pEvent)
{
    /* Reset the event */
    xQueueReset(pEvent);
    return true;
}
/*****************************************************************************
End of function  eventReset
******************************************************************************/

/*****************************************************************************
Function Name: eventState
Description:   Function to return the current state of an event
Arguments:     IN  pEvent - Pointer to the event
Return value:  The state of the event
*****************************************************************************/
e_event_state_t eventState(pevent_t pEvent)
{
    e_event_state_t eventState = EV_RESET;
    xQueuePeek(pEvent, &eventState, 0UL);
    return eventState;
}
/*****************************************************************************
End of function  eventState
******************************************************************************/

/*****************************************************************************
Function Name: eventStateEx
Description:   Function to return the state of an event and then reset it
Arguments:     IN  pEvent - Pointer to the event
Return value:  The state of the event
*****************************************************************************/
e_event_state_t eventStateEx(pevent_t pEvent)
{
    e_event_state_t eventState = EV_RESET;
    xQueuePeek(pEvent, &eventState, 0UL);
    if (EV_RESET != eventState)
    {
        eventReset(pEvent);
    }
    return eventState;
}
/*****************************************************************************
End of function  eventStateEx
******************************************************************************/

/*****************************************************************************
Function Name: eventWait
Description:   Function to wait for one or more events to be set
Arguments:     IN  ppEventList - Pointer to the list of events
               IN  uiNumber - The number of events to wait on
               IN  bfSingle - true if just one event being set is to wake
                              the task or false if all the events must be set
                              to wake the task
Return value:  When bfSingle is true the index of the event in the list which
               was set. Always 0 when bfSingle is false
*****************************************************************************/
uint32_t eventWait(ppevent_t  ppEventList,
                   uint32_t uiNumber,
                   _Bool    bfSingle)
{
    (void) bfSingle;
    /* Check for waiting on a single event. */
    if (1 == uiNumber)
    {
        e_event_state_t eventState;
        xQueueReceive(ppEventList[0], &eventState, portMAX_DELAY);
        return 0;
    }
    else
    {
        uint32_t    uiCount = uiNumber;
        e_event_state_t     eventState;

        /* Mask interrupts so that the queue's can't be filled */
        int         iMask = R_OS_SysLock(NULL);

        /* Check that all queue's are empty */
        while (uiCount--)
        {
            eventState = eventStateEx(ppEventList[uiCount]);
            if (eventState == EV_SET)
            {
                R_OS_SysUnlock(NULL, iMask);
                return uiCount;
            }
        }

        /* Create the queue set large enough to hold an event for every space in
           every queue and semaphore that is to be added to the set. */
        xQueueSetHandle xQueueSet;
        xQueueSet = xQueueCreateSet(sizeof(e_event_state_t) * uiNumber);
        if (xQueueSet)
        {
            pevent_t      pEvent;
            uint32_t    uiResult = 0;
            uiCount = uiNumber;
            while (uiCount--)
            {
                xQueueAddToSet(ppEventList[uiCount], xQueueSet);
            }

            /* Now they have been added to the queues can be filled */
            R_OS_SysUnlock(NULL, iMask);

            /* Wait for a queue event */
            pEvent = xQueueSelectFromSet(xQueueSet, portMAX_DELAY);

            /* Reset the queue - possible problem if the event is set again */
            xQueueReset(pEvent);

            /* Find out which queue it was */
            uiCount = uiNumber;
            while (uiCount--)
            {
                if (ppEventList[uiCount] == pEvent)
                {
                    uiResult = uiCount;
                }
                xQueueRemoveFromSet(ppEventList[uiCount], xQueueSet);
            }

            /* Free the queue set */
            vQueueDelete(xQueueSet);

            /* Return the result */
            return uiResult;
        }
        else
        {
            R_OS_SysUnlock(NULL, iMask);
        }
    }

    /* This is an error! */
    return 0;
}
/*****************************************************************************
End of function  eventWait
******************************************************************************/

/*****************************************************************************
Function Name: eventWaitMutex
Description:   Function to wait for a mutex event
Arguments:     IN / OUT ppEvent - Pointer to the mutex event pointer
               IN  dwTimeOut - The time out in uS or EV_WAIT_INFINITE
Return value:  true for success, false if the event could not be created
               or the wait timed out
*****************************************************************************/
_Bool eventWaitMutex(ppevent_t ppEvent, uint32_t dwTimeOut)
{
    xSemaphoreHandle xSemaphore = NULL;

    /* Check to see if it is pointing to NULL */
    if (!*ppEvent)
    {
        /* Create the semaphore */
        xSemaphore = xSemaphoreCreateMutex();

        /* Return the mutex to the user */
        *((xSemaphoreHandle*)ppEvent) = xSemaphore;
    }
    else
    {
        /* Get the mutex handle from the user */
        xSemaphore = *((xSemaphoreHandle*)ppEvent);
    }

    /* Check that the semaphore was created */
    if (xSemaphore)
    {
        /* Get mutex */
        if (xSemaphoreTake(xSemaphore, dwTimeOut))
        {
            return 0;
        }
    }
    return -1;
}
/*****************************************************************************
End of function  eventWaitMutex
******************************************************************************/

/*****************************************************************************
Function Name: eventReleaseMutex
Description:   Function to release a mutex event
Arguments:     IN / OUT ppEvent - Pointer to the mutex event pointer
Return value:  none
*****************************************************************************/
void eventReleaseMutex(ppevent_t ppEvent)
{
    xSemaphoreHandle xSemaphore = *((xSemaphoreHandle*)ppEvent);

    /* Release mutex */
    xSemaphoreGive(xSemaphore);
}
/*****************************************************************************
End of function  eventReleaseMutex
******************************************************************************/

/*****************************************************************************
Function Name: eventUsage
Description:   Function to get the number of events used
Arguments:     OUT pstEventsInUse - Pointer to the number of events in use
               OUT pstEventTotal - Pointer to the peak number of events used
Return value:  none
*****************************************************************************/
void eventUsage(size_t *pstEventsInUse, size_t *pstEventTotal)
{
    *pstEventsInUse = gstEventCount;
    *pstEventTotal = gstEventMax;
}
/*****************************************************************************
End of function  eventUsage
******************************************************************************/

/******************************************************************************
End  Of File
******************************************************************************/
