/******************************************************************************
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
 *******************************************************************************
 * Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.    */
/******************************************************************************
 * File Name    : r_camera_ov7740.c
 * Version      : 1.00
 * Device(s)    : RZ/A1L
 * Tool-Chain   : GNUARM-NONE-EABI-v16.01
 * OS           : None
 * H/W Platform : Stream it! v2 board
 * Description  : Interface and configuration for an OmniVision
 *                OV7740 Camera Module
 *******************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 13.02.2017 1.00 First Release
 ******************************************************************************/

/******************************************************************************
 WARNING!  IN ACCORDANCE WITH THE USER LICENCE THIS CODE MUST NOT BE CONVEYED
 OR REDISTRIBUTED IN COMBINATION WITH ANY SOFTWARE LICENSED UNDER TERMS THE
 SAME AS OR SIMILAR TO THE GNU GENERAL PUBLIC LICENCE
 ******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include "mcu_board_select.h"
#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
#include <stdio.h>
#include    <fcntl.h>

#include "r_typedefs.h"
#include "iodefine_cfg.h"
#include "Renesas_RZ_A1.h"
#include "r_camera_if.h"
#include "r_camera_ov7740.h"
#include "r_os_abstraction_api.h"
#include "control.h"
#include "r_devlink_wrapper.h"
#include "r_riic_drv_sc_cfg.h"

/******************************************************************************
Typedef definitions
******************************************************************************/
typedef struct
{
    uint8_t addr;
    uint8_t val;
} omni_register_t;

/******************************************************************************
 Macro Definitions
 ******************************************************************************/

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/

/* For more information on the OV7740 camera, please visit the Omnivision web site */
/* http://www.ovt.com */

