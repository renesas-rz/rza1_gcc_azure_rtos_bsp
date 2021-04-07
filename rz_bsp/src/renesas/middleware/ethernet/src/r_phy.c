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
* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name   : r_phy.c
* $Rev: 2233 $
* $Date:: 2015-02-03 15:25:08 +0000#$
* Description : ETHER driver
*******************************************************************************/

/******************************************************************************
Includes <System Includes> , �gProject Includes�h
******************************************************************************/
#include <stdio.h>

#include "compiler_settings.h"
#include "r_task_priority.h"

#include "ether_iodefine.h"
#include "r_phy.h"

/******************************************************************************
Macro definitions
******************************************************************************/
/* PHYƒŒƒWƒXƒ^ */
#define BASIC_MODE_CONTROL_REG          (0)
#define BASIC_MODE_STATUS_REG           (1)
#define PHY_IDENTIFIER1_REG             (2)
#define PHY_IDENTIFIER2_REG             (3)
#define AN_ADVERTISEMENT_REG            (4)
#define AN_LINK_PARTNER_ABILITY_REG     (5)
#define AN_EXPANSION_REG                (6)
/* PHYƒAƒhƒŒƒX */
#define PHY_ADDR                        (0)        /* Confirm the pin connection of the PHY-LSI */
/* MIIŠÇ—�ƒCƒ“ƒ^ƒtƒF�[ƒXƒAƒNƒZƒX—p */
#define PHY_ST                          (1)
#define PHY_WRITE                       (1)
#define PHY_READ                        (2)
#define MDC_WAIT                        (6)        /* 400ns/4 < (IƒÓ*7)*MDC_WAIT   */

#define LOOP_100us                      (6700)     /* Loop counter for software wait */
                                                   /*  6666=100us/((1/400MHz)*6cyc) */

/******************************************************************************
Private global variables and functions
******************************************************************************/
static uint16_t phy_reg_read(uint16_t reg_addr);
//static void     phy_reg_write(uint16_t reg_addr, uint16_t data);
static void     mii_preamble(void);
static void     mii_cmd(uint16_t reg_addr, int32_t option);
static void     mii_reg_read(uint16_t *data);
#if 0
static void     mii_reg_write(uint16_t data);
static void     mii_ta10(void);
#endif
static void     mii_write_1(void);
static void     mii_write_0(void);
static void     mii_z(void);

