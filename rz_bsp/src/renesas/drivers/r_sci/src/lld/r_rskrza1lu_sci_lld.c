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
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************
* File Name    : r_rskrza1lu_sci_lld.c
* Description  : Generic driver for the Serial Communications Interface with
*                FIFO
*******************************************************************************
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
#if (TARGET_RZA1 <= TARGET_RZA1LU)
#include <string.h>

#include "r_devlink_wrapper.h"

#include "r_os_abstraction_api.h"

#include "r_rskrza1lu_sci_lld.h"

#include "gpio_iobitmask.h"
#include "cpg_iobitmask.h"
#include "scif_iobitmask.h"

#include "r_intc.h"

//#include "task.h"

/******************************************************************************
Defines
******************************************************************************/

#define SCI_BAUD_MAX_PERCENT_ERR    4
#define SCI_MAX_DIVIDER_CLOCK       4
#define SCI_BAUD_OUT_OF_RANGE       SCI_MAX_DIVIDER_CLOCK

/******************************************************************************
Function Prototypes
******************************************************************************/

static SCIERR sciConfigureSerialMode(PSCIF pPORT, PSCIFCFG pConfig);
static uint32_t sciSetBaudRate(uint8_t * pbyBRR, uint32_t dwBaud, uint32_t dwPeripheralClockFrequency,
        _Bool bfSync, uint8_t byDividerClockCode, uint16_t *pwSCEMR);
static _Bool sciCheckPercentageError(uint32_t dwDesired, uint32_t dwActual);
static uint8_t sciCalculateDividerClock(uint32_t dwClockFrequencyInHz, uint32_t dwBaud, _Bool bfSync, _Bool bfABCS);
static uint8_t sciCalculateBRRSetting(uint32_t dwClockFrequencyInHz, uint32_t dwBaud, uint8_t byDivisor, _Bool bfSync, _Bool bfABCS);
static uint32_t sciCalculateBaud(uint32_t dwClockFrequencyInHz, uint8_t  byDivisor, uint8_t byBRR, _Bool bfSync, _Bool bfABCS);

/******************************************************************************
Constant Data
******************************************************************************/

/* Baud multiplier tables for clock divisor setting */
static const uint16_t gcSCIAsynchMultiply[4] =
{
    (uint16_t) 32, (uint16_t) 128, (uint16_t) 512, (uint16_t) 2048
};

static const uint16_t gcSCISynchMultiply[4] =
{
    (uint16_t) 4, (uint16_t) 16, (uint16_t) 64, (uint16_t) 256
};

typedef struct _r_drv_sci_cfg_ch_config_t
{
    int_t                 channel;            /* specific channel */

    /* These elements can be modified using configurations from smart configurator */
    uint16_t              isr_priority;

    /* These elements should not be modified */
    void*                 sci;                /* Enable channel in the peripheral, must exist */

    volatile uint8_t *    cpg_reg;            /* CPG MODULE Register */

    /* Interrupts */
    uint32_t              intc_id_bri;        /* INTC_ID_BRI Interrupt handler */
    uint32_t              intc_id_eri;        /* INTC_ID_ERI Interrupt handler */
    uint32_t              intc_id_rxi;        /* INTC_ID_RXI Interrupt handler */
    uint32_t              intc_id_txi;        /* INTC_ID_TXI Interrupt handler */
    uint32_t              scif_scscr_tie;     /* SCIFx_SCSCR_TIE Interrupt Enable */

    uint32_t              cpg_stb_ctrl;       /* CPG_STBCR4_MSTP44 Module Control */

    /* fifo's */
    uint32_t             scifx_scfcr_tfrst;   /* SCIFx_SCFCR_TFRST */
    uint32_t             scifx_scfcr_rfrst;   /* SCIFx_SCFCR_RFRST */
    uint32_t             scifx_scfcr_rtrg;    /* SCIFx_SCFCR_RTRG */
    uint32_t             scifx_scfcr_ttrg;    /* SCIFx_SCFCR_TTRG */
    uint32_t             scifx_scsmr_cks;     /* SCIFx_SCSMR_CKS */
    uint32_t             scifx_scscr_te;      /* SCIFx_SCSCR_TE */
    uint32_t             scifx_scscr_re;      /* SCIFx_SCSCR_RE */
    uint32_t             scifx_scscr_rie;     /* SCIFx_SCSCR_RIE */
    uint32_t             scifx_scfdr_t;       /* SCIFx_SCFDR_T */
    uint32_t             scifx_scfsr_tend;    /* SCIFx_SCFSR_TEND */


} r_drv_sci_cfg_ch_config_t;

