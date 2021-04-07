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
 * @headerfile     r_sci_drv_link.h
 * @brief          SCI Link high-low level driver include file
 * @version        1.00
 * @date           27.06.2018
 * H/W Platform    RZA1 CPU Board
 *****************************************************************************/
/*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 30.06.2018 1.00 First Release
 *****************************************************************************/

/* Multiple inclusion prevention macro */
#ifndef SRC_RENESAS_DRIVERS_R_SCI_INC_R_SCI_DRV_LINK_H_
    #define SRC_RENESAS_DRIVERS_R_SCI_INC_R_SCI_DRV_LINK_H_

/*************************************************************************//**
 * @ingroup R_SW_PKG_93_POSIX_Drivers
 * @defgroup R_SW_PKG_93_SCIF_API RZA1 SCIF driver
 * @brief This interface provides access to the Serial SCIF peripheral on
 * the RZA1 board.
 * See @ref R_SW_PKG_93_SCIF_HLD_API for details.
 *
 * @anchor R_SW_PKG_93_SCIF_API_LIMITATIONS
 * @par Known Limitations
 * @ref NONE_YET
 *
 * @anchor R_SW_PKG_93_SCIF_API_INSTANCES
 * @par Known Implementations
 * @ref RENESAS_APPLICATION_SOFTWARE_PACKAGE
 *
 * @ref RENESAS_OS_ABSTRACTION  Renesas OS Abstraction interface
 * @{
 *****************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
 #include "mcu_board_select.h"
 
 #if (TARGET_RZA1 <= TARGET_RZA1LU)
     #include "r_rskrza1lu_sci_lld.h"
#else /* TARGET_RZA1H */
	#include "r_rskrza1h_sci_lld.h"
#endif

/******************************************************************************
 Macro definitions
 ******************************************************************************/

/******************************************************************************
 Typedef definitions
 ******************************************************************************/

/******************************************************************************
 Exported global variables
 ******************************************************************************/

#endif /* SRC_RENESAS_DRIVERS_R_SCI_INC_R_SCI_DRV_LINK_H_ */
/**************************************************************************//**
 * @} (end addtogroup)
 *****************************************************************************/
/******************************************************************************
 End  Of File
 ******************************************************************************/

