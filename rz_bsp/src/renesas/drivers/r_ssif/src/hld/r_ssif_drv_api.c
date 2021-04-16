/******************************************************************************
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
 *******************************************************************************
 * Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
 *******************************************************************************
 * File Name    : r_ssid_drv_api.c
 * Version      : 1.00
 * Description  : This file contains the API for the SSIF Driver.
 *******************************************************************************
 * History      : DD.MM.YYYY Version Description
 *              : 04.02.2010 1.00    First Release
 ******************************************************************************/

/******************************************************************************
 WARNING!  IN ACCORDANCE WITH THE USER LICENCE THIS CODE MUST NOT BE CONVEYED
 OR REDISTRIBUTED IN COMBINATION WITH ANY SOFTWARE LICENSED UNDER TERMS THE
 SAME AS OR SIMILAR TO THE GNU GENERAL PUBLIC LICENCE
 ******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include <fcntl.h>

#include "r_typedefs.h"
#include "compiler_settings.h"
#include "r_devlink_wrapper.h"
#include "r_ssif_drv_api.h"
#include "ssif.h"
#include "dev_drv.h"

/* Adding this dependency ensures that this file is built every time because of the build counter */
#include "version.h"

/*****************************************************************************
 Function Prototypes
 ******************************************************************************/

/* Driver Functions */
static int_t ssif_open (st_stream_ptr_t pStream);
static void ssif_close (st_stream_ptr_t pStream);
static int_t ssif_control (st_stream_ptr_t pStream, uint32_t ctlCode, void *pCtlStruct);
static int_t ssif_get_version (st_stream_ptr_t pStream, st_ver_info_t *pVerInfo);
static int_t ssif_write (st_stream_ptr_t pStream, uint8_t *pbyBuffer, uint32_t uiCount);
static int_t ssif_read (st_stream_ptr_t pStream, uint8_t *pbyBuffer, uint32_t uiCount);

/* SSIF Initialise functions */
static void *ssif_init (void * const config_data, int32_t * const p_errno);
static int_t ssif_un_init (void * const driver_instance, int32_t * const p_errno);
static int_t configure_ssif_channel (int_t channel);

/*****************************************************************************
 Constant Data
 ******************************************************************************/
static const st_drv_info_t gs_hld_info =
{
{ ((R_SSIF_RZ_HLD_VERSION_MAJOR << 16) + R_SSIF_RZ_HLD_VERSION_MINOR) },

    R_SSIF_RZ_HLD_BUILD_NUM,
    R_SSIF_RZ_HLD_DRV_NAME
};

/* structure pointer for setting up DMA read */
static AIOCB *gsp_aio_r = NULL;

/* structure pointer for setting up DMA write */
static AIOCB *gsp_aio_w = NULL;

/* pointer to info for SSIF channel */
static ssif_info_ch_t *gsp_info_ch = (ssif_info_ch_t *)NULL;

/* Configured Inputs to Open */
static ssif_channel_cfg_t gs_ssif_cfg[SSIF_NUM_CHANS];

/* Define the driver function table for this */
const st_r_driver_t g_ssif_driver =
{ "SSIF Device Driver", ssif_open, ssif_close, ssif_read, ssif_write, ssif_control, ssif_get_version };

extern ssif_info_drv_t g_ssif_info_drv;

/******************************************************************************
 Function Name: ssif_open
 Description:   Function to open the system timer device driver
 Arguments:     IN  pStream - Pointer to the file stream
 Return value:  0 for success otherwise -1
 ******************************************************************************/
