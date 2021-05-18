/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Technology Corp. and is only 
* intended for use with Renesas products. No other uses are authorized.

* This software is owned by Renesas Technology Corp. and is protected under 
* all applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, 
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
* PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY 
* DISCLAIMED.

* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* TECHNOLOGY CORP. NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES 
* FOR ANY REASON RELATED TO THE THIS SOFTWARE, EVEN IF RENESAS OR ITS 
* AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

* Renesas reserves the right, without notice, to make changes to this 
* software and to discontinue the availability of this software.  
* By using this software, you agree to the additional terms and 
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
******************************************************************************
* Copyright (C) 2008. Renesas Technology Corp., All Rights Reserved.
******************************************************************************* 
* File Name    : phy.c
* Version      : 1.01
* Description  : Ethernet PHY device driver
******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 15.02.2010 1.00    First Release
*         : 06.04.2010 1.01    RX62N changes 
*         : 08.07.2011 2.00    Second release 
******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
//#include "tx_api.h"
#include "phy.h"
#include "iodefine.h"

/******************************************************************************
Macro definitions
******************************************************************************/
/* Media Independent Interface */
#define PHY_MII_ST                      (1)
#define PHY_MII_READ                    (2)
#define PHY_MII_WRITE                   (1)
#define PHY_MII_ADDR                    (0)
#define PHY_MII_WAIT                    (8)

/* Standard PHY Registers */
#define PHY_REG_CONTROL                 (0)       
#define PHY_REG_STATUS                  (1)       
#define PHY_REG_IDENTIFIER1             (2)       
#define PHY_REG_IDENTIFIER2             (3)       
#define PHY_REG_AN_ADVERTISEMENT        (4)       
#define PHY_REG_AN_LINK_PARTNER         (5)       
#define PHY_REG_AN_EXPANSION            (6)

#define PHY_REG_SPECIAL_MODES           (18)

/* Basic Mode Control Register Bit Definitions */
#define PHY_CONTROL_RESET               (1 << 15)
#define PHY_CONTROL_LOOPBACK            (1 << 14)
#define PHY_CONTROL_100_MBPS            (1 << 13)
#define PHY_CONTROL_AN_ENABLE           (1 << 12)
#define PHY_CONTROL_POWER_DOWN          (1 << 11)
#define PHY_CONTROL_ISOLATE             (1 << 10)
#define PHY_CONTROL_AN_RESTART          (1 << 9)
#define PHY_CONTROL_FULL_DUPLEX         (1 << 8)
#define PHY_CONTROL_COLLISION           (1 << 7)

/* Basic Mode Status Register Bit Definitions */
#define PHY_STATUS_100_T4               (1 << 15)
#define PHY_STATUS_100F                 (1 << 14)
#define PHY_STATUS_100H                 (1 << 13)
#define PHY_STATUS_10F                  (1 << 12)
#define PHY_STATUS_10H                  (1 << 11)
#define PHY_STATUS_AN_COMPLETE          (1 << 5)
#define PHY_STATUS_RM_FAULT             (1 << 4)
#define PHY_STATUS_AN_ABILITY           (1 << 3)
#define PHY_STATUS_LINK_UP              (1 << 2)
#define PHY_STATUS_JABBER               (1 << 1)
#define PHY_STATUS_EX_CAPABILITY        (1 << 0)

/* Auto Negotiation Advertisement Bit Definitions */
#define PHY_AN_ADVERTISEMENT_NEXT_PAGE  (1 << 15)  
#define PHY_AN_ADVERTISEMENT_RM_FAULT   (1 << 13)
#define PHY_AN_ADVERTISEMENT_ASM_DIR    (1 << 11)
#define PHY_AN_ADVERTISEMENT_PAUSE      (1 << 10)
#define PHY_AN_ADVERTISEMENT_100_T4     (1 << 9)
#define PHY_AN_ADVERTISEMENT_100F       (1 << 8)
#define PHY_AN_ADVERTISEMENT_100H       (1 << 7)
#define PHY_AN_ADVERTISEMENT_10F        (1 << 6)
#define PHY_AN_ADVERTISEMENT_10H        (1 << 5)
#define PHY_AN_ADVERTISEMENT_SELECTOR   (1 << 0)

