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
 * @headerfile     sound_if.h
 * @brief          Sound Driver API header
 * @version        1.00
 * @date           27.06.2018
 * H/W Platform    RZA1H
 *****************************************************************************/
 /*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 30.06.2018 1.00 First Release
 *****************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef SOUND_IF_H
#define SOUND_IF_H

/**************************************************************************//**
 * @ingroup R_SW_PKG_93_NONOS_MIDDLEWARE
 * @defgroup R_SW_PKG_93_SOUND_API Sound
 * @brief Sound module responsible for setting up the Audio Codec
 *
 * @anchor R_SW_PKG_93_SOUND_API_SUMMARY
 * @par Summary
 *
 * This middleware module is responsible for configuring the Audio Codec, 
 * using the I2C driver to communicate with the MAX9856 Registers. 
 *
 * @anchor R_SW_PKG_93_SOUND_API_INSTANCES
 * @par Known Implementations:
 * This driver is used in the RZA1H Software Package.
 * @see RENESAS_APPLICATION_SOFTWARE_PACKAGE
 *
 * @see RENESAS_OS_ABSTRACTION  Renesas OS Abstraction interface
 * @see SSGSTARC_56_RIIC_API 
 * @{
 *****************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "r_errno.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/******************************************************************************
Macro definitions
******************************************************************************/
#define SOUND_VOL_MUTE          (0u)     /*!< Mute Volume Level */
#define SOUND_VOL_MAX           (100u)   /*!< Max Volume Level */

#define SOUND_FREQ_8000         (8000u)  /*!< 8KHz Sampling Frequency */
#define SOUND_FREQ_11025        (11025u) /*!< 11KHz Sampling Frequency */
#define SOUND_FREQ_12000        (12000u) /*!< 12KHz Sampling Frequency */
#define SOUND_FREQ_16000        (16000u) /*!< 16KHz Sampling Frequency */
#define SOUND_FREQ_22050        (22050u) /*!< 22KHz Sampling Frequency */
#define SOUND_FREQ_24000        (24000u) /*!< 24KHz Sampling Frequency */
#define SOUND_FREQ_32000        (32000u) /*!< 32KHz Sampling Frequency */
#define SOUND_FREQ_44100        (44100u) /*!< 44KHz Sampling Frequency */
#define SOUND_FREQ_48000        (48000u) /*!< 48KHz Sampling Frequency */
#define SOUND_FREQ_88200        (88200u) /*!< 88KHz Sampling Frequency */
#define SOUND_FREQ_96000        (96000u) /*!< 96KHz Sampling Frequency */

/** Microphone volume percentages */
#define SOUND_MIC_VOL_00_PERCENT    (0u)   /*!< Microphone volume 00% */
#define SOUND_MIC_VOL_10_PERCENT    (10u)  /*!< Microphone volume 10% */
#define SOUND_MIC_VOL_20_PERCENT    (20u)  /*!< Microphone volume 20% */
#define SOUND_MIC_VOL_30_PERCENT    (30u)  /*!< Microphone volume 30% */
#define SOUND_MIC_VOL_40_PERCENT    (40u)  /*!< Microphone volume 40% */
#define SOUND_MIC_VOL_50_PERCENT    (50u)  /*!< Microphone volume 50% */
#define SOUND_MIC_VOL_60_PERCENT    (60u)  /*!< Microphone volume 60% */
#define SOUND_MIC_VOL_70_PERCENT    (70u)  /*!< Microphone volume 70% */
#define SOUND_MIC_VOL_80_PERCENT    (80u)  /*!< Microphone volume 80% */
#define SOUND_MIC_VOL_90_PERCENT    (90u)  /*!< Microphone volume 90% */
#define SOUND_MIC_VOL_100_PERCENT   (100u) /*!< Microphone volume 100% */

#define SOUND_FMT_I2S           (0u) /*!< I2S Sample Format*/
#define SOUND_FMT_RIGHT         (1u) /*!< Right Sample Format */ 
#define SOUND_FMT_LEFT          (2u) /*!< Left Sample Format */

