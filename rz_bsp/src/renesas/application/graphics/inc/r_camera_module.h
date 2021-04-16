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
 * @headerfile     camera_module.h
 * @brief          Defines for selecting the LCD panel type which is connected.
 * @version        1.00
 * @date           27.06.2018
 * H/W Platform    RZA1H
 *****************************************************************************/
 /*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 30.06.2018 1.00 First Release
 *****************************************************************************/

/* Multiple inclusion prevention macro */
#ifndef CAMERA_MODULE_H
#define CAMERA_MODULE_H

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include "mcu_board_select.h"

/******************************************************************************
 Macro definitions
 ******************************************************************************/
#if ( TARGET_BOARD == TARGET_BOARD_STREAM_IT2 )
#include "r_camera_if.h"

#define INPUT_SELECT   (CAMERA_SELECT)

/** Setting of CMOS Camera */
#define GRAPHICS_CAM_IMAGE_SIZE   (CAMERA_RES_IMAGE_SIZE)

#if ( INPUT_SELECT == CAMERA_OV7670 )
#include "camera/OV7670_OmniVision.h"
#elif ( INPUT_SELECT == CAMERA_OV7740 )
#include "camera/OV7740_OmniVision.h"
#else
#error ERROR: Invalid INPUT_SELECT.
#endif

#elif ( TARGET_BOARD == TARGET_BOARD_RSK )

#define ANALOG_NTSC    (0u)
#define ANALOG_PAL     (1u)
#define INPUT_SELECT   (ANALOG_NTSC)

#if ( INPUT_SELECT == ANALOG_NTSC )
#include "camera/ntsc.h"
#elif ( INPUT_SELECT == ANALOG_PAL )
#include "camera/pal.h"
#else
#error ERROR: Invalid INPUT_SELECT.
#endif

/******************************************************************************
 ******************************************************************************/
#else
#error ERROR: Invalid board defined.
#endif

#endif  /*  CAMERA_MODULE_H */
