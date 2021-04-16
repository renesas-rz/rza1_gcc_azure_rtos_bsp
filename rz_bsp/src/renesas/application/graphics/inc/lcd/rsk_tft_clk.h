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
*
* Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/**************************************************************************//**
* @file         rsk_tft_clk.h
* @version      0.01
* $Rev:  $
* $Date:: #$
* @brief        Defines for VDC5 clock.
******************************************************************************/
#ifndef LCD_PANEL_CLKT_H
#define LCD_PANEL_CLKT_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include    <stdlib.h>

#include    "r_typedefs.h"

#include    "r_vdc5.h"

/******************************************************************************
Macro definitions
******************************************************************************/

/* vdc5 panel clock setting
   Select from the following.
    VDC_PANEL_ICKSEL_IMG          Divided video image clock (VIDEO_X1)
    VDC_PANEL_ICKSEL_IMG_DV       Divided video image clock (DV_CLK)
    VDC_PANEL_ICKSEL_EXT_0        Divided external clock (LCD0_EXTCLK)
    VDC_PANEL_ICKSEL_EXT_1        Divided external clock (LCD1_EXTCLK)
    VDC_PANEL_ICKSEL_PERI         Divided peripheral clock 1
    VDC_PANEL_ICKSEL_LVDS         LVDS PLL clock
    VDC_PANEL_ICKSEL_LVDS_DIV7    LVDS PLL clock divided by 7         */
#define LCD_CH0_PANEL_CLK         (VDC_PANEL_ICKSEL_PERI)

/* Clock frequency division ratio
   Select from the following.
    VDC_PANEL_CLKDIV_1_0          Division Ratio 1/1
    VDC_PANEL_CLKDIV_1_2          Division Ratio 1/2
    VDC_PANEL_CLKDIV_1_3          Division Ratio 1/3
    VDC_PANEL_CLKDIV_1_4          Division Ratio 1/4
    VDC_PANEL_CLKDIV_1_5          Division Ratio 1/5
    VDC_PANEL_CLKDIV_1_6          Division Ratio 1/6
    VDC_PANEL_CLKDIV_1_7          Division Ratio 1/7
    VDC_PANEL_CLKDIV_1_8          Division Ratio 1/8
    VDC_PANEL_CLKDIV_1_9          Division Ratio 1/9
    VDC_PANEL_CLKDIV_1_12         Division Ratio 1/12
    VDC_PANEL_CLKDIV_1_16         Division Ratio 1/16
    VDC_PANEL_CLKDIV_1_24         Division Ratio 1/24
    VDC_PANEL_CLKDIV_1_32         Division Ratio 1/32 */

/* Clock frequency division ratio */
#define LCD_CH0_PANEL_CLK_DIV     (VDC_PANEL_CLKDIV_1_2)

/* Lvds clock setting
    If LCD_CH0_PANEL_CLK define VDC_PANEL_ICKSEL_LVDS or VDC_PANEL_ICKSEL_LVDS_DIV7,
    please set the Lvds clock below. */

    /*  Note LVDS Clock
        ex)
        Input clock to the LVDS: P1, 66.67 MHz
        Output clock from the LVDS: 40.0 MHz
        NIDIV (Divider 1): 1/4
          FIN  = 66.67 / 4
               = 16.67 MHz
        NFD: 384
        NRD: 5
          FVCO = 16.67 * 384 / 5
               = 1280.0 MHz
        NOD: 8
          FOUT = 1280.0 / 8
               = 160.0 MHz
        NODIV (Divider 2): 1/4
          Output clock from the LVDS
               = 160.0 / 4
               = 40.0 MHz

        NIDIV   NODIV   NFD     NRD     NOD
        4       4       384     5       8       40.0 MHz, SVGA Signal  800 x 600 (60 Hz)
        4       4       312     5       4       65.0 MHz, XGA Signal  1024 x 768 (60 Hz)
        4       4       290     6       8       25.175 MHz, VGA Signal 640 x 480 (60 Hz)
    For LVDS Panel
        4       4       860     5       8       51.19305 MHZ (with /7)
    */

/* The clock input to frequency divider 1
   Select from the following.
    VDC_LVDS_INCLK_SEL_IMG        Video image clock (VIDEO_X1)
    VDC_LVDS_INCLK_SEL_DV_0       Video image clock (DV0_CLK)
    VDC_LVDS_INCLK_SEL_DV_1       Video image clock (DV1_CLK)
    VDC_LVDS_INCLK_SEL_EXT_0      External clock (LCD0_EXTCLK)
    VDC_LVDS_INCLK_SEL_EXT_1      External clock (LCD1_EXTCLK)
    VDC_LVDS_INCLK_SEL_PERI       Peripheral clock 1           */
#define LVDS_PLL_INPUT_CLK        (VDC_LVDS_INCLK_SEL_PERI)

/* The frequency dividing value (NIDIV or NODIV)
   Select from the following.
    VDC_LVDS_NDIV_1               Div 1
    VDC_LVDS_NDIV_2               Div 2
    VDC_LVDS_NDIV_4               Div 4 */
#define LVDS_PLL_NIDV             (VDC_LVDS_NDIV_4)
#define LVDS_PLL_NODIV            (VDC_LVDS_NDIV_4)

/* NFD  */
#define LVDS_PLL_NFD              (384u)

/* NRD */
#define LVDS_PLL_NRD              (5u-1u)

/* The frequency dividing value (NOD) for the output frequency
   Select from the following.
    VDC_LVDS_PLL_NOD_1           Div 1
    VDC_LVDS_PLL_NOD_2           Div 2
    VDC_LVDS_PLL_NOD_4           Div 4
    VDC_LVDS_PLL_NOD_8           Div 8 */

#define LVDS_PLL_NOD             (VDC_LVDS_PLL_NOD_8)

/*******************************************************************************
Functions Prototypes
*******************************************************************************/
void GRAPHICS_SetLcdPanel_Ch0(void);
void GRAPHICS_SetLcdTconSettings_Ch0(const vdc_lcd_tcon_timing_t * * const outctrl);

#endif  /* LCD_PANEL_CLKT_H */
