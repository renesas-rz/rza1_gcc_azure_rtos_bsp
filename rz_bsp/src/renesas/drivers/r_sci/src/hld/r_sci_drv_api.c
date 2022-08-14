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
 * Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.
 *******************************************************************************
 * File Name    : r_sci_drv_api.c
 * Version      : 1.00
 * Device(s)    : Renesas
 * Tool-Chain   : GNUARM-NONE-EABI v14.02
 * OS           : FreeRTOS
 * H/W Platform : RSK+
 * Description  : Simple sample device driver for a SCIF HLD module
 ******************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 01.08.2009 1.00 First Release
 ******************************************************************************/

/******************************************************************************
 WARNING!  IN ACCORDANCE WITH THE USER LICENCE THIS CODE MUST NOT BE CONVEYED
 OR REDISTRIBUTED IN COMBINATION WITH ANY SOFTWARE LICENSED UNDER TERMS THE
 SAME AS OR SIMILAR TO THE GNU GENERAL PUBLIC LICENCE
 ******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include <stdio.h>
#include <string.h>

#include "iodefine_cfg.h"
#include "r_devlink_wrapper.h"

#include "r_os_abstraction_api.h"

#include "r_sci_drv_api.h"
#include "r_sci_drv_link.h"
#include "r_intc.h"

#include "scif_iobitmask.h"

/* Common configuration file for all ISR's */
#include "r_task_priority.h"
#include "mcu_board_select.h"


/******************************************************************************
 Defines
 ******************************************************************************/

/* Define the base pointer of the SCIF to use */
#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
#define P_SCI_BASE                  SCIF3
#elif (TARGET_BOARD == TARGET_BOARD_RSK)
#define P_SCI_BASE                  SCIF2
#endif

/* Define the size of the software FIFOs */
#define SCIF_RX_SOFTWARE_FIFO_SIZE  (1024)
#define SCIF_TX_SOFTWARE_FIFO_SIZE  (64 * 1024)
#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
#define SCI_PERIPHERAL_HZ           (64000000UL)
#elif (TARGET_BOARD == TARGET_BOARD_RSK)
#define SCI_PERIPHERAL_HZ           (66666666UL)
#endif

/* Define the interrupt priority in r_task_priority.h */
#define SCIF_INTERRUPT_DISABLED     0x00

/******************************************************************************
 Function Prototypes
 ******************************************************************************/
int scifOutputDebugString (uint8_t *pbyBuffer, uint32_t uiCount);

static int_t scif_open (st_stream_ptr_t pStream);
static void scif_close(st_stream_ptr_t pStream);
#if 1	/* grape */
int scif_read (st_stream_ptr_t pStream, uint8_t *pbyBuffer, uint32_t uiCount);
int scif_write (st_stream_ptr_t pStream, uint8_t *pbyBuffer, uint32_t uiCount);
#else
static int scif_read (st_stream_ptr_t pStream, uint8_t *pbyBuffer, uint32_t uiCount);
static int scif_write (st_stream_ptr_t pStream, uint8_t *pbyBuffer, uint32_t uiCount);
#endif
static int scif_control(st_stream_ptr_t pStream, uint32_t ctlCode, void *pCtlStruct);
static int_t scif_get_version(st_stream_ptr_t stream_ptr, st_ver_info_t *pVerInfo);

static void sciSetIRQ (_Bool bfEnable);
static void INT_SCIF_BRI (void);
static void INT_SCIF_ERI (void);
static void INT_SCIF_RXI (void);
static void INT_SCIF_TXI (void);

/******************************************************************************
 Constant Data
 ******************************************************************************/

static const st_drv_info_t gs_hld_info =
{
    {
        ((STDIO_SCIF_RZ_HLD_VERSION_MAJOR << 16) + STDIO_SCIF_RZ_HLD_VERSION_MINOR)
    },
    STDIO_SCIF_RZ_HLD_BUILD_NUM,
    STDIO_SCIF_RZ_HLD_DRV_NAME
};