/* Initialisation sequence for VGA resolution (640x480) */
static const omni_register_t ov7740_reset_regs_vga[] =
{
    /* {register, value} */
    {0x04, 0x60},       // "reserved"
    {0x0c, 0x12},       // REG0C
    {0x0d, 0x34},       // "reserved"
    {0x11, 0x03},       // CLK
    {0x12, 0x00},       // REG12
    {0x13, 0xff},       // REG13
    {0x14, 0x38},       // REG14
    {0x17, 0x24},       // AHSTART
    {0x18, 0xa0},       // AHSIZE
    {0x19, 0x03},       // AVSTART
    {0x1a, 0xf0},       // AVSIZE
    {0x1b, 0x85},       // PSHFT
    {0x1e, 0x13},       // REG1E
    {0x20, 0x00},       // "reserved"
    {0x21, 0x23},       // "reserved"
    {0x22, 0x03},       // "reserved"
    {0x24, 0x3c},       // WPT
    {0x25, 0x30},       // BPT
    {0x26, 0x72},       // VPT
    {0x27, 0x80},       // REG27
    {0x29, 0x17},       // REG29
    {0x2b, 0xf8},       // REG2B
    {0x2c, 0x01},       // REG2C
    {0x31, 0xa0},       // HOUTSIZE
    {0x32, 0xf0},       // VOUTSIZE
    {0x33, 0xc4},       // "reserved"
    {0x36, 0x3f},       // "reserved"
    {0x38, 0x11},       // REG38 - see OV7740 data sheet
    {0x3a, 0xb4},       // "reserved"
    {0x3d, 0x0f},       // "reserved"
    {0x3e, 0x82},       // "reserved"
    {0x3f, 0x40},       // "reserved"
    {0x40, 0x7f},       // "reserved"
    {0x41, 0x6a},       // "reserved"
    {0x42, 0x29},       // "reserved"
    {0x44, 0xe5},       // "reserved"
    {0x45, 0x41},       // "reserved"
    {0x47, 0x42},       // "reserved"
    {0x48, 0x00},       // "reserved"
    {0x49, 0x61},       // "reserved"
    {0x4a, 0xa1},       // "reserved"
    {0x4b, 0x46},       // "reserved"
    {0x4c, 0x18},       // "reserved"
    {0x4d, 0x50},       // "reserved"
    {0x4e, 0x13},       // "reserved"
    {0x50, 0x97},       // REG50
    {0x51, 0x7e},       // REG51
    {0x52, 0x00},       // REG52
    {0x53, 0x00},       // "reserved"
    {0x55, 0x42},       // "reserved"
    {0x56, 0x55},       // REG56
    {0x57, 0xff},       // REG57
    {0x58, 0xff},       // REG58
    {0x59, 0xff},       // REG59
    {0x5a, 0x24},       // "reserved"
    {0x5b, 0x1f},       // "reserved"
    {0x5c, 0x88},       // "reserved"
    {0x5d, 0x60},       // "reserved"
    {0x5f, 0x04},       // "reserved"
    {0x64, 0x00},       // "reserved"
    {0x67, 0x88},       // REG67
    {0x68, 0x1a},       // "reserved"
    {0x69, 0x08},       // REG69
    {0x70, 0x00},       // "reserved"
    {0x71, 0x34},       // "reserved"
    {0x74, 0x28},       // "reserved"
    {0x75, 0x98},       // "reserved"
    {0x76, 0x00},       // "reserved"
    {0x77, 0x08},       // "reserved"
    {0x78, 0x01},       // "reserved"
    {0x79, 0xc2},       // "reserved"
    {0x7a, 0x9c},       // "reserved"
    {0x7b, 0x40},       // "reserved"
    {0x7c, 0x0c},       // "reserved"
    {0x7d, 0x02},       // "reserved"
    {0x80, 0x7f},       // ISP CTRL00
    {0x81, 0x3f},       // ISP CTRL01
    {0x82, 0x32},       // ISP CTRL02
    {0x83, 0x03},       // ISP CTRL03
    {0x84, 0x70},       // REG84 - see OV7740 data sheet
    {0x85, 0x00},       // AGC OFFSET
    {0x86, 0x03},       // AGC BASE1
    {0x87, 0x01},       // AGC BASE2
    {0x88, 0x05},       // AGC CTRL
    {0x89, 0x30},       // LENC CTRL
    {0x8d, 0x40},       // LENC RED A1
    {0x8e, 0x00},       // LENC RED B1
    {0x8f, 0x33},       // LENC RED AB2
    {0x93, 0x28},       // LENC GREEN A1
    {0x94, 0x20},       // LENC GREEN B1
    {0x95, 0x33},       // LENC GREEN AB2
    {0x99, 0x30},       // LENC BLUE A1
    {0x9a, 0x14},       // LENC BLUE B1
    {0x9b, 0x33},       // LENC BLUE AB2
    {0x9c, 0x08},       // GMA YST01
    {0x9d, 0x12},       // GMA YST02
    {0x9e, 0x23},       // GMA YST03
    {0x9f, 0x45},       // GMA YST04
    {0xa0, 0x55},       // GMA YST05
    {0xa1, 0x64},       // GMA YST06
    {0xa2, 0x72},       // GMA YST07
    {0xa3, 0x7f},       // GMA YST08
    {0xa4, 0x8b},       // GMA YST09
    {0xa5, 0x95},       // GMA YST10
    {0xa6, 0xa7},       // GMA YST11
    {0xa7, 0xb5},       // GMA YST12
    {0xa8, 0xcb},       // GMA YST13
    {0xa9, 0xdd},       // GMA YST14
    {0xaa, 0xec},       // GMA YST15
    {0xab, 0x1a},       // GMA YSLP
    {0xac, 0x6e},       // "reserved"
    {0xbe, 0xff},       // "reserved"
    {0xbf, 0x00},       // "reserved"
    {0xce, 0x78},       // CMX M1
    {0xcf, 0x6e},       // CMX M2
    {0xd0, 0x0a},       // CMX M3
    {0xd1, 0x0c},       // CMX M4
    {0xd2, 0x84},       // CMX M5
    {0xd3, 0x90},       // CMX M6
    {0xd4, 0x1e},       // CMX CTRL
    {0xd5, 0x10},       // SCALE SMTH CTRL
    {0xda, 0x04},       // SDE CTRL
    {0xe9, 0x00},       // YAVG BLK THRE
    {0xec, 0x02},       // REGEC

    {0xff, 0xff}        // structure end marker
};

