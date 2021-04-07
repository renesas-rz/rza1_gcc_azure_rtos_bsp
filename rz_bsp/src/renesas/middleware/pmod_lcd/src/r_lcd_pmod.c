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
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name     : lcd_pmod.c
* Device(s)     : RZ/A1H (R7S721001)
* Tool-Chain    : GNUARM-RZv13.01-EABI
* H/W Platform  : RSK+RZA1H CPU Board
* Description   : Contains functions for writing to the PMOD display. Please
*                 refer to the header file lcd_pmod.h for detail explanation
*******************************************************************************/
/*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 18.06.2013 1.00
*               : 21.03.2014 2.00
*******************************************************************************/

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/
/* Includes assembly level definitions */
#include <string.h>

/* Interchangeable compiler specific header */
#include "compiler_settings.h"

/* Default  type definition header */
#include "r_typedefs.h"

/* ASCII font header */
#include "ascii.h"

/* rspi Device Driver header */
#include "rspi.h"

/* Device driver header */
#include "dev_drv.h"

/* PMOD LCD controlling function prototypes & macros */
#include "r_lcd_pmod.h"

/* Comment this line out to turn ON module trace in this file */
#undef _TRACE_ON_

#ifndef _TRACE_ON_
#undef TRACE
#define TRACE(x)
#endif

#define PMOD_LCD_PRV_BMP_OFFSET (10u)
#define PMOD_LCD_PRV_GET_RED_COMP(a)   ((((a) >> 0x10) & 0xFF))
#define PMOD_LCD_PRV_GET_GREEN_COMP(a) ((((a) >> 0x08) & 0xFF))
#define PMOD_LCD_PRV_GET_BLUE_COMP(a)  (((a) & 0xFF))


/* PMOD LCD details */
typedef struct
{
    uint8_t  curx;
    uint8_t  cury;
    uint8_t  data_block_content;
    uint8_t  angle;

    uint32_t cmd;
    uint32_t font_colour;
    uint32_t back_colour;

    uint8_t  display_buffer[128][128 * 3];
    uint8_t  final_buffer[128][128 * 3];

} st_lcd_object_t;

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
static void init_pmod_lcd (void);
static void charput (uint8_t const val);
static void display_write_command (uint8_t const  cmd);
static void display_write_data (uint8_t data);
static void display_write_pixel (uint8_t const x, uint8_t const y,
                                 uint32_t const colour);
static void display_draw_horz_line (uint8_t const line, uint32_t const  colour);
static void display_set_addr_window (uint8_t xs, uint8_t ys,
                                     uint8_t xe, uint8_t ye);
static void display_write_char_column (uint8_t const x, uint8_t const y,
                                       uint8_t const pattern);
static void display_write_image (uint8_t const *data, int16_t count);
static void display_delay_ms (uint32_t  time_ms);

static  event_t s_pmod_lcd_lock = 0;

/*******************************************************************************
Global variables and functions
*******************************************************************************/
st_lcd_object_t g_my_lcd;

/*******************************************************************************
* Function Name : charput
* Description   : Displays a character at the current cursor position.
*                 Advances the cursor by 1 position.
*                 Cursor wraps to next line at the end of the line.
*                 Cursor wraps to Home position at the end of the display.
*
*                 The following control codes are accepted:
*
*                 LF increments y cursor position
*                 CR resets x cursor to 0.
*                 BS moves x cursor back by 1 place if possible.
* Argument      : none
* Return value  : none
*******************************************************************************/
static void charput (uint8_t const val)
{
    uint8_t * ptr;
    uint8_t   i;
    uint8_t   pattern;
    uint8_t   x;
    uint8_t   y;

    switch (val)
    {
        /* Carriage return character */
        case CR:
        {
            g_my_lcd.curx = 0;
        }
        break;

        /* Line feed character */
        case LF:
        {
            g_my_lcd.cury++;
            if (g_my_lcd.cury >= MAX_LINES)
            {
                g_my_lcd.cury = 0;
            }
        }
        break;

        /* Backspace character */
        case BS:
        {
            if (g_my_lcd.curx > 0)
            {
                g_my_lcd.curx--;
            }
        }
        break;

        /* Normal ` characters */
        default:
        {
            /* Ensure value is within the ASCII range */
            if ((val >= 0x20) && (val <= 0x7f))
            {
                /* Get pointer to start of representation of character
                 * in ASCII table */
                ptr = (uint8_t *)&ASCII_TABLE[val - 0x20][0];

                /* Copy character over */
                for (i = 0; i < FONT_WIDTH; i++)
                {
                    pattern = (*ptr++);
                    x = (uint8_t)((g_my_lcd.curx * FONT_WIDTH) + i);
                    y = (uint8_t)(g_my_lcd.cury * FONT_HEIGHT);
                    display_write_char_column(x, y, pattern);
                }

                /* move cursor to next co-ordinate on LCD */
                g_my_lcd.curx++;

                /* move to next row if reached the end of line */
                if (g_my_lcd.curx >= CHAR_PER_LINE)
                {
                    g_my_lcd.curx = 0;
                    g_my_lcd.cury++;
                    if (g_my_lcd.cury >= MAX_LINES)
                    {
                        /* loop back to top line if the
                         * last line of the display is reached */
                        g_my_lcd.cury = 0;
                    }
                }
            }
        }
        break;
    }
}
/*******************************************************************************
* End of function charput
*******************************************************************************/

