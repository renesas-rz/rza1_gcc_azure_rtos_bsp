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
 * @headerfile     lcd_controller_if.h
 * @brief          LCD Controller API file for communicating with panel 
 *                 registers. 
 * @version        1.00
 * @date           27.06.2018
 * H/W Platform    Platform Independent
 *****************************************************************************/
 /*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 21.12.2016 1.00 First Release
 *****************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef     LCD_CONTROLLER_IF_H
#define     LCD_CONTROLLER_IF_H
/**************************************************************************//**
 * @ingroup R_SW_PKG_93_TOUCH_API Touch
 * @defgroup R_SW_PKG_93_TOUCH_LCD LCD Controller
 * @brief Touch Panel LCD Controller 
 * 
 * @anchor R_SW_PKG_93_LCD_CONTROLLER_API_SUMMARY
 * @par Summary
 *
 * This is a LCD Controller middleware module, which allows the user to write 
 * to and read from the registers of the LCD Display. 
 *
 * This  module is specific to the LCD panel, but not the device or board.
 *
 * @anchor R_SW_PKG_93_LCD_CONTROLLER_API_INSTANCES
 * @par Known Implementations:
 * This middleware is used in the RZA1H Software Package.
 * @see RENESAS_APPLICATION_SOFTWARE_PACKAGE
 * @see RENESAS_OS_ABSTRACTION  Renesas OS Abstraction interface
 * @{
 *****************************************************************************/

/******************************************************************************
Includes <System Includes>, "Project Includes"
******************************************************************************/
#include "mcu_board_select.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef void (*LcdCBFunc) (void*);
/******************************************************************************
Macro definitions
******************************************************************************/
/*! LCD slave address */

#define LCD_SLAVE_ADDRESS    (0x38 << 1)

/*************************************************************************
 Enumerated Types
*************************************************************************/
/*! The type of touch panel event entry */
typedef enum {
    LCDEVT_ENTRY_NONE = 0x0000,      /*!< None */
    LCDEVT_ENTRY_TP   = 0x0001,      /*!< None */

    LCDEVT_ENTRY_ALL  = 0x0001       /*!< All */
} LcdEvt_EntryType ;


/*************************************************************************
 Structures
*************************************************************************/


/***********************************************************************************
 Global Vaiables
***********************************************************************************/


/***********************************************************************************
 Function Prototypes
***********************************************************************************/

/**
 * @brief         Sets the LCD board initialization counter (nLcdInitCnt) to 0.
 * 
 * @retval        NONE
*/
extern void  R_LCD_Init( void );

/**
 * @brief       Opens a communication environment with the LCD board.\n
 *              This function enables the user to perform multiple open operations.
 *
 * @param[in]       unIrqLv:  IRQ interrupt priority (0 to 255)\n
 *                            Sets the GIC interrupt priority
 * @param[in]       nTskPri:  Task Priority\n
 *                            Sets the value of osPriority type.
 * @param[in]       unTskStk: Not Used.     
 * 
 * @retval       0: Normal end
 * @retval      -1: Open error
*/
extern int_t R_LCD_Open( const uint32_t unIrqLv, const int16_t nTskPri, const uint32_t unTskStk );

/**
 * @brief        Closes a communication environment with the LCD board.
 *               When LCD_Open is used to perform multiple open operations, 
 *               this function must be called the same number of times.
 * 
 * @retval        NONE
*/
extern int_t R_LCD_Close( void );

/*
 * @brief       Sends data to the LCD board via the RIIC
 * 
 * @param[in]       unDevAddr: LCD Device Address
 * @param[in]       uCmd:      Not Used
 * @param[in]       uData:     Send Data
 * @param[in]       unSize:    Send Data Length
 * 
 * @retval       0: normal end
 * @retval      -1: data send processing error
*/
extern uint8_t R_LCD_WriteCmd( const uint16_t unDevAddr, const uint8_t uCmd, const uint8_t uData, const uint32_t unSize );

/**
 * @brief       Receives data from the LCD board via the RIIC.
 * 
 * @param[in]       unDevAddr: LCD Device Address
 * @param[in]       uCmd:      Not Used
 * @param[in]       *puData:   Receive data buffer pointer 
 * @param[out]       unSize:   Receive Data Length
 * 
 * @retval       0: normal end
 * @retval      -1: data send processing error
*/
extern uint8_t R_LCD_ReadCmd( const uint16_t unDevAddr, const uint8_t uCmd, uint8_t *puData, const uint32_t unSize );

/**
 * @brief       Registers an LCD board event.
 * 
 * @param[in]   eType Specified Interrupt type
 * @param[in]   function Call-back function
 * 
 * @retval       0: (LCDEVT_ENTRY_MAX-1) registration value
 * @retval      -1: Event registration failure
*/
extern int_t R_LCD_EventEntry( const LcdEvt_EntryType eType, const LcdCBFunc function );

/**
 * @brief       Removes an LCD board event.
 * 
 * @param[in]   nId: Event ID
 * 
 * @retval      NONE
*/
extern int_t R_LCD_EventErase( const int_t nId );

/**
 * @brief       Removes masking of specified interrupt type.
 * 
 * @param[in]   eType: Not Used
 * 
 * @retval      0 event successfully removed
 * @retval      -1 event removal failure
*/
extern int_t R_LCD_StartInt( const LcdEvt_EntryType eType );

/**
 * @brief       Reset LCD board.
 * 
 * @warning Function not currently supported.
 * 
 * @retval      0: Always Returned
*/
extern int_t R_LCD_Restart( void );

/**
 * @brief       Read LCD FW version.
 * 
 * @param[out]  *puData: pointer to receive buffer
 * 
 * @warning Function not currently supported.
 * 
 * @retval      0: Always Returned
*/
extern void  R_LCD_ReadVersion( uint8_t *puData );

/**
 * @brief       Set bright level of backlight.
 * 
 * @param[in]   uLevel: bright level
 * 
 * @warning Function not currently supported.
 * 
 * @return      None.
*/
extern void  R_LCD_SetBacklight( const uint8_t uLevel );

/**
 * @brief       Set scale of buzzer.
 * 
 * @param[in]   uScale: scale
 * 
 * @warning Function not currently supported.
 * 
 * @return      None.
*/
extern void  R_LCD_SetBuzzer( const uint8_t uScale );

#if defined(__cplusplus)
}
#endif


#endif      /* LCD_CONTROLLER_IF_H */
/**************************************************************************//**
 * @} (end addtogroup)
 *****************************************************************************/
