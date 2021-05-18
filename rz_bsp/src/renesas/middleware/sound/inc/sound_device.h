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
 * @headerfile     sound_device.h
 * @brief          sound driver functions header
 * @version        1.00
 * @date           27.06.2018
 * H/W Platform    RZA1H
 *****************************************************************************/
 /*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 30.06.2018 1.00 First Release
 *****************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef SOUND_DEVICE_H
#define SOUND_DEVICE_H

/**************************************************************************//**
 * @ingroup R_SW_PKG_93_SOUND_API Sound
 * @defgroup R_SW_PKG_93_SOUND_DEVICE Sound Device
 * @brief MAX9856 Audio Codec Control
 * 
 * @anchor R_SW_PKG_93_SOUND_DEVICE_API_SUMMARY
 * @par Summary
 *
 * This module contains all the Audio Codec specific configurations
 * which are specific to the implementation  of this codec on the 
 * RZA1H!
 *
 * @anchor R_SW_PKG_93_SOUND_DEVICE_API_INSTANCES
 * @par Known Implementations:
 * This driver is used in the RZA1H Software Package.
 * @see RENESAS_APPLICATION_SOFTWARE_PACKAGE
 *
 * @see RENESAS_OS_ABSTRACTION  Renesas OS Abstraction interface
 * @see TODO: Add I2C 
 * @{
 *****************************************************************************/
/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Functions Prototypes
******************************************************************************/

/**
 * @brief       Configures the default settings for the Audio Codec, 
 *              setting MAX9856 registers up so that it can perform
 *              recording and playback.          
 * 
 * @retval      DEVDRV_SUCCESS: Success
 * @retval      error code:     Failure
 */
int_t sound_max9856_open(void);

/**
 * @brief       Sets the MAX9856 into shutdown mode disabling 
 *              operation. Closes access.
 * 
 * @retval      DEVDRV_SUCCESS: Success
 * @retval      error code:     Failure 
 */
int_t sound_max9856_close(void);

/**
 * @brief       Mutes the output volume of speaker connected 
 *              to jack. 
 * 
 * @retval      DEVDRV_SUCCESS: Success
 * @retval      error code:     Failure 
 */
int_t sound_max9856_set_mute(void);

/**
 * @brief       Mutes the input volume of microphone connected 
 *              to jack. 
 * 
 * @retval      DEVDRV_SUCCESS: Success
 * @retval      error code:     Failure 
 */
int_t sound_max9856_set_mic_mute(void);

/**
 * @brief      Sets the stereo output volume of speaker connected. 
 *             Allows user assign different volume levels to Left 
 *             and Right Speakers. 
 * 
 * @param[in]   lvol:  left  volume value 1(min) to 100(max)
 * @param[in]   rvol:  right volume value 1(min) to 100(max)
 * 
 * @retval      DEVDRV_SUCCESS: Success
 * @retval      error code:     Failure 
 */
int_t sound_max9856_set_volume(const int32_t lvol, const int32_t rvol);

/**
 * @brief      Sets the stereo input volume of microphone connected.
 *             Allows user to assign different volume levels to 
 *             Left and Right inputs.
 * 
 * @param[in]   lvol:  left  volume value 1(min) to 100(max)
 * @param[in]   rvol:  right volume value 1(min) to 100(max)
 * 
 * @retval      DEVDRV_SUCCESS: Success
 * @retval      error code:     Failure  
 */
int_t sound_max9856_set_mic_volume(const int32_t lvol, const int32_t rvol);

/**
 * @brief       MAX9856 features are DAC(max96kHz) and ADC(max48kHz).
 *              This board configured ADC and DAC are at the same sample rate.
 *              So maximum samplingrate is 48kHz.
 * 
 * @param[in]   smpfreq:  sampling rate(8k-48k)
 * 
 * @retval      DEVDRV_SUCCESS: Success
 * @retval      error code:     Failure  
 */
int_t sound_max9856_set_sampling_rate(const uint32_t smpfreq);

/**
 * @brief         Set MAX9856 Audio interface format.
 * 
 * @param[in]     fmt:          data format<br>
 *                              SOUND_FMT_I2S(0)<br>
 *                              SOUND_FMT_LEFT(2)
 * @param[in]     bits:         bit width<br>
 *                              SOUND_DWORD_16(16)<br>
 *                              SOUND_DWORD_18(18)
 * @retval        DEVDRV_SUCCESS    : Success
 * @retval        error code  : Failure
 */
int_t sound_max9856_set_audio_format(const uint32_t fmt, const uint32_t bits);
#endif /* SOUND_DEVICE_H */
/**************************************************************************//**
 * @} (end addtogroup)
 *****************************************************************************/