/* Auto Negostiate Link Partner Ability Bit Definitions */
#define PHY_AN_LINK_PARTNER_NEXT_PAGE   (1 << 15)
#define PHY_AN_LINK_PARTNER_ACK         (1 << 14)
#define PHY_AN_LINK_PARTNER_RM_FAULT    (1 << 13)
#define PHY_AN_LINK_PARTNER_PAUSE       (1 << 10) 
#define PHY_AN_LINK_PARTNER_100_T4      (1 << 9) 
#define PHY_AN_LINK_PARTNER_100F        (1 << 8)
#define PHY_AN_LINK_PARTNER_100H        (1 << 7)
#define PHY_AN_LINK_PARTNER_10F         (1 << 6)
#define PHY_AN_LINK_PARTNER_10H         (1 << 5)
#define PHY_AN_LINK_PARTNER_SELECTOR    (1 << 0)

/* Delay constants */
#define PHY_DELAY_RESET                 0x00020000L 
#define PHY_DELAY_AN                    0x00800000L 

#define LOOP_100us                      (6700)     /* Loop counter for software wait */
                                                   /*  6666=100us/((1/400MHz)*6cyc) */

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
uint16_t  _Phy_Read(uint16_t reg_addr);
void  _Phy_Write(uint16_t reg_addr, uint16_t data);
void  _Phy_Preamble(void);
void  _Phy_RegSet(uint16_t reg_addr, int32_t option);
void  _Phy_RegRead(uint16_t *data);
void  _Phy_RegWrite(uint16_t data);
void  _Phy_TransZto0(void);
void  _Phy_Trans1to0(void);
void  _Phy_MiiWrite1(void);
void  _Phy_MiiWrite0(void);


void  _Phy_PhyWrite(uint16_t phy_addr, uint16_t reg_addr, uint16_t data);
void  _Phy_PhyRegSet(uint16_t phy_addr, uint16_t reg_addr, int32_t option);
uint16_t status, an;

/**
 * Public functions
 */

/******************************************************************************
* Function Name: Phy_Init
* Description  : Resets Ethernet PHY device
* Arguments    : none
* Return Value : none
******************************************************************************/
int16_t  Phy_Init(void)
{
    uint16_t reg;
    uint32_t count;
    
    /* Reset PHY */
    _Phy_Write(PHY_REG_CONTROL, PHY_CONTROL_RESET);
    
    /* Write 0x00E0 to Special Modes register - Renesas advice to allow chip to 
     * properly autonegotiate. */
    _Phy_Write(PHY_REG_SPECIAL_MODES, 0x00E0);
    
    count = 0;

    do
    {
        reg = _Phy_Read(PHY_REG_CONTROL);
        count++;
    } while ( (reg & PHY_CONTROL_RESET) && (count < PHY_DELAY_RESET) );

    if( count < PHY_DELAY_RESET )
    {
        //Config GPIO for LEDs.
        _Phy_PhyWrite(7, 0, 0xF887);    //Set GPIO0 to LED0
        _Phy_PhyWrite(7, 1, 0xF6FA);    //Set GPIO4 to LED3
        _Phy_PhyWrite(7, 3, 0x08FF);    //Set GPIO14 to LED1
        
        _Phy_PhyWrite(7, 24, 0x0008);
        
        return R_PHY_OK;
    }
  
    return R_PHY_ERROR;
}

/******************************************************************************
* Function Name: Phy_Set100Full
* Description  : Set Ethernet PHY device to 100 Mbps full duplex
* Arguments    : none
* Return Value : none
******************************************************************************/
void Phy_Set100Full(void)
{
    _Phy_Write(PHY_REG_CONTROL, (PHY_CONTROL_100_MBPS | 
                                 PHY_CONTROL_FULL_DUPLEX));
}

/******************************************************************************
* Function Name: Phy_Set10Half
* Description  : Sets Ethernet PHY device to 10 Mbps half duplexR
* Arguments    : none
* Return Value : none
******************************************************************************/
void Phy_Set10Half(void)
{
    _Phy_Write(PHY_REG_CONTROL, 0);
}

