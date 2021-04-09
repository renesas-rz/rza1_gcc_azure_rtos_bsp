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
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name   : r_ether.c
* Description : ETHER driver
*******************************************************************************/

/******************************************************************************
Includes <System Includes> , ï¿½gProject Includesï¿½h
******************************************************************************/
#include <stdio.h>
#include <string.h>
#include "r_typedefs.h"
#include "compiler_settings.h"
#include "r_task_priority.h"
#include "r_cache_l1_rz_api.h"

#include "dev_drv.h"
#include "r_intc.h"

#include "cpg_iodefine.h"
#include "gpio_iodefine.h"
#include "ether_iodefine.h"

#include "gpio_iobitmask.h"
#include "cpg_iobitmask.h"

#include "r_phy.h"
#include "r_ether.h"
#include "trace.h"

#include "riic_max9856_drv.h"

#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
/* nothing */
#elif (TARGET_BOARD == TARGET_BOARD_RSK)
#include "riic_cat9554_if.h"
#endif /* TARGET_BOARD */

/******************************************************************************
Defines
******************************************************************************/


/* Comment this line out to turn ON module trace in this file */
#undef _TRACE_ON_

#ifndef _TRACE_ON_
#undef TRACE
#define TRACE(x)
#endif


/* Functions to allocate the memory */
#ifdef _ALLOC_MEM_
extern  void *etMalloc(size_t stLength, int32_t iAlign, void **ppvBase);
extern  void etFree(void *pvBase);
#endif

/******************************************************************************
Private global variables and functions
******************************************************************************/
/* ---- Descriptor ---- */
static uint32_t geth_desc[((sizeof(txrx_descriptor_set_t) + sizeof(uint32_t))/sizeof(uint32_t))] __attribute__ ((section(".ETHER_DESCRIPTOR")));
static volatile txrx_descriptor_set_t_ptr   geth_desc_ptr;
static void *gpv_txrx_descriptor_base = NULL;
/* ---- Buffer ---- */
static volatile txrx_buffer_set_t_ptr       geth_buf_ptr;
static void *gpv_txrx_buffer_base = NULL;
/* ---- PHY link status ---- */
static int32_t  glink_status;
/* ---- EtherC operating mode ---- */
static _Bool    gbf_promiscuous = false;
/* ---- Rx call-back function ---- */
static void (*gpfn_rx_call_back)(void *) = NULL;
/* ---- Rx call-back parameter ---- */
static void *gpv_rx_parameter = NULL;
/* ---- Tx call-back function ---- */
static void (*gpfn_tx_call_back)(void *) = NULL;
/* ---- Tx call-back parameter ---- */
static void *gpv_tx_parameter = NULL;

static int32_t lan_desc_create(void);
static void lan_reg_reset(void);
static void lan_reg_set(int32_t link);

#define I_DIV_P         (4) /* Ick:Pck0 = 4:1 */
#define PCLK_5CYC       ((5 * I_DIV_P) / 2)