/******************************************************************************
* ID            : �|
* Outline       : Automatic negotiation 
* Function Name : phy_autonego
* Description   : The result of automatic negotiation is read out by using the MII
*               : management interface, and indicated in the return value.
* Argument      : none
* Return Value  : 4(FULL_TX) :100 Mbps in full-duplex mode
*               : 3(HALF_TX) :100 Mbps in half-duplex mode
*               : 2(FULL_10M):10 Mbps in full-duplex mode
*               : 1(HALF_10M):10 Mbps in half-duplex mode
*               : 0(NEGO_FAIL) :Failure in negotiation
******************************************************************************/
int32_t phy_autonego (void)
{
    uint16_t         data;
    int32_t          link = NEGO_FAIL;
#if 0
    int32_t          i;
    volatile int32_t wait_10ms;
    volatile int32_t wait_50ms;
    volatile int32_t cnt_5sec;
    
    /* ==== Resets the PHY-LSI ==== */
    phy_reg_write(BASIC_MODE_CONTROL_REG, 0x8000);

    /* ++ REE/EDC */
    /*
     * This code assumes that the cable is connected when the driver is opened.
     * The WebEngine demonstration requires that the cable can be connected and disconnected
     * at any time. The function has been modified to use RTOS API's and simply to place the PHY
     * into auto negotiate mode and then the application will poll the PHY for link change conditions.
     */

    wait_50ms = LOOP_100us * 500;
    while (--wait_50ms)
    {
        ;
    }
    /* ==== Loop to wait for completion of automatic negotiation (for up to 5 seconds) ==== */
    cnt_5sec = 500;
    for (i = 0; i < cnt_5sec; i++)
    {
        /* ==== Wait for 10 ms to set up a benchmark for measurement of elapsed-time ==== */
        wait_10ms = LOOP_100us * 100;
        while (--wait_10ms)
        {
            ;
        }

        /* ==== Confirm completion of automatic negotiation ==== */
        data = phy_reg_read(BASIC_MODE_STATUS_REG);

        if (data & 0x0020)
        {                               /* PHY register 1 :Basic Status *
                                         * bit 5 :(1) completion of automatic negotiation process *
                                         *       :(0) non-completion of automatic negotiation process */
            /* ---- The capability of the partner as determined on completion of automatic negotiation ---- */
            data = phy_reg_read(AN_LINK_PARTNER_ABILITY_REG);
                                        /* PHY register 5 :Auto-Nego Link Partner Ability *
                                         * bit 8 :(1)100 Mbps/full-duplex mode is possible *
                                         * bit 7 :(1)100 Mbps/half-duplex mode is possible *
                                         * bit 6 :(1) 10 Mbps/full-duplex mode is possible *
                                         * bit 5 :(1) 10 Mbps/half-duplex mode is possible */
            /* ---- Break on completion of negotiation following detection of results ---- */
            if (data & 0x0100)
            {
                link = FULL_TX;
            }
            else if (data & 0x0080)
            {
                link = HALF_TX;
            }
            else if (data & 0x0040)
            {
                link = FULL_10M;
            }
            else if (data & 0x0020)
            {
                link = HALF_10M;
            }
            else
            {
                link = NEGO_FAIL;       /* This line is not expected to be executed */
            }
            /* ---- Processing to detect the result of automatic negotiation is complete ---- */
            break;
        }
    }
#else
    R_OS_TaskSleep(50UL);
    
    /* ==== Confirm completion of automatic negotiation ==== */
    data = phy_reg_read(BASIC_MODE_STATUS_REG);

    if (data & 0x0020)
    {                               /* PHY register 1 :Basic Status *
                                     * bit 5 :(1) completion of automatic negotiation process *
                                     *       :(0) non-completion of automatic negotiation process */
        /* ---- The capability of the partner as determined on completion of automatic negotiation ---- */
        data = phy_reg_read(AN_LINK_PARTNER_ABILITY_REG);
                                    /* PHY register 5 :Auto-Nego Link Partner Ability *
                                     * bit 8 :(1)100 Mbps/full-duplex mode is possible *
                                     * bit 7 :(1)100 Mbps/half-duplex mode is possible *
                                     * bit 6 :(1) 10 Mbps/full-duplex mode is possible *
                                     * bit 5 :(1) 10 Mbps/half-duplex mode is possible */
        /* ---- Break on completion of negotiation following detection of results ---- */
        if (data & 0x0100)
        {
            link = FULL_TX;
        }
        else if (data & 0x0080)
        {
            link = HALF_TX;
        }
        else if (data & 0x0040)
        {
            link = FULL_10M;
        }
        else if (data & 0x0020)
        {
            link = HALF_10M;
        }
        else
        {
            link = NO_LINK;
        }
    }
    else
    {
        link = NO_LINK;
    }
#endif
    return link;
 }

/******************************************************************************
* ID            : �|
* Outline       : link status check 
* Function Name : phy_linkcheck
* Description   : PHY Link Status Check function
* Argument      : none
* Return Value  : 4(FULL_TX) :100 Mbps in full-duplex mode
*               : 3(HALF_TX) :100 Mbps in half-duplex mode
*               : 2(FULL_10M):10 Mbps in full-duplex mode
*               : 1(HALF_10M):10 Mbps in half-duplex mode
*               : 0(NO_LINK) : The cable is not connected
******************************************************************************/
int32_t phy_linkcheck(void)
{
    uint16_t         data;
    int32_t          link;
    
    link = NEGO_FAIL;
    data = phy_reg_read(BASIC_MODE_STATUS_REG);

    if (data & 0x0004)
    {
        data = phy_reg_read(AN_LINK_PARTNER_ABILITY_REG);
                                    /* PHY register 5 :Auto-Nego Link Partner Ability *
                                     * bit 8 :(1)100 Mbps/full-duplex mode is possible *
                                     * bit 7 :(1)100 Mbps/half-duplex mode is possible *
                                     * bit 6 :(1) 10 Mbps/full-duplex mode is possible *
                                     * bit 5 :(1) 10 Mbps/half-duplex mode is possible */
        if (data & 0x0100)
        {
            link = FULL_TX;
        }
        else if (data & 0x0080)
        {
            link = HALF_TX;
        }
        else if (data & 0x0040)
        {
            link = FULL_10M;
        }
        else if (data & 0x0020)
        {
            link = HALF_10M;
        }
        else
        {
            link = NO_LINK;
        }
    }
    return link;
}