/* Define the driver function table for this device */
const st_r_driver_t g_scif_driver =
{
    "SCIF Device Driver",
    scif_open,
    scif_close,
    scif_read,
    scif_write,
    scif_control,
    scif_get_version
};

/* Define the default configuration of this device */
static const SCIFCFG gScifDefaultConfig =
{
    /* Baud rate */
    115200UL,

    /* Line Coding - asynchronous with no flow control (N,8,1) */
    (SCI_PARITY_NONE | SCI_DATA_BITS_EIGHT | SCI_ONE_STOP_BIT),

    /* Peripheral clock frequency */
    SCI_PERIPHERAL_HZ,

    /* Pointer to interrupt control function */
    sciSetIRQ
};

/******************************************************************************
 Global Variables
 ******************************************************************************/
/* The data structure required by the lower level driver */
static DDSCIF gDDSCIF;

/* A reference counter for open and close functions */
static int giRefCount = 0;

/******************************************************************************
 Public Functions
 ******************************************************************************/

/******************************************************************************
 Function Name: scifOutputDebugString
 Description:   Function to write a debug string (for TRACE((""));)
 Arguments:     IN  pbyBuffer - Pointer to the source memory
 IN  uiCount - The number of bytes to write
 Return value:  0 for success -1 on error
 ******************************************************************************/
int scifOutputDebugString (uint8_t *pbyBuffer, uint32_t uiCount)
{
    if (giRefCount > 0)
    {
        cbPutPacket(gDDSCIF.pTxBuffer, (size_t) uiCount, pbyBuffer);
        cbCheckIn(gDDSCIF.pTxBuffer, (size_t) uiCount);

        /* Enable transmit interrupt */
        gDDSCIF.pPORT->SCSCR |= (uint16_t)gDDSCIF.scif_scscr_tie;
        return 0;
    }

    return -1;
}
/******************************************************************************
 End of function  scifOutputDebugString
 ******************************************************************************/

/******************************************************************************
 Private Functions
 ******************************************************************************/

/******************************************************************************
 Function Name: scif_open
 Description:   Function to open SCIF
 Arguments:     IN  pStream - Pointer to the file stream
 Return value:  0 for success otherwise -1
 ******************************************************************************/
