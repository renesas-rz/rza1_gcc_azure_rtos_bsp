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
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/**************************************************************************//**
* @file          riic_cat9554_if.h
* $Rev:          1.0
* $Date:         24.04.2019
* @brief         CAT9554 driver API header
******************************************************************************/

#ifndef RIIC_CAT9554_IF_H
#define RIIC_CAT9554_IF_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "mcu_board_select.h"

#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
/* nothing */
#elif (TARGET_BOARD == TARGET_BOARD_RSK)

#include "r_typedefs.h"
#include "r_errno.h"
#include "riic_cat9554_drv.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/******************************************************************************
Macro definitions
******************************************************************************/
/* Device Address */
#define CAT9554_I2C_PX1      (0x40)
#define CAT9554_I2C_PX2      (0x42)

/* Port Expander 1 */
#define PX1_LED1             (0x01)
#define PX1_LED2             (0x02)
#define PX1_LED3             (0x04)
#define PX1_NOR_A25          (0x08)
#define PX1_PMOD1_RST        (0x10)
#define PX1_PMOD2_RST        (0x20)
#define PX1_SD_CONN_PWR_EN   (0x40)
#define PX1_SD_MMC_PWR_EN    (0x80)

/* Port Expander 2 */
#define PX2_PX1_EN0          (0x01)
#define PX2_PX1_EN1          (0x02)
#define PX2_TFT_CS           (0x04)
#define PX2_PX1_EN3          (0x08)
#define PX2_USB_OVR_CUR      (0x10)
#define PX2_USB_PWR_ENA      (0x20)
#define PX2_USB_PWR_ENB      (0x40)
#define PX2_PX1_EN7          (0x80)

/******************************************************************************
Functions Prototypes
******************************************************************************/

/**
 * @brief   Open RIIC CAT9554 and initialize synchronous resource
 *
 * @retval        DEVDRV_SUCCESS: Success.
 * @retval        error code:     Failure.
 */
extern int32_t R_RIIC_CAT9554_Open(void);

/**
 * @brief    Close RIIC CAT9554 and free synchronous resource
 *
 * @retval        DEVDRV_SUCCESS: Success.
 * @retval        error code:     Failure.
 */
extern int32_t R_RIIC_CAT9554_Close(void);

/**
 * @brief         Write register to CAT9554
 *
 * @param[in] addr:    register address 0x40, 0x42
 * @param[in] data:    register data 0-255
 * @param[in] config:  register config 0-255
 *
 * @retval        DEVDRV_SUCCESS:   Success.
 * @retval        error code:       Failure.
 */
extern int32_t R_RIIC_CAT9554_Write(const uint8_t addr, const uint8_t data, const uint8_t config);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TARGET_BOARD */

#endif /* RIIC_CAT9554_IF_H */

