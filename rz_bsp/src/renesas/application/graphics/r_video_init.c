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
 * File Name :    video_init.c
 * @file          video_init.c
 * $Rev: $
 * $Date:: #$
 * @brief         Video setting for VDC5
 ******************************************************************************/

/*******************************************************************************
 Includes <System Includes>, "Project Includes"
 *******************************************************************************/
#include "mcu_board_select.h"

#include <stdio.h>
#include "cmsis_os.h"

#include "r_typedefs.h"

#include "lcd_panel.h"
#include "r_camera_module.h"
#include "r_rvapi_header.h"

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
#if ( TARGET_BOARD == TARGET_BOARD_RSK )
extern void VDC5_CameraPortSetting(uint32_t param);
#elif ( TARGET_BOARD == TARGET_BOARD_STREAM_IT2 )
extern void CEU_PortSetting(uint32_t param);
#endif

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/
vdc_error_t video_quality_adjustment(vdc_channel_t vdc_ch);
ceu_error_t video_init_ceu(void);

#if (TARGET_BOARD == TARGET_BOARD_RSK)
/*!****************************************************************************
 * Function Name: video_init_vdc5
 * @brief         Video setting
 * @param[in]     vdc_ch    :
 * @retval        vdc_error_t
 ******************************************************************************/
vdc_error_t video_init_vdc(const vdc_channel_t vdc_ch)
{
    vdc_error_t error;

#if ((INPUT_SELECT == ANALOG_NTSC)||(INPUT_SELECT == ANALOG_PAL))
    {
        vdec_channel_t vdec_ch;
        vdec_adc_vinsel_t vdec_adc_vinsel;
        ana_video_col_sys_t video_col_sys;
        digital_in_t * digital;

        /* VDC5 analog video input init */
        digital = NULL;
        error = R_RVAPI_VideoControlVDC (vdc_ch, digital);

        /* Wait for Color system determination */
        R_OS_TaskSleep(500);

        /* VDEC init */
        if (VDC_CHANNEL_0 == vdc_ch)
        {
            vdec_ch = VDEC_CHANNEL_0;
        }
        else
        {
            vdec_ch = VDEC_CHANNEL_1;
        }
        vdec_adc_vinsel = VDEC_ADC_VINSEL_VIN1;
        video_col_sys = GRPH_COL_SYS_UNKNOWN;

        video_col_sys = R_RVAPI_AnalogVideDecodeDVDEC (vdec_ch, vdec_adc_vinsel);
        if (GRPH_COL_SYS_UNKNOWN == video_col_sys)
        {
            printf ("[ERROR] Color system unknown.\n");
        }
    }
#elif (INPUT_SELECT ==  CMOS_CAM)
    {
        digital_in_t digital;

        /* vdc5 digital video input init */
        digital.inp_format = VDC5_EXTIN_FORMAT_BT601;
        digital.inp_pxd_edge = VDC5_EDGE_RISING;
        digital.inp_endian_on = VDC5_OFF;
        digital.inp_swap_on = VDC5_OFF;
        digital.inp_vs_inv = VDC5_SIG_POL_NOT_INVERTED;
        digital.inp_hs_inv = VDC5_SIG_POL_NOT_INVERTED;
        digital.inp_h_edge_sel = VDC5_EXTIN_REF_H_EAV;
        digital.inp_f525_625 = VDC5_EXTIN_LINE_525;
        digital.inp_h_pos = VDC5_EXTIN_H_POS_CBYCRY;
        error = R_RVAPI_VideoControlVDC (vdc_ch, &digital);
    }

    /* vdc5 digital video input port setting */
    if (VDC5_OK == error)
    {
        R_RVAPI_VideoPortSettingVDC5 (vdc_ch, VDC5_CameraPortSetting);
    }
#endif

    return error;
} /* End of function video_init_vdc5() */
#endif /* ((TARGET_BOARD == TARGET_BOARD_RSK)||(TARGET_BOARD == TARGET_BOARD_GENMAI)) */

#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
/*!****************************************************************************
 * Function Name: video_init_ceu
 * @brief         Video setting for ceu
 * @param[in]     None    :
 * @retval        ceu_error_t
 ******************************************************************************/