static int_t scif_open (st_stream_ptr_t pStream)
{
    PCBUFF pRxBuffer;
    PCBUFF pTxBuffer;

    int_t  res = (-1);

    /* File stream is not used */
    (void) pStream;

    /* Check to see if it is already open */
    if (giRefCount > 0)
    {
        giRefCount++;
        return 0;
    }

    /* Create the software FIFOs */
    pRxBuffer = cbCreate( SCIF_RX_SOFTWARE_FIFO_SIZE);
    if (!pRxBuffer)
    {
        return (-1);
    }

    pTxBuffer = cbCreate( SCIF_TX_SOFTWARE_FIFO_SIZE);
    if (!pTxBuffer)
    {
        cbDestroy(gDDSCIF.pRxBuffer);
        return (-1);
    }

    res = R_SCI_InitialiseHwIf(pStream->sc_config_index);

    /* R_SCI_InitialiseHwIf needs to return DRV_SUCCESS for driver to be used */
    if(DRV_SUCCESS == res)
    {
#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
        /* Get the interrupt vectors */
        R_INTC_RegistIntFunc(INTC_ID_BRI3, (void (*) (uint32_t)) INT_SCIF_BRI);
        R_INTC_RegistIntFunc(INTC_ID_ERI3, (void (*) (uint32_t)) INT_SCIF_ERI);
        R_INTC_RegistIntFunc(INTC_ID_RXI3, (void (*) (uint32_t)) INT_SCIF_RXI);
        R_INTC_RegistIntFunc(INTC_ID_TXI3, (void (*) (uint32_t)) INT_SCIF_TXI);

        /* Set the interrupt priority */
        R_INTC_SetPriority(INTC_ID_BRI3, ISR_SCIF3_PRIORITY);
        R_INTC_SetPriority(INTC_ID_ERI3, ISR_SCIF3_PRIORITY);
        R_INTC_SetPriority(INTC_ID_RXI3, ISR_SCIF3_PRIORITY);
        R_INTC_SetPriority(INTC_ID_TXI3, ISR_SCIF3_PRIORITY);

        /* send the configuration id as an input to the sciOpen function */

        gDDSCIF.smart_config_id = pStream->sc_config_index;

        /* Start the lower level driver */
		if (sciOpen(&gDDSCIF, &P_SCI_BASE, (PSCIFCFG)&gScifDefaultConfig, pRxBuffer, pTxBuffer))
        {
            /* Destroy the software FIFOS */
            cbDestroy(pTxBuffer);
            cbDestroy(pRxBuffer);
            return (-1);
        }
#elif (TARGET_BOARD == TARGET_BOARD_RSK)
        /* Get the interrupt vectors */
        R_INTC_RegistIntFunc(INTC_ID_BRI2, (void (*) (uint32_t)) INT_SCIF_BRI);
        R_INTC_RegistIntFunc(INTC_ID_ERI2, (void (*) (uint32_t)) INT_SCIF_ERI);
        R_INTC_RegistIntFunc(INTC_ID_RXI2, (void (*) (uint32_t)) INT_SCIF_RXI);
        R_INTC_RegistIntFunc(INTC_ID_TXI2, (void (*) (uint32_t)) INT_SCIF_TXI);

        /* Set the interrupt priority */
        R_INTC_SetPriority(INTC_ID_BRI2, ISR_SCIF2_PRIORITY);
        R_INTC_SetPriority(INTC_ID_ERI2, ISR_SCIF2_PRIORITY);
        R_INTC_SetPriority(INTC_ID_RXI2, ISR_SCIF2_PRIORITY);
        R_INTC_SetPriority(INTC_ID_TXI2, ISR_SCIF2_PRIORITY);

        /* send the configuration id as an input to the sciOpen function */

        gDDSCIF.smart_config_id = pStream->sc_config_index;

        /* Start the lower level driver */
        if (sciOpen(&gDDSCIF, &P_SCI_BASE, (PSCIFCFG)&gScifDefaultConfig, pRxBuffer, pTxBuffer))
        {
            /* Destroy the software FIFOS */
            cbDestroy(pTxBuffer);
            cbDestroy(pRxBuffer);
            return (-1);
        }
#endif

        /* Increment the reference count */
        giRefCount++;
        return (0);
    }
    else
    {
        /* Destroy the software FIFOS */
        cbDestroy(pTxBuffer);
        cbDestroy(pRxBuffer);
    }
    return (-1);
}
/******************************************************************************
 End of function scif_open
 ******************************************************************************/

/******************************************************************************
 Function Name: scif_close
 Description:   Function to close the SCIF
 Arguments:     IN  pStream - Pointer to the file stream
 Return value:  none
 ******************************************************************************/
static void scif_close(st_stream_ptr_t pStream)
{
    /* File stream is not used */
    (void) pStream;

    /* Decrement the reference counter */
    giRefCount--;
    if (0 == giRefCount)
    {
        /* Stop the SCIF driver */
        sciClose(&gDDSCIF);

        /* Set pins back to default function */
        //TODO: Set SCI pins back to port pins
#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
        /* Revert the interrupt priority */
        R_INTC_SetPriority(INTC_ID_BRI3, SCIF_INTERRUPT_DISABLED);
        R_INTC_SetPriority(INTC_ID_ERI3, SCIF_INTERRUPT_DISABLED);
        R_INTC_SetPriority(INTC_ID_RXI3, SCIF_INTERRUPT_DISABLED);
        R_INTC_SetPriority(INTC_ID_TXI3, SCIF_INTERRUPT_DISABLED);
#elif (TARGET_BOARD == TARGET_BOARD_RSK)
        /* Revert the interrupt priority */
        R_INTC_SetPriority(INTC_ID_BRI2, SCIF_INTERRUPT_DISABLED);
        R_INTC_SetPriority(INTC_ID_ERI2, SCIF_INTERRUPT_DISABLED);
        R_INTC_SetPriority(INTC_ID_RXI2, SCIF_INTERRUPT_DISABLED);
        R_INTC_SetPriority(INTC_ID_TXI2, SCIF_INTERRUPT_DISABLED);
#endif

        /* Destroy the software FIFOS */
        cbDestroy(gDDSCIF.pTxBuffer);
        cbDestroy(gDDSCIF.pRxBuffer);

        /* Vectors not released as there is no API */
    }
}
/******************************************************************************
 End of function scif_close
 ******************************************************************************/

