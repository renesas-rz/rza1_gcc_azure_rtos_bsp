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
* @file         r_riic_max9856_if.c
* $Rev:         1.0
* $Date:        24.04.2019
* @brief        MAX9856 driver interface functions
******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "mcu_board_select.h"
#include "r_errno.h"
#include "r_os_abstraction_api.h"
#include "riic_max9856_if.h"
#include "dev_drv.h"

#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)

#include "riic_max9856_drv.h"

/******************************************************************************
Typedef definitions
******************************************************************************/
typedef struct riic_ch1_ctrl
{
    bool_t           is_open;
    uint32_t         semid;
} riic_ch1_ctrl_t;

/******************************************************************************
Macro definitions
******************************************************************************/
#define RIIC_CH1_API_TMOUT (500u)

/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
static riic_ch1_ctrl_t s_riic1_ctrl =
{
    false,  /* is_open */
    0       /* semid   */
};

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/

/**************************************************************************//**
* Function Name: R_RIIC_MAX9856_Open
* @brief         RIIC_CH1 driver initialize.
*
*                Description:<br>
*                Open RIIC CH1 and initialize synchronous resource
* @param         none
* @retval        DEVDRV_SUCCESS   :Success.
* @retval        error code :Failure.
******************************************************************************/
int32_t R_RIIC_MAX9856_Open(void)
{
    int32_t ercd = DEVDRV_SUCCESS;

    if (false != s_riic1_ctrl.is_open)
    {
        ercd = DEVDRV_ERROR;
    }
    else
    {
        R_OS_CreateSemaphore(&s_riic1_ctrl.semid, 1);

        if (0 == s_riic1_ctrl.semid)
        {
            ercd = DEVDRV_ERROR;
        }

        if (DEVDRV_SUCCESS == ercd)
        {
            ercd = RIIC_MAX9856_Open();
        }

        if (DEVDRV_SUCCESS == ercd)
        {
            s_riic1_ctrl.is_open = true;
        }
        else
        {
            s_riic1_ctrl.is_open = false;

            if (0 != s_riic1_ctrl.semid)
            {
                R_OS_DeleteSemaphore(&s_riic1_ctrl.semid);
            }
        }
    }

    return (ercd);
}
/******************************************************************************
End of function R_RIIC_MAX9856_Open
******************************************************************************/

/**************************************************************************//**
* Function Name: R_RIIC_MAX9856_Close
* @brief         RIIC_CH1 driver uninitialize.
*
*                Description:<br>
*                Close RIIC CH1 and free synchronous resource
* @param         none
* @retval        DEVDRV_SUCCESS   :Success.
* @retval        error code :Failure.
******************************************************************************/
int32_t R_RIIC_MAX9856_Close(void)
{
    int32_t ercd = DEVDRV_SUCCESS;
    bool_t sem_token = false;

    if (false == s_riic1_ctrl.is_open)
    {
        ercd = DEVDRV_ERROR;
    }
    else
    {
        sem_token = R_OS_WaitForSemaphore(&s_riic1_ctrl.semid, RIIC_CH1_API_TMOUT);

        if (!sem_token)
        {
            ercd = DEVDRV_ERROR;
        }
        else
        {
            ercd = RIIC_MAX9856_Close();

            if (DEVDRV_SUCCESS == ercd)
            {
                s_riic1_ctrl.is_open = false;
                R_OS_DeleteSemaphore(&s_riic1_ctrl.semid);
            }
        }
    }

    return (ercd);
}
/******************************************************************************
End of function R_RIIC_MAX9856_Close
******************************************************************************/

/**************************************************************************//**
* Function Name: R_RIIC_MAX9856_Write
* @brief         Write register to MAX9856
*
*                Description:<br>
*
* @param         addr       :register address 0-28
* @param         dat        :register data 0-255
* @retval        DEVDRV_SUCCESS   :Success.
* @retval        error code :Failure.
******************************************************************************/
int32_t R_RIIC_MAX9856_Write(const uint8_t addr, const uint8_t dat)
{
    int32_t ercd = DEVDRV_SUCCESS;
    bool_t sem_token;

    if (false == s_riic1_ctrl.is_open)
    {
        ercd = DEVDRV_ERROR;
    }
    else
    {
        sem_token = R_OS_WaitForSemaphore(&s_riic1_ctrl.semid, RIIC_CH1_API_TMOUT);
        if (!sem_token)
        {
            ercd = DEVDRV_ERROR;
        }
        else
        {
            ercd = RIIC_MAX9856_Write(addr, dat);
            R_OS_ReleaseSemaphore(&s_riic1_ctrl.semid);
        }
    }

    return (ercd);
}
/******************************************************************************
End of function R_RIIC_MAX9856_Write
******************************************************************************/

