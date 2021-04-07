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
 * Copyright (C) 2017 Renesas Electronics Corporation. All rights reserved.
 *******************************************************************************/
/**************************************************************************//**
 * @file         OV7670_OmniVision.h
 * @version      0.01
 * $Rev:  $
 * $Date:: #$
 * @brief        Camera for vdc5 definition header
 ******************************************************************************/
#ifndef OV7670_OMNIVISION_H
#define OV7670_OMNIVISION_H

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include    "r_ceu.h"

/******************************************************************************
 Macro definitions
 ******************************************************************************/
/* capture mode */
#define CAP_CEU_MODE       (CEU_DATA_SYNC_MODE)

/* Capture resolution */
#if (GRAPHICS_CAM_IMAGE_SIZE == CAMERA_RES_QVGA)
#define CAP_CEU_SIZE_HW    (320u)
#define CAP_CEU_SIZE_VW    (240u)
#elif  (GRAPHICS_CAM_IMAGE_SIZE == CAMERA_RES_VGA)
#define CAP_CEU_SIZE_HW    (640u)
#define CAP_CEU_SIZE_VW    (480u)
#else
#error ERROR: Invalid Capture Size defined in camera_module.h...
#endif

/* Polarity of a signal */
#define CAP_CEU_HPOL       (CEU_HIGH_ACTIVE)
#define CAP_CEU_VPOL       (CEU_HIGH_ACTIVE)

/* Y/Cb/Y/Cr data string start timing */
#define CAP_CEU_DTARY      (CEU_Y0_CB0_Y1_CR0)

/* Capture timing */
#define CAP_CEU_VS         (0u)
#define CAP_CEU_HS         (0u)
#define CAP_CEU_HW         (CAP_CEU_SIZE_HW * 2u)
#define CAP_CEU_VW         (CAP_CEU_SIZE_VW)

/* digital image input pins */
#define CAP_CEU_DATAWIDTH  (CEU_8BIT_DATA_PINS)

/* write swap*/
#define CAP_CEU_SWAP_32    (CEU_OFF)
#define CAP_CEU_SWAP_16    (CEU_OFF)
#define CAP_CEU_SWAP_08    (CEU_OFF)

#endif  /*  OV7670_OMNIVISION_H */
