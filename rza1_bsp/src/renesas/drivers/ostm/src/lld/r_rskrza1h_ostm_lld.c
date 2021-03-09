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
 * File Name   : r_rskrza1h_ostm_lld.c
 * Description : OS timer driver (lower layer )
 *******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

#include <stdio.h>

/* hardware access includes */
#include "iodefine_typedef.h"
#include "rza_io_regrw.h"

#include "r_compiler_abstraction_api.h"

#include "dev_drv.h"

#include "cpg_iodefine.h"
#include "gpio_iodefine.h"
#include "intc_iodefine.h"
#include "ostm_iodefine.h"

#include "cpg_iobitmask.h"
#include "gpio_iobitmask.h"
#include "intc_iobitmask.h"
#include "ostm_iobitmask.h"

/* SHOULD BE REMOVED WHEN INTC DRIVER IS IN PLACE */
#include "r_intc.h"

/* end of hardware access includes */

#include "r_devlink_wrapper.h"
#include "r_ostm_drv_api.h"
#include "r_ostm_drv_sc_private.h"

/* Comment this line out to turn ON module trace in this file */
#undef _TRACE_ON_

#ifndef _TRACE_ON_
#undef DEBUG_PRV_TRACE
#define  DEBUG_PRV_TRACE(x)
#endif

/******************************************************************************
 Imported global variables and functions (from other files)
 ******************************************************************************/

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/

static const st_drv_info_t gs_lld_info =
{
    {
        ((STDIO_OSTM_RZ_LLD_VERSION_MAJOR << 16) + STDIO_OSTM_RZ_LLD_VERSION_MINOR)
    },
    STDIO_OSTM_RZ_LLD_BUILD_NUM,
    STDIO_OSTM_RZ_LLD_DRV_NAME
};


/******************************************************************************
 Exported global variables and functions (to be accessed by other files)
 ******************************************************************************/
bool_t gs_r_drv_ostm_open[R_CFG_OSTM_DRV_CHANNELS_AVAIL_PRV] = {};

/***********************************************************************************************************************
 * Function Name: ostm_start
 * Description  : Executes the register setting to start the timer counter.
 * Arguments    : volatile struct st_ostm * ostm : Start address of OSTM register
 *              :                                : by channel
 * Return Value : none
 ***********************************************************************************************************************/
static void ostm_start(volatile struct st_ostm * ostm)
{
    ostm->OSTMnTS = 0x01u; /* Start counting */
}
/***********************************************************************************************************************
 End of function ostm_start
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: ostm_stop
 * Description  : Executes the register setting to stop the timer counter.
 * Arguments    : volatile struct st_ostm * ostm : Start address of OSTM register
 *              :                                : by channel
 * Return Value : none
 ***********************************************************************************************************************/
static void ostm_stop(volatile struct st_ostm * ostm)
{
    ostm->OSTMnTT = 0x01u; /* Stop counting      */
}
/***********************************************************************************************************************
 End of function ostm_stop
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: ostm_read_cnt
 * Description  : reads the value of CNT for the specified channel of the ostm.
 * Arguments    : volatile struct st_ostm * ostm : Start address of OSTM register
 *              :                                : by channel
 * Return Value : none
 ***********************************************************************************************************************/
static uint32_t ostm_read_cnt(volatile struct st_ostm * ostm)
{
    return ostm->OSTMnCNT;
}
/***********************************************************************************************************************
 End of function ostm_read_cnt
 **********************************************************************************************************************/


/***********************************************************************************************************************
 * Function Name: ostm_init
 * Description  : Initialise the OSTM channel 0
 *              : using the timer cycle specified by the argument cycle in the operating mode specified by the argument
 *              : mode. In this sample code, the timer count value is set assuming that the P0 clock is set to
 *              : 33.33MH. The initial setting for the use of the OSTM0 interrupt is also executed.
 * Arguments    : res              : OSTM channel (0 or 1 supported for this driver)
 *                config_ptr       : channel configuration
 * Return Value : DRV_SUCCESS      : Success to initialise OSTM
 *              : DRV_ERROR        : Failure to initialise OSTM
 **********************************************************************************************************************/
