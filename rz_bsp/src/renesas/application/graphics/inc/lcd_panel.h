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
 * @headerfile     lcd_panel.h
 * @brief          Panel type selection
 * @version        1.00
 * @date           27.06.2018
 * H/W Platform    RZA1H
 *****************************************************************************/
 /*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 30.06.2018 1.00 First Release
 *****************************************************************************/

/* Multiple inclusion prevention macro */
#ifndef LCD_PANEL_H
#define LCD_PANEL_H

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include "mcu_board_select.h"

/******************************************************************************
 Macro definitions
 ******************************************************************************/

#if ( TARGET_BOARD == TARGET_BOARD_RSK )
#include "lcd/rsk_tft_clk.h"                 /* user define */
#include "lcd/rsk_tft_ch0.h"                 /* QE for Video Display Controller 5 output header file */
#include "lcd/rsk_tft_ch1.h"
#elif ( TARGET_BOARD == TARGET_BOARD_STREAM_IT2 )
#include "lcd/stream2_tft_clk.h"             /* user define */
#include "lcd/stream2_tft_ch0.h"             /* QE for Video Display Controller 5 output header file */
#else
#error ERROR: Invalid DISPLAY TYPE defined in lcd_panel.h...
#endif

#endif  /*  LCD_PANEL_H */