/******************************************************************************
* Outline       : Open the Ethernet driver
* Include       : none
* Function Name : R_Ether_Open
* Description   : Initialises the EtherC, E-DMAC, PHY, and buffer memory.
* Argument      : uint32_t ch       ; I : Ethernet channel number
*               : uint8_t mac_addr[]; I : Pointer to MAC address array
* Return Value  : R_ETHER_OK(0)    : Success
*               : R_ETHER_ERROR(-1): Error
******************************************************************************/
int32_t R_Ether_Open (uint32_t ch, uint8_t mac_addr[])
{
    (void) ch;
    int32_t             link;
    volatile uint8_t    dummy;

#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)

    /* Enable the clock to the ETHER module */
    CPG.STBCR7 &= (uint8_t)(~CPG_STBCR7_MSTP74);
    dummy = CPG.STBCR7;
    (void)dummy;

    //Configure ET_TXD0 (P8_0)
    GPIO.PMC8 |= GPIO_PMC8_PMC80;    // PMC8.0 = 1
    GPIO.PFC8 |= GPIO_PFC8_PFC80;    // PFC8.0 = 1
    GPIO.PFCE8 &= (uint16_t)(~GPIO_PFCE8_PFCE80);    // PFCE8.0 = 0
    GPIO.PFCAE8 &= (uint16_t)(~GPIO_PFCAE8_PFCAE80);    // PFCAE8.0 = 0
    GPIO.PIPC8 |= GPIO_PIPC8_PIPC80;    // PIPC8.0 = 1

    //Configure ET_TXD1 (P8_1)
    GPIO.PMC8 |= GPIO_PMC8_PMC81;   // PMC8.1 = 1
    GPIO.PFC8 |= GPIO_PFC8_PFC81;   // PFC8.1 = 1
    GPIO.PFCE8 &= (uint16_t)(~GPIO_PFCE8_PFCE81); // PFCE8.1 = 0
    GPIO.PFCAE8 &= (uint16_t)(~GPIO_PFCAE8_PFCAE81);  // PFCAE8.1 = 0
    GPIO.PIPC8 |= GPIO_PIPC8_PIPC81;    // PIPC8.1 = 1



    //Configure ET_TXD2 (P8_2)
    GPIO.PMC8 |= GPIO_PMC8_PMC82;   // PMC8.2 = 1
    GPIO.PFC8 |= GPIO_PFC8_PFC82;   // PFC8.2 = 1
    GPIO.PFCE8 &= (uint16_t)(~GPIO_PFCE8_PFCE82); // PFCE8.2 = 0
    GPIO.PFCAE8 &= (uint16_t)(~GPIO_PFCAE8_PFCAE82);  // PFCAE8.2 = 0
    GPIO.PIPC8 |= GPIO_PIPC8_PIPC82;    // PIPC8.2 = 1


    //Configure ET_TXD3 (P8_3)
    GPIO.PMC8 |= GPIO_PMC8_PMC83;   // PMC8.3 = 1
    GPIO.PFC8 |= GPIO_PFC8_PFC83;   // PFC8.3 = 1
    GPIO.PFCE8 &= (uint16_t)(~GPIO_PFCE8_PFCE83); // PFCE8.3 = 0
    GPIO.PFCAE8 &= (uint16_t)(~GPIO_PFCAE8_PFCAE83);  // PFCAE8.3 = 0
    GPIO.PIPC8 |= GPIO_PIPC8_PIPC83;    // PIPC8.3 = 1

    //Configure ET_TXCLK (P8_4)
    GPIO.PMC8 |= GPIO_PMC8_PMC84;    // PMC8.4 = 1
    GPIO.PFC8 |= GPIO_PFC8_PFC84;   // PFC8.4 = 1
    GPIO.PFCE8 &= (uint16_t)(~GPIO_PFCE8_PFCE84); // PFCE8.4 = 0
    GPIO.PFCAE8 &= (uint16_t)(~GPIO_PFCAE8_PFCAE84);  // PFCAE8.4 = 0
    GPIO.PIPC8 |= GPIO_PIPC8_PIPC84;    // PIPC8.4 = 1


    //Configure ET_TXER (P8_5)
    GPIO.PMC8 |= GPIO_PMC8_PMC85;   // PMC8.5 = 1
    GPIO.PFC8 |= GPIO_PFC8_PFC85;   // PFC8.5 = 1
    GPIO.PFCE8 &= (uint16_t)(~GPIO_PFCE8_PFCE85); // PFCE8.5 = 0
    GPIO.PFCAE8 &= (uint16_t)(~GPIO_PFCAE8_PFCAE85);  // PFCAE8.5 = 0
    GPIO.PIPC8 |= GPIO_PIPC8_PIPC85;    // PIPC8.5 = 1

    //Configure ET_TXEN (P8_6)
    GPIO.PMC8 |= GPIO_PMC8_PMC86;   // PMC8.6 = 1
    GPIO.PFC8 |= GPIO_PFC8_PFC86;   // PFC8.6 = 1
    GPIO.PFCE8 &= (uint16_t)(~GPIO_PFCE8_PFCE86); // PFCE8.6 = 0
    GPIO.PFCAE8 &= (uint16_t)(~GPIO_PFCAE8_PFCAE86);  // PFCAE8.6 = 0
    GPIO.PIPC8 |= GPIO_PIPC8_PIPC86;    // PIPC8.6 = 1


    //Configure ET_RXD0 (P8_7)
    GPIO.PMC8 |= GPIO_PMC8_PMC87;   // PMC8.7 = 1
    GPIO.PFC8 |= GPIO_PFC8_PFC87;   // PFC8.7 = 1
    GPIO.PFCE8 &= (uint16_t)(~GPIO_PFCE8_PFCE87); // PFCE8.7 = 0
    GPIO.PFCAE8 &= (uint16_t)(~GPIO_PFCAE8_PFCAE87);  // PFCAE8.7 = 0
    GPIO.PIPC8 |= GPIO_PIPC8_PIPC87;    // PIPC8.7 = 1


    //Configure ET_RXD1 (P8_8)
    GPIO.PMC8 |= GPIO_PMC8_PMC88;   // PMC8.8 = 1
    GPIO.PFC8 |= GPIO_PFC8_PFC88;   // PFC8.8 = 1
    GPIO.PFCE8 &= (uint16_t)(~GPIO_PFCE8_PFCE88); // PFCE8.8 = 0
    GPIO.PFCAE8 &= (uint16_t)(~GPIO_PFCAE8_PFCAE88);  // PFCAE8.8 = 0
    GPIO.PIPC8 |= GPIO_PIPC8_PIPC88;    // PIPC8.8 = 1

    //Configure ET_RXD2 (P8_9)
    GPIO.PMC8 |= GPIO_PMC8_PMC89;   // PMC8.9 = 1
    GPIO.PFC8 |= GPIO_PFC8_PFC89;   // PFC8.9 = 1
    GPIO.PFCE8 &= (uint16_t)(~GPIO_PFCE8_PFCE89); // PFCE8.9 = 0
    GPIO.PFCAE8 &= (uint16_t)(~GPIO_PFCAE8_PFCAE89);  // PFCAE8.9 = 0
    GPIO.PIPC8 |= GPIO_PIPC8_PIPC89;    // PIPC8.9 = 1


    //Configure ET_RXD3 (P8_10)
    GPIO.PMC8 |= GPIO_PMC8_PMC810;  // PMC8.10 = 1
    GPIO.PFC8 |= GPIO_PFC8_PFC810;  // PFC8.10 = 1
    GPIO.PFCE8 &= (uint16_t)(~GPIO_PFCE8_PFCE810);    // PFCE8.10 = 0
    GPIO.PFCAE8 &= (uint16_t)(~GPIO_PFCAE8_PFCAE810); // PFCAE8.10 = 0
    GPIO.PIPC8 |= GPIO_PIPC8_PIPC810;   // PIPC8.10 = 1


    //Configure ET_COL (P8_14)
    GPIO.PMC8 |= GPIO_PMC8_PMC814;  // PMC8.14 = 1
    GPIO.PFC8 |= GPIO_PFC8_PFC814;  // PFC8.14 = 1
    GPIO.PFCE8 &= (uint16_t)(~GPIO_PFCE8_PFCE814);    // PFCE8.14 = 0
    GPIO.PFCAE8 &= (uint16_t)(~GPIO_PFCAE8_PFCAE814); // PFCAE8.14 = 0
    GPIO.PIPC8 |= GPIO_PIPC8_PIPC814;   // PIPC8.14 = 1


    //Configure ET_CRS (P8_15)
    GPIO.PMC8 |= GPIO_PMC8_PMC815;  // PMC8.15 = 1
    GPIO.PFC8 |= GPIO_PFC8_PFC815;  // PFC8.15 = 1
    GPIO.PFCE8 &= (uint16_t)(~GPIO_PFCE8_PFCE815);    // PFCE8.15 = 0
    GPIO.PFCAE8 &= (uint16_t)(~GPIO_PFCAE8_PFCAE815); // PFCAE8.15 = 0
    GPIO.PIPC8 |= GPIO_PIPC8_PIPC815;   // PIPC8.15 = 1


    //Configure ET_MDC (P9_0)
    GPIO.PMC9 |= GPIO_PMC9_PMC90;   // PMC9.0 = 1
    GPIO.PFC9 |= GPIO_PFC9_PFC90;   // PFC9.0 = 1
    GPIO.PFCE9 &= (uint16_t)(~GPIO_PFCE9_PFCE90); // PFCE9.0 = 0
    GPIO.PFCAE9 &= (uint16_t)(~GPIO_PFCAE9_PFCAE90);  // PFCAE9.0 = 0
    GPIO.PIPC9 |= GPIO_PIPC9_PIPC90;    // PIPC9.0 = 1


    //Configure ET_MDIO (P9_1)
    GPIO.PMC9 |= GPIO_PMC9_PMC91;   // PMC9.1 = 1
    GPIO.PFC9 |= GPIO_PFC9_PFC91;   // PFC9.1 = 1
    GPIO.PFCE9 &= (uint16_t)(~GPIO_PFCE9_PFCE91); // PFCE9.1 = 0
    GPIO.PFCAE9 &= (uint16_t)(~GPIO_PFCAE9_PFCAE91);  // PFCAE9.1 = 0
    GPIO.PIPC9 |= GPIO_PIPC9_PIPC91;    // PIPC9.1 = 1

    //Configure ET_RXCLK (P9_2)
    GPIO.PMC9 |= GPIO_PMC9_PMC92;   // PMC9.2 = 1
    GPIO.PFC9 |= GPIO_PFC9_PFC92;   // PFC9.2 = 1
    GPIO.PFCE9 &= (uint16_t)(~GPIO_PFCE9_PFCE92); // PFCE9.2 = 0
    GPIO.PFCAE9 &= (uint16_t)(~GPIO_PFCAE9_PFCAE92);  // PFCAE9.2 = 0
    GPIO.PIPC9 |= GPIO_PIPC9_PIPC92;    // PIPC9.2 = 1


    //Configure ET_RXER (P9_3)
    GPIO.PMC9 |= GPIO_PMC9_PMC93;   // PMC9.3 = 1
    GPIO.PFC9 |= GPIO_PFC9_PFC93;   // PFC9.3 = 1
    GPIO.PFCE9 &= (uint16_t)(~GPIO_PFCE9_PFCE93); // PFCE9.3 = 0
    GPIO.PFCAE9 &= (uint16_t)(~GPIO_PFCAE9_PFCAE93);  // PFCAE9.3 = 0
    GPIO.PIPC9 |= GPIO_PIPC9_PIPC93;    // PIPC9.3 = 1


    //Configure ET_RXDV (P9_4)
    GPIO.PMC9 |= GPIO_PMC9_PMC94;   // PMC9.4 = 1
    GPIO.PFC9 |= GPIO_PFC9_PFC94;   // PFC9.4 = 1
    GPIO.PFCE9 &= (uint16_t)(~GPIO_PFCE9_PFCE94); // PFCE9.4 = 0
    GPIO.PFCAE9 &= (uint16_t)(~GPIO_PFCAE9_PFCAE94);  // PFCAE9.4 = 0
    GPIO.PIPC9 |= GPIO_PIPC9_PIPC94;    // PIPC9.4 = 1


    //Configure PHY_RST (P2_7)
    GPIO.PMC2 &= (uint16_t)(~GPIO_PMC2_PMC27);    // PMC2.7 = 0
    GPIO.PIPC2 &= (uint16_t)(~GPIO_PIPC2_PIPC27); // PIPC2.7 = 0
    GPIO.PM2 &= (uint16_t)(~GPIO_PM2_PM27);   // PM2.7 = 0


    //Reset the PHY transceiver
    GPIO.P2 &= (uint16_t)(~GPIO_P2_P27);   // P2.7 = 0
    R_OS_TaskSleep(10);

    //Take the PHY transceiver out of reset
    GPIO.P2 |= GPIO_P2_P27;  // P2.7 = 1

#elif (TARGET_BOARD == TARGET_BOARD_RSK)
	uint8_t px_addr, px_data, px_config;

	/* Port Expander 2 setting */
    /* PX1 EN1: High = Ethernet */
	R_RIIC_CAT9554_Open();
    px_addr = CAT9554_I2C_PX2;
	px_data = PX2_PX1_EN1;
    px_config = PX2_PX1_EN1;
	R_RIIC_CAT9554_Write(px_addr, px_data, px_config);
	R_RIIC_CAT9554_Close();

	/* Enable the clock to the ETHER module */
    CPG.STBCR7 &= (uint8_t)(~CPG_STBCR7_MSTP74);
    dummy = CPG.STBCR7;
    (void)dummy;

    //Configure ET_TXD0 (P2_4)
    GPIO.PMC2 |= GPIO_PMC2_PMC24;    // PMC2.4 = 1
    GPIO.PFC2 |= GPIO_PFC2_PFC24;    // PFC2.4 = 1
    GPIO.PFCE2 &= (uint16_t)(~GPIO_PFCE2_PFCE24);    // PFCE2.4 = 0
    GPIO.PFCAE2 &= (uint16_t)(~GPIO_PFCAE2_PFCAE24);    // PFCAE2.4 = 0
    GPIO.PIPC2 |= GPIO_PIPC2_PIPC24;    // PIPC2.4 = 1

    //Configure ET_TXD1 (P2_5)
    GPIO.PMC2 |= GPIO_PMC2_PMC25;   // PMC2.5 = 1
    GPIO.PFC2 |= GPIO_PFC2_PFC25;   // PFC2.5 = 1
    GPIO.PFCE2 &= (uint16_t)(~GPIO_PFCE2_PFCE25); // PFCE2.5 = 0
    GPIO.PFCAE2 &= (uint16_t)(~GPIO_PFCAE2_PFCAE25);  // PFCAE2.5 = 0
    GPIO.PIPC2 |= GPIO_PIPC2_PIPC25;    // PIPC2.5 = 1

    //Configure ET_TXD2 (P2_6)
    GPIO.PMC2 |= GPIO_PMC2_PMC26;   // PMC2.6 = 1
    GPIO.PFC2 |= GPIO_PFC2_PFC26;   // PFC2.6 = 1
    GPIO.PFCE2 &= (uint16_t)(~GPIO_PFCE2_PFCE26); // PFCE2.6 = 0
    GPIO.PFCAE2 &= (uint16_t)(~GPIO_PFCAE2_PFCAE26);  // PFCAE2.6 = 0
    GPIO.PIPC2 |= GPIO_PIPC2_PIPC26;    // PIPC2.6 = 1

    //Configure ET_TXD3 (P2_7)
    GPIO.PMC2 |= GPIO_PMC2_PMC27;   // PMC2.7 = 1
    GPIO.PFC2 |= GPIO_PFC2_PFC27;   // PFC2.7 = 1
    GPIO.PFCE2 &= (uint16_t)(~GPIO_PFCE2_PFCE27); // PFCE2.7 = 0
    GPIO.PFCAE2 &= (uint16_t)(~GPIO_PFCAE2_PFCAE27);  // PFCAE2.7 = 0
    GPIO.PIPC2 |= GPIO_PIPC2_PIPC27;    // PIPC2.7 = 1

    //Configure ET_TXCLK (P2_0)
    GPIO.PMC2 |= GPIO_PMC2_PMC20;    // PMC2.0 = 1
    GPIO.PFC2 |= GPIO_PFC2_PFC20;   // PFC2.0 = 1
    GPIO.PFCE2 &= (uint16_t)(~GPIO_PFCE2_PFCE20); // PFCE2.0 = 0
    GPIO.PFCAE2 &= (uint16_t)(~GPIO_PFCAE2_PFCAE20);  // PFCAE2.0 = 0
    GPIO.PIPC2 |= GPIO_PIPC2_PIPC20;    // PIPC2.0 = 1

    //Configure ET_TXER (P2_1)
    GPIO.PMC2 |= GPIO_PMC2_PMC21;   // PMC2.1 = 1
    GPIO.PFC2 |= GPIO_PFC2_PFC21;   // PFC2.1 = 1
    GPIO.PFCE2 &= (uint16_t)(~GPIO_PFCE2_PFCE21); // PFCE2.1 = 0
    GPIO.PFCAE2 &= (uint16_t)(~GPIO_PFCAE2_PFCAE21);  // PFCAE2.1 = 0
    GPIO.PIPC2 |= GPIO_PIPC2_PIPC21;    // PIPC2.1 = 1

    //Configure ET_TXEN (P2_2)
    GPIO.PMC2 |= GPIO_PMC2_PMC22;   // PMC2.2 = 1
    GPIO.PFC2 |= GPIO_PFC2_PFC22;   // PFC2.2 = 1
    GPIO.PFCE2 &= (uint16_t)(~GPIO_PFCE2_PFCE22); // PFCE2.2 = 0
    GPIO.PFCAE2 &= (uint16_t)(~GPIO_PFCAE2_PFCAE22);  // PFCAE2.2 = 0
    GPIO.PIPC2 |= GPIO_PIPC2_PIPC22;    // PIPC2.2 = 1

    //Configure ET_RXD0 (P2_8)
    GPIO.PMC2 |= GPIO_PMC2_PMC28;   // PMC2.8 = 1
    GPIO.PFC2 |= GPIO_PFC2_PFC28;   // PFC2.8 = 1
    GPIO.PFCE2 &= (uint16_t)(~GPIO_PFCE2_PFCE28); // PFCE2.8 = 0
    GPIO.PFCAE2 &= (uint16_t)(~GPIO_PFCAE2_PFCAE28);  // PFCAE2.8 = 0
    GPIO.PIPC2 |= GPIO_PIPC2_PIPC28;    // PIPC2.8 = 1

    //Configure ET_RXD1 (P2_9)
    GPIO.PMC2 |= GPIO_PMC2_PMC29;   // PMC2.9 = 1
    GPIO.PFC2 |= GPIO_PFC2_PFC29;   // PFC2.9 = 1
    GPIO.PFCE2 &= (uint16_t)(~GPIO_PFCE2_PFCE29); // PFCE2.9 = 0
    GPIO.PFCAE2 &= (uint16_t)(~GPIO_PFCAE2_PFCAE29);  // PFCAE2.9 = 0
    GPIO.PIPC2 |= GPIO_PIPC2_PIPC29;    // PIPC2.9 = 1

    //Configure ET_RXD2 (P2_10)
    GPIO.PMC2 |= GPIO_PMC2_PMC210;   // PMC2.10 = 1
    GPIO.PFC2 |= GPIO_PFC2_PFC210;   // PFC2.10 = 1
    GPIO.PFCE2 &= (uint16_t)(~GPIO_PFCE2_PFCE210); // PFCE2.10 = 0
    GPIO.PFCAE2 &= (uint16_t)(~GPIO_PFCAE2_PFCAE210);  // PFCAE2.10 = 0
    GPIO.PIPC2 |= GPIO_PIPC2_PIPC210;    // PIPC2.10 = 1

    //Configure ET_RXD3 (P2_11)
    GPIO.PMC2 |= GPIO_PMC2_PMC211;  // PMC2.11 = 1
    GPIO.PFC2 |= GPIO_PFC2_PFC211;  // PFC2.11 = 1
    GPIO.PFCE2 &= (uint16_t)(~GPIO_PFCE2_PFCE211);    // PFCE2.11 = 0
    GPIO.PFCAE2 &= (uint16_t)(~GPIO_PFCAE2_PFCAE211); // PFCAE2.11 = 0
    GPIO.PIPC2 |= GPIO_PIPC2_PIPC211;   // PIPC2.11 = 1

    //Configure ET_COL (P1_14)
    GPIO.PMC1 |= GPIO_PMC1_PMC114;  // PMC1.14 = 1
    GPIO.PFC1 |= GPIO_PFC1_PFC114;  // PFC1.14 = 1
    GPIO.PFCE1 &= (uint16_t)(~GPIO_PFCE1_PFCE114);    // PFCE1.14 = 0
    GPIO.PFCAE1 &= (uint16_t)(~GPIO_PFCAE1_PFCAE114); // PFCAE1.14 = 0
    GPIO.PIPC1 |= GPIO_PIPC1_PIPC114;   // PIPC1.14 = 1

    //Configure ET_CRS (P2_3)
    GPIO.PMC2 |= GPIO_PMC2_PMC23;  // PMC2.3 = 1
    GPIO.PFC2 |= GPIO_PFC2_PFC23;  // PFC2.3 = 1
    GPIO.PFCE2 &= (uint16_t)(~GPIO_PFCE2_PFCE23);    // PFCE2.3 = 0
    GPIO.PFCAE2 &= (uint16_t)(~GPIO_PFCAE2_PFCAE23); // PFCAE2.3 = 0
    GPIO.PIPC2 |= GPIO_PIPC2_PIPC23;   // PIPC2.3 = 1

    //Configure ET_MDC (P5_9)
    GPIO.PMC5 |= GPIO_PMC5_PMC59;   // PMC5.9 = 1
    GPIO.PFC5 |= GPIO_PFC5_PFC59;   // PFC5.9 = 1
    GPIO.PFCE5 &= (uint16_t)(~GPIO_PFCE5_PFCE59); // PFCE5.9 = 0
    GPIO.PFCAE5 &= (uint16_t)(~GPIO_PFCAE5_PFCAE59);  // PFCAE5.9 = 0
    GPIO.PIPC5 |= GPIO_PIPC5_PIPC59;    // PIPC5.9 = 1

    //Configure ET_MDIO (P3_3)
    GPIO.PMC3 |= GPIO_PMC3_PMC33;   // PMC3.3 = 1
    GPIO.PFC3 |= GPIO_PFC3_PFC33;   // PFC3.3 = 1
    GPIO.PFCE3 &= (uint16_t)(~GPIO_PFCE3_PFCE33); // PFCE3.3 = 0
    GPIO.PFCAE3 &= (uint16_t)(~GPIO_PFCAE3_PFCAE33);  // PFCAE3.3 = 0
    GPIO.PIPC3 |= GPIO_PIPC3_PIPC33;    // PIPC3.3 = 1

    //Configure ET_RXCLK (P3_4)
    GPIO.PMC3 |= GPIO_PMC3_PMC34;   // PMC3.4 = 1
    GPIO.PFC3 |= GPIO_PFC3_PFC34;   // PFC3.4 = 1
    GPIO.PFCE3 &= (uint16_t)(~GPIO_PFCE3_PFCE34); // PFCE3.4 = 0
    GPIO.PFCAE3 &= (uint16_t)(~GPIO_PFCAE3_PFCAE34);  // PFCAE3.4 = 0
    GPIO.PIPC3 |= GPIO_PIPC3_PIPC34;    // PIPC3.4 = 1

    //Configure ET_RXER (P3_5)
    GPIO.PMC3 |= GPIO_PMC3_PMC35;   // PMC3.5 = 1
    GPIO.PFC3 |= GPIO_PFC3_PFC35;   // PFC3.5 = 1
    GPIO.PFCE3 &= (uint16_t)(~GPIO_PFCE3_PFCE35); // PFCE3.5 = 0
    GPIO.PFCAE3 &= (uint16_t)(~GPIO_PFCAE3_PFCAE35);  // PFCAE3.5 = 0
    GPIO.PIPC3 |= GPIO_PIPC3_PIPC35;    // PIPC3.5 = 1

    //Configure ET_RXDV (P3_6)
    GPIO.PMC3 |= GPIO_PMC3_PMC36;   // PMC3.6 = 1
    GPIO.PFC3 |= GPIO_PFC3_PFC36;   // PFC3.6 = 1
    GPIO.PFCE3 &= (uint16_t)(~GPIO_PFCE3_PFCE36); // PFCE3.6 = 0
    GPIO.PFCAE3 &= (uint16_t)(~GPIO_PFCAE3_PFCAE36);  // PFCAE3.6 = 0
    GPIO.PIPC3 |= GPIO_PIPC3_PIPC36;    // PIPC3.6 = 1

#endif /* TARGET_BOARD */

	R_OS_TaskSleep(10);

    /* ==== Resets the E-MAC,E-DMAC === */
    lan_reg_reset();

    /* ==== Initialise of buffer memory ==== */
    if (lan_desc_create())
    {
        return R_ETHER_ERROR;
    }

    /* ==== MAC address setting ==== */
    if (NULL == mac_addr)
    {
        /*
         * If 2nd parameter is 0, the MAC address should be acquired from the system,
         * depending on user implementation. (e.g.: EEPROM)
         */
    }
    else
    {
        lan_mac_set(mac_addr);
    }

    /* ==== E-MAC/E-DMAC setting ==== */
    link = phy_autonego();                    /* Detects the communication mode */
    glink_status = link;
    /* ++ REE/EDC */
    /* This assumes that the Ethernet cable will be connected when the interface
     * is opened. The driver has been changed so that it is configured for a full duplex link.
     * If the link is different then the lan_link_check function will change the MAC configuration
     * to suit the link. The caveat in this modification is that this function must be called!
     */
#if 0
    if ( NEGO_FAIL == link )
    {
        return R_ETHER_ERROR;                 /* Failed to initialize */
    }
    else
    {
        lan_reg_set(link);
    }
#else
    lan_reg_set(FULL_TX);
#endif
    /* -- REE/EDC */

    return R_ETHER_OK;
}

