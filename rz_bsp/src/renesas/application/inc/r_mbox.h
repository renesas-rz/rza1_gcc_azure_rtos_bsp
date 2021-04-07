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
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************
* File Name    : r_mbox.h
* Version      : 1.0
* Device(s)    : Renesas
* Tool-Chain   : GNUARM-NONE-EABI v14.02
* OS           : FreeRTOS
* H/W Platform : RSK+
* Description  : Mail box interface for lwIP
*******************************************************************************
* History      : DD.MM.YYYY Ver. Description
*              : 01.08.2009 1.00 First Release
******************************************************************************/

/******************************************************************************
  WARNING!  IN ACCORDANCE WITH THE USER LICENCE THIS CODE MUST NOT BE CONVEYED
  OR REDISTRIBUTED IN COMBINATION WITH ANY SOFTWARE LICENSED UNDER TERMS THE
  SAME AS OR SIMILAR TO THE GNU GENERAL PUBLIC LICENCE
******************************************************************************/

#include "r_typedefs.h"

#ifndef MBOX_H_INCLUDED
#define MBOX_H_INCLUDED

/******************************************************************************
Macro definitions
******************************************************************************/
/* Define a default size for the mail box */
#ifndef MBOX_SIZE
#define MBOX_SIZE                   1024
#endif

/*****************************************************************************
Typedef definitions
******************************************************************************/

/** The data structure is encapsulated */
typedef void *PMBOX;

/*****************************************************************************
Public Functions
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Function to create a mail box object
 * @param iSize
 * @return
 */
extern  PMBOX mboxCreate(int iSize);

/**
 * @brief Function to destroy a mail box object
 * @param pMBox - Pointer to the mail box object
 */
extern void mboxDestroy(PMBOX pMBox);

/**
 * @brief Function to post a message
 * @param pMBox - Pointer to the mail box object
 * @param pvMessage - Pointer to the message pointer
 */
extern  void mboxPost(PMBOX pMBox, void *pvMessage);

/**
 * @brief Function to try and post a message
 * @param pMBox - Pointer to the mail box object
 * @param pvMessage - Pointer to the message pointer
 * @return
 */
extern  int mboxTryPost(PMBOX pMBox, void *pvMessage);

/**
 * @brief Function to fetch mail from a mail box
 * @param pMBox - Pointer to the mail box object
 * @param pvMessage - Pointer to the message pointer
 * @param uiTimeOut - The maximum time to wait for the message
 * @return
 */
extern  int32_t mboxFetch(PMBOX pMBox, void **pvMessage, uint32_t uiTimeOut);

/**
 * @brief Function to try and fetch mail from a mail box
 * @param pMBox - Pointer to the mail box object
 * @param pvMessage - Pointer to the message pointer
 * @return
 */
extern int32_t mboxTryFetch(PMBOX pMBox, void **pvMessage);

#ifdef __cplusplus
}
#endif

#endif /* MBOX_H_INCLUDED */

/******************************************************************************
End  Of File
******************************************************************************/
