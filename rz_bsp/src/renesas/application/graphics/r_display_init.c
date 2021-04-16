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
 * File Name :    r_display_init.c
 * @file          r_display_init.c
 * $Rev:          1.0
 * $Date:         24.04.2019
 * @brief         Display setting for VDC
 ******************************************************************************/

/*******************************************************************************
 Includes <System Includes>, "Project Includes"
 *******************************************************************************/
#include "mcu_board_select.h"

#include <stdio.h>
#include "cmsis_os.h"

#include "r_typedefs.h"

#include "r_rvapi_header.h"
#include "lcd_panel.h"
#include "r_display_init.h"
#include "r_image_config.h"

/******************************************************************************
 Macro definitions
 ******************************************************************************/

/******************************************************************************
 Typedef definitions
 ******************************************************************************/

/******************************************************************************
 Exported global variables
 ******************************************************************************/
#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
uint8_t video_buffer[0x7f800] __attribute__ ((section(".VRAM_SECTION0")));
#elif (TARGET_BOARD == TARGET_BOARD_RSK)
uint8_t video_buffer[0x177000] __attribute__ ((section(".VRAM_SECTION0")));
#endif /* TARGET_BOARD */
/******************************************************************************
 Private global variables and functions
 ******************************************************************************/

/*!****************************************************************************
 * Function Name: r_display_init
 * @brief         Display setting
 * @param[in]     vdc_ch    :
 * @retval        vdc_error_t
 ******************************************************************************/
vdc_error_t r_display_init(const vdc_channel_t vdc_ch)
{
    vdc_error_t error;

    /* Display clock setting */
    {
        clock_config_t clock_config;
        vdc_lvds_t lvds;

        clock_config.panel_clk = LCD_CH0_PANEL_CLK;
        clock_config.panel_clk_div = LCD_CH0_PANEL_CLK_DIV;

        lvds.lvds_vdc_sel = vdc_ch; /* A channel in VDC whose data is to be output through the LVDS */
        lvds.lvds_in_clk_sel = LVDS_PLL_INPUT_CLK; /* The clock input to frequency divider 1 */
        lvds.lvds_idiv_set = LVDS_PLL_NIDV; /* The frequency dividing value (NIDIV) for frequency divider 1 */
        lvds.lvds_odiv_set = LVDS_PLL_NODIV; /* The frequency dividing value (NODIV) for frequency divider 2 */
        lvds.lvdspll_fd = LVDS_PLL_NFD; /* The frequency dividing value (NFD) for the feedback frequency */
        lvds.lvdspll_rd = LVDS_PLL_NRD; /* The frequency dividing value (NRD) for the input frequency */
        lvds.lvdspll_od = LVDS_PLL_NOD; /* The frequency dividing value (NOD) for the output frequency */
        clock_config.lvds = &lvds;

        error = R_RVAPI_InitializeVDC (vdc_ch, &clock_config);
    }

    /* Display setting */
    if (VDC_OK == error)
    {
        qe_config_t q_cnf;

        q_cnf.vps = LCD_CH0_S_VSYNC; /* Vertical Pulse start position                 */
        q_cnf.vpw = LCD_CH0_W_VSYNC; /* Vertical Pulse Width(VPW)                     */
        q_cnf.vs = LCD_CH0_DISP_VS; /* LCD display area vertical start position      */
        q_cnf.vdp = LCD_CH0_DISP_VW; /* Vertical Display Period(VDP)                  */
        q_cnf.hps = LCD_CH0_S_HSYNC; /* Horizontal Pulse start position               */
        q_cnf.hpw = LCD_CH0_W_HSYNC; /* Horizontal Pulse Width(HPW)                   */
        q_cnf.hs = LCD_CH0_DISP_HS; /* LCD display area horizontal start position    */
        q_cnf.hdp = LCD_CH0_DISP_HW; /* Horizontal Display Period(HDP                 */
        q_cnf.vtp = LCD_CH0_SIG_FV; /* Vertical Total Period(VTP)                    */
        q_cnf.htp = LCD_CH0_SIG_FH; /* Horizontal Total Period(HTP)                  */
        q_cnf.tcon_vsync = LCD_CH0_TCON_PIN_VSYNC; /* Vsync_TCON_select                       */
        q_cnf.tcon_hsync = LCD_CH0_TCON_PIN_HSYNC; /* Hsync_TCON_select                       */
        q_cnf.tcon_de = LCD_CH0_TCON_PIN_DE; /* DE_TCON_select                          */
        q_cnf.tcon_vsync_inv = LCD_CH0_TOCN_POL_VSYNC; /* Vsync_POL                               */
        q_cnf.tcon_hsync_inv = LCD_CH0_TOCN_POL_HSYNC; /* Hsync_POL                               */
        q_cnf.tcon_de_inv = LCD_CH0_TOCN_POL_DE; /* DE_POL                                  */
        q_cnf.tcon_half = LCD_CH0_TCON_HALF; /* TCON reference timing, 1/2fH timing(HTP)   */
        q_cnf.tcon_ofset = LCD_CH0_TCON_OFFSET; /* TCON reference timing, offset Hsync signal timing */
        q_cnf.lcd_data_out_edge = LCD_CH0_OUT_EDGE; /* LCD_DATA_OUT_EDGE                           */
        q_cnf.lcd_outformat = LCD_CH0_OUT_FORMAT; /* LCD output format select                    */
        error = R_RVAPI_DispControlVDC (vdc_ch, VDC_ON, &q_cnf);
    }

    return error;
} /* End of function r_display_init() */