/******************************************************************************
* Function Name: Phy_SetAutonegotiate
* Description  : Starts autonegotiate and reports the other side's 
*              : physical capability
* Arguments    : local_advertise - the capabilities of the local link as PHY data
*                *line_speed_duplex - a pointer to the location of both the line
*                                     speed and the duplex
*                *local_pause - a pointer to the location to store the local
*                               pause bits.
*                *partner_pause - a pointer to the location to store the partner
*                                 pause bits.
* Return Value : bit 10 - Pause operation
*              : bit 8 - Full duplex 100 mbps
*              : bit 7 - Half duplex 100 mbps
*              : bit 6 - Full duplex 10 mbps
*              : bit 5 - Half duplex 10 mbps
*              : bit 4:0 - Always set to 00001 (IEEE 802.3)
*              : -1 if error
******************************************************************************/
int16_t Phy_SetAutonegotiate(uint16_t *line_speed_duplex, 
                             uint16_t *local_pause, 
                             uint16_t *partner_pause)
{
    uint16_t reg;
    //uint16_t local_advertise;
    //uint32_t count;
    
    int32_t i;
    volatile int32_t wait_10ms;
    volatile int32_t wait_50ms;
    volatile int32_t cnt_5sec;
    
    *partner_pause = 0;
    *local_pause = 0;

    /* ==== Resets the PHY-LSI ==== */
    _Phy_Write(PHY_REG_CONTROL, PHY_CONTROL_RESET);
    
    wait_50ms = LOOP_100us * 500;
    while (--wait_50ms)
    {
        ;
    }
    //tx_thread_sleep(5);
    
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
        
        //tx_thread_sleep(1);

        /* ==== Confirm completion of automatic negotiation ==== */
        reg = _Phy_Read(PHY_REG_STATUS);

        if (reg & 0x0020)
        {
            /* PHY register 1 :Basic Status *
             * bit 5 :(1) completion of automatic negotiation process *
             *       :(0) non-completion of automatic negotiation process */
        	/* ---- The capability of the partner as determined on completion of automatic negotiation ---- */
            reg = _Phy_Read(PHY_REG_AN_LINK_PARTNER);
            /* PHY register 5 :Auto-Nego Link Partner Ability *
             * bit 8 :(1)100 Mbps/full-duplex mode is possible *
             * bit 7 :(1)100 Mbps/half-duplex mode is possible *
             * bit 6 :(1) 10 Mbps/full-duplex mode is possible *
             * bit 5 :(1) 10 Mbps/half-duplex mode is possible */

            /* Fall through setting line_speed_duplex. */
            if ( reg & PHY_AN_LINK_PARTNER_10H )
            {
                *line_speed_duplex = PHY_LINK_10H;
            }

            if ( reg & PHY_AN_LINK_PARTNER_10F )
            {
                *line_speed_duplex = PHY_LINK_10F;
            }

            if ( reg & PHY_AN_LINK_PARTNER_100H )
            {
                *line_speed_duplex = PHY_LINK_100H;
            }

            if ( reg & PHY_AN_LINK_PARTNER_100F )
            {
                *line_speed_duplex = PHY_LINK_100F;
            }

            return R_PHY_OK;
        }
    }
    
    return R_PHY_ERROR;
    
}

/******************************************************************************
* Function Name: Phy_GetLinkStatus 
* Description : Returns the status of the physical link 
* Arguments : none 
* Return Value : -1 if links is down, 0 otherwise 
******************************************************************************/
int16_t Phy_GetLinkStatus(void)
{
    uint16_t reg;

    reg = _Phy_Read(PHY_REG_STATUS);

    /* When the link isn't up, return error */
    if (!(reg & PHY_STATUS_LINK_UP))
    {
        return R_PHY_ERROR;
    }
    else
    {
        return R_PHY_OK;
    }
}


/**
 * Private functions
 */

/******************************************************************************
* Function Name: _Phy_Read
* Description  : Reads a PHY register
* Arguments    : reg_addr - address of the PHY register
* Return Value : read value
******************************************************************************/
uint16_t _Phy_Read(uint16_t reg_addr)
{
    uint16_t data;

    _Phy_Preamble();
    _Phy_RegSet(reg_addr, PHY_MII_READ);
    _Phy_TransZto0();
    _Phy_RegRead(&data);
    _Phy_TransZto0();

    return(data);
}