/******************************************************************************
* ID            : ï¿½|
* Outline       : Close the ethernet driver
* Include       : none
* Function Name : R_Ether_Close
* Description   : Stops the E-MAC/E-DMAC.
* Argument      : uint32_t ch; I : Ethernet channel number
* Return Value  : R_ETHER_OK(0)     : Success
*               : R_ETHER_ERROR(-1) : Error
******************************************************************************/
int32_t R_Ether_Close (uint32_t ch)
{
    (void) ch;
    /* ==== Resets the E-MAC,E-DMAC === */
    lan_reg_reset();

    /* ==== Disables the E-DMAC interrupt === */
    R_INTC_Disable(INTC_ID_ETHERI);

    /* ==== Free the memory if it has been allocated === */
#ifdef _ALLOC_MEM_
    etFree(gpv_txrx_descriptor_base);
    etFree(gpv_txrx_buffer_base);
#endif
    return R_ETHER_OK;
}

/******************************************************************************
* ID            : ï¿½|
* Outline       : Read the frame
* Include       : none
* Function Name : R_Ether_Read
* Description   : Copies the received Ethernet frame on the Ethernet channel
*               : specified by channel number to the receive buffer.
* Argument      : uint32_t ch; I : Ethernet channel number
*               : void *buf  ; I : Pointer to Ethernet receive buffer
* Return Value  : Greater than 0   : Success. Returns number of bytes received
*               : R_ETHER_ERROR(-1): Error
*               : R_ETHER_HARD_ERROR(-3): Hardware error. Software reset is necessary to recover
*               : R_ETHER_RECOVERABLE(-4): Recoverable error
*               : R_ETHER_NODATA(-5): No data received
******************************************************************************/
int32_t R_Ether_Read (uint32_t ch, void *buf)
{
    (void) ch;
    edmac_recv_desc_t * p   = geth_desc_ptr->pRecv_end;   /* Current descriptor */
    int32_t             ret = 0;

    /* Sanity check 1 */
    if ((p < (edmac_recv_desc_t *)geth_desc_ptr)
    ||  (p > (edmac_recv_desc_t *)(geth_desc_ptr + sizeof(txrx_descriptor_set_t))))
    {
        TRACE(("R_Ether_Read: Error in list 0x%p\r\n", p));
        return R_ETHER_ERROR;
    }

    /* ==== No data ==== */
    if (p->rd0.BIT.RACT == 1)
    {
        return R_ETHER_NODATA;
    }
    /* ==== Receives 1 frame ==== */
    //TRACE(("Rx%p ", p);
    /* ---- Receive frame error ---- */
    if ((p->rd0.BIT.RFE == 1)  &&  ((p->rd0.LONG & 0x025f0000) != 0))
    {
        p->rd0.LONG &= 0x70000000;          /* Processes the error flag */
        ret = R_ETHER_ERROR;
    }
    /* ---- Copies the received frame ---- */
    else
    {
        /* Need to invalidate the cache */
        {
            void *pVirtual_addr = p->rd2.RBA;
            R_CACHE_L1_CleanInvalidLine((uint32_t) pVirtual_addr,p->rd1.RDL);
        }
        memcpy(buf, p->rd2.RBA, (size_t)p->rd1.RDL);
        ret = p->rd1.RDL;                   /* number of bytes received */
    }

    /* ---- Sets the receive descriptor to receive again ---- */
    p->rd0.BIT.RACT = 1;

    /* ---- Starts receiving frame ---- */
    if( (ETHER.EDRRR0 & 0x00000001) == 0 )
    {
        ETHER.EDRRR0 |=  0x00000001;
    }

    /* Sanity check 2 */
    if ((p->pNext < (edmac_recv_desc_t *)geth_desc_ptr)
    ||  (p->pNext > (edmac_recv_desc_t *)(geth_desc_ptr + sizeof(txrx_descriptor_set_t))))
    {
        TRACE(("R_Ether_Read: Error in list next 0x%p\r\n", p->pNext));
        return R_ETHER_ERROR;
    }
    /* ==== Update the current pointer value ==== */
    geth_desc_ptr->pRecv_end = p->pNext;
    //TRACE(("Rx%p\r\n", geth_desc_ptr->pRecv_end);
    return ret;
}
/******************************************************************************
* ID            : ï¿½|
* Outline       : Transfer the frame
* Include       : none
* Function Name : R_Ether_Write
* Description   : This function sends an Ethernet frame pointed by Ethernet frame
*               : pointer on the Ethernet channel specified by channel number.
*               : It updates the status of the transmit descriptor as new Ethernet
*               : frame is processed. It doesnï¿½ft check whether Ethernet frame
*               : transmission completed or not.
* Argument      : uint32_t ch; I : Ethernet channel number
*               : void *buf  ; I : Pointer to Ethernet transmit buffer
*               : uint32_t len;I : Ethernet frame length (unit:byte)
* Return Value  : R_ETHER_OK(0)    : Success
*               : R_ETHER_ERROR(-1): Error
******************************************************************************/
int32_t R_Ether_Write (uint32_t ch, void * buf, uint32_t len)
{
    (void) ch;
    edmac_send_desc_t * p   = geth_desc_ptr->pSend_top;   /* Current descriptor */

    /* ==== link is down ==== */
    if (glink_status == NEGO_FAIL)
    {
        return R_ETHER_ERROR;
    }

    /* Sanity check 1 */
    if ((p < (edmac_send_desc_t *)geth_desc_ptr)
    ||  (p > (edmac_send_desc_t *)(geth_desc_ptr + sizeof(txrx_descriptor_set_t))))
    {
        TRACE(("R_Ether_Write: Error in list 0x%p\r\n", p));
        return R_ETHER_ERROR;
    }

    /* ==== When the buffer is full ==== */
    if (p->td0.BIT.TACT == 1)
    {
        return R_ETHER_ERROR;
    }
    //TRACE(("Tx%p ", p);
    /* ==== Transfer 1 frame ==== */

    /* ---- Copies the transmit frame ---- */
    memcpy(p->td2.TBA, buf, len);
    
    /* Need to write back the cache to physical mem */
    {
        void *pVirtual_addr = (void *)p->td2.TBA;
        R_CACHE_L1_CleanInvalidLine((uint32_t) pVirtual_addr,len);
    }

    /* ---- Padding for the short frame ---- */
    if (len < MIN_FRAME_SIZE)
    {
        memset((p->td2.TBA + len), 0, (MIN_FRAME_SIZE - len));
        len = MIN_FRAME_SIZE;
    }

    /* ---- Sets the frame length ---- */
    p->td1.TDL = (uint16_t)len;

    /* ---- Sets the transmit descriptor to transmit again ---- */
    p->td0.BIT.TACT = 1;

    /* ---- Starts the transmission ---- */
    if ((ETHER.EDTRR0&0x00000003) != 3)
    {
        ETHER.EDTRR0 |= 0x00000003;
    }

    /* Sanity check 2 */
    if ((p->pNext < (edmac_send_desc_t *)geth_desc_ptr)
    ||  (p->pNext > (edmac_send_desc_t *)(geth_desc_ptr + sizeof(txrx_descriptor_set_t))))
    {
        TRACE(("R_Ether_Write: Error in list next 0x%p\r\n", p->pNext));
        return R_ETHER_ERROR;
    }

    /* ==== Update the current pointer value ==== */
    geth_desc_ptr->pSend_top = p->pNext;

    //TRACE(("Tx%p\r\n", geth_desc_ptr->pSend_top);
    return R_ETHER_OK;
}