static const r_drv_sci_cfg_ch_config_t r_sci_device_config[] =
{
    /* channel 3 */
   {
       R_SC3, 0, &SCIF3, &CPG.STBCR4,

        /* Interrupts */
       INTC_ID_BRI3, INTC_ID_ERI3, INTC_ID_RXI3, INTC_ID_TXI3, SCIF3_SCSCR_TIE, CPG_STBCR4_MSTP44,

       /* fifo's */
       SCIF3_SCFCR_TFRST, SCIF3_SCFCR_RFRST, SCIF3_SCFCR_RTRG, SCIF3_SCFCR_TTRG,  SCIF3_SCSMR_CKS,
       SCIF3_SCSCR_TE, SCIF3_SCSCR_RE, SCIF3_SCSCR_RIE, SCIF3_SCFDR_T, SCIF3_SCFSR_TEND
   },

   /* channel x */
   {

   },
};

static int32_t lookup_config(e_sc_id_t id)
{
    int32_t index = (sizeof(r_sci_device_config))/sizeof(r_drv_sci_cfg_ch_config_t);
    int32_t found = (-1);

    while(index--)
    {
        if(r_sci_device_config[index].channel == id)
        {
            found = index;
            break;
        }
    }
    return (found);
}

/******************************************************************************
Public Functions
******************************************************************************/


/**********************************************************************************************************************
 * Function Name: R_SCI_InitialiseHwIf
 * Description  : LowLevel driver initialise interface function
 * Arguments    : none
 * Return value : DRV_SUCCESS : Success when attempting to initialise SCI
 *                DRV_ERROR   : Failure to initialise SCI
 *********************************************************************************************************************/
int_t   R_SCI_InitialiseHwIf(e_sc_id_t id)
{
    int_t res = (-1);

    res = lookup_config(id);

    if((-1) != res)
    {
        volatile unsigned char dummy;

        /* Configure the port pins for use by SCIF3 NEED SMART CONFIGURATOR  */
        /* TXD3 (P7_11) */
        GPIO.PMC7 |= GPIO_PMC7_PMC711;
        GPIO.PFC7 &= (uint16_t)(~GPIO_PFC7_PFC711);
        GPIO.PFCE7 &= (uint16_t)(~GPIO_PFCE7_PFCE711);
        GPIO.PFCAE7 |= GPIO_PFCAE7_PFCAE711;
        GPIO.PIPC7 |= GPIO_PIPC7_PIPC711;

        /* RXD3 (P7_10) */
        GPIO.PMC7 |= GPIO_PMC7_PMC710;
        GPIO.PFC7 &=(uint16_t) (~GPIO_PFC7_PFC710);
        GPIO.PFCE7 &=(uint16_t) (~GPIO_PFCE7_PFCE710);
        GPIO.PFCAE7 |= GPIO_PFCAE7_PFCAE710;
        GPIO.PIPC7 |= GPIO_PIPC7_PIPC710;

        /* Enable SCIF3 module */
        (*r_sci_device_config[res].cpg_reg) = (volatile uint8_t) (*r_sci_device_config[res].cpg_reg) & (uint8_t) ~(r_sci_device_config[res].cpg_stb_ctrl);
        dummy = (volatile uint8_t) *r_sci_device_config[res].cpg_reg;
        (void) dummy;
        res = DRV_SUCCESS;
    }
    return (res);
}
/**********************************************************************************************************************
 End of function R_SCI_InitialiseHwIf
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_SCI_UninitialiseHwIf
 * Description  : LowLevel driver un-initialise interface function
 * Arguments    : none
 * Return Value : none
 **********************************************************************************************************************/
void R_SCI_UninitialiseHwIf(e_sc_id_t id)
{
    volatile unsigned char dummy;
    int_t res = (-1);

    res = lookup_config(id);

    if((-1) != res)
    {
        /* Disable SCIF module */
        (*r_sci_device_config[id].cpg_reg) = (volatile uint8_t) (*r_sci_device_config[id].cpg_reg) | (uint8_t) r_sci_device_config[id].cpg_stb_ctrl;
        dummy = (volatile uint8_t) *r_sci_device_config[id].cpg_reg;
        (void) dummy;
    }
}
/*******************************************************************************
 End of function R_SCI_UninitialiseHwIf
 ******************************************************************************/

