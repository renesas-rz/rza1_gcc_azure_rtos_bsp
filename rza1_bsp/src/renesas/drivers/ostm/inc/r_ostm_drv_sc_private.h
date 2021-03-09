/***********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
 * SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2017 Renesas Electronics Corporation. All rights reserved.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : r_ostm_drv_sc_private.h
 * Version      : 1.0 <- Optional as long as history is shown below
 * Description  : This module solves all the world's problems
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 15.01.2007 1.00     First Release
 **********************************************************************************************************************/

/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup SSGSTARC_37_OSTM_DRIVER
 * @ingroup R_SW_PKG_93_SC_CFG
 * @defgroup OSTM_DRIVER_SC_PRIVATE RZA1LU OSTM private interface to SC
 * @brief Manages the private interface between the driver and SC.
 *
 * @anchor OSTM_DRIVER_SC_PRIVATE_SUMMARY
 * @par Summary
 * @brief The OSTM driver for the RSK+RZA1H is comprised of the following components
 *
 * @anchor OSTM_DRIVER_SC_PRIVATE_LIMITATIONS
 * @par Known Limitations
 * NONE
 *
 * @anchor OSTM_DRIVER_SC_PRIVATE_INSTANCES
 * @par Known Implementations
 * @ref NONE_YET
 *
 * @anchor OSTM_DRIVER_SC_PRIVATE_RELATED
 * @par Related modules
 * @ref RENESAS_OS_ABSTRACTION  Renesas OS Abstraction interface
 * @{
 **********************************************************************************************************************/

#ifndef SRC_RENESAS_DRIVERS_R_OSTM_INC_R_OSTM_DRV_SC_PRIVATE_H_
    #define SRC_RENESAS_DRIVERS_R_OSTM_INC_R_OSTM_DRV_SC_PRIVATE_H_

/***********************************************************************************************************************
 Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 Exported global variables
 **********************************************************************************************************************/

/* private interface to smart configurator data */
extern st_r_drv_ostm_lld_config_t g_r_ostm_sc_config;
extern const st_r_drv_ostm_lld_ch_config_t g_r_ostm_sc_ch_config [];

/* private interface to smart configurator initialisation function */
extern void initialise_ostm_config_from_sc(void);


/***********************************************************************************************************************
 Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/

#endif /* SRC_RENESAS_DRIVERS_R_OSTM_INC_R_OSTM_DRV_SC_PRIVATE_H_ */

/******************************************************************************
 End  Of File
 ******************************************************************************/

/*******************************************************************************************************************//**
 * @} (end addtogroup RZA1H_RSK_OSTM_PRV)
 ***********************************************************************************************************************/
