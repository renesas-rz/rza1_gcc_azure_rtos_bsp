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
 * File Name    : r_draw_jpeg.c
 * Version      : 1.00
 * Device(s)    : RZ/A1L
 * Tool-Chain   : GNUARM-NONE-EABI-v16.01
 * OS           : None
 * H/W Platform : Stream it! v2 board
 * Description  : Handles  communication with the capacitive controllers
 *               for the three screens of the Stream It board
 *******************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 01.08.2016 1.00 First Release
 ******************************************************************************/

/******************************************************************************
 WARNING!  IN ACCORDANCE WITH THE USER LICENCE THIS CODE MUST NOT BE CONVEYED
 OR REDISTRIBUTED IN COMBINATION WITH ANY SOFTWARE LICENSED UNDER TERMS THE
 SAME AS OR SIMILAR TO THE GNU GENERAL PUBLIC LICENCE
 ******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include "string.h"
#include "r_typedefs.h"
#include "r_vdc_portsetting.h"
#include "compiler_settings.h"
#include "r_rvapi_header.h"
#include "lcd_panel.h"
#include "r_draw_jpeg.h"
#include "r_display_init.h"
#include "r_jcu.h"
#include "jcu_swap.h"
#include "Image/JCU_ExampleImage.h"

/******************************************************************************
 Macro definitions
 ******************************************************************************/
/* diplay layer for rectangle */
#define GRAPHICS_BUFFER_NUM             (2u)
#define GRAPHICS_BUFFER_BYTE_PER_PIXEL  (2u)
#define GRAPHICS_BUFFER_STRIDE  ((( GRAPHICS_LAYER_WIDTH * GRAPHICS_BUFFER_BYTE_PER_PIXEL ) + 31u) & ~31u)
#define ALPHA_VAL_MAX                   (0xFF)

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/
static uint32_t  gs_drawing_buffer_index;
static uint32_t  gs_showing_buffer_index;
static psemaphore_t   gs_event_from_v_sync;  /* event_t */
static psemaphore_t   gs_event_from_JPEG;  /* event_t */
static psemaphore_t   gs_event_from_JPEG_fin;  /* event_t */
    /* event_t is more appropriate than semaphore_t. */
    /* But waiting with event_t wakes by the event during waiting */
    /* Before calling waiting function, R_JCU_TerminateAsync() sets the event. */

static void  R_TOUCH_on_v_sync_fromISR( vdc_int_type_t  in_InterruptType );
/******************************************************************************
 Imported global variables and functions (from other files)
 ******************************************************************************/
extern  uint8_t   video_buffer[ GRAPHICS_BUFFER_NUM ][ GRAPHICS_BUFFER_STRIDE * GRAPHICS_LAYER_HEIGHT ]   __attribute__ ((section(".VRAM_SECTION0")))  __attribute__((aligned( JCU_BUFFER_ALIGNMENT )));