/******************************************************************************
Function Name: sciOpen
Description:   Function to initialise/start the SCIF
Arguments:     I/O pDDSCIF - Pointer to the driver data
               IN  pSCIF - Pointer to the base address of the SCIF
               IN  pConfiguration - Pointer to the configuration data
               IN  pRxBuffer - Pointer to the Receive circular buffer
               IN  pTxBuffer - Pointer to the Transmit circular buffer
Return value:  0 for success otherwise error code
******************************************************************************/
SCIERR sciOpen(PDDSCIF pDDSCIF, PSCIF pSCIF, PSCIFCFG pConfiguration, PCBUFF pRxBuffer, PCBUFF pTxBuffer)
{
    _Bool bfSync = false;
    SCIERR sciError = DDSCI_OK;
    PSCIF pPORT = (PSCIF) pSCIF;
    uint8_t byDividerClockCode;
    volatile int_t res = (-1);
    e_sc_id_t sc_config_index = pDDSCIF->smart_config_id;

    res = lookup_config(sc_config_index);

    if((-1) != res)
    {
        /* Initialise the driver data */
        memset(pDDSCIF, 0, sizeof(DDSCIF));

        /* Update the channel information */
        pDDSCIF->smart_config_id = sc_config_index ;
        pDDSCIF->res = (uint32_t)res;

        pDDSCIF->intc_id_bri     = r_sci_device_config[res].intc_id_bri;
        pDDSCIF->intc_id_eri     = r_sci_device_config[res].intc_id_eri;
        pDDSCIF->intc_id_rxi     = r_sci_device_config[res].intc_id_rxi;
        pDDSCIF->intc_id_txi     = r_sci_device_config[res].intc_id_txi;
        pDDSCIF->scif_scscr_tie  = r_sci_device_config[res].scif_scscr_tie;

        /* Create the RX char event */
        R_OS_CreateEvent(&pDDSCIF->evReceive);

        /* Take a copy of the configuration */
        pDDSCIF->sciConfig = *pConfiguration;

        /* Set the pointer to the port */
        pDDSCIF->pPORT = pPORT;

        /* Set the pointers to the software buffers */
        pDDSCIF->pRxBuffer = pRxBuffer;
        pDDSCIF->pTxBuffer = pTxBuffer;

        /* Check for synchronous mode */
        if (pConfiguration->dwConfig & SCI_SYNCHRONOUS)
        {
            bfSync = true;
        }

/* 1. Clear TE, RE, RIE, TIE and TEIE to 0 disable the serial port*/  // LLD
        pPORT->SCSCR = (uint16_t) 0;
        pPORT->SCSMR = (uint16_t) 0;

        /* Clear the FIFO */
        pPORT->SCFCR = (volatile uint16_t) (pPORT->SCFCR | r_sci_device_config[res].scifx_scfcr_tfrst);    /* SCIFx_SCFCR_TFRST */
        pPORT->SCFCR = (volatile uint16_t) (pPORT->SCFCR | r_sci_device_config[res].scifx_scfcr_rfrst);    /* SCIFx_SCFCR_RFRST */

        /* Set the FIFO trigger levels */
        pPORT->SCFCR &= (uint16_t) ~(r_sci_device_config[res].scifx_scfcr_rtrg);  /* SCIFx_SCFCR_RTRG */
        pPORT->SCFCR &= (uint16_t)~(r_sci_device_config[res].scifx_scfcr_ttrg);  /* SCIFx_SCFCR_TTRG */

        /* Allow FIFO operation */
        pPORT->SCFCR &= (uint16_t)~(r_sci_device_config[res].scifx_scfcr_tfrst); /* SCIFx_SCFCR_TFRST */
        pPORT->SCFCR &= (uint16_t)~(r_sci_device_config[res].scifx_scfcr_rfrst); /* SCIFx_SCFCR_RFRST */

        /* Calculate the divider clock first */
        byDividerClockCode = sciCalculateDividerClock(pConfiguration->dwPeripheralClockFrequency,
                                     pConfiguration->dwBaud, bfSync, false);

        /* set the divider clock */
        pPORT->SCSMR &= (uint16_t)~(r_sci_device_config[res].scifx_scsmr_cks); /* SCIFx_SCSMR_CKS */
        pPORT->SCSMR = (uint16_t) (pPORT->SCSMR | (uint16_t) byDividerClockCode);

/* 2. set the port configuration */ // HLD
        sciError |= sciConfigureSerialMode(pPORT, pConfiguration);

/* 3. set the baud rate */ // LLD
		pDDSCIF->dwActualBaud = sciSetBaudRate((uint8_t *)&pPORT->SCBRR, pConfiguration->dwBaud,
                           pConfiguration->dwPeripheralClockFrequency, bfSync, byDividerClockCode, NULL);

        /* Check the error */
        if (!sciCheckPercentageError(pConfiguration->dwBaud, pDDSCIF->dwActualBaud))
        {
            sciError |= DDSCI_BAD_BAUD_MATCH;
        }

/* 4. Enable RXD & TXD */ // LLD
        pPORT->SCSCR = (volatile uint16_t) (pPORT->SCSCR | r_sci_device_config[res].scifx_scscr_te); /* SCIFx_SCSCR_TE */
        pPORT->SCSCR = (volatile uint16_t) (pPORT->SCSCR | r_sci_device_config[res].scifx_scscr_re); /* SCIFx_SCSCR_RE */

        /* Enable receive interrupts */
        pPORT->SCSCR = (volatile uint16_t) (pPORT->SCSCR | r_sci_device_config[res].scifx_scscr_rie); /* SCIFx_SCSCR_RIE */

        /* Enable module interrupts */
        pConfiguration->pSetIRQ(true);
    }
    else
    {
        sciError = DDSCI_DEVICE_ERROR;
    }

    return sciError;
}
/******************************************************************************
End of function sciOpen
******************************************************************************/



