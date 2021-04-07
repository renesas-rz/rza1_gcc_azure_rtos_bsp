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
* Copyright (C) 2013-2017 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/

/**************************************************************************//**
* @file         tp_task.c
* $Rev: 30 $
* $Date:: 2016-12-21 12:02:44 +0900#$
* @brief        TouchPanel Driver task functions
******************************************************************************/

/*******************************************************************************
Includes <System Includes>, "Project Includes"
*******************************************************************************/
#include    <stdio.h>
#include    <string.h>

#include    "mcu_board_select.h"
#include    "r_typedefs.h"
#include    "iodefine_cfg.h"
#include    "dev_drv.h"
#include    "tp.h"
#include    "tp_task.h"
#include    "lcd_controller_if.h"

/******************************************************************************
Macro definitions
******************************************************************************/
    /*! delay time in touch panel task cycle */
#define     TP_TASK_DELAY       (5)

/*! Threshold for determining the "Move" event, pixel^2 */
#define     TPEVT_MOVE_MINTHD   (2UL)
#define     TPEVT_PRESS_THD     (1000)


/******************************************************************************
 Enumerated Types
******************************************************************************/


/******************************************************************************
 Structures
******************************************************************************/
typedef struct {
    int32_t x;
    int32_t y;
} TpAddress_st;


/***********************************************************************************
 Global Vaiables
***********************************************************************************/


/*******************************************************************************
 Private global variables and functions
*******************************************************************************/
SCOPE_STATIC void  TP_SetFingerInfo( TP_TouchFinger_st *psTouch, TPEVT_COORDINATES *psLastCoords, int_t *pnPenStatus, const uint8_t* const puReadBuf, const int_t nFinger );
SCOPE_STATIC void  TP_LineUpPos( const int16_t nXin, const int16_t nYin, int32_t *pnXout, int32_t *pnYout );
SCOPE_STATIC int_t TP_EventHandle( TpEvt_EntryType *eEvent, const int32_t* const nPosX, const int32_t* const nPosY, TPEVT_COORDINATES *psLastCoords );
SCOPE_STATIC int_t TP_CheckDiffMin( const TPEVT_COORDINATES* const psPox_1, const TPEVT_COORDINATES* const psPox_2, const uint32_t unThreshold );
SCOPE_STATIC int_t TP_DirectSrchCb( TP_TouchEvent_st *psTouchEvt );

static TP_TASKSTAT_et  eTpTaskStat;

