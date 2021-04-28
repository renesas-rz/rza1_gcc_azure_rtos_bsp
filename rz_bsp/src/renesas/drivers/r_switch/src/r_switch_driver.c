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
 * Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.    */
/******************************************************************************
 * File Name     : r_switch_driver.c
 * Device(s)     : RZ/A1L
 * Tool-Chain    : GNUARM-NONE-EABI-v16.01
 * H/W Platform  : Stream it! v2
 * Description   : Routines for handling USER key switch input.
 *
 *******************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 28.06.2016 1.00     First Release
 ******************************************************************************/

/******************************************************************************
 WARNING!  IN ACCORDANCE WITH THE USER LICENCE THIS CODE MUST NOT BE CONVEYED
 OR REDISTRIBUTED IN COMBINATION WITH ANY SOFTWARE LICENSED UNDER TERMS THE
 SAME AS OR SIMILAR TO THE GNU GENERAL PUBLIC LICENCE
 ******************************************************************************/

/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 ***********************************************************************************************************************/

#include "r_intc.h"                    /* INTC driver header */
#include "r_switch_driver.h"
#include "iodefine_cfg.h"
#include "gpio_iobitmask.h"
#include "rza_io_regrw.h"
#include "r_switch_driver.h"
#include "r_task_priority.h"
#include "r_compiler_abstraction_api.h"
#include "r_switch_driver.h"

#include "r_port_if.h"
#include "r_port_sc_cfg.h"




/*******************************************************************************
 Macro definitions
 *******************************************************************************/
#define ICR1_LOW	 0x0
#define ICR1_FALLING 0x1
#define ICR1_RISING  0x2
#define ICR1_BOTH	 0x3

// Set delay to 256ms
#define DEBOUCE_TIMEOUT 8000
// Set delay to 128ms
#define MULTI_BUT_TIMEOUT 4000

#define NUM_OF_IRQ_SW 5
/*******************************************************************************
 Typedef definitions
 ******************************************************************************/
typedef struct {
	int irq;
	PinName sw_pin;
	PinName led_pin;
}sw_info_t;

/*******************************************************************************
 Imported global variables and functions (from other files)
 *******************************************************************************/

/*******************************************************************************
 Private global variables and functions
 *******************************************************************************/
/* switch interrupt handler function prototype */
static void irq_switch_trigger (uint32_t sense);
static void irq_switch_debounce (uint32_t int_sense);

static void R_SWITCH_RemoveSetCallback (int irq );

/* Functions used for creating the switch software debounce delay */
static void init_switch_debounce_timer (void);
static void start_debouce_timer ( int irq_num );
static void restart_debouce_timer ( int irq_num );
static void stop_debouce_timer ( int irq_num );

static sw_info_t sw_info[NUM_OF_IRQ_SW] = {
		{ INTC_ID_IRQ0, P1_8, P5_8},	// Volume up button
		{ INTC_ID_IRQ1, P1_9, P5_2},	// Volume down button
		{ INTC_ID_IRQ2, P1_10, P5_10},	// Input Select
		{ INTC_ID_IRQ3, P1_11, NC},		// Power Button
		{ INTC_ID_IRQ4, P1_12, P5_9},	// Bluetooth Pairing
};
/*******************************************************************************
 Exported global variables and functions (to be accessed by other files)
 *******************************************************************************/

/* Switch press call-back pointer declaration */
static int (*g_switch_press_callback_func[5]) (uint32_t,int) = {0,0,0,0,0};

/* Switch release call-back pointer declaration */
static void (*g_switch_release_callback_func[5]) (int) = {0,0,0,0,0};

static int g_pending_sw_irq;
static int g_pending_sw_irq_count = 0;

/***********************************************************************************************************************
 Private global variables and functions
 ***********************************************************************************************************************/

/*****************************************************************************
 Function Name: R_SWITCH_Init
 Description:   Initialises the switch portpin and associated interrupt(s).
 Parameters:    interrupt false - Switch is Polled
 true -  Switch is Interrupt Driven
 Return value:  None
 *****************************************************************************/