/*******************************************************************************
* Function Name : display_delay_ms
* Description   : Delay routine for LCD or any other devices.
* Argument      : (uint32_t) time_ms - time in millisecond
* Return value  : None
*******************************************************************************/
static void display_delay_ms (uint32_t  time_ms)
{
    R_OS_TaskSleep(time_ms);
}
/*******************************************************************************
* End of function display_delay_ms
*******************************************************************************/

/*******************************************************************************
* Function Name : display_write_pixel
* Description   : Write the colour to the current cursor position
* Argument      : x,y - pixel position
*                 colour in R5G6B5 format
* Return value  : none
*******************************************************************************/
static void display_write_pixel (uint8_t const x,
                                 uint8_t const y,
                                 uint32_t const colour)
{
    g_my_lcd.display_buffer[SCREEN_HEIGHT - y][((x*3)+0)] = PMOD_LCD_PRV_GET_RED_COMP(colour);
    g_my_lcd.display_buffer[SCREEN_HEIGHT - y][((x*3)+1)] = PMOD_LCD_PRV_GET_GREEN_COMP(colour);
    g_my_lcd.display_buffer[SCREEN_HEIGHT - y][((x*3)+2)] = PMOD_LCD_PRV_GET_BLUE_COMP(colour);
}

/*******************************************************************************
* End of function display_write_pixel
*******************************************************************************/

/*******************************************************************************
* Function Name : display_draw_horz_line
* Description   : Draw horz line with the specified colour
* Argument      : line - line number to draw
*          colour in R5G6B5 format
* Return value  : None
*******************************************************************************/
static void display_draw_horz_line (uint8_t const line, uint32_t const colour)
{
    uint8_t x;
    uint8_t y;

    if (line < SCREEN_HEIGHT)
    {
        y = (uint8_t)(line + 1);

        for (x = 0; x < SCREEN_WIDTH; x++)
        {
            g_my_lcd.display_buffer[SCREEN_HEIGHT - y][((x*3)+0)] = PMOD_LCD_PRV_GET_RED_COMP(colour);
            g_my_lcd.display_buffer[SCREEN_HEIGHT - y][((x*3)+1)] = PMOD_LCD_PRV_GET_GREEN_COMP(colour);
            g_my_lcd.display_buffer[SCREEN_HEIGHT - y][((x*3)+2)] = PMOD_LCD_PRV_GET_BLUE_COMP(colour);
        }
    }
}
/*******************************************************************************
* End of function display_draw_horz_line
*******************************************************************************/
/*******************************************************************************
* Function Name : display_write_char_column
* Description   : Write a character at a current cursor position
* Argument      : x,y pixel position
*                 pattern to draw 1 = font colour, 0 = back colour
* Return value  : None
*******************************************************************************/
static void display_write_char_column (uint8_t const x,
                                         uint8_t const y, uint8_t const pattern)
{
    uint8_t mask;
    uint8_t i;
    uint8_t y1;

    mask = 0x80;
    for (i = 0; i < FONT_HEIGHT; i++)
    {
        y1 = (uint8_t)(y + i);
        if (pattern & mask)
        {
            display_write_pixel(x, y1,g_my_lcd.font_colour);
        }
        else
        {
            display_write_pixel(x, y1,g_my_lcd.back_colour);
        }
        mask = (uint8_t)(mask >> 1);
    }
}
/*******************************************************************************
* End of function display_write_char_column
*******************************************************************************/