/******************************************************************************
Function Name: sciClose
Description:   Function to stop the SCIF
Arguments:     IN  pDDSCIF - Pointer to the driver data
Return value:  none
******************************************************************************/
void sciClose(PDDSCIF pDDSCIF)
{
    PSCIF pPORT = pDDSCIF->pPORT;

    /* Wait for any remaining data to be transmitted */ // make this deterministic timeout todo RC
    while (pPORT->SCFDR & r_sci_device_config[pDDSCIF->res].scifx_scfdr_t) /* SCIFx_SCFDR_T */
    {
        /* Wait */;
    }

    /* Wait for the last byte to go */
    while ((!pPORT->SCFSR) & r_sci_device_config[pDDSCIF->res].scifx_scfsr_tend) /* SCIF3_SCFSR_TEND */
    {
        /* Wait */;
    }

    /* Disable interrupts */
    pDDSCIF->sciConfig.pSetIRQ(false);

    /* Disable SCIF */
    pPORT->SCSCR = (uint16_t) 0;

    /* Free the receive signal */ // HLD
//    eventDestroy(&pDDSCIF->pevReceive, 1);
    R_OS_DeleteEvent(&pDDSCIF->evReceive);
}
/******************************************************************************
End of function sciClose
******************************************************************************/

/******************************************************************************
Function Name: sciReConfigure
Description:   Function to re-configure the line-coding
Arguments:     IN  pDDSCIF - Pointer to the driver data
               IN  dwBaud - The desired baud rate
               IN  dwConfig - The desired configuration
Return value:  0 for success
******************************************************************************/
int sciReConfigure(PDDSCIF pDDSCIF, uint32_t dwBaud, uint32_t dwConfig)
{
    SCIFCFG sciNewConfig = pDDSCIF->sciConfig;
    sciNewConfig.dwBaud = dwBaud;
    sciNewConfig.dwConfig = dwConfig;

    /* Close the port */
    sciClose(pDDSCIF);

    /* Try to re-open with the updated configuration */
    return sciOpen(pDDSCIF, pDDSCIF->pPORT, &sciNewConfig, pDDSCIF->pRxBuffer, pDDSCIF->pTxBuffer);
}
/******************************************************************************
End of function  sciReConfigure
******************************************************************************/

/******************************************************************************
Function Name: sciReadData
Description:   Function to start reading data from the SCIF
Arguments:     IN  pDDSCIF - Pointer to the driver data
               IN  pbyDest - Pointer to the destination memory
               IN  stLength - The length of data to read
Return value:  0 for success otherwise an error code
******************************************************************************/
SCIERR sciReadData(PDDSCIF pDDSCIF, uint8_t *pbyDest, size_t stLength)
{ // TODO RC Add  a mode, option 1 wait for data option 2 return with how much data you have and return now
    PCBUFF pcBuffer = pDDSCIF->pRxBuffer;
    uint8_t * pbyEnd = pbyDest + stLength;

    /* For the length of data */
    while (pbyDest < pbyEnd)
    {
        /* Get the data from the buffer */
        if (cbGet(pcBuffer, pbyDest))
        {
            pbyDest++;
        }
        else
        {
            R_OS_WaitForEvent(&pDDSCIF->evReceive, R_OS_ABSTRACTION_PRV_EV_WAIT_INFINITE);
        }
    }

    /* Check to see if there has been an error */
    if (pDDSCIF->errorCode)
    {
        /* Return the error code */
        SCIERR  errorCode = pDDSCIF->errorCode;
        pDDSCIF->errorCode = DDSCI_OK;
        return errorCode;
    }

    return DDSCI_OK;
}
/******************************************************************************
End of function sciReadData
******************************************************************************/



/******************************************************************************
Function Name: sciWriteData
Description:   Function to start writing data to the SCIF
Arguments:     IN  pDDSCIF - Pointer to the driver data
               IN  pbySrc - Pointer to the source  memory
               IN  stLength - The length of data to write
Return value:  none
******************************************************************************/
void sciWriteData(PDDSCIF pDDSCIF, uint8_t *pbySrc, size_t stLength)
{ // same as read
    PCBUFF pcBuffer = pDDSCIF->pTxBuffer;
    uint8_t * pbyEnd = pbySrc + stLength;

    /* For the length of data */
    while (pbySrc < pbyEnd)
    {
        /* Put the data in the buffer */
        if (cbPut(pcBuffer, *pbySrc))
        {
            /* Increment the pointer */
            pbySrc++;
        }
        else
        {
            /* Switch to another task */
            R_OS_Yield();
        }
    }

    /* Enable transmit interrupt */
    pDDSCIF->pPORT->SCSCR = (volatile uint16_t) (pDDSCIF->pPORT->SCSCR | r_sci_device_config[pDDSCIF->res].scif_scscr_tie); /* SCIFx_SCSCR_TIE */
}
/******************************************************************************
End of function sciWriteData
******************************************************************************/