/******************************************************************************
* Function Name: _Phy_Write
* Description  : Writes to a PHY register
* Arguments    : reg_addr - address of the PHY register
*              : data - value
* Return Value : none
******************************************************************************/
void  _Phy_Write(uint16_t reg_addr, uint16_t data)
{
    _Phy_Preamble();
    _Phy_RegSet(reg_addr, PHY_MII_WRITE);
    _Phy_Trans1to0();
    _Phy_RegWrite(data);
    _Phy_TransZto0();
}

/******************************************************************************
* Function Name: _Phy_Preamble
* Description  : As preliminary preparation for access to the PHY module register,
*                "1" is output via the MII management interface.                  
* Arguments    : none
* Return Value : none
******************************************************************************/
void  _Phy_Preamble(void)
{
    int16_t i;

    i = 32;
    while( i > 0 )
    {
        _Phy_MiiWrite1();
        i--;
    }
}

/******************************************************************************
* Function Name: _Phy_RegSet
* Description  : Sets a PHY device to read or write mode
* Arguments    : reg_addr - address of the PHY register
*              : option - mode
* Return Value : none
******************************************************************************/
void  _Phy_RegSet(uint16_t reg_addr, int32_t option)
{
    int32_t    i;
    uint16_t data;

    data = 0;
    data = (PHY_MII_ST << 14);        /* ST code    */

    if( option == PHY_MII_READ )
    {
        data |= (PHY_MII_READ << 12);  /* OP code(RD)  */
    }
    else
    {
        data |= (PHY_MII_WRITE << 12);  /* OP code(WT)  */
    }

    data |= (PHY_MII_ADDR << 7);    /* PHY Address  */
    data |= (reg_addr << 2);    /* Reg Address  */

    i = 14;
    while( i > 0 )
    {
        if( (data & 0x8000) == 0 )
        {
            _Phy_MiiWrite0();
        }
        else
        {
            _Phy_MiiWrite1();
        }
        data <<= 1;
        i--;
    }
}

/******************************************************************************
* Function Name: _Phy_RegRead
* Description  : Reads PHY register through MII interface
* Arguments    : data - pointer to store the data read
* Return Value : none
******************************************************************************/
void  _Phy_RegRead(uint16_t *data)
{
    int32_t      i, j;
    uint16_t   reg_data;

    reg_data = 0;
    i = 16;
    while( i > 0 )
    {
        for(j = PHY_MII_WAIT; j > 0; j--)
        {
            ETHER.PIR0 = 0x00000000;
        }
        for(j = PHY_MII_WAIT; j > 0; j--)
        {
            ETHER.PIR0 = 0x00000001;
        }

        reg_data <<= 1;
        reg_data |= (uint16_t)((ETHER.PIR0 & 0x00000008) >> 3);  /* MDI read  */

        for(j = PHY_MII_WAIT; j > 0; j--)
        {
            ETHER.PIR0 = 0x00000001;
        }
        for(j = PHY_MII_WAIT; j > 0; j--)
        {
            ETHER.PIR0 = 0x00000000;
        }
        i--;
    }
    *data = reg_data;
}

/******************************************************************************
* Function Name: _Phy_RegWrite
* Description  : Writes to PHY register through MII interface
* Arguments    : data - value to write
* Return Value : none
******************************************************************************/
void  _Phy_RegWrite(uint16_t data)
{
    int32_t  i;

    i = 16;
    while( i > 0 )
    {
        if( (data & 0x8000) == 0 )
        {
            _Phy_MiiWrite0();
        }
        else
        {
            _Phy_MiiWrite1();
        }
        i--;
        data <<= 1;
    }
}

