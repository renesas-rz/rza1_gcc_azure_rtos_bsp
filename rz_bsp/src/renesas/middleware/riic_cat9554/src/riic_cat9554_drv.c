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
/******************************************************************************
* @file           riic_cat9554_drv.c
* @brief          CAT9554 driver interface functions
* @version        1.00
* @date           24.04.2019
* H/W Platform    CAT9554 driver riic device depended functions
*****************************************************************************/
/*****************************************************************************
* History      : DD.MM.YYYY Ver. Description
*              : 24.04.2019 1.00 First Release
*****************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "mcu_board_select.h"

#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
/* nothing */
#elif (TARGET_BOARD == TARGET_BOARD_RSK)

#include    <stdio.h>
#include    <string.h>
#include    <fcntl.h>

#include 	"dev_drv.h"
#include    "riic_cat9554_drv.h"
#include    "r_devlink_wrapper.h"
#include    "r_riic_drv_sc_cfg.h"
#include 	"r_os_abstraction_api.h"

/******************************************************************************
Typedef definitions
******************************************************************************/
typedef struct {
    semaphore_t    semid;       /*!< Access Semaphore*/
    int_t          hi2c3;       /*!< Channel Handle*/
} riic_ch3_aio_ctrl_t;

/******************************************************************************
Macro definitions
******************************************************************************/
#define PX_CMD_WRITE_OUT_REG      (0x01u)
#define PX_CMD_CONFIGURATION_REG  (0x03u)

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
static riic_ch3_aio_ctrl_t s_i2c3_ctrl;

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/

/**************************************************************************//**
* Function Name: RIIC_CAT9554_Open
* @brief         Open riic channel 3
*                Description:<br>
*                Open RIIC channel 3 for access to i2c devices
* @param         none
* @retval        DEVDRV_SUCCESS    : Success
* @retval        error code  : Failure
******************************************************************************/
int32_t RIIC_CAT9554_Open(void)
{
    int_t ercd = DEVDRV_ERROR;
    st_r_drv_riic_create_t riic_clock;

    s_i2c3_ctrl.hi2c3 = DEVDRV_ERROR;
    /* open the I2C channel3 driver */
    s_i2c3_ctrl.hi2c3 = open(DEVICE_INDENTIFIER "iic3", O_RDWR);

    if (DEVDRV_ERROR == s_i2c3_ctrl.hi2c3)
    {
        ercd = DEVDRV_ERROR;
    }
    else
    {
        /* set the clock frequency for the I2C channel */
        riic_clock.frequency = RIIC_FREQUENCY_100KHZ;

        /* Create the I2C channel */
        ercd = control(s_i2c3_ctrl.hi2c3, CTL_RIIC_CREATE, &riic_clock);
    }

    return (ercd);
}
/******************************************************************************
End of function RIIC_CAT9554_Open
******************************************************************************/

/**************************************************************************//**
* Function Name: RIIC_CAT9554_Close
* @brief         Close riic channel 3
*                Description:<br>
*                Close RIIC channel 3 for access to i2c devices
* @param         none
* @retval        DEVDRV_SUCCESS    : Success
* @retval        error code  : Failure
******************************************************************************/
int32_t RIIC_CAT9554_Close(void)
{
    int_t ercd = DEVDRV_ERROR;

    /*Close Channel*/
    ercd = close(s_i2c3_ctrl.hi2c3);
    s_i2c3_ctrl.hi2c3 = DEVDRV_ERROR;

    return (ercd);
}
/******************************************************************************
End of function RIIC_CAT9554_Close
******************************************************************************/

/**************************************************************************//**
* Function Name: RIIC_CAT9554_Write
* @brief         Write register to CAT9554
*
*                Description:<br>
*
* @param         addr   :register address 0x40, 0x42
* @param         data   :register data 0-255
* @param         config :register config 0-255
* @retval        DEVDRV_SUCCESS   :Success.
* @retval        error code :Failure.
******************************************************************************/
int32_t RIIC_CAT9554_Write(const uint8_t addr, const uint8_t data, const uint8_t config)
{
    int32_t ercd = DEVDRV_ERROR;
    st_r_drv_riic_config_t i2c_write;
    uint8_t config_data = (~config);

	/* Port Expander Configuration */
    i2c_write.device_address = addr;
	i2c_write.number_of_bytes = 1u;
    i2c_write.sub_address = PX_CMD_CONFIGURATION_REG;
    i2c_write.p_data_buffer = (uint8_t *)&config_data;

	/* Write Data */
    ercd = control(s_i2c3_ctrl.hi2c3, CTL_RIIC_WRITE, &i2c_write);
	if(ercd != DEVDRV_SUCCESS){
		return DEVDRV_ERROR;
	}

	/* Port Expander Write */
    i2c_write.sub_address = PX_CMD_WRITE_OUT_REG;
    i2c_write.p_data_buffer = (uint8_t *)&data;

    /*Write Data*/
    ercd = control(s_i2c3_ctrl.hi2c3, CTL_RIIC_WRITE, &i2c_write);

    return (ercd);
}
/******************************************************************************
End of function RIIC_CAT9554_Write
******************************************************************************/

#endif /* TARGET_BOARD */