/**************************************************************************//**
* Function Name: TP_Task
* @brief         Task function.
*
*                Description:<br>
*
* @param         [in]void const *vpArg  : pointer to argument
* @retval        None.
******************************************************************************/
void TP_Task(void *params)
{
    UNUSED_PARAM(params);

    int_t               nRet;
    int32_t             nEvtVal;
    uint8_t             auReadBuf[0x20];
    TP_TouchEvent_st    sTouchEvt;
    TPEVT_COORDINATES   sLastCoords[TP_TOUCHNUM_MAX];
    int_t               nPenStatus[TP_TOUCHNUM_MAX];
    int_t               nI, nJ;
    uint8_t             nTouchEndFlag;
    uint32_t            uiCnt;

    eTpTaskStat = TP_TASKSTAT_ACTIVE;
    memset( &sTouchEvt, 0, sizeof(TP_TouchEvent_st) );
    memset( &sLastCoords, 0, sizeof(TPEVT_COORDINATES) );
    memset( nPenStatus, 0, sizeof(nPenStatus) );

    while( 1 )
    {
        DBG_printf_DBG( "[NO INTC] ISPR : 0x%08lX, ABR : 0x%08lX\n", INTC.ICDISPR1, INTC.ICDABR1 );
        nEvtVal = TP_WaitEvtMsg();
        DBG_printf_DBG( "[TP EVT MSG]\n" );
        if( (nEvtVal & TP_EVTFLG_EXIT) == TP_EVTFLG_EXIT )
        {
            TP_ClearEvtMsg( TP_EVTFLG_EXIT );
            eTpTaskStat = TP_TASKSTAT_IDLE;

            R_OS_DeleteTask(p_os_task);
            p_os_task = NULL;

        }
        else if( (nEvtVal & TP_EVTFLG_PENIRQ) == TP_EVTFLG_PENIRQ )
        {
            TP_ClearEvtMsg( TP_EVTFLG_PENIRQ );
            while( 1 )
            {
                /* get touch information via RIIC */
                nRet = R_LCD_ReadCmd(LCD_SLAVE_ADDRESS, 0, auReadBuf, 32);
                if( nRet != DEVDRV_SUCCESS )
                {
                    DBG_printf_ERR( "[ERROR] touch status is not got\n" );
                    R_LCD_Restart();
                    break;
                }
                else
                {
                    /* make touch information for output */
                    for( nI = 0; nI < TP_TOUCHNUM_MAX; nI ++ )
                    {
                        TP_SetFingerInfo( &sTouchEvt.sFinger[nI], &sLastCoords[nI], &nPenStatus[nI], auReadBuf, nI );
                    }
                    /* determine unmoved finger's state */
                    for( nI = 0; nI < TP_TOUCHNUM_MAX; nI ++ )
                    {
                        if( sTouchEvt.sFinger[nI].eState == TPEVT_ENTRY_UNKNOWN )
                        {
                            sTouchEvt.sFinger[nI].eState = TPEVT_ENTRY_NONE;
                            for( nJ = 0; nJ < TP_TOUCHNUM_MAX; nJ ++ )
                            {
                                if( (sTouchEvt.sFinger[nJ].eState == TPEVT_ENTRY_UP  ) ||
                                    (sTouchEvt.sFinger[nJ].eState == TPEVT_ENTRY_DOWN) ||
                                    (sTouchEvt.sFinger[nJ].eState == TPEVT_ENTRY_MOVE) )
                                {
                                    sTouchEvt.sFinger[nI].eState = TPEVT_ENTRY_MOVE;
                                    break;
                                }
                            }
                        }
                    }
                }

                /* refer to previous coordinate if state is UP */
                for( nI = 0; nI < TP_TOUCHNUM_MAX; nI ++ )
                {
                    if( sTouchEvt.sFinger[nI].eState == TPEVT_ENTRY_UP )
                    {
                        sTouchEvt.sFinger[nI].unPosX = (uint16_t) sLastCoords[nI].x;
                        sTouchEvt.sFinger[nI].unPosY = (uint16_t) sLastCoords[nI].y;
                    }
                }

                /* execute callback function */
                if( nRet >= 0 )
                {
                    if( TP_GetEventLockInf() == TP_EVT_UNLOCK )
                    {
                        nRet = TP_DirectSrchCb( &sTouchEvt ) ;
                    }
                }

                /* touch flag clear */
                nTouchEndFlag = 0;

                /* move to waiting mode when touch is released */
                for(uiCnt = 0; uiCnt < TP_TOUCHNUM_MAX; uiCnt ++){
                    nTouchEndFlag |= (uint8_t) nPenStatus[uiCnt];
                }

                if( (nRet >= 0) && (nTouchEndFlag == 0) )
                {
                    DBG_printf_DBG("[ESCAPE]\n");
                    break;
                }
                R_OS_TaskSleep( TP_TASK_DELAY );
            }
            /* enable LCD touch event interrupt */
            nRet = R_LCD_StartInt( LCDEVT_ENTRY_TP );
            if( nRet < 0 )
            {
                DBG_printf_ERR("[ERROR] TP interrupt is not started\n");
                R_LCD_Restart();
            }
        }
        else
        {
            /* do nothing */
        }
    }
}


/**************************************************************************//**
* Function Name: TP_Kitb01_GetTaskStatus
* @brief         Get task status.
*
*                Description:<br>
*
* @param         None.
* @retval        Task status.
******************************************************************************/
TP_TASKSTAT_et  TP_GetTaskStatus( void )
{
    return eTpTaskStat;
}