/**********************************************************************************
Function Name: sciWaitTx
Description:   Function to wait for the TX software FIFO to become empty
Parameters:    IN  pDDSCIF - Pointer to the driver data
Return value:  none
**********************************************************************************/
void sciWaitTx(PDDSCIF pDDSCIF)
{
    PCBUFF pcBuffer = pDDSCIF->pTxBuffer;

    while (true)
    {
        if (cbUsed(pcBuffer) == 0UL)
        {
            break;
        }

        R_OS_Yield();
    }
}
/**********************************************************************************
End of function  sciWaitTx
***********************************************************************************/

/******************************************************************************
The interrupt service routines
******************************************************************************/

/******************************************************************************
Function Name: sciINTBreak
Description:   The break error interrupt handler
Arguments:     IN  pDDSCIF - Pointer to the driver data
Return value:  none
******************************************************************************/
void sciINTBreak(PDDSCIF pDDSCIF)
{
    PSCIF pPORT = pDDSCIF->pPORT;

    if ((pPORT->SCFSR&SCIF0_SCFSR_BRK) == SCIF0_SCFSR_BRK)
    {
        pPORT->SCFSR &= (uint16_t)~SCIF0_SCFSR_BRK;
        pDDSCIF->errorCode |= DDSCI_BREAK_ERROR;
    }

    if ((pPORT->SCLSR&SCIF0_SCLSR_ORER) == SCIF0_SCLSR_ORER)
    {
        pPORT->SCLSR &= (uint16_t)~SCIF0_SCLSR_ORER;
        pDDSCIF->errorCode |=  DDSCI_OVERRUN_ERROR;
    }
}
/******************************************************************************
End of function sciINTBreak
******************************************************************************/

/******************************************************************************
Function Name: sciINTError
Description:   The break error interrupt handler
Arguments:     IN  pDDSCIF - Pointer to the driver data
Return value:  none
******************************************************************************/
void sciINTError(PDDSCIF pDDSCIF)
{
    PSCIF pPORT = pDDSCIF->pPORT;

    if ((pPORT->SCFSR&SCIF0_SCFSR_ER) == SCIF0_SCFSR_ER)
    {
        pPORT->SCFSR &= (uint16_t)~SCIF0_SCFSR_ER;
        pDDSCIF->errorCode |=  DDSCI_DEVICE_ERROR;
    }

    if ((pPORT->SCFSR&SCIF0_SCFSR_FER) == SCIF0_SCFSR_FER)
    {
        pPORT->SCFSR &= (uint16_t)~SCIF0_SCFSR_FER;
        pDDSCIF->errorCode |=  DDSCI_FRAME_ERROR;
    }

    if ((pPORT->SCFSR&SCIF0_SCFSR_PER) == SCIF0_SCFSR_PER)
    {
        pPORT->SCFSR &= SCIF0_SCFSR_PER;
        pDDSCIF->errorCode |=  DDSCI_PARITY_ERROR;
    }
}
/******************************************************************************
End of function sciINTError
******************************************************************************/

/******************************************************************************
Function Name: sciINTRxi
Description:   Receive data interrupt service routine handler
Arguments:     IN  pDDSCIF - Pointer to the driver data
Return value:  none
******************************************************************************/
void sciINTRxi(PDDSCIF pDDSCIF)
{
    PSCIF pPORT = pDDSCIF->pPORT;
    PCBUFF pcBuffer = pDDSCIF->pRxBuffer;

    /* While there is data in the FIFO */
    while (pPORT->SCFDR & SCIF0_SCFDR_R)
    {
        /* If there is remaining buffer */
        if (!cbPut(pcBuffer, pPORT->SCFRDR))
        {
            /* Show that data has been lost */
            pDDSCIF->errorCode |= DDSCI_RX_BUFFER_FULL;
        }
    }

    /* Set the event to wake a task waiting on the event */
    R_OS_SetEvent(&pDDSCIF->evReceive);

    /* Clear the FULL flag */
    pPORT->SCFSR &= (uint16_t)~SCIF0_SCFSR_RDF;
}
/******************************************************************************
End of function sciINTRxi
******************************************************************************/