/******************************************************************************
* ID            : ï¿½|
* Outline       : Create the descriptor
* Include       : none
* Function Name : lan_desc_create
* Description   : Ethernet performs initialization of the required transceiver
*               : buffer and the descriptor
* Argument      : none
* Return Value  : 0 for success or -1 on error
******************************************************************************/
static int32_t lan_desc_create (void)
{
    int32_t    i;
    /* ==== Descriptor area configuration ==== */
#ifdef _ALLOC_MEM_
    /* ---- Memory clear ---- */
    geth_desc_ptr = (txrx_descriptor_set_t_ptr)geth_desc;
    memset((void *)geth_desc_ptr, 0, sizeof(txrx_descriptor_set_t));
#else
    /* ---- Memory clear ---- */
    geth_desc_ptr = (txrx_descriptor_set_t_ptr)ADDR_OF_TXRX_DESC;
    memset((void *)geth_desc_ptr, 0, sizeof(txrx_descriptor_set_t));
#endif
    /* ==== Buffer area configuration ==== */
#ifdef _ALLOC_MEM_
    geth_buf_ptr  = (txrx_buffer_set_t_ptr)etMalloc(sizeof(txrx_buffer_set_t),
                                                    32UL,
                                                    &gpv_txrx_buffer_base);
    if (geth_buf_ptr)
    {
        memset((void *)geth_buf_ptr, 0, sizeof(txrx_buffer_set_t));
    }
    else
    {
        return -1;
    }
#else
    /* ---- Clear the area ---- */
    geth_buf_ptr  = (txrx_buffer_set_t_ptr)ADDR_OF_TXRX_BUFF;
    memset((void *)geth_buf_ptr, 0, sizeof(txrx_buffer_set_t));
#endif

    /* ---- Transmit descriptor ---- */
    for (i = 0; i < NUM_OF_TX_DESCRIPTOR; i++)
    {
        TRACE(("geth_desc_ptr->dsend[i].td2.TBA = 0x%p\r\n", geth_buf_ptr->bsend[i]));
        geth_desc_ptr->dsend[i].td2.TBA  = geth_buf_ptr->bsend[i];  /* TD2 */
        geth_desc_ptr->dsend[i].td1.TDL  = 0;                      /* TD1 */
        geth_desc_ptr->dsend[i].td0.LONG = 0x30000000;        /* TD0:1frame/1buf1buf, transmission disabled */

        if ((NUM_OF_TX_DESCRIPTOR - 1) != i)
        {
            geth_desc_ptr->dsend[i].pNext = &geth_desc_ptr->dsend[i + 1]; /* pNext */
        }
    }

    geth_desc_ptr->dsend[i - 1].td0.BIT.TDLE = 1;
    geth_desc_ptr->dsend[i - 1].pNext        = &geth_desc_ptr->dsend[0];

    /* ---- Receive descriptor ---- */
    for (i = 0; i < NUM_OF_RX_DESCRIPTOR; i++)
    {
        TRACE(("geth_desc_ptr->drecv[i].rd2.RBA = 0x%p\r\n", geth_buf_ptr->brecv[i]));
        geth_desc_ptr->drecv[i].rd2.RBA = geth_buf_ptr->brecv[i];      /* RD2 */
        geth_desc_ptr->drecv[i].rd1.RBL = (uint16_t)SIZE_OF_BUFFER;   /* RD1 */
        geth_desc_ptr->drecv[i].rd0.LONG= 0xB0000000;        /* RD0:1frame/1buf, reception enabled */

        if ((NUM_OF_RX_DESCRIPTOR - 1) != i )
        {
            geth_desc_ptr->drecv[i].pNext = &geth_desc_ptr->drecv[i + 1]; /* pNext */
        }
    }

    geth_desc_ptr->drecv[i - 1].rd0.BIT.RDLE = 1;                /* Set the last descriptor */
    geth_desc_ptr->drecv[i - 1].pNext        = &geth_desc_ptr->drecv[0];

    /* ---- Initialize descriptor management information ---- */
    geth_desc_ptr->pSend_top = &geth_desc_ptr->dsend[0];
    geth_desc_ptr->pRecv_end = &geth_desc_ptr->drecv[0];

    return 0;
}