/******************************************************************************
 Function Name: scif_read
 Description:   Function to read data from the serial port
 Arguments:     IN  pStream - Pointer to the file stream
                IN  pbyBuffer - Pointer to the destination memory
                IN  uiCount - The number of bytes to read
 Return value:  data count for success or Error Code on error
 ******************************************************************************/
#if 1	/* grape */
int scif_read (st_stream_ptr_t pStream, uint8_t *pbyBuffer, uint32_t uiCount)
#else
static int scif_read (st_stream_ptr_t pStream, uint8_t *pbyBuffer, uint32_t uiCount)
#endif
{
    /* File stream is not used */
    (void) pStream;

    /* Check to make sure that the SCIF has been opened */
    if (giRefCount > 0)
    {
        /* Call the driver read function */
        if (!sciReadData(&gDDSCIF, pbyBuffer, (size_t) uiCount))
        {
            return (int) uiCount;
        }
    }

    return -1;
}
/******************************************************************************
 End of function scif_read
 ******************************************************************************/

/******************************************************************************
 Function Name: scif_write
 Description:   Function to read data from the serial port
 Arguments:     IN  pStream - Pointer to the file stream
                IN  pbyBuffer - Pointer to the source memory
                IN  uiCount - The number of bytes to write
 Return value:  data count for success or Error Code on error
 ******************************************************************************/
#if 1	/* grape */
int scif_write (st_stream_ptr_t pStream, uint8_t *pbyBuffer, uint32_t uiCount)
#else
static int scif_write (st_stream_ptr_t pStream, uint8_t *pbyBuffer, uint32_t uiCount)
#endif
{
    /* File stream is not used */
    (void) pStream;

    /* Check to make sure that the SCIF has been opened */
    if (giRefCount > 0)
    {
        R_OS_SuspendAllTasks();
        sciWriteData(&gDDSCIF, pbyBuffer, (size_t) uiCount);
        R_OS_ResumeAllTasks();
        R_OS_TaskSleep(5);

        return (int)uiCount;
    }

    return -1;
}
/******************************************************************************
 End of function scif_write
 ******************************************************************************/

/******************************************************************************
 Function Name: scif_control
 Description:   Function to handle custom control functions for SCIF
 Arguments:     IN  pStream - Pointer to the file stream
                IN  ctlCode - The custom control code
                IN  pCtlStruct - Pointer to the custom control structure
 Return value:  0 for success -1 or error code on error
 ******************************************************************************/
static int scif_control(st_stream_ptr_t pStream, uint32_t ctlCode, void *pCtlStruct)
{
    /* File stream is not used */
    (void) pStream;

    if (giRefCount > 0)
    {
        if ((ctlCode == CTL_SCI_SET_CONFIGURATION) && (pCtlStruct))
        {
            PSCICFG pSciCfg = (PSCICFG) pCtlStruct;

            /* Try to re-configure */
            if (sciReConfigure(&gDDSCIF, pSciCfg->dwBaud, pSciCfg->dwConfig))
            {
                /* If it fails set back to default */
                sciOpen(&gDDSCIF, &P_SCI_BASE, (PSCIFCFG)&gScifDefaultConfig, gDDSCIF.pRxBuffer, gDDSCIF.pTxBuffer);
                return -1;
            }

            return 0;
        }
        else if (CTL_GET_RX_BUFFER_COUNT == ctlCode)
        {
            return (int) cbUsed(gDDSCIF.pRxBuffer);
        }

        return -1;
    }

    return -1;
}
/******************************************************************************
 End of function  scif2Control
 ******************************************************************************/

/*******************************************************************************
 * Function Name: scif_get_version
 * Description  : Provides build information even if driver fails to open
 *                version information is updated by developer
 * Arguments    : none
 * Return Value : 0 (never fails)
 ******************************************************************************/
