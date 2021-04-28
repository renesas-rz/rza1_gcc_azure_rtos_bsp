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
 * File Name     : r_port.c
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
#include <stddef.h>
#include "gpio_addrdefine.h"
#include "r_port_if.h"
#include "rza_io_regrw.h"

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
static void pin_function(PinName pin, int function, PinDirection dir);


void set_pin_function ( const st_port_config_t *map) {
	if (sizeof(st_port_config_t) <= sizeof(map)) {
		pin_function(map->pin, map->function, map->dir);
	}
}

int set_pins_function( const st_port_init_config_t *maps) {
	uint8_t count = 0;
	st_port_config_t* pFunc;
	if ( maps != NULL ){
		count = maps->count;
		pFunc = maps->p_config_table;
		for (int i = 0; i < count; i++){
			set_pin_function(pFunc);
			pFunc++;
		}
	}
	return maps->count;
}

static void pin_function(PinName pin, int function, PinDirection dir) {

	int shift = pin & 0xF;
	int bitmask = (1<<shift);
	int n = pin >> 4;

	/* Port initialize */
	rza_io_reg_write_16( PIBC(n), 0, shift, bitmask);
	rza_io_reg_write_16( PBDC(n), 0, shift, bitmask);
	rza_io_reg_write_16( PM(n),   dir&1?1:0, shift, bitmask);
	rza_io_reg_write_16( PMC(n),  0, shift, bitmask);
	rza_io_reg_write_16( PIPC(n), 0, shift, bitmask);

	/* Port mode : Multiplex mode                 */
	/* Set Port function setting 					  */
	/* I/O control mode : Peripheral function         */
	/* Set Bidirectional mode                         */
	rza_io_reg_write_16( PBDC(n),  dir==3? 1:0, shift, bitmask);
	rza_io_reg_write_16( PFC(n),   SET_PIN(function, FUNC_PFC_MODE), shift, bitmask);
	rza_io_reg_write_16( PFCE(n),  SET_PIN(function, FUNC_PFCE_MODE), shift, bitmask);
	rza_io_reg_write_16( PFCAE(n), SET_PIN(function, FUNC_PFCAE_MODE), shift, bitmask);
	rza_io_reg_write_16( PIPC(n),  1, shift, bitmask);
	rza_io_reg_write_16( PMC(n),   1, shift, bitmask);
}