void R_SWITCH_Init (PinName pin, int (* func)(uint32_t int_sense, int count))
{
	static bool_t debounce_enabled = false;
	uint8_t mask = 0;

    if (func != NULL)  // Interrupt driven
    {

        uint8_t irq_num = 0;

        switch ( pin ) {
        	case P1_8 :
        		irq_num = 0;
        		break;
        	case P1_9 :
        	    irq_num = 1;
        		break;
        	case P1_10 :
        	    irq_num = 2;
        	    break;
        	case P1_11 :
        	    irq_num = 3;
        	    break;
        	case P1_12 :
        		irq_num = 4;
        		break;
        	default:
        		irq_num = 0xFF;

        }

        if ( irq_num < NUM_OF_IRQ_SW) {
        /* Configure and enable IRQ5 interrupts received from the user switch */
        	/* Configure IRQs detections on falling edge */
        	INTC.ICR1 |= ICR1_LOW << (irq_num*2);    // IRQ5 config = '01' = Falling Edge

        	/* Clear IRQ3/5 interrupt flag  */
        	mask = 1 << (irq_num);
        	INTC.IRQRR &= ~mask;

        	// Initialize Debounce timer
        	if ( !debounce_enabled ) {
        		debounce_enabled = true;
        		init_switch_debounce_timer();
        	}

        	R_INTC_Disable(sw_info[irq_num].irq);
			R_INTC_RegistIntFunc(sw_info[irq_num].irq, &irq_switch_trigger);
			R_SWITCH_SetPressCallback( irq_num, func);
			R_INTC_SetPriority(sw_info[irq_num].irq, ISR_SWITCH_IRQ_PRIORITY);

			R_INTC_Enable(sw_info[irq_num].irq);

			for ( int i = 0; i < GPIO_SC_INIT_intc.count; i++ )
			{
				if (  GPIO_SC_INIT_intc.p_config_table[i].pin == pin )
				{
					set_pin_function( &GPIO_SC_INIT_intc.p_config_table[i] );
					break;
				}
			}
        }
    }

}
/*****************************************************************************
 End of function  R_SWITCH_Init
 ******************************************************************************/

/*****************************************************************************
 Function Name: R_SWITCH_remove
 Description:   Initialises the switch portpin and associated interrupt(s).
 Parameters:    interrupt false - Switch is Polled
 true -  Switch is Interrupt Driven
 Return value:  None
 *****************************************************************************/
void R_SWITCH_remove (PinName pin)
{
	int irq = 0xFF;
	int irq_num = 0;

	st_port_config_t config = {0, FUNCTION_MODE2, PIN_INPUT};
	st_port_init_config_t sw = { 0, 1, NULL};
	sw.p_config_table = &config;

	switch ( pin ) {
		case P1_8 :
			irq = INTC_ID_IRQ0;
			irq_num = 0;
			break;
		case P1_9 :
			irq = INTC_ID_IRQ1;
			irq_num = 1;
			break;
		case P1_10 :
			irq = INTC_ID_IRQ2;
			irq_num = 2;
			break;
		case P1_11 :
			irq = INTC_ID_IRQ3;
			irq_num = 3;
			break;
		case P1_12 :
			irq = INTC_ID_IRQ4;
			irq_num = 4;
			break;
		default:
			irq = 0xFF;

		 sw.p_config_table->pin = pin;
		 set_pins_function(&sw);

		 // Setup switch for polling
		 gpio_init(pin);
		 gpio_dir(pin, PIN_INPUT);

	}

	if ( irq >= INTC_ID_IRQ0 && irq <= INTC_ID_IRQ4) {

			// Disable IRQ interrupt
			R_INTC_Disable(irq);
			//R_INTC_ClearPendingInt(irq);
			// UnRegister IRQ callback
			R_INTC_UnRegistIntFunc(irq);
			// Un Register Switch Callback
			R_SWITCH_RemoveSetCallback(irq_num);
	}

}
/*****************************************************************************
 End of function  R_SWITCH_remove
 ******************************************************************************/

bool_t R_SWITCH_Poll ( PinName pin )
{
	bool_t ret = false;

	if ( 0 < gpio_read( pin ) )
		ret = true;

	return ret;
}

/*******************************************************************************
 * Function Name: R_SWITCH_SetPressCallback
 * Description  : Takes a pointer to a function, and sets it as the call-back
 *                function for the switch interrupts. The passed function is
 *                executed every time any switch is pressed down.
 * Arguments    : void (* func)(uint32_t) : Function to be called by the IRQ
 * Return Value : none
 *******************************************************************************/