/*******************************************************************************
* Function Name : init_pmod_lcd
* Description   : Initialises the LCD display.
* Argument      : none
* Return value  : none
*******************************************************************************/
static void init_pmod_lcd (void)
{
    /* Preset the output ports for i/o pins prior to setting as outputs */
    /* data cmd pin high to signify data */
    DATA_CMD_PIN |= DATA_CMD_PIN_BIT;

    /* de-assert chip select */
    ENABLE_PIN  |= ENABLE_PIN_BIT;

    /* software reset */
    display_write_command(ST7735_SWRESET);
    display_delay_ms(120);

    /* out of sleep mode */
    display_write_command(ST7735_SLPOUT);
    display_delay_ms(120);

    /* set colour mode */
    display_write_command(ST7735_COLMOD);

    /* 16 bit colour */
    display_write_data(0x05);
    display_delay_ms(10);

    /* memory access control (directions) */
    display_write_command(ST7735_MADCTL);

    /* row address/col address, bottom to top refresh */
    display_write_data(0xC8);

    /* normal mode */
    display_write_command(ST7735_NORON);
    display_delay_ms(10);

    R_LCD_DisplayClear(g_my_lcd.back_colour);
    R_LCD_UpdateDisplay();

    /* display on */
    display_write_command(ST7735_DISPON);
    display_delay_ms(120);

}
/*******************************************************************************
* End of function init_pmod_lcd
*******************************************************************************/

/*******************************************************************************
* Function Name : display_set_addr_window
* Description   : Draw horz line with the specified colour
* Argument      : xs, ys start pixel position
*                 ys, ye end pixel position
* Return value  : None
*******************************************************************************/
static void display_set_addr_window (uint8_t xs, uint8_t ys, uint8_t xe, uint8_t ye)
{
    if((ys <= ye) && ((xs <= xe) && ((ye < SCREEN_HEIGHT) && (xe < SCREEN_WIDTH))))
    {
        xs = (uint8_t)( xs + LEFT_MARGIN);
        xe = (uint8_t)( xe + LEFT_MARGIN);
        ys = (uint8_t)( ys + TOP_MARGIN);
        ye = (uint8_t)( ye + TOP_MARGIN);

        /* column addr set */
        display_write_command(ST7735_CASET);
        display_write_data(0x00);

        /* XSTART */
        display_write_data(xs);
        display_write_data(0x00);

        /* XEND   */
        display_write_data(xe);

        /* row addr set */
        display_write_command(ST7735_RASET);
        display_write_data(0x00);

        /* YSTART */
        display_write_data(ys);
        display_write_data(0x00);

        /* YEND   */
        display_write_data(ye);
    }
}
/*******************************************************************************
* End of function display_set_addr_window
*******************************************************************************/