#define SOUND_DWORD_16          (16u) /*!< 16-bit Data Word Size*/ 
#define SOUND_DWORD_18          (18u) /*!< 18-bit Data Word Size*/
#define SOUND_DWORD_20          (20u) /*!< 20-bit Data Word Size*/
#define SOUND_DWORD_24          (24u) /*!< 24-bit Data Word Size*/
#define SOUND_DWORD_32          (32u) /*!< 32-bit Data Word Size*/

/******************************************************************************
Functions Prototypes
******************************************************************************/

/**
 * @brief       Initialises the sound driver, creating semaphore and 
 *              initialising I2C channel of the DAC.
 * 
 * @retval  DEVDRV_SUCCESS: Successful initialisation
 * @retval  DEVDRV_ERROR:   Failed initialisation
 */
extern int32_t R_SOUND_Init(void);

/**
 * @brief       Uninitialises the sound driver, deleting semaphore and 
 *              closing I2C channel of the DAC.
 * 
 * @retval  DEVDRV_SUCCESS: Successful uninitialisation
 * @retval  DEVDRV_ERROR:   Failed uninitialisation
 */
extern int32_t R_SOUND_UnInit(void);

/**
 * @brief       Establish a connection with Codec.
 *              And setup defaults.
 * 
 * @param[in] chnl:  SSIF Channel 
 * 
 * @retval  DEVDRV_SUCCESS: Successful open
 * @retval  DEVDRV_ERROR:   Failed to open
 */
extern int32_t R_SOUND_Open(const uint32_t chnl);

/**
 * @brief        Close an established connection with Codec.
 *
 * @param[in] chnl: SSIF Channel
 * 
 * @retval  DEVDRV_SUCCESS: Successful open
 * @retval  DEVDRV_ERROR:   Failed to open
 */
extern int32_t R_SOUND_Close(const uint32_t chnl);

/**
 * @brief       Set output volume of Codec.
 * 
 * @param[in]  chnl:  SSIF Channel #
 * @param[in]  vol:   volume(0 - 100)
 * 
 * @retval        DEVDRV_SUCCESS:  Success.
 * @retval        error code:      Failure.
 */
extern int32_t R_SOUND_SetVolume(const uint32_t chnl, const uint32_t vol);

/**
 * @brief       Set input volume of microphone.
 * 
 * @param[in]   chnl:   SSIF Channel #
 * @param[in]   vol:    volume 0(min) - 100(max)
 * 
 * @retval      DEVDRV_SUCCESS:  Success.
 * @retval      error code:      Failure.
 */
extern int32_t R_SOUND_SetMicVolume(const uint32_t chnl, const uint32_t vol);

/**
 * @brief      Set Sampling rate of the Codec.
 * 
 * @param[in]     chnl:     SSIF Channel #
 * @param[in]     freq:     sampling rate(8k - 96k)
 *
 * @retval        DEVDRV_SUCCESS:  Success.
 * @retval        error code:      Failure.
 */
extern int32_t R_SOUND_SetSamplingRate(const uint32_t chnl, const uint32_t freq);

/**
 * @brief         Set Communication format of the SSIF connection to the Codec.
 * 
 * @param[in]     chnl:   SSIF Channel #
 * @param[in]     fmt:    format (I2S, LEFT, RIGHT)
 * @param[in]     dwdsz:  dataword size (16bit, 18bit, 20bit, 24bit, 32bit)
 * 
 * @retval        DEVDRV_SUCCESS:  Success.
 * @retval        error code:      Failure.
 */
extern int32_t R_SOUND_SetFormat(const uint32_t chnl, const uint32_t fmt, const uint32_t dwdsz);

/**
 * @brief         Get SOUND driver version.
 * 
 * @retval        driver version -
 *                    upper 8bit=MAJOR
 *                    lower 8bit=MINOR
 */
extern uint16_t R_SOUND_GetVersion(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SOUND_IF_H */
/**************************************************************************//**
 * @} (end addtogroup)
 *****************************************************************************/
