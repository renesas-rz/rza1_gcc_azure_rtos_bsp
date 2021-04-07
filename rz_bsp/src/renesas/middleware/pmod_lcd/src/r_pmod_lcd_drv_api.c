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
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************
* File Name    : r_pmod_lcd_drv_api.c
* Version      : 1.00
* Description  : PMOD LCD Driver
*******************************************************************************
* History      : DD.MM.YYYY Ver. Description
*              : 10.06.2018 1.00 First Release
******************************************************************************/

/******************************************************************************
  WARNING!  IN ACCORDANCE WITH THE USER LICENCE THIS CODE MUST NOT BE CONVEYED
  OR REDISTRIBUTED IN COMBINATION WITH ANY SOFTWARE LICENSED UNDER TERMS THE
  SAME AS OR SIMILAR TO THE GNU GENERAL PUBLIC LICENCE
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/

#include "compiler_settings.h"   /* Interchangeable compiler specific header */
#include "r_os_abstraction_api.h"
#include "r_pmod_lcd_drv_api.h"
#include "rspi.h"
#include "r_lcd_pmod.h"
#include "trace.h"


/******************************************************************************
Macro definitions
******************************************************************************/

/* Comment this line out to turn ON module trace in this file */
#undef _TRACE_ON_

#ifndef _TRACE_ON_
#undef TRACE
#define TRACE(x)
#endif

/*****************************************************************************
Function Prototypes
******************************************************************************/

static int_t okaya_open(st_stream_ptr_t stream_ptr);
static void   okaya_close(st_stream_ptr_t st_stream_ptr_t);
static int_t  okaya_control(st_stream_ptr_t st_stream_ptr_t, uint32_t ctlCode, void *pCtlStruct);

/*****************************************************************************
Constant Data
******************************************************************************/

/* Define the driver function table for this */
const st_r_driver_t g_pmod_okaya_lcd_driver =
{
    "LCD PMod OKAYA Display Driver",
    okaya_open,
    okaya_close,
    no_dev_io,
    no_dev_io,
    okaya_control,
    no_dev_get_version
};

/*****************************************************************************
Private Functions
******************************************************************************/

/******************************************************************************
Function Name: okaya_open
Description:   Function to open the device driver
Arguments:     IN  st_stream_ptr_t - Pointer to the file stream
Return value:  0 for success otherwise -1
******************************************************************************/
static int_t okaya_open(st_stream_ptr_t stream_ptr)
{
    /* not used */
    UNUSED_PARAM(stream_ptr);

    /* Initialise SPI Used by PMOD */
    R_SPI_Init();
    R_LCD_PmodInit();

    return 0;
}
/******************************************************************************
End of function  okaya_open
******************************************************************************/

/******************************************************************************
Function Name: okaya_close
Description:   Function to close the standard IN driver.
Arguments:     IN  st_stream_ptr_t - Pointer to the file stream
Return value:  none
******************************************************************************/
static void okaya_close(st_stream_ptr_t stream_ptr)
{
    /* Avoid unused parameter compiler warning */
    (void)stream_ptr;
}
/******************************************************************************
End of function  okaya_close
******************************************************************************/

