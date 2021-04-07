/***********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
 * No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
 * LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
 * INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
 * ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability
 * of this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * File Name     : r_camera_ov7670.c
 * Device(s)     : RZ/A1L
 * Tool-Chain    : GNUARM-NONEv16.01-EABI
 * H/W Platform  : Stream it! v2 board
 * Description   : Routines for setting up and driving an Omnivision OV7670 Camera
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * History       : DD.MM.YYYY Version Description
 *               : 13.02.2017 1.00
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 Includes
 ***********************************************************************************************************************/
#include "mcu_board_select.h"
#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)

#include <stdio.h>
#include <fcntl.h>

#include "r_typedefs.h"
#include "iodefine_cfg.h"
#include "r_camera_if.h"
#include "r_camera_ov7670.h"
#include "r_os_abstraction_api.h"
#include "control.h"
#include "r_devlink_wrapper.h"
#include "r_riic_drv_sc_cfg.h"
// TODO RC #include "riic_ch1_if.h"

/***********************************************************************************************************************
 Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 Global variables and functions
 ***********************************************************************************************************************/
static void write_omni_table (const omniregister_t Table[]);

/* For more information on the OV7670 camera, please visit the Omnivision web site */
/* http://www.ovt.com */

static const omniregister_t omni_common_regs[] =
{
    /*
     * Clock scale: 3 = 15fps
     *              2 = 20fps
     *              1 = 30fps
     */
    {REG_CLKRC, CLKRC_EXT},                     /* OV: clock scale (30 fps) */
    {REG_TSLB, TSLB_YLAST},                     /* Set YUV orders (in combination with REG_COM13) */
    {REG_COM13, COM13_GAMMA | COM13_UVSAT},     /* Gamma enable, UV sat enable, UYVY or VYUY (see TSLB_REG) */

    /* Set the hardware window. These values supplied by OmniVision. */
    {REG_HSTART, 0x13},
    {REG_HSTOP, 0x01},
    {REG_HREF, 0xb6},
    {REG_VSTART, 0x02},
    {REG_VSTOP, 0x7a},
    {REG_VREF, 0x0a},
    {REG_COM3, 0},
    {REG_COM14, 0},

    /* Mystery scaling numbers */
    {0x70, 0x3a},
    {0x71, 0x35},
    {0x72, 0x11},
    {0x73, 0xf0},
    {0xa2, 0x02},
    {REG_COM10, COM10_VS_NEG},

    /* Gamma curve values */
    {0x7a, 0x20},
    {0x7b, 0x10},
    {0x7c, 0x1e},
    {0x7d, 0x35},
    {0x7e, 0x5a},
    {0x7f, 0x69},
    {0x80, 0x76},
    {0x81, 0x80},
    {0x82, 0x88},
    {0x83, 0x8f},
    {0x84, 0x96},
    {0x85, 0xa3},
    {0x86, 0xaf},
    {0x87, 0xc4},
    {0x88, 0xd7},
    {0x89, 0xe8},

    /* AGC and AEC parameters. Note we start by disabling those features,
     * then turn them only after tweaking the values. */
    {REG_COM8, ((COM8_FASTAEC | COM8_AECSTEP) | COM8_BFILT)},
    {REG_GAIN, 0},
    {REG_AECH, 0},
    {REG_COM4, 0x40},       /* magic reserved bit */
    {REG_COM9, 0x18},       /* 4x gain + magic rsvd bit */
    {REG_BD50MAX, 0x05},
    {REG_BD60MAX, 0x07},
    {REG_AEW, 0x95},
    {REG_AEB, 0x33},
    {REG_VPT, 0xe3},
    {REG_HAECC1, 0x78},
    {REG_HAECC2, 0x68},
    {0xa1, 0x03},           /* magic */
    {REG_HAECC3, 0xd8},
    {REG_HAECC4, 0xd8},
    {REG_HAECC5, 0xf0},
    {REG_HAECC6, 0x90},
    {REG_HAECC7, 0x94},
    {REG_COM8, ((((COM8_FASTAEC | COM8_AECSTEP) | COM8_BFILT) | COM8_AGC) | COM8_AEC)},

    /* Almost all of these are magic "reserved" values */
    {REG_COM5, 0x61},
    {REG_COM6, 0x4b},
    {0x16, 0x02},
    {REG_MVFP, 0x07},
    {0x21, 0x02},
    {0x22, 0x91},
    {0x29, 0x07},
    {0x33, 0x0b},
    {0x35, 0x0b},
    {0x37, 0x1d},
    {0x38, 0x71},
    {0x39, 0x2a},
    {REG_COM12, 0x78},
    {0x4d, 0x40},
    {0x4e, 0x20},
    {REG_GFIX, 0},
    {REG_DBLV, DBLV_BYPASS_PLL},
    {0x74, 0x10},
    {0x8d, 0x4f},
    {0x8e, 0},
    {0x8f, 0},
    {0x90, 0},
    {0x91, 0},
    {0x96, 0},
    {0x9a, 0},
    {0xb0, 0x84},
    {0xb1, 0x0c},
    {0xb2, 0x0e},
    {0xb3, 0x82},
    {0xb8, 0x0a},

    /* More reserved magic, some of which tweaks white balance */
    {0x43, 0x0a},
    {0x44, 0xf0},
    {0x45, 0x34},
    {0x46, 0x58},
    {0x47, 0x28},
    {0x48, 0x3a},
    {0x59, 0x88},
    {0x5a, 0x88},
    {0x5b, 0x44},
    {0x5c, 0x67},
    {0x5d, 0x49},
    {0x5e, 0x0e},
    {0x6c, 0x0a},
    {0x6d, 0x55},
    {0x6e, 0x11},
    {0x6f, 0x9f},               /* "9e for advance AWB" */
    {0x6a, 0x40},
    {REG_BLUE, 0x40},
    {REG_RED, 0x60},
    {REG_COM8, (((((COM8_FASTAEC | COM8_AECSTEP) | COM8_BFILT) | COM8_AGC) | COM8_AEC) | COM8_AWB)},

    /* Matrix coefficients */
    {0x4f, 0x80},
    {0x50, 0x80},
    {0x51, 0},
    {0x52, 0x22},
    {0x53, 0x5e},
    {0x54, 0x80},
    {0x58, 0x9e},
    {REG_COM16, COM16_AWBGAIN},
    {REG_EDGE, 0},
    {0x75, 0x05},
    {0x76, 0xe1},
    {0x4c, 0},
    {0x77, 0x01},
    {0x4b, 0x09},
    {0xc9, 0x60},
    {REG_COM16, 0x38},
    {0x56, 0x40},
    {0x34, 0x11},
    {REG_COM11, COM11_EXP | COM11_HZAUTO},
    {0xa4, 0x88},
    {0x96, 0},
    {0x97, 0x30},
    {0x98, 0x20},
    {0x99, 0x30},
    {0x9a, 0x84},
    {0x9b, 0x29},
    {0x9c, 0x03},
    {0x9d, 0x4c},
    {0x9e, 0x3f},
    {0x78, 0x04},
    {0x79, 0x01},
    {0xc8, 0xf0},
    {0x79, 0x0f},
    {0xc8, 0x00},
    {0x79, 0x10},
    {0xc8, 0x7e},
    {0x79, 0x0a},
    {0xc8, 0x80},
    {0x79, 0x0b},
    {0xc8, 0x01},
    {0x79, 0x0c},
    {0xc8, 0x0f},
    {0x79, 0x0d},
    {0xc8, 0x20},
    {0x79, 0x09},
    {0xc8, 0x80},
    {0x79, 0x02},
    {0xc8, 0xc0},
    {0x79, 0x03},
    {0xc8, 0x40},
    {0x79, 0x05},
    {0xc8, 0x30},
    {0x79, 0x26},
    {REG_COM2, COM2_HIGHDRIVE},

    {0xff, 0xff}                    /* structure end marker */
};