/**************************************************************************//**
* Function Name: TP_SetFingerInfo
* @brief         Set touch information to output table.
*
*                Description:<br>
*
* @param         [out]TP_TouchFinger_st *psTouch        : pointer to output table
* @param         [out]TPEVT_COORDINATES *psLastCoords   : pointer to last coordinate
* @param         [out]int_t *pnPenStatus                : pointer to pending status
* @param         [in]uint8_t *puReadBuf                 : pointer to receive buffer
* @param         [in]int_t nFinger                      : number of finger index
* @retval        None.
******************************************************************************/
SCOPE_STATIC void TP_SetFingerInfo( TP_TouchFinger_st *psTouch, TPEVT_COORDINATES *psLastCoords, int_t *pnPenStatus, const uint8_t* const puReadBuf, const int_t nFinger )
{
    int                      nXin;
    int                      nYin;
    int32_t                  nXout;
    int32_t                  nYout;
    uint8_t                  unState;
    int32_t                  nPressure;
    nPressure = TPEVT_PRESS_THD;

    /* decide touch coordinate */
#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
    nXin = ((puReadBuf[5 + (nFinger * 6)] & 0x7) << 8) + puReadBuf[6 + (nFinger * 6)];
    nYin = ((puReadBuf[3 + (nFinger * 6)] & 0x7) << 8) + puReadBuf[4 + (nFinger * 6)];
#elif (TARGET_BOARD == TARGET_BOARD_RSK)
    nXin = ((puReadBuf[3 + (nFinger * 6)] & 0x7) << 8) + puReadBuf[4 + (nFinger * 6)];
    nYin = ((puReadBuf[5 + (nFinger * 6)] & 0x7) << 8) + puReadBuf[6 + (nFinger * 6)];
#endif

    TP_LineUpPos((int16_t) nXin, (int16_t) nYin, &nXout, &nYout);
    psTouch->unPosX = (uint16_t)nXout;
    psTouch->unPosY = (uint16_t)nYout;

    if(puReadBuf[2] > nFinger){
        unState = 1;
    }
    else{
        unState = 0;
    }

    if( unState == 0 )
    {
        if( *pnPenStatus == 1 )
        {
            /* EVENT : UP */
            *pnPenStatus    = 0;        /* Touch Panel pen status, 0 : up */
            psTouch->eState = TPEVT_ENTRY_UP;
        }
        else
        {
            /* EVENT : NONE */
            psTouch->eState = TPEVT_ENTRY_NONE;
        }
    }
    else
    {
        if( nPressure < TPEVT_PRESS_THD )
        {
            /* EVENT : NONE */
            psTouch->eState = TPEVT_ENTRY_NONE;
        }
        else if( *pnPenStatus == 0 )
        {
            /* EVENT : DOWN */
            *pnPenStatus    = 1;        /* Touch Panel pen status, 1 : down */
            psTouch->eState = TPEVT_ENTRY_DOWN;
        }
        else
        {
            /* EVENT : MOVE */
            psTouch->eState = TPEVT_ENTRY_MOVE;
        }
        (void)TP_EventHandle( &psTouch->eState, &nXout, &nYout, psLastCoords );
    }
}


/**************************************************************************//**
* Function Name: TP_LineUpPos
* @brief         Line up touch position.
*
*                Description:<br>
*
* @param         [in]int16_t nXin       : before X-coordinate
* @param         [in]int16_t nYin       : before Y-coordinate
* @param         [out]int32_t *pnXout   : pointer to after X-coordinate
* @param         [out]int32_t *pnYout   : pointer to after Y-coordinate
* @retval        None.
******************************************************************************/
SCOPE_STATIC void TP_LineUpPos( const int16_t nXin, const int16_t nYin, int32_t *pnXout, int32_t *pnYout )
{
    int_t   nScrWidth;
    int_t   nScrHeight;

    TP_GetScreenSize( &nScrWidth, &nScrHeight );

#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
    *pnXout = nXin;
    *pnYout = nYin;
#elif (TARGET_BOARD == TARGET_BOARD_RSK)
    if (1 == TP_GetAppBoardVersion())
    {
        *pnXout = nScrWidth - nXin;
        *pnYout = nScrHeight - nYin;
    }
    else if (2 == TP_GetAppBoardVersion())
    {
        *pnXout = nXin;
        *pnYout = nYin;
    }
    else
    {
        *pnXout = nScrWidth - nXin;
        *pnYout = nScrHeight - nYin;
    }
#endif

    DBG_printf_DBG("[TPDBG] S(%d:%d), I(%d:%d), O(%ld:%ld)\n", nScrWidth, nScrHeight, nXin, nYin, *pnXout, *pnYout);

    if(*pnXout < 0)
    {
        *pnXout = 0;
    }
    else if(*pnXout >= nScrWidth)
    {
        *pnXout = nScrWidth - 1 ;
    }
    else
    {
        /* do nothing */
    }

    if(*pnYout < 0)
    {
        *pnYout = 0 ;
    }
    else if (*pnYout >= nScrHeight)
    {
        *pnYout = nScrHeight - 1 ;
    }
    else
    {
        /* do nothing */
    }
}