/******************************************************************************
Function Name: sciINTTxi
Description:   Receive data interrupt service routine handler
Arguments:     IN  pDDSCIF - Pointer to the driver data
Return value:  none
******************************************************************************/
void sciINTTxi(PDDSCIF pDDSCIF)
{
    PSCIF pPORT = pDDSCIF->pPORT;
    PCBUFF pcBuffer = pDDSCIF->pTxBuffer;

    /* While there is data to be transmitted and there is free FIFO */
    while (((pPORT->SCFDR&SCIF0_SCFDR_T) >> SCIF0_SCFDR_T_SHIFT) < 0x10)
    {
        /* Deliver the data to the FIFO */
        if (!cbGet(pcBuffer, (uint8_t *) &pPORT->SCFTDR))
        {
            /* If there is no more to go then clear the empty flag */
            pPORT->SCSCR = (volatile uint16_t) (pPORT->SCSCR & ~SCIF0_SCSCR_TIE);
            break;
        }
    }

    /* Clear the empty flag */
    pPORT->SCFSR = (volatile uint16_t) (pPORT->SCFSR & ~SCIF0_SCFSR_TDFE);
}
/******************************************************************************
End of function sciINTTxi
******************************************************************************/

/******************************************************************************
Private Functions
******************************************************************************/

/******************************************************************************
Function Name: sciConfigureSerialMode
Description:   Function to configure the SCIF serial mode
Arguments:     IN  pPORT - Pointer to the SCIF port to configure
               IN  pConfig - Pointer to the configuration
Return value:  0 for success otherwise error code
******************************************************************************/
static SCIERR sciConfigureSerialMode(PSCIF pPORT, PSCIFCFG pConfig)
{
    SCIERR sciError = DDSCI_OK;

    /* Check for Sync/Async operation */
    if (pConfig->dwConfig & SCI_SYNCHRONOUS)
    {
        /* Set clock settings before serial mode */
        if (pConfig->dwConfig & SCI_SYNCCLOCK_INPUT)
        {
            pPORT->SCSCR = (volatile uint16_t) (pPORT->SCSCR & ~SCIF0_SCSCR_CKE);
            pPORT->SCSCR |= 2 << SCIF0_SCSCR_CKE_SHIFT;
        }

        /* Set serial mode */
        pPORT->SCSMR = (uint16_t) 0;

        /* Check for Async specific settings */
        if (pConfig->dwConfig & (SCI_PARITY_EVEN | SCI_PARITY_ODD | SCI_DATA_BITS_SEVEN | SCI_TWO_STOP_BIT | SCI_X16CLOCK_INPUT | SCI_ASYNCLOCK_OUTPUT))
        {
            sciError = DDSCI_INVALID_MODE;
        }
    }
    else
    {
        /* Set clock settings before serial mode */
        if (pConfig->dwConfig & SCI_ASYNCLOCK_OUTPUT)
        {
            /* Clock output @ BRR */
            pPORT->SCSCR = (volatile uint16_t) (pPORT->SCSCR & ~SCIF0_SCSCR_CKE);
            pPORT->SCSCR |=  1 << SCIF0_SCSCR_CKE_SHIFT;
            if (pConfig->dwConfig & SCI_X16CLOCK_INPUT)
            {
                sciError = DDSCI_INVALID_MODE;
            }
        }

        if (pConfig->dwConfig & SCI_X16CLOCK_INPUT)
        {
            /* Clock output @ BRR * 16 */
            pPORT->SCSCR = (volatile uint16_t) (pPORT->SCSCR & ~SCIF0_SCSCR_CKE);
            pPORT->SCSCR |=  3 << SCIF0_SCSCR_CKE_SHIFT;
        }

        /* set parity */
        if (pConfig->dwConfig & SCI_PARITY_EVEN)
        {
            pPORT->SCSMR |= SCIF0_SCSMR_PE;
            pPORT->SCSMR = (volatile uint16_t) (pPORT->SCSMR & ~SCIF0_SCSMR_OE);
        }

        if (pConfig->dwConfig & SCI_PARITY_ODD)
        {
            pPORT->SCSMR |= SCIF0_SCSMR_PE;
            pPORT->SCSMR |= SCIF0_SCSMR_OE;
        }

        /* set data bits */
        if (pConfig->dwConfig & SCI_DATA_BITS_SEVEN)
        {
            pPORT->SCSMR |= SCIF0_SCSMR_CHR;
        }

        /* set stop bits */
        if (pConfig->dwConfig & SCI_TWO_STOP_BIT)
        {
            pPORT->SCSMR |= SCIF0_SCSMR_STOP;
        }

        /* Check synchronous specific settings */
        if (pConfig->dwConfig & (SCI_MULTI_PROCESSOR | SCI_SYNCCLOCK_INPUT))
        {
            sciError = DDSCI_INVALID_MODE;
        }
    }

    return sciError;
}
/******************************************************************************
End of function sciConfigureSerialMode
******************************************************************************/

