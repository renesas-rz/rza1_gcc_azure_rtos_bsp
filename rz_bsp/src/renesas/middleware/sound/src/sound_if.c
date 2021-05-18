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
 * and to discontinue the availability of this software. By using this software,
 * you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 * Copyright (C) 2014-2017 Renesas Electronics Corporation. All rights reserved.
 *******************************************************************************/
/**************************************************************************//**
 * @file         sound_if.c
 * $Rev: 8475 $
 * $Date:: 2018-06-18 11:21:41 +0100#$
 * @brief        sond driver interface functions
 ******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include "sound.h"
#include "mcu_board_select.h"
#include "r_os_abstraction_api.h"
#include "dev_drv.h"

/******************************************************************************
 Macro definitions
 ******************************************************************************/
#define SOUND_DRV_REQ_TMOUT_PRV_ (500u)

/******************************************************************************
 Typedef definitions
 ******************************************************************************/

typedef enum
{
    SOUND_DRVSTS_UNINIT = 0, SOUND_DRVSTS_INIT
} e_sound_drv_stat_t;

typedef enum
{
    SOUND_CHSTS_UNINIT = 0, SOUND_CHSTS_OPEN
} e_sound_ch_stat_t;

typedef struct sound_drv_ctrl
{
    e_sound_drv_stat_t drv_sts;
    e_sound_ch_stat_t  ch_sts[SOUND_CH_NUM];
    psemaphore_t           semid;
} st_sound_drv_ctrl_t;

/******************************************************************************
 Exported global variables (to be accessed by other files)
 ******************************************************************************/
//osSemaphoreDef(sem_drv);
/******************************************************************************
 Private global variables and functions
 ******************************************************************************/
static st_sound_drv_ctrl_t gs_gb_snd_ctrl =
{ SOUND_DRVSTS_UNINIT, /* drv_sts   */
{ SOUND_CHSTS_UNINIT, /* ch_sts[0] */
SOUND_CHSTS_UNINIT, /* ch_sts[1] */
SOUND_CHSTS_UNINIT, /* ch_sts[2] */
#if (TARGET_RZA1 <= TARGET_RZA1LU)
        SOUND_CHSTS_UNINIT /* ch_sts[3] */
#else /* TARGET_RZA1H */
        SOUND_CHSTS_UNINIT, /* ch_sts[3] */
        SOUND_CHSTS_UNINIT, /* ch_sts[4] */
        SOUND_CHSTS_UNINIT /* ch_sts[5] */
#endif /* (TARGET_RZA1 <= TARGET_RZA1LU) */
    }, 0 /* semid     */
};

static bool_t is_valid_freq (const uint32_t freq);
static bool_t is_valid_fmt (const uint32_t fmt);
static bool_t is_valid_dwd (const uint32_t dwdsz);

/******************************************************************************
 Exported global functions (to be accessed by other files)
 ******************************************************************************/

/**************************************************************************//**
 * Function Name: R_SOUND_Init
 * @brief         SOUND driver initialize.
 *
 *                Description:<br>
 *
 * @param         none
 * @retval        DEVDRV_SUCCESS   :Success.
 * @retval        error code      :Failure.
 ******************************************************************************/
int32_t R_SOUND_Init (void)
{
    int32_t ercd = DEVDRV_SUCCESS;

    if (SOUND_DRVSTS_UNINIT != gs_gb_snd_ctrl.drv_sts)
    {
        ercd = DEVDRV_ERROR;
    }
    else
    {
        R_OS_CreateSemaphore( &gs_gb_snd_ctrl.semid, 1);

        if (0 == &gs_gb_snd_ctrl.semid)
        {
            ercd = DEVDRV_ERROR;
        }

        if (DEVDRV_SUCCESS == ercd)
        {
            ercd = sound_init();
        }

        if (DEVDRV_SUCCESS == ercd)
        {
            gs_gb_snd_ctrl.drv_sts = SOUND_DRVSTS_INIT;
        }
        else
        {
            if (0 != &gs_gb_snd_ctrl.semid)
            {
                R_OS_DeleteSemaphore( gs_gb_snd_ctrl.semid);
            }
        }
    }

    return ercd;
}
/*******************************************************************************
 End of function R_SOUND_Init
 ******************************************************************************/

