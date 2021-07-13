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
 * @headerfile     r_devlink_wrapper_cfg.h
 * @brief          Define and Configure driver sub system
 * @version        1.00
 * @date           27.06.2018
 *****************************************************************************/
/*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 30.06.2018 1.00 First Release
 *****************************************************************************/

/* Multiple inclusion prevention macro */
#ifndef RENESAS_CONFIGURATION_R_DEVLINK_WRAPPER_CFG_H_
#define RENESAS_CONFIGURATION_R_DEVLINK_WRAPPER_CFG_H_

/* Modified by sc do not manually edit */
/* End of sc modification */

#include "compiler_settings.h"

/* Modified by user please add your drivers here */
#include "mcu_board_select.h"
#include "r_sc_cfg.h"
#include "r_ostm_drv_api.h"
#include "r_riic_drv_api.h"
#include "r_sci_drv_api.h"
#include "r_led_drv_api.h"
#include "r_rtc_drv_api.h"
#include "r_wdt_drv_api.h"
#include "r_eeprom_drv_api.h"

#include "r_dmac_drv_api.h"

#if R_SELF_LOAD_MIDDLEWARE_USB_HOST_CONTROLLER
#if (TARGET_BOARD == TARGET_BOARD_RSK)
#include "r_usb1_drv_api.h"
#elif (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
#include "r_usb0_drv_api.h"
#endif
#include "fileDriver.h"
#include "drvStdIn.h"
#endif

#if R_SELF_INSERT_APP_CDC_SERIAL_PORT
#include "hwusbf_cdc_rskrza1_0.h"
#endif

#if R_SELF_INSERT_APP_HID_MOUSE
#include "hwusbf_hid_rskrza1_0.h"
#endif

#if R_SELF_INSERT_APP_PMOD
#include "r_pmod_lcd_drv_api.h"
#endif

#if R_SELF_LOAD_MIDDLEWARE_ETHERNET_MODULES
#include "r_eeprom_drv_api.h"
#include "drvEthernet.h"
#include "drvFileSocket.h"
#include "drvSocket.h"
#endif

#if R_SELF_INSERT_APP_SOUND
#include "r_ssif_drv_api.h"
#endif


/* End of user modification */

/* Define the static "Mount Table" in which a device is associated with a
   link name */
/** mount table element */
typedef const struct _mount_table_t
{
    const char *    p_stream_name;
    st_r_driver_t * p_device_driver;
    const uint32_t  sc_config_index;
} st_mount_table_t;