/*******************************************************************************
* Function Name : display_write_image
* Description   : This function puts a single line of a 24-bit image on the
*                 16-bit display, colour reduction to 16-bit is embedded in
*                 function.
* Argument      : uint8_t  *data  - pointer to image data in RGB888 format RAW
*                 data no header
*                 uint16_t count  - Target image width * 3 (1 byte per colour
*                 (RGB) per pixel)
* Return value  : none
* Note          : DATA_CMD_PIN is set hi for data.
*******************************************************************************/
static void display_write_image (uint8_t const *data, int16_t count)
{
    uint16_t trans_data = 0u;
    uint16_t idata = 0u;
    uint8_t r;
    uint8_t g;
    uint8_t b;

    UNUSED_VARIABLE(trans_data);

    /* data cmd pin high to signify data */
    DATA_CMD_PIN |= DATA_CMD_PIN_BIT;

    /* assert chip select */
    ENABLE_PIN  &= (uint16_t)(~ENABLE_PIN_BIT);

    while(count > 0)
    {
        r = ((*(data++)) >>3);
        g = ((*(data++)) >>2);
        b = ((*(data++)) >>3);
        count = (int16_t)(count - 3);

        idata  = (uint16_t)(((b<<11) + (g<<5)) + r);

        g_my_lcd.data_block_content = (uint8_t)(idata>>8);

        /*send command */
        SPDR_1L = (uint16_t)(g_my_lcd.data_block_content << 8u);

        while (0u == (SPSR_1 & 0x80))
        {
            /*wait for send command completion */
        }

        /* check if number of receive data is reached */
        if (0u != (SPSR_1 & 0x80))
        {
            /* Read receiver buffer */
            trans_data = SPDR_1L;
        }

        if (0u != (SPSR_1 & 0x01))
        {
            while(1)
            {
                /* Overrun error occurred */
            };
        }

        g_my_lcd.data_block_content = (uint8_t)idata;

        /*send command */
        SPDR_1L = (uint16_t)(g_my_lcd.data_block_content << 8u);

        while (0u == (SPSR_1 & 0x80))
           {
            /*wait for send command completion */
           }

        /* check if number of receive data is reached */
        if (0u != (SPSR_1 & 0x80))
        {
            /* Read receiver buffer */
            trans_data = SPDR_1L;
        }

        if (0u != (SPSR_1 & 0x01))
        {
            while(1)
            {
                /* Overrun error occurred */
            };
        }
    }

    /* de-assert chip select */
    ENABLE_PIN  |= ENABLE_PIN_BIT;

    /* data cmd pin high to signify data */
    DATA_CMD_PIN |= DATA_CMD_PIN_BIT;
}
/*******************************************************************************
* End of function display_write_image
*******************************************************************************/

/*******************************************************************************
* Function Name : display_write_command
* Description   : Encapsulation of autocode SPI5 send, sends 1 cmd byte.
* Argument      : cmd - Command to write
* Return value  : None
* Note          : DATA_CMD_PIN is set lo for command
*******************************************************************************/
static void display_write_command (uint8_t const cmd)
{
    uint16_t trans_data = 0u;

    UNUSED_VARIABLE(trans_data);

    /* data cmd pin low to indicate a command */
    DATA_CMD_PIN &= (uint16_t)(~DATA_CMD_PIN_BIT);

    /* assert chip select */
    ENABLE_PIN  &= (uint16_t)(~ENABLE_PIN_BIT);

    g_my_lcd.cmd = (uint32_t)cmd;

    SPDR_1L = (uint16_t)(cmd << 8u);

    while (0u == (SPSR_1 & 0x80))
       {
        display_delay_ms(0);
       }

    /* check if number of receive data is reached */
    if (0u != (SPSR_1 & 0x80))
    {
        /* Read receiver buffer */
        trans_data = SPDR_1L;
    }

    if (0u != (SPSR_1 & 0x01))
    {
        while(1)
        {
            /* Overrun error occurred */
            display_delay_ms(0);
        };
    }

    /* de-assert chip select */
    ENABLE_PIN  = 0x1;

    /* data cmd pin high to signify data */
    DATA_CMD_PIN |= DATA_CMD_PIN_BIT;

}
/*******************************************************************************
* End of function display_write_command
*******************************************************************************/

/*******************************************************************************
* Function Name : display_write_data
* Description   : Write a data byte to the display
* Argument      : data - data to write
* Return value  : None
* Note          : DATA_CMD_PIN is set hi for data.
*******************************************************************************/
static void display_write_data (uint8_t const data)
{
    uint16_t trans_data = 0u;

    UNUSED_VARIABLE(trans_data);

    /* data cmd pin high to signify data */
    DATA_CMD_PIN |= DATA_CMD_PIN_BIT;

    /* assert chip select */
    ENABLE_PIN  &= (uint16_t)(~ENABLE_PIN_BIT);

    g_my_lcd.data_block_content = data;

    /* send command */
    SPDR_1L = (uint16_t)(g_my_lcd.data_block_content << 8u);

    while (0u == (SPSR_1 & 0x80))
       {
        display_delay_ms(0);
       }

    /* check if number of receive data is reached */
    if ((uint8_t)0u != (SPSR_1 & 0x80))
    {
        /* Read receiver buffer */
        trans_data = SPDR_1L;
    }

    if (0u != (SPSR_1 & 0x01))
    {
        while(1)
        {
            /* Overrun error occurred */
            display_delay_ms(0);
        };
    }

    /* de-assert chip select */
    ENABLE_PIN  = 0x1;

    /* data cmd pin high to signify data */
    DATA_CMD_PIN |= DATA_CMD_PIN_BIT;
}
/*******************************************************************************
* End of function display_write_data
*******************************************************************************/

