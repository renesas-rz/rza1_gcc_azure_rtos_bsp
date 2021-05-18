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
* http://www.renesas.com/disclaimer
* Copyright (C) 2014-2017 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/**************************************************************************//**
* @file          sound.h
* $Rev: 8475 $
* $Date:: 2018-06-18 11:21:41 +0100#$
* @brief         sound driver functions header
******************************************************************************/

#ifndef SOUND_H
#define SOUND_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "sound_if.h"
#include "iodefine_cfg.h"
#include "Renesas_RZ_A1.h"

#include "sound_device.h"
#include "mcu_board_select.h"

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/
#if (TARGET_RZA1 <= TARGET_RZA1LU)
#define SOUND_CH_NUM (4u)
#else /* TARGET_RZA1H */
#define SOUND_CH_NUM (6u)
#endif /* (TARGET_RZA1 <= TARGET_RZA1LU) */

/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Functions Prototypes
******************************************************************************/
int32_t sound_init(void);
int32_t sound_un_init(void);
int32_t sound_open(const uint32_t chnl);
int32_t sound_close(const uint32_t chnl);
int32_t sound_set_volume(const uint32_t chnl, const uint32_t vol);
int32_t sound_set_mic_volume(const uint32_t chnl, const uint32_t vol);
int32_t sound_set_sampling_rate(const uint32_t chnl, const uint32_t freq);
int32_t sound_set_format(const uint32_t chnl, const uint32_t fmt, const uint32_t dwdsz);
#endif /* SOUND_H */