/**************************************************************************//**
* Function Name: R_RIIC_MAX9856_Read
* @brief         Read register from MAX9856
*
*                Description:<br>
*
* @param         addr       :register address 0-28
* @param         p_dat      :store address of register data 0-255
* @retval        DEVDRV_SUCCESS   :Success.
* @retval        error code :Failure.
******************************************************************************/
int32_t R_RIIC_MAX9856_Read(const uint8_t addr, uint8_t* const p_dat)
{
    int32_t ercd = DEVDRV_SUCCESS;
    bool_t sem_token;

    if (false == s_riic1_ctrl.is_open)
    {
        ercd = DEVDRV_ERROR;
    }
    else
    {
        sem_token = R_OS_WaitForSemaphore(&s_riic1_ctrl.semid, RIIC_CH1_API_TMOUT);

        if (!sem_token)
        {
            ercd = DEVDRV_ERROR;
        }
        else
        {
            ercd = RIIC_MAX9856_Read(addr, p_dat);
            R_OS_ReleaseSemaphore(&s_riic1_ctrl.semid);
        }
    }

    return (ercd);
}
/******************************************************************************
End of function R_RIIC_MAX9856_Read
******************************************************************************/

#elif (TARGET_BOARD == TARGET_BOARD_RSK)

#include "riic_max9856_drv.h"

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
#define RIIC_ch3_API_TMOUT (500u)

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
* Function Name: R_RIIC_MAX9856_Open
* @brief         RIIC_ch3 driver initialize.
*
*                Description:<br>
*                Open RIIC ch3 and initialize synchronous resource
* @param         none
* @retval        DEVDRV_SUCCESS   :Success.
* @retval        error code :Failure.
******************************************************************************/
int32_t R_RIIC_MAX9856_Open(void)
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
            ercd = RIIC_MAX9856_Open();
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
End of function R_RIIC_MAX9856_Open
******************************************************************************/

/**************************************************************************//**
* Function Name: R_RIIC_MAX9856_Close
* @brief         RIIC_ch3 driver uninitialize.
*
*                Description:<br>
*                Close RIIC ch3 and free synchronous resource
* @param         none
* @retval        DEVDRV_SUCCESS   :Success.
* @retval        error code :Failure.
******************************************************************************/
int32_t R_RIIC_MAX9856_Close(void)
{
    int32_t ercd = DEVDRV_SUCCESS;
    bool_t sem_token = false;

    if (false == s_riic3_ctrl.is_open)
    {
        ercd = DEVDRV_ERROR;
    }
    else
    {
        sem_token = R_OS_WaitForSemaphore(&s_riic3_ctrl.semid, RIIC_ch3_API_TMOUT);

        if (!sem_token)
        {
            ercd = DEVDRV_ERROR;
        }
        else
        {
            ercd = RIIC_MAX9856_Close();

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
End of function R_RIIC_MAX9856_Close
******************************************************************************/

/**************************************************************************//**
* Function Name: R_RIIC_MAX9856_Write
* @brief         Write register to MAX9856
*
*                Description:<br>
*
* @param         addr       :register address 0-28
* @param         dat        :register data 0-255
* @retval        DEVDRV_SUCCESS   :Success.
* @retval        error code :Failure.
******************************************************************************/
int32_t R_RIIC_MAX9856_Write(const uint8_t addr, const uint8_t dat)
{
    int32_t ercd = DEVDRV_SUCCESS;
    bool_t sem_token;

    if (false == s_riic3_ctrl.is_open)
    {
        ercd = DEVDRV_ERROR;
    }
    else
    {
        sem_token = R_OS_WaitForSemaphore(&s_riic3_ctrl.semid, RIIC_ch3_API_TMOUT);
        if (!sem_token)
        {
            ercd = DEVDRV_ERROR;
        }
        else
        {
            ercd = RIIC_MAX9856_Write(addr, dat);
            R_OS_ReleaseSemaphore(&s_riic3_ctrl.semid);
        }
    }

    return (ercd);
}
/******************************************************************************
End of function R_RIIC_MAX9856_Write
******************************************************************************/

/**************************************************************************//**
* Function Name: R_RIIC_MAX9856_Read
* @brief         Read register from MAX9856
*
*                Description:<br>
*
* @param         addr       :register address 0-28
* @param         p_dat      :store address of register data 0-255
* @retval        DEVDRV_SUCCESS   :Success.
* @retval        error code :Failure.
******************************************************************************/
int32_t R_RIIC_MAX9856_Read(const uint8_t addr, uint8_t* const p_dat)
{
    int32_t ercd = DEVDRV_SUCCESS;
    bool_t sem_token;

    if (false == s_riic3_ctrl.is_open)
    {
        ercd = DEVDRV_ERROR;
    }
    else
    {
        sem_token = R_OS_WaitForSemaphore(&s_riic3_ctrl.semid, RIIC_ch3_API_TMOUT);

        if (!sem_token)
        {
            ercd = DEVDRV_ERROR;
        }
        else
        {
            ercd = RIIC_MAX9856_Read(addr, p_dat);
            R_OS_ReleaseSemaphore(&s_riic3_ctrl.semid);
        }
    }

    return (ercd);
}
/******************************************************************************
End of function R_RIIC_MAX9856_Read
******************************************************************************/

#endif /* TARGET_BOARD */
