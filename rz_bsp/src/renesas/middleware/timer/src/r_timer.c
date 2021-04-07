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
 * Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
 *******************************************************************************
 * File Name    : timer.c
 * Version      : 1.10
 * Device(s)    : Renesas
 * Tool-Chain   : GNUARM-NONE-EABI v14.02
 * OS           : FreeRTOS
 * H/W Platform : RSK+
 * Description  : Timer functions
 *******************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 01.08.2009 1.00 First Release
 *              : 19.02.2016 1.10 Resolved overflow calcs in timerStopMeasurement
 ******************************************************************************/

/******************************************************************************
 WARNING!  IN ACCORDANCE WITH THE USER LICENCE THIS CODE MUST NOT BE CONVEYED
 OR REDISTRIBUTED IN COMBINATION WITH ANY SOFTWARE LICENSED UNDER TERMS THE
 SAME AS OR SIMILAR TO THE GNU GENERAL PUBLIC LICENCE
 ******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#include "tx_api.h"
#include "timers.h"
#include "Trace.h"
#include "control.h"
#include "portmacro.h"
#include "r_timer.h"
#include "r_devlink_wrapper.h"

/******************************************************************************
 Macro definitions
 ******************************************************************************/

#define TIMER_QUEUE_BLOCK_TIME              (1024UL)

#define TIMER_OFLOW_MS (128.85 * 1000)        /* mS when counter register overflows */
/* This is ((1 / portTICK_CNT_FREQ_HZ)
 *           * 0xffffffff * 1000) */
static int_t s_timerh = ( -1);

/*****************************************************************************
 Typedefs
 ******************************************************************************/

/*****************************************************************************
 Function Prototypes
 ******************************************************************************/

/*****************************************************************************
 Function Name: timerStartMeasurement
 Description:   Function to start a measurement timer (low resolution)
 Arguments:     OUT pTimeStamp - Pointer to the destination time stamp
 Return value:  none
 *****************************************************************************/
void timerStartMeasurement (PTMSTMP pTimeStamp)
{
    s_timerh = open(DEVICE_INDENTIFIER "wdt", O_RDWR);

    control(s_timerh, CTL_GET_TIME_STAMP, pTimeStamp);
}
/*****************************************************************************
 End of function  timerStartMeasurement
 ******************************************************************************/

/*****************************************************************************
 Function Name: timerStopMeasurement
 Description:   Function to stop a measurement timer and return the elapsed time
 Arguments:     IN  pTimeStamp - Pointer to the staring time stamp
 Return value:  The elapsed time in seconds
 *****************************************************************************/
float timerStopMeasurement (PTMSTMP pTimeStamp)
{
    TMSTMP timeNow;
    float fResult = 0.0f;

    if (control(s_timerh, CTL_GET_TIME_STAMP, &timeNow) == 0)
    {
        uint32_t ulTime = 0u;
        int32_t iDelta;

        /* Calculate the number of mS */
        if (timeNow.ulMilisecond >= pTimeStamp->ulMilisecond)
        {
            ulTime = timeNow.ulMilisecond - pTimeStamp->ulMilisecond;
        }
        else
        {
            ulTime = ((uint32_t)TIMER_OFLOW_MS - pTimeStamp->ulMilisecond) + timeNow.ulMilisecond;
        }

        /* Set the number of mS */
        fResult = ((float)1e-3 * (float) ulTime);

        /* Calculate the sub 1mS time */
        if (timeNow.usSubMilisecond >= pTimeStamp->usSubMilisecond)
        {
            iDelta = timeNow.usSubMilisecond - pTimeStamp->usSubMilisecond;
        }
        else
        {
            const uint32_t uiCountMax = (portTICK_CNT_FREQ_HZ / 1000);
            iDelta = (int32_t)((uiCountMax - pTimeStamp->usSubMilisecond) + timeNow.usSubMilisecond);

            /* If the timer has wrapped then there is possibly an extra mS
             counted which has not happened */
            if (fResult > 1e-3)
            {
                fResult -= (float)1e-3;
            }
        }

        fResult += (float) ((float) iDelta * 1.0f / (float) portTICK_CNT_FREQ_HZ);
    }

    /* ensure timer is stopped */
    close(s_timerh);

    /* Throw a divide by 0 exception */
    return (fResult);
}
/*****************************************************************************
 End of function  timerStopMeasurement
 ******************************************************************************/


/*****************************************************************************
 Function Name: timerStopMeasurement
 Description:   Function to stop a measurement timer and return the elapsed time
 Arguments:     IN  pTimeStamp - Pointer to the starting time stamp
 Return value:  The elapsed time in seconds
 *****************************************************************************/
float ptimerStopMeasurement (PTMSTMP pTimeStamp, float *p_fresult)
{
    TMSTMP timeNow;
    *p_fresult = 0.0f;

    if (control(s_timerh, CTL_GET_TIME_STAMP, &timeNow) == 0)
    {
        uint32_t ulTime = 0u;
        int32_t iDelta;

        /* Calculate the number of mS */
        if (timeNow.ulMilisecond >= pTimeStamp->ulMilisecond)
        {
            ulTime = timeNow.ulMilisecond - pTimeStamp->ulMilisecond;
        }
        else
        {
            ulTime = ((uint32_t)TIMER_OFLOW_MS - pTimeStamp->ulMilisecond) + timeNow.ulMilisecond;
        }

        /* Set the number of mS */
        *p_fresult = ((float)1e-3 * (float) ulTime);

        /* Calculate the sub 1mS time */
        if (timeNow.usSubMilisecond >= pTimeStamp->usSubMilisecond)
        {
            iDelta = timeNow.usSubMilisecond - pTimeStamp->usSubMilisecond;
        }
        else
        {
            const uint32_t uiCountMax = (portTICK_CNT_FREQ_HZ / 1000);
            iDelta = (int32_t)((uiCountMax - pTimeStamp->usSubMilisecond) + timeNow.usSubMilisecond);

            /* If the timer has wrapped then there is possibly an extra mS
             counted which has not happened */
            if (*p_fresult > 1e-3)
            {
                *p_fresult -= (float)1e-3;
            }
        }

        *p_fresult += (float) ((float) iDelta * 1.0f / (float) portTICK_CNT_FREQ_HZ);
    }

    /* ensure timer is stopped */
    close(s_timerh);

    /* Throw a divide by 0 exception */
    return (*p_fresult);
}
/*****************************************************************************
 End of function  ptimerStopMeasurement
 ******************************************************************************/




/******************************************************************************
 Private Functions
 ******************************************************************************/

/******************************************************************************
 End  Of File
 ******************************************************************************/
