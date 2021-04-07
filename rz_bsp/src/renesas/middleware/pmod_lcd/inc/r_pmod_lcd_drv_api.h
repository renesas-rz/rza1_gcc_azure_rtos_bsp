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
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
 *****************************************************************************/
/******************************************************************************
 * @headerfile     r_pmod_lcd_drv_api.h
 * @brief          Header file for PMOD Driver
 * @version        2.00
 * @date           27.06.2018
 * H/W Platform    RZA1H
 *****************************************************************************/
 /*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 18.06.2013 1.00
 *              : 21.03.2014 2.00
 *****************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef R_PMOD_LCD_DRV_API_H_INCLUDED
#define R_PMOD_LCD_DRV_API_H_INCLUDED

/**************************************************************************//**
 * @ingroup R_SW_PKG_93_PMOD_API PMOD
 * @defgroup R_SW_PKG_93_LCD_PMOD PMOD LCD Display API
 * @brief LCD Driver Functionality 
 * 
 * @anchor R_SW_PKG_93_PMOD_API_SUMMARY
 * @par Summary
 * 
 *          This module contains the necessary control codes,
 *          data structures and enumeration for a user to use the high level
 *          API for the PMOD Driver. 
 * 
 * @see RENESAS_APPLICATION_SOFTWARE_PACKAGE
 *
 * @see RENESAS_OS_ABSTRACTION  Renesas OS Abstraction interface
 * @{
 *****************************************************************************/
/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_devlink_wrapper.h"

/******************************************************************************
Constant Data
******************************************************************************/

#define REQUIRES_CFG_PMOD_UID (112)


/**
 @enum e_colour_code_pmod_lcd_t
 @brief DISPLAY COLOUR DEFINITIONS (16 bits) R5G6B5 format

 Only primary & secondary colours are defined here. Other colours can be created using RGB values. 0x00RRGGBB <br>
 Example 0x00FFFFFF is White (High byte is never used, RR = 0xFF, GG = 0xFF, BB = 0xFF)
*/
typedef enum
{
    R_PMOD_COL_BLACK      = (0x00000000), /**< RGB definition for Black used by PMOD display */
    R_PMOD_COL_RED        = (0x000000FF), /**< RGB definition for Red used by PMOD display */
    R_PMOD_COL_GREEN      = (0x0000FF00), /**< RGB definition for Green used by PMOD display */
    R_PMOD_COL_BLUE       = (0x00FF0000), /**< RGB definition for Blue used by PMOD display */
    R_PMOD_COL_YELLOW     = (0x0000FFFF), /**< RGB definition for Yellow used by PMOD display */
    R_PMOD_COL_CYAN       = (0x00FFFF00), /**< RGB definition for Cyan used by PMOD display */
    R_PMOD_COL_MAGENTA    = (0x00FF00FF), /**< RGB definition for Magenta used by PMOD display */
    R_PMOD_COL_WHITE      = (0x00FFFFFF)  /**< RGB definition for White used by PMOD display */
} e_colour_code_pmod_lcd_t;