/**************************************************************************//**
 * Function Name: R_SOUND_UnInit
 * @brief         SOUND driver uninitialize.
 *
 *                Description:<br>
 *
 * @param         none
 * @retval        DEVDRV_SUCCESS   :Success.
 * @retval        error code      :Failure.
 ******************************************************************************/
int32_t R_SOUND_UnInit (void)
{
    int32_t ercd = DEVDRV_SUCCESS;
    bool_t sem_token;
    uint32_t chnl;

    if (SOUND_DRVSTS_UNINIT == gs_gb_snd_ctrl.drv_sts)
    {
        ercd = DEVDRV_ERROR;
    }
    else
    {
        sem_token = R_OS_WaitForSemaphore( gs_gb_snd_ctrl.semid, SOUND_DRV_REQ_TMOUT_PRV_);

        if ( !sem_token)
        {
            ercd = DEVDRV_ERROR;
        }
        else
        {
            for (chnl = 0u; chnl < SOUND_CH_NUM; chnl++)
            {
                if (SOUND_CHSTS_UNINIT != gs_gb_snd_ctrl.ch_sts[chnl])
                {
                    ercd = DEVDRV_ERROR;
                }
            }

            if (DEVDRV_SUCCESS == ercd)
            {
                ercd = sound_un_init();
            }

            if (DEVDRV_SUCCESS == ercd)
            {
                gs_gb_snd_ctrl.drv_sts = SOUND_DRVSTS_UNINIT;
                R_OS_DeleteSemaphore( gs_gb_snd_ctrl.semid);
            }
        }
    }

    return ercd;
}
/*******************************************************************************
 End of function R_SOUND_UnInit
 ******************************************************************************/

/**************************************************************************//**
 * Function Name: R_SOUND_Open
 * @brief         Initialize DAC.
 *
 *                Description:<br>
 *                Establish a connection with DAC.<br>
 *                And setup defaults.
 * @param[in]     chnl         :SSIF Channel #
 * @retval        DEVDRV_SUCCESS   :Success.
 * @retval        error code      :Failure.
 ******************************************************************************/
int32_t R_SOUND_Open (const uint32_t chnl)
{
    int32_t ercd = DEVDRV_SUCCESS;
    bool_t sem_token;

    if (SOUND_DRVSTS_UNINIT == gs_gb_snd_ctrl.drv_sts)
    {
        ercd = DEVDRV_ERROR;
    }
    else
    {
        sem_token = R_OS_WaitForSemaphore( gs_gb_snd_ctrl.semid, SOUND_DRV_REQ_TMOUT_PRV_);

        if ( !sem_token)
        {
            ercd = DEVDRV_ERROR;
        }
        else
        {
            if (SOUND_CH_NUM <= chnl)
            {
                ercd = DEVDRV_ERROR;
            }
            else
            {
                if (SOUND_CHSTS_UNINIT != gs_gb_snd_ctrl.ch_sts[chnl])
                {
                    ercd = DEVDRV_ERROR;
                }

                if (DEVDRV_SUCCESS == ercd)
                {
                    ercd = sound_open(chnl);
                }

                if (DEVDRV_SUCCESS == ercd)
                {
                    gs_gb_snd_ctrl.ch_sts[chnl] = SOUND_CHSTS_OPEN;
                }
            }

            R_OS_ReleaseSemaphore( gs_gb_snd_ctrl.semid);
        }
    }

    return ercd;
}
/*******************************************************************************
 End of function R_SOUND_Open
 ******************************************************************************/

/**************************************************************************//**
 * Function Name: R_SOUND_Close
 * @brief         Uninitialize DAC.
 *
 *                Description:<br>
 *                Set Mute.<br>
 *                And close an established connection with DAC.
 * @param[in]     chnl         :SSIF Channel #
 * @retval        DEVDRV_SUCCESS   :Success.
 * @retval        error code      :Failure.
 ******************************************************************************/
