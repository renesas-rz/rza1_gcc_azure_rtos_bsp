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
 * and to discontinue the availability of this software. By using this
 * software, you agree to the additional terms and conditions found by
 * accessing the following link:
 * http://www.renesas.com/disclaimer
*******************************************************************************
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
 *****************************************************************************/
/******************************************************************************
 * @headerfile     r_timer.h
 * @brief          Timer functions
 * @version        1.00
 * @date           27.06.2018
 * H/W Platform    RZA1H
 *****************************************************************************/
 /*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 05.08.2010 1.00 First Release
 *****************************************************************************/
/******************************************************************************
  WARNING!  IN ACCORDANCE WITH THE USER LICENCE THIS CODE MUST NOT BE CONVEYED
  OR REDISTRIBUTED IN COMBINATION WITH ANY SOFTWARE LICENSED UNDER TERMS THE
  SAME AS OR SIMILAR TO THE GNU GENERAL PUBLIC LICENCE
******************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

/**************************************************************************//**
 * @ingroup R_SW_PKG_93_NONOS_MIDDLEWARE
 * @defgroup R_SW_PKG_93_TIMER Timer
 * @brief Timer module which allows user to start and stop a timer, 
 *        returning the time elapsed in seconds between calls. 
 *
 * @anchor R_SW_PKG_93_TIMER_API_SUMMARY
 * @par Summary
 *
 * This middleware module provides basic timer functionality allowing the 
 * user to trigger a timer to start and stop, returning the time elapsed
 * between calls, in seconds. 
 *
 * @anchor R_SW_PKG_93_TIMER_DEVICE_API_INSTANCES
 * @par Known Implementations:
 * This driver is used in the RZA1H Software Package.
 * @see RENESAS_APPLICATION_SOFTWARE_PACKAGE
 *
 * @see RENESAS_OS_ABSTRACTION  Renesas OS Abstraction interface
 * @{
 *****************************************************************************/
/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "FreeRTOS.h"
#include "queue.h"
#include "control.h"

/*****************************************************************************
Typedefs
******************************************************************************/

/******************************************************************************
Function Prototypes
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief      Function to start a measurement timer (low resolution)
 * 
 * @param[out] pTimeStamp: Pointer to the destination time stamp
 * 
 * @return None. 
 */
extern void timerStartMeasurement(PTMSTMP pTimeStamp);

/**
 * @brief      Function to stop a measurement timer and return 
 *             the elapsed time.
 * 
 * @param[in]  pTimeStamp:    Pointer to the starting time stamp
 * 
 * @retval     time_in_secs:  The elapsed time in seconds  
 */
extern float timerStopMeasurement(PTMSTMP pTimeStamp);

/**
 * @brief      Function to stop a measurement timer and return 
 *             the elapsed time.
 * 
 * @param[in]  pTimeStamp:    Pointer to the starting time stamp
 * @param[out] p_fresult:     Set to elapsed time in seconds 
 * 
 * @retval     time_in_secs:  The elapsed time in seconds 
 */
extern float ptimerStopMeasurement (PTMSTMP pTimeStamp, float *p_fresult);

#ifdef __cplusplus
}
#endif

#endif /* TIMER_H_INCLUDED */
/**************************************************************************//**
 * @} (end addtogroup)
 *****************************************************************************/
/******************************************************************************
End  Of File
******************************************************************************/