/* *************************************** */
/* Start of output format register settings*/
/* *************************************** */

/* 30 fps YUV mode settings for different display sizes from VGA down to QQVGA */
static const omniregister_t omni_yuv422[] =
{
    {REG_COM7, 0x0},                /* Selects YUV mode */
    {REG_RGB444, 0},                /* No RGB444 please */
    {REG_COM1, 0},                  /* CCIR601 */
    {REG_COM15, COM15_R00FF},
    {REG_COM9, 0x18},               /* 4x gain ceiling; 0x8 is reserved bit */
    {0x4f, 0x80},                   /* "matrix coefficient 1" */
    {0x50, 0x80},                   /* "matrix coefficient 2" */
    {0x51, 0},                      /* vb */
    {0x52, 0x22},                   /* "matrix coefficient 4" */
    {0x53, 0x5e},                   /* "matrix coefficient 5" */
    {0x54, 0x80},                   /* "matrix coefficient 6" */

    {0xff, 0xff}                    /* structure end marker */
};

/* Image scaling on the OV7670 is possible using the 'down sampling' and 'digital zoom out' blocks. Down sampling
 * supports 1/2^N scaling whereas the digital zoom out block supports fractional scaling.
 * Scaling values calculated using the formula:
 * Scaling ratio = 0x20 x Image Size from Down sampling circuit / New image size
 * In this case the predefined QCIF resolution is fractionally scaled to get the desired size.
 */