void R_SWITCH_SetPressCallback (int irq, int (*func) (uint32_t,int))
{
    /* Store the call-back function pointer into the global variable */
    g_switch_press_callback_func[irq] = func;
}
/*****************************************************************************
 End of function  R_SWITCH_SetPressCallback
 ******************************************************************************/
/*******************************************************************************
 * Function Name: R_SWITCH_SetPressCallback
 * Description  : Takes a pointer to a function, and sets it as the call-back
 *                function for the switch interrupts. The passed function is
 *                executed every time any switch is pressed down.
 * Arguments    : void (* func)(uint32_t) : Function to be called by the IRQ
 * Return Value : none
 *******************************************************************************/
static void R_SWITCH_RemoveSetCallback (int irq )
{
    /* Store the call-back function pointer into the global variable */
    g_switch_press_callback_func[irq] = NULL;
}
/*****************************************************************************
 End of function  R_SWITCH_SetPressCallback
 ******************************************************************************/

/*******************************************************************************
 * Function Name : R_SWITCH_SetReleaseCallback
 * Description   : Takes a pointer to a function, and sets it as the call-back
 *                 function for the switch interrupts. The passed function is
 *                 executed every time any switch is released.
 * Argument      : pointer to call-back function (set to NULL to disable)
 * Return value  : none
 *******************************************************************************/
void R_SWITCH_SetReleaseCallback (int irq, void (*callback) (int))
{
    /* Store the callback function pointer into the global variable */
    g_switch_release_callback_func[irq] = callback;
}
/*****************************************************************************
 End of function  R_SWITCH_SetReleaseCallback
 ******************************************************************************/

/******************************************************************************
 Private Functions
 ******************************************************************************/

/******************************************************************************
 * Function Name: init_switch_debounce_timer
 * Description  : Initialises MTU2 channel 3 to generate a 10ms delay for
 *                debouncing the switches.
 * Arguments    : none
 * Return Value : none
 ******************************************************************************/
static void init_switch_debounce_timer (void)
{
    /* Initialise the register of MTU2 */
    MTU2.TSTR &= 0x7F;

    /* Enable access to write-protected MTU2 registers */
    MTU2.TRWER = 1u;

    /* Clear the timer count */
    MTU2.TCNT_4 = 0u;

    /* MTU2 channel4 normal operation */
    MTU2.TMDR_4 = 0u;

    /* Timer output is not required */
    MTU2.TIORH_4 = 0;

    /* Clear the compare match status flag */
    MTU2.TSR_4 &= 0xFE;

    /* Enable TGRA interrupts */
    MTU2.TIER_4 = 0x01;

    /* Set the period */
    MTU2.TGRA_4 = DEBOUCE_TIMEOUT;

    /* TCNT cleared by TGRA compare match, rising edge count,
     clock source prescale P0/1024 = 32.226KHz,  */
    MTU2.TCR_4 = (0x20 | 0x05);

    /* Enable access to MTU2 write-protected registers */
    MTU2.TRWER = 0u;

    /* The setup process the interrupt IntTgfa function.*/
    R_INTC_RegistIntFunc(INTC_ID_TGI4A, &irq_switch_debounce);
    R_INTC_SetPriority(INTC_ID_TGI4A, ISR_MTU2_TGI4A_PRIORITY);
    R_INTC_Enable(INTC_ID_TGI4A);
}
/*****************************************************************************
 End of function  init_switch_debounce_timer
 ******************************************************************************/
static void start_debouce_timer ( int irq_num ) {


	/* Reload the period */
	MTU2.TGRA_4 = DEBOUCE_TIMEOUT;

	R_INTC_Enable(INTC_ID_TGI4A);

	/* Start the count of channel 4 of MTU2. */
	MTU2.TSTR |= 0x80;
	if ( irq_num < NUM_OF_IRQ_SW )
		if ( sw_info[irq_num].led_pin != NC )
			gpio_write(sw_info[irq_num].led_pin, 1);
}

