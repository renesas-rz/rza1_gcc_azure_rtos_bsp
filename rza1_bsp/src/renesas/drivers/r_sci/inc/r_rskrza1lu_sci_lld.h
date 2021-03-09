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
 * @headerfile     r_rskrza1lu_sci_lld.h
 * @brief          Low Level Driver for SCIF
 * @version        1.00
 * @date           27.06.2018
 * H/W Platform    RZ/A1LU
 *****************************************************************************/
 /*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 30.06.2018 1.00 First Release
 *****************************************************************************/

/* Multiple inclusion prevention macro */
#ifndef R_SW_PKG_93_SCI_LLD_API_H_INCLUDED
#define R_SW_PKG_93_SCI_LLD_API_H_INCLUDED

/**************************************************************************//**
 * @ingroup R_SW_PKG_93_SCIF_API
 * @defgroup R_SW_PKG_93_SCIF_LLD_API SCIF RZA1LU Low Level Driver
 * @brief The API for the low level SCIF driver
 *
 * @anchor R_SW_PKG_93_SCIF_LLD_API_SUMMARY
 * @par Summary
 * @brief This interface defines the functionality that the low level driver
 * must implement for compatibility with the high level driver.
 *
 * This is a device and board-specific implementation of an SCI driver,
 * configured for the RZA1LU Stream-IT board
 *
 * @anchor R_SW_PKG_93_SCIF_LLD_API_LIMITATIONS
 * @par Known Limitations
 * None
 *
 * @anchor R_SW_PKG_93_SCIF_LLD_API_INSTANCES
 * @par Known Implementations:
 * This driver is used in the RZA1LU Software Package.
 * @see RENESAS_APPLICATION_SOFTWARE_PACKAGE
 *
 * @see RENESAS_OS_ABSTRACTION  Renesas OS Abstraction interface
 * @{
 *****************************************************************************/

/*****************************************************************************
 Includes   <System Includes> , "Project Includes"
 *****************************************************************************/

#include "r_typedefs.h"
#include "iodefine_cfg.h"
#include "control.h"
#include "r_intc.h"
#include "r_cbuffer.h"
#include "r_os_abstraction_api.h"

/*******************************************************************************
 Macro definitions
 *******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/

/**
 * @typedef st_scif Define a type that points to the SCIF
 */
typedef volatile struct st_scif *PSCIF;

/**
 * @typedef PSETIRQ Define a pointer to a function to set the SCIF interrupt priority
 */
typedef void (*PSETIRQ)(_Bool bfEnable);


/**
 * @typedef _SCIFCFG Define a structure to contain the SCIF configuration data
 */
typedef struct _SCIFCFG
{
    uint32_t   dwBaud;                      /*!< The desired baud rate */
    uint32_t   dwConfig;                    /*!< The SCIF line coding configuration*/
    uint32_t   dwPeripheralClockFrequency;  /*!< The frequency of the peripheral
                                               clock */
    PSETIRQ pSetIRQ;                        /*!< Pointer to a function to set the
                                               interrupt priority of the SCIF */
} SCIFCFG,
*PSCIFCFG;

/* Define the structure of data required by the driver */
/**
 * @typedef _DDSCIF Define the structure of data required by the driver
 */
typedef struct _DDSCIF
{
    e_sc_id_t   smart_config_id;            /*!< Smart Configuration ID */
    uint32_t    res;                        /*!< result of searching for smart_config_id in the r_drv_sci_cfg_ch_config_t table of supported drivers */
    PSCIF       pPORT;                      /*!< Pointer to the hardware */
    SCIFCFG     sciConfig;                  /*!< The SCI configuration */
    uint32_t    dwActualBaud;               /*!< The actual baud rate set */
    event_t     evReceive;                  /*!< Set when data received */
    PCBUFF      pRxBuffer;                  /*!< Pointer to the receive software FIFO */
    PCBUFF      pTxBuffer;                  /*!< Pointer to the transmit software FIFO */
    SCIERR      errorCode;                  /*!< The last error code */
    uint32_t    intc_id_bri;                /*!< INTC_ID_BRI Interrupt handler */
    uint32_t    intc_id_eri;                /*!< INTC_ID_ERI Interrupt handler */
    uint32_t    intc_id_rxi;                /*!< INTC_ID_RXI Interrupt handler */
    uint32_t    intc_id_txi;                /*!< INTC_ID_TXI Interrupt handler */
    uint32_t    scif_scscr_tie;             /*!< SCIF_SCSCR_TIE Interrupt Enable */
} DDSCIF,
*PDDSCIF;

