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
 * and to discontinue the availability of this software. By using this
 * software, you agree to the additional terms and conditions found by
 * accessing the following link:
 * http://www.renesas.com/disclaimer
*******************************************************************************
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
 *****************************************************************************/
/******************************************************************************
 * @headerfile     r_phy.h
 * @brief          ETHER driver header
 * @version        1.00
 * @date           27.06.2018
 * H/W Platform    RZA1H
 *****************************************************************************/
 /*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 22.10.2014 1.00 First Release
 *****************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef _R_PHY_H_
#define _R_PHY_H_

/**************************************************************************//**
 * @ingroup R_SW_PKG_93_ETHER_DRV
 * @{
 *****************************************************************************/
/******************************************************************************
Includes <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#define NEGO_FAIL           (0)
#define NO_LINK             (1)
#define HALF_10M            (2)
#define FULL_10M            (3)
#define HALF_TX             (4)
#define FULL_TX             (5)

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/
/**
 * @brief        The result of automatic negotiation is read out by using the MII
 *               management interface, and indicated in the return value.
 * 
 * @retval       4(FULL_TX):   100 Mbps in full-duplex mode
 * @retval       3(HALF_TX):   100 Mbps in half-duplex mode
 * @retval       2(FULL_10M):  10 Mbps in full-duplex mode
 * @retval       1(HALF_10M):  10 Mbps in half-duplex mode
 * @retval       0(NEGO_FAIL): Failure in negotiation
*/
int32_t phy_autonego(void);

/**
 * @brief        PHY Link Status Check function
 * 
 * @retval       4(FULL_TX):   100 Mbps in full-duplex mode
 * @retval       3(HALF_TX):   100 Mbps in half-duplex mode
 * @retval       2(FULL_10M):  10 Mbps in full-duplex mode
 * @retval       1(HALF_10M):  10 Mbps in half-duplex mode
 * @retval       0(NEGO_FAIL): Failure in negotiation
*/
int32_t phy_linkcheck(void);

#endif  /* _R_PHY_H_ */
/**************************************************************************//**
 * @} (end addtogroup)
 *****************************************************************************/
/* End of File */