/* Mount table should only be access directly by this file */
/** Main driver mounting table */
static st_mount_table_t gs_mount_table[] =
{

 /* Modified by SC do not manually edit */

 /* End of SC modification */

 /* Modified by user, drivers that are not under the control of sc added here */

#if (TARGET_BOARD == TARGET_BOARD_RSK)
 /** SCI Channel 3 Driver added by USER */
   {"stdin", (st_r_driver_t *)&g_scif_driver, R_SC2},

/** SCI Channel 3 Driver added by USER */
   {"stdout", (st_r_driver_t *)&g_scif_driver, R_SC2},

/** SCI Channel 3 Driver added by USER */
   {"stderr", (st_r_driver_t *)&g_scif_driver, R_SC2},
#elif (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
 /** SCI Channel 3 Driver added by USER */
   {"stdin", (st_r_driver_t *)&g_scif_driver, R_SC3},

/** SCI Channel 3 Driver added by USER */
   {"stdout", (st_r_driver_t *)&g_scif_driver, R_SC3},

/** SCI Channel 3 Driver added by USER */
   {"stderr", (st_r_driver_t *)&g_scif_driver, R_SC3},
#endif

/** OSTM Channel 0 Driver added by USER */
   {"ostm0",    (st_r_driver_t *)&g_ostm_driver, R_SC0},

/** OSTM Channel 1 Driver added by USER */
   {"ostm1",    (st_r_driver_t *)&g_ostm_driver, R_SC1},

 /** LED Driver added by USER */
   {"led", (st_r_driver_t *)&g_stdio_led_driver, R_SC0},

   /** IIC Channel 0 Driver added by USER */
   {"iic0", (st_r_driver_t *)&g_riic_driver, R_SC0},

   /** IIC Channel 1 Driver added by USER */
   {"iic1", (st_r_driver_t *)&g_riic_driver, R_SC1},

#if (TARGET_BOARD == TARGET_BOARD_RSK)
   /** IIC Channel 3 Driver added by USER */
   {"iic3", (st_r_driver_t *)&g_riic_driver, R_SC3},

#endif

   /** RTC driver added by USER */
   {"rtc", (st_r_driver_t *)&g_rtc_driver, R_SC0},

   /** RTC driver added by USER */
   {"wdt", (st_r_driver_t *)&g_wdt_driver, R_SC0},

   /** SSIF DMA driver added by USER */
   {"dma_ssif_wr", (st_r_driver_t *)&g_dmac_driver, R_SC0},
   {"dma_ssif_rd", (st_r_driver_t *)&g_dmac_driver, R_SC1},


#if R_SELF_INSERT_APP_PMOD
   /** PMOD driver added by USER */
   {"pmod okaya", (st_r_driver_t *)&g_pmod_okaya_lcd_driver, R_SC0},
#endif

#if R_SELF_LOAD_MIDDLEWARE_USB_HOST_CONTROLLER
   /** FATFS driver added by USER */
   {"fatfs", (st_r_driver_t *)&gFatfsDriver, R_SC0},

#if (TARGET_BOARD == TARGET_BOARD_RSK)
   /** USB1 driver added by USER */
   {"usb1", (st_r_driver_t *)&g_usb1_host_driver, R_SC1},
#elif (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
   /** USB0 driver added by USER */
   {"usb0", (st_r_driver_t *)&g_usb0_host_driver, R_SC0},
#endif

   /** stdin driver added by USER */
   {"usb_std_in", (st_r_driver_t *)&gUsbStdInDriver, R_SC0},
#endif

#if R_SELF_INSERT_APP_CDC_SERIAL_PORT
   /** USBF0 cdc driver added by USER */
   {"usbf0_cdc",  (st_r_driver_t *)&g_usbf0_cdc_driver, R_SC0},
#endif

#if R_SELF_INSERT_APP_HID_MOUSE
   /** USBF0 hid driver added by USER */
   {"usbf0_hid",  (st_r_driver_t *)&g_usbf0_hid_driver, R_SC0},
#endif

#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
   /** EEPROM driver added by USER */
   {"eeprom", (st_r_driver_t *)&g24C04Driver, R_SC0},
#elif (TARGET_BOARD == TARGET_BOARD_RSK)
   /** EEPROM driver added by USER */
   {"eeprom", (st_r_driver_t *)&gR1EX24016Driver, R_SC3},
#endif /* TARGET_BOARD */

#if R_SELF_LOAD_MIDDLEWARE_ETHERNET_MODULES
   /** ethernet driver added by USER */
   {"ether0", (st_r_driver_t *)&gEtherCDriver, R_SC0},

   /** lwip socket driver added by USER */
   {"lwip socket", (st_r_driver_t *)&gSocketDriver, R_SC0},

   /** lwip socket driver added by USER */
   {"lwip fsocket", (st_r_driver_t *)&gFileSocketDriver, R_SC0},
#endif

#if R_SELF_INSERT_APP_SOUND
   /** ssif serial sound driver added by USER */
   {"ssif", (st_r_driver_t *)&g_ssif_driver, R_SC0},
#endif

 /* End of user modification */

};

extern const st_r_driver_t gFileDriver;

/**
 * @brief Returns address of dummy driver configuration, used
 * @return    dummy driver configuration
 **/
st_r_driver_t * get_no_driver(void);


#endif /* RENESAS_CONFIGURATION_R_DEVLINK_WRAPPER_CFG_H_ */