/******************************************************************************
* Function Name: _Phy_TransZto0
* Description  : Performs bus release so that PHY can drive data
*              : for read operation 
* Arguments    : none
* Return Value : none
******************************************************************************/
void  _Phy_TransZto0(void)
{
    int32_t j;

    for(j = PHY_MII_WAIT; j > 0; j--)
    {
        ETHER.PIR0 = 0x00000000;
    }
    for(j = PHY_MII_WAIT; j > 0; j--)
    {
        ETHER.PIR0 = 0x00000001;
    }
    for(j = PHY_MII_WAIT; j > 0; j--)
    {
        ETHER.PIR0 = 0x00000001;
    }
    for(j = PHY_MII_WAIT; j > 0; j--)
    {
        ETHER.PIR0 = 0x00000000;
    }
}

/******************************************************************************
* Function Name: _Phy_Trans1to0
* Description  : Switches data bus so MII interface can drive data
*              : for write operation 
* Arguments    : none
* Return Value : none
******************************************************************************/
void _Phy_Trans1to0(void)
{
    _Phy_MiiWrite1();
    _Phy_MiiWrite0();
}

/******************************************************************************
* Function Name: _Phy_MiiWrite1
* Description  : Outputs 1 to the MII interface 
* Arguments    : none
* Return Value : none
******************************************************************************/
void  _Phy_MiiWrite1(void)
{
    int32_t j;

    for(j = PHY_MII_WAIT; j > 0; j--)
    {
        ETHER.PIR0 = 0x00000006;
    }
    for(j = PHY_MII_WAIT; j > 0; j--)
    {
        ETHER.PIR0 = 0x00000007;
    }
    for(j = PHY_MII_WAIT; j > 0; j--)
    {
        ETHER.PIR0 = 0x00000007;
    }
    for(j = PHY_MII_WAIT; j > 0; j--)
    {
        ETHER.PIR0 = 0x00000006;
    }
}

/******************************************************************************
* Function Name: _Phy_MiiWrite0
* Description  : Outputs 0 to the MII interface 
* Arguments    : none
* Return Value : none
******************************************************************************/
void  _Phy_MiiWrite0(void)
{
    int32_t j;

    for(j = PHY_MII_WAIT; j > 0; j--)
    {
        ETHER.PIR0 = 0x00000002;
    }
    for(j = PHY_MII_WAIT; j > 0; j--)
    {
        ETHER.PIR0 = 0x00000003;
    }
    for(j = PHY_MII_WAIT; j > 0; j--)
    {
        ETHER.PIR0 = 0x00000003;
    }
    for(j = PHY_MII_WAIT; j > 0; j--)
    {
        ETHER.PIR0 = 0x00000002;
    }
}

/******************************************************************************
* Function Name: _Phy_Write
* Description  : Writes to a PHY register
* Arguments    : phy_addr - address of the PHY
*              : reg_addr - address of the PHY register
*              : data - value
* Return Value : none
******************************************************************************/
void  _Phy_PhyWrite(uint16_t phy_addr, uint16_t reg_addr, uint16_t data)
{
    _Phy_Preamble();
    _Phy_PhyRegSet(phy_addr, reg_addr, PHY_MII_WRITE);
    _Phy_Trans1to0();
    _Phy_RegWrite(data);
    _Phy_TransZto0();
}

/******************************************************************************
* Function Name: _Phy_PhyRegSet
* Description  : Sets a PHY device to read or write mode
* Arguments    : phy_addr - address of the PHY
*              : reg_addr - address of the PHY register
*              : option - mode
* Return Value : none
******************************************************************************/
void  _Phy_PhyRegSet(uint16_t phy_addr, uint16_t reg_addr, int32_t option)
{
    int32_t    i;
    uint16_t data;

    data = 0;
    data = (PHY_MII_ST << 14);        /* ST code    */

    if( option == PHY_MII_READ )
    {
        data |= (PHY_MII_READ << 12);  /* OP code(RD)  */
    }
    else
    {
        data |= (PHY_MII_WRITE << 12);  /* OP code(WT)  */
    }

    data |= (phy_addr << 7);    /* PHY Address  */
    data |= (reg_addr << 2);    /* Reg Address  */

    i = 14;
    while( i > 0 )
    {
        if( (data & 0x8000) == 0 )
        {
            _Phy_MiiWrite0();
        }
        else
        {
            _Phy_MiiWrite1();
        }
        data <<= 1;
        i--;
    }
}
