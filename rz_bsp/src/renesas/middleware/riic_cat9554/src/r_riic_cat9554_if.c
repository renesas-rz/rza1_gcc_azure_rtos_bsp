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
/******************************************************************************
* @file           r_riic_cat9554_if.c
* @brief          CAT9554 driver interface functions
* @version        1.00
* @date           24.04.2019
* H/W Platform    Renesas Starter Kit+ for RZ/A1H
*****************************************************************************/
/*****************************************************************************
* History      : DD.MM.YYYY Ver. Description
*              : 24.04.2019 1.00 First Release
*****************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "mcu_board_select.h"

#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
/* nothing */
#elif (TARGET_BOARD == TARGET_BOARD_RSK)

#include "r_errno.h"
#include "r_os_abstraction_api.h"
#include "riic_cat9554_if.h"
#include "dev_drv.h"

#include "riic_cat9554_drv.h"

/******************************************************************************
Typedef definitions
******************************************************************************/
typedef struct riic_ch3_ctrl
{
    bool_t           is_open;
    uint32_t         semid;
} riic_ch3_ctrl_t;

/******************************************************************************
Macro definitions
******************************************************************************/
#define RIIC_CH3_API_TMOUT (500u)

/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
static riic_ch3_ctrl_t s_riic3_ctrl =
{
    false,  /* is_open */
    0       /* semid   */
};

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/

/**************************************************************************//**
* Function Name: R_RIIC_CAT9554_Open
* @brief         RIIC_CAT9554 driver initialize.
*
*                Description:<br>
*                Open RIIC CH3 and initialize synchronous resource
* @param         none
* @retval        DEVDRV_SUCCESS   :Success.
* @retval        error code :Failure.
******************************************************************************/
int32_t R_RIIC_CAT9554_Open(void)
{
    int32_t ercd = DEVDRV_SUCCESS;

    if (false != s_riic3_ctrl.is_open)
    {
        ercd = DEVDRV_ERROR;
    }
    else
    {
        R_OS_CreateSemaphore(&s_riic3_ctrl.semid, 1);

        if (0 == s_riic3_ctrl.semid)
        {
            ercd = DEVDRV_ERROR;
        }

        if (DEVDRV_SUCCESS == ercd)
        {
            ercd = RIIC_CAT9554_Open();
        }

        if (DEVDRV_SUCCESS == ercd)
        {
            s_riic3_ctrl.is_open = true;
        }
        else
        {
            s_riic3_ctrl.is_open = false;

            if (0 != s_riic3_ctrl.semid)
            {
                R_OS_DeleteSemaphore(&s_riic3_ctrl.semid);
            }
        }
    }

    return (ercd);
}
/******************************************************************************
End of function R_RIIC_CAT9554_Open
******************************************************************************/

/**************************************************************************//**
* Function Name: R_RIIC_CAT9554_Close
* @brief         RIIC_CAT9554 driver uninitialize.
*
*                Description:<br>
*                Close RIIC CH3 and free synchronous resource
* @param         none
* @retval        DEVDRV_SUCCESS   :Success.
* @retval        error code :Failure.
******************************************************************************/
int32_t R_RIIC_CAT9554_Close(void)
{
    int32_t ercd = DEVDRV_SUCCESS;
    bool_t sem_token = false;

    if (false == s_riic3_ctrl.is_open)
    {
        ercd = DEVDRV_ERROR;
    }
    else
    {
        sem_token = R_OS_WaitForSemaphore(&s_riic3_ctrl.semid, RIIC_CH3_API_TMOUT);

        if (!sem_token)
        {
            ercd = DEVDRV_ERROR;
        }
        else
        {
            ercd = RIIC_CAT9554_Close();

            if (DEVDRV_SUCCESS == ercd)
            {
                s_riic3_ctrl.is_open = false;
                R_OS_DeleteSemaphore(&s_riic3_ctrl.semid);
            }
        }
    }

    return (ercd);
}
/******************************************************************************
End of function R_RIIC_CAT9554_Close
******************************************************************************/

/**************************************************************************//**
* Function Name: R_RIIC_CAT9554_Write
* @brief         Write register to CAT9554
*
*                Description:<br>
*
* @param         addr   :register address 0x40, 0x42
* @param         data   :register data 0-255
* @param         config :register config 0-255
* @retval        DEVDRV_SUCCESS   :Success.
* @retval        error code :Failure.
******************************************************************************/
int32_t R_RIIC_CAT9554_Write(const uint8_t addr, const uint8_t data, const uint8_t config)
{
    int32_t ercd = DEVDRV_SUCCESS;
    bool_t sem_token;

    if (false == s_riic3_ctrl.is_open)
    {
        ercd = DEVDRV_ERROR;
    }
    else
    {
        sem_token = R_OS_WaitForSemaphore(&s_riic3_ctrl.semid, RIIC_CH3_API_TMOUT);
        if (!sem_token)
        {
            ercd = DEVDRV_ERROR;
        }
        else
        {
            ercd = RIIC_CAT9554_Write(addr, data, config);
            R_OS_ReleaseSemaphore(&s_riic3_ctrl.semid);
        }
    }

    return (ercd);
}
/******************************************************************************
End of function R_RIIC_CAT9554_Write
******************************************************************************/
#endif /* TARGET_BOARD */
