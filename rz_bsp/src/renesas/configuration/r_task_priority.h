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
 * @headerfile     r_task_priority.h
 * @brief          The task priority definitions.
 *                 Priorities set in this file have no meaning in OSLess system
 * @version        1.00
 * @date           27.06.2018
 *****************************************************************************/
/*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 30.06.2018 1.00 First Release
 *****************************************************************************/
#include "mcu_board_select.h"

/* Multiple inclusion prevention macro */
#ifndef TASKPRIORITY_H_INCLUDED
#define TASKPRIORITY_H_INCLUDED

#define NUMBER_OF_HEAPS              (1)

#define R_REGION_LARGE_CAPACITY_RAM  (78957)
#define R_REGION_UNCACHED_RAM        (54882)

#define TC_SOFT_ISR_PRIORITY         (10)

/* This is designed to soak CPU time but because it is low priority
   responsiveness of the system is not lost */

/** Hardware Interrupts 0-Highest .. 31-Lowest */
#define R_HARDWARE_API_ISR_PRIORITY (25)

/** priority of Touch Panel interrupts, used in r_touch_capacitive.c */
#define ISR_TP_IRQ_PRIORITY        (R_HARDWARE_API_ISR_PRIORITY + 5)

/** priority of JCU interrupts, used in jcu_pl.c */
#define ISR_JCU_IRQ_PRIORITY       (R_HARDWARE_API_ISR_PRIORITY + 5)

/** priority of VDC interrupts, used in jcu_pl.c */
#define ISR_VDC_IRQ_PRIORITY       (R_HARDWARE_API_ISR_PRIORITY + 5)

/** priority of SSIF interrupts, used in r_ssif_drv_api.c */
#define ISR_SSIF_IRQ_PRIORITY      (R_HARDWARE_API_ISR_PRIORITY + 5)

/** OS Timer Interrupt priority configured in r_ostm_drv_sc_cfg.c  */
#define ISR_OSTM0_PRIORITY         (R_HARDWARE_API_ISR_PRIORITY + 5 )

/** iic0 driver interrupts, used in r_rskrza1h_riic_lld.c */
#define ISR_IIC0_PRIORITY          (R_HARDWARE_API_ISR_PRIORITY + 4)

/** iic1 driver interrupts, used in r_rskrza1h_riic_lld.c */
#define ISR_IIC1_PRIORITY          (R_HARDWARE_API_ISR_PRIORITY + 4)

#if (TARGET_BOARD == TARGET_BOARD_RSK)
/** iic3 driver interrupts, used in r_rskrza1h_riic_lld.c */
#define ISR_IIC3_PRIORITY          (R_HARDWARE_API_ISR_PRIORITY + 4)
#elif (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
#define ISR_IIC3_PRIORITY          (R_HARDWARE_API_ISR_PRIORITY + 4)
#endif

/** USB Host must have higher priority than timer for enumerator */
#define ISR_USBH_PRIORITY          (R_HARDWARE_API_ISR_PRIORITY + 3)

/** priority of DMA transfer end interrupts, used in dma.c */
#define ISR_DMA_TEND_IRQ_PRIORITY  (R_HARDWARE_API_ISR_PRIORITY + 3)

/** priority of RSPI interrupts, used in rspi.h */
#define ISR_RSPI_BASE_IRQ_PRIORITY (R_HARDWARE_API_ISR_PRIORITY + 2)

/** USB DMA interrupt, used in hwDmaIf.h */
#define ISR_USBH_DMA_PRIORITY      (R_HARDWARE_API_ISR_PRIORITY + 2)

#if (TARGET_BOARD == TARGET_BOARD_RSK)
/** SCI2 driver interrupts, used in r_sci_drv_api.c */
#define ISR_SCIF2_PRIORITY         (R_HARDWARE_API_ISR_PRIORITY + 1)
#elif (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
/** SCI3 driver interrupts, used in r_sci_drv_api.c */
#define ISR_SCIF3_PRIORITY         (R_HARDWARE_API_ISR_PRIORITY + 1)
#endif

/** priority of DMA Error interrupts, used in dma.c */
#define ISR_DMA_ERR_IRQ_PRIORITY   (R_HARDWARE_API_ISR_PRIORITY + 1)

/** priority of USB Function HID interrupts */
#define ISR_USBF_HID_IRQ_PRIORITY  (R_HARDWARE_API_ISR_PRIORITY + 1)

/** Ethernet peripheral interrupt used in r_ether.c */
#define ISR_ETHER_PRIORITY         (R_HARDWARE_API_ISR_PRIORITY + 0)