/******************************************************************************
* ID            : �|
* Outline       : Read the MII register 
* Function Name : phy_reg_read
* Description   : Obtains the value from a register of the PHY module.
* Argument      : uint16_t reg_addr
* Return Value  : The obtained register value
******************************************************************************/
static uint16_t phy_reg_read (uint16_t reg_addr)
{
    uint16_t  data;

    mii_preamble();
    mii_cmd(reg_addr, PHY_READ);
    mii_z();
    mii_reg_read(&data);
    mii_z();

    return data;
}

///******************************************************************************
//* ID            : �|
//* Outline       : Write the MII register
//* Function Name : phy_reg_write
//* Description   : Set the value in the PHY module register.
//* Argument      : uint16_t reg_addr, uint16_t data
//* Return Value  : none
//******************************************************************************/
//static void phy_reg_write (uint16_t reg_addr, uint16_t data)
//{
//    mii_preamble();
//    mii_cmd(reg_addr, PHY_WRITE);
//    mii_ta10();
//    mii_reg_write(data);
//    mii_z();
//
//}

/******************************************************************************
* ID            : �|
* Outline       : Execute preambles
* Function Name : mii_preamble
* Description   : 1 is output via the MII management interface.
* Argument      : none
* Return Value  : none
******************************************************************************/
static void mii_preamble (void)
{
    int16_t   i;

    i = 32;

    while (i > 0) 
    {
         /* 1 is output via the MII (Media Independent Interface) block. */
        mii_write_1();
        i--;
    }
}

/******************************************************************************
* ID            : �|
* Outline       : Execute commands
* Function Name : mii_cmd
* Description   : Placing the PHY module register in read or write mode.
* Argument      : uint16_t reg_addr, int32_t option
* Return Value  : none
******************************************************************************/
static void mii_cmd (uint16_t reg_addr, int32_t option)
{
    int32_t        i;
    uint16_t    data;

    data = 0;
    data = (PHY_ST << 14);              /* ST code      */
    if (PHY_READ == option)
    {
        data |= (PHY_READ << 12);       /* OP code(RD)  */
    }
    else
    {
        data |= (PHY_WRITE << 12);      /* OP code(WT)  */
    }

    data |= (PHY_ADDR << 7);            /* PHY Address  */
    data |= (uint16_t)(reg_addr << 2);  /* Reg Address  */

    for (i = 14; i > 0; i--)
    {
        if ((data & 0x8000) == 0)
        {
            mii_write_0();
        }
        else
        {
            mii_write_1();
        }

        data = (uint16_t)(data << 1);
    }
}