/*******************************************************************************
* Function Name : update_display_buffer
* Description   : Write a data byte to the display
* Argument      : data - data to write
* Argument      : uint8_t  *image       - 24 bit RGB888 format RAW data no
*                                         header
*                 uint16_t image_width  - Target image width
*                 uint16_t image_height - Target image height
*                 uint16_t loc_x        - Image horizontal position on display
*                 uint16_t loc_y        - Image vertical position on display
* Return value  : None
*******************************************************************************/
static void update_display_buffer (uint8_t *image, uint8_t image_width,
                         uint8_t image_height, uint8_t loc_x, uint8_t loc_y,
                         uint32_t offset)
{
    int_t y = loc_y;
    int_t x = loc_x;
    int_t count = 0;
    int_t hdr_offset = 0;

    if(offset)
    {
        hdr_offset = (*(image + offset));
    }

    for(y = 0; y < image_height; y++)
    {
        for(x = 0; x < image_width; x++)
        {
            g_my_lcd.display_buffer[y + loc_y][(3 * (x + loc_x)) + 0] = (*(image + (hdr_offset  + (count++))));
            g_my_lcd.display_buffer[y + loc_y][(3 * (x + loc_x)) + 1] = (*(image + (hdr_offset  + (count++))));
            g_my_lcd.display_buffer[y + loc_y][(3 * (x + loc_x)) + 2] = (*(image + (hdr_offset  + (count++))));
        }
    }
}
/*******************************************************************************
* End of function update_display_buffer
*******************************************************************************/

/*******************************************************************************
* Function Name : rotate_image
* Description   : rotate image as specified by g_my_lcd.angle
* Argument      : none g_my_lcd.angle is set externally to this function
* Return value  : None
*******************************************************************************/
static void rotate_image(void)
{
    int_t y;
    int_t x;
    int_t count = 0;
    uint8_t *image = (uint8_t *)&g_my_lcd.display_buffer;

    switch (g_my_lcd.angle)
    {
        /* 90 degrees rotate */
        case 1:
            for(x = 0; x < SCREEN_WIDTH; x++)
            {
                for(y = (SCREEN_HEIGHT-1); y >= 0; y--)
                {
                    g_my_lcd.final_buffer[y][(3 * x) + 0] = (*(image + (count++)));
                    g_my_lcd.final_buffer[y][(3 * x) + 1] = (*(image + (count++)));
                    g_my_lcd.final_buffer[y][(3 * x) + 2] = (*(image + (count++)));
                }
            }
            break;

        /* 180 degrees rotate */
        case 2:
            for(y = (SCREEN_HEIGHT-1); y >= 0; y--)
            {
                for(x = (SCREEN_WIDTH-1); x >= 0; x--)
                {
                    g_my_lcd.final_buffer[y][(3 * x) + 0] = (*(image + (count++)));
                    g_my_lcd.final_buffer[y][(3 * x) + 1] = (*(image + (count++)));
                    g_my_lcd.final_buffer[y][(3 * x) + 2] = (*(image + (count++)));
                }
            }
            break;

        /* 270 degrees rotate */
        case 3:
            for(x = (SCREEN_WIDTH-1); x >= 0; x--)
            {
                for(y = 0; y < SCREEN_HEIGHT; y++)
                {
                    g_my_lcd.final_buffer[y][(3 * x) + 0] = (*(image + (count++)));
                    g_my_lcd.final_buffer[y][(3 * x) + 1] = (*(image + (count++)));
                    g_my_lcd.final_buffer[y][(3 * x) + 2] = (*(image + (count++)));
                }
            }
            break;

        /* 0 degrees rotate */
        case 0:
            for(y = 0; y < SCREEN_HEIGHT; y++)
            {
                for(x = 0; x < SCREEN_WIDTH; x++)
                {
                    g_my_lcd.final_buffer[y][(3 * x) + 0] = (*(image + (count++)));
                    g_my_lcd.final_buffer[y][(3 * x) + 1] = (*(image + (count++)));
                    g_my_lcd.final_buffer[y][(3 * x) + 2] = (*(image + (count++)));
                }
            }
        break;
        default:
        break;
    }
}
/*******************************************************************************
* End of function rotate_image
*******************************************************************************/

