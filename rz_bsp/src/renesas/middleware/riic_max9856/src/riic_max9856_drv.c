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
* Copyright (C) 2017 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/**************************************************************************//**
* @file         riic_ch1_drv.c
* $Rev: 8700 $
* $Date:: 2018-06-29 15:46:04 +0100#$
* @brief        riic_ch1 driver riic device depended functions
******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "mcu_board_select.h"
#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)

#include    <stdio.h>
#include    <string.h>
#include    <fcntl.h>

//#include "ioif_public.h"
//#include "riic_if.h"
//#include "r_camera_if.h"

#include 	"dev_drv.h"
#include    "riic_max9856_drv.h"
#include    "r_devlink_wrapper.h"
#include    "r_riic_drv_sc_cfg.h"
#include 	"r_os_abstraction_api.h"

/******************************************************************************
Typedef definitions
******************************************************************************/
typedef struct {
    semaphore_t    semid;       /*!< Access Semaphore*/
    int_t          hi2c1;       /*!< Channel Handle*/
} riic_ch1_aio_ctrl_t;

/******************************************************************************
Macro definitions
******************************************************************************/
#define RIIC_ACCESS_TMOUT           (100u)

#define RIIC_CH1_PORT_NAME          ("iic1")

/* MAX9856 I2C Address */
#define MAX9856_RIIC_ADDR           (0x10u << 1) 

#define MAX9856_REG_MAX             (0x1Cu)

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
static int32_t RIIC_CH1_REG8_Write(const uint8_t riic_addr, const uint8_t reg_addr, const uint8_t reg_data);
static int32_t RIIC_CH1_REG8_Read(const uint8_t riic_addr, const uint8_t reg_addr, uint8_t* const p_reg_data);

static riic_ch1_aio_ctrl_t s_i2c1_ctrl = 
{
    NULL,             /* semaphore */
    DEVDRV_ERROR,     /* hi2c1 */

};

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/

/**************************************************************************//**
* Function Name: RIIC_MAX9856_Open
* @brief         Open riic channel 1
*                Description:<br>
*                Open RIIC channel 1 for access to i2c devices
* @param         none
* @retval        DEVDRV_SUCCESS    : Success
* @retval        error code  : Failure
******************************************************************************/
int32_t RIIC_MAX9856_Open(void)
{
    int_t ercd = DEVDRV_ERROR;
    st_r_drv_riic_create_t riic_clock;

    /* open the I2C channel1 driver */
    s_i2c1_ctrl.hi2c1 = open(DEVICE_INDENTIFIER "iic1", O_RDWR);

    if (DEVDRV_ERROR == s_i2c1_ctrl.hi2c1)
    {
        ercd = DEVDRV_ERROR;
    }
    else 
    {
        /* set the clock frequency for the I2C channel */
        riic_clock.frequency = RIIC_FREQUENCY_100KHZ;

        /* Create the I2C channel */
        ercd = control(s_i2c1_ctrl.hi2c1, CTL_RIIC_CREATE, &riic_clock);
    }

    return (ercd);
}
/******************************************************************************
End of function RIIC_MAX9856_Open
******************************************************************************/

/**************************************************************************//**
* Function Name: RIIC_MAX9856_Close
* @brief         Close riic channel 1
*                Description:<br>
*                Close RIIC channel 1 for access to i2c devices
* @param         none
* @retval        DEVDRV_SUCCESS    : Success
* @retval        error code  : Failure
******************************************************************************/
int32_t RIIC_MAX9856_Close(void)
{
    int_t ercd = DEVDRV_SUCCESS;

    /*Close Channel*/
    ercd = close(s_i2c1_ctrl.hi2c1);
    s_i2c1_ctrl.hi2c1 = DEVDRV_ERROR;

    return (ercd);
}
/******************************************************************************
End of function RIIC_MAX9856_Close
******************************************************************************/

/**************************************************************************//**
* Function Name: RIIC_MAX9856_Write
* @brief         Write register to MAX9856
*
*                Description:<br>
*                
* @param         addr       :register address 0-28
* @param         data       :register data 0-255
* @retval        DEVDRV_SUCCESS   :Success.
* @retval        error code :Failure.
******************************************************************************/
int32_t RIIC_MAX9856_Write(const uint8_t addr, const uint8_t data)
{
    int_t ercd;

    if (addr > MAX9856_REG_MAX)
    {
        ercd = DEVDRV_ERROR;
    }
    else
    {
        ercd = RIIC_CH1_REG8_Write(MAX9856_RIIC_ADDR, addr, data);
    }

    return (ercd);
}
/******************************************************************************
End of function RIIC_MAX9856_Write
******************************************************************************/