/**************************************************************************//**
* Function Name: TP_EventHandle
* @brief         Handle event type.
*
*                Description:<br>
*
* @param         [out]TpEvt_EntryType *eEvent           : pointer to event variable
* @param         [in]int32_t nPosX                      : X-coordinate
* @param         [in]int32_t nPosY                      : Y-coordinate
* @param         [out]TPEVT_COORDINATES *psLastCoords   : pointer to last coordinate
* @retval          0 : Operation successfull.
*                 -1 : Error occured.
******************************************************************************/
SCOPE_STATIC int_t TP_EventHandle( TpEvt_EntryType *eEvent, const int32_t* const nPosX, const int32_t* const nPosY, TPEVT_COORDINATES *psLastCoords )
{
    int_t                    nRet ;
    int_t                    nOver ;
    TPEVT_COORDINATES        Coords ;

    nRet    = 0;

    Coords.x = *nPosX ;
    Coords.y = *nPosY ;
    switch( *eEvent ){
        case TPEVT_ENTRY_DOWN :
            psLastCoords->x = *nPosX ;
            psLastCoords->y = *nPosY ;
            break ;
        case TPEVT_ENTRY_MOVE :
            nOver = TP_CheckDiffMin( psLastCoords, &Coords, TPEVT_MOVE_MINTHD ) ;
            psLastCoords->x = *nPosX ;
            psLastCoords->y = *nPosY ;
            if( nOver == 0 )
            {
                *eEvent = TPEVT_ENTRY_UNKNOWN;
            }
            break ;
        case TPEVT_ENTRY_UP :
            break ;
        default :
            nRet = -1;
            break;
    }

    return nRet ;
}


/**************************************************************************//**
* Function Name: TP_CheckDiffMin
* @brief         Check amount of slide.
*
*                Description:<br>
*
* @param         [in]TPEVT_COORDINATES *psPox_1     : pointer to last coordinate
* @param         [in]TPEVT_COORDINATES *psPox_2     : pointer to current coordinate
* @param         [in]int32_t unThreshold            : threshold value
* @retval          1 : Move.
*                  0 : Not move.
******************************************************************************/
SCOPE_STATIC int_t TP_CheckDiffMin( const TPEVT_COORDINATES* const psPox_1, const TPEVT_COORDINATES* const psPox_2, const uint32_t unThreshold )
{
    int32_t     nX, nY;
    uint32_t    unData;

    nX = psPox_1->x - psPox_2->x ;
    nY = psPox_1->y - psPox_2->y ;
    unData = ( uint32_t )( nX * nX ) + ( uint32_t )( nY * nY ) ;

    return(( unData > unThreshold ) ? 1 : 0 ) ;
}


/**************************************************************************//**
* Function Name: TP_DirectSrchCb
* @brief         Search and call callback function.
*
*                Description:<br>
*
* @param         [in]TP_TouchEvent_st   : pointer to touch information
* @retval        Found event number
******************************************************************************/
SCOPE_STATIC int_t TP_DirectSrchCb( TP_TouchEvent_st *psTouchEvt )
{
    int_t           nRet;
    int_t           nI, nJ;
    TPEVT_ENTRY     *psEntry;

    nRet    = 0;

    DBG_printf_LOG( "[TOUCH],%d,%d,%d,%d,%d,%d\n",
                    psTouchEvt->sFinger[0].eState, psTouchEvt->sFinger[0].unPosX, psTouchEvt->sFinger[0].unPosY,
                    psTouchEvt->sFinger[1].eState, psTouchEvt->sFinger[1].unPosX, psTouchEvt->sFinger[1].unPosY );

    psEntry = TP_GetEventTable( 0 );
    for( nI = 0 ; nI < TPEVT_ENTRY_MAX ; nI++ )
    {
        if( psEntry->evtlock != TP_EVT_UNLOCK )
        {
            continue ;                  /* Locked */
        }
        for( nJ = 0; nJ < TP_TOUCHNUM_MAX; nJ ++ )
        {
            if( (( psTouchEvt->sFinger[nJ].eState & psEntry->mode ) != TPEVT_ENTRY_NONE) &&
                (( psTouchEvt->sFinger[nJ].unPosX >= psEntry->st.x ) && ( psTouchEvt->sFinger[nJ].unPosX < psEntry->ed.x )) &&
                (( psTouchEvt->sFinger[nJ].unPosY >= psEntry->st.y ) && ( psTouchEvt->sFinger[nJ].unPosY < psEntry->ed.y )) )
            {
                /** Event notification callback function */
                psEntry->function( nI, psTouchEvt );
                nRet ++;                       /* Exist */
                break;
            }
        }
        psEntry++;
    }

    return nRet ;
}


