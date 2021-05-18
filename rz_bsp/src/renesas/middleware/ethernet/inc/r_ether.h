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
 * @headerfile     r_ether.h
 * @brief          ETHER driver header
 * @version        1.00
 * @date           27.06.2018
 * H/W Platform    RZA1H
 *****************************************************************************/
 /*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 30.06.2018 1.00 First Release
 *****************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef _R_ETHER_H_
#define _R_ETHER_H_

/**************************************************************************//**
 * @ingroup R_SW_PKG_93_ETHER_API
 * @defgroup R_SW_PKG_93_ETHER_DRV Ethernet Driver Interface
 * @brief Ethernet Driver Interface 
 *
 * @anchor R_SW_PKG_93_ETHERNET_API_SUMMARY
 * @par Summary
 *
 * This middleware module contains the API for the ethernet
 * driver. 
 * 
 * @anchor R_SW_PKG_93_ETHERNET_API_INSTANCES
 * @par Known Implementations:
 * This driver is used in the RZA1H Software Package.
 * @see RENESAS_APPLICATION_SOFTWARE_PACKAGE
 *
 * @see RENESAS_OS_ABSTRACTION  Renesas OS Abstraction interface
 * @{
 *****************************************************************************/
/******************************************************************************
Includes <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"

/******************************************************************************
Macro definitions
******************************************************************************/
/** Allocate the memory for the driver instead of the hard-coded allocation below */
#define _ALLOC_MEM_

/** This is very nasty. Get the addresses from the linker or allocate dynamically */
#define ADDR_OF_TXRX_DESC       0x60500000
#define ADDR_OF_TXRX_BUFF       0x60501000

#define NUM_OF_TX_DESCRIPTOR    (8)
#define NUM_OF_RX_DESCRIPTOR    (8)
#define NUM_OF_TX_BUFFER        NUM_OF_TX_DESCRIPTOR
#define NUM_OF_RX_BUFFER        NUM_OF_RX_DESCRIPTOR
#define SIZE_OF_BUFFER          (1600)    /* Must be an integral multiple of 32 */

#define R_ETHER_OK              (0)
#define R_ETHER_ERROR           (-1)
#define R_ETHER_TIMEOUT         (-2)
#define R_ETHER_HARD_ERROR      (-3)
#define R_ETHER_RECOVERAVLE     (-4)
#define R_ETHER_NODATA          (-5)
#define MIN_FRAME_SIZE          (60)
#define MAX_FRAME_SIZE          (1514)

#define LOOP_100us              (6700)    /*!< Loop counter for software wait 
                                               6666=100us/((1/400MHz)*6cyc)         */

#define EDMAC_EESIPR_INI_SEND   (0x24280700)    /*!< 0x04000000 : Detect transmit suspended <BR>
                                                     0x20200000 : Frame transmission completed <BR>
                                                     0x00080000 : Transmit FIFO underflow <BR>
                                                     0x00000400 : Carrier lost detected<BR>
                                                     0x00000200 : Delayed collision detected<BR>
                                                     0x00000100 : Transmit retry-over condition */
#define EDMAC_EESIPR_INI_RECV   (0x0205001F)    /*!< 0x02000000 : Detect reception suspended <BR>
                                                     0x00040000 : Detect frame reception <BR>
                                                     0x00010000 : Receive FIFO overflow <BR>
                                                     0x00000010 : Residual bit frame reception <BR>
                                                     0x00000008 : Long frame reception <BR>
                                                     0x00000004 : Short frame reception <BR>
                                                     0x00000002 : PHY-LSI reception error <BR>
                                                     0x00000001 : Receive frame CRC error */
#define EDMAC_EESIPR_INI_EtherC (0x00400000)    /*!< 0x00400000 : E-MAC status register */
#define EtherC_ECSIPR_INI       (0x00000004)    /*!< 0x00000004 : Link signal change */

/******************************************************************************
Typedef definitions
******************************************************************************/
/* ==== Transmit descriptor ==== */
typedef union
{
    uint32_t LONG;
    struct{
        uint32_t reserved2 :8;      /*!< Reserved */
        uint32_t TFS8:1;            /*!< Transmit abort detect (TABT bit in EESR) */
        uint32_t TFS9:1;            /*!< Transmit FIFO underflow (TCU bit in EESR) */
        uint32_t reserved1 :16;     /*!< Reserved */
        uint32_t TWBI :1;           /*!< Write-back completion interrupt notification */
        uint32_t TFE :1;            /*!< Transmit frame error */
        uint32_t TFP :2;            /*!< Location 1, 0 within transmit frame */
        uint32_t TDLE:1;            /*!< End of transmit descriptor */
        uint32_t TACT:1;            /*!< Transmit descriptor enabled */
    }BIT;
} td0_t;