/**************************************************************************//**
* Function Name: RIIC_MAX9856_Read
* @brief         Read register from MAX9856
*
*                Description:<br>
*                
* @param         addr       :register address 0-28
* @param         p_data     :store address of register data 0-255
* @retval        DEVDRV_SUCCESS   :Success.
* @retval        error code :Failure.
******************************************************************************/
int32_t RIIC_MAX9856_Read(const uint8_t addr, uint8_t* const p_data)
{
    int_t ercd;

    if (addr > MAX9856_REG_MAX)
    {
        ercd = DEVDRV_ERROR;
    }
    else if (NULL == p_data)
    {
        ercd = DEVDRV_ERROR;
    }
    else
    {
        ercd = RIIC_CH1_REG8_Read(MAX9856_RIIC_ADDR, addr, p_data);
    }

    return (ercd);
}
/******************************************************************************
End of function RIIC_MAX9856_Read
******************************************************************************/


/******************************************************************************
Private functions
******************************************************************************/

/**************************************************************************//**
* Function Name: RIIC_CH1_REG8_Write
* @brief         Write data to target device register.
*
*                Description:<br>
*                
* @param[in]     riic_addr       : riic 8bit address 0-255
* @param[in]     reg_addr        : register address  0-255
* @param[in]     reg_data        : register data     0-255
* @retval        DEVDRV_SUCCESS   :Success.
* @retval        error code :Failure.
******************************************************************************/
static int32_t RIIC_CH1_REG8_Write(const uint8_t riic_addr, const uint8_t reg_addr, const uint8_t reg_data)
{
    int32_t riic_ret = DEVDRV_ERROR;
    st_r_drv_riic_config_t i2c_write;
    uint8_t cpy_reg_data = reg_data;

    /*Set RIIC Address*/
    i2c_write.device_address = riic_addr;
    i2c_write.sub_address = reg_addr; 

    /* Assign Data to Write */
    i2c_write.number_of_bytes = 1u;
    i2c_write.p_data_buffer = &cpy_reg_data;
    
    /*Write Data*/
    riic_ret = control(s_i2c1_ctrl.hi2c1, CTL_RIIC_WRITE, &i2c_write);

    return (riic_ret);
}
/******************************************************************************
End of function RIIC_CH1_REG8_Write
******************************************************************************/

/**************************************************************************//**
* Function Name: RIIC_CH1_REG8_Read
* @brief         Read data from target device register.
*
*                Description:<br>
*                
* @param[in]     riic_addr       : riic 8bit address 0-255
* @param[in]     reg_addr        : register address  0-255
* @param[in,out] p_reg_data      : store address of register data 0-255
* @retval        DEVDRV_SUCCESS   :Success.
* @retval        error code :Failure.
******************************************************************************/
static int32_t RIIC_CH1_REG8_Read(const uint8_t riic_addr, const uint8_t reg_addr, uint8_t* const p_reg_data)
{
    int32_t riic_ret= DEVDRV_ERROR;
    st_r_drv_riic_config_t i2c_read;

    /*Set RIIC address*/
    i2c_read.device_address = riic_addr;
    i2c_read.sub_address = reg_addr;
    
    /*Set location to read to*/
    i2c_read.number_of_bytes = 1u;
    i2c_read.p_data_buffer = p_reg_data;

    riic_ret = control(s_i2c1_ctrl.hi2c1, CTL_RIIC_READ_NEXT, &i2c_read);

    return (riic_ret);
}
/******************************************************************************
End of function RIIC_CH1_REG8_Read
******************************************************************************/
#else

#include    <stdio.h>
#include    <string.h>
#include    <fcntl.h>

//#include "ioif_public.h"
//#include "riic_if.h"
//#include "r_camera_if.h"

#include 	"dev_drv.h"
#include    "riic_max9856_drv.h"
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
#define RIIC_ACCESS_TMOUT           (100u)

#define RIIC_CH3_PORT_NAME          ("iic3")

/* MAX9856 I2C Address */
#define MAX9856_RIIC_ADDR           (0x10u << 1) 

#define MAX9856_REG_MAX             (0x1Cu)

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
static int32_t RIIC_CH3_REG8_Write(const uint8_t riic_addr, const uint8_t reg_addr, const uint8_t reg_data);
static int32_t RIIC_CH3_REG8_Read(const uint8_t riic_addr, const uint8_t reg_addr, uint8_t* const p_reg_data);

static riic_ch3_aio_ctrl_t s_i2c3_ctrl = 
{
    NULL,             /* semaphore */
    DEVDRV_ERROR,     /* hi2c3 */

};

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/