/******************************************************************************
Function Name: sciSetBuadRate
Description:   Function to calculate the BRR setting
Arguments:     OUT pbyBRR - Pointer to the BRR
               IN  dwBaud - The baud rate to set
               IN  dwPeripheralClockFrequency - The peripheral clock frequency
               IN  bfSync - true for synchronous mode
               IN  byDividerClockCode - The CKS diver clock setting
               IN  pwSCEMR - Pointer to the extended serial mode register
Return value:  The actual baud rate set
******************************************************************************/
static uint32_t sciSetBaudRate(uint8_t     *pbyBRR,
                               uint32_t     dwBaud,
                               uint32_t     dwPeripheralClockFrequency,
                               _Bool        bfSync,
                               uint8_t      byDividerClockCode,
                               uint16_t    *pwSCEMR)
{
    uint32_t dwBaudHigh, dwDiffHigh, dwBaudLow, dwDiffLow, dwCount, dwDelayCnt;
    uint32_t dwActualBaud;
    uint8_t byBitRateRegister;
    _Bool bfABCS = false;

    do
    {
        /* Calculate the BRR setting */
        byBitRateRegister = sciCalculateBRRSetting(dwPeripheralClockFrequency,
                                                   dwBaud,
                                                   byDividerClockCode,
                                                   bfSync,
                                                   bfABCS);

        /* Calculate the higher bit rate */
        dwBaudHigh = sciCalculateBaud(dwPeripheralClockFrequency,
                                      byDividerClockCode,
                                      byBitRateRegister,
                                      bfSync,
                                      bfABCS);

        /* Test and range */
        if ((byBitRateRegister) < 255)
        {
            /* calculate the lower bit rate */
            dwBaudLow = sciCalculateBaud(dwPeripheralClockFrequency,
                                         byDividerClockCode,
                                         (uint8_t)(byBitRateRegister + 1),
                                         bfSync,
                                         bfABCS);
        }
        else
        {
            /* This is the closest because we have run out of bits */
            dwBaudLow = dwBaudHigh;
        }

        /* Calculate the difference between target and high */
        if (dwBaudHigh >= dwBaud)
        {
            dwDiffHigh = dwBaudHigh - dwBaud;
        }
        else
        {
            dwDiffHigh =  dwBaud - dwBaudHigh;
        }

        /* Calculate the difference between target and low */
        if (dwBaudLow >= dwBaud)
        {
            dwDiffLow = dwBaudLow - dwBaud;
        }
        else
        {
            dwDiffLow =  dwBaud - dwBaudLow;
        }

        /* Choose the closest match */
        if (dwDiffHigh <= dwDiffLow)
        {
            *pbyBRR = byBitRateRegister;
            dwActualBaud = dwBaudHigh;
        }
        else
        {
            *pbyBRR = (uint8_t)(byBitRateRegister + 1);
            dwActualBaud = dwBaudLow;
        }

        /* Check to see if we have a good match */
        if (sciCheckPercentageError(dwBaud, dwActualBaud))
        {
            break;
        }        

        /* If there is not an extended mode register stop now */
        if (!pwSCEMR)
        {
            break;
        }

    }
    while (!bfABCS++);

    /* Set the ABCS bit as desired */
    if (pwSCEMR)
    {
        if (bfABCS)
        {
            /* The ABCS bit has been moved to BIT_0 and in its old position
               (BIT_7) is now a "double rate" flag - which has the same effect
                but does not half the signal sampling */
            *pwSCEMR = 0x80;
        }
        else
        {
            *pwSCEMR = 0;
        }
    }

    /* More than 1 bit delay */
    dwDelayCnt = dwPeripheralClockFrequency / dwActualBaud >> 3;

    for (dwCount = 0; dwCount < dwDelayCnt; dwCount++)
    {
        /* wait */;
    }

    return dwActualBaud;
}
/******************************************************************************
End  of function sciSetBuadRate
******************************************************************************/

/******************************************************************************
Function Name: sciCheckPercentageError
Description:   Function to check the percentage error
Arguments:     IN  dwDesired - The desired baud rate
               IN  dwActual - The actual baud rate
Return value:  true if the actual baud rate is within the error tolerance
******************************************************************************/
static _Bool sciCheckPercentageError(uint32_t dwDesired, uint32_t dwActual)
{
    uint32_t   dwError;
    float   fPercentError;

    /* Calculate the difference */
    if (dwDesired > dwActual)
    {
        dwError = dwDesired - dwActual;
    }
    else
    {
        dwError = dwActual - dwDesired;
    }

    /* Calculate the percentage error */
    fPercentError = ((float) dwError / (float) dwDesired) * 100;

    /* Test for range */
    if (fPercentError > SCI_BAUD_MAX_PERCENT_ERR)
    {
        return false;
    }

    return true;
}
/******************************************************************************
End  of function sciCheckPercentageError
******************************************************************************/