/* Initialisation sequence for QVGA resolution (320x240) */
static const omni_register_t ov7740_reset_regs_qvga[] =
{
    /* {register, value} */
    {0x04, 0x60},       // "reserved"
    {0x0c, 0x12},       // REG0C
    {0x0d, 0x34},       // "reserved"
    {0x11, 0x03},       // CLK
    {0x12, 0x00},       // REG12
    {0x13, 0xff},       // REG13
    {0x14, 0x38},       // REG14
    {0x17, 0x24},       // AHSTART
    {0x18, 0xa0},       // AHSIZE
    {0x19, 0x03},       // AVSTART
    {0x1a, 0xf0},       // AVSIZE
    {0x1b, 0x85},       // PSHFT
    {0x1e, 0x13},       // REG1E
    {0x20, 0x00},       // "reserved"
    {0x21, 0x23},       // "reserved"
    {0x22, 0x03},       // "reserved"
    {0x24, 0x3c},       // WPT
    {0x25, 0x30},       // BPT
    {0x26, 0x72},       // VPT
    {0x27, 0x80},       // REG27
    {0x29, 0x17},       // REG29
    {0x2b, 0xf8},       // REG2B
    {0x2c, 0x01},       // REG2C
    {0x31, 0x50},       // HOUTSIZE
    {0x32, 0x78},       // VOUTSIZE
    {0x33, 0xc4},       // "reserved"
    {0x36, 0x3f},       // "reserved"
    {0x38, 0x11},       // REG38 - see OV7740 data sheet
    {0x3a, 0xb4},       // "reserved"
    {0x3d, 0x0f},       // "reserved"
    {0x3e, 0x82},       // "reserved"
    {0x3f, 0x40},       // "reserved"
    {0x40, 0x7f},       // "reserved"
    {0x41, 0x6a},       // "reserved"
    {0x42, 0x29},       // "reserved"
    {0x44, 0xe5},       // "reserved"
    {0x45, 0x41},       // "reserved"
    {0x47, 0x42},       // "reserved"
    {0x48, 0x00},       // "reserved"
    {0x49, 0x61},       // "reserved"
    {0x4a, 0xa1},       // "reserved"
    {0x4b, 0x46},       // "reserved"
    {0x4c, 0x18},       // "reserved"
    {0x4d, 0x50},       // "reserved"
    {0x4e, 0x13},       // "reserved"
    {0x50, 0x97},       // REG50
    {0x51, 0x7e},       // REG51
    {0x52, 0x00},       // REG52
    {0x53, 0x00},       // "reserved"
    {0x55, 0x42},       // "reserved"
    {0x56, 0x55},       // REG56
    {0x57, 0xff},       // REG57
    {0x58, 0xff},       // REG58
    {0x59, 0xff},       // REG59
    {0x5a, 0x24},       // "reserved"
    {0x5b, 0x1f},       // "reserved"
    {0x5c, 0x88},       // "reserved"
    {0x5d, 0x60},       // "reserved"
    {0x5f, 0x04},       // "reserved"
    {0x64, 0x00},       // "reserved"
    {0x67, 0x88},       // REG67
    {0x68, 0x1a},       // "reserved"
    {0x69, 0x08},       // REG69
    {0x70, 0x00},       // "reserved"
    {0x71, 0x34},       // "reserved"
    {0x74, 0x28},       // "reserved"
    {0x75, 0x98},       // "reserved"
    {0x76, 0x00},       // "reserved"
    {0x77, 0x08},       // "reserved"
    {0x78, 0x01},       // "reserved"
    {0x79, 0xc2},       // "reserved"
    {0x7a, 0x9c},       // "reserved"
    {0x7b, 0x40},       // "reserved"
    {0x7c, 0x0c},       // "reserved"
    {0x7d, 0x02},       // "reserved"
    {0x80, 0x7f},       // ISP CTRL00
    {0x81, 0x3f},       // ISP CTRL01
    {0x82, 0x3f},       // ISP CTRL02
    {0x83, 0x03},       // ISP CTRL03
    {0x84, 0x70},       // REG84 - see OV7740 data sheet
    {0x85, 0x00},       // AGC OFFSET
    {0x86, 0x03},       // AGC BASE1
    {0x87, 0x01},       // AGC BASE2
    {0x88, 0x05},       // AGC CTRL
    {0x89, 0x30},       // LENC CTRL
    {0x8d, 0x40},       // LENC RED A1
    {0x8e, 0x00},       // LENC RED B1
    {0x8f, 0x33},       // LENC RED AB2
    {0x93, 0x28},       // LENC GREEN A1
    {0x94, 0x20},       // LENC GREEN B1
    {0x95, 0x33},       // LENC GREEN AB2
    {0x99, 0x30},       // LENC BLUE A1
    {0x9a, 0x14},       // LENC BLUE B1
    {0x9b, 0x33},       // LENC BLUE AB2
    {0x9c, 0x08},       // GMA YST01
    {0x9d, 0x12},       // GMA YST02
    {0x9e, 0x23},       // GMA YST03
    {0x9f, 0x45},       // GMA YST04
    {0xa0, 0x55},       // GMA YST05
    {0xa1, 0x64},       // GMA YST06
    {0xa2, 0x72},       // GMA YST07
    {0xa3, 0x7f},       // GMA YST08
    {0xa4, 0x8b},       // GMA YST09
    {0xa5, 0x95},       // GMA YST10
    {0xa6, 0xa7},       // GMA YST11
    {0xa7, 0xb5},       // GMA YST12
    {0xa8, 0xcb},       // GMA YST13
    {0xa9, 0xdd},       // GMA YST14
    {0xaa, 0xec},       // GMA YST15
    {0xab, 0x1a},       // GMA YSLP
    {0xac, 0x6e},       // "reserved"
    {0xbe, 0xff},       // "reserved"
    {0xbf, 0x00},       // "reserved"
    {0xce, 0x78},       // CMX M1
    {0xcf, 0x6e},       // CMX M2
    {0xd0, 0x0a},       // CMX M3
    {0xd1, 0x0c},       // CMX M4
    {0xd2, 0x84},       // CMX M5
    {0xd3, 0x90},       // CMX M6
    {0xd4, 0x1e},       // CMX CTRL
    {0xd5, 0x10},       // SCALE SMTH CTRL
    {0xda, 0x04},       // SDE CTRL
    {0xe9, 0x00},       // YAVG BLK THRE
    {0xec, 0x02},       // REGEC

    {0xff, 0xff}        // structure end marker
};

