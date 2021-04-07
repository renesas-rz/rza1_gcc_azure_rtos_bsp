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
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
 *****************************************************************************/
/******************************************************************************
 * @headerfile     r_lcd_pmod.h
 * @brief          This Header file contains the Macro Definitions & prototypes
 *                 for the functions used in lcd.c
 * @version        3.00
 * @date           24.04.2019
 * H/W Platform    Renesas Starter Kit+ for RZ/A1H
 *****************************************************************************/
 /*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 18.06.2013 1.00
 *              : 21.03.2014 2.00
 *              : 24.04.2019 3.00 Porting for RZ/A1H
 *****************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef LCD_H
#define LCD_H

/**************************************************************************//**
 * @ingroup R_SW_PKG_93_POSIX_MIDDLEWARE
 * @defgroup R_SW_PKG_93_PMOD_API PMOD API
 * @brief PMOD Driver
 *
 * @anchor R_SW_PKG_93_LCD_PMOD_SUMMARY
 * @par Summary
 *
 *           This function is created to drive the Okaya LCD display with
 *           either ST7735 or ST7715 driver device. The commands for both
 *           the devices are the same.
 *
 *           The display is controlled using the SPI bus. In this example,
 *           the SCI5 is used. This can be modified to the SCI connected to
 *           the PMOD interface. The SCI driver file will also be required.
 *
 *           The display memory has an offset with respect to the actual
 *           pixel. This is not documented but realised from driving the
 *           display. The offset is set as LEFT MARGIN and TOP MARGIN.
 *           This offset is catered for internally, so as far as the user
 *           is concerned, cursor position 0,0 is the top left pixel.
 *
 *           The simplest procedure to run the display is as follows:
 *           Init_LCD(); Initialise the serial port and set up the display.
 *
 *           Clear the display.
 *           The font colour is set to white and background colour to black.
 *
 *           DisplaySetFontColour(COL_YELLOW);
 *                              set the font colour to desired colour
 *           DisplaySetBackColour(COL_BLUE);
 *                              set the background colour to desired value
 *           DisplayCenter(1,"Renesas");
 *                              write a title on line 1 of the display.
 *
 * @anchor R_SW_PKG_93_LCD_PMOD_INSTANCES
 * @par Known Implementations:
 * This driver is used in the RZA1H Software Package.
 * @see RENESAS_APPLICATION_SOFTWARE_PACKAGE
 * @see RENESAS_OS_ABSTRACTION  Renesas OS Abstraction interface
 * @{
 *****************************************************************************/
/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* Defines standard variable types used in this file */
#include <stdint.h>
#include "iodefine_cfg.h"
#include "r_pmod_lcd_drv_api.h"

#include "mcu_board_select.h"

/*******************************************************************************
Macro Definitions
*******************************************************************************/

/*******************************************************************************
*  SCREEN
*
*  The screen size is 128 x 128 pixels, with coordinate 0,0 at the top left.
*  The display controller is ST7735 or ST7715.
*
*******************************************************************************/
#define SCREEN_HEIGHT (128)            /* 16 lines @ 8 bits = 128. */
#define SCREEN_WIDTH  (128)

#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)

/* DATA/COMMAND select pin SIT2:P9.5 = P9_5*/
#define DATA_CMD_PIN              (GPIO.P9)
/* DATA/COMMAND select pin bit */
#define DATA_CMD_PIN_BIT          ((uint16_t)0x0020)

/* Display/enable pin SSL1 = 0P 6.13 */
#define ENABLE_PIN                (GPIO.P6)
/* Display enable pin */
#define ENABLE_PIN_BIT            ((uint16_t)0x2000)

/* Vbat enable pin */
#define VBAT_ENABLE_PIN           (GPIO.PM9)
/* Vbat enable pin */
#define VBAT_ENABLE_PIN_BIT       ((uint16_t)0x0020)

#elif (TARGET_BOARD == TARGET_BOARD_RSK)