/******************************************************************************
Function Name: okaya_control
Description:   Function to handle custom controls for the standard IN device
Arguments:     IN  st_stream_ptr_t - Pointer to the file stream
               IN  ctlCode - The custom control code
               IN  pCtlStruct - Pointer to the custom control structure
Return value:  0 for success -1 on error
******************************************************************************/
static int_t okaya_control(st_stream_ptr_t stream_ptr, uint32_t ctlCode, void *pCtlStruct)
{
    int8_t i_result = -1;

    /* Avoid unused parameter compiler warning */
    (void)stream_ptr;

    switch (ctlCode)
    {
        case CTL_PMOD_LCD_R_DISPLAY_ON:
        {
            i_result = 0;

            TRACE(("drvLCDPMod: CTL_PMOD_LCD_R_DISPLAY_ON Request\r\n"));
            R_LCD_DisplayOn();
            break;
        }

        case CTL_PMOD_LCD_R_DISPLAY_OFF:
        {
            i_result = 0;

            TRACE(("drvLCDPMod: CTL_PMOD_LCD_R_DISPLAY_OFF Request\r\n"));
            R_LCD_DisplayOff();
            break;
        }

        case CTL_PMOD_LCD_R_LOCK:
        {
            i_result = 0;

            TRACE(("drvLCDPMod: CTL_PMOD_LCD_R_LOCK Request\r\n"));
            R_LCD_ResourceLockPMOD();
            break;
        }

        case CTL_PMOD_LCD_R_UNLOCK:
        {
            i_result = 0;

            TRACE(("drvLCDPMod: CTL_PMOD_LCD_R_UNLOCK Request\r\n"));
            R_LCD_ResourceUnlockPMOD();
            break;
        }

        case CTL_PMOD_LCD_UPDATE_DISPLAY:
        {
            i_result = 0;

            TRACE(("drvLCDPMod: CTL_PMOD_LCD_UPDATE_DISPLAY Request\r\n"));
            R_LCD_UpdateDisplay();
            break;
        }

        case CTRL_PMOD_LCD_DRAW_BMP_IMAGE:
        {
            if (pCtlStruct)
            {
                /* st_image_def_pmod_lcd_t is required for this control, no validation implemented */
                st_image_def_pmod_lcd_t *p_image = (st_image_def_pmod_lcd_t *) pCtlStruct;
                i_result = 0;

                TRACE(("drvLCDPMod: CTRL_PMOD_LCD_DRAW_BMP_IMAGE Request\r\n"));
                R_LCD_DisplayBMPImage (p_image->image,
                                       p_image->image_width,
                                       p_image->image_height,
                                       p_image->loc_x,
                                       p_image->loc_y);
            }
            break;
        }

        case CTRL_PMOD_LCD_DRAW_RAW_IMAGE:
        {
            if (pCtlStruct)
            {
                /* st_image_def_pmod_lcd_t is required for this control, no validation implemented */
                st_image_def_pmod_lcd_t *p_image = (st_image_def_pmod_lcd_t *) pCtlStruct;
                i_result = 0;

                TRACE(("drvLCDPMod: CTRL_PMOD_LCD_DRAW_BMP_IMAGE Request\r\n"));
                R_LCD_DisplayRAWImage (p_image->image,
                                       p_image->image_width,
                                       p_image->image_height,
                                       p_image->loc_x,
                                       p_image->loc_y);
            }
            break;
        }

        case CTRL_PMOD_LCD_DISPLAY_STRING_AT_LINE:
        {
            if (pCtlStruct)
            {
                /* st_line_display_pmod_lcd_t is required for this control, no vailidation implemented */
                st_line_display_pmod_lcd_t *p_data = (st_line_display_pmod_lcd_t *) pCtlStruct;
                i_result = 0;

                TRACE(("drvLCDPMod: CTRL_PMOD_LCD_DISPLAY_STRING_AT_LINE Request\r\n"));
                R_LCD_DisplayStringAtLine (p_data->line_num, p_data->str);
            }
            break;
        }

        case CTRL_PMOD_LCD_DISPLAY_STR:
        {
            if (pCtlStruct)
            {
                /* uint8_t is required for this control, no vailidation implemented */
                uint8_t * str = (uint8_t *) pCtlStruct;
                i_result = 0;

                TRACE(("drvLCDPMod: CTRL_PMOD_LCD_DISPLAY_STR Request\r\n"));
                R_LCD_DisplayStr (str);

            }
            break;
        }

        case CTRL_PMOD_LCD_DISPLAY_CENTER:
        {
            if (pCtlStruct)
            {
                /* st_line_display_pmod_lcd_t is required for this control, no vailidation implemented */
                st_line_display_pmod_lcd_t *p_data = (st_line_display_pmod_lcd_t *) pCtlStruct;
                i_result = 0;

                TRACE(("drvLCDPMod: CTRL_PMOD_LCD_DISPLAY_CENTER Request\r\n"));
                R_LCD_DisplayCenter (p_data->line_num, p_data->str);
            }
            break;
        }

        case CTRL_PMOD_LCD_DISPLAY_CLEAR:
        {
            if (pCtlStruct)
            {
                /* uint32_t is required for this control, no vailidation implemented */
                uint32_t *p_col = (uint32_t *) pCtlStruct;
                i_result = 0;

                TRACE(("drvLCDPMod: CTRL_PMOD_LCD_DISPLAY_CLEAR Request\r\n"));
                R_LCD_DisplayClear(*p_col);
            }
            break;
        }

        case CTRL_PMOD_LCD_DISPLAY_CLEAR_LINE:
        {
            if (pCtlStruct)
            {
                /* uint8_t is required for this control, no validation implemented */
                uint8_t *p_line = (uint8_t *) pCtlStruct;
                i_result = 0;

                TRACE(("drvLCDPMod: CTRL_PMOD_LCD_DISPLAY_CLEAR_LINE Request\r\n"));
                R_LCD_DisplayClearLine(*p_line);
            }
            break;
        }

        case CTRL_PMOD_LCD_DISPLAY_SET_CURSOR:
        {
            if (pCtlStruct)
            {
                /* st_cursor_pos_pmod_lcd_t is required for this control, no validation implemented */
                st_cursor_pos_pmod_lcd_t *p_pos = (st_cursor_pos_pmod_lcd_t *) pCtlStruct;
                i_result = 0;

                TRACE(("drvLCDPMod: CTRL_PMOD_LCD_DISPLAY_SET_CURSOR Request\r\n"));
                R_LCD_DisplaySetCursor (p_pos->x, p_pos->y);

            }
            break;
        }

        case CTRL_PMOD_LCD_DISPLAY_SET_FONT_COLOUR:
        {
            if (pCtlStruct)
            {
                /* uint32_t is required for this control, no validation implemented */
                uint32_t *p_col = (uint32_t *) pCtlStruct;
                i_result = 0;

                TRACE(("drvLCDPMod: CTRL_PMOD_LCD_DISPLAY_SET_FONT_COLOUR Request\r\n"));
                R_LCD_DisplaySetFontColour(*p_col);

            }
            break;
        }

        case CTRL_PMOD_LCD_DISPLAY_SET_BACK_COLOUR:
        {
            if (pCtlStruct)
            {
                /* uint32_t is required for this control, no validation implemented */
                uint32_t *p_col = (uint32_t *) pCtlStruct;
                i_result = 0;

                TRACE(("drvLCDPMod: CTRL_PMOD_LCD_DISPLAY_SET_BACK_COLOUR Request\r\n"));
                R_LCD_DisplaySetBackColour(*p_col);
            }
            break;
        }

        default:
        {
            TRACE(("drvLCDPMod: Unknown control code\r\n"));
            break;
        }
    }
    return i_result;
}
/******************************************************************************
End of function  okaya_control
******************************************************************************/

/******************************************************************************
End  Of File
******************************************************************************/
