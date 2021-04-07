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
 * @headerfile     tp_if.h
 * @brief          Interface to the touch panel 
 * @version        1.00
 * @date           27.06.2018
 * H/W Platform    Platform Independent
 *****************************************************************************/
 /*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 21.12.2016 1.00 First Release
 *****************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef     TP_IF_H
#define     TP_IF_H
/**************************************************************************//**
 * @ingroup R_SW_PKG_93_NONOS_MIDDLEWARE
 * @defgroup R_SW_PKG_93_TOUCH_API Touch
 * @brief Touch Panel Interface
 *
 * @anchor R_SW_PKG_93_TP_INTERFACE_API_SUMMARY 
 * @par Summary
 * This is a Touch Panel Interface middleware module, 
 * which allows the user to write to and read from the registers of 
 * the touch panels, detecting touch events and receiving their coordinates. 
 * This  module is specific to the Touch panel, but not the device or board.
 *
 * @anchor R_SW_PKG_93_TP_INTERFACE_API_INSTANCES 
 * @par Known Implementations:
 * This middleware is used in the RZA1H Software Package.
 * @see RENESAS_APPLICATION_SOFTWARE_PACKAGE
 * @see RENESAS_OS_ABSTRACTION  Renesas OS Abstraction interface
 * @{
 *****************************************************************************/
/******************************************************************************
Includes <System Includes> , "Project Includes"
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/******************************************************************************
Macro definitions
******************************************************************************/
#define TP_TOUCHNUM_MAX (2)

/*************************************************************************
 Enumerated Types
*************************************************************************/
/*! The type of touch panel event entry */
typedef enum {
    TPEVT_ENTRY_NONE    = 0x0000,      /*!< None */
    TPEVT_ENTRY_UP      = 0x0001,      /*!< Up */
    TPEVT_ENTRY_DOWN    = 0x0002,      /*!< Down */
    TPEVT_ENTRY_MOVE    = 0x0004,      /*!< Move */

    TPEVT_ENTRY_ALL     = 0x0007,      /*!< All */

    TPEVT_ENTRY_UNKNOWN = 0x8000       /*!< internal event state */
} TpEvt_EntryType ;


/*************************************************************************
 Structures
*************************************************************************/
typedef struct {
    TpEvt_EntryType     eState;
    uint16_t            unPosX;
    uint16_t            unPosY;
} TP_TouchFinger_st;

typedef struct {
    TP_TouchFinger_st   sFinger[TP_TOUCHNUM_MAX];
} TP_TouchEvent_st;

typedef void (*TpCBFunc)(int_t, TP_TouchEvent_st*);


/***********************************************************************************
 Global Vaiables
***********************************************************************************/


/***********************************************************************************
 Function Prototypes
***********************************************************************************/

/**
 * @brief         Initializes the touch panel driver by calling the TP_Init.
 * @retval        NONE
*/
extern void  TouchPanel_Init( void );

/**
 * @brief       Generates and initializes a touch panel task by calling the TP_Open.\n
 *              Do not call this function during touch panel utility has been opened.
 *
 * @param[in]       nWidth    screen width
 * @param[in]       nHeight   screen height
 * @param[in]       unIrqLv   IRQ interrupt level
 * @param[in]       nTskPri   task priority
 * @param[in]       unTskStk  task stack size
 * @retval       0 Operation successfull.
 * @retval      -1 Error occured.
*/
extern int_t TouchPanel_Open( const int_t nWidth, const int_t nHeight, const uint32_t unIrqLv, const int16_t nTskPri, const uint32_t unTskStk );

/**
 * @brief      Touch Panel utility close function
 * @retval     NONE
*/
extern int_t TouchPanel_Close( void );