/* DATA/COMMAND select pin P4_15 = P4_15*/
#define DATA_CMD_PIN              (GPIO.P4)
/* DATA/COMMAND select pin bit */
#define DATA_CMD_PIN_BIT          ((uint16_t)0x8000)

/* DATA/COMMAND select pin P4_15 = P4_15*/
#define DATA_CMD_PIN2             (GPIO.P3)
/* DATA/COMMAND select pin bit */
#define DATA_CMD_PIN2_BIT         ((uint16_t)0x0080)

/* Display/enable pin PMOD1_CS = 0P 1_4 */
#define ENABLE_PIN                (GPIO.P1)
/* Display enable pin */
#define ENABLE_PIN_BIT            ((uint16_t)0x0010)

#endif /* TARGET_BOARD */

/* Automatic calculation of parameters */

 /* including a space */
#define FONT_WIDTH                (6u)
/* including 1 pixel space */
#define FONT_HEIGHT               (8u)
#define MAX_LINES                 (SCREEN_HEIGHT / FONT_HEIGHT)
#define CHAR_PER_LINE             (SCREEN_WIDTH / FONT_WIDTH)

/* Allow 2 pixel margin on the left and the top */
#define LEFT_MARGIN (2u)
#define TOP_MARGIN  (3u)
#define CR (0x0d)
#define LF (0x0a)
#define BS (0x08)

#define DEFAULT_FONT_COLOUR       (R_PMOD_COL_BLACK)
#define DEFAULT_BACKGROUND_COLOUR (R_PMOD_COL_WHITE)

/*******************************************************************************

  DISPLAY COMMAND SET ST7735

*******************************************************************************/
#define ST7735_NOP      (0x0)
#define ST7735_SWRESET  (0x01)
#define ST7735_SLPIN    (0x10)
#define ST7735_SLPOUT   (0x11)
#define ST7735_PTLON    (0x12)
#define ST7735_NORON    (0x13)
#define ST7735_INVOFF   (0x20)
#define ST7735_INVON    (0x21)
#define ST7735_DISPOFF  (0x28)
#define ST7735_DISPON   (0x29)
#define ST7735_CASET    (0x2A)
#define ST7735_RASET    (0x2B)
#define ST7735_RAMWR    (0x2C)
#define ST7735_COLMOD   (0x3A)
#define ST7735_MADCTL   (0x36)
#define ST7735_FRMCTR1  (0xB1)
#define ST7735_INVCTR   (0xB4)
#define ST7735_DISSET5  (0xB6)
#define ST7735_PWCTR1   (0xC0)
#define ST7735_PWCTR2   (0xC1)
#define ST7735_PWCTR3   (0xC2)
#define ST7735_VMCTR1   (0xC5)
#define ST7735_PWCTR6   (0xFC)
#define ST7735_GMCTRP1  (0xE0)
#define ST7735_GMCTRN1  (0xE1)

/* delay for delay counter */
#define DELAY_TIMING    (0x08)

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
/**
* @brief          Initialises the serial port and the LCD display. Clear the
*                 display with black background.
*                 Set the font colour to white.
*
* @return         None.
*/
void R_LCD_PmodInit (void);

/**
* @brief          This function controls LCD writes to the start of the line of
*                 the LCD. specified by position of the LCD. If position is
*                 greater than the number of lines available then the LCD write
*                 will be to the last line.
*
* @param[in]      position:   Line number of display
* @param[in]      string:     Pointer to data to be written to display.
*                             Last character should be null.
* @return         None.
*/
void R_LCD_DisplayStringAtLine (uint8_t const position,  uint8_t * const string);

/**
* @brief          Displays a string at the current cursor position
*
* @param[in]      string:  pointer to a string
*
* @return         None.
*/
void R_LCD_DisplayStr (uint8_t * str);

/**
* @brief          Display a string in the center of the specified line.
*
* @param[in]      line_num: line number (0-15)
* @param[in]      str:      pointer to a string.
*
* @return         None.
*/
void R_LCD_DisplayCenter (uint8_t const line_num, uint8_t * const str);