static int_t ssif_open (st_stream_ptr_t pStream)
{
    int_t ercd = DEVDRV_SUCCESS;

    /* Configure SSIF Channel 0. 4 channels available (0-3) */
    configure_ssif_channel(SSIF_CHNUM_0);

    /* get access to channel to enable it */
    if (false == R_OS_WaitForSemaphore( gsp_info_ch->sem_access, R_OS_ABSTRACTION_PRV_EV_WAIT_INFINITE))
    {
        ercd = DEVDRV_ERROR;
    }
    else
    {

        gsp_info_ch->openflag = pStream->file_flag;

        /* Initialise the tx buffer element */
        gsp_info_ch->p_aio_tx_curr = NULL;

        /* Initialise the rx buffer element */
        gsp_info_ch->p_aio_rx_curr = NULL;

        ercd = SSIF_EnableChannel(gsp_info_ch);
        if (DEVDRV_SUCCESS == ercd)
        {
            gsp_info_ch->ch_stat = SSIF_CHSTS_OPEN;
        }

    }

    R_OS_ReleaseSemaphore( gsp_info_ch->sem_access);

    return (ercd);
}
/******************************************************************************
 End of function  ssif_open
 ******************************************************************************/

/******************************************************************************
 Function Name: ssif_close
 Description:   Function to close the standard IN driver. All open devices
 connected to the tmr are closed.
 Arguments:     IN  pStream - Pointer to the file stream
 Return value:  none
 ******************************************************************************/
static void ssif_close (st_stream_ptr_t pStream)
{
    /* Unused parameter*/
    (void) pStream;

    int32_t ercd = DEVDRV_SUCCESS;

    ercd = ssif_un_init( &g_ssif_info_drv, &ercd);

    /* Ensure that there is a drive to close */
    if ((NULL == gsp_info_ch) || (DEVDRV_SUCCESS != ercd))
    {
        ercd = DEVDRV_ERROR;
    }
    else
    {
        /* Get semaphore to access the channel data */
        if (false == R_OS_WaitForSemaphore( gsp_info_ch->sem_access, R_OS_ABSTRACTION_PRV_EV_WAIT_INFINITE))
        {
            ercd = DEVDRV_ERROR;
        }
        else
        {
            if (SSIF_CHSTS_OPEN != gsp_info_ch->ch_stat)
            {
                ercd = DEVDRV_ERROR;
            }
            else
            {
                /* No p_errno not required */
                SSIF_PostAsyncCancel(gsp_info_ch, NULL);

                ercd = SSIF_DisableChannel(gsp_info_ch);

                if (DEVDRV_SUCCESS == ercd)
                {
                    gsp_info_ch->ch_stat = SSIF_CHSTS_INIT;
                }
            }

            /* Release semaphore */
            R_OS_ReleaseSemaphore( gsp_info_ch->sem_access);
        }
    }
}
/******************************************************************************
 End of function  ssif_close
 ******************************************************************************/

/******************************************************************************
 Function Name: ssif_control
 Description:   Function to handle custom controls for the standard IN device
 Arguments:     IN  pStream - Pointer to the file stream
 IN  ctlCode - The custom control code
 IN  pCtlStruct - Pointer to the custom control structure
 Return value:  0 for success -1 on error
 ******************************************************************************/