int32_t R_SOUND_Close (const uint32_t chnl)
{
    int32_t ercd = DEVDRV_SUCCESS;
    bool_t sem_token;

    if (SOUND_DRVSTS_UNINIT == gs_gb_snd_ctrl.drv_sts)
    {
        ercd = DEVDRV_ERROR;
    }
    else
    {
        sem_token = R_OS_WaitForSemaphore( gs_gb_snd_ctrl.semid, SOUND_DRV_REQ_TMOUT_PRV_);

        if ( !sem_token)
        {
            ercd = DEVDRV_ERROR;
        }
        else
        {
            if (SOUND_CH_NUM <= chnl)
            {
                ercd = DEVDRV_ERROR;
            }
            else
            {
                if (SOUND_CHSTS_UNINIT == gs_gb_snd_ctrl.ch_sts[chnl])
                {
                    ercd = DEVDRV_ERROR;
                }

                if (DEVDRV_SUCCESS == ercd)
                {
                    ercd = sound_close(chnl);
                }

                if (DEVDRV_SUCCESS == ercd)
                {
                    gs_gb_snd_ctrl.ch_sts[chnl] = SOUND_CHSTS_UNINIT;
                }
            }

            R_OS_ReleaseSemaphore( gs_gb_snd_ctrl.semid);
        }
    }

    return (ercd);
}
/*******************************************************************************
 End of function R_SOUND_Close
 ******************************************************************************/

/**************************************************************************//**
 * Function Name: R_SOUND_SetVolume
 * @brief         Set spk volume to DAC.
 *
 *                Description:<br>
 *                Set volume.
 * @param[in]     chnl         :SSIF Channel #
 * @param[in]     vol        :volume(0 - 100)
 * @retval        DEVDRV_SUCCESS   :Success.
 * @retval        error code      :Failure.
 ******************************************************************************/
int32_t R_SOUND_SetVolume (const uint32_t chnl, const uint32_t vol)
{
    int32_t ercd = DEVDRV_SUCCESS;
    bool_t sem_token;

    if (SOUND_DRVSTS_UNINIT == gs_gb_snd_ctrl.drv_sts)
    {
        ercd = DEVDRV_ERROR;
    }
    else
    {
        sem_token = R_OS_WaitForSemaphore( gs_gb_snd_ctrl.semid, SOUND_DRV_REQ_TMOUT_PRV_);

        if ( !sem_token)
        {
            ercd = DEVDRV_ERROR;
        }
        else
        {
            if ((SOUND_CH_NUM <= chnl) || (SOUND_VOL_MAX < vol))
            {
                ercd = DEVDRV_ERROR;
            }
            else
            {
                if (SOUND_CHSTS_UNINIT == gs_gb_snd_ctrl.ch_sts[chnl])
                {
                    ercd = DEVDRV_ERROR;
                }

                if (DEVDRV_SUCCESS == ercd)
                {
                    ercd = sound_set_volume(chnl, vol);
                }
            }

            R_OS_ReleaseSemaphore( gs_gb_snd_ctrl.semid);
        }
    }

    return (ercd);
}
/*******************************************************************************
 End of function R_SOUND_SetVolume
 ******************************************************************************/

/**************************************************************************//**
 * Function Name: R_SOUND_SetMicVolume
 * @brief         Set mic volume to DAC.
 *
 *                Description:<br>
 *                Set volume.
 * @param[in]     chnl       :SSIF Channel #
 * @param[in]     vol        :volume 0(min) - 100(max)
 * @retval        DEVDRV_SUCCESS   :Success.
 * @retval        error code      :Failure.
 ******************************************************************************/