static int_t scif_get_version(st_stream_ptr_t stream_ptr, st_ver_info_t *pVerInfo)
{
    UNUSED_PARAM(stream_ptr);

    pVerInfo->hld.version.sub.major = gs_hld_info.version.sub.major;
    pVerInfo->hld.version.sub.minor = gs_hld_info.version.sub.minor;
    pVerInfo->hld.build = gs_hld_info.build;
    pVerInfo->hld.p_szdriver_name = gs_hld_info.p_szdriver_name;

    /* Obtain version information from Low layer Driver */
//    R_OSTM_GetVersion(pVerInfo);

    return 0;
}
/*******************************************************************************
 End of function scif_get_version
 ******************************************************************************/


/******************************************************************************
 Function Name: sciSetIRQ
 Description:   Function to enable / disable the SCIF interrupts
 Arguments:     IN  bfEnable - true to enable
 Return value:  0 for success otherwise error code
 ******************************************************************************/
static void sciSetIRQ (_Bool bfEnable)
{
    if (bfEnable)
    {
#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
        R_INTC_Enable(INTC_ID_BRI3);
        R_INTC_Enable(INTC_ID_ERI3);
        R_INTC_Enable(INTC_ID_RXI3);
        R_INTC_Enable(INTC_ID_TXI3);
#elif (TARGET_BOARD == TARGET_BOARD_RSK)
        R_INTC_Enable(INTC_ID_BRI2);
        R_INTC_Enable(INTC_ID_ERI2);
        R_INTC_Enable(INTC_ID_RXI2);
        R_INTC_Enable(INTC_ID_TXI2);
#endif
    }
    else
    {
#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
        R_INTC_Disable(INTC_ID_BRI3);
        R_INTC_Disable(INTC_ID_ERI3);
        R_INTC_Disable(INTC_ID_RXI3);
        R_INTC_Disable(INTC_ID_TXI3);
#elif (TARGET_BOARD == TARGET_BOARD_RSK)
        R_INTC_Disable(INTC_ID_BRI2);
        R_INTC_Disable(INTC_ID_ERI2);
        R_INTC_Disable(INTC_ID_RXI2);
        R_INTC_Disable(INTC_ID_TXI2);
#endif
    }
}
/******************************************************************************
 End of function sciSetIRQ
 ******************************************************************************/

/******************************************************************************
 Function Name: INT_SCIF_BRI
 Description:   Interrupt Service Routine for INT_SCIF_BRI
 Arguments:     none
 Return value:  none
 ******************************************************************************/
static void INT_SCIF_BRI (void)
{
    sciINTBreak(&gDDSCIF);
}
/******************************************************************************
 End of function INT_SCIF_BRI
 ******************************************************************************/

/******************************************************************************
 Function Name: INT_SCIF_ERI
 Description:   Interrupt Service Routine for INT_SCIF_ERI
 Arguments:     none
 Return value:  none
 ******************************************************************************/
static void INT_SCIF_ERI (void)
{
    sciINTError(&gDDSCIF);
}
/******************************************************************************
 End of function INT_SCIF_ERI
 ******************************************************************************/

/******************************************************************************
 Function Name: INT_SCIF_RXI
 Description:   Interrupt Service Routine for INT_SCIF_RXI
 Arguments:     none
 Return value:  none
 ******************************************************************************/
static void INT_SCIF_RXI (void)
{
    sciINTRxi(&gDDSCIF);
}
/******************************************************************************
 End of function INT_SCIF_RXI
 ******************************************************************************/

/******************************************************************************
 Function Name: INT_SCIF_TXI
 Description:   Interrupt Service Routine for INT_SCIF_TXI
 Arguments:     none
 Return value:  none
 ******************************************************************************/
static void INT_SCIF_TXI (void)
{
    sciINTTxi(&gDDSCIF);
}
/******************************************************************************
 End of function INT_SCIF_TXI
 ******************************************************************************/

/******************************************************************************
 End  Of File
 ******************************************************************************/