static int_t ssif_control (st_stream_ptr_t pStream, uint32_t ctlCode, void *pCtlStruct)
{
    int_t result = DEVDRV_ERROR;

    /* Avoid unused parameter compiler warning */
    (void) pStream;
    if (pCtlStruct)
    {
        switch (ctlCode)
        {
            case R_SSIF_CONTROL_CONFIGURE:
            {
                st_r_ssif_drv_control_t *p_control_struct = pCtlStruct;

                /* Comparison with NULL */
                if (NULL == p_control_struct->p_buf)
                {
                    result = DEVDRV_ERROR;
                }
                else
                {
                    ssif_channel_cfg_t * const p_new_ch_info = p_control_struct->p_buf;
                    result = SSIF_IOCTL_ConfigChannel(gsp_info_ch, p_new_ch_info);
                    if (DEVDRV_SUCCESS != result)
                    {
                        result = DEVDRV_ERROR;
                    }
                }
                break;
            }

            case R_SSIF_CONTROL_STATUS:
            {
                st_r_ssif_drv_control_t *p_control_struct = pCtlStruct;

                /* Comparison with NULL */
                if (NULL == p_control_struct->p_buf)
                {
                    result = DEVDRV_ERROR;
                }
                else
                {
                    result = SSIF_IOCTL_GetStatus(gsp_info_ch, p_control_struct->p_buf);
                    if (DEVDRV_SUCCESS != result)
                    {
                        result = DEVDRV_ERROR;
                    }
                }
                break;
            }
            case R_SSIF_CONTROL_CANCEL:
            {
                /* nothing */
                ;
            }
            break;

            case R_SSIF_AIO_READ_CONTROL:
            {
                /* point to read setup */
                gsp_aio_r = (AIOCB *) pCtlStruct;
                result = DEVDRV_SUCCESS;
            }
            break;

            case R_SSIF_AIO_WRITE_CONTROL:
            {
                /* point to write setup */
                gsp_aio_w = (AIOCB *) pCtlStruct;
                result = DEVDRV_SUCCESS;
            }
            break;

            default:
            {
                result = DEVDRV_ERROR;
                break;
            }
        }
    }
    return result;
}
/******************************************************************************
 End of function  ssif_control
 ******************************************************************************/

/******************************************************************************
 * Function Name: ssif_write
 * @brief         Enqueue asynchronous write request
 *
 *                Description:<br>
 *
 * @param[in,out] p_fd       :channel handle which was returned by R_SSIF_Open
 * @param[in]     p_aio      :aio control block of write request
 * @param[in,out] p_errno    :pointer of error code
 * @retval        DEVDRV_SUCCESS   :Success.
 * @retval        DEVDRV_ERROR     :Failure.
 ******************************************************************************/
static int_t ssif_write (st_stream_ptr_t pStream, uint8_t *pbyBuffer, uint32_t uiCount)
{

    int_t ercd = DEVDRV_SUCCESS;

    /* Ensure writing is allowed */
    if (O_RDONLY != pStream->file_flag)
    {

        /* Ensure that the ssif is configured correctly */
        if ((NULL == gsp_info_ch) || (NULL == gsp_aio_w))
        {
            ercd = DEVDRV_ERROR;
        }
        else
        {
            /* Validate parameters */
            if ((0u == uiCount) || (NULL == pbyBuffer))
            {
                ercd = DEVDRV_ERROR;
            }
            else
            {
                /* update file descriptor field with pointer to channel configuration */
                gsp_aio_w->aio_fildes = (int) gsp_info_ch;

                /* Enable callback on message */
                gsp_aio_w->aio_sigevent.sigev_notify = SIGEV_THREAD;

                /* set operation type */
                gsp_aio_w->aio_return = SSIF_ASYNC_W;

                /* number of bytes */
                gsp_aio_w->aio_nbytes = uiCount;

                /* pointer to buffer */
                gsp_aio_w->aio_buf = (void *) pbyBuffer;

                /* pass message to config structure */
                gsp_info_ch->p_aio_tx_next = gsp_aio_w;

                /* Go! */
                SSIF_PostAsyncIo(gsp_info_ch, gsp_aio_w);
            }
        }
    }

    return ercd;
}
/*******************************************************************************
 End of function ssif_write
 ******************************************************************************/

/******************************************************************************
 * Function Name: ssif_read
 * @brief         Enqueue asynchronous read request
 *
 *                Description:<br>
 *
 * @param[in,out] p_fd       :channel handle which was returned by R_SSIF_Open
 * @param[in]     p_aio      :aio control block of read request
 * @param[in,out] p_errno    :pointer of error code
 * @retval        DEVDRV_SUCCESS   :Success.
 * @retval        DEVDRV_ERROR     :Failure.
 ******************************************************************************/