/**
 @enum e_ctrl_code_pmod_lcd_t
 @brief The PMOD control functions <br>
 When using this driver please remember the following process <br>
 <br>
 call the control function CTL_PMOD_LCD_R_LOCK to grab control of the display <br>
 call the desired image DISPLAY or DRAW control functions to update the display's buffer <br>
 call the control function CTL_PMOD_LCD_UPDATE_DISPLAY to update the screen (if required) <br>
 call the control function CTL_PMOD_LCD_R_UNLOCK to release control of the display allowing others to update <br>
 <br>
*/
typedef enum
{
    CTL_PMOD_LCD_R_DISPLAY_ON   = (REQUIRES_CFG_PMOD_UID << 8), 
                                          /**< Enable Display (display can be written to whiles disabled) <br> 
                                               pCtlStruct is not used as there is no associated data with this control.*/
    
    CTL_PMOD_LCD_R_DISPLAY_OFF,           /**< Disable Display (display can be written to whiles disabled) <br> 
                                               pCtlStruct is not used as there is no associated data with this control.*/
                                               
    CTL_PMOD_LCD_R_LOCK,                  /**< Grabs screen lock, task holding lock should update the display  <br> 
                                               pCtlStruct is not used as there is no associated data with this control.*/
                                               
    CTL_PMOD_LCD_R_UNLOCK,                /**< Release screen lock <br> 
                                               pCtlStruct is not used as there is no associated data with this control.*/
                                               
    CTL_PMOD_LCD_UPDATE_DISPLAY,          /**< This function puts a 24-bit image on the 16-bit display, <br>
                                               colour reduction is embedded in display routine. <br> 
                                               pCtlStruct is not used as there is no associated data with this control.*/

    CTRL_PMOD_LCD_DRAW_BMP_IMAGE,         /**< Send bitmap Image (including BMP header) to with header to back buffer,
                                               call refresh to show <br> 
                                               pCtlStruct uses the ::st_image_def_pmod_lcd_t data structure */
                                               
    CTRL_PMOD_LCD_DRAW_RAW_IMAGE,         /**< Send bitmap Image to with header to back buffer, <br>
                                               call refresh to show <br> 
                                               pCtlStruct uses the ::st_image_def_pmod_lcd_t data structure */
    
     CTRL_PMOD_LCD_DISPLAY_STRING_AT_LINE, /**< This function controls LCD writes to the start of the line of the LCD.<br>
                                                 specified by position of the LCD. If position is greater than the <br>
                                               number of lines available then the LCD write will be to the last line. <br> 
                                               pCtlStruct uses the ::st_line_display_pmod_lcd_t data structure */
                                               
    CTRL_PMOD_LCD_DISPLAY_STR,            /**< Display the string at current cursor position <br> 
                                               pCtlStruct uses the ::uint8_t* data structure */
                                               
    CTRL_PMOD_LCD_DISPLAY_CENTER,         /**< Display the sting at the center of the specified line <br> 
                                               pCtlStruct uses the ::st_line_display_pmod_lcd_t data structure */
                                               
    CTRL_PMOD_LCD_DISPLAY_CLEAR,          /**< Clear the entire display <br> 
                                               pCtlStruct uses the ::uint32_t data structure */
                                               
    CTRL_PMOD_LCD_DISPLAY_CLEAR_LINE,     /**< Clear a specified line <br> 
                                               pCtlStruct uses the ::uint32_t data structure */
                                               
    CTRL_PMOD_LCD_DISPLAY_SET_CURSOR,     /**< Set the current cursor position <br> 
                                               pCtlStruct uses the ::st_cursor_pos_pmod_lcd_t data structure */
                                               
    CTRL_PMOD_LCD_DISPLAY_SET_FONT_COLOUR,/**< Set Font colour <br> 
                                               pCtlStruct uses the ::uint32_t data structure */
                                               
    CTRL_PMOD_LCD_DISPLAY_SET_BACK_COLOUR /**< Set Background colour <br> 
                                               pCtlStruct uses the ::uint32_t data structure */
} e_ctrl_code_pmod_lcd_t;



/**
 @struct st_image_def_pmod_lcd_t
 @brief The PMOD Image definition structure <br>

 @details Describes the attributes of a image, and its position of the PMOD display <br>
 <br>
 Used by the following controls  <br>
 e_ctrl_code_pmod_lcd_t::CTRL_PMOD_LCD_DRAW_BMP_IMAGE <br>
 e_ctrl_code_pmod_lcd_t::CTRL_PMOD_LCD_DRAW_RAW_IMAGE <br>
 <br>
*/
typedef struct
{
    uint8_t *image;         /**< Image data */
    uint8_t image_width;    /**< Image width in pixels */
    uint8_t image_height;   /**< Image height in pixels  */
    uint8_t loc_x;          /**< Screen horizontal offset  */
    uint8_t loc_y;          /**< Screen vertical offset */
} st_image_def_pmod_lcd_t;

/**
 @struct st_line_display_pmod_lcd_t
 @brief The PMOD text position attributes <br>

 @details Describes the attributes of a line of text, and its position on the PMOD display <br>
 <br>
 Used by the following controls  <br>
 e_ctrl_code_pmod_lcd_t::CTRL_PMOD_LCD_DISPLAY_STRING_AT_LINE <br>
 e_ctrl_code_pmod_lcd_t::CTRL_PMOD_LCD_DISPLAY_CENTER <br>
 <br>
*/
typedef struct
{
    uint8_t line_num;       /**< Line offset */
    uint8_t *str;           /**< Text to display */
} st_line_display_pmod_lcd_t;

/**
 @struct st_cursor_pos_pmod_lcd_t
 @brief The PMOD text cursor position attributes <br>

 @details Describes the attributes th the cursor text position on the PMOD display <br>
 <br>
 Used by the following controls  <br>
 e_ctrl_code_pmod_lcd_t::CTRL_PMOD_LCD_DISPLAY_SET_CURSOR <br>
 <br>
*/
typedef struct
{
    uint8_t x;              /**< Screen horizontal offset  */
    uint8_t y;              /**< Screen vertical offset  */
} st_cursor_pos_pmod_lcd_t;

extern const st_r_driver_t g_pmod_okaya_lcd_driver;

#endif                              /* R_PMOD_LCD_DRV_API_H_INCLUDED */
/**************************************************************************//**
 * @} (end addtogroup)
 *****************************************************************************/
/******************************************************************************
End  Of File
******************************************************************************/
