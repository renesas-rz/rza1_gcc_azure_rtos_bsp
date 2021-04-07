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
 * Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
 ******************************************************************************/
/*******************************************************************************
 * File Name    : r_riic_drv_api.c
 * Version      : 1.0
 * Description  : I2C high level device driver
 ******************************************************************************
 * History      : DD.MM.YYYY Version Description
 *              : 06.02.2018 1.00    First Release
 ******************************************************************************/

/******************************************************************************
 WARNING!  IN ACCORDANCE WITH THE USER LICENCE THIS CODE MUST NOT BE CONVEYED
 OR REDISTRIBUTED IN COMBINATION WITH ANY SOFTWARE LICENSED UNDER TERMS THE
 SAME AS OR SIMILAR TO THE GNU GENERAL PUBLIC LICENCE
 ******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

#include "r_riic_drv_api.h"
#include "r_sc_cfg.h"
#include "r_riic_drv_sc_cfg.h"
#include "trace.h"
#include "r_riic_hld_prv.h"
#include "r_riic_drv_link.h"    /* Link include to low level driver */
#include "dev_drv.h"
#include "r_os_abstraction_api.h"

/******************************************************************************
 Macro definitions
 ******************************************************************************/

/* Comment this line out to turn ON module trace in this file */
#undef _TRACE_ON_

#ifndef _TRACE_ON_
    #undef TRACE
    #define TRACE(x)
#endif

/*****************************************************************************
 Function Prototypes
 ******************************************************************************/

static int_t iic_hld_open (st_stream_ptr_t pStream);
static void iic_hld_close (st_stream_ptr_t pStream);
static int_t iic_hld_control (st_stream_ptr_t pStream, uint32_t ctlCode, void *pCtlStruct);
static int_t iic_hld_get_version (st_stream_ptr_t stream_ptr, st_ver_info_t *pVerInfo);
static int_t riic_get_channel (st_stream_ptr_t stream_ptr);

/*****************************************************************************
 Constant Data
 ******************************************************************************/

static const st_drv_info_t gs_hld_info =
{
{ ((STDIO_RIIC_RZ_HLD_VERSION_MAJOR << 16) + STDIO_RIIC_RZ_HLD_VERSION_MINOR) },
STDIO_RIIC_RZ_HLD_BUILD_NUM,
STDIO_RIIC_RZ_HLD_DRV_NAME };

static uint16_t gs_channel_open = 0;

/* Define the driver function table for this implementation */
const st_r_driver_t g_riic_driver =
{ "I2C Device Driver", iic_hld_open, iic_hld_close, no_dev_io, no_dev_io, iic_hld_control, iic_hld_get_version };

static semaphore_t   iic_hld_sem;

/******************************************************************************
 Private global variables
 ******************************************************************************/

/*****************************************************************************
 Private Functions
 ******************************************************************************/

/******************************************************************************
 Function Name: iic_hld_open
 Description:   Function to open the I2C device driver
 Arguments:     IN pStream - Pointer to the file stream
 Return value:  0 for success otherwise -1
 ******************************************************************************/
static int_t iic_hld_open (st_stream_ptr_t p_stream)
{
    int_t ret_value = DEVDRV_SUCCESS;

    (void) p_stream;

    /* increase channel open count if open was successful */
    if (gs_channel_open == 0)
    {
        gs_channel_open = 1;
        R_OS_CreateSemaphore(&iic_hld_sem, 1);
    }

    /* return status */
    return (ret_value);
}
/******************************************************************************
 End of function iic_hld_open
 ******************************************************************************/

/******************************************************************************
 Function Name: iic_hld_close
 Description:   Function to close the I2C driver
 Arguments:     IN pStream - Pointer to the file stream
 Return value:  none
 ******************************************************************************/
static void iic_hld_close (st_stream_ptr_t p_stream)
{
    /* determine hardware channel number */
    int_t channel = riic_get_channel(p_stream);

    /* close channel */
    close_channel(channel);

    /* decrease channel open count */
    if (gs_channel_open == 1)
    {
        gs_channel_open = 0;
        R_OS_DeleteSemaphore(&iic_hld_sem);
    }
}
/******************************************************************************
 End of function iic_hld_close
 ******************************************************************************/

/******************************************************************************
 Function Name: iic_hld_control
 Description:   Function to handle custom controls for the I2C device driver
 Arguments:     IN  p_stream - Pointer to the file stream
 IN  ctl_code - The custom control code
 IN  p_ctl_struct - Pointer to the custom control structure
 Return value:  DEVDRV_SUCCESS for success DEVDRV_ERROR on error
 ******************************************************************************/
