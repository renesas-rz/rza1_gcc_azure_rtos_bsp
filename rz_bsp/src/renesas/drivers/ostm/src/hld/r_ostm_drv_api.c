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
 * Copyright (C) 2012 - 2014 Renesas Electronics Corporation. All rights reserved.
 *******************************************************************************/
/*******************************************************************************
 * File Name   : r_ostm_drv_api.c
 * Description : OS timer driver high layer driver
 *******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include <string.h>

#include "r_ostm_drv_api.h"
#include "r_ostm_drv_sc_cfg.h"
#include "r_ostm_drv_sc_private.h"
#include "r_os_abstraction_api.h"

/******************************************************************************
 Typedef definitions
 ******************************************************************************/

/******************************************************************************
 Macro definitions
 ******************************************************************************/
/* Comment this line out to turn ON module trace in this file */
#undef _TRACE_ON_

#ifndef _TRACE_ON_
#undef DEBUG_PRV_TRACE
#define  DEBUG_PRV_TRACE(x)
#endif

/******************************************************************************
 Enumerated Types
 ******************************************************************************/

/******************************************************************************
 Typedefs
 ******************************************************************************/
typedef struct r_drvostmdt
{
    int_t mapping;
    int_t lld_ch;
} st_r_drv_resource_t;

typedef struct r_drvostm
{
    uint32_t access_handles_available;
    uint8_t drv_free;
} st_r_drv_res_map_t;

/*****************************************************************************
 Function Prototypes
 ******************************************************************************/
static int_t ostm_open(st_stream_ptr_t stream_ptr);
static void ostm_close(st_stream_ptr_t stream_ptr);
static int_t ostm_read(st_stream_ptr_t stream_ptr, uint8_t *buffer, uint32_t count);

static int_t ostm_control(st_stream_ptr_t stream_ptr, uint32_t ctrl_code, void *ctrl_ptr);
static int_t ostm_get_version(st_stream_ptr_t stream_ptr, st_ver_info_t *pVerInfo);

/*****************************************************************************
 Constant Data
 ******************************************************************************/

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/

/* default configuration used to initialise gs_r_drv_ostm */
static const st_r_drv_res_map_t gs_r_drv_ostm_default[R_CFG_OSTM_DRV_CHANNELS_AVAIL_PRV] =
{
    {
      R_CFG_OSTM_DRV_MAXIMUM_ACCESS_PRV,
      1
    },
    {
      R_CFG_OSTM_DRV_MAXIMUM_ACCESS_PRV,
      1
    }
};

/* active configuration initialised by gs_r_drv_ostm_default, used by driver */
static st_r_drv_res_map_t gs_r_drv_ostm[R_CFG_OSTM_DRV_CHANNELS_AVAIL_PRV] = {};

static const st_drv_info_t gs_hld_info =
{
{ (( STDIO_OSTM_RZ_HLD_VERSION_MAJOR << 16) + STDIO_OSTM_RZ_HLD_VERSION_MINOR) },
  STDIO_OSTM_RZ_HLD_BUILD_NUM,
  STDIO_OSTM_RZ_HLD_DRV_NAME };

/* Define the driver function table for this implementation */
/* GSCE Warning (global variable 'g_ostm_Driver' not declared static [9.3a])
 * can be safely ignored */
const st_r_driver_t g_ostm_driver =
{
  /* st_r_driver_t p_device */
    "OS Timer Driver",
    ostm_open,
    ostm_close,
    ostm_read,
    no_dev_io,
    ostm_control,
    ostm_get_version
};

/***********************************************************************************************************************
 * Function Name: create_timer
 * Description  : configures a timer with a resource as described by the config
 * Arguments    : res - The unique identifier in which to reference the resource
 * Return Value : error codes -
 *                      DRV_SUCCESS   : Success to initialise OSTM
 *                      DRV_ERROR     : Failure to initialise OSTM
 **********************************************************************************************************************/
static int_t create_timer(st_stream_ptr_t res, st_r_drv_ostm_config_t *config_ptr)
{
    int_t ret = DRV_ERROR;

    ret = R_OSTM_Open(res->sc_config_index, config_ptr);

    return (ret);
}
/***********************************************************************************************************************
 End of function create_timer
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: destroy_timer
 * Description  : destroys a configured timer
 * Arguments    : res - The unique identifier in which to reference the resource
 * Return Value : error codes -
 *                      DRV_SUCCESS   : Success to initialise OSTM
 *                      DRV_ERROR     : Failure to initialise OSTM
 **********************************************************************************************************************/
