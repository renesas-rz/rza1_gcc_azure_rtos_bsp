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
 * and to discontinue the availability of this software. By using this
 * software, you agree to the additional terms and conditions found by
 * accessing the following link:
 * http://www.renesas.com/disclaimer
*******************************************************************************
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
 *****************************************************************************/
/******************************************************************************
 * @headerfile     r_switch_driver.h
 * @brief          Routines for handling USER key switch input.
 * @version        1.00
 * @date           27.06.2018
 * H/W Platform  : RSK+RZA1H CPU Board
 *****************************************************************************/
 /*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 30.06.2018 1.00 First Release
 *****************************************************************************/

/* Multiple inclusion prevention macro */
#ifndef R_SW_PKG_93_SWITCH_API_H_INCLUDED
#define R_SW_PKG_93_SWITCH_API_H_INCLUDED
/**************************************************************************//**
 * @ingroup R_SW_PKG_93_NONOS_DRIVERS
 * @defgroup R_SW_PKG_93_SWITCH_API SWITCH RZA1H Driver
 * @brief This is a simple Switch Driver developed for the RZA1H
 * board.
 * @anchor R_SW_PKG_93_SWITCH_API_SUMMARY
 * @par Summary
 * @brief This is a simple Switch Driver developed for the RZA1H
 * board.
 *
 * @anchor R_SW_PKG_93_SWITCH_API_INSTANCES
 * @par Known Implementations:
 * This driver is used in the RZA1H Software Package.
 * @see RENESAS_APPLICATION_SOFTWARE_PACKAGE
 *
 * @ref RENESAS_OS_ABSTRACTION  Renesas OS Abstraction interface
 *
 * @ref R_SW_PKG_93_SWITCH_APP
 * @{
 *****************************************************************************/

#include "r_typedefs.h"

/***********************************************************************************************************************
 Macro definitions
 ***********************************************************************************************************************/
#define USER_SWITCH          ((GPIO.PPR7 & GPIO_P7_P79) != GPIO_P7_P79)
#define USER_SWITCH_PRESSED  ((GPIO.PPR7 & GPIO_P7_P79) != GPIO_P7_P79)
#define USER_SWITCH_RELEASED ((GPIO.PPR7 & GPIO_P7_P79) == GPIO_P7_P79)

/***********************************************************************************************************************
 Typedef definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 Exported global variables
 ***********************************************************************************************************************/
extern volatile uint8_t g_switch_press_flg;

/***********************************************************************************************************************
 Exported global functions (to be accessed by other files)
 ***********************************************************************************************************************/

/**
 * @brief Initialises the switch portpin and associated interrupt(s).
 *
 * @param[in] interrupt: Boolean to switch between polled or Interrupt.<BR>
 *                       False - Switch is Polled. <BR>
 *                       True -  Switch is Interrupt Driven. <BR>
 * @return None.
 */
void R_SWITCH_Init (bool_t interrupt);

/**
 * @brief       Takes a pointer to a function, and sets it as the call-back
 *              function for the switch interrupts. The passed function is
 *              executed every time any switch is pressed down.
 *
 * @param[in]   func:  Function to be called by the IRQ
 *
 * @retrun None.
 */
void R_SWITCH_SetPressCallback (void (*func) (void));

/**
 * @brief       Takes a pointer to a function, and sets it as the call-back
 *              function for the switch interrupts. The passed function is
 *              executed every time any switch is released.
 *
 * @param[in]   callback: Pointer to call-back function (set to NULL to disable)
 */
void R_SWITCH_SetReleaseCallback (void (*callback) (void));

#endif /* R_SW_PKG_93_SWITCH_API_H_INCLUDED */
/**************************************************************************//**
 * @} (end addtogroup)
 *****************************************************************************/