/****************************************************************************/
/* Function Name : R_TOUCH_draw_JPEG_cursor                                 */
/* Explanation   : Draw a JPEG                                              */
/* PARAMETERS    : none                                                     */
/* RETURNS       : none                                                     */
/* NOTES         : Especially, none.                                        */
/****************************************************************************/
void  R_TOUCH_draw_JPEG_cursor( int32_t  in_PointX,  int32_t  in_PointY )
{
    bool_t           is_JCU_initialized = false;
    uintptr_t        physical_address_of_JPEG;
    uintptr_t        physical_address_of_RAW;
    uint8_t*         virtual_address_of_RAW;

    /* clear buffer  */
    memset(video_buffer[gs_drawing_buffer_index], 0, sizeof(video_buffer[0]));

    /* physical_address_of_JPEG = ... */
    physical_address_of_JPEG = (uintptr_t) g_Arrow_jpg;

    /* physical_address_of_RAW = ... */
    {
        int32_t  left_x      = in_PointX;
        int32_t  top_y       = in_PointY - g_Arrow_jpg_height + 1;
        int32_t  max_left_x  = GRAPHICS_LAYER_WIDTH  - g_Arrow_jpg_width;
        int32_t  max_top_y   = GRAPHICS_LAYER_HEIGHT - g_Arrow_jpg_height;

        if ( left_x < 0 )          { left_x = 0; }
        if ( left_x > max_left_x ) { left_x = max_left_x; }
        if ( top_y < 0 )           { top_y = 0; }
        if ( top_y > max_top_y )   { top_y = max_top_y; }

        virtual_address_of_RAW = &video_buffer[gs_drawing_buffer_index][(((uint32_t) left_x * GRAPHICS_BUFFER_BYTE_PER_PIXEL)
                        + ((uint32_t) top_y * GRAPHICS_BUFFER_STRIDE))];

        physical_address_of_RAW = (uintptr_t) virtual_address_of_RAW;
        physical_address_of_RAW &= (uintptr_t) ~(JCU_BUFFER_ALIGNMENT - 1);
    }

    /* Calls JCU API */
    {
        jcu_decode_param_t        decode;
        jcu_buffer_param_t        buffer;

        buffer.source.swapSetting       = JCU_SWAP_JPEG_NONE;
        buffer.source.address           = (uint32_t*) physical_address_of_JPEG;
        buffer.destination.address      = (uint32_t*) physical_address_of_RAW;
        buffer.lineOffset               = (int16_t)( GRAPHICS_BUFFER_STRIDE / GRAPHICS_BUFFER_BYTE_PER_PIXEL );
        { /* RGB565 */
            decode.decodeFormat            = JCU_OUTPUT_RGB565;
            buffer.destination.swapSetting = JCU_SWAP_RGB565_PIXEL0123;
            decode.outputCbCrOffset        = JCU_CBCR_OFFSET_0;
        }
        decode.alpha                 = ALPHA_VAL_MAX;
        decode.horizontalSubSampling = JCU_SUB_SAMPLING_1_1;
        decode.verticalSubSampling   = JCU_SUB_SAMPLING_1_1;

        R_JCU_Initialize( NULL );
        is_JCU_initialized = true;
        R_JCU_SelectCodec( JCU_DECODE );
        R_JCU_SetDecodeParam( &decode,  &buffer );

        //R_OS_WaitForSemaphore( gs_event_from_JPEG, 0 ); /* R_OSM_ResetEvent */

        R_JCU_StartAsync( (r_co_function_t) R_OS_ReleaseSemaphore, (void*) &gs_event_from_JPEG ); /* R_OS_SetEvent */
        R_OS_WaitForSemaphore( gs_event_from_JPEG,  R_OS_ABSTRACTION_PRV_EV_WAIT_INFINITE ); /* R_OS_WaitForEvent */
    }

    gs_showing_buffer_index = gs_drawing_buffer_index;
    gs_drawing_buffer_index = 1 - gs_drawing_buffer_index;

    {
        R_RVAPI_GraphChangeSurfaceVDC( VDC_CHANNEL_0,  VDC_LAYER_ID_0_RD,
            (void*) video_buffer[ gs_showing_buffer_index ] );
    }

    {
        R_OS_WaitForSemaphore( gs_event_from_v_sync,  0 ); /* R_OS_ResetEvent */

        R_OS_WaitForSemaphore( gs_event_from_v_sync,  R_OS_ABSTRACTION_PRV_EV_WAIT_INFINITE ); /* R_OS_WaitForEvent */
    }

    if ( is_JCU_initialized ) {
        //R_OS_WaitForSemaphore( gs_event_from_JPEG_fin,  0 ); /* R_OS_ResetEvent */

        R_JCU_TerminateAsync( (r_co_function_t) R_OS_ReleaseSemaphore,(void*) &gs_event_from_JPEG_fin ); /* R_OS_SetEvent */
        R_OS_WaitForSemaphore( gs_event_from_JPEG_fin,  R_OS_ABSTRACTION_PRV_EV_WAIT_INFINITE ); /* R_OS_WaitForEvent */
    }

} /* End of function R_TOUCH_draw_JPEG_cursor() */