typedef struct
{
#if defined(_BIG)
    uint16_t    TDL;                /*!< Transmit buffer data length (Big endian) */
    uint16_t    reserved;
#else
    uint16_t    reserved;
    uint16_t    TDL;                /*!< Transmit buffer data length (Little endian) */
#endif
} td1_t;

typedef struct
{
    uint8_t   * TBA;                /*!< Address of transmit buffer */
} td2_t;

typedef struct tag_edmac_send_desc
{
    td0_t    td0;
    td1_t    td1;
    td2_t    td2;
    struct tag_edmac_send_desc * pNext;
} edmac_send_desc_t;

/* ==== Receive descriptor ==== */
typedef union
{
    uint32_t LONG;
    struct{
        uint32_t RCS:16;        /*!< Receive Check Sum */
        uint32_t RFS0:1;        /*!< Receive frame CRC error detected (CERF bit in EESR) */
        uint32_t RFS1:1;        /*!< PHY-LSI receive error (PRE bit in EESR) */
        uint32_t RFS2:1;        /*!< Short frame receive error (RTSF bit in EESR) */
        uint32_t RFS3:1;        /*!< Long frame receive error (RTLE bit in EESR) */
        uint32_t RFS4:1;        /*!< Residual bits frame receive error (RRF bit in EESR) */
        uint32_t RFS5:1;        /*!< Carrier extension loss (CELF bit in EESR) */
        uint32_t RFS6:1;        /*!< Carrier extension error (CEEF bit in EESR) */
        uint32_t RFS7:1;        /*!< Receive multicast frames (RMAF bit in EESR) */
        uint32_t RFS8:1;        /*!< Receive abort detect (RABT bit in EESR) */
        uint32_t RFS9:1;        /*!< Receive FIFO overflow (RFOF bit in EESR) */
        uint32_t RCSE:1;        /*!< Padding insertion */
        uint32_t RFE :1;        /*!< Receive Check Sum */
        uint32_t RFP :2;        /*!< Location 1,0 within receive frame */
        uint32_t RDLE:1;        /*!< End of receive descriptor */
        uint32_t RACT:1;        /*!< Receive descriptor enabled */
    }BIT;
} rd0_t;

typedef struct
{
#if defined(_BIG)
    uint16_t    RBL;                /*!< Receive buffer length (Big endian) */
    uint16_t    RDL;                /*!< Receive data length (Big endian) */
#else
    uint16_t    RDL;                /*!< Receive data length (Little endian) */
    uint16_t    RBL;                /*!< Receive buffer length (Little endian) */
#endif
} rd1_t;

typedef struct
{
    uint8_t *RBA;                   /*!< Receive buffer address */
} rd2_t;

typedef struct tag_edmac_recv_desc
{
    rd0_t    rd0;
    rd1_t    rd1;
    rd2_t    rd2;
    struct tag_edmac_recv_desc * pNext;
} edmac_recv_desc_t;

/** @brief The whole transmit/receive descriptors (must be allocated in 16-byte boundaries) */
typedef struct
{
    edmac_send_desc_t dsend[NUM_OF_TX_DESCRIPTOR];
    edmac_recv_desc_t drecv[NUM_OF_RX_DESCRIPTOR];
    edmac_send_desc_t * pSend_top;  /*!<  Registration location of transmit descriptors */
    edmac_recv_desc_t * pRecv_end;  /*!<  Registration location and reception end of transmit descriptors */
} txrx_descriptor_set_t;
typedef txrx_descriptor_set_t * txrx_descriptor_set_t_ptr;

/** @brief Transmit/receive buffers (must be allocated in 32-byte boundaries) 
 *         Definition of all transmit/receive buffer areas */
typedef struct
{
    uint8_t bsend[NUM_OF_TX_BUFFER][SIZE_OF_BUFFER];
    uint8_t brecv[NUM_OF_RX_BUFFER][SIZE_OF_BUFFER];
} txrx_buffer_set_t; 
typedef txrx_buffer_set_t * txrx_buffer_set_t_ptr;

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/