static int32_t ostm_init(uint32_t res, st_r_drv_ostm_config_t *config_ptr)
{
    volatile uint8_t dummy_buf = 0;
    bool_t set_config = true;
    int32_t retval = DRV_SUCCESS;

    /* variable needed but not used to enforce read requirement of peripheral */
    (void) dummy_buf;

    /* ==== Argument check ==== */
    /* cast ok as only checking for non-zero value, not using the ptr */
    if ((OSTM_MODE_INTERVAL == config_ptr->mode) && (NULL == config_ptr->callback_fn))
    {
        printf ("no callback defined\r\n");
        set_config = false;
    }

    if (set_config)
    {
        /* ==== Module standby clear ==== */
        /* ---- Supply clock to the OSTM( ---- */
        rza_io_reg_write_8 (g_r_ostm_sc_ch_config[res].cpg_reg, 0, g_r_ostm_sc_ch_config[res].cpg_shift,
                            g_r_ostm_sc_ch_config[res].cpg_mask);

        /* Dummy read cast is ok as value is ignored */
        dummy_buf = (volatile uint8_t) *g_r_ostm_sc_ch_config[res].cpg_reg;

        /* Stop counting */
        /* ---- OSTM count stop trigger register (TT) setting ---- */
        /* cast is safe as ostm is TRUSTED to reference a structure of type st_ostm */
        (*(struct st_ostm *) g_r_ostm_sc_ch_config[res].ostm).OSTMnTT = 0x01u;

        /* ---- OSTM control register (CTL) setting ---- */
        /* Counter operating mode                */
        rza_io_reg_write_8 (&((*(struct st_ostm *) g_r_ostm_sc_ch_config[res].ostm).OSTMnCTL),
                            config_ptr->mode,
                            g_r_ostm_sc_ch_config[res].ostm_ctl_mdx_shift,
                            g_r_ostm_sc_ch_config[res].ostm_ctl_mdx_mask);

        /* Determine  Mode */
        switch (config_ptr->mode)
        {
            case OSTM_MODE_INTERVAL:
            {
                /* ---- OSTM compare register (CMP) setting ---- */
                /* Set timer in the cycle specified by the cycle */
                /* cast is safe as ostm is TRUSTED to reference a structure of type st_ostm */
                (*(struct st_ostm *) g_r_ostm_sc_ch_config[res].ostm).OSTMnCMP =
                 config_ptr->frequency;

                /* Reset the counter */
                /* cast is safe as ostm is TRUSTED to reference a structure of type st_ostm */
                (*(struct st_ostm *) g_r_ostm_sc_ch_config[res].ostm).OSTMnCNT = 0;

                /* Only configure the associated interrupt if it is required. */
                /* cast is ok as the data is only used as a id and not an abject */
                if (NULL != config_ptr->callback_fn)
                {
                    /* ==== Register OS timer interrupt handler ==== */
                    R_INTC_RegistIntFunc (g_r_ostm_sc_ch_config[res].isr_id, config_ptr->callback_fn);

                    /* ==== Set priority of OS timer interrupt ==== */
                    R_INTC_SetPriority (g_r_ostm_sc_ch_config[res].isr_id,
                                        (uint8_t)g_r_ostm_sc_ch_config[res].isr_priority);

                    /* ==== Validate OS timer interrupt ==== */
                    R_INTC_Enable (g_r_ostm_sc_ch_config[res].isr_id);
                }

                break;
            }

            case OSTM_MODE_COMPARE:
            {
                /* ---- OSTM clear compare register (CMP) setting ---- */
                (*(struct st_ostm *) g_r_ostm_sc_ch_config[res].ostm).OSTMnCMP = 0;

                /* ==== Disabled OS timer interrupt ==== */
                R_INTC_Disable(g_r_ostm_sc_ch_config[res].isr_id);

                break;
            }

            default:
            {
                retval = DRV_ERROR;
            }
        }
    }
    return retval;
}
/***********************************************************************************************************************
 End of function ostm_init
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: ostm_uninit
 * Description  : un-initialise a channel of the driver
 * Arguments    : res              : OSTM channel (0 or 1 supported for this driver)
 * Return Value : none
 **********************************************************************************************************************/
static void ostm_uninit(uint32_t res)
{
    volatile uint8_t dummy_buf = 0;

    /* ---- Remove clock to the OSTM Channel ---- */
    rza_io_reg_write_8 (g_r_ostm_sc_ch_config[res].cpg_reg, 0, g_r_ostm_sc_ch_config[res].cpg_shift,
                        g_r_ostm_sc_ch_config[res].cpg_mask);

    /* Dummy read cast is ok as value is ignored */
    dummy_buf = (volatile uint8_t) *g_r_ostm_sc_ch_config[res].cpg_reg;
    (void) dummy_buf;
}
/***********************************************************************************************************************
 End of function ostm_uninit
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_OSTM_InitialiseHwIf
 * Description  : LowLevel driver initialise interface function
 * Arguments    : none
 * Return value : DRV_SUCCESS : Success when attempting to initialise OSTM
 *                DRV_ERROR   : Failure to initialise OSTM
 *********************************************************************************************************************/
