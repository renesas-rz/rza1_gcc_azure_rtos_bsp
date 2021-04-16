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
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name    : main.h
* Device(s)    : RZ/A1LU
* Tool-Chain   : GCC for Renesas RZ
* OS           : FreeRTOS Version See src/freertos/include/FreeRTOS.h
* H/W Platform : Stream it! v2 board
* Description  : Include.h file for main.c source code
*******************************************************************************/

#ifndef R_MAIN_H
#define R_MAIN_H

/******************************************************************************
Functions Prototypes
******************************************************************************/

/* main function header prototype */
extern int_t main (void);

/** Ethernet function required as the Stream it! board must disable Ethernet module to Screen */
/**
 * @brief start ethernet
 */
extern void R_ETH_MiddlewareStart(void);

 /**
  * @brief stop ethernet
  */
extern void R_ETH_MiddlewareStop(void);

/* R_MAIN_H */
#endif
