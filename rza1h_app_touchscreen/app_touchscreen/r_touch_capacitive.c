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
 * File Name    : r_touch_capacitive.c
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#include "iodefine_cfg.h"
#include "compiler_settings.h"
#include "application_cfg.h"
#include "dev_drv.h"
#include "control.h"
#include "r_devlink_wrapper.h"

#include "gpio_iobitmask.h"

#include "tp_if.h"
#include "r_intc.h"
#include "rza_io_regrw.h"
#include "r_task_priority.h"
#include "r_touch_capacitive.h"

#include "r_draw_jpeg.h"


/******************************************************************************
 Macro definitions
 ******************************************************************************/

/* FocalTech CTP Controller */
#define RZA1H_APP_I2C_FT5216    (0x70)

#define RECTANGLE_SIZE          (5)

#define TP_INT_PRI              (30uL)
#define TP_TSK_PRI              (R_OS_TASK_MAIN_TASK_PRI+1)

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/
/* I2C related functions */
static void tpevt_cb_func( int_t nId, TP_TouchEvent_st* psTouchEvt );

/******************************************************************************
 Imported global variables and functions (from other files)
 ******************************************************************************/
static pst_comset_t    s_dsp_console;
static int_t           an_tp_evt_id[6];

/******************************************************************************
 Public Functions
 ******************************************************************************/

/******************************************************************************
 Private Functions
 ******************************************************************************/

/***********************************************************************************************************************
 * Function Name: touch_demo
 * Description  : Touch screen demonstration
 * Arguments    : void *parameters
 * Return Value : none
 ***********************************************************************************************************************/
static void touch_demo (void *parameters)
{
    pst_comset_t console = parameters;
    fprintf(console->p_out,"Touch Demo: supporting %2d touch points\r\n", 1);

    /* endless loop */
    // while (control(R_DEVLINK_FilePtrDescriptor(s_dsp_console->p_in), CTL_GET_RX_BUFFER_COUNT, NULL) == 0)
    while(1)
    {
        R_OS_TaskSleep(5);
    }

    fgetc(console->p_in);
}
/***********************************************************************************************************************
 End of function touch_task
 ***********************************************************************************************************************/

/****************************************************************************/
/* Function Name : tpevt_cb_func                                            */
/* Explanation   : touch event callback function                            */
/* PARAMETERS    : [IN]nId        : event ID                                */
/*               : [IN]psTouchEvt : touch information                       */
/* RETURNS       : none                                                     */
/* NOTES         : Especially, none.                                        */
/****************************************************************************/
static void tpevt_cb_func( int_t nId, TP_TouchEvent_st* psTouchEvt )
{
    UNUSED_PARAM(nId);
    uint32_t disp_x = psTouchEvt->sFinger[0].unPosX;
    uint32_t disp_y = psTouchEvt->sFinger[0].unPosY;

    printf("Touch: x = %-4d, y = %-4d", (int)disp_x, (int)disp_y);

    switch(psTouchEvt->sFinger[0].eState)
    {
        case TPEVT_ENTRY_UP:
            printf("[ UP ]\r");
            break;
        case TPEVT_ENTRY_DOWN:
            printf("[DOWN]\r");
            break;
        case TPEVT_ENTRY_MOVE:
            printf("[MOVE]\r");
            break;
        default:
            printf("      \r");
            break;
    }

    R_TOUCH_draw_JPEG_cursor( disp_x,  disp_y );
}
/***********************************************************************************************************************
 End of function touch_task
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_TOUCH_ApplicationMain
 * Description  : Creates touch screen task
 * Arguments    : None
 * Return Value : none
 ***********************************************************************************************************************/
void R_TOUCH_ApplicationMain(pst_comset_t pCom)
{

    s_dsp_console = pCom;
    printf("Touch panel sample program start\r\n");

    /* initialise capacitive touch screen */
    TouchPanel_Init();

    /* initialize screen */
    R_TOUCH_init_screen();

    /* open the I2C channel1 driver */
    if(0 > TouchPanel_Open(GRAPHICS_LAYER_WIDTH, GRAPHICS_LAYER_HEIGHT, TP_INT_PRI, TP_TSK_PRI, R_OS_ABSTRACTION_PRV_DEFAULT_STACK_SIZE))
    {
        printf( "I2c driver can not loaded, demo not possible\r\n");
    }
    else
    {
        printf( "I2c driver loaded initializing demo\r\n");

        printf( "Touch the LCD to display the contact point on this console\r\n\r\n");
        printf( "Press any key to terminate demo\r\n\r\n");

        fflush(pCom->p_out);

        an_tp_evt_id[0] = TouchPanel_EventEntry(TPEVT_ENTRY_ALL, 0, 0, GRAPHICS_LAYER_WIDTH, GRAPHICS_LAYER_HEIGHT, &tpevt_cb_func);

        /* run the demo */
        touch_demo(pCom);

        printf( "\r\n\r\n");

        printf( "Touch panel demo terminated\r\n");

        TouchPanel_Close();
        R_TOUCH_uninit_screen();

    }
    fflush(pCom->p_out);
}
/***********************************************************************************************************************
 End of function R_TOUCH_ApplicationMain
 ***********************************************************************************************************************/


/******************************************************************************
 End  Of File
 ******************************************************************************/