static int_t ssif_read (st_stream_ptr_t pStream, uint8_t *pbyBuffer, uint32_t uiCount)
{
    int_t ercd = DEVDRV_SUCCESS;

    /* Ensure reading is allowed */
    if (O_WRONLY != pStream->file_flag)
    {

        /* Ensure that the driver is configured */
        if ((NULL == gsp_info_ch) || (NULL == gsp_aio_r))
        {
            ercd = DEVDRV_ERROR;
        }
        else
        {
            /* update file descriptor field with pointer to channel configuration */
            gsp_aio_r->aio_fildes = (int) gsp_info_ch;

            /* Enable callback on message */
            gsp_aio_r->aio_sigevent.sigev_notify = SIGEV_THREAD;

            /* set operation type */
            gsp_aio_r->aio_return = SSIF_ASYNC_R;

            /* number of bytes */
            gsp_aio_r->aio_nbytes = uiCount;

            /* pointer to buffer */
            gsp_aio_r->aio_buf = (void *) pbyBuffer;

            /* pass message to config structure */
            gsp_info_ch->p_aio_rx_next = gsp_aio_r;

            /* Go! */
            SSIF_PostAsyncIo(gsp_info_ch, gsp_aio_r);
        }
    }

    return ercd;
}
/*******************************************************************************
 End of function ssif_read
 ******************************************************************************/

/*******************************************************************************
 Function Name: ssif_get_version
 Description  : Provides build information even if driver fails to open
 version information is updated by developer
 Arguments    : none
 Return Value : 0 (never fails)
 ******************************************************************************/
static int_t ssif_get_version (st_stream_ptr_t pStream, st_ver_info_t *pVerInfo)
{
    /* Unused parameter */
    (void) pStream;

    pVerInfo->hld.version.sub.major = gs_hld_info.version.sub.major;
    pVerInfo->hld.version.sub.minor = gs_hld_info.version.sub.minor;
    pVerInfo->hld.build = gs_hld_info.build;
    pVerInfo->hld.p_szdriver_name = gs_hld_info.p_szdriver_name;

    /* Obtain version information from Low layer Driver */
    R_SSIF_GetVersion(pVerInfo);

    return 0;
}
/*******************************************************************************
 End of function ssif_get_version
 ******************************************************************************/

/******************************************************************************
 * Function Name: ssif_init
 * @brief         Initialises the SSIF driver.
 *
 *                Description: Initialises the SSIF Driver.
 *
 * @param[in,out] driver_instance :driver instance which was returned by<br>
 ssif_init
 * @param[in,out] p_errno         :pointer of error code
 * @retval        DEVDRV_SUCCESS        :Success.
 * @retval        DEVDRV_ERROR          :Failure.
 ******************************************************************************/
static void *ssif_init (void * const p_config_data, int32_t * const p_errno)
{
    /* Unused parameter */
    (void) p_errno;

    int_t ercd = DEVDRV_ERROR;

    /* cast to void */
    void *p_ret = (void *)NULL;

    /* driver instance must not be NULL */
    if (NULL == p_config_data)
    {
        ercd = DEVDRV_ERROR;
    }
    else if (SSIF_DRVSTS_UNINIT != g_ssif_info_drv.drv_stat)
    {
        ercd = DEVDRV_ERROR;
    }
    else
    {
        g_ssif_info_drv.drv_stat = SSIF_DRVSTS_INIT;

        /* cast to ssif_channel_cfg_t pointer */
        ercd = SSIF_Initialise((ssif_channel_cfg_t *) p_config_data);

        if (DEVDRV_SUCCESS == ercd)
        {
            /* cast to void */
            p_ret = (void *) &g_ssif_info_drv;
        }
        else
        {
            g_ssif_info_drv.drv_stat = SSIF_DRVSTS_UNINIT;
        }
    }

    if (DEVDRV_ERROR == ercd)
    {
        /* Set p_ret to NULL */
        p_ret = NULL;
    }

    return p_ret;
}
/*******************************************************************************
 End of function ssif_init
 ******************************************************************************/

