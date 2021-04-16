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
 * @file         ntsc.h
 * @version      0.01
 * $Rev:  $
 * $Date:: #$
 * @brief        Camera for vdc5 definition header
 ******************************************************************************/
#ifndef NTSC_H
#define NTSC_H

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include    "r_vdc5.h"

/******************************************************************************
 Macro definitions
 ******************************************************************************/

/* External input format select */
#define CAP_VDC_MODE       (VDC_EXTIN_FORMAT_BT601)    /* Not use */

/* Capture resolution */
#define CAP_VDC_SIZE_VW    (240u)                       /* use */
#define CAP_VDC_SIZE_HW    (720u)                       /* use */

/* Polarity of a signal */
#define CAP_VDC_HPOL       (VDC_SIG_POL_NOT_INVERTED)  /* Not use */
#define CAP_VDC_VPOL       (VDC_SIG_POL_NOT_INVERTED)  /* Not use */

/* Y/Cb/Y/Cr data string start timing */
#define CAP_VDC_DTARY      (VDC_EXTIN_H_POS_CBYCRY)    /* Not use */

/* Capture timing */
#define CAP_VDC_HS         (122u * 2u)
#define CAP_VDC_VS         (16u)
#define CAP_VDC_HW         (CAP_VDC_SIZE_HW * 2u)
#define CAP_VDC_VW         (CAP_VDC_SIZE_VW)

/* Clock edge select for capturing external input video image signalsl */
#define CAP_VDC_DATAEGDE   (VDC_EDGE_RISING)           /* Not use */

/* External input B/R signal swap on/off control */
#define CAP_VDC_PORTSWAP   (VDC_OFF)                   /* Not use */

/* External input bit endian change on/off control */
#define CAP_VDC_PORTENDIAN (VDC_OFF)                   /* Not use */

/* Field operating mode select */
#define CAP_VDC_FIELD      (VDC_RES_INTER_INTERLACE)   /* use */

/* Frame buffer video-signal writing format */
#define CAP_VDC_WFORMAT    (VDC_RES_MD_YCBCR422)       /* use */

#endif  /*  NTSC_H */
