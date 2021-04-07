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
* @file          tp_task.h
* $Rev: 30 $
* $Date:: 2016-12-21 12:02:44 +0900#$
* @brief         TouchPanel driver internal hedder for task
******************************************************************************/
#ifndef     TP_TASK_H
#define     TP_TASK_H

/******************************************************************************
Includes <System Includes>, "Project Includes"
******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif


/******************************************************************************
Macro definitions
******************************************************************************/


/***********************************************************************************
 Enumerated Types
***********************************************************************************/
typedef enum {
    TP_TASKSTAT_IDLE   = 0,
    TP_TASKSTAT_ACTIVE
} TP_TASKSTAT_et;



/***********************************************************************************
 Structures
***********************************************************************************/


/***********************************************************************************
 Global Vaiables
***********************************************************************************/


/***********************************************************************************
 Function Prototypes
***********************************************************************************/
void            TP_Task(void *params);
TP_TASKSTAT_et  TP_GetTaskStatus( void );

#if defined(__cplusplus)
}
#endif


#endif      /* TP_TASK_H */