/******************************************************************************
* ID            : ï¿½|
* Outline       : Reset registers
* Include       : none
* Function Name : lan_reg_reset
* Description   : E-MAC,E-DMAC registers initialization function
* Argument      : none
* Return Value  : none
******************************************************************************/
static void lan_reg_reset (void)
{
    volatile int32_t    j = 400;            /* Wait for BÆ’Ã“256 cycles */

                                            /*  ((IÆ’Ã“/BÆ’Ã“)*256)/6cyc = 8*256/6 = 342 */
    /* ---- ETHER software reset ---- */
    /* Ethernet Controller (ETHER)
    ARSTR - Software Reset Register
    b31:b1  Reserved   - Read only bit
    b0      ARST       - Software Reset : Reset all blocks of the ETHER (1) */
    ETHER.ARSTR |=  0x00000001;
    while (j--)
    {
        /* Wait for BÆ’Ã“256 cycles */
    }

    /* ---- E-DMAC software reset ---- */
    /* Ethernet Controller (ETHER)
    EDSR - E-DMAC Start Register
    b31:b2  Reserved   - Read only bit
    b1      ENT        - E-DMAC Transmitting Unit Start : Start (1)
    b0      ENR        - E-DMAC Receiving Unit Start    : Start (1) */
    ETHER.EDSR0  =  0x00000003;

    /* Ethernet Controller (ETHER)
    EDMR - E-DMAC Mode Register
    b31:b7  Reserved   - Read only bit
    b6      DE         - Transmit/Receive Frame Endian
    b5:b4   DL[1:0]    - Transmit/Receive Descriptor Length
    b3:b2   Reserved   - Read only bit
    b1      SWRT       - Software Reset of Transmit FIFO Controller : Reset start (1)
    b0      SWRR       - Software Reset of Receive FIFO Controller  : Reset start (1) */
    ETHER.EDMR0  =  0x00000003;     /* Set SWRR and SWRT simultaneously */

    /* ---- Check clear software reset ---- */
    while(ETHER.EDMR0 !=  0x00000000)
    {
//        nop();
//        nop();
    }
}