/* Configuration register table for QVGA */
static const omniregister_t omni_qvga[] =
{
    {REG_COM7, COM7_FMT_CIF},       /* Selects YUV mode / Output format CIF Selection */
    {REG_COM3, COM3_DCWEN},         /* COM3[2] = 1 (Down Sampling selected) */
    {REG_COM14, 0x19},              /* COM14 */
    {0x72, 0x11},                   /* Scaling_DCWCTR -> OK (Down Sampling : 1/2 to obtain 320x240 from 640x480) */
    {0x73, 0xF8},                   /* Scaling_PCLK_DIV -> Bypass clock divider control for DSP Scale Control */
    {0xA2, 0x00},                   /* Scaling_PCLK_DELAY */

    {0xff, 0xff}                    /* structure end marker */
};

/* Configuration register table for VGA */
static const omniregister_t omni_vga[] =
{
    {REG_COM3, 0x00},               /* COM3 */
    {REG_COM14, 0x00},              /* COM14 */
    {0x70, 0x3A},                   /* Scaling_XSC */
    {0x71, 0x35},                   /* Scaling_YSC */
    {0x72, 0x11},                   /* Scaling_DCWCTR */
    {0x73, 0xF0},                   /* Scaling_PCLK_DIV */
    {0xA2, 0x02},                   /* Scaling_PCLK_DELAY */

    {0xff, 0xff}                    /* structure end marker */
};

/***********************************************************************************************************************
 * Function Name: write_omni_table
 * Description  : Programs Omnivision camera configuration registers.
 * Arguments    : Table - Pointer to a table containing the configuration.
 * Return Value : void
 ***********************************************************************************************************************/

static void write_omni_table (const omniregister_t Table[])
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
                i2c_write.device_address = OV7670_I2C_ADDR;
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

/***********************************************************************************************************************
 End of Function write_omni_table
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_CAMERA_Ov7670Init
 * Description  : API function to initialise the OV7670 camera.
 * Arguments    : videoResolution 
 * Return Value : void
 ***********************************************************************************************************************/
void R_CAMERA_Ov7670Init (int videoResolution)
{
    /* Reset camera module and wait a while */
    // TODO RC   R_RIIC_CH1_CAM_CAMERA_W(REG_COM7, COM7_RESET);
    R_OS_TaskSleep(5);

    /* Release reset and wait a while */
    // TODO RC   R_RIIC_CH1_CAM_CAMERA_W(REG_COM7, COM7_FMT_VGA);
    R_OS_TaskSleep(5);

    /* Write the default values in the camera module */
    write_omni_table(omni_common_regs);

    /* Set the output format */
    write_omni_table(omni_yuv422);

    /* Set camera output size */
    switch (videoResolution)
    {
        default :
        case CAMERA_RES_VGA :
        {
            write_omni_table(omni_vga);
            break;
        }

        case CAMERA_RES_QVGA :
        {
            write_omni_table(omni_qvga);
            break;
        }
    }
}
/***********************************************************************************************************************
 End of Function R_CAMERA_Ov7670Init
 ***********************************************************************************************************************/
#else
#endif
