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
 * File Name    : r_sc_cfg.h
 * Description  : This module should be modified by smart configurator
 **********************************************************************************************************************/


/*******************************************************************************************************************//**
 * @ingroup Interface_Library
 * @defgroup SC_CFG Smart Configurator (SC) interface
 * @brief Configures the Smart Configurator (SC) driver list for the application
 *
 * @anchor R_SW_PKG_93_SC_CFG_SUMMARY
 * @par Summary
 * @brief The smart configurator setup file.
 *
 * @anchor R_SW_PKG_93_SC_CFG_LIMITATIONS
 * @par Known Limitations
 * @ref NONE_YET
 *
 * @anchor R_SW_PKG_93_SC_CFG_INSTANCES
 * @par Known Implementations
 * @ref NONE_YET
 *
 * @anchor R_SW_PKG_93_SC_CFG_RELATED
 * @par Related modules
 * @ref RENESAS_OS_ABSTRACTION  Renesas OS Abstraction interface
 * @{
 **********************************************************************************************************************/

#ifndef SRC_RENESAS_CONFIG_R_SC_CFG_H_
#define SRC_RENESAS_CONFIG_R_SC_CFG_H_

/** SC Configuration File */
#define STDIO_R_SC_CFG_NAME ("SC Configuration File")

/** Major Version Number of API.
 * Updated by product owner */
#define STDIO_R_SC_CFG_VERSION_MAJOR      (0)

/** Minor Version Number of API.
 * Updated by developer */
#define STDIO_R_SC_CFG_VERSION_MINOR      (1)

/* DRIVER R_LED Enable  */
#define DRIVER_CFG_PRV_65_DEFINED_R_LED_DRV_API_H_
/* High Layer Driver Requirement */
#define REQUIRES_CFG_STDIO_LED_RZ_HLD_UID  (65)
/* Low Layer Driver Requirement */
#define REQUIRED_CFG_STDIO_OSTM_RZ_LLD_UID (68)

/* DRIVER R_OSTM */
#define DRIVER_CFG_PRV_63_DEFINED_R_OSTM_DRV_API_H_
/* High Layer Driver Requirement */
#define REQUIRES_CFG_STDIO_OSTM_RZ_HLD_UID (63)


/* DRIVER R_RIIC */
#define DRIVER_CFG_PRV_63_DEFINED_R_RIIC_DRV_API_H_
/* High Layer Driver Requirement */
#define REQUIRES_CFG_STDIO_RIIC_RZ_HLD_UID (63)

#endif /* SRC_RENESAS_CONFIG_R_SC_CFG_H_ */

/*******************************************************************************************************************//**
 * @} (end addtogroup RZA1H_SC_CFG)
 ***********************************************************************************************************************/

/*******************************************************************************************************************//**
End  Of File
 ***********************************************************************************************************************/