/****************************************************************************/
/* Function Name : R_TOUCH_init_screen                                      */
/* Explanation   : Initialize screen for touch screen application           */
/* PARAMETERS    : none                                                     */
/* RETURNS       : none                                                     */
/* NOTES         : Especially, none.                                        */
/****************************************************************************/
void R_TOUCH_init_screen(void)
{
    vdc_error_t error;
    vdc_channel_t vdc_ch = VDC_CHANNEL_0;

    gs_drawing_buffer_index = 1;
    gs_showing_buffer_index = 0;
    R_OS_CreateSemaphore(&gs_event_from_v_sync,1 );  /* R_OS_CreateEvent */
    R_OS_CreateSemaphore(&gs_event_from_JPEG,  1 );  /* R_OS_CreateEvent */
    R_OS_CreateSemaphore(&gs_event_from_JPEG_fin,  1 );  /* R_OS_CreateEvent */

    memset(video_buffer, 0, sizeof(video_buffer));

    /***********************************************************************/
    /* display init (VDC output setting) */
    /***********************************************************************/
    {
        error = r_display_init (vdc_ch);
    }
    /***********************************************************************/
    /* Graphic Layer 0 RGB565 */
    /***********************************************************************/
    if (error == VDC_OK)
    {
        gr_surface_disp_config_t gr_disp_cnf;

#if (1) /* not use camera captured layer */
        gr_disp_cnf.layer_id         = VDC_LAYER_ID_0_RD;
#else   /* blend over camera captured image */
        gr_disp_cnf.layer_id         = VDC_LAYER_ID_2_RD;
#endif
        gr_disp_cnf.disp_area.hs_rel = 0;
        gr_disp_cnf.disp_area.hw_rel = GRAPHICS_LAYER_WIDTH;
        gr_disp_cnf.disp_area.vs_rel = 0;
        gr_disp_cnf.disp_area.vw_rel = GRAPHICS_LAYER_HEIGHT;
        gr_disp_cnf.fb_buff          = &video_buffer[gs_showing_buffer_index];
        gr_disp_cnf.fb_stride        = GRAPHICS_BUFFER_STRIDE;
        gr_disp_cnf.read_format      = VDC_GR_FORMAT_RGB565;
        gr_disp_cnf.clut_table       = NULL;
        gr_disp_cnf.read_ycc_swap    = VDC_GR_YCCSWAP_Y0CBY1CR;
        gr_disp_cnf.read_swap        = VDC_WR_RD_WRSWA_32_16BIT;
#if (1) /* not use camera captured data */
        gr_disp_cnf.disp_mode        = VDC_DISPSEL_CURRENT;
#else   /* blend over camera captured image */
        gr_disp_cnf.disp_mode        = VDC_DISPSEL_BLEND;
#endif
        error = R_RVAPI_GraphCreateSurfaceVDC(vdc_ch, &gr_disp_cnf);
    }

    /* Image Quality Adjustment */
    if (VDC_OK == error)
    {
        error = r_image_quality_adjustment(vdc_ch);
    }

    /* Enable signal output */
    if (VDC_OK == error)
    {
        /* Wait for register update */
        R_OS_TaskSleep(5);

        R_RVAPI_DispPortSettingVDC(vdc_ch, &VDC_LcdPortSetting);
    }

    /* Enable V-sync interrupt handler */
    if (VDC_OK == error)
    {
        error = R_RVAPI_InterruptEnableVDC(vdc_ch, VDC_INT_TYPE_VLINE, 0, R_TOUCH_on_v_sync_fromISR);
    }

    /* Draws Initial screen */
    R_TOUCH_draw_JPEG_cursor( GRAPHICS_LAYER_WIDTH / 2,  GRAPHICS_LAYER_HEIGHT / 2 );

} /* End of function R_TOUCH_init_screen() */

/****************************************************************************/
/* Function Name : R_TOUCH_uninit_screen                                    */
/* Explanation   : Un-initialize screen for touch screen application        */
/* PARAMETERS    : none                                                     */
/* RETURNS       : none                                                     */
/* NOTES         : Especially, none.                                        */
/****************************************************************************/
void R_TOUCH_uninit_screen(void)
{
    vdc_error_t error;
    vdc_channel_t vdc_ch = VDC_CHANNEL_0;

    /***********************************************************************/
    /* display init (VDC output setting) */
    /***********************************************************************/
    error = R_RVAPI_TerminateVDC(vdc_ch);
    if (VDC_OK != error)
    {
        while(1U);
    }

    /***********************************************************************/
    /* OS object uninit */
    /***********************************************************************/
    R_OS_DeleteSemaphore(gs_event_from_v_sync);  /* R_OS_DeleteEvent */
    R_OS_DeleteSemaphore(gs_event_from_JPEG);  /* R_OS_DeleteEvent */
    R_OS_DeleteSemaphore(gs_event_from_JPEG_fin);  /* R_OS_DeleteEvent */
} /* End of function R_TOUCH_uninit_screen() */

/****************************************************************************/
/* Function Name : R_TOUCH_on_v_sync_fromISR                                */
/* Explanation   : on_v_sync                                                */
/* PARAMETERS    : none                                                     */
/* RETURNS       : none                                                     */
/* NOTES         : Especially, none.                                        */
/****************************************************************************/
static void  R_TOUCH_on_v_sync_fromISR( vdc_int_type_t  in_InterruptType )
{
    UNUSED_PARAM(in_InterruptType);
    R_OS_ReleaseSemaphore( gs_event_from_v_sync ); /* R_OS_SetEvent */

} /* End of function R_TOUCH_on_v_sync_fromISR() */