/**         
 * @brief        Initialises the EtherC, E-DMAC, PHY, and buffer memory.
 *
 * @param[in]    ch:            Ethernet channel number
 * @param[in]    mac_addr:      Pointer to MAC address array
 *
 * @retval       R_ETHER_OK(0): Success                
 * @retval       R_ETHER_ERROR(-1): Error                
*/
int32_t R_Ether_Open(uint32_t ch, uint8_t mac_addr[]);

/**           
 * @brief        Stops the E-MAC/E-DMAC.
 * 
 * @param[in]    ch: Ethernet channel number
 * 
 * @retval       R_ETHER_OK(0):     Success                
 * @retval       R_ETHER_ERROR(-1): Error                
*/
int32_t R_Ether_Close(uint32_t ch);

/**            
 * Description   Copies the received Ethernet frame on the Ethernet channel
 *               specified by channel number to the receive buffer.
 * 
 * @param[in]    ch:  Ethernet channel number
 * @param[in]    buf: Pointer to Ethernet receive buffer
 * 
 * @retval       Greater than 0   : Success. Returns number of bytes received                
 * @retval       R_ETHER_ERROR(-1): Error
 * @retval       R_ETHER_HARD_ERROR(-3): Hardware error. 
 *                                       Software reset is necessary to recover
 * @retval       R_ETHER_RECOVERABLE(-4): Recoverable error
 * @retval       R_ETHER_NODATA(-5): No data received
*/
int32_t R_Ether_Read(uint32_t ch, void *buf);

/**
 * Description   This function sends an Ethernet frame pointed by Ethernet frame
 *               pointer on the Ethernet channel specified by channel number. 
 *               It updates the status of the transmit descriptor as new Ethernet
 *               frame is processed. It doesn't check whether Ethernet frame
 *               transmission completed or not.
 * 
 * @param[in]    ch:  Ethernet channel number
 * @param[in]    buf: Pointer to Ethernet transmit buffer
 * @param[in]    len: Ethernet frame length (unit:byte)
 * 
 * @retval       R_ETHER_OK(0):     Success              
 * @retval       R_ETHER_ERROR(-1): Error
*/ 
int32_t R_Ether_Write(uint32_t ch, void *buf, uint32_t len);

void INT_Ether(uint32_t status);

/**               
 * @breif        Interrupt handler related to transmission 
 *               regarding E-DMAC(EESR)
 * 
 * @param[in]    status: Interrupt Status
 * 
 * @return       None.
*/
void lan_send_handler_isr(uint32_t status);

/**             
 * Description   Interrupt handler related to reception regarding E-DMAC (EESR)
 * 
 * @param[in]    status: Interrupt Status
 * 
 * @return       None.
*/
void lan_recv_handler_isr(uint32_t status);

/**
 * @brief        E-MAC interrupt function
 * 
 * @param[in]    status: Interrupt Status
 * 
 * @return       None.
*/
void lan_etherc_handler_isr(uint32_t status);

/**
 * @brief        PHY Link Status Check function
 * 
 * @retval       link_status: The link status from the PHY
*/
int32_t lan_link_check(void);

/**
 * @brief        Function to change the MAC address filtering mode
 * 
 * @param[in]    bf_promiscuous - true to receive all messages
 *
 * @return       None.
*/
void lan_promiscuous_mode(_Bool bf_promiscuous);

/**
 * @brief        Function to set the MAC address
 * 
 * @param[in]    bf_promiscuous: True to receive all messages
 * 
 * @return       None.
*/
void lan_mac_set(uint8_t mac_addr[]);

/**
 * @brief        Function to set the RX ISR call-back function
 * 
 * @param[in]    pfn_rx_call_back: Pointer to the received call-back
 * @param[in]    gpv_rx_parameter: The parameter to pass to the call-back
 * 
 * @return       None.
*/
void lan_set_rx_call_back(void (*pfn_rx_call_back)(void *),
                          void *pv_rx_parameter);

/**
 * @brief        Function to set the TX ISR call-back function
 * 
 * @param[in]    pfn_tx_call_back: Pointer to the transmit call-back
 * @param[in]    gpv_tx_parameter: The parameter to pass to the call-back
 * 
 * @return       None.
*/                          
void lan_set_tx_call_back(void (*pfn_tx_call_back)(void *),
                          void *pv_tx_parameter);

#endif /* _R_ETHER_H_ */
/**************************************************************************//**
 * @} (end addtogroup)
 *****************************************************************************/
/* End of File */
