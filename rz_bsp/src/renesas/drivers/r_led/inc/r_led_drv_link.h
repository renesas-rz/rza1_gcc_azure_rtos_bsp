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
 * @headerfile     r_led_drv_link.h
 * @brief          LED Driver link configuration
 * @version        1.00
 * @date           27.06.2018
 * H/W Platform    RZA1 CPU Board
 *****************************************************************************/
 /*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 30.06.2018 1.00 First Release
 *****************************************************************************/

/* Multiple inclusion prevention macro */
#ifndef SRC_RENESAS_DRIVERS_R_LED_INC_R_LED_DRV_LINK_H_
    #define SRC_RENESAS_DRIVERS_R_LED_INC_R_LED_DRV_LINK_H_

 /*************************************************************************//**
 * @ingroup R_SW_PKG_93_POSIX_Drivers
 * @defgroup R_SW_PKG_93_LED_API RZA1 LED driver
 * @brief This interface provides access to the LED on the RZA1H
 * board
 * See @ref LED_HLD_API for details.
 *
 * @anchor R_SW_PKG_93_LED_API_SUMMARY
 * @par Summary
 * @brief The LED driver for the RSK+RZA1H is comprised of the following
 * components
 *
 * @anchor R_SW_PKG_93_LED_API_LIMITATIONS
 * @par Known Limitations
 * @ref NONE_YET
 *
 * @anchor R_SW_PKG_93_LED_API_INSTANCES
 * @par Known Implementations
 * @ref RENESAS_APPLICATION_SOFTWARE_PACKAGE
 * @{
 *****************************************************************************/


 #include "mcu_board_select.h"
 
#include "r_sitrza1_led_lld.h"


/*****************************************************************************
 Macro definitions
 *****************************************************************************/

/*****************************************************************************
 Typedef definitions
 *****************************************************************************/

/*****************************************************************************
 Exported global variables
 *****************************************************************************/

/*****************************************************************************
 Exported global functions (to be accessed by other files)
 *****************************************************************************/

#endif /* SRC_RENESAS_DRIVERS_R_LED_INC_R_LED_DRV_LINK_H_ */

/******************************************************************************
 End  Of File
 ******************************************************************************/

 /*************************************************************************//**
 * @} (end addtogroup RZA1_RSK_LED_LINK)
 *****************************************************************************/

