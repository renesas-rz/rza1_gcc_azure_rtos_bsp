/******************************************************************************
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
*******************************************************************************
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.

*******************************************************************************
* File Name    : version.h
* Device(s)    : RZ/A1LU
* Tool-Chain   : GCC for Renesas RZ
* OS           : FreeRTOS Version See src/freertos/include/FreeRTOS.h
* H/W Platform : Stream it! v2 board
* Description  : The version release and build numbers.
*                build version
*                APPLICATION_INFO_VERSION.APPLICATION_INFO_RELEASE.APPLICATION_INFO_BUILD
*******************************************************************************
* History      : DD.MM.YYYY Version Description
*              : 05.08.2010 1.00    First Release
*              : 12.02.2016 1.10    Release 1.1 - includes USB CDC Support
*              : 25.01.2018 1.02    First Release
*              : 20.07.2018 3.01    First Release GCC build with sound demo
******************************************************************************/

/******************************************************************************
  WARNING!  IN ACCORDANCE WITH THE USER LICENCE THIS CODE MUST NOT BE CONVEYED
  OR REDISTRIBUTED IN COMBINATION WITH ANY SOFTWARE LICENSED UNDER TERMS THE
  SAME AS OR SIMILAR TO THE GNU GENERAL PUBLIC LICENCE
******************************************************************************/

#ifndef VERSION_H_INCLUDED
#define VERSION_H_INCLUDED

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
 * The following definitions are not surrounded by braces as the utility
 * buildinc does not recognise numbers surrounded in braces
******************************************************************************/
#define APPLICATION_INFO_VERSION          3
#define APPLICATION_INFO_RELEASE          1
#define APPLICATION_INFO_BUILD            93



#endif                              /* VERSION_H_INCLUDED */

/******************************************************************************
End  Of File
******************************************************************************/
