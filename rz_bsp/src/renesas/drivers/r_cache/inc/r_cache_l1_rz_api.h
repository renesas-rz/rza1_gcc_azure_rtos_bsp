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
* File Name    : r_cache_l1_rz_api.h
* Version      : 1.0
* Description  : Definition of the ARMv7 L1 cache API function.
*******************************************************************************/
#ifndef R_CACHE_L1_RZ_API_H
#define R_CACHE_L1_RZ_API_H

/*******************************************************************************
Includes <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>

/******************************************************************************
Functions Prototypes
******************************************************************************/

void R_CACHE_L1_InvalidAll(void);
void R_CACHE_L1_CleanAll(void);
void R_CACHE_L1_CleanInvalidAll(void);
void R_CACHE_L1_InvalidLine(uint32_t line_addr, uint32_t size);
void R_CACHE_L1_CleanLine(uint32_t line_addr, uint32_t size);
void R_CACHE_L1_CleanInvalidLine(uint32_t line_addr, uint32_t size);
#if 1//SAWABE_20181121E
void R_CACHE_disable();
void R_CACHE_enable();
#endif

#endif/*R_CACHE_L2_RZ_API_H*/
