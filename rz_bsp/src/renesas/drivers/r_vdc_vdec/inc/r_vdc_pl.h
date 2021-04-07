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
* Copyright (C) 2012 - 2014 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/**************************************************************************//**
* @file         r_vdc_h.h
* @version      1.00
* $Rev: 99 $
* $Date:: 2017-04-10 20:56:26 +0900#$
* @brief        VDC driver API definitions
******************************************************************************/

#ifndef R_VDC_PL_H
#define R_VDC_PL_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_vdc.h"

#ifdef  __cplusplus
extern  "C"
{
#endif  /* __cplusplus */

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/
void R_VDC_OnInitialize ( const uint32_t user_num );
void R_VDC_OnFinalize( const uint32_t user_num );
int32_t R_VDC_OnEnableInterrupt(const vdc_channel_t ch);
int32_t R_VDC_OnDisableInterrupt(const vdc_channel_t ch);

#ifdef  __cplusplus
}
#endif  /* __cplusplus */


#endif  /* R_VDC_PL_H */