/******************************************************************************
* ID            : ï¿½|
* Outline       : Set registers
* Include       : none
* Function Name : lan_reg_set
* Description   : E-DMAC, E-MAC initialization
* Argument      : int32_t link
* Return Value  : none
******************************************************************************/
static void lan_reg_set (int32_t link)
{
    /* ==== EDMAC ==== */
    /* Ethernet Controller (ETHER)
    TDLAR - Transmit Descriptor List Start Address Register
    b31:b0  TDLA[31:0] - Transmit Descriptor Start Address */
    ETHER.TDLAR0        = (uint32_t)&geth_desc_ptr->dsend[0];

    /* Ethernet Controller (ETHER)
    RDLAR - Receive Descriptor List Start Address Register
    b31:b0  RDLA[31:0] - Receive Descriptor Start Address */
    ETHER.RDLAR0        = (uint32_t)&geth_desc_ptr->drecv[0];

    /* Ethernet Controller (ETHER)
    TDFAR - Transmit Descriptor Fetch Address Register
    b31:b0  TDFA[31:0] - Transmit Descriptor Fetch Address */
    ETHER.TDFAR0        = (uint32_t)&geth_desc_ptr->dsend[0];

    /* Ethernet Controller (ETHER)
    RDFAR - Receive Descriptor Fetch Address Register
    b31:b0  RDFA[31:0] - Receive Descriptor Fetch Address */
    ETHER.RDFAR0        = (uint32_t)&geth_desc_ptr->drecv[0];

    /* Ethernet Controller (ETHER)
    TDFXR - Transmit Descriptor Finished Address Register
    b31:b0  TDFX[31:0] - Transmit Descriptor Finished Address */
    ETHER.TDFXR0        = (uint32_t)&geth_desc_ptr->dsend[NUM_OF_TX_DESCRIPTOR - 1];

    /* Ethernet Controller (ETHER)
    RDFXR - Receive Descriptor Finished Address Register
    b31:b0  RDFX[31:0] - Receive Descriptor Finished Address */
    ETHER.RDFXR0        = (uint32_t)&geth_desc_ptr->drecv[NUM_OF_RX_DESCRIPTOR - 1];

    /* Ethernet Controller (ETHER)
    TDFFR - Transmit Descriptor Final Flag Register
    b31:b1  Reserved   - Read only bit
    b0      TDLF       - Transmit Descriptor Queue Last Flag : Last descriptor (1) */
    ETHER.TDFFR0   = 0x00000001;

    /* Ethernet Controller (ETHER)
    RDFFR - Receive Descriptor Final Flag Register
    b31:b1  Reserved   - Read only bit
    b0      RDLF       - Receive Descriptor Queue Last Flag : Last descriptor (1) */
    ETHER.RDFFR0   = 0x00000001;

    /* Ethernet Controller (ETHER)
    EDMR - E-DMAC Mode Register
    b31:b7  Reserved   - Read only bit
    b6      DE         - Transmit/Receive Frame Endian : Little endian (1)
    b5:b4   DL[1:0]    - Transmit/Receive Descriptor Length : 16 bytes (00)
    b3:b2   Reserved   - Read only bit
    b1      SWRT       - Software Reset of Transmit FIFO Controller
    b0      SWRR       - Software Reset of Receive FIFO Controller */
#if defined(_BIG)
    ETHER.EDMR0         = 0x00000000; /* Big endian */
#else
    ETHER.EDMR0         = 0x00000040; /* Little endian */
#endif

    /* Ethernet Controller (ETHER)
    TRSCER - Transmit/Receive Status Copy Enable Register
    b31:b18 Reserved   - Read only bit
    b17     TABTCE     - TABT Bit Copy Direcrive : Reflects in the TFE bit (0)
    b16     RABTCE     - RABT Bit Copy Direcrive : Reflects in the RFE bit (0)
    b15:b11 Reserved   - Read only bit
    b10     DLCCE      - DLC Bit Copy Directive  : Reflects in the TFE bit (0)
    b9      CDCE       - CD  Bit Copy Directive  : Reflects in the TFE bit (0)
    b8      TROCE      - TRO Bit Copy Directive  : Reflects in the TFE bit (0)
    b7      RMAFCE     - RMAF Bit Copy Directive : Reflects in the RFE bit (0)
    b6      CEEFCE     - CEEF Bit Copy Directive : Reflects in the RFE bit (0)
    b5      CELFCE     - CELF Bit Copy Directive : Reflects in the RFE bit (0)
    b4      RRFCE      - RRF  Bit Copy Directive : Reflects in the RFE bit (0)
    b3      RTLFCE     - RTLF Bit Copy Directive : Reflects in the RFE bit (0)
    b2      RTSFCE     - RTSF Bit Copy Directive : Reflects in the RFE bit (0)
    b1      PRECE      - PRE  Bit Copy Directive : Reflects in the RFE bit (0)
    b0      CERFCE     - CERF Bit Copy Directive : Reflects in the RFE bit (0) */
    ETHER.TRSCER0  = 0x00000000;

    /* Ethernet Controller (ETHER)
    TFTR - Transmit FIFO Threshold Register
    b31:b11 Reserved   - Read only bit
    b10:b0  TFT[10:0]  - Transmit FIFO Threshold : Store and forward modes (H'000) */
    ETHER.TFTR0         = 0x00000000;

    /* Ethernet Controller (ETHER)
    FDR - FIFO Depth Register
    b31:b11 Reserved   - Read only bit
    b10:b8  TFD[2:0]   - Transmit FIFO Size : 2048 bytes (H'07)
    b7:b5   Reserved   - Read only bit
    b4:b0   RFD[4:0]   - Receive FIFO Size : 2048 bytes (H'07) */
    ETHER.FDR0  = (ETHER.FDR0 & ((uint32_t) ~0x0000071F)) | 0x00000707;

    /* Ethernet Controller (ETHER)
    RMCR - Receiving Method Control Register
    b31:b1  Reserved   - Read only bit
    b0      RNC        - Receive Enable Control : Continuous reception enabled (1) */
    ETHER.RMCR0 |= 0x00000001;

    /* Ethernet Controller (ETHER)
    FCFTR - Overflow Alert FIFO Threshold Register
    b31:b21 Reserved   - Read only bit
    b20:b16 RFF[4:0]   - Receive FIFO Overflow Alert Signal Output Threshold : 8 receive frams (H'07)
    b15:b8  Reserved   - Read only bit
    b7:b0   RFD[7:0]   - Receive FIFO Overflow Alert Signal Output Threshold : (2048-64) bytes (H'07) */
    ETHER.FCFTR0   = 0x00070007; /* Flow control is turned on when either of the condition is satisfied.*/

    /* Ethernet Controller (ETHER)
    RPADIR - Receive Data Padding Insert Register
    b31:b21 Reserved   - Read only bit
    b20:b16 PADS[4:0]  - Padding Size : No padding insertion (H'00)
    b15:b0  RADR[15:0] - Padding Slot : Inserts at first byte (H'0000) */
    ETHER.RPADIR0  = 0x00000000;


    /* ==== E-MAC ==== */
    /* Ethernet Controller (ETHER)
    ECMR - E-MAC Mode Register
    b31:b27 Reserved   - Read only bit
    b26     TRCCM      - Counter Clear Mode : Counter register is cleared to 0 by writing H'11111111 (0)
    b25:b24 Reserved   - Read only bit
    b23     RCSC       - Checksum Calculation : not automatically (0)
    b22     Reserved   - Read only bit
    b21     DPAD       - Data Padding : Enable insert (0)
    b20     RZPF       - PAUSE Frame Receptin with TIME = 0 : Disable (0)
    b19     ZPF        - PAUSE Frame Usage with TIME = 0 Enable : Disable
                         /Lost Carrier Error Detection Enable : Enable (0)
    b18     PFR        - PAUSE Frame Receive Mode : Not transferred to E-DMAC (0)
    b17     RXF        - Operating Mode for Receiving Port Flow Control : PAUSE frame detection is disabled (0)
    b16     TXF        - Operating Mode for Transmitting Port Flow Control : Automatic PAUSE frame is not transmitted (0)
    b15:b14 Reserved   - Read only bit
    b13     MCT        - Multicast Address Frame Receive Mode : When CAM is disabled, all multicast frames are received. (0)
    b12:b10 Reserved   - Read only bit
    b9      MPDE       - Magic Packet Detection Enable : Disable (0)
    b8:b7   Reserved   - Read only bit
    b6      RE         - Reception Enable : Disable (0)
    b5      TE         - Transmission Enable : Disable (0)
    b4      Reserved   - Read only bit
    b3      ILB        - Internal Loop Back Mode : Disable loop back (0)
    b2      Reserved   - Read only bit
    b1      DM         - Duplex Mode
    b0      PRM        - Promiscuous Mode : Normal operation (0) */
    if (gbf_promiscuous)
    {
        ETHER.ECMR0      = 0x00000001;
    }
    else
    {
        ETHER.ECMR0      = 0x00000000;
    }

    /* Ethernet Controller (ETHER)
    RFLR - Receive Frame Length Register
    b31:b18 Reserved   - Read only bit
    b17:b0  RFL[17:0]  - Receive Frame Length : 1518 bytes (H'00000) */
    ETHER.RFLR0           = 0x00000;

    /* Ethernet Controller (ETHER)
    APR - Automatic PAUSE Frame Register
    b31:b16 Reserved   - Read only bit
    b15:b0  AP[15:0]   - Automatic PAUSE : Flow control is disabled (H'0000)*/
    ETHER.APR0            = 0x00000000;

    /* Ethernet Controller (ETHER)
    MPR - Manual PAUSE Frame Register
    b31:b16 Reserved   - Read only bit
    b15:b0  MP[15:0]   - Manual PAUSE : Flow control is disabled (H'0000)*/
    ETHER.MPR0            = 0x00000000;

    /* Ethernet Controller (ETHER)
    TPAUSER - Automatic PAUSE Frame Retransmit Count Register
    b31:b16 Reserved   - Read only bit
    b15:b0  TPAUSE[15:0] - Upper Limit for Automatic PAUSE Frame : Retransmit count is unlimited (H'0000) */
    ETHER.TPAUSER0        = 0x00000000;

    /* Ethernet Controller (ETHER)
    ECMR - E-MAC Mode Register
    b1      DM         - Duplex Mode : Full-duplex (1) or Half-duplex (0) */
    if ((FULL_TX == link) || (FULL_10M == link))
    {
        ETHER.ECMR0 |= 0x00000002;               /* Set to full-duplex mode */
    }
    else
    {
        ETHER.ECMR0 &= (uint32_t)~(0x00000002);               /* Set to half-duplex mode */
    }


    /* ==== Interrupt-related ==== */
    /* Ethernet Controller (ETHER)
    EESR - E-MAC/E-DMAC Status Register
    b31:b30 TWB[1:0]   - Write-Back Complete                          : Has not completed, or no transmission directive (00)
    b29     TC[1]      - Frame Transmission Complete(TC[1:0]=11)      : Has not completed, or no transmission directive (00)
    b28     TUC        - Transmit Underflow Frame Write-Back Complete : Has not completed for the frame causing transmit underflow (0)
    b27     ROC        - Receive Overflow Frame Write-Back Complete   : Has not completed for the frame causing transmit overflow (0)
    b26     TABT       - Transmit Abort Detect                        : Has not aborted or no transmission direcrive (0)
    b25     RABT       - Receive Abort Detect                         : Has not aborted or no reception direcrive (0)
    b24     RFCOF      - Receive Frame Counter Overflow               : Has not overflowed (0)
    b23     Reserved   - Read only bit
    b22     ECI        - E-MAC Status Register Source                 : E-MAC status interrupt source has not been detected (0)
    b21     TC[0]      - Frame Transmission Complete(TC[1:0]=11)      : Has not completed, or no transmission directive (00)
    b20     TDE        - Transmit Descriptor Empty                    : TDE error has not occurred (0)
    b19     TFUF       - Transmit FIFO Underflow                      : Has not underflowed (0)
    b18     FR         - Frame Reception                              : Has not received (0)
    b17     RDE        - Receive Descriptor Empty                     : RDE error has not occurred (0)
    b16     RFOF       - Receive FIFO Overflow                        : Has not overflowed (0)
    b15:b11 Reserved   - Read only bit
    b10     DLC        - Detect Loss of Carrier                       : Has not detected (0)
    b9      CD         - Delayed Collision Detect                     : Has not detected (0)
    b8      TRO        - Transmit Retry Over                          : Has not detected (0)
    b7      RMAF       - Receive Multicast Address Frame              : Has not received (0)
    b6      CEEF       - Carrier Extension Error                      : Has not occurred (0)
    b5      CELF       - Carrier Extension Loss                       : Has not occurred (0)
    b4      RRF        - Receive Residual-Bit Frame                   : Has not received (0)
    b3      RTLF       - Receive Too-Long Frame                       : Has not received (0)
    b2      RTSF       - Receive Too-Short Frame                      : Has not received (0)
    b1      PRE        - PHY-LSI Receive Error                        : Has not detected (0)
    b0      CERF       - CRC Error on Received Frame                  : Has not detected (0) */
    ETHER.EESR0          = 0xFF7F07FF;      /* Clear all status (by writing 1) */

    /* Ethernet Controller (ETHER)
    EESIPR - E-MAC/E-DMAC Status Interrupt Permission Register
    b31:b0             - Refer to EESR comment : Enable/Disable (1/0) */
    ETHER.EESIPR0   = 0x00040000;      /* b18     FR         - Frame Reception (1) */

    /* Ethernet Controller (ETHER)
    CheckSum Mode Register */
    ETHER.CSMR      = 0x00000000;

    /* Ethernet Controller (ETHER)
    ECSR - E-MAC Status Register
    b31:b5  Reserved   - Read only bit
    b4      PFROI      - PAUSE Frame Retransmit Retry Over            : Has not exceeded the upper limit (0)
    b3:b1   Reserved   - Read only bit
    b0      ICD        - Illegal Carrier Detection                    : Has not detected (0) */
    ETHER.ECSR0            = 0x00000011;      /* Clear all status (clear by writing 1) */

    /* Ethernet Controller (ETHER)
    ECSIPR - E-MAC Interrupt Permission Register
    b31:b0             - Refer to ECSR comment : Enable (1) or Disable (0) */
    ETHER.ECSIPR0     = 0;

    /* ==== EthernetÅ â€žâ€šÃ¨ï¿½Å¾â€šÃ�Æ’nÆ’â€œÆ’hÆ’â€°â€œoËœ^ ==== */
    R_INTC_RegistIntFunc(INTC_ID_ETHERI, INT_Ether);

    /* ==== EthernetÅ â€žâ€šÃ¨ï¿½Å¾â€šÃ�â€”Dï¿½Ã¦â€œx=6ï¿½Ã�â€™Ã¨ ==== */
    R_INTC_SetPriority(INTC_ID_ETHERI, ISR_ETHER_PRIORITY);

    /* ==== Enables the E-DMAC interrupt === */
    R_INTC_Enable(INTC_ID_ETHERI);

    /* Ethernet Controller (ETHER)
    ECMR - E-MAC Mode Register
    b6      RE         - Reception Enable    : Enable (1)
    b5      TE         - Transmission Enable : Enable (1) */
    ETHER.ECMR0    |= 0x00000060;

    /* ==== Enable transmission/reception ==== */
    /* Ethernet Controller (ETHER)
    EDRRR - E-DMAC Receive Request Register
    b31:b1  Reserved   - Read only bit
    b0      RR         - Receive Request : The E-DMAC starts receive operation (1) */
    if((ETHER.EDRRR0&0x00000001) == 0)
    {
        ETHER.EDRRR0    |= 0x00000001;
    }
}

