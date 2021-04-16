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
* http://www.renesas.com/disclaimer*
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name : iodefine_cfg.h
* $Rev: 1738 $
* $Date:: 2016-08-01 17:08:06 +0100#$
* Description : Definition of I/O Register for RZ/A1H (V2.00h)
******************************************************************************/
#ifndef SRC_DRIVERS_INC_IODEFINE_CFG_H_
#define SRC_DRIVERS_INC_IODEFINE_CFG_H_

#include "iodefine_typedef.h"                         /* (V2.00h) */

#include "adc_iodefine.h"                             /* (V2.00h) */
#include "bsc_iodefine.h"                             /* (V2.00h) */
#include "ceu_iodefine.h"                             /* (V2.00h) */
#include "cpg_iodefine.h"                             /* (V2.00h) */
#if ((TARGET_RZA1 == TARGET_RZA1H) || (TARGET_RZA1 == TARGET_RZA1M))
#include "disc_iodefine.h"                            /* (V2.00h) */
#endif
#include "dmac_iodefine.h"                            /* (V2.00h) */
#if ((TARGET_RZA1 == TARGET_RZA1H) || (TARGET_RZA1 == TARGET_RZA1M))
#include "dvdec_iodefine.h"                           /* (V2.00h) */
#endif
#include "ether_iodefine.h"                           /* (V2.00h) */
#if ((TARGET_RZA1 == TARGET_RZA1H) || (TARGET_RZA1 == TARGET_RZA1M))
#include "flctl_iodefine.h"                           /* (V2.00h) */
#endif
#include "gpio_iodefine.h"                            /* (V2.00h) */
#if ((TARGET_RZA1 == TARGET_RZA1H) || (TARGET_RZA1 == TARGET_RZA1M))
#include "ieb_iodefine.h"                             /* (V2.00h) */
#endif
#include "inb_iodefine.h"                             /* (V2.00h) */
#include "intc_iodefine.h"                            /* (V2.00h) */
#include "irda_iodefine.h"                            /* (V2.00h) */
#include "jcu_iodefine.h"                             /* (V2.00h) */
#include "l2c_iodefine.h"                             /* (V2.00h) */
#if ((TARGET_RZA1 == TARGET_RZA1H) || (TARGET_RZA1 == TARGET_RZA1M))
#include "lin_iodefine.h"                             /* (V2.00h) */
#include "lvds_iodefine.h"                            /* (V2.00h) */
#include "mlb_iodefine.h"                             /* (V2.00h) */
#endif
#include "mmc_iodefine.h"                             /* (V2.00h) */
#include "mtu2_iodefine.h"                            /* (V2.00h) */
#include "ostm_iodefine.h"                            /* (V2.00h) */
#if ((TARGET_RZA1 == TARGET_RZA1H) || (TARGET_RZA1 == TARGET_RZA1M))
#include "pfv_iodefine.h"                             /* (V2.00h) */
#include "pwm_iodefine.h"                             /* (V2.00h) */
#endif
#include "riic_iodefine.h"                            /* (V2.00h) */
#if ((TARGET_RZA1 == TARGET_RZA1H) || (TARGET_RZA1 == TARGET_RZA1M))
#include "romdec_iodefine.h"                          /* (V2.00h) */
#endif
#include "rscan0_iodefine.h"                          /* (V2.00h) */
#include "rspi_iodefine.h"                            /* (V2.00h) */
#include "rtc_iodefine.h"                             /* (V2.00h) */
#include "scif_iodefine.h"                            /* (V2.00h) */
#include "scim_iodefine.h"                            /* (V2.00h) */
#include "scux_iodefine.h"                            /* (V2.00h) */
#if ((TARGET_RZA1 == TARGET_RZA1H) || (TARGET_RZA1 == TARGET_RZA1M))
#include "sdg_iodefine.h"                             /* (V2.00h) */
#endif
#include "spdif_iodefine.h"                           /* (V2.00h) */
#include "spibsc_iodefine.h"                          /* (V2.00h) */
#include "ssif_iodefine.h"                            /* (V2.00h) */
#include "usb20_iodefine.h"                           /* (V2.00h) */
#include "vdc5_iodefine.h"                            /* (V2.00h) */
#include "wdt_iodefine.h"                             /* (V2.00h) */

#endif /* SRC_DRIVERS_INC_IODEFINE_CFG_H_ */
