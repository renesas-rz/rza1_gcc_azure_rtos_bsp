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
 * Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
 *****************************************************************************/
/******************************************************************************
 * @headerfile     r_rskrza1h_riic_lld.h
 * @brief          RIIC low level driver api header. Low level driver is
 *                 expected to implement these functions.
 * @version        2.00
 * @date           24.04.2019
 * H/W Platform    Platform independent
 *****************************************************************************/
/*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 30.06.2018 1.00 First Release
 *              : 24.04.2019 2.00 Porting for RZ/A1
 *****************************************************************************/

/* Multiple inclusion prevention macro */
#ifndef RENESAS_DRIVERS_R_RIIC_INC_R_RIIC_LLD_API_H_
    #define RENESAS_DRIVERS_R_RIIC_INC_R_RIIC_LLD_API_H_

 /*************************************************************************//**
 * @ingroup SSGSTARC_56_RIIC_API
 * @defgroup SSGSTARC_56_RIIC_LLD_API RIIC Low Level Driver API
 * @brief The API for the low level RIIC driver
 *
 * @anchor SSGSTARC_56_RIIC_LLD_API_SUMMARY
 * @par Summary
 * @brief This interface defines the functionality that the low level driver
 * must implement for compatibility with the high level driver.
 *
 * @anchor SSGSTARC_56_RIIC_LLD_API_LIMITATIONS
 * @par Known Limitations
 * @ref NONE_YET
 *
 * @anchor SSGSTARC_56_RIIC_LLD_API_INSTANCES
 * @par Known Implementations
 * @ref RENESAS_APPLICATION_SOFTWARE_PACKAGE
 * @{
 *****************************************************************************/


/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
    #include "control.h"
    #include "r_riic_drv_sc_cfg.h"

    #include "mcu_board_select.h"

/*******************************************************************************
 Macro definitions
 *******************************************************************************/
/** Driver name */
    #define STDIO_RIIC_RZ_LLD_DRV_NAME ("LLD RIIC")

/** Major Version Number of API. */
    #define STDIO_RIIC_RZ_LLD_VERSION_MAJOR      (0)
/** Minor Version Number of API. */
    #define STDIO_RIIC_RZ_LLD_VERSION_MINOR      (4)
/** Minor Version Number of API. */
    #define STDIO_RIIC_RZ_LLD_BUILD_NUM          (6027)
/** Unique ID. */
    #define STDIO_RIIC_RZ_LLD_UID                (77)

/** Specifies what channels are supported by this low level driver.
 * It is used to inform the high level driver the supported channels */
#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
    #define RIIC_LLD_SUPPORTED_CHANNELS    ((e_channel_id_t) (R_CH0 | R_CH1))
#elif (TARGET_BOARD == TARGET_BOARD_RSK)
    #define RIIC_LLD_SUPPORTED_CHANNELS    ((e_channel_id_t) (R_CH0 | R_CH3))
#endif /* TARGET_BOARD */

/** Specifies the total number of channels supported by this low level driver */
    #define RIIC_LLD_NUM_CHANNELS           (2)

/******************************************************************************
 Functions Prototypes
 ******************************************************************************/
/* ==== api for low level i2c driver ==== */

/**
 * @brief initialise the specified i2c channel
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 * @return    DEVDRV_SUCCESS       : Success of RIIC operation
 *            DEVDRV_ERROR         : Failure of RIIC operation
 **/
int_t R_RIIC_InitChannel (int_t channel, e_clk_frequency_riic_t frequency);

/**
 * @brief Close the specified i2c channel
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 **/
void R_RIIC_CloseChannel (int_t channel);

/**
 * @brief Make the request to issue a stop condition.
 *        Perform the following sequence of actions
 *        Clear Receive data full flag
 *        Requests to issue a stop condition
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 **/
void R_RIIC_TransmitStop (int_t channel);

/**
 * @brief Make the request to issue a start condition.
 *        Perform the following sequence of actions
 *        Clear Receive data full flag
 *        Requests to issue a start condition
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 **/
void R_RIIC_TransmitStart (int_t channel);

/**
 * @brief Make the request to issue a restart condition.
 *        Perform the following sequence of actions
 *        Clear Receive data full flag
 *        Requests to issue a restart condition
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 **/
void R_RIIC_TransmitRestart (int_t channel);

/**
 * @brief Clear NACK detection flag of selected channel
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 **/
void R_RIIC_ClearNack (int_t channel);

/**
 * @brief Determine ACK status of selected channel
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 * @return    ACK status : 0 - ACK
 *                       : 1 - NACK
 **/
uint8_t R_RIIC_GetAckStatus (int_t channel);

/**
 * @brief Set ACK flag of selected channel
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 **/
void R_RIIC_TransmitAck (int_t channel);

/**
 * @brief Set NACK flag of selected channel
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 **/
void R_RIIC_TransmitNack (int_t channel);

/**
 * @brief Assert wait period by holding low during the period between 9th and 1st clock cycles
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 **/
void R_RIIC_AssertLowHold (int_t channel);

/**
 * @brief Release any wait period by holding low during the period between 9th and 1st clock cycles
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 **/
void R_RIIC_ReleaseLowHold (int_t channel);

/**
 * @brief Write a byte on the selected i2c channel
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 * @param[in] byte : byte to send
 **/
void R_RIIC_WriteByte (int_t channel, uint8_t byte);

/**
 * @brief Read a byte on the selected i2c channel
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 * @return    byte read
 **/
uint8_t R_RIIC_ReadByte (int_t channel);

/**
 * @brief Determine if bus is busy
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 * @return    bus status 1: busy
 *                       0: not busy
 **/
uint8_t R_RIIC_IsBusBusy (int_t channel);

/**
 * @brief Determine if stop is asserted on the selected i2c channel
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 * @return          0 : stop not asserted
 *                  1 : stop asserted
 **/
uint8_t R_RIIC_IsStopAsserted (int_t channel);

/**
 * @brief Return version of low level driver
 * @param[in] pinfo - pointer to detailed driver information structure
 * @return    DRV_SUCCESS
 **/
int_t R_RIIC_GetVersion (st_ver_info_t *pinfo);

#endif  /* RENESAS_DRIVERS_R_RIIC_INC_R_RIIC_LLD_API_H_ */
/**************************************************************************//**
 * @} (end addtogroup)
 *****************************************************************************/
/* End of File */