static int_t destroy_timer(st_stream_ptr_t res)
{
    int_t ret = DRV_ERROR;
    uint32_t not_reported;

    /* Stop the Timer */
    ret = R_OSTM_Stop(res->sc_config_index);

    if(DRV_SUCCESS == ret)
    {
        /* read the final value of the counter (not currently used) */
        ret = R_OSTM_ReadCnt(res->sc_config_index, &not_reported);
    }

    if(DRV_SUCCESS == ret)
    {
        /* Close the Channel */
        ret = R_OSTM_Close(res->sc_config_index);
    }

    return (ret);
}
/***********************************************************************************************************************
 End of function destroy_timer
 **********************************************************************************************************************/


/***********************************************************************************************************************
 * Function Name: start_timer
 * Description  : starts the specified timer
 * Arguments    : res - The unique identifier in which to reference the resource
 * Return Value : error codes -
 *                      DRV_SUCCESS   : Success to initialise OSTM
 *                      DRV_ERROR     : Failure to initialise OSTM
 **********************************************************************************************************************/
static int_t start_timer(st_stream_ptr_t res)
{
    int_t ret = DRV_ERROR;

    ret = R_OSTM_Start(res->sc_config_index);

    return (ret);
}
/***********************************************************************************************************************
 End of function start_timer
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: stop_timer
 * Description  : stops the specified timer
 * Arguments    : res - The unique identifier in which to reference the resource
 * Return Value : error codes -
 *                      DRV_SUCCESS   : Success to initialise OSTM
 *                      DRV_ERROR     : Failure to initialise OSTM
 **********************************************************************************************************************/
static int_t stop_timer(st_stream_ptr_t res)
{
    int_t ret = DRV_ERROR;

    ret = R_OSTM_Stop(res->sc_config_index);

    return (ret);
}
/***********************************************************************************************************************
 End of function stop_timer
 **********************************************************************************************************************/


/******************************************************************************
 Function Name: ostm_open
 Description:   Function to open the system timer device driver
 Arguments:     IN  stream_ptr - Pointer to the file stream
 Return value:  DRV_SUCCESS : Success when attempting to initialise OSTM
 *              DRV_ERROR   : Failure to initialise OSTM
 ******************************************************************************/
static int_t ostm_open(st_stream_ptr_t stream_ptr)
{
    int_t ret = -1;

    /* get live configuration data from hld */
    if(false == gs_r_drv_ostm_open[stream_ptr->sc_config_index])
    {
        gs_r_drv_ostm_open[stream_ptr->sc_config_index] = true;

        /* currently this driver does not support sc */
        /* initialise_ostm_config_from_sc() should be called here */;

        /* reset active configuration */
        memcpy (&gs_r_drv_ostm[stream_ptr->sc_config_index], &gs_r_drv_ostm_default[stream_ptr->sc_config_index], sizeof(st_r_drv_res_map_t));

        gs_r_drv_ostm[stream_ptr->sc_config_index].access_handles_available--;

        ret = R_OSTM_InitialiseHwIf ();
    }
    else
    {
        /* No more modules can be granted simultaneous access to this driver */
        DEBUG_PRV_TRACE(("Error - Can not open %s channel is already opened\r\n", stream_ptr->pszStreamName));
    }

    return ret;
}
/******************************************************************************
 End of function  ostm_open
 ******************************************************************************/

/******************************************************************************
 Function Name: ostm_close
 Description:   Function to close the standard IN driver. All open devices
 connected to the led are closed.
 Arguments:     IN  stream_ptr - Pointer to the file stream
 Return value:  none
 ******************************************************************************/