/******************************************************************************
Function Name: sciCalculateDividerClock
Description:   Function to calculate the settings of the CKS bits given the
               peripheral clock frequency and the desired baud rate
Arguments:     IN  dwClockFrequencyInHz - The peripheral clock frequency
               IN  dwBaud - The desired baud rate
               IN  bfSync - true to calculate for synchronous mode
               IN  bfABCS - The Asynchronous base clock select setting
Return value:  0 for success otherwise error code
******************************************************************************/
static uint8_t sciCalculateDividerClock(uint32_t dwClockFrequencyInHz, uint32_t dwBaud,
                                     _Bool  bfSync, _Bool  bfABCS)
{
    uint8_t    byDivCode;
    uint32_t   dwMaxBaud, dwMinBaud;

    for (byDivCode = (uint8_t) 0; byDivCode < SCI_MAX_DIVIDER_CLOCK; byDivCode++)
    {
        /* Calculate the maximum baud possible with this setting */
        dwMaxBaud = sciCalculateBaud(dwClockFrequencyInHz,
                                     byDivCode,
                                     (uint8_t) 0,
                                     bfSync,
                                     bfABCS);

        /* Calculate the minimum baud possible with this setting */
        dwMinBaud = sciCalculateBaud(dwClockFrequencyInHz,
                                     byDivCode,
                                     (uint8_t) 255,
                                     bfSync,
                                     bfABCS);

        if ((dwMaxBaud > dwBaud) && (dwBaud > dwMinBaud))
        {
            return byDivCode;
        }
    }

    /* This is the top clock code */
    return (uint8_t) SCI_BAUD_OUT_OF_RANGE;
}
/******************************************************************************
End  of function sciCalculateDividerClock
******************************************************************************/

/******************************************************************************
Function Name: sciCalculateBRRSetting
Description:   Function to calculate the BRR value for a given peripheral clock
               frequency, baud rate and CKS divider clock setting
Arguments:     IN  dwClockFrequencyInHz - The peripheral clock frequency
               IN  dwBaud - The desired baud rate
               IN  byDivisor - The CKS divisor setting
               IN  bfSync - true for synchronous mode
               IN  bfABCS - The Asynchronous base clock select setting
Return value:  0 for success otherwise error code
******************************************************************************/
static uint8_t sciCalculateBRRSetting(uint32_t dwClockFrequencyInHz, uint32_t dwBaud,
                                   uint8_t  byDivisor, _Bool bfSync, _Bool bfABCS)
{
    uint16_t    wClockDivider;

    if (bfSync == true)
    {
        /* look up synchronous clock divider */
        wClockDivider = gcSCISynchMultiply[byDivisor];
    }
    else
    {
        /* look up asynchronous clock divider */
        wClockDivider = gcSCIAsynchMultiply[byDivisor];
        if (bfABCS)
        {
            wClockDivider >>= 1;
        }
    }

    /* calculate BRR value */
    return (uint8_t)((dwClockFrequencyInHz / (wClockDivider * dwBaud)) - 1);
}
/******************************************************************************
End  of function sciCalculateBRRSetting
******************************************************************************/

/******************************************************************************
Function Name: sciCalculateBaud
Description:   Function to calculate the BRR value for a given peripheral clock
               frequency, BRR and CKS divider clock setting
Arguments:     IN  dwClockFrequencyInHz - The peripheral clock frequency
               IN  byDivisor - The divisor setting to look up
               IN  byBRR - The BRR setting
               IN  bfSync - true for synchronous mode
               IN  bfABCS - The Asynchronous base clock select setting
Return value:  The actual baud rate
******************************************************************************/
static uint32_t sciCalculateBaud(uint32_t dwClockFrequencyInHz,
                              uint8_t  byDivisor,
                              uint8_t  byBRR,
                              _Bool  bfSync,
                              _Bool  bfABCS)
{
    uint16_t wClockDivider;

    if (bfSync == true)
    {
        /* look up synchronous clock divider */
        wClockDivider = gcSCISynchMultiply[byDivisor];
    }
    else
    {
        /* look up asynchronous clock divider */
        wClockDivider = gcSCIAsynchMultiply[byDivisor];
        if (bfABCS)
        {
            wClockDivider >>= 1;
        }
    }

    /* calculate baud rate */
    return (uint32_t) ((dwClockFrequencyInHz / ((uint32_t) byBRR + 1)) / (uint32_t) wClockDivider);
}
/******************************************************************************
End  of function sciCalculateBaud
******************************************************************************/
#endif
/******************************************************************************
End  Of File
******************************************************************************/