/*******************************************************************************
* Function Name : R_LCD_ImageRotation
* Description   : Set image orientation relative to the PMOD connector
* Argument      : angle
*                 0 : base of image 0 degrees clockwise to base of the PMOD
*                 1 : base of image 90 degrees clockwise to base of the PMOD
*                 2 : base of image 180 degrees clockwise to base of the PMOD
*                 3 : base of image 270 degrees clockwise to base of the PMOD
* Return value  : None
*******************************************************************************/
void R_LCD_ImageRotation(uint8_t angle)
{
    switch (angle)
    {
        case 0:
        case 1:
        case 2:
        case 3:
            {
                g_my_lcd.angle = angle;
            }
        break;

        default:
            {
                R_COMPILER_Nop();
            }
    }
}
/*******************************************************************************
* End of function R_LCD_ImageRotation
*******************************************************************************/

/*******************************************************************************
* Function Name : R_LCD_DisplayOn
* Description   : Enable Display (display can be written to whiles disabled)
* Argument      : None
* Return value  : None
*******************************************************************************/
void R_LCD_DisplayOn (void)
{
    /* display on */
    display_write_command(ST7735_DISPON);

    /* delay requirement stated in data sheet */
    display_delay_ms(120);
}
/*******************************************************************************
* End of function R_LCD_DisplayOn
*******************************************************************************/

/*******************************************************************************
* Function Name : R_LCD_DisplayOff
* Description   : Disable Display (display can be written to whiles disabled)
* Argument      : None
* Return value  : None
*******************************************************************************/
void R_LCD_DisplayOff (void)
{
    display_write_command(ST7735_DISPOFF);   /* display off */
    display_delay_ms(120);  /* delay requirement stated in data sheet */

}
/*******************************************************************************
* End of function R_LCD_DisplayOff
*******************************************************************************/

/*******************************************************************************
* Function Name : R_LCD_PmodInit
* Description   : Initialises the serial port and the LCD display. Clear the
*                 display with black background.
*                 Set the font colour to white.
* Argument      : none
* Return value  : none
*******************************************************************************/
void R_LCD_PmodInit (void)
{

    /* initialise defaults */
    g_my_lcd.curx = 0;
    g_my_lcd.cury = 0;
    g_my_lcd.cmd = 0;
    g_my_lcd.data_block_content = 0;
    g_my_lcd.font_colour = DEFAULT_FONT_COLOUR;
    g_my_lcd.back_colour = DEFAULT_BACKGROUND_COLOUR;

    /* initialise Standard PMOD display */
    init_pmod_lcd();
}
/*******************************************************************************
* End of function R_LCD_Init
*******************************************************************************/

/*******************************************************************************
* Function Name : R_LCD_DisplayStringAtLine
* Description   : This function controls LCD writes to the start of the line of
*                 the LCD. specified by position of the LCD. If position is
*                 greater than the number of lines available then the LCD write
*                 will be to the last line.
* Argument      : (uint8_t)position -   Line number of display
*                 (const char *) string -Pointer to data to be written to
*                                display. Last character should be null.
* Return value  : none
*******************************************************************************/
void R_LCD_DisplayStringAtLine (uint8_t const position,  uint8_t * const string)
{
    R_LCD_DisplaySetCursor(0, position);
    R_LCD_DisplayStr(string);
}
/*******************************************************************************
* End of function R_LCD_DisplayStringAtLine
*******************************************************************************/

/*******************************************************************************
* Function Name : R_LCD_DisplayStr
* Description   : Displays a string at the current cursor position
* Argument      : char *string - pointer to a string
* Return value  : none
*******************************************************************************/
void R_LCD_DisplayStr (uint8_t * str)
{
    uint16_t i;
    uint16_t size;

    size = (uint16_t)strlen((const char *)str);

    /* load characters into screen bitmap */
    for (i = 0; i < size; i++)
    {
        charput(*str++);
    }
}
/*******************************************************************************
* End of function R_LCD_DisplayStr
*******************************************************************************/