static void restart_debouce_timer ( int irq_num ) {

	/* Reload the period */
	MTU2.TGRA_4 = MULTI_BUT_TIMEOUT;

	R_INTC_Enable(INTC_ID_TGI4A);

	/* Start the count of channel 4 of MTU2. */
	MTU2.TSTR |= 0x80;

	if ( irq_num < NUM_OF_IRQ_SW )
		if ( sw_info[irq_num].led_pin != NC )
			gpio_write(sw_info[irq_num].led_pin, 1);
}
static void stop_debouce_timer ( int irq_num ) {

	uint16_t dummy_read = 0u;

	/* Stop the count of channel 4 of MTU2. */
	MTU2.TSTR &= 0x7F;

	/* Disable the MTU2 channel 4 interrupt */
	R_INTC_Disable(INTC_ID_TGI4A);

	/* Clearing the status flag requires a dummy read */
	dummy_read = MTU2.TSR_4;

	/* Clear the TGFA flag */
	if (0x01 == (dummy_read & 0x01))
	{
		MTU2.TSR_4 &= 0xFE;
	}

	if ( irq_num < NUM_OF_IRQ_SW )
		if ( sw_info[irq_num].led_pin != NC )
			gpio_write(sw_info[irq_num].led_pin, 0);
}
/******************************************************************************
 * Function Name: switch_debounce_delay
 * Description  : MT2 channel 4's interrupt handler. Sets flag to indicate end
 *                completion of interrupt.
 * Arguments    : none
 * Return Value : none
 ******************************************************************************/
static void switch_debounce_delay (void)
{


    /* Wait for the the 10ms period to expire */
    while (0x01 != (MTU2.TSR_4 & 0x01))
    {
        R_COMPILER_Nop();
    }

    /* Reload the period */
    MTU2.TGRA_4 = 5000;
}
/*****************************************************************************
 End of function  switch_debounce_delay
 ******************************************************************************/

/******************************************************************************
 * Function Name: mtu_int_gfa
 * Description  : MT2 channel 4's interrupt handler. Sets flag to indicate end
 *                completion of interrupt.
 * Arguments    : uint32_t int_sense (1:edge )
 *                                   (0:level)
 * Return Value : none
 ******************************************************************************/
static void irq_switch_debounce (uint32_t int_sense)
{
    (void) int_sense;
    int ret = 0;
    int irq_num = g_pending_sw_irq-INTC_ID_IRQ0;

    stop_debouce_timer(irq_num);

	// send callback

	if ( g_pending_sw_irq >= INTC_ID_IRQ0 && g_pending_sw_irq <= INTC_ID_IRQ4 ){
		/* Check if switch press call-back function is not NULL */
		if (g_switch_press_callback_func[irq_num])
		{
			/* Execute user call-back function */
			ret = g_switch_press_callback_func[irq_num](irq_num, ++g_pending_sw_irq_count);
			if ( ret ) {
				// restart timer for multi-touch
				restart_debouce_timer( irq_num );
			} else {
				g_pending_sw_irq_count = 0;
				R_INTC_Enable(g_pending_sw_irq);
			}
		}
	}
}
/*****************************************************************************
 End of function  mtu_int_gfa
 ******************************************************************************/
/*******************************************************************************
 * Function Name: irq_switch_trigger
 * Description  : Interrupt
 *              : Handler for IRQ3 / IRQ5 (SW1) interrupts
 * Arguments    : uint32_t int_sense : Interrupt detection
 *              :                    : INTC_LEVEL_SENSITIVE : Level sense
 *              :                    : INTC_EDGE_TRIGGER    : Edge trigger
 * Return Value : none
 *******************************************************************************/
static void irq_switch_trigger (uint32_t irq)
{
    uint8_t mask = 0;
    uint8_t dummy;
    uint16_t dummy_read;
    uint8_t irq_num = 0;


    R_INTC_Disable(irq);

    // Add pending switch to queue
    g_pending_sw_irq = irq;

    /* Add delay to Debounce the switch */
    /* Start Debounce timer  */
    start_debouce_timer( irq - INTC_ID_IRQ0);


    /* Clearing the status flag requires a dummy read */
    dummy_read = INTC.IRQRR;

    if ( irq < INTC_ID_IRQ5 ) {

    	// Create mask
    	irq_num = irq - INTC_ID_IRQ0;
    	mask = 1 << (irq_num);

		/* Clear IRQ3/5 interrupt flag  */
    	dummy = INTC.IRQRR;
		INTC.IRQRR &= ~mask;
    }


    //R_INTC_Enable(irq);
}
/*******************************************************************************
 * End of Function irq_switch_trigger
 *******************************************************************************/

/******************************************************************************
 End  Of File
 ******************************************************************************/