static const omni_register_t ov7740_test[] =
{
    {0x28, 0x02},       // VSYNC Negative Value
    {0xda, 0x00},       // Fixed YU disable

    {0xff, 0xff}        // structure end marker
};

static void write_omni_table (const omni_register_t Table[]);


/*****************************************************************************
 Function Name: R_CAMERA_Ov7740Init
 Description:   Initialise an OmniVision OV7740 camera module
 Parameters:    resolution  - CAMERA_RES_VGA or  CAMERA_RES_QVGA
 Return value:  None
 *****************************************************************************/
void R_CAMERA_Ov7740Init (int videoResolution)
{
    /* P2.8 CAM_PWRDN*/
    GPIO.PIBC2  &= (uint16_t) (~(uint32_t)GPIO_BIT_N8);
    GPIO.PBDC2  &= (uint16_t) (~(uint32_t)GPIO_BIT_N8);
    GPIO.PM2    |= (uint16_t)   (uint32_t)GPIO_BIT_N8;
    GPIO.PMC2   &= (uint16_t) (~(uint32_t)GPIO_BIT_N8);
    GPIO.PIPC2  &= (uint16_t) (~(uint32_t)GPIO_BIT_N8);

    GPIO.PBDC2  &= (uint16_t) (~(uint32_t)GPIO_BIT_N8);
    GPIO.P2     |=   (uint32_t)GPIO_BIT_N8;
    GPIO.PM2    &= (uint16_t) (~(uint32_t)GPIO_BIT_N8);

    R_OS_TaskSleep(5);                  // The Omnivision chip needs some wait time after reset

    /* P2.9 CAM_TFT_RST*/
    GPIO.PIBC2  &= (uint16_t)(~(uint32_t)GPIO_BIT_N9);
    GPIO.PBDC2  &= (uint16_t)(~(uint32_t)GPIO_BIT_N9);
    GPIO.PM2    |=   (uint32_t)GPIO_BIT_N9;
    GPIO.PMC2   &= (uint16_t)(~(uint32_t)GPIO_BIT_N9);
    GPIO.PIPC2  &= (uint16_t)(~(uint32_t)GPIO_BIT_N9);

    GPIO.PBDC2  &= (uint16_t)(~(uint32_t)GPIO_BIT_N9);
    GPIO.P2     &= (uint16_t)(~(uint32_t)GPIO_BIT_N9);
    GPIO.PM2    &= (uint16_t)(~(uint32_t)GPIO_BIT_N9);

    R_OS_TaskSleep(5);                  // The Omnivision chip needs some wait time after reset

    /* Active Power up on the camera */
    /* P2.9 CAM_TFT_RST*/
    GPIO.PIBC2  &= (uint16_t)(~(uint32_t)GPIO_BIT_N9);
    GPIO.PBDC2  &= (uint16_t)(~(uint32_t)GPIO_BIT_N9);
    GPIO.PM2    |=   (uint32_t)GPIO_BIT_N9;
    GPIO.PMC2   &= (uint16_t)(~(uint32_t)GPIO_BIT_N9);
    GPIO.PIPC2  &= (uint16_t)(~(uint32_t)GPIO_BIT_N9);

    GPIO.PBDC2  &= (uint16_t)(~(uint32_t)GPIO_BIT_N9);
    GPIO.P2     |=   (uint32_t)GPIO_BIT_N9;
    GPIO.PM2    &= (uint16_t)(~(uint32_t)GPIO_BIT_N9);

    R_OS_TaskSleep(5);                  // The Omnivision chip needs some wait time after reset

    /* P2.8 CAM_PWRDN*/
    GPIO.PIBC2  &= (uint16_t)(~(uint32_t)GPIO_BIT_N8);
    GPIO.PBDC2  &= (uint16_t)(~(uint32_t)GPIO_BIT_N8);
    GPIO.PM2    |=   (uint32_t)GPIO_BIT_N8;
    GPIO.PMC2   &= (uint16_t)(~(uint32_t)GPIO_BIT_N8);
    GPIO.PIPC2  &= (uint16_t)(~(uint32_t)GPIO_BIT_N8);

    GPIO.PBDC2  &= (uint16_t)(~(uint32_t)GPIO_BIT_N8);
    GPIO.P2     &= (uint16_t)(~(uint32_t)GPIO_BIT_N8);
    GPIO.PM2    &= (uint16_t)(~(uint32_t)GPIO_BIT_N8);

    R_OS_TaskSleep(5);                  // The Omnivision chip needs some wait time after reset

    switch (videoResolution)
    {
        default :
        case CAMERA_RES_VGA :
            write_omni_table(ov7740_reset_regs_vga);
        break;

        case CAMERA_RES_QVGA :
            write_omni_table(ov7740_reset_regs_qvga);
        break;
    }

    write_omni_table(ov7740_test);
}
/*****************************************************************************
 End of function R_CAMERA_Ov7740Init
 *****************************************************************************/

