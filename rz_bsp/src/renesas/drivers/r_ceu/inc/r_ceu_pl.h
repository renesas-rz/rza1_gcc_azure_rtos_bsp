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
 * File Name :   r_ceu_pl.h
 * @file         r_ceu_pl.h
 * @version      1.00
 * $Rev: 182 $
 * $Date:: 2017-05-11 07:35:51 +0900#$
 * @brief        CEU OS Porting header
 *****************************************************************************/
 /*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 30.06.2018 1.00 First Release
 *****************************************************************************/

/* Multiple inclusion prevention macro */
#ifndef R_CEU_PL_H
#define R_CEU_PL_H
/**************************************************************************//**
 * @ingroup R_SW_PKG_93_NONOS_DRIVERS
 * @defgroup R_SW_PKG_93_CEU_PL ADC RZA1H and RZ/A1LU Driver Porting File
 *
 * @anchor R_SW_PKG_93_CEU_PL_SUMMARY
 * @par Summary
 *
 * This fiel provides the control interface for porting the driver into a
 * new system..

 *
 * @anchor R_SW_PKG_93_CEU_PL_INSTANCES
 * @par Known Implementations:
 * This driver is used in the RZA1H and RZ/A1LU Software Package.
 * @see RENESAS_APPLICATION_SOFTWARE_PACKAGE
 * @see RENESAS_OS_ABSTRACTION  Renesas OS Abstraction interface
 * @{
 *****************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include "r_ceu.h"

/******************************************************************************
 Functions Prototypes
 ******************************************************************************/
void R_CEU_OnInitialize(const uint32_t user_num);
void R_CEU_OnFinalize(const uint32_t user_num);
int32_t R_CEU_OnEnableInterrupt(const uint32_t user_num);
int32_t R_CEU_OnDisableInterrupt(const uint32_t user_num);

/****************************************************************************/
/* End of module                                                            */
/****************************************************************************/
#endif /* R_CEU_PL_H */
/**************************************************************************//**
 * @} (end addtogroup)
 *****************************************************************************/