static void ostm_close(st_stream_ptr_t stream_ptr)
{
    if(true == gs_r_drv_ostm_open[stream_ptr->sc_config_index])
    {
        gs_r_drv_ostm[stream_ptr->sc_config_index].access_handles_available++;
    }

    /* Ensure that the driver has been closed for the final time */
    if (R_CFG_OSTM_DRV_MAXIMUM_ACCESS_PRV == gs_r_drv_ostm[stream_ptr->sc_config_index].access_handles_available)
    {
        /* reset the state of this driver so that the next time it is opened the driver is reinitialized */
        gs_r_drv_ostm_open[stream_ptr->sc_config_index] = false;

        /* Only initialize during first use */
        R_OSTM_UninitialiseHwIf ();
    }
}
/******************************************************************************
 End of function  ostm_close
 ******************************************************************************/


/******************************************************************************
 Function Name: ostm_read
 Description:   Function to read data from the ostm device
 Arguments:     IN  stream_ptr - Pointer to the file stream
                IN  buffer - Pointer to the destination memory
                IN  count - The number of bytes to read
 Return value:  data count for success or Error Code on error
 ******************************************************************************/
static int_t ostm_read(st_stream_ptr_t stream_ptr, uint8_t *buffer, uint32_t count)
{
    int_t ret = DRV_ERROR;
    uint32_t data;


    /* ptr used for reference only */
    ret = R_OSTM_ReadCnt(stream_ptr->sc_config_index, &data);

    if(4 == count)
    {
        *(buffer + 3) = (uint8_t) (data >> 24);
        *(buffer + 2) = (uint8_t) (data >> 16);
        *(buffer + 1) = (uint8_t) (data >> 8);
        *(buffer + 0) = (uint8_t) data;
    }
    else
    {
        ret = DRV_ERROR;
    }

    return (ret);

}
/******************************************************************************
 End of function ostm_read
 ******************************************************************************/


/******************************************************************************
 Function Name: ostm_control
 Description:   Function to handle custom controls for the standard IN device
 Arguments:     IN  stream_ptr - Pointer to the file stream
 IN  ctrl_code - The custom control code
 IN  ctrl_ptr - Pointer to the custom control structure
 Return value:  0 for success -1 on error
 ******************************************************************************/
static int_t ostm_control(st_stream_ptr_t stream_ptr, uint32_t ctrl_code, void *ctrl_ptr)
{
    /*  */
    static pevent_t ostm_mutex = NULL;
    int_t retval = -1;

    /* The following controls require exclusive access to resource */
    R_OS_EventWaitMutex (ostm_mutex, R_OS_ABSTRACTION_PRV_EV_WAIT_INFINITE);
    switch (ctrl_code)
    {
        case CTL_OSTM_CREATE_TIMER:
        {
            /* cast */
            retval = create_timer(stream_ptr, (st_r_drv_ostm_config_t*)ctrl_ptr);
            break;
        }

        case CTL_OSTM_DESTORY_TIMER:
        {
            retval = destroy_timer(stream_ptr);
            break;
        }

        case CTL_OSTM_START_TIMER:
        {
            /* stream_ptr is being used as a uniquie_id not a structure */
            retval = start_timer(stream_ptr);
            break;
        }

        case CTL_OSTM_STOP_TIMER:
        {
            /* stream_ptr is being used as a uniquie_id not a structure */
            retval = stop_timer(stream_ptr);
            break;
        }

        default:
        {
            DEBUG_PRV_TRACE(("drvOSTM: Unknown control code\r\n"));
            break;
        }
    }

    R_OS_EventReleaseMutex (ostm_mutex);

    return (retval);
}
/******************************************************************************
 End of function  ostm_control
 ******************************************************************************/

/*******************************************************************************
 * Function Name: ostm_get_version
 * Description  : Provides build information even if driver fails to open
 *                version information is updated by developer
 * Arguments    : none
 * Return Value : 0 (never fails)
 ******************************************************************************/
static int_t ostm_get_version(st_stream_ptr_t stream_ptr, st_ver_info_t *pVerInfo)
{
    UNUSED_PARAM(stream_ptr);
    pVerInfo->hld.version.sub.major = gs_hld_info.version.sub.major;
    pVerInfo->hld.version.sub.minor = gs_hld_info.version.sub.minor;
    pVerInfo->hld.build = gs_hld_info.build;
    pVerInfo->hld.p_szdriver_name = gs_hld_info.p_szdriver_name;

    /* Obtain version information from Low layer Driver */
    R_OSTM_GetVersion (pVerInfo);

    return 0;
}
/*******************************************************************************
 End of function ostm_get_version
 ******************************************************************************/

/* End of File */

