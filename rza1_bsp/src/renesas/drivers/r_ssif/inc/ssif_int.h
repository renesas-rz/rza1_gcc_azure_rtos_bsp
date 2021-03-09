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
 * @headerfile     ssif_int.h
 * @brief          ssif low level header file
 * @version        1.00
 * @date           27.06.2018
 * H/W Platform    RZA1LU Steam-IT
 *****************************************************************************/
/*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 30.06.2018 1.00 First Release
 *****************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef SSIF_INT_H
#define SSIF_INT_H

/*************************************************************************//**
 * @ingroup R_SW_PKG_93_SSIF_API
 * @defgroup R_SW_PKG_93_SSIF_LLD_INT SSIF Low Level Driver API
 * @brief Internal low level SSIF driver component
 *
 * @anchor R_SW_PKG_93_SSIF_LLD_INT_SUMMARY
 * @par Summary
 * @brief This interface defines the functionality that the low level driver
 * must implement for compatibility with the high level driver.
 *
 * @anchor R_SW_PKG_93_SSIF_LLD_INT_LIMITATIONS
 * @par Known Limitations
 * @ref NONE_YET
 *
 * @anchor R_SW_PKG_93_SSIF_LLD_INT_INSTANCES
 * @par Known Implementations
 * @ref RENESAS_APPLICATION_SOFTWARE_PACKAGE
 * @{
 *****************************************************************************/

/*******************************************************************************
Includes <System Includes>, "Project Includes"
*******************************************************************************/
#include "r_typedefs.h"

/******************************************************************************
Macro definitions
******************************************************************************/


/*************************************************************************
 Enumerated Types
*************************************************************************/

/*************************************************************************
 Structures
*************************************************************************/

/******************************************************************************
 Function Prototypes
 *****************************************************************************/

/**
 * @brief Initialize interrupt setting for SSIF channel.
 * @param[in] ssif_ch    :SSIF channel
 * @param[in] int_level  :GIC priority level of interrupt
 **/
void SSIF_InterruptInit(const uint32_t ssif_ch, const uint8_t int_level);

/**
 * @brief Uninitialize interrupt setting for SSIF channel. Disable interrupt
 * and unregister interrupt handler
 * @param[in] ssif_ch    :SSIF channel
 **/
void SSIF_InterruptShutdown(const uint32_t ssif_ch);

/**
 * @brief Enable the SSIF channel error interrupt
 * @param[in] ssif_ch    :SSIF channel
 **/
void SSIF_EnableErrorInterrupt(const uint32_t ssif_ch);

/**
 * @brief Disable the SSIF channel error interrupt
 * @param[in] ssif_ch    :SSIF channel
 **/
void SSIF_DisableErrorInterrupt(const uint32_t ssif_ch);

#endif /* SSIF_INT_H */
/**************************************************************************//**
 * @} (end addtogroup)
 *****************************************************************************/
