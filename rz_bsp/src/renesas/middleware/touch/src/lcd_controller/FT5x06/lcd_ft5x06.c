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
 * http://www.renesas.com/disclaimer*
 * Copyright (C) 2017 Renesas Electronics Corporation. All rights reserved.
 *******************************************************************************/

/**************************************************************************//**
 * @file         lcd_ft5x06.c
 * $Rev: 30 $
 * $Date:: 2016-12-21 12:02:44 +0900#$
 * @brief        LCD Driver functions
 ******************************************************************************/
/*******************************************************************************
 Includes <System Includes>, "Project Includes"
 *******************************************************************************/
#include    "mcu_board_select.h"
#include "r_riic_drv_sc_cfg.h"
#include "r_typedefs.h"

#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)

    #include    <stdio.h>
    #include    <string.h>
    #include    <fcntl.h>

    #include    "FreeRTOS.h"
    #include    "r_typedefs.h"
    #include    "r_errno.h"
    #include    "control.h"
    #include    "r_devlink_wrapper.h"
    #include    "lcd_ft5x06.h"
    #include    "lcd_ft5x06_int.h"
    #include    "r_riic_drv_sc_cfg.h"

/******************************************************************************
 Macro definitions
 ******************************************************************************/

/******************************************************************************
 Enumerated Types
 ******************************************************************************/

/******************************************************************************
 Structures
 ******************************************************************************/

/***********************************************************************************
 Global Vaiables
 ***********************************************************************************/
uint32_t sLcdSemIdTsk;
uint32_t sLcdSemIdAcc;

/*******************************************************************************
 Private global variables and functions
 *******************************************************************************/
SCOPE_STATIC uint32_t unEvtMsg;
SCOPE_STATIC LCDEVT_ENTRY LcdEvt_Entry[LCDEVT_ENTRY_MAX];
//static int_t *nLcdkitIoifErr;
int_t hi2c1 = ( -1);

/**************************************************************************//**
 * Function Name: LCD_Ft5x06_Open
 * @brief         Open LCD driver.
 *
 *                Description:<br>
 *
 * @param         [in]uint32_t unIrqLv   : IRQ interrupt level
 * @retval          0 : Operation successfull.
 *                 -1 : Error occured.
 ******************************************************************************/
int_t LCD_Ft5x06_Open (const uint32_t unIrqLv, int16_t nTskPri, uint32_t unTskStk)
{
    UNUSED_PARAM(unTskStk);
    UNUSED_PARAM(nTskPri);

    int_t nRet;
    int_t error = -1;
    st_r_drv_riic_create_t riic_clock;

    /* set the clock frequency for the I2C channel */
    riic_clock.frequency = RIIC_FREQUENCY_100KHZ;

    nRet = 0;
    memset( &LcdEvt_Entry, 0, sizeof(LcdEvt_Entry));
    unEvtMsg = 0;

    /* open the I2C channel1 driver */
    hi2c1 = open(DEVICE_INDENTIFIER "iic1", O_RDWR);

    if (( -1) == hi2c1)
    {
        nRet = -1;
    }
    else
    {
        /* Create the I2C channel */
        error = control(hi2c1, CTL_RIIC_CREATE, &riic_clock);

        if (0 > error)
        {
            nRet = -1;
        }
    }

    /** create access semaphore for driver */
    if (nRet >= 0)
    {
        if (R_OS_CreateSemaphore( &sLcdSemIdAcc, 1) != true)
        {
            DBG_printf_ERR("[ERROR] semaphore for internal access is not created\n");
            nRet = -1;
        }
    }

    /** create synchronous semaphore for task */
    if (nRet >= 0)
    {
        if (R_OS_CreateSemaphore( &sLcdSemIdTsk, 0) != true)
        {
            DBG_printf_ERR("[ERROR] semaphore for task is not created\n");
            nRet = -1;
            R_OS_DeleteSemaphore( &sLcdSemIdAcc);
        }
    }

    /* start LCD interrupt */
    if (nRet >= 0)
    {
        nRet = LCD_Ft5x06_Int_Open(unIrqLv);
        if (nRet < 0)
        {
            R_OS_DeleteSemaphore( &sLcdSemIdAcc);
            R_OS_DeleteSemaphore( &sLcdSemIdTsk);
        }
    }

    if ((( -1) == nRet) && (( -1) != hi2c1))
    {
        close(hi2c1);
    }

    return nRet;
}