int_t R_OSTM_InitialiseHwIf(void)
{
    int_t res = DRV_SUCCESS;

    return (res);
}
/**********************************************************************************************************************
 End of function R_OSTM_InitialiseHwIf
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_OSTM_UninitialiseHwIf
 * Description  : LowLevel driver un-initialise interface function
 * Arguments    : none
 * Return Value : none
 **********************************************************************************************************************/
void R_OSTM_UninitialiseHwIf(void)
{
    //R_COMPILER_Nop();
}
/*******************************************************************************
 End of function R_OSTM_UninitialiseHwIf
 ******************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_OSTM_Open
 * Description  : Starts the OSTM timer counter specified by the argument channel.
 * Arguments    : res         : OSTM channel (0 or 1 supported for this driver)
 * Return Value : DRV_SUCCESS : Success to start counting OSTM
 *              : DRV_ERROR   : Failure to start counting OSTM
 **********************************************************************************************************************/
int_t R_OSTM_Open(uint32_t res, st_r_drv_ostm_config_t *config_ptr)
{
    int_t ret = DRV_ERROR;

    /* ---- Supply clock to the OSTM Channel ---- */
     rza_io_reg_write_8 (g_r_ostm_sc_ch_config[res].cpg_reg,
                         1,
                         g_r_ostm_sc_ch_config[res].cpg_shift,
                         g_r_ostm_sc_ch_config[res].cpg_mask);

     /* Check to see if the use wants to override the default sc configuration ? */
     if(NULL != config_ptr)
     {
         ret = ostm_init (res, config_ptr);
     }
     return (ret);
}
/***********************************************************************************************************************
 End of function R_OSTM_Open
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_OSTM_Close
 * Description  : Starts the OSTM timer counter specified by the argument channel.
 * Arguments    : res         : OSTM channel (0 or 1 supported for this driver)
 * Return Value : DRV_SUCCESS : Success to start counting OSTM
 *              : DRV_ERROR   : Failure to start counting OSTM
 **********************************************************************************************************************/
int_t   R_OSTM_Close(uint32_t res)
{
    int_t ret = DRV_ERROR;

      ostm_uninit(res);

    ret = DRV_SUCCESS;

    return (ret);

}
/***********************************************************************************************************************
 End of function R_OSTM_Close
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_OSTM_Start
 * Description  : Starts the timer
 * Arguments    : res         : OSTM channel (0 or 1 supported for this driver)
 * Return Value : DRV_SUCCESS : Success to start counting OSTM
 *              : DRV_ERROR   : Failure to start counting OSTM
 **********************************************************************************************************************/
int_t R_OSTM_Start(int_t res)
{
    int_t ret = DRV_ERROR;

    ostm_start (g_r_ostm_sc_ch_config[res].ostm);

    ret = DRV_SUCCESS;

    return ret;
}
/***********************************************************************************************************************
 End of function R_OSTM_Start
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_OSTM_Stop
 * Description  : Stops the timer
 * Arguments    : int_t res        : OSTM channel (0 or 1)
 * Return Value : DRV_SUCCESS      : Success to start counting OSTM
 *              : DRV_ERROR        : Failure to start counting OSTM
 **********************************************************************************************************************/
int_t R_OSTM_Stop(int_t res)
{
    int_t ret = DRV_ERROR;

    ostm_stop (g_r_ostm_sc_ch_config[res].ostm);

    ret = DRV_SUCCESS;

    return ret;
}
/***********************************************************************************************************************
 End of function R_OSTM_Stop
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_OSTM_ReadCnt
 * Description  : Reads the OSTMnCNT register of the specified channel
 * Arguments    : int_t res        : OSTM channel (0 or 1)
 *                uint8_t* val     : value of the OSTMnCNT register for the specified channel 'n'
 * Return Value :                  : data from the CNT register
 **********************************************************************************************************************/
int_t R_OSTM_ReadCnt(int_t res, uint32_t* val)
{
    int_t ret = DRV_ERROR;

    *val = ostm_read_cnt (g_r_ostm_sc_ch_config[res].ostm);
    ret = DRV_SUCCESS;

    return ret;
}
/***********************************************************************************************************************
 End of function R_OSTM_ReadCnt
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_OSTM_GetVersion
 * Description  :
 * Arguments    : pinfo - detailed driver information structure
 * Return Value : 0 can not fail
 **********************************************************************************************************************/
int32_t R_OSTM_GetVersion(st_ver_info_t *pinfo)
{
    pinfo->lld.version.sub.major = gs_lld_info.version.sub.major;
    pinfo->lld.version.sub.minor = gs_lld_info.version.sub.minor;
    pinfo->lld.build = gs_lld_info.build;
    pinfo->lld.p_szdriver_name = gs_lld_info.p_szdriver_name;
    return (0);

}
/**********************************************************************************************************************
 End of function R_OSTM_GetVersion
 **********************************************************************************************************************/

/* End of File */