ceu_error_t video_init_ceu(void)
{
    ceu_error_t ceu_error;

    {
        ceu_config_t config;
        ceu_cap_rect_t cap;


        R_RVAPI_InitializeCEU ();

        R_RVAPI_PortSettingCEU (&CEU_PortSetting);

        /* CEU mode */
        config.jpg = CAP_CEU_MODE;

        /* Capture timing */
        cap.hofst = CAP_CEU_HS;
        cap.vofst = CAP_CEU_VS;
        cap.hwdth = CAP_CEU_HW;
        cap.vwdth = CAP_CEU_VW;
        config.cap = &cap;

        /* Clip area not support */
        config.clp = NULL;

        /* Data bus */
        config.dtif = CAP_CEU_DATAWIDTH; /* Data pin 8bit  */

        /* input order */
        config.dtary = CAP_CEU_DTARY;

        /* Write Data Swap */
        config.cols = CAP_CEU_SWAP_32; /* 32bit swap */
        config.cows = CAP_CEU_SWAP_16; /* 16bit swap */
        config.cobs = CAP_CEU_SWAP_08; /*  8bit swap */

        /* Signal polarity */
        config.hdpol = CAP_CEU_HPOL; /* Hsync */
        config.vdpol = CAP_CEU_VPOL; /* Vsync */

        ceu_error = R_RVAPI_OpenCEU (&config);
    }

    return ceu_error;
} /* End of function video_init_ceu() */
#endif /* (TARGET_BOARD == TARGET_BOARD_STREAM_IT2) */

/*!****************************************************************************
 * Function Name: video_quality_adjustment
 * @brief         Video Quality Adjustment
 * @param[in]     vdc_ch    :
 * @retval        vdc_error_t
 ******************************************************************************/