/**************************************************************************//**
 * Function Name: LCD_Ft5x06_Close
 * @brief         Close LCD driver.
 *
 *                Description:<br>
 *
 * @param         None.
 * @retval          0 : Operation successfull.
 *                 -1 : Error occured.
 ******************************************************************************/
int_t LCD_Ft5x06_Close (void)
{
    int_t nRet;

    nRet = 0;

    /* stop LCD interrupt */
    nRet = LCD_Ft5x06_Int_Close();

    /** delete access semaphore */
    if (sLcdSemIdAcc != 0)
    {
        R_OS_DeleteSemaphore( &sLcdSemIdAcc);
        sLcdSemIdAcc = 0;
    }
    else
    {
        DBG_printf_ERR("[ERROR] semaphore for internal access is not deleted\n");
        nRet = -1;
    }

    /** delete synchronous semaphore */
    if (sLcdSemIdTsk != 0)
    {
        R_OS_DeleteSemaphore( &sLcdSemIdTsk);
        sLcdSemIdTsk = 0;
    }
    else
    {
        DBG_printf_ERR("[ERROR] semaphore for task is not deleted\n");
        nRet = -1;
    }

    if (( -1) != hi2c1)
    {
        close(hi2c1);
    }

    return nRet;
}

/**************************************************************************//**
 * Function Name: LCD_Ft5x06_WriteCmd
 * @brief         Write command to LCD device via RIIC.
 *
 *                Description:<br>
 *
 * @param         [in]uint16_t unDevAddr : device address
 * @param         [in]uint8_t uData      : data
 * @param         [in]uint32_t unSize    : size of data
 * @retval          0 : Operation successfull.
 *                 -1 : Error occured.
 ******************************************************************************/
uint8_t LCD_Ft5x06_WriteCmd (const uint16_t unDevAddr, const uint8_t uData, const uint32_t unSize)
{
    int_t nRet;

    st_r_drv_riic_config_t i2c_write;

    /* cast to uint8_t */
    i2c_write.device_address = (uint8_t)unDevAddr;
    i2c_write.sub_address = 0;
    i2c_write.number_of_bytes = unSize;

    /* cast to uint8_t pointer */
    i2c_write.p_data_buffer = (uint8_t *) &uData;

    nRet = control(hi2c1, CTL_RIIC_WRITE, &i2c_write);

    return (uint8_t)nRet;
}

/**************************************************************************//**
 * Function Name: LCD_Ft5x06_ReadCmd
 * @brief         Write command to LCD device and read data via RIIC.
 *
 *                Description:<br>
 *
 * @param         [in]uint16_t unDevAddr : device address
 * @param         [out]uint8_t *puData   : pointer to receive buffer
 * @param         [in]uint32_t unSize    : size of data
 * @retval          0 : Operation successfull.
 *                 -1 : Error occured.
 ******************************************************************************/
uint8_t LCD_Ft5x06_ReadCmd (const uint16_t unDevAddr, uint8_t *puData, const uint32_t unSize)
{
    int_t nRet;

    st_r_drv_riic_config_t i2c_read;

    /* cast to uint8_t */
    i2c_read.device_address = (uint8_t)unDevAddr;
    i2c_read.sub_address = 0;
    i2c_read.number_of_bytes = unSize;
    i2c_read.p_data_buffer = puData;

    nRet = control(hi2c1, CTL_RIIC_READ_NEXT, &i2c_read);

    return (uint8_t)nRet;
}

/**************************************************************************//**
 * Function Name: LCD_Ft5x06_EventEntry
 * @brief         Enter callback event to event table.
 *
 *                Description:<br>
 *
 * @param         [in]LcdEvt_EntryType eType : event type
 * @param         [in]void *function         : callback function
 * @retval         0..(LCDEVT_ENTRY_MAX-1) : event ID.
 *                -1                       : Error occured.
 ******************************************************************************/
int_t LCD_Ft5x06_EventEntry (const LcdEvt_EntryType eType, const LcdCBFunc function)
{
    int_t nI;
    LCDEVT_ENTRY *Entry;

    nI = -1;

    if ((eType > LCDEVT_ENTRY_NONE) && (eType <= LCDEVT_ENTRY_MAX))
    {
        Entry = &LcdEvt_Entry[eType - 1];
        if (Entry->mode == LCDEVT_ENTRY_NONE)
        {
            Entry->mode = eType;
            Entry->function = function;
            nI = eType - 1;

            /* enable LCD interrupt */
            LCD_Ft5x06_Int_Start();
        }
    }
    /** Return ERROR ( -1 ), when the event entry fails. */
    return nI;
}

