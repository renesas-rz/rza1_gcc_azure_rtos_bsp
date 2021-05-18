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
* @file          lcd_ft5x06.h
* Rev: 30 
* Date:: 2016-12-21 12:02:44 +0900#
* @brief         LCD Driver internal hedder
******************************************************************************/
#ifndef     LCD_FT5x06_H
#define     LCD_FT5x06_H

/******************************************************************************
Includes <System Includes>, "Project Includes"
******************************************************************************/
#include    "mcu_board_select.h"
#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)

#include    "r_os_abstraction_api.h"
#include    "lcd_controller_if.h"

#if defined(__cplusplus)
extern "C" {
#endif


/******************************************************************************
Macro definitions
******************************************************************************/
#define DBG_LEVEL_OT                (-1)                /* onetime debug */
#define DBG_LEVEL_DEF               (0)                 /* default */
#define DBG_LEVEL_ERR               (1)                 /* error */
#define DBG_LEVEL_MSG               (2)                 /* message */
#define DBG_LEVEL_LOG               (3)                 /* log */
#define DBG_LEVEL_DBG               (4)                 /* debug */

#define DBG_LEVEL                   (DBG_LEVEL_ERR)
#if (DBG_LEVEL >= DBG_LEVEL_OT)
#define DBG_printf_OT                   printf
#else
#define DBG_printf_OT                   1 ? (int32_t) 0 : printf
#endif
#if (DBG_LEVEL >= DBG_LEVEL_DEF)
#define DBG_printf_DEF                  printf
#else
#define DBG_printf_DEF                  1 ? (int32_t) 0 : printf
#endif
#if (DBG_LEVEL >= DBG_LEVEL_ERR)
#define DBG_printf_ERR                  printf
#else
#define DBG_printf_ERR                  1 ? (int32_t) 0 : printf
#endif
#if (DBG_LEVEL >= DBG_LEVEL_MSG)
#define DBG_printf_MSG                  printf
#else
#define DBG_printf_MSG                  1 ? (int32_t) 0 : printf
#endif
#if (DBG_LEVEL >= DBG_LEVEL_LOG)
#define DBG_printf_LOG                  printf
#else
#define DBG_printf_LOG                  1 ? (int32_t) 0 : printf
#endif
#if (DBG_LEVEL >= DBG_LEVEL_DBG)
#define DBG_printf_DBG                  printf
#else
#define DBG_printf_DBG                  1 ? (int32_t) 0 : printf
#endif

//#define   __DEBUG
#ifdef  __DEBUG
#define SCOPE_STATIC
#else   /* __DEBUG */
#define SCOPE_STATIC    static
#endif  /* __DEBUG */

/*! The max number of event entry */
#define     LCDEVT_ENTRY_MAX            (1)


/*************************************************************************
 Enumerated Types
*************************************************************************/
/*! Touch panel event lock state */
typedef enum {
    LCD_EVT_UNLOCK = 0,  /*!< Unlocked */
    LCD_EVT_LOCK         /*!< Locked */
} LcdEvt_LockState ;


/*************************************************************************
 Structures
*************************************************************************/
/*! Event entry struct */
typedef struct {
    LcdEvt_EntryType mode ;      /*!< The type of touch panel event entry */
    LcdCBFunc function;
    LcdEvt_LockState evtlock ;   /*!< Event lock state */
} LCDEVT_ENTRY ;


/***********************************************************************************
 Global Vaiables
***********************************************************************************/
extern psemaphore_t   sLcdSemIdAcc;


/***********************************************************************************
 Function Prototypes
***********************************************************************************/

/**
 * @brief           Opens the communication environment with the FT5x06.
 * @param[in]       unIrqLv IRQ interrupt priority (0 to 255)\n
   Sets the GIC interrupt priority
 * @param[in]       nTskPri Task Priority\n
   Sets the value of osPriority type.
 * @param[in]       unTskStk Not Used.     
 * @retval      0 Normal end
 * @retval      -1 failure to open
*/
int_t        LCD_Ft5x06_Open( const uint32_t unIrqLv, int16_t nTskPri, uint32_t unTskStk );

/**
 * @brief       Closes the communication environment with the FT5x06.
 * @retval      NONE
*/
int_t        LCD_Ft5x06_Close( void );

/**
 * @brief       Sends data to the FT5x06 via the RIIC DeviceController ch1.
 * @param[in]       unDevAddr LCD Device Address
 * @param[in]       uData Send Data
 * @param[in]       unSize Send Data Length
 * @retval      0 normal end
 * @retval      -1 data send processing error
*/
uint8_t      LCD_Ft5x06_WriteCmd( const uint16_t unDevAddr, const uint8_t uData, const uint32_t unSize );

/**
 * @brief       Reads data from the FT5x06 via the RIIC DeviceController ch1.
 * @param[in]       unDevAddr LCD Device Address
 * @param[in]       *puData Receive data buffer pointer 
 * @param[out]       unSize Receive Data Length
 * @retval      0 normal end
 * @retval      -1 data receive error
*/
uint8_t      LCD_Ft5x06_ReadCmd( const uint16_t unDevAddr, uint8_t *puData, const uint32_t unSize );

/**
 * @brief       Registers in the event management structure a call-back function linked to an interrupt from the FT5x06.\n
   After registration finishes, the LCD interrupt is enabled and the event ID is sent as a return value.
 *
 * @param[in]       eType Specified Interrupt type
 * @param[in]       function Call-back function
 * @retval      0 to (LCDEVT_ENTRY_MAX - 1)
 * @retval      -1 event registration failure
*/
int_t        LCD_Ft5x06_EventEntry( const LcdEvt_EntryType eType, const LcdCBFunc function );

/**
 * @brief       Removes the registration information for the specified event ID from the event management structure.
 * @param[in]       nId Event ID\n
   return value of LCD_EventEntry function.
 * @retval      NONE
*/
int_t        LCD_Ft5x06_EventErase( const int_t nId );

/**
 * @brief       Removes masking of specified interrupt type.
 * @param[in]   eType Specified interrupt type
 * @retval      0 Always, normal end
*/
int_t        LCD_Ft5x06_StartInt( const LcdEvt_EntryType eType );

/**
 * @brief         Get assigned callback event.
 * @param[in]     nId event ID
 * @retval        LCDEVT_ENTRY pointer to event.
*/
LCDEVT_ENTRY *LCD_Ft5x06_GetEventTable( const int_t nId );

/**
 * @brief         Send event message to synchronism.
 * @param[in]       unEvtFlg event flag
 * @retval          0 Operation successful.
 * @retval          -1 Error occurred.
*/
int32_t      LCD_Ft5x06_SendEvtMsg( const uint32_t unEvtFlg );

/**
 * @brief         Wait event message to synchronism.
 * @retval       0 event flag list.
 * @retval       -1 : Error occurred.
*/
int32_t      LCD_Ft5x06_WaitEvtMsg( void );

/**
 * @brief         Clear assigned event flag.
 * @param[in]     unEvtFlg      : event flag
 * @retval        None.
*/
void         LCD_Ft5x06_ClearEvtMsg( const uint32_t unEvtFlg );

#if defined(__cplusplus)
}
#endif

#endif /* TARGET_BOARD == TARGET_BOARD_STREAM_IT2 */

#endif      /* LCD_FT5x06_H */
