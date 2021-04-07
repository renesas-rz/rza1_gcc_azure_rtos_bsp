/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************
  Copyright (C) 2008. Renesas Technology Corp., All Rights Reserved.
*******************************************************************************
* File Name    : phy.h
* Version      : 1.02
* Description  : Ethernet PHY device driver
******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 15.02.2010 1.00    First Release
*         : 17.03.2010 1.01    Modification of macro definitions for access timing
*         : 06.04.2010 1.02    RX62N changes 
*         : 08.07.2011 2.00    Second release 
*****************************************************************************/ 

#ifndef PHY_H
#define PHY_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdint.h>

/******************************************************************************
Typedef definitions
******************************************************************************/
enum LinkStatE
{
    PHY_NO_LINK = 0,
    PHY_LINK_10H,
    PHY_LINK_10F,
    PHY_LINK_100H,
    PHY_LINK_100F,
};

typedef enum LinkStatE linkstat_e;

/******************************************************************************
Macro definitions
******************************************************************************/
/* PHY return definitions */
#define R_PHY_OK        (0)
#define R_PHY_ERROR     (-1)

/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Functions Prototypes
******************************************************************************/
int16_t Phy_Init(void);
void    Phy_Set100Full(void);
void    Phy_Set10Half(void);
int16_t Phy_SetAutonegotiate(uint16_t *line_speed_duplex, 
                             uint16_t *local_pause, 
                             uint16_t *partner_pause);
int16_t Phy_GetLinkStatus(void);

#endif /* PHY_H */