static int_t iic_hld_control (st_stream_ptr_t p_str, uint32_t ctl_code, void *p_ctl_struct)
{
    int_t ret_value = DEVDRV_ERROR;

    /* get channel number */
    int_t channel = riic_get_channel(p_str);

    /* fail if pStream has wrong name */
    if ((DEVDRV_ERROR) == channel)
    {
        return (ret_value);
    }

    /* fail if the channel is not open */
    if (0 == gs_channel_open)
    {
        return (ret_value);
    }

    if (p_ctl_struct)
    {
        switch (ctl_code)
        {
            case CTL_RIIC_CREATE:
            {
                /* obtain selected channel configuration from the generic pointer argument */
                st_r_drv_riic_create_t *p_channel_config = (st_r_drv_riic_create_t *) p_ctl_struct;

                /* open selected channel with selected frequency */
                ret_value = open_channel(channel, p_channel_config);
            }
            break;

            case CTL_RIIC_READ:
            {
                R_OS_WaitForSemaphore( &iic_hld_sem, R_OS_ABSTRACTION_PRV_EV_WAIT_INFINITE );

                /* cast control pointer to control struct */
                st_r_drv_riic_config_t *p_i2c_read = (st_r_drv_riic_config_t *) p_ctl_struct;
                int_t error = read_data(channel, p_i2c_read->device_address, p_i2c_read->sub_address,
                        p_i2c_read->number_of_bytes, p_i2c_read->p_data_buffer);
                R_OS_ReleaseSemaphore( &iic_hld_sem );

                if (DEVDRV_SUCCESS == error)
                {
                    ret_value = DEVDRV_SUCCESS;
                }
                break;
            }

            case CTL_RIIC_READ_NEXT:
            {
                R_OS_WaitForSemaphore( &iic_hld_sem, R_OS_ABSTRACTION_PRV_EV_WAIT_INFINITE );

                /* cast control pointer to control struct */
                st_r_drv_riic_config_t *p_i2c_read = (st_r_drv_riic_config_t *) p_ctl_struct;
                int_t error = read_next_data(channel, p_i2c_read->device_address,
                        p_i2c_read->number_of_bytes, p_i2c_read->p_data_buffer);
                R_OS_ReleaseSemaphore( &iic_hld_sem );

                if (DEVDRV_SUCCESS == error)
                {
                    ret_value = DEVDRV_SUCCESS;
                }
                break;
            }

            case CTL_RIIC_WRITE:
            {
                R_OS_WaitForSemaphore( &iic_hld_sem, R_OS_ABSTRACTION_PRV_EV_WAIT_INFINITE );

                /* cast control pointer to control struct */
                st_r_drv_riic_config_t *p_i2c_write = (st_r_drv_riic_config_t *) p_ctl_struct;
                int_t error = write_data(channel, p_i2c_write->device_address, p_i2c_write->sub_address,
                        p_i2c_write->number_of_bytes, p_i2c_write->p_data_buffer);
                R_OS_ReleaseSemaphore( &iic_hld_sem );

                if (DEVDRV_SUCCESS == error)
                {
                    ret_value = DEVDRV_SUCCESS;
                }
                break;
            }

            default:
            {
                TRACE(("riic Driver: Unknown control code\r\n"));
                break;
            }
        }
    }

    return (ret_value);
}
/******************************************************************************
 End of function  iic_hld_control
 ******************************************************************************/

/*******************************************************************************
 * Function Name: iic_hld_get_version
 * Description  : Provides build information even if driver fails to open
 *                version information is updated by developer
 * Arguments    : none
 * Return Value : DEVDRV_SUCCESS (never fails)
 ******************************************************************************/
static int_t iic_hld_get_version (st_stream_ptr_t stream_ptr, st_ver_info_t *p_ver_info)
{
    (void) stream_ptr;

    p_ver_info->hld.version.sub.major = gs_hld_info.version.sub.major;
    p_ver_info->hld.version.sub.minor = gs_hld_info.version.sub.minor;
    p_ver_info->hld.build = gs_hld_info.build;
    p_ver_info->hld.p_szdriver_name = gs_hld_info.p_szdriver_name;

    /* Obtain version information from Low layer Driver */
    R_RIIC_GetVersion(p_ver_info);

    return (DEVDRV_SUCCESS);
}
/*******************************************************************************
 End of function iic_hld_get_version
 ******************************************************************************/

/*******************************************************************************
 * Function Name: riic_get_channel
 * Description  : Provides channel number written in stream_ptr
 * Arguments    : IN pStream - Pointer to the file stream
 * Return value:  0 to 3 for channel number -1 on error
 ******************************************************************************/
static int_t riic_get_channel (st_stream_ptr_t stream_ptr)
{
    int_t ret_value = DEVDRV_ERROR;
    int_t channel_id;

    if (NULL != stream_ptr)
    {
        /* cast channel id to an int_t */
        channel_id = (int_t) (stream_ptr->sc_config_index);

        /* verify that the channel is supported */

        /* look for channel in the conglomeration  of available channels from low level driver */
        /* (e_channel_id_t). This is a bitfield where the power of 2 is the available channel */
        if (((1 << channel_id) <= R_CH15))
        {
            /* set channel as bitfield where value is 2 << channel */
            int_t test1 = (1 << channel_id);

            /* get bitfield of supported channels */
            int_t test2 = RIIC_LLD_SUPPORTED_CHANNELS;
            if (0 != (test1 & test2))
            {
                /* Channel is supported */
                ret_value = (int_t) channel_id;
            }
            else
            {
                /* Channel is supported */
                ret_value = DEVDRV_ERROR;
            }
        }
    }

    /* return status as error or channel number  */
    return (ret_value);
}
/*******************************************************************************
 End of function riic_get_channel
 ******************************************************************************/

/******************************************************************************
 End  Of File
 ******************************************************************************/
