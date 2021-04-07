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
* @file         sound_ver.c
* $Rev: 8410 $
* $Date:: 2018-06-13 14:25:37 +0100#$
* @brief        SOUND Driver get verion function
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "sound_if.h"

/******************************************************************************
Macro definitions
******************************************************************************/

/* Ex. V2.12 -> MAJOR=2, MINOR=12 */
#define SOUND_DRV_VER_MAJOR_PRV_  (1u)
#define SOUND_DRV_VER_MINOR_PRV_  (0u)

#define SOUND_DRV_VER_MASK_PRV_   (0xFFu)
#define SOUND_DRV_VER_SHIFT_PRV_  (8u)

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/

/**************************************************************************//**
* Function Name: R_SOUND_GetVersion
* @brief         Get SOUND driver version.
*
*                Description:<br>
*
* @param         none
* @retval        driver version -
*                    upper 8bit=MAJOR
*                    lower 8bit=MINOR
******************************************************************************/
uint16_t R_SOUND_GetVersion(void)
{
    const uint16_t version =
        ((SOUND_DRV_VER_MAJOR_PRV_ & SOUND_DRV_VER_MASK_PRV_) << SOUND_DRV_VER_SHIFT_PRV_)
        | (SOUND_DRV_VER_MINOR_PRV_ & SOUND_DRV_VER_MASK_PRV_);

    return version;
}
/******************************************************************************
End of function R_SOUND_GetVersion
******************************************************************************/