/******************************************************************************
* ID            : ï¿½|
* Outline       : Interrupt handler (Transmission)
* Include       : none
* Function Name : lan_send_handler_isr
* Description   : Interrupt handler related to transmission regarding E-DMAC(EESR)
* Argument      : uint32_t status
* Return Value  : none
******************************************************************************/
void lan_send_handler_isr (uint32_t status)
{
    UNUSED_PARAM(status);
    if (gpfn_tx_call_back)
    {
        gpfn_tx_call_back(gpv_tx_parameter);
    }
}

/******************************************************************************
* ID            : ï¿½|
* Outline       : Interrupt handler (Reception)
* Include       : none
* Function Name : lan_recv_handler_isr
* Description   : Interrupt handler related to reception regarding E-DMAC (EESR)
* Argument      : uint32_t status
* Return Value  : none
******************************************************************************/
void lan_recv_handler_isr (uint32_t status)
{
    UNUSED_PARAM(status);
    if (gpfn_rx_call_back)
    {
        gpfn_rx_call_back(gpv_rx_parameter);
    }
}

/******************************************************************************
* ID            : ï¿½|
* Outline       : Interrupt handler (E-MAC)
* Function Name : lan_etherc_handler_isr
* Description   : E-MAC interrupt function
* Argument      : uint32_t status
* Return Value  : none
******************************************************************************/
void lan_etherc_handler_isr (uint32_t status)
{
    UNUSED_PARAM(status);
    /* Please add the process as needed */

}