/*******************************************************************************
* Function Name : R_LCD_DisplayCenter
* Description   : Display a string in the center of the specified line.
* Argument      : uint8_t line_num - line number (0-15)
*                 pointer to a string.
* Return value  : none
*******************************************************************************/
void R_LCD_DisplayCenter (uint8_t const line_num, uint8_t * const str)
{
    uint8_t spaces;

    R_LCD_DisplaySetCursor(0, line_num);

    spaces = (uint8_t)((CHAR_PER_LINE - strlen((const char *)str)) / 2);
    while (spaces--)
    {
        charput(' ');
    }
    R_LCD_DisplayStr(str);
}
/*******************************************************************************
* End of function R_LCD_DisplayCenter
*******************************************************************************/

/*******************************************************************************
* Function Name : R_LCD_DisplayClear
* Description   : This function clears the screen
* Argument      : uint16_t colour - in R5G6B5 format
* Return value  : none
*******************************************************************************/
void R_LCD_DisplayClear (uint32_t colour)
{
    int_t y;
    int_t x;

    for(y = 0; y < SCREEN_HEIGHT; y++)
    {
        for(x = 0; x < SCREEN_WIDTH; x++)
        {
            g_my_lcd.display_buffer[y][(3 * x) + 0] = (uint8_t)PMOD_LCD_PRV_GET_RED_COMP(colour);
            g_my_lcd.display_buffer[y][(3 * x) + 1] = (uint8_t)PMOD_LCD_PRV_GET_GREEN_COMP(colour);
            g_my_lcd.display_buffer[y][(3 * x) + 2] = (uint8_t)PMOD_LCD_PRV_GET_BLUE_COMP(colour);
        }
    }
}
/*******************************************************************************
* End of function R_LCD_DisplayClear
*******************************************************************************/

/*******************************************************************************
* Function Name : R_LCD_DisplayBMPImage
* Description   : This function puts a 24-bit image on the 16-bit display,
*                 colour reduction is embedded in display routine.
* Argument      : uint8_t  *image       - 24 bit RGB888 format BMP data no
*                                         header
*                 uint16_t image_width  - Target image width
*                 uint16_t image_height - Target image height
*                 uint16_t loc_x        - Image horizontal position on display
*                 uint16_t loc_y        - Image vertical position on display
* Return value  : none
*******************************************************************************/
void R_LCD_DisplayBMPImage (uint8_t *image, uint8_t image_width,
                         uint8_t image_height, uint8_t loc_x, uint8_t loc_y)
{
    update_display_buffer(image, image_width, image_height,
                          loc_x, loc_y, PMOD_LCD_PRV_BMP_OFFSET);
}
/*******************************************************************************
* End of function R_LCD_DisplayBMPImage
*******************************************************************************/

/*******************************************************************************
* Function Name : R_LCD_DisplayRAWImage
* Description   : This function puts a 24-bit image on the 16-bit display,
*                 colour reduction is embedded in display routine.
* Argument      : uint8_t  *image       - 24 bit RGB888 format RAW data no
*                                         header
*                 uint16_t image_width  - Target image width
*                 uint16_t image_height - Target image height
*                 uint16_t loc_x        - Image horizontal position on display
*                 uint16_t loc_y        - Image vertical position on display
* Return value  : none
*******************************************************************************/
void R_LCD_DisplayRAWImage (uint8_t *image, uint8_t image_width,
                         uint8_t image_height, uint8_t loc_x, uint8_t loc_y)
{
    update_display_buffer(image, image_width, image_height,
                          loc_x, loc_y, 0);
}

/*******************************************************************************
* End of function R_LCD_DisplayRAWImage
*******************************************************************************/

