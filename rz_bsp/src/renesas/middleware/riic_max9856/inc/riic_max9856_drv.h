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
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
 *****************************************************************************/
/******************************************************************************
 * @headerfile     riic_max9856_drv.h
 * $Date:          2019-04-24
 * @brief          MAX9856 driver header
 * @version        1.00
 * @date           24.04.2019
 * H/W Platform    Renesas Starter Kit+ for RZ/A1H
 *****************************************************************************/
 /*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 30.06.2018 1.00 First Release
 *              : 24.04.2019 2.00 Porting for RZ/A1H
 *****************************************************************************/
#ifndef RIIC_MAX9856_DRV_H
#define RIIC_MAX9856_DRV_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "mcu_board_select.h"
#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)

#include "r_typedefs.h"


/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Functions Prototypes
******************************************************************************/

/**
 * @brief    Open RIIC channel 1 for access to i2c devices.
 *
 * @retval   DEVDRV_SUCCESS: Success
 * @retval   error code:     Failure
 */
int32_t RIIC_MAX9856_Open(void);

/**
 * @brief    Close RIIC channel 1 for access to i2c devices
 *
 * @retval   DEVDRV_SUCCESS: Success
 * @retval   error code:     Failure
 */
int32_t RIIC_MAX9856_Close(void);

/**
 * @brief     Write to a  register on the MAX9856 Codec.
 *
 * @param[in] addr:    register address 0-28
 * @param[in] data:    register data 0-255
 *
 * @retval        DEVDRV_SUCCESS:  Success.
 * @retval        error code:      Failure.
 */
int32_t RIIC_MAX9856_Write(const uint8_t addr, const uint8_t data);

/**
 * @brief     Read from a register on the MAX9856 Codec.
 *
 * @param[in] addr:    register address 0-28
 * @param[in] data:    register data 0-255
 *
 * @retval        DEVDRV_SUCCESS:  Success.
 * @retval        error code:      Failure.
 */
int32_t RIIC_MAX9856_Read(const uint8_t addr, uint8_t* const p_data);

#elif (TARGET_BOARD == TARGET_BOARD_RSK)

#include "r_typedefs.h"


/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Functions Prototypes
******************************************************************************/

/**
 * @brief    Open RIIC channel 3 for access to i2c devices.
 *
 * @retval   DEVDRV_SUCCESS: Success
 * @retval   error code:     Failure
 */
int32_t RIIC_MAX9856_Open(void);

/**
 * @brief    Close RIIC channel 3 for access to i2c devices
 *
 * @retval   DEVDRV_SUCCESS: Success
 * @retval   error code:     Failure
 */
int32_t RIIC_MAX9856_Close(void);

/**
 * @brief     Write to a  register on the MAX9856 Codec.
 *
 * @param[in] addr:    register address 0-28
 * @param[in] data:    register data 0-255
 *
 * @retval        DEVDRV_SUCCESS:  Success.
 * @retval        error code:      Failure.
 */
int32_t RIIC_MAX9856_Write(const uint8_t addr, const uint8_t data);

/**
 * @brief     Read from a register on the MAX9856 Codec.
 *
 * @param[in] addr:    register address 0-28
 * @param[in] data:    register data 0-255
 *
 * @retval        DEVDRV_SUCCESS:  Success.
 * @retval        error code:      Failure.
 */
int32_t RIIC_MAX9856_Read(const uint8_t addr, uint8_t* const p_data);

#endif /* TARGET_BOARD */

#endif /* RIIC_MAX9856_DRV_H */
