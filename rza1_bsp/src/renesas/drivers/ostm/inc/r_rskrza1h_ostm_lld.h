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
 * @headerfile     r_rskrza1h_ostm_lld.h
 * @brief          OS timer driver API header
 * @version        1.00
 * @date           27.06.2018
 * H/W Platform    RZ/A1LU
 *****************************************************************************/
 /*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 30.06.2018 1.00 First Release
 *****************************************************************************/

/* Multiple inclusion prevention macro */
#ifndef SRC_RENESAS_DRIVERS_R_OSTM_INC_R_RSKRZA1H_OSTM_LLD_H_
#define SRC_RENESAS_DRIVERS_R_OSTM_INC_R_RSKRZA1H_OSTM_LLD_H_

/****************************************************************************//**
 * @ingroup SSGSTARC_37_OSTM_DRIVER
 * @defgroup RZA1LU_STREAM_IT_OSTM_LLD OSTM low layer driver configuration
 * @brief This interface allows control of the OSTM interface on the RZA1LU
 * Stream-IT board.
 *
 * @anchor RZA1LU_STREAM_IT_OSTM_LLD_SUMMARY
 * @par Summary
 * @brief The RZA1LU low layer driver has direct access to the OSTM peripheral.
 * It is configured by smart configurator.
 *
 * @anchor RZA1LU_STREAM_IT_OSTM_LLD_LIMITATIONS
 * @par Known Limitations
 * @ref NONE_YET
 *
 * @anchor RZA1LU_STREAM_IT_OSTM_LLD_INSTANCES
 * @par Known Implementations
 * @ref NONE_YET
 *
 * @anchor RZA1LU_STREAM_IT_OSTM_LLD_RELATED
 * @par Related modules
 * @ref RENESAS_OS_ABSTRACTION  Renesas OS Abstraction interface
 * @{
 *******************************************************************************/

/* Guard against failure to include driver support interface */
#ifndef DRIVER_CFG_PRV_63_DEFINED_R_OSTM_DRV_API_H_
#error Failure to include driver support interface in "r_sc_cfg.h"
#endif

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

/******************************************************************************
 Typedef definitions
 ******************************************************************************/

/******************************************************************************
 Macro definitions
 ******************************************************************************/

/* Version Number of API. */

/** Driver name */
#define STDIO_OSTM_RZ_LLD_DRV_NAME ("LLD RSKRZA1H.OSTM")

/** Major Version Number of API. */
#define STDIO_OSTM_RZ_LLD_VERSION_MAJOR      (0)
/** Minor Version Number of API. */
#define STDIO_OSTM_RZ_LLD_VERSION_MINOR      (4)
/** Minor Version Number of API. */
#define STDIO_OSTM_RZ_LLD_BUILD_NUM          (6027)
/** Unique ID. */
#define STDIO_OSTM_RZ_LLD_UID                (77)

/******************************************************************************
 Constant Data
 ******************************************************************************/
/** Exposed Driver configuration data structure */

/** ---- P0 clock (Unit : kHz) ---- */
#define CPG_PRV_P0_CLOCK_FREQUENCY_KHZ_CFG_  (33.333 * 1000)     /* Assume 33.333MHz */

/**
 * @var CPG_PRV_MAX_CYCLE_MSEC_CFG_
 *  ---- Timer cycle specification(Argument cycle)(Unit : ms) maximum value ----
 * The maximum value is H'FFFF_FFFF because CMP is the register which has 32 bits
 * 128,850msec * 33.333MHz = 4,294,957,050 (H'FFFF_D7FA)   : No Error
 * 128,851msec * 33.333MHz = 4,294,990,383 (H'1_0000_5A2F) : Error
 * Timer cycle specification > Error when the cycle is 128850ms                   */
#define CPG_PRV_MAX_CYCLE_MSEC_CFG_          (0xFFFFFFFF / CPG_PRV_P0_CLOCK_FREQUENCY_KHZ_CFG_)

/******************************************************************************
 Interface
 ******************************************************************************/

/** OSTM Initialise Hardware Interface Function
 *  @brief Low layer driver initialise interface function
 *  @retval None.
*/
int_t   R_OSTM_InitialiseHwIf(void);

 /** OSTM Un-initialise Hardware Interface Function
  *  @brief Low layer driver disable the hardware interface function
  *  @retval None.
  */
void    R_OSTM_UninitialiseHwIf(void);

/** OSTM Initialise Hardware Interface Function
 *  @brief Low layer driver open interface function
 *  @param[in] res        : OSTM channel (0 or 1 supported for this driver)
 *  @param[in] config_ptr : configuration pointer
 *  @retval DRV_SUCCESS : Success to start counting OSTM
 *          DRV_ERROR   : Failure to start counting OSTM
 */
int_t R_OSTM_Open(uint32_t res, st_r_drv_ostm_config_t *config_ptr);

/** OSTM Initialise Hardware Interface Function
 *  @brief Low layer driver close interface function
 *  @retval None.
 */
int_t   R_OSTM_Close(uint32_t res);

/** OSTM Initialise Hardware Interface Function
 *  @brief Low layer driver start interface function
 *  @retval None.
 */
int_t   R_OSTM_Start(int_t channel);

/** OSTM Initialise Hardware Interface Function
 *  @brief Low layer driver stop interface function
 *  @retval None.
 */
int_t   R_OSTM_Stop(int_t channel);

/** OSTM Initialise Hardware Interface Function
 *  @brief Low layer driver read interface function.
 *  This version of the driver supports reading the CNT register only.
 *
 *  @param[in] channel Selected channel of the OSTM peripheral.
 *  @param[in] val Value of the OSTMnCNT register for the specified channel 'n'
 *  @retval    data from the CNT register
 */
int_t R_OSTM_ReadCnt(int_t channel, uint32_t* val);


/** OSTM Initialise Hardware Interface Function
 *  @brief Low layer driver initialise interface function
 *  @retval None.
 */
int32_t R_OSTM_GetVersion(st_ver_info_t *pinfo);

#endif  /* SRC_RENESAS_DRIVERS_R_OSTM_INC_R_RSKRZA1H_OSTM_LLD_H_*/
/**************************************************************************//**
 * @} (end addtogroup)
 *****************************************************************************/
/* End of File */