/******************************************************************************
Public Functions
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 Interface
 ******************************************************************************/

/** 
 *  @brief Low layer driver initialise interface function
 *  @retval None.
*/
int_t   R_SCI_InitialiseHwIf(e_sc_id_t id);

 /** 
  *  @brief Low layer driver disable the hardware interface function
  *  @retval None.
  */
void    R_SCI_UninitialiseHwIf(e_sc_id_t id);

/**
 * @brief   Function to configure the SCIF
 * 
 * @param[in/out] pDDSCIF:  Pointer to the driver data
 * @param[in]     pSCIF: Pointer to the base address of the SCIF
 * @param[in]     pConfiguration: Pointer to the configuration data
 * @param[in]     pRxBuffer: Pointer to the Receive circular buffer
 * @param[in]     pTxBuffer: Pointer to the Transmit circular buffer
 *  
 * @retval        0: Success.
 * @retval        Otherwise: Error Code.  
 */
extern  SCIERR sciOpen(PDDSCIF pDDSCIF, PSCIF pSCIF, PSCIFCFG pConfiguration, PCBUFF pRxBuffer, PCBUFF pTxBuffer);

/**
 * @brief Function to stop the SCIF
 * 
 * @param[in] pDDSCIF: Pointer to the driver data
 * 
 * @return None. 
 */
extern  void sciClose(PDDSCIF pDDSCIF);

/**
 * @brief Function to re-configure the line-coding
 * 
 * @param[in] pDDSCIF:  Pointer to the driver data
 * @param[in] dwBaud:   The desired baud rate
 * @param[in] dwConfig: The desired configuration 
 * 
 * @retval 0: Success 
 */
extern  int sciReConfigure(PDDSCIF pDDSCIF, uint32_t dwBaud, uint32_t dwConfig);

/**
 * @brief Function to start reading data from the SCIF
 * 
 * @param[in] pDDSCIF:  Pointer to the driver data
 * @param[in] pbyDest:  Pointer to the destination memory
 * @param[in] stLength: The length of data to read
 * 
 * @retval 0: Success 
 */
extern  SCIERR sciReadData(PDDSCIF pDDSCIF, uint8_t *pbyDest, size_t stLength);

/**
 * @brief Function to start reading data from the SCIF for a period of time
 * 
 * @param[in] pDDSCIF: Pointer to the driver data
 * @param[in] pbyDest: Pointer to the destination memory
 * @param[in] stLength: The length of the data buffer
 * 
 * @retval Num_Bytes: Number of bytes read
 */
extern  int sciReadDataTimed(PDDSCIF pDDSCIF, uint8_t *pbyDest, size_t stLength);

/**
 * @brief Function to start writing data to the SCIF
 * 
 * @param[in] pDDSCIF: Pointer to the driver data
 * @param[in] pbySrc: Pointer to the source  memory 
 * @param[in] stLength: The length of data to write
 * 
 * @return None.  
 */
extern  void sciWriteData(PDDSCIF pDDSCIF, uint8_t *pbySrc, size_t stLength);


/**
 * @brief Function to wait for the TX software FIFO to become empty
 * 
 * @param[in] pDDSCIF:  Pointer to the driver data
 * 
 * @return None. 
 */
extern  void sciWaitTx(PDDSCIF pDDSCIF);

/******************************************************************************
The interrupt service routines
******************************************************************************/

/**
 * @brief ISR - The break error interrupt handler
 * 
 * @param[in] pDDSCIF: Pointer to the driver data
 * 
 * @return None. 
 */
extern  void sciINTBreak(PDDSCIF pDDSCIF);

/**
 * @brief ISR - The break error interrupt handler
 * 
 * @param[in] pDDSCIF: Pointer to the driver data 
 * 
 * @return None. 
 */
extern  void sciINTError(PDDSCIF pDDSCIF);

/**
 * @brief ISR - Receive data interrupt service routine handler 
 * 
 * @param[in] pDDSCIF: Pointer to the driver data 
 * 
 * @return None. 
 */
extern  void sciINTRxi(PDDSCIF pDDSCIF);

/**
 * @brief Receive data interrupt service routine handler
 * 
 * @param[in] pDDSCIF: Pointer to the driver data
 * 
 * @return None.  
 */
extern  void sciINTTxi(PDDSCIF pDDSCIF);

#endif  /* R_SW_PKG_93_SCI_LLD_API_H_INCLUDED */
/**************************************************************************//**
 * @} (end addtogroup)
 *****************************************************************************/
/******************************************************************************
End  Of File
******************************************************************************/