int32_t R_SOUND_SetMicVolume (const uint32_t chnl, const uint32_t vol)
{
    int32_t ercd = DEVDRV_SUCCESS;
    bool_t sem_token;

    if (SOUND_DRVSTS_UNINIT == gs_gb_snd_ctrl.drv_sts)
    {
        ercd = DEVDRV_ERROR;
    }
    else
    {
        sem_token = R_OS_WaitForSemaphore( gs_gb_snd_ctrl.semid, SOUND_DRV_REQ_TMOUT_PRV_);

        if ( !sem_token)
        {
            ercd = DEVDRV_ERROR;
        }
        else
        {
            if ((SOUND_CH_NUM <= chnl) || (SOUND_VOL_MAX < vol))
            {
                ercd = DEVDRV_ERROR;
            }
            else
            {
                if (SOUND_CHSTS_UNINIT == gs_gb_snd_ctrl.ch_sts[chnl])
                {
                    ercd = DEVDRV_ERROR;
                }

                if (DEVDRV_SUCCESS == ercd)
                {
                    ercd = sound_set_mic_volume(chnl, vol);
                }
            }

            R_OS_ReleaseSemaphore( gs_gb_snd_ctrl.semid);
        }
    }

    return (ercd);
}
/*******************************************************************************
 End of function R_SOUND_SetMicVolume
 ******************************************************************************/

/**************************************************************************//**
 * Function Name: R_SOUND_SetSamplingRate
 * @brief         Set sampling rate to DAC
 *
 *                Description:<br>
 *                Set sampling rate.
 * @param[in]     chnl         :SSIF Channel #
 * @param[in]     freq       :sampling rate(8k - 96k)
 * @retval        DEVDRV_SUCCESS   :Success.
 * @retval        error code      :Failure.
 ******************************************************************************/
int32_t R_SOUND_SetSamplingRate (const uint32_t chnl, const uint32_t freq)
{
    int32_t ercd = DEVDRV_SUCCESS;
    bool_t sem_token;

    if (SOUND_DRVSTS_UNINIT == gs_gb_snd_ctrl.drv_sts)
    {
        ercd = DEVDRV_ERROR;
    }
    else
    {
        sem_token = R_OS_WaitForSemaphore( gs_gb_snd_ctrl.semid, SOUND_DRV_REQ_TMOUT_PRV_);

        if ( !sem_token)
        {
            ercd = DEVDRV_ERROR;
        }
        else
        {
            if ((SOUND_CH_NUM <= chnl) || (false == is_valid_freq(freq)))
            {
                ercd = DEVDRV_ERROR;
            }
            else
            {
                if (SOUND_CHSTS_UNINIT == gs_gb_snd_ctrl.ch_sts[chnl])
                {
                    ercd = DEVDRV_ERROR;
                }

                if (DEVDRV_SUCCESS == ercd)
                {
                    ercd = sound_set_sampling_rate(chnl, freq);
                }
            }

            R_OS_ReleaseSemaphore( gs_gb_snd_ctrl.semid);
        }
    }

    return (ercd);
}
/*******************************************************************************
 End of function R_SOUND_SetSamplingRate
 ******************************************************************************/

/**************************************************************************//**
 * Function Name: R_SOUND_SetFormat
 * @brief         Set I2S BUS format to DAC.
 *
 *                Description:<br>
 *                Set I2S BUS format.
 * @param[in]     chnl         :SSIF Channel #
 * @param[in]     fmt        :format(I2S, LEFT, RIGHT)
 * @param[in]     dwdsz      :dataword size(16bit, 18bit, 20bit, 24bit, 32bit)
 * @retval        DEVDRV_SUCCESS   :Success.
 * @retval        error code      :Failure.
 ******************************************************************************/