/*****************************************************************************
 Private Functions
 *****************************************************************************/

/*****************************************************************************
 Function Name: write_omni_table
 Description:   Writes a set of configuration values to the OV Camera Module
 Parameters:   Table pointer
 Return value:  None
 *****************************************************************************/
static void write_omni_table (const omni_register_t Table[])
{
    uint16_t i;
    st_r_drv_riic_config_t i2c_write;
    st_r_drv_riic_create_t riic_clock;
    int_t hi2c1 = ( -1);
    int_t error = -1;

    hi2c1 = open(DEVICE_INDENTIFIER "iic1", O_RDWR);

    if (( -1) == hi2c1)
    {
        /* error */
    }
    else
    {
        riic_clock.frequency = RIIC_FREQUENCY_100KHZ;
        error = control(hi2c1, CTL_RIIC_CREATE, &riic_clock);
        if (0 > error)
        {
            /* error */
        }
        else
        {
            for (i = 0; ((0xff != Table[i].addr) || (0xff != Table[i].val)); i++)
            {
                i2c_write.device_address = OV7740_I2C_ADDR;
                i2c_write.sub_address = Table[i].addr;
                i2c_write.number_of_bytes = 1;
                i2c_write.p_data_buffer = (uint8_t *) &Table[i].val;

                error = control(hi2c1, CTL_RIIC_WRITE, &i2c_write);
                if (0 > error)
                {
                    /* error */
                    break;
                }
            }
        }
        close(hi2c1);
    }
}
/*****************************************************************************
 End of function  write_omni_table
 *****************************************************************************/

/******************************************************************************
 End  Of File
 *****************************************************************************/
#else
#endif