/**
 * @brief       Registers a call-back function linked to the LCD area 
 *              where a touch panel event occurs in the touch panel event 
 *              management structure.
 * 
 *              Calls the function described in TP_EventEntry, to perform the following processing:
 *              Searching for a free area in the touch panel event table 
 *              (Up to 16 touch panel events can be registered,
 *              and error processing occurs if no free area is available.)
 *              Making “specified touch action,” “X coordinate of specified area,” 
 *              “Y coordinate of specified area,” “width of specified area,” “height of specified area,” 
 *              “specified call-back function” settings for the touch panel event table free area.
 *  
 *              Note: If events occur simultaneously in multiple registered areas that overlap,
 *                    the associated call-back functions are executed in order, starting with the 
 *                    one with the lowest event ID.
 *
 * @param[in]       eMode:    Specified touch action        
 * @param[in]       nPosX:    X coordinate of specified area
 * @param[in]       nPosY:    Y coordinate of specified area
 * @param[in]       nWidth:   width of specified area
 * @param[in]       nHeight:  height of specified area
 * @param[in]       function: Specified call-back function 
 * 
 * @retval      Success event ID of 0 to (TPEVT_ENTRY_MAX -1) if successful
 * @retval      Fail returns -1 
*/
extern int_t TouchPanel_EventEntry( const TpEvt_EntryType eMode, const int32_t nPosX, const int32_t nPosY, const int32_t nWidth, const int32_t nHeight, const TpCBFunc function );

/**
 * @brief       Removes registration information for the specified event ID from the 
 *              touch panel event management structure.
 *              Calls the function described in TP_EventErase, to perform the following processing:
 *              Event ID checking (within range of 0 to TPEVT_ENTRY_MAX)
 *              Disabling of event associated with event ID
 *
 * @param[in]       nId: Event ID
 * 
 * @retval       0: normal end
 * @retval      -1: event removal failure 
 */
extern int_t TouchPanel_EventErase( const int_t nId );

/**
 * @brief       Changes the LCD area of the specified event ID.
 *              Calls the function described in TP_ChangeEventEntry, 
 *              to perform the following processing:
 *              Event ID checking (within range of 0 to TPEVT_ENTRY_MAX),
 *              Event ID checking (unregistered ID or removed ID) 
 *              Registration of event in area of specified ID in touch panel event table.
 *
 * @param[in]       nId:      Event ID
 * @param[in]       nPosX:    X coordinate of area after change
 * @param[in]       nPosY:    Y coordinate of area after change
 * @param[in]       nWidth:   Width of area after change
 * @param[in]       nHeight:  Height of area after change 
 * 
 * @retval       0: normal end
 * @retval      -1: LCD area change failure 
 */
extern int_t TouchPanel_ChangeEventEntry( const int_t nId, const int32_t nPosX, const int32_t nPosY, const int32_t nWidth, const int32_t nHeight );

/**
 * @brief       Locks processing of all touch panel events.
 *              Calls the function described in TP_EventLockAll, 
 *              to perform the following processing:
 *              Setting all events in the touch panel event table to the locked state.
 *
 * @retval      0 normal end
 * @retval      -1 touch panel event locking failure
 */
extern int_t TouchPanel_EventLockAll( void );

/**
 * @brief       Unlocks processing of all touch panel events.
 *              Calls the function described in TP_EventUnlockAll, 
 *              to perform the following processing:
 *              Setting all events in the touch panel event table to the unlocked state.
 *
 * @retval      0 normal end
 * @retval      -1 touch panel event unlocking failure
 */
extern int_t TouchPanel_EventUnlockAll( void );

/**
 * @brief       Locks processing of the touch panel event specified by the event ID.
 *              Calls the function described in TP_EventLock, to perform 
 *              the following processing:
 *              Event ID checking (within range of 0 to TPEVT_ENTRY_MAX)
 *              Setting the event specified by the event ID to the locked state 
 *              in the touch panel event table.
 *
 * @param[in]       nId: Event ID
 * 
 * @retval       0: normal end
 * @retval      -1: event removal failure 
 */
extern int_t TouchPanel_EventLock( const int_t nId );

/**
 * @brief       Unlocks processing of the touch panel event specified by the event ID.
 *              Calls the function described in TP_EventUnlock, 
 *              to perform the following processing:
 *              Event ID checking (within range of 0 to TPEVT_ENTRY_MAX)
 *              Setting the event specified by the event ID to the unlocked 
 *              state in the touch panel event table.
 *
 * @param[in]       nId: Event ID
 * 
 * @retval       0: normal end
 * @retval      -1: event removal failure 
 */
extern int_t TouchPanel_EventUnlock( const int_t nId );

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif      /* TP_IF_H */
/**************************************************************************//**
 * @} (end addtogroup)
 *****************************************************************************/