/** priority of interrupt TGI4A HR Timer for switch module */
#define ISR_MTU2_TGI4A_PRIORITY    (R_HARDWARE_API_ISR_PRIORITY + 0)

/** priority of interrupt TGI3A HR timer for USB Function Module */
#define ISR_MTU2_TGI3A_PRIORITY    (R_HARDWARE_API_ISR_PRIORITY + 0)

/** priority of switch interrupt */
#define ISR_SWITCH_IRQ_PRIORITY    (R_HARDWARE_API_ISR_PRIORITY + 0)

/** priority of ADC interrupts, used in r_adc.c */
#define ISR_ADC_IRQ_PRIORITY       (R_HARDWARE_API_ISR_PRIORITY + 0)

/** priority of CEU interrupts, used in r_ceu_pl.c */
#define ISR_CEU_IRQ_PRIORITY       (R_HARDWARE_API_ISR_PRIORITY + 0)

/** priority of USB Function interrupts, used in usb_common.h */
#define ISR_USBF_IRQ_PRIORITY      (R_HARDWARE_API_ISR_PRIORITY + 0)

/* Software interrupt tasks - Give each task an individual priority */
/* The higher the number, the higher priority for software tasks */
/* These definitions are not used in an non-OS environment */

/* Software Task Priorities 1-19 (TC_SOFT_ISR_PRIORITY - 9) to (TC_SOFT_ISR_PRIORITY + 9) */

#define R_TASK_NUMBER_OF_PRIORITIES (TC_SOFT_ISR_PRIORITY + 10)

#define R_OS_TASK_MAIN_TASK_PRI     (6) /* Application main priority */

#define TASK_GRAPHICS_TASK_PRI      (R_OS_TASK_MAIN_TASK_PRI + 1)
#define TASK_CONSOLE_TASK_PRI       (R_OS_TASK_MAIN_TASK_PRI + 1)
#define TASK_DISK_MANAGER_PRI       (TC_SOFT_ISR_PRIORITY - 9)
#define TASK_USB_ENMERATOR_PRI      (R_OS_TASK_MAIN_TASK_PRI - 1)
#define TASK_TCP_IP_CONSOLE_PRI     (R_OS_TASK_MAIN_TASK_PRI - 1)
#define TASK_USB_CONSOLE_PRI        (R_OS_TASK_MAIN_TASK_PRI - 1)
#define TASK_USB_KEYBOARD_STD_PRI   (R_OS_TASK_MAIN_TASK_PRI - 1)
#define TASK_HID_KEYBOARD_PRI       (R_OS_TASK_MAIN_TASK_PRI - 1)

#define TASK_ETHERC_LINK_MON_PRI    (TC_SOFT_ISR_PRIORITY + 3)
#define TASK_ETHERC_INPUT_PRI       (TC_SOFT_ISR_PRIORITY - 1)
#define TASK_ETHERC_OUTPUT_PRI      (TC_SOFT_ISR_PRIORITY - 1)
#define TASK_LWIP_MAIN_PRI          (TC_SOFT_ISR_PRIORITY - 1)
#define TASK_WEB_SERVER_PRI         (TC_SOFT_ISR_PRIORITY - 4)
#define TASK_UDP_IP_CONSOLE_PRI     (TC_SOFT_ISR_PRIORITY - 6)
#define TASK_PMOD_APP_PRI           (TC_SOFT_ISR_PRIORITY - 9)
#define TASK_MAC_ERROR_FLASH_PRI    (TC_SOFT_ISR_PRIORITY - 9)
#define TASK_TELNET_MON_PRI         (TC_SOFT_ISR_PRIORITY - 9)
#define TASK_UDP_IP_LINK_MON_PRI    (TC_SOFT_ISR_PRIORITY - 9)
#define TASK_RESOURCE_MONITOR_PRI   (TC_SOFT_ISR_PRIORITY - 9)
#define TASK_HID_MOUSE_PRI          (TC_SOFT_ISR_PRIORITY - 9)
#define TASK_WR_PERF_TEST_PRI       (TC_SOFT_ISR_PRIORITY - 9)
#define TASK_BLINK_TASK_PRI         (TC_SOFT_ISR_PRIORITY - 9)
#define TASK_SWITCH_TASK_PRI        (TC_SOFT_ISR_PRIORITY - 9)
#define TASK_PLAY_SOUND_APP_PRI     (TC_SOFT_ISR_PRIORITY - 9)
#define TASK_RECORD_SOUND_APP_PRI   (TC_SOFT_ISR_PRIORITY - 9)

#endif /* TASKPRIORITY_H_INCLUDED */

/******************************************************************************
End  Of File
******************************************************************************/