/**************************************************************************//**
* Function Name: RIIC_MAX9856_Open
* @brief         Open riic channel 1
*                Description:<br>
*                Open RIIC channel 1 for access to i2c devices
* @param         none
* @retval        DEVDRV_SUCCESS    : Success
* @retval        error code  : Failure
******************************************************************************/
int32_t RIIC_MAX9856_Open(void)
{
    int_t ercd = DEVDRV_ERROR;
    st_r_drv_riic_create_t riic_clock;

    /* open the I2C channel1 driver */
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
End of function RIIC_MAX9856_Open
******************************************************************************/

/**************************************************************************//**
* Function Name: RIIC_MAX9856_Close
* @brief         Close riic channel 1
*                Description:<br>
*                Close RIIC channel 1 for access to i2c devices
* @param         none
* @retval        DEVDRV_SUCCESS    : Success
* @retval        error code  : Failure
******************************************************************************/
int32_t RIIC_MAX9856_Close(void)
{
    int_t ercd = DEVDRV_SUCCESS;

    /*Close Channel*/
    ercd = close(s_i2c3_ctrl.hi2c3);
    s_i2c3_ctrl.hi2c3 = DEVDRV_ERROR;

    return (ercd);
}
/******************************************************************************
End of function RIIC_MAX9856_Close
******************************************************************************/

/**************************************************************************//**
* Function Name: RIIC_MAX9856_Write
* @brief         Write register to MAX9856
*
*                Description:<br>
*                
* @param         addr       :register address 0-28
* @param         data       :register data 0-255
* @retval        DEVDRV_SUCCESS   :Success.
* @retval        error code :Failure.
******************************************************************************/
int32_t RIIC_MAX9856_Write(const uint8_t addr, const uint8_t data)
{
    int_t ercd;

    if (addr > MAX9856_REG_MAX)
    {
        ercd = DEVDRV_ERROR;
    }
    else
    {
        ercd = RIIC_CH3_REG8_Write(MAX9856_RIIC_ADDR, addr, data);
    }

    return (ercd);
}
/******************************************************************************
End of function RIIC_MAX9856_Write
******************************************************************************/

/**************************************************************************//**
* Function Name: RIIC_MAX9856_Read
* @brief         Read register from MAX9856
*
*                Description:<br>
*                
* @param         addr       :register address 0-28
* @param         p_data     :store address of register data 0-255
* @retval        DEVDRV_SUCCESS   :Success.
* @retval        error code :Failure.
******************************************************************************/
int32_t RIIC_MAX9856_Read(const uint8_t addr, uint8_t* const p_data)
{
    int_t ercd;

    if (addr > MAX9856_REG_MAX)
    {
        ercd = DEVDRV_ERROR;
    }
    else if (NULL == p_data)
    {
        ercd = DEVDRV_ERROR;
    }
    else
    {
        ercd = RIIC_CH3_REG8_Read(MAX9856_RIIC_ADDR, addr, p_data);
    }

    return (ercd);
}
/******************************************************************************
End of function RIIC_MAX9856_Read
******************************************************************************/


/******************************************************************************
Private functions
******************************************************************************/

/**************************************************************************//**
* Function Name: RIIC_CH3_REG8_Write
* @brief         Write data to target device register.
*
*                Description:<br>
*                
* @param[in]     riic_addr       : riic 8bit address 0-255
* @param[in]     reg_addr        : register address  0-255
* @param[in]     reg_data        : register data     0-255
* @retval        DEVDRV_SUCCESS   :Success.
* @retval        error code :Failure.
******************************************************************************/
static int32_t RIIC_CH3_REG8_Write(const uint8_t riic_addr, const uint8_t reg_addr, const uint8_t reg_data)
{
    int32_t riic_ret = DEVDRV_ERROR;
    st_r_drv_riic_config_t i2c_write;
    uint8_t cpy_reg_data = reg_data;

    /*Set RIIC Address*/
    i2c_write.device_address = riic_addr;
    i2c_write.sub_address = reg_addr; 

    /* Assign Data to Write */
    i2c_write.number_of_bytes = 1u;
    i2c_write.p_data_buffer = &cpy_reg_data;
    
    /*Write Data*/
    riic_ret = control(s_i2c3_ctrl.hi2c3, CTL_RIIC_WRITE, &i2c_write);

    return (riic_ret);
}
/******************************************************************************
End of function RIIC_CH3_REG8_Write
******************************************************************************/

/**************************************************************************//**
* Function Name: RIIC_CH3_REG8_Read
* @brief         Read data from target device register.
*
*                Description:<br>
*                
* @param[in]     riic_addr       : riic 8bit address 0-255
* @param[in]     reg_addr        : register address  0-255
* @param[in,out] p_reg_data      : store address of register data 0-255
* @retval        DEVDRV_SUCCESS   :Success.
* @retval        error code :Failure.
******************************************************************************/
static int32_t RIIC_CH3_REG8_Read(const uint8_t riic_addr, const uint8_t reg_addr, uint8_t* const p_reg_data)
{
    int32_t riic_ret= DEVDRV_ERROR;
    st_r_drv_riic_config_t i2c_read;

    /*Set RIIC address*/
    i2c_read.device_address = riic_addr;
    i2c_read.sub_address = reg_addr;
    
    /*Set location to read to*/
    i2c_read.number_of_bytes = 1u;
    i2c_read.p_data_buffer = p_reg_data;

    riic_ret = control(s_i2c3_ctrl.hi2c3, CTL_RIIC_READ_NEXT, &i2c_read);

    return (riic_ret);
}
/******************************************************************************
End of function RIIC_CH3_REG8_Read
******************************************************************************/
#endif