/*******************************************************************************
* Function Name : R_LCD_UpdateDisplay
* Description   : This function puts a 24-bit image on the 16-bit display,
*                 colour reduction is embedded in display routine.
* Argument      : uint8_t  *image       - 24 bit RGB888 format RAW data no
*                                         header
*                 uint16_t image_width  - Target image width
*                 uint16_t image_height - Target image height
*                 uint16_t loc_x        - Image horizontal position on display
*                 uint16_t loc_y        - Image vertical position on display
* Return value  : none
*******************************************************************************/
void R_LCD_UpdateDisplay (void)
{
    uint8_t y;
    uint8_t image_width = SCREEN_WIDTH;
    uint8_t image_height = SCREEN_HEIGHT;
    uint8_t loc_x = 0;
    uint8_t loc_y = 0;

    rotate_image();

    for(y = 0; y < image_height; y++)
    {
        display_set_addr_window((uint8_t)(loc_x), (uint8_t)(loc_y + y),
              (uint8_t)(image_width - 1), (uint8_t)((loc_y + image_height) - 1));
        display_write_command(ST7735_RAMWR);
        display_write_image((&g_my_lcd.final_buffer[((image_height - y) - 1)][0]),
                            (int16_t)(image_width * 3));
    }
}
/*******************************************************************************
* End of function R_LCD_UpdateDisplay
*******************************************************************************/

/*******************************************************************************
* Function Name : R_LCD_DisplayClearLine
* Description   : Clear the specified line
* Argument      : uint8_t line numb 0 - 15
* Return value  : none
*******************************************************************************/
void R_LCD_DisplayClearLine (uint8_t line_num)
{
    uint8_t i;
    uint8_t y;

    if (line_num < MAX_LINES)
    {
        for (i = 0; i < FONT_HEIGHT; i++)
        {
            y = (uint8_t)((line_num * FONT_HEIGHT) + i);
            display_draw_horz_line(y, g_my_lcd.back_colour);
        }
        R_LCD_DisplaySetCursor(0, line_num);
    }
}
/*******************************************************************************
* End of function R_LCD_DisplayClearLine
*******************************************************************************/

/*******************************************************************************
* Function Name : R_LCD_DisplaySetCursor
* Description   : Set the cursor to the specified position. the position must be
*                 within the limits.
* Argument      : uint8_t x - X position
*                         y - Y position
* Return value  : none
*******************************************************************************/
void R_LCD_DisplaySetCursor (uint8_t const x, uint8_t const y)
{
    if (x < CHAR_PER_LINE)
    {
        g_my_lcd.curx = (x);
    }

    if (y < MAX_LINES)
    {
        g_my_lcd.cury = (y);
    }
}
/*******************************************************************************
* End of function R_LCD_DisplaySetCursor
*******************************************************************************/

/*******************************************************************************
* Function Name : R_LCD_DisplaySetFontColour
* Description   : Set the font (foreground) colour
* Argument      : uint16_t col - colour value in r5g6b5 format
* Return value  : none
*******************************************************************************/
void R_LCD_DisplaySetFontColour (uint32_t const col)
{
    g_my_lcd.font_colour = col;
}
/*******************************************************************************
* End of function R_LCD_DisplaySetFontColour
*******************************************************************************/

/*******************************************************************************
* Function Name : R_LCD_DisplaySetBackColour
* Description   : Set the background colour
* Argument      : uint16_t col - colour value in r5g6b5 format
* Return value  : none
*******************************************************************************/
void R_LCD_DisplaySetBackColour (uint32_t const col)
{
    g_my_lcd.back_colour = col;
}
/*******************************************************************************
* End of function R_LCD_DisplaySetBackColour
*******************************************************************************/

/*******************************************************************************
* Function Name : R_LCD_ResourceLockPMOD
* Description   : Get Software lock on PMOD resource
* Argument      : none
* Return value  : none
*******************************************************************************/
void R_LCD_ResourceLockPMOD (void)
{
    if(0 == s_pmod_lcd_lock)
    {
        s_pmod_lcd_lock = R_OS_CreateMutex();
    }

    R_OS_EventWaitMutex(&s_pmod_lcd_lock, R_OS_ABSTRACTION_PRV_EV_WAIT_INFINITE);
}
/*******************************************************************************
* End of function R_LCD_ResourceLockPMOD
*******************************************************************************/

/*******************************************************************************
* Function Name : R_LCD_ResourceUnlockPMOD
* Description   : Release Software lock on PMOD resource
* Argument      : none
* Return value  : none
*******************************************************************************/
void R_LCD_ResourceUnlockPMOD (void)
{
    R_OS_EventReleaseMutex(&s_pmod_lcd_lock);
}
/*******************************************************************************
* End of function R_LCD_ResourceUnlockPMOD
*******************************************************************************/

