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
 * File Name    : r_ostm_drv_sc_cfg.c
 * Description  : OS timer low layer driver configuration file, modified by the smart configurator.
 *                Do not edit when using smart configurator in your project as this file MAY get re-written by SC.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 09.12.2017 1.00     First Release
 **********************************************************************************************************************/

/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
#include "r_devlink_wrapper.h"
#include "control.h"
#include "r_ostm_drv_api.h"
#include "r_ostm_drv_sc_cfg.h"
#include "r_ostm_drv_sc_private.h"

/* Common configuration file for all ISR's */
#include "r_task_priority.h"

/* SHOULD BE REMOVED WHEN INTC DRIVER IS IN PLACE */
#include "r_intc.h"


/* sc needs to know the names of the channels to config this driver */
#include "ostm_iodefine.h"
#include "ostm_iobitmask.h"

/* sc needs to know the names of the cpg module required to config this driver */
#include "cpg_iobitmask.h"
#include "cpg_iodefine.h"

/***********************************************************************************************************************
 Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 Typedef definitions
 **********************************************************************************************************************/

#if R_OSTM_DRV_SC_PRV_REFERENCE_ONLY

 /* channel 0 of the peripheral is supported in this driver */
 { R_CH0,  OSTM_MODE_INTERVAL, 30, &OSTM0,  &CPG.STBCR5, CPG_STBCR5_MSTP51_SHIFT, CPG_STBCR5_MSTP51, OSTM0_OSTMnCTL_MD0_SHIFT, OSTM0_OSTMnCTL_MD0, INTC_ID_OSTM0TINT },

 /* channel 1 of the peripheral is supported in this driver */
 { R_CH1,  OSTM_MODE_INTERVAL, 5, &OSTM1,  &CPG.STBCR5, CPG_STBCR5_MSTP50_SHIFT, CPG_STBCR5_MSTP50, OSTM0_OSTMnCTL_MD1_SHIFT, OSTM0_OSTMnCTL_MD1, INTC_ID_OSTM1TINT },

#endif

/***********************************************************************************************************************
 Exported global variables (to be accessed by other files)
 **********************************************************************************************************************/
//todo
const st_r_drv_ostm_lld_ch_config_t g_r_ostm_sc_ch_config[R_CFG_OSTM_DRV_CHANNELS_AVAIL_PRV] =
{
 /* channel 0 of the peripheral is supported in this driver */
 { R_CH0,  ISR_OSTM0_PRIORITY, &OSTM0,  &CPG.STBCR5, CPG_STBCR5_MSTP51_SHIFT, CPG_STBCR5_MSTP51, OSTM0_OSTMnCTL_MD0_SHIFT, OSTM0_OSTMnCTL_MD0, INTC_ID_OSTM0TINT },

 /* channel 1 of the peripheral is supported in this driver */
 { R_CH1,  0, &OSTM1,  &CPG.STBCR5, CPG_STBCR5_MSTP50_SHIFT, CPG_STBCR5_MSTP50, OSTM0_OSTMnCTL_MD1_SHIFT, OSTM0_OSTMnCTL_MD1, INTC_ID_OSTM1TINT },
};

st_r_drv_ostm_lld_config_t g_r_ostm_sc_config = {};

/***********************************************************************************************************************
 Private global variables and functions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: initialise_ostm_config_from_sc
 * Description  : initialise function to configure the ostm driver as described by the smart configurator (sc)
 *                This function is called by the low layer driver during the driver initialisation
 * Arguments    : none
 * Return Value : none
 **********************************************************************************************************************/
void initialise_ostm_config_from_sc(void)
{
    int_t ndx = ((sizeof(g_r_ostm_sc_ch_config)) / (sizeof(st_r_drv_ostm_lld_ch_config_t)));

    /* reset dynamic data members */
    g_r_ostm_sc_config.ch_avaliable = 0;
    g_r_ostm_sc_config.ch_supported = 0;

    while(ndx--)
    {
        g_r_ostm_sc_config.ch_avaliable++;
        g_r_ostm_sc_config.ch_supported += g_r_ostm_sc_ch_config[ndx].channel;
    }
}
/***********************************************************************************************************************
 End of function initialise_ostm_config_from_sc
 **********************************************************************************************************************/
