/*
 * r_port_if.h
 *
 *  Created on: May 6, 2020
 *      Author: Oceania
 */

#ifndef RENESAS_DRIVERS_R_GPIO_R_PORT_IF_H_
#define RENESAS_DRIVERS_R_GPIO_R_PORT_IF_H_

#include "PinNames.h"

typedef struct {
    PinName 		pin;
    PinFunc 		function;
    PinDirection	dir;
} st_port_config_t;

typedef struct {
	uint8_t channel;
	uint8_t count;
	st_port_config_t* p_config_table;
} st_port_init_config_t;

void set_pin_function( const st_port_config_t *map);
int set_pins_function( const st_port_init_config_t *maps);

#endif /* RENESAS_DRIVERS_R_GPIO_R_PORT_IF_H_ */
