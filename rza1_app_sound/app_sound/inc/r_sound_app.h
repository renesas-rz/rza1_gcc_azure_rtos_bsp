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
#ifndef R_SOUND_APP_H
#define R_SOUND_APP_H

/**************************************************************************//**
 * @ingroup RENESAS_APPLICATION_SOFTWARE_PACKAGE Software Package
 * @defgroup R_SW_PKG_93_SOUND_APP Sound Application
 * @brief Application that uses the audio codec to record and playback.
 * @anchor R_SW_PKG_93_SOUND_APP_SUMMARY
 * @par Summary
 * @brief Application that uses the audio codec to record and playback.<br>
 *
 * Example interaction<br>
 * <em><br>
 * RZA1H RZ/A Software Package Ver.3.01.0001<br>
 * Copyright (C) Renesas Electronics Europe.<br>
 * REE> play                                         </em><- Invoke the play demonstration application<br><em>
 * Play Sound sample program start                   </em><- Sample sound will play on headphones<br><em>
 * Press any key to terminate demo<br>
 * task_play_sound_demo playing<br>
 * task_play_sound_demo complete<br>
 * Play Sound sample complete<br>
 * <br>
 * REE> record                                       </em><- Invoke the record demonstration application<br><em>
 * Play/record sample program start                  </em><- Sound from MIC input will play on headphones<br><em>
 * Press any key to terminate demo<br>
 * record sample user quit<br>
 * Record Sound sample complete<br>
 * <br>
 * REE><br>
 * </em>
 *
 * @anchor R_SW_PKG_93_SOUND_APP_INSTANCES
 * @par Known Implementations:
 * This driver is used in the RZA1H Software Package.
 * @see RENESAS_APPLICATION_SOFTWARE_PACKAGE
 * @see R_SW_PKG_93_SSIF_API
 * @{
 *****************************************************************************/

/******************************************************************************
User Includes
******************************************************************************/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "r_typedefs.h"
#include "compiler_settings.h"

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

#endif /* RENESAS_APPLICATION_APP_SWITCH_INC_R_SWITCH_MONITOR_H_ */
/**************************************************************************//**
 * @} (end addtogroup)
 *****************************************************************************/