int32_t R_SOUND_SetFormat (const uint32_t chnl, const uint32_t fmt, const uint32_t dwdsz)
{
    int32_t ercd = DEVDRV_SUCCESS;
    bool_t sem_token;

    if (SOUND_DRVSTS_UNINIT == gs_gb_snd_ctrl.drv_sts)
    {
        ercd = DEVDRV_ERROR;
    }
    else
    {
        sem_token = R_OS_WaitForSemaphore( gs_gb_snd_ctrl.semid, SOUND_DRV_REQ_TMOUT_PRV_);

        if ( !sem_token)
        {
            ercd = DEVDRV_ERROR;
        }
        else
        {
            if ((SOUND_CH_NUM <= chnl) || (false == is_valid_fmt(fmt)) || (false == is_valid_dwd(dwdsz)))
            {
                ercd = DEVDRV_ERROR;
            }
            else
            {
                if (SOUND_CHSTS_UNINIT == gs_gb_snd_ctrl.ch_sts[chnl])
                {
                    ercd = DEVDRV_ERROR;
                }

                if (DEVDRV_SUCCESS == ercd)
                {
                    ercd = sound_set_format(chnl, fmt, dwdsz);
                }
            }

            R_OS_ReleaseSemaphore( gs_gb_snd_ctrl.semid);
        }
    }

    return (ercd);
}
/*******************************************************************************
 End of function R_SOUND_SetFormat
 ******************************************************************************/

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/

/**************************************************************************//**
 * Function Name: is_valid_freq
 * @brief         Check frequency.
 *
 *                Description:<br>
 *                If value is valid then return true.
 * @param[in]     freq       :sampling rate(8k - 96k)
 * @retval        true       :valid.
 * @retval        false      :invalid.
 ******************************************************************************/
static bool_t is_valid_freq (const uint32_t freq)
{
    bool_t ret;

    switch (freq)
    {
        /* fall through */
        case SOUND_FREQ_8000:

            /* fall through */
        case SOUND_FREQ_11025:

            /* fall through */
        case SOUND_FREQ_12000:

            /* fall through */
        case SOUND_FREQ_16000:

            /* fall through */
        case SOUND_FREQ_22050:

            /* fall through */
        case SOUND_FREQ_24000:

            /* fall through */
        case SOUND_FREQ_32000:

            /* fall through */
        case SOUND_FREQ_44100:

            /* fall through */
        case SOUND_FREQ_48000:

            /* fall through */
        case SOUND_FREQ_88200:

        case SOUND_FREQ_96000:
        {
            ret = true;
        }
        break;

        default:
        {
            ret = false;
        }
        break;
    }

    return (ret);
}
/*******************************************************************************
 End of function is_valid_freq
 ******************************************************************************/

/**************************************************************************//**
 * Function Name: is_valid_fmt
 * @brief         Check format.
 *
 *                Description:<br>
 *                If value is valid then return true.
 * @param[in]     fmt        :format(I2S, LEFT, RIGHT)
 * @retval        true       :valid.
 * @retval        false      :invalid.
 ******************************************************************************/
static bool_t is_valid_fmt (const uint32_t fmt)
{
    bool_t ret;

    switch (fmt)
    {
        /* fall through */
        case SOUND_FMT_I2S:

            /* fall through */
        case SOUND_FMT_RIGHT:

            /* fall through */
        case SOUND_FMT_LEFT:
        {
            ret = true;
        }
        break;

        default:
        {
            ret = false;
        }
        break;
    }

    return (ret);
}
/*******************************************************************************
 End of function is_valid_fmt
 ******************************************************************************/

/**************************************************************************//**
 * Function Name: is_valid_dwd
 * @brief         Check dataword size.
 *
 *                Description:<br>
 *                If value is valid then return true.
 * @param[in]     dwdsz      :dataword size(16bit, 18bit, 20bit, 24bit, 32bit)
 * @retval        true       :valid.
 * @retval        false      :invalid.
 ******************************************************************************/
static bool_t is_valid_dwd (const uint32_t dwdsz)
{
    bool_t ret;

    switch (dwdsz)
    {
        /* fall through */
        case SOUND_DWORD_16:

            /* fall through */
        case SOUND_DWORD_18:

            /* fall through */
        case SOUND_DWORD_20:

            /* fall through */
        case SOUND_DWORD_24:

        case SOUND_DWORD_32:
        {
            ret = true;
        }
        break;

        default:
        {
            ret = false;
        }
        break;
    }

    return (ret);
}
/*******************************************************************************
 End of function is_valid_dwd
 ******************************************************************************/