/*!****************************************************************************
 * Function Name: r_image_quality_adjustment
 * @brief         Image Quality Adjustment
 * @param[in]     vdc_ch    :
 * @retval        vdc_error_t
 ******************************************************************************/
vdc_error_t r_image_quality_adjustment(vdc_channel_t vdc_ch)
{
    vdc_error_t error;

    /***********************************************************************/
    /* Display out cakibration */
    /***********************************************************************/
    {
        vdc_calibr_bright_t bright;
        vdc_calibr_contrast_t contrast;
        vdc_calibr_dither_t dither;

        /* Bright */
        bright.pbrt_g = IMGC_BRIGHT_OUTCTL_OFFSET_G;
        bright.pbrt_b = IMGC_BRIGHT_OUTCTL_OFFSET_B;
        bright.pbrt_r = IMGC_BRIGHT_OUTCTL_OFFSET_R;

        /* Contrast */
        contrast.cont_g = IMGC_CONTRAST_OUTCTL_GAIN_G;
        contrast.cont_b = IMGC_CONTRAST_OUTCTL_GAIN_B;
        contrast.cont_r = IMGC_CONTRAST_OUTCTL_GAIN_R;

        /* Dither settings */
        dither.pdth_sel = IMGC_DITHER_MODE;
        dither.pdth_pa = IMGC_DITHER_2X2_PA;
        dither.pdth_pb = IMGC_DITHER_2X2_PB;
        dither.pdth_pc = IMGC_DITHER_2X2_PC;
        dither.pdth_pd = IMGC_DITHER_2X2_PD;
        error = R_RVAPI_DispCalibrationVDC (vdc_ch, IMGC_OUTCTL_CALIB_ROUTE, &bright, &contrast, &dither);
    }

    /***********************************************************************/
    /*  Display Gamma */
    /***********************************************************************/
    if (VDC_OK == error)
    {
        static const uint16_t gam_g_gain[VDC_GAM_GAIN_ADJ_NUM] =
        { IMGC_GAMMA_G_GAIN_00, IMGC_GAMMA_G_GAIN_01, IMGC_GAMMA_G_GAIN_02, IMGC_GAMMA_G_GAIN_03, IMGC_GAMMA_G_GAIN_04,
          IMGC_GAMMA_G_GAIN_05, IMGC_GAMMA_G_GAIN_06, IMGC_GAMMA_G_GAIN_07, IMGC_GAMMA_G_GAIN_08, IMGC_GAMMA_G_GAIN_09,
          IMGC_GAMMA_G_GAIN_10, IMGC_GAMMA_G_GAIN_11, IMGC_GAMMA_G_GAIN_12, IMGC_GAMMA_G_GAIN_13, IMGC_GAMMA_G_GAIN_14,
          IMGC_GAMMA_G_GAIN_15, IMGC_GAMMA_G_GAIN_16, IMGC_GAMMA_G_GAIN_17, IMGC_GAMMA_G_GAIN_18, IMGC_GAMMA_G_GAIN_19,
          IMGC_GAMMA_G_GAIN_20, IMGC_GAMMA_G_GAIN_21, IMGC_GAMMA_G_GAIN_22, IMGC_GAMMA_G_GAIN_23, IMGC_GAMMA_G_GAIN_24,
          IMGC_GAMMA_G_GAIN_25, IMGC_GAMMA_G_GAIN_26, IMGC_GAMMA_G_GAIN_27, IMGC_GAMMA_G_GAIN_28, IMGC_GAMMA_G_GAIN_29,
          IMGC_GAMMA_G_GAIN_30, IMGC_GAMMA_G_GAIN_31 };
        static const uint8_t gam_g_th[VDC_GAM_START_TH_NUM] =
        { IMGC_GAMMA_G_TH_01, IMGC_GAMMA_G_TH_02, IMGC_GAMMA_G_TH_03, IMGC_GAMMA_G_TH_04, IMGC_GAMMA_G_TH_05,
          IMGC_GAMMA_G_TH_06, IMGC_GAMMA_G_TH_07, IMGC_GAMMA_G_TH_08, IMGC_GAMMA_G_TH_09, IMGC_GAMMA_G_TH_10,
          IMGC_GAMMA_G_TH_11, IMGC_GAMMA_G_TH_12, IMGC_GAMMA_G_TH_13, IMGC_GAMMA_G_TH_14, IMGC_GAMMA_G_TH_15,
          IMGC_GAMMA_G_TH_16, IMGC_GAMMA_G_TH_17, IMGC_GAMMA_G_TH_18, IMGC_GAMMA_G_TH_19, IMGC_GAMMA_G_TH_20,
          IMGC_GAMMA_G_TH_21, IMGC_GAMMA_G_TH_22, IMGC_GAMMA_G_TH_23, IMGC_GAMMA_G_TH_24, IMGC_GAMMA_G_TH_25,
          IMGC_GAMMA_G_TH_26, IMGC_GAMMA_G_TH_27, IMGC_GAMMA_G_TH_28, IMGC_GAMMA_G_TH_29, IMGC_GAMMA_G_TH_30,
          IMGC_GAMMA_G_TH_31 };

        static const uint16_t gam_b_gain[VDC_GAM_GAIN_ADJ_NUM] =
        { IMGC_GAMMA_B_GAIN_00, IMGC_GAMMA_B_GAIN_01, IMGC_GAMMA_B_GAIN_02, IMGC_GAMMA_B_GAIN_03, IMGC_GAMMA_B_GAIN_04,
          IMGC_GAMMA_B_GAIN_05, IMGC_GAMMA_B_GAIN_06, IMGC_GAMMA_B_GAIN_07, IMGC_GAMMA_B_GAIN_08, IMGC_GAMMA_B_GAIN_09,
          IMGC_GAMMA_B_GAIN_10, IMGC_GAMMA_B_GAIN_11, IMGC_GAMMA_B_GAIN_12, IMGC_GAMMA_B_GAIN_13, IMGC_GAMMA_B_GAIN_14,
          IMGC_GAMMA_B_GAIN_15, IMGC_GAMMA_B_GAIN_16, IMGC_GAMMA_B_GAIN_17, IMGC_GAMMA_B_GAIN_18, IMGC_GAMMA_B_GAIN_19,
          IMGC_GAMMA_B_GAIN_20, IMGC_GAMMA_B_GAIN_21, IMGC_GAMMA_B_GAIN_22, IMGC_GAMMA_B_GAIN_23, IMGC_GAMMA_B_GAIN_24,
          IMGC_GAMMA_B_GAIN_25, IMGC_GAMMA_B_GAIN_26, IMGC_GAMMA_B_GAIN_27, IMGC_GAMMA_B_GAIN_28, IMGC_GAMMA_B_GAIN_29,
          IMGC_GAMMA_B_GAIN_30, IMGC_GAMMA_B_GAIN_31 };
        static const uint8_t gam_b_th[VDC_GAM_START_TH_NUM] =
        { IMGC_GAMMA_B_TH_01, IMGC_GAMMA_B_TH_02, IMGC_GAMMA_B_TH_03, IMGC_GAMMA_B_TH_04, IMGC_GAMMA_B_TH_05,
          IMGC_GAMMA_B_TH_06, IMGC_GAMMA_B_TH_07, IMGC_GAMMA_B_TH_08, IMGC_GAMMA_B_TH_09, IMGC_GAMMA_B_TH_10,
          IMGC_GAMMA_B_TH_11, IMGC_GAMMA_B_TH_12, IMGC_GAMMA_B_TH_13, IMGC_GAMMA_B_TH_14, IMGC_GAMMA_B_TH_15,
          IMGC_GAMMA_B_TH_16, IMGC_GAMMA_B_TH_17, IMGC_GAMMA_B_TH_18, IMGC_GAMMA_B_TH_19, IMGC_GAMMA_B_TH_20,
          IMGC_GAMMA_B_TH_21, IMGC_GAMMA_B_TH_22, IMGC_GAMMA_B_TH_23, IMGC_GAMMA_B_TH_24, IMGC_GAMMA_B_TH_25,
          IMGC_GAMMA_B_TH_26, IMGC_GAMMA_B_TH_27, IMGC_GAMMA_B_TH_28, IMGC_GAMMA_B_TH_29, IMGC_GAMMA_B_TH_30,
          IMGC_GAMMA_B_TH_31 };

        static const uint16_t gam_r_gain[VDC_GAM_GAIN_ADJ_NUM] =
        { IMGC_GAMMA_R_GAIN_00, IMGC_GAMMA_R_GAIN_01, IMGC_GAMMA_R_GAIN_02, IMGC_GAMMA_R_GAIN_03, IMGC_GAMMA_R_GAIN_04,
          IMGC_GAMMA_R_GAIN_05, IMGC_GAMMA_R_GAIN_06, IMGC_GAMMA_R_GAIN_07, IMGC_GAMMA_R_GAIN_08, IMGC_GAMMA_R_GAIN_09,
          IMGC_GAMMA_R_GAIN_10, IMGC_GAMMA_R_GAIN_11, IMGC_GAMMA_R_GAIN_12, IMGC_GAMMA_R_GAIN_13, IMGC_GAMMA_R_GAIN_14,
          IMGC_GAMMA_R_GAIN_15, IMGC_GAMMA_R_GAIN_16, IMGC_GAMMA_R_GAIN_17, IMGC_GAMMA_R_GAIN_18, IMGC_GAMMA_R_GAIN_19,
          IMGC_GAMMA_R_GAIN_20, IMGC_GAMMA_R_GAIN_21, IMGC_GAMMA_R_GAIN_22, IMGC_GAMMA_R_GAIN_23, IMGC_GAMMA_R_GAIN_24,
          IMGC_GAMMA_R_GAIN_25, IMGC_GAMMA_R_GAIN_26, IMGC_GAMMA_R_GAIN_27, IMGC_GAMMA_R_GAIN_28, IMGC_GAMMA_R_GAIN_29,
          IMGC_GAMMA_R_GAIN_30, IMGC_GAMMA_R_GAIN_31 };

        static const uint8_t gam_r_th[VDC_GAM_START_TH_NUM] =
        { IMGC_GAMMA_R_TH_01, IMGC_GAMMA_R_TH_02, IMGC_GAMMA_R_TH_03, IMGC_GAMMA_R_TH_04, IMGC_GAMMA_R_TH_05,
          IMGC_GAMMA_R_TH_06, IMGC_GAMMA_R_TH_07, IMGC_GAMMA_R_TH_08, IMGC_GAMMA_R_TH_09, IMGC_GAMMA_R_TH_10,
          IMGC_GAMMA_R_TH_11, IMGC_GAMMA_R_TH_12, IMGC_GAMMA_R_TH_13, IMGC_GAMMA_R_TH_14, IMGC_GAMMA_R_TH_15,
          IMGC_GAMMA_R_TH_16, IMGC_GAMMA_R_TH_17, IMGC_GAMMA_R_TH_18, IMGC_GAMMA_R_TH_19, IMGC_GAMMA_R_TH_20,
          IMGC_GAMMA_R_TH_21, IMGC_GAMMA_R_TH_22, IMGC_GAMMA_R_TH_23, IMGC_GAMMA_R_TH_24, IMGC_GAMMA_R_TH_25,
          IMGC_GAMMA_R_TH_26, IMGC_GAMMA_R_TH_27, IMGC_GAMMA_R_TH_28, IMGC_GAMMA_R_TH_29, IMGC_GAMMA_R_TH_30,
          IMGC_GAMMA_R_TH_31 };

        error = R_RVAPI_DispGammaVDC (vdc_ch, IMGC_GAMMA_ACTIVE, &gam_r_gain[0], &gam_r_th[0], &gam_g_gain[0],
                                       &gam_g_th[0], &gam_b_gain[0], &gam_b_th[0]);
    }

    return error;
} /* End of function r_image_quality_adjustment() */

