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
 * Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
 *****************************************************************************/
/*******************************************************************************
 * File Name   : tx_timer_config.c
 * Description : OS timer driver high layer driver
 *******************************************************************************/
/******************************************************************************
 * History     : DD.MM.YYYY Version Description
 *             : 18.06.2013 1.00
 *             : __#dd#__.__#mm#__.__#yyyy#__ __#ver_maj#__.__#ver_min#__ First Release
 *****************************************************************************/

/******************************************************************************
 WARNING!  IN ACCORDANCE WITH THE USER LICENCE THIS CODE MUST NOT BE CONVEYED
 OR REDISTRIBUTED IN COMBINATION WITH ANY SOFTWARE LICENSED UNDER TERMS THE
 SAME AS OR SIMILAR TO THE GNU GENERAL PUBLIC LICENCE
 ******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 *****************************************************************************/
/* OSTM Driver Header */
#include "r_ostm_drv_api.h"

#include "intc_iobitmask.h"

#include "intc_iodefine.h"

#include "control.h"
/******************************************************************************
 Macro definitions
 *****************************************************************************/

/******************************************************************************
 Typedef definitions
 *****************************************************************************/
typedef void (*ISR_FUNCTION)( uint32_t );
/******************************************************************************
 Exported global variables (to be accessed by other files)
 *****************************************************************************/

/******************************************************************************
 Private global variables and functions
 *****************************************************************************/
static int_t gs_tx_timer_ch0;

/******************************************************************************
 Imported global variables and functions (from other files)
 *****************************************************************************/
extern void _tx_timer_interrupt (void );
/******************************************************************************
 * Function Name: tx_timer_config
 * Description  : Configure the Threadx RTOS Timer
 * Arguments    : none
 * Return Value : none
 *****************************************************************************/
void tx_timer_config (void ) {
	   uint32_t temp;
	   st_r_drv_ostm_config_t config = {0}; /* force structure to initialise */

	   config.channel     = R_CH0;

	   /*  */
	   //config.frequency   = configPERIPHERAL_CLOCK_HZ / configTICK_RATE_HZ;
	   config.frequency   = 333333;

	   /*  */
	   config.callback_fn = (ISR_FUNCTION) _tx_timer_interrupt;

	   /*  */
	   config.mode        = OSTM_MODE_INTERVAL;

	   /* Open the ostm driver never close it. */
	   gs_tx_timer_ch0 = direct_open("ostm0",0);

	   /* Only continue if the drive has been successfully created */
	   direct_control (gs_tx_timer_ch0, CTL_OSTM_CREATE_TIMER, &config);


	   /* Configure binary point */
	   temp = INTC.ICCBPR & ~INTC_ICCBPR_Binarypoint;
	   INTC.ICCBPR = temp | (0 << INTC_ICCBPR_Binarypoint_SHIFT);

	   direct_control (gs_tx_timer_ch0, CTL_OSTM_START_TIMER, NULL);

}