/******************************************************************************
 * Function Name: ssif_un_init
 * @brief         Uninitialise the SSIF driver.
 *
 *                Description: Unitailises the SSIF Driver.
 *
 * @param[in,out] driver_instance :driver instance which was returned by<br>
 ssif_init
 * @param[in,out] p_errno         :pointer of error code
 * @retval        DEVDRV_SUCCESS        :Success.
 * @retval        DEVDRV_ERROR          :Failure.
 ******************************************************************************/
static int_t ssif_un_init (void * const driver_instance, int32_t * const p_errno)
{
    /* Unused parameter */
    (void) p_errno;

    int_t ercd;
    int_t ret = DEVDRV_SUCCESS;
    ssif_info_drv_t * const p_info_drv = driver_instance;

    /* Null comparison */
    if (NULL == p_info_drv)
    {
        ercd = DEVDRV_ERROR;
    }
    else
    {
        if (SSIF_DRVSTS_INIT != p_info_drv->drv_stat)
        {
            ercd = DEVDRV_ERROR;
        }
        else
        {
            ercd = SSIF_UnInitialise();
            p_info_drv->drv_stat = SSIF_DRVSTS_UNINIT;
        }
    }

    if (DEVDRV_SUCCESS != ercd)
    {
        ret = DEVDRV_ERROR;
    }

    return ret;
}
/*******************************************************************************
 End of function ssif_un_init
 ******************************************************************************/

/*******************************************************************************
 * Function Name: configure_ssif_channel
 * Description  : Configures the appropriate SSIF channel. The limit to which is
 * 				  defined in the driver. Currently the function is limited to a
 * 				  hard coded gs_ssif_cfg structure.
 * Arguments    : int_t channel - configures the appropriate channel
 ******************************************************************************/
static int_t configure_ssif_channel (int_t channel)
{
    ssif_info_drv_t *p_info_drv;
    int_t ecrd = DEVDRV_SUCCESS;

    gs_ssif_cfg[channel].enabled = true;
    gs_ssif_cfg[channel].int_level = ISR_SSIF_IRQ_PRIORITY;
    gs_ssif_cfg[channel].slave_mode = false;
    gs_ssif_cfg[channel].sample_freq = 44100u;
    gs_ssif_cfg[channel].clk_select = SSIF_CFG_CKS_AUDIO_X1;
    gs_ssif_cfg[channel].multi_ch = SSIF_CFG_MULTI_CH_1;
    gs_ssif_cfg[channel].data_word = SSIF_CFG_DATA_WORD_16;
    gs_ssif_cfg[channel].system_word = SSIF_CFG_SYSTEM_WORD_32;
    gs_ssif_cfg[channel].bclk_pol = SSIF_CFG_FALLING;
    gs_ssif_cfg[channel].ws_pol = SSIF_CFG_WS_LOW;
    gs_ssif_cfg[channel].padding_pol = SSIF_CFG_PADDING_LOW;
    gs_ssif_cfg[channel].serial_alignment = SSIF_CFG_DATA_FIRST;
    gs_ssif_cfg[channel].parallel_alignment = SSIF_CFG_LEFT;
    gs_ssif_cfg[channel].ws_delay = SSIF_CFG_DELAY;
    gs_ssif_cfg[channel].noise_cancel = SSIF_CFG_DISABLE_NOISE_CANCEL;
    gs_ssif_cfg[channel].tdm_mode = SSIF_CFG_DISABLE_TDM;

    /* initialise driver data structures */
    p_info_drv = ssif_init( &gs_ssif_cfg, NULL);

    /* Casting NULL */
    if (NULL == p_info_drv)
    {
        /* Error - SSIF not initialised correctly */
        ecrd = DEVDRV_ERROR;
    }
    else
    {
        gsp_info_ch = &p_info_drv->info_ch[channel];
    }

    return ecrd;
}
/*******************************************************************************
 End of function configure_ssif_channel
 ******************************************************************************/