vdc_error_t video_quality_adjustment(vdc_channel_t vdc_ch)
{
    vdc_error_t error = VDC_OK;

    {
        vdc_color_matrix_t color_matrix;

        /* Operating mode */
        color_matrix.module = IMGC_BRIGHT_INPUTCTL_MODULE;

        /* Mode Select */
        color_matrix.mtx_mode = IMGC_BRIGHT_INPUTCTL_MODE;

        /* Bright */
        color_matrix.offset[VDC_COLORMTX_OFFST_YG] = IMGC_BRIGHT_INPUTCTL_OFFSET_YG;
        color_matrix.offset[VDC_COLORMTX_OFFST_B]  = IMGC_BRIGHT_INPUTCTL_OFFSET_B;
        color_matrix.offset[VDC_COLORMTX_OFFST_R]  = IMGC_BRIGHT_INPUTCTL_OFFSET_R;

        /* Contrast */
        color_matrix.gain[VDC_COLORMTX_GAIN_GG] = IMGC_CONTRAST_INPUTCTL_GAIN_GG;
        color_matrix.gain[VDC_COLORMTX_GAIN_GB] = IMGC_CONTRAST_INPUTCTL_GAIN_GB;
        color_matrix.gain[VDC_COLORMTX_GAIN_GR] = IMGC_CONTRAST_INPUTCTL_GAIN_GR;
        color_matrix.gain[VDC_COLORMTX_GAIN_BG] = IMGC_CONTRAST_INPUTCTL_GAIN_BG;
        color_matrix.gain[VDC_COLORMTX_GAIN_BB] = IMGC_CONTRAST_INPUTCTL_GAIN_BB;
        color_matrix.gain[VDC_COLORMTX_GAIN_BR] = IMGC_CONTRAST_INPUTCTL_GAIN_BR;
        color_matrix.gain[VDC_COLORMTX_GAIN_RG] = IMGC_CONTRAST_INPUTCTL_GAIN_RG;
        color_matrix.gain[VDC_COLORMTX_GAIN_RB] = IMGC_CONTRAST_INPUTCTL_GAIN_RB;
        color_matrix.gain[VDC_COLORMTX_GAIN_RR] = IMGC_CONTRAST_INPUTCTL_GAIN_RR;
        error = R_RVAPI_VideoCalibrationVDC (vdc_ch, &color_matrix);
    }

    if (VDC_OK == error)
    {
        vdc_enhance_sharp_t sharp_param;
        vdc_enhance_lti_t lti_param;
        vdc_period_rect_t enh_area;

        /* Sharpness of H1 */
        sharp_param.hrz_sharp[VDC_IMGENH_SHARP_H1].shp_clip_o = IMGC_SHARPNESS_H1_CLIP_O;
        sharp_param.hrz_sharp[VDC_IMGENH_SHARP_H1].shp_clip_u = IMGC_SHARPNESS_H1_CLIP_U;
        sharp_param.hrz_sharp[VDC_IMGENH_SHARP_H1].shp_gain_o = IMGC_SHARPNESS_H1_GAIN_O;
        sharp_param.hrz_sharp[VDC_IMGENH_SHARP_H1].shp_gain_u = IMGC_SHARPNESS_H1_GAIN_U;
        sharp_param.hrz_sharp[VDC_IMGENH_SHARP_H1].shp_core = IMGC_SHARPNESS_H1_CORE;

        /* Sharpness of H2 */
        sharp_param.hrz_sharp[VDC_IMGENH_SHARP_H2].shp_clip_o = IMGC_SHARPNESS_H2_CLIP_O;
        sharp_param.hrz_sharp[VDC_IMGENH_SHARP_H2].shp_clip_u = IMGC_SHARPNESS_H2_CLIP_U;
        sharp_param.hrz_sharp[VDC_IMGENH_SHARP_H2].shp_gain_o = IMGC_SHARPNESS_H2_GAIN_O;
        sharp_param.hrz_sharp[VDC_IMGENH_SHARP_H2].shp_gain_u = IMGC_SHARPNESS_H2_GAIN_U;
        sharp_param.hrz_sharp[VDC_IMGENH_SHARP_H2].shp_core = IMGC_SHARPNESS_H2_CORE;

        if (IMGC_SHARPNESS_H2_LPF == 0)
        {
            sharp_param.shp_h2_lpf_sel = VDC5_OFF;
        }
        else
        {
            sharp_param.shp_h2_lpf_sel = VDC5_ON;
        }

        /* Sharpness of H3 */
        sharp_param.hrz_sharp[VDC_IMGENH_SHARP_H3].shp_clip_o = IMGC_SHARPNESS_H3_CLIP_O;
        sharp_param.hrz_sharp[VDC_IMGENH_SHARP_H3].shp_clip_u = IMGC_SHARPNESS_H3_CLIP_U;
        sharp_param.hrz_sharp[VDC_IMGENH_SHARP_H3].shp_gain_o = IMGC_SHARPNESS_H3_GAIN_O;
        sharp_param.hrz_sharp[VDC_IMGENH_SHARP_H3].shp_gain_u = IMGC_SHARPNESS_H3_GAIN_U;
        sharp_param.hrz_sharp[VDC_IMGENH_SHARP_H3].shp_core = IMGC_SHARPNESS_H3_CORE;

        /* LTI H2 */
        if (IMGC_SHARPNESS_LTI_H2_LPF == 0)
        {
            lti_param.lti_h2_lpf_sel = VDC5_OFF;
        }
        else
        {
            lti_param.lti_h2_lpf_sel = VDC5_ON;
        }
        lti_param.lti[VDC_IMGENH_LTI1].lti_inc_zero = IMGC_SHARPNESS_LTI_H2_ZERO;
        lti_param.lti[VDC_IMGENH_LTI1].lti_gain = IMGC_SHARPNESS_LTI_H2_GAIN;
        lti_param.lti[VDC_IMGENH_LTI1].lti_core = IMGC_SHARPNESS_LTI_H2_CORE;

        /* LTI H4 */
        if (IMGC_SHARPNESS_LTI_H4_MEDTAP_SEL == VDC_LTI_MDFIL_SEL_ADJ2)
        {
            lti_param.lti_h4_median_tap_sel = VDC_LTI_MDFIL_SEL_ADJ2;
        }
        else
        {
            lti_param.lti_h4_median_tap_sel = VDC_LTI_MDFIL_SEL_ADJ1;
        }

        lti_param.lti[VDC_IMGENH_LTI2].lti_inc_zero = IMGC_SHARPNESS_LTI_H4_ZERO;
        lti_param.lti[VDC_IMGENH_LTI2].lti_gain = IMGC_SHARPNESS_LTI_H4_GAIN;
        lti_param.lti[VDC_IMGENH_LTI2].lti_core = IMGC_SHARPNESS_LTI_H4_CORE;

        /* Sharpness Effective area */
        enh_area.hs = IMGC_SHARPNESS_HS;
        enh_area.hw = IMGC_SHARPNESS_HW;
        enh_area.vs = IMGC_SHARPNESS_VS;
        enh_area.vw = IMGC_SHARPNESS_VW;
        error = R_RVAPI_VideoSharpnessLtiVDC (vdc_ch, IMGC_IMG_IMPRV_ID, IMGC_SHARPNESS_ACTIVE, &sharp_param,
                                              IMGC_SHARPNESS_LTI_ACTIVE, &lti_param, &enh_area);

    }

    return error;
} /* End of function video_quality_adjustment() */
