/*
 * gpio_addrdefine.h
 *
 *  Created on: May 6, 2020
 *      Author: Oceania
 */

#ifndef RENESAS_DRIVERS_R_GPIO_INC_GPIO_ADDRDEFINE_H_
#define RENESAS_DRIVERS_R_GPIO_INC_GPIO_ADDRDEFINE_H_

#include "iodefine_typedef.h"
#include "gpio_iodefine.h"

#define GPIO_BASE 0xFCFE3000uL


#define PORT(n)    (volatile unsigned short *)(GPIO_BASE + 0x000 + ((n)*4))
#define PSR(n)     (volatile unsigned  long *)(GPIO_BASE + 0x100 + ((n)*4))
#define PPR(n)     (volatile unsigned short *)(GPIO_BASE + 0x200 + ((n)*4))
#define PM(n)      (volatile unsigned short *)(GPIO_BASE + 0x300 + ((n)*4))
#define PMC(n)     (volatile unsigned short *)(GPIO_BASE + 0x400 + ((n)*4))
#define PFC(n)     (volatile unsigned short *)(GPIO_BASE + 0x500 + ((n)*4))
#define PFCE(n)    (volatile unsigned short *)(GPIO_BASE + 0x600 + ((n)*4))
#define PNOT(n)    (volatile unsigned short *)(GPIO_BASE + 0x700 + ((n)*4))
#define PMSR(n)    (volatile unsigned  long *)(GPIO_BASE + 0x800 + ((n)*4))
#define PMCSR(n)   (volatile unsigned  long *)(GPIO_BASE + 0x900 + ((n)*4))
#define PFCAE(n)   (volatile unsigned short *)(GPIO_BASE + 0xa00 + ((n)*4))
#define PIBC(n)    (volatile unsigned short *)(GPIO_BASE + 0x4000 +((n)*4))
#define PBDC(n)    (volatile unsigned short *)(GPIO_BASE + 0x4100 +((n)*4))
#define PIPC(n)    (volatile unsigned short *)(GPIO_BASE + 0x4200 +((n)*4))

#define FUNC_PMC_MODE 		0x08
#define FUNC_PFCAE_MODE		0x04
#define FUNC_PFCE_MODE		0x02
#define FUNC_PFC_MODE		0x01
#define FUNC_PM_MODE		0x01

#define SET_PIN(F,M) F&M ? 1:0

#endif /* RENESAS_DRIVERS_R_GPIO_INC_GPIO_ADDRDEFINE_H_ */
