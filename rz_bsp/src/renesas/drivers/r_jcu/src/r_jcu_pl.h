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
* Copyright (C) 2012 - 2016 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/******************************************************************************
* File: jcu_pl.h
*    JPEG Codec Unit (JCU) Sample Driver. OS Porting Layer.
*
* - $Version: $ (=JCU_VERSION)
* - $ModuleRevision: {Masters}\JCU\RZ2\Single\04_RZA1 $
* - $Rev: $
* - $Date: 2018-03-02T18:42:30+09:00 $
******************************************************************************/

#ifndef JCU_PL_H
#define JCU_PL_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
//#include  "use_list.h"
#include  "r_jcu.h"
#include  "rz_co_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Functions Prototypes
******************************************************************************/
errnum_t  R_JCU_OnInitialize(void);
errnum_t  R_JCU_OnFinalize(void);
bool_t    R_JCU_DisableInterrupt(void);
void      R_JCU_EnableInterrupt(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* JCU_PL_H */