/**************************************************************************//**
 * Function Name: LCD_Ft5x06_EventErase
 * @brief         Erase callback event from event table.
 *
 *                Description:<br>
 *
 * @param         [in]int_t nId              : event ID
 * @retval          0 : Operation successfull.
 *                 -1 : Error occured.
 ******************************************************************************/
int_t LCD_Ft5x06_EventErase (const int_t nId)
{
    int_t ercd = 0;

    if ((nId < 0) || (nId >= LCDEVT_ENTRY_MAX))
    {
        ercd = -1; /* id error */
    }
    else
    {
        LcdEvt_Entry[nId].mode = LCDEVT_ENTRY_NONE;
    }

    return ercd;
}

/**************************************************************************//**
 * Function Name: LCD_Ft5x06_StartInt
 * @brief         Enable interrupt of assigned type.
 *
 *                Description:<br>
 *
 * @param         [in]LcdEvt_EntryType eType : interrupt type
 * @retval          0 : Operation successfull.
 ******************************************************************************/
int_t LCD_Ft5x06_StartInt (const LcdEvt_EntryType eType)
{
    UNUSED_PARAM(eType);
    int_t nRet;

    nRet = LCD_Ft5x06_Int_Start();

    return nRet;
}

/**************************************************************************//**
 * Function Name: LCD_Ft5x06_GetEventTable
 * @brief         Get assigned callback event.
 *
 *                Description:<br>
 *
 * @param         [in]int_t nId              : event ID
 * @retval        pointer to event.
 ******************************************************************************/
LCDEVT_ENTRY *LCD_Ft5x06_GetEventTable (const int_t nId)
{
    return &LcdEvt_Entry[nId];
}

/**************************************************************************//**
 * Function Name: LCD_Ft5x06_SendEvtMsg
 * @brief         Send event message to synchronism.
 *
 *                Description:<br>
 *
 * @param         [in]uint32_t unEvtFlg      : event flag
 * @retval          0 : Operation successfull.
 *                 -1 : Error occured.
 ******************************************************************************/
int32_t LCD_Ft5x06_SendEvtMsg (const uint32_t unEvtFlg)
{
    int32_t nRet;

    nRet = 0;

    unEvtMsg |= unEvtFlg;
    if (sLcdSemIdTsk == 0)
    {
        DBG_printf_ERR("[ERROR] message is not put -- msg = 0x%0X\n", (unsigned int)unEvtFlg);
        unEvtMsg &= ~unEvtFlg;
        nRet = -1;
    }
    else
    {
        R_OS_ReleaseSemaphore( &sLcdSemIdTsk);
    }
    return nRet;
}

/**************************************************************************//**
 * Function Name: LCD_Ft5x06_WaitEvtMsg
 * @brief         Wait event message to synchronism.
 *
 *                Description:<br>
 *
 * @param         None.
 * @retval        >= 0 : event flag list.
 *                  -1 : Error occured.
 ******************************************************************************/
int32_t LCD_Ft5x06_WaitEvtMsg (void)
{
    int32_t nRet;
    bool_t nNum;

    nNum = R_OS_WaitForSemaphore( &sLcdSemIdTsk, R_OS_ABSTRACTION_PRV_EV_WAIT_INFINITE);
    if (nNum == true)
    {
        nRet = (int32_t) unEvtMsg;
    }
    else
    {
        DBG_printf_ERR("[ERROR] message is not got -- ret = %d\n", nNum);
        nRet = -1;
    }

    return nRet;
}

/**************************************************************************//**
 * Function Name: LCD_Ft5x06_ClearEvtMsg
 * @brief         Clear assigned event flag.
 *
 *                Description:<br>
 *
 * @param         [in]uint32_t unEvtFlg      : event flag
 * @retval        None.
 ******************************************************************************/
void LCD_Ft5x06_ClearEvtMsg (const uint32_t unEvtFlg)
{
    unEvtMsg &= ~unEvtFlg;
}
#endif /* TARGET_BOARD == TARGET_BOARD_STREAM_IT2 */
