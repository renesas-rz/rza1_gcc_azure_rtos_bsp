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
 * @headerfile     r_sound_app.h
 * @brief          Application that uses the audio codec to record and playback
 * @version        1.00
 * @date           27.06.2018
 * H/W Platform    RZA1H
 *****************************************************************************/
 /*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 30.06.2018 1.00 First Release
 *****************************************************************************/

/* Multiple inclusion prevention macro */
#ifndef RENESAS_APPLICATION_APP_SOUND_INC_R_SOUND_H_
#define RENESAS_APPLICATION_APP_SOUND_INC_R_SOUND_H_

/**************************************************************************//**
 * @ingroup R_SW_PKG_93_SOUND_APP
 * @defgroup R_SW_PKG_93_SOUND_CALL_APP Sound Application
 * @brief Functions that uses the audio codec to record and playback.
 * @{
 *****************************************************************************/

/******************************************************************************
User Includes
******************************************************************************/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

/******************************************************************************
Typedefs
******************************************************************************/

/******************************************************************************
Constant Data
******************************************************************************/

/******************************************************************************
External Variables
******************************************************************************/

/******************************************************************************
Global Variables
******************************************************************************/

/******************************************************************************
Public Functions
******************************************************************************/
/**
 * @brief Play Sound application
 * Play pre-recorded sound sample (LR_44_1K16B_S.dat)
 * @param p_in : Standard input from console.
 * @param p_out : Standard output to console
 */
extern void R_SOUND_PlaySample(FILE *p_in, FILE *p_out);

/**
 * @brief Run the record/playback Sound application
 * @param p_in : Standard input from console.
 * @param p_out : Standard output to console
 */
extern void R_SOUND_RecordSample(FILE *p_in, FILE *p_out);

#endif /* RENESAS_APPLICATION_APP_SOUND_INC_R_SOUND_H_ */
/**************************************************************************//**
 * @} (end addtogroup)
 *****************************************************************************/
