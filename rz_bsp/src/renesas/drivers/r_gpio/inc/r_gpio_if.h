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
 *
 * Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
 *******************************************************************************/
/*******************************************************************************
 * File Name     : r_gpio_if.h
 * Device(s)     : RZ/A1L
 * Tool-Chain    : GNUARM-NONEv16.01-EABI
 * H/W Platform  : Stream It! v2 board
 * Description   : RIIC driver (User define function)
 *******************************************************************************/
/*******************************************************************************
 * History       : DD.MM.YYYY Version Description
 *               : May 7, 2020 1.00
 *******************************************************************************/


/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include "PinNames.h"
#include "r_typedefs.h"
/******************************************************************************
 Typedef definitions
 ******************************************************************************/

/******************************************************************************
 Macro definitions
 ******************************************************************************/

/*****************************************************************************
 Constant Data
 ******************************************************************************/

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/

#ifndef RENESAS_DRIVERS_R_GPIO_R_GPIO_IF_H_
#define RENESAS_DRIVERS_R_GPIO_R_GPIO_IF_H_


/** Initialize the GPIO pin
 *
 * @param obj The GPIO object to initialize
 * @param pin The GPIO pin to initialize
 */
void gpio_init(PinName pin);
void gpios_init(uint8_t port, uint16_t pins, PinDirection mode);
/** Set the input pin mode
 *
 * @param obj  The GPIO object
 * @param mode The pin mode to be set
 */
void gpio_mode(PinName pin, PinMode mode);

/** Set the pin direction
 *
 * @param obj       The GPIO object
 * @param direction The pin direction to be set
 */
void gpio_dir(PinName pin, PinDirection direction);

/** Set the output value
 *
 * @param obj   The GPIO object
 * @param value The value to be set
 */
void gpio_write(PinName pin, int value);
void gpios_write(uint8_t port, uint16_t pins, int value);
/** Read the input value
 *
 * @param obj The GPIO object
 * @return An integer value 1 or 0
 */
int gpio_read(PinName pin);


#endif /* RENESAS_DRIVERS_R_GPIO_R_GPIO_IF_H_ */