/******************************************************************************
* ID            : �|
* Outline       : Read bits
* Function Name : mii_reg_read
* Description   : Obtains the value of the PHY module register bit by bit.
* Argument      : uint16_t *data
* Return Value  : none
******************************************************************************/
static void mii_reg_read (uint16_t * data)
{
    volatile int32_t    i;
    volatile int32_t    j;
    uint16_t            reg_data;

    /* Data are read in one bit at a time */
    reg_data = 0;

    for (i = 16; i > 0; i--)
    {
        for (j = MDC_WAIT; j > 0; j--)
        {
            ETHER.PIR0 = 0x00000000;
        }

        for (j = MDC_WAIT; j > 0; j--)
        {
            ETHER.PIR0 = 0x00000001;
        }

        reg_data = (uint16_t)(reg_data << 1);
        reg_data |= (uint16_t)((ETHER.PIR0 & 0x00000008) >> 3);   /* MDI read */

        for (j = MDC_WAIT; j > 0; j--)
        {
            ETHER.PIR0 = 0x00000001;
        }

        for (j = MDC_WAIT; j > 0; j--)
        {
            ETHER.PIR0 = 0x00000000;
        }
    }
    *data = reg_data;
}
#if 0
/******************************************************************************
* ID            : �|
* Outline       : Wtite bits
* Function Name : mii_reg_write
* Description   : The value of the PHY module register is set one bit at a time.
* Argument      : uint16_t data
* Return Value  : none
******************************************************************************/
static void mii_reg_write (uint16_t data)
{
    volatile int32_t i;

    /* Data are written one bit at a time */
    for (i = 16; i > 0; i--)
    {
        if ((data & 0x8000) == 0)
        {
            mii_write_0();
        }
        else
        {
            mii_write_1();
        }

        data = (uint16_t)(data << 1);
    }
}
#endif
/******************************************************************************
* ID            : �|
* Outline       : Execute one clock
* Function Name : mii_z
* Description   : Reading is selected as the direction of access to the PHY module.
*               :
* Argument      : none
*               : 
* Return Value  : none
*               : 
******************************************************************************/
static void mii_z (void)
{
    volatile int32_t j;

    for (j = MDC_WAIT; j > 0; j--)
    {
        ETHER.PIR0 = 0x00000000;
    }

    for (j = MDC_WAIT; j > 0; j--)
    {
        ETHER.PIR0 = 0x00000001;
    }

    for (j = MDC_WAIT; j > 0; j--)
    {
        ETHER.PIR0 = 0x00000001;
    }

    for (j = MDC_WAIT; j > 0; j--)
    {
        ETHER.PIR0 = 0x00000000;
    }
}

#if 0
/******************************************************************************
* ID            : �|
* Outline       : Outputs 1 and 0 
* Function Name : mii_ta10
* Description   : Outputs 1 and 0 to the MII management interface of the PHY module.
*               :
* Argument      : none
*               : 
* Return Value  : none
*               : 
******************************************************************************/
static void mii_ta10 (void)
{
    mii_write_1();
    mii_write_0();
}
#endif
/******************************************************************************
* ID            : �|
* Outline       : Output 1 
* Function Name : mii_write_1
* Description   : 1 is output to the MII management interface of the PHY module.
*               :
* Argument      : none
*               : 
* Return Value  : none
*               : 
******************************************************************************/
static void mii_write_1 (void)
{
    volatile int32_t j;

    for (j = MDC_WAIT; j > 0; j--)
    {
        ETHER.PIR0 = 0x00000006;
    }

    for (j = MDC_WAIT; j > 0; j--)
    {
        ETHER.PIR0 = 0x00000007;
    }

    for (j = MDC_WAIT; j > 0; j--)
    {
        ETHER.PIR0 = 0x00000007;
    }

    for (j = MDC_WAIT; j > 0; j--)
    {
        ETHER.PIR0 = 0x00000006;
    }
}

/******************************************************************************
* ID            : �|
* Outline       : Output 0 
* Function Name : mii_write_0
* Description   : 0 is output to the MII management interface of the PHY module.
* Argument      : none
* Return Value  : none
******************************************************************************/
static void mii_write_0 (void)
{
    volatile int32_t j;

    for (j = MDC_WAIT; j > 0; j--)
    {
        ETHER.PIR0 = 0x00000002;
    }

    for (j = MDC_WAIT; j > 0; j--)
    {
        ETHER.PIR0 = 0x00000003;
    }

    for (j = MDC_WAIT; j > 0; j--)
    {
        ETHER.PIR0 = 0x00000003;
    }

    for (j = MDC_WAIT; j > 0; j--)
    {
        ETHER.PIR0 = 0x00000002;
    }
}

/* End of File */