/* H'CA0 Ether */
void INT_Ether(uint32_t status)
{
    uint32_t stat_edmac;
    uint32_t stat_EtherC;
    volatile uint32_t w;
    (void)status;
       /* ---- Clear the interrupt request flag ---- */
    stat_edmac = ETHER.EESR0 & ETHER.EESIPR0;    /* Targets are restricted to allowed interrupts */
    ETHER.EESR0 = stat_edmac;
       /* ==== Transmission-related ==== */
    if (stat_edmac & EDMAC_EESIPR_INI_SEND)
    {
        lan_send_handler_isr(stat_edmac & EDMAC_EESIPR_INI_SEND);
    }
       /* ==== Reception-related ==== */
    if (stat_edmac & EDMAC_EESIPR_INI_RECV)
    {
        lan_recv_handler_isr( stat_edmac & EDMAC_EESIPR_INI_RECV );
    }
    /* ==== E-MAC-related ==== */
    if (stat_edmac & EDMAC_EESIPR_INI_EtherC)
    {
        /* ---- Clear the interrupt request flag ---- */
        stat_EtherC = ETHER.ECSR0 & ETHER.ECSIPR0;    /* Targets are restricted to allowed interrupts */
        ETHER.ECSR0   = stat_EtherC;
        lan_etherc_handler_isr(stat_EtherC);
    }
    /* ---- Wait the priority judging time ---- */
    for (w = 0; w < PCLK_5CYC; w++)
    {
        /* DO NOTHING */
    }
}

/******************************************************************************
* ID            : ï¿½|
* Outline       : link status check
* Function Name : lan_link_check
* Description   : PHY Link Status Check function
* Argument      : none
* Return Value  : The link status from the PHY
******************************************************************************/
int32_t lan_link_check(void)
{
    int32_t             link;

    link = phy_linkcheck();

    if (link != glink_status)
    {
        if ((FULL_TX == link) || (FULL_10M == link))
        {
            ETHER.ECMR0 |= 0x00000002;               /* Set to full-duplex mode */
        }
        else
        {
            ETHER.ECMR0 &= (uint32_t)~(0x00000002);               /* Set to half-duplex mode */
        }
        glink_status = link;
    }
    return link;
}

/******************************************************************************
* ID            : ï¿½|
* Outline       : lan_promiscuous_mode
* Function Name : lan_promiscuous_mode
* Description   : Function to change the MAC address filtering mode
* Argument      : bf_promiscuous - true to receive all messages
* Return Value  : none
******************************************************************************/
void lan_promiscuous_mode(_Bool bf_promiscuous)
{
    /* Set the global variable */
    gbf_promiscuous = bf_promiscuous;
    /* Check to see if there is a link */
    if (glink_status)
    {
        /* Update the register setting */
        if (bf_promiscuous==0)
        {
            ETHER.ECMR0 &= (uint32_t) ~(0x00000001);
        }
        else
        {
            ETHER.ECMR0 |= 0x00000001;
        }
    }
}

/******************************************************************************
* Outline       : lan_mac_set
* Function Name : lan_mac_set
* Description   : Function to set the MAC address
* Argument      : bf_promiscuous - true to receive all messages
* Return Value  : none
******************************************************************************/
void lan_mac_set(uint8_t mac_addr[])
{
    uint32_t            mac_h;
    uint32_t            mac_l;
    mac_h = ((uint32_t)mac_addr[0] << 24) |
            ((uint32_t)mac_addr[1] << 16) |
            ((uint32_t)mac_addr[2] << 8 ) |
            (uint32_t)mac_addr[3];

    mac_l = ((uint32_t)mac_addr[4] << 8 ) |
            (uint32_t)mac_addr[5];

    /* If the mac address is 0 set the adapter into promiscuous mode */
    if ((0 == mac_h) && (0 == mac_l))
    {
        gbf_promiscuous = true;
    }
    else
    {
        ETHER.MAHR0 = mac_h;
        ETHER.MALR0 = mac_l;
    }
}
/******************************************************************************
* End of Function : lan_mac_set
******************************************************************************/

/******************************************************************************
* ID            : ï¿½|
* Outline       : lan_set_rx_call_back
* Function Name : lan_set_rx_call_back
* Description   : Function to set the RX ISR call-back function
* Argument      : IN  pfn_rx_call_back - Pointer to the received call-back
*                 IN  gpv_rx_parameter - The parameter to pass to the call-back
* Return Value  : none
******************************************************************************/
void lan_set_rx_call_back(void (*pfn_rx_call_back)(void *),
                          void *pv_rx_parameter)
{
    gpfn_rx_call_back = pfn_rx_call_back;
    gpv_rx_parameter = pv_rx_parameter;
}

/******************************************************************************
* ID            : ï¿½|
* Outline       : lan_set_tx_call_back
* Function Name : lan_set_tx_call_back
* Description   : Function to set the TX ISR call-back function
* Argument      : IN  pfn_tx_call_back - Pointer to the transmit call-back
*                 IN  gpv_tx_parameter - The parameter to pass to the call-back
* Return Value  : none
******************************************************************************/
void lan_set_tx_call_back(void (*pfn_tx_call_back)(void *),
                          void *pv_tx_parameter)
{
    gpfn_tx_call_back = pfn_tx_call_back;
    gpv_tx_parameter = pv_tx_parameter;
}



/* End of file */