/**
* @brief          This function clears the screen
*
* @param[in]      colour: Colour in R5G6B5 format
*
* @return         None.
*/
void R_LCD_DisplayClear (uint32_t colour);

/**
* @brief          Clear the specified line
*
* @param[in]      line_num:  line numb 0 - 15
*
* @return         None.
*/
void R_LCD_DisplayClearLine(uint8_t line_num);

/**
* @brief          Set the cursor to the specified position. the position must be
*                 within the limits.
*
* @param[in]      x:  X position
* @param[in]      y:  Y position
*
* @return         None
*/
void R_LCD_DisplaySetCursor (uint8_t const x, uint8_t const y);

/**
* @brief          Set the font (foreground) colour
*
* @param[in]      col: colour value in r5g6b5 format
*
* @return         None
*/
void R_LCD_DisplaySetFontColour (uint32_t const col);

/**
* @brief          Set the background colour
*
* @param[in]      col: colour value in r5g6b5 format
*
* @return         None
*/
void R_LCD_DisplaySetBackColour (uint32_t const col);

/**
* @brief          This function puts a 24-bit image on the 16-bit display,
*                 colour reduction is embedded in display routine.
*
* @param[in]      image:        24 bit RGB888 format BMP data no header
* @param[in]      image_width:  Target image width
* @param[in]      image_height: Target image height
* @param[in]      loc_x:        Image horizontal position on display
* @param[in]      loc_y:        Image vertical position on display
*
* @return         None
*/
void R_LCD_DisplayBMPImage (uint8_t *image, uint8_t image_width,
                         uint8_t image_height, uint8_t loc_x, uint8_t loc_y);

/**
* @brief          This function puts a 24-bit image on the 16-bit display,
*                 colour reduction is embedded in display routine.
*
* @param[in]      image:        24 bit RGB888 format RAW data no header
* @param[in]      image_width:  Target image width
* @param[in]      image_height: Target image height
* @param[in]      loc_x:        Image horizontal position on display
* @param[in]      loc_y:        Image vertical position on display
*
* @return         None
*/
void R_LCD_DisplayRAWImage (uint8_t *image, uint8_t image_width,
                         uint8_t image_height, uint8_t loc_x, uint8_t loc_y);

/**
* @brief          This function puts a 24-bit image on the 16-bit display,
*                 colour reduction is embedded in display routine.
*
* @param[in]      image:        24 bit RGB888 format RAW data no header
* @param[in]      image_width:  Target image width
* @param[in]      image_height: Target image height
* @param[in]      loc_x:        Image horizontal position on display
* @param[in]      loc_y:        Image vertical position on display
*
* @return         None
*/
void R_LCD_UpdateDisplay (void);

/**
* @brief          Enable Display (display can be written to whiles disabled)
*
* @return         None
*/
void R_LCD_DisplayOn (void);

/**
* @brief          Disable Display (display can be written to whiles disabled)
*
* @return         None
*/
void R_LCD_DisplayOff (void);

/**
* @brief          Set image orientation relative to the PMOD connector
*
* @param[in]      angle: 0 - base of image 0 degrees clockwise to base of the
*                            PMOD <BR>
*                        1 - base of image 90 degrees clockwise to base of the
*                            PMOD <BR>
*                        2 - base of image 180 degrees clockwise to base of the
*                            PMOD <BR>
*                        3 - base of image 270 degrees clockwise to base of the
*                            PMOD <BR>
* @return         None
*/
void R_LCD_ImageRotation(uint8_t angle);

/**
* @brief          Get Software lock on PMOD resource
*
* @return         None
*/
void R_LCD_ResourceLockPMOD (void);

/**
* @brief          Release Software lock on PMOD resource
*
* @return         None
*/
void R_LCD_ResourceUnlockPMOD (void);

#endif /* LCD_H */
/**************************************************************************//**
 * @} (end addtogroup)
 *****************************************************************************/
