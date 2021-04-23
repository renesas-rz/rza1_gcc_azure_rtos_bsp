/**************************************************************************/
/*                                                                        */
/*       Copyright (c) Microsoft Corporation. All rights reserved.        */
/*                                                                        */
/*       This software is licensed under the Microsoft Software License   */
/*       Terms for Microsoft Azure RTOS. Full text of the license can be  */
/*       found in the LICENSE file at https://aka.ms/AzureRTOS_EULA       */
/*       and in the root directory of this software.                      */
/*                                                                        */
/**************************************************************************/
#ifndef NX_DRIVER_RENESAS_RZ_H
#define NX_DRIVER_RENESAS_RZ_H

#include "nx_api.h"

#ifdef   __cplusplus
/* Yes, C++ compiler is present.  Use standard C.  */
extern   "C" {
#endif


/* by default no stats */
/*
#ifndef NX_STAT
#define NX_STAT                     1
#endif
*/

#define NUM_RX_DESC                 32       
#define NUM_TX_DESC                 8        /* Need more than 1 TX BD (HW requirement). */

#define RX_BUF_SIZE                 1536     /* Must be multiple of 32. */

#define TX_QUEUE_DEPTH              64       /* How many tx packets queued before dropping packets. */

#ifndef NX_ENET_MAX_MTU
#define NX_ENET_MAX_MTU             1516    /* Max Ethernet packet size (14 + 1500 + 2). */
#endif

/* NX_REC.nx_state values. */

#define NX_NOT_INITIALIZED          0x0000
#define NX_INITIALIZE_DONE          0x0002    /* Device is initialized. */
#define NX_LINK_ENABLED             0x0004    /* Device is enabled. */
#define NX_CANNOT_INITSTORAGE       0x0001
#define NX_CANNOT_INITHARDWARE      0x0002

/*** Macros. ***/
#define NX_BACKOFF_RELEASE(p)   \
{   \
    p -> nx_packet_prepend_ptr =  p -> nx_packet_prepend_ptr + NX_ETHERNET_SIZE;  \
    p -> nx_packet_length =  p -> nx_packet_length - NX_ETHERNET_SIZE;    \
    nx_packet_transmit_release(p); \
}
    
/* MAC Address - place lower 4 bytes in _L and upper 2 bytes in _H. */
#define ETHERNET_MAC_H              0x00000011
#define ETHERNET_MAC_L              0x22334455

/* Descriptor structure. */
typedef struct EMAC_BD_STRUCT
{
    /* RX/TX d0 */
    volatile unsigned long  bd_status;          /* Status - 32 bits. */
    
    /* RX/TX d1 */
#ifdef NX_LITTLE_ENDIAN                         /* Little endian. */
    volatile unsigned short bd_rxdatalength;    /* TX-Reserved, RX-Received data length */
    volatile unsigned short bd_bufsize;         /* RX-buffer size, TX-valid tx data length. */
#else                                           /* Big endian. */
    volatile unsigned short bd_bufsize;         /* RX-buffer size, TX-valid tx data length. */
    volatile unsigned short bd_rxdatalength;    /* TX-Reserved, RX-Received data length. */
#endif
    
    /* RX/TX d2 */
    unsigned char  *bd_buffer_ptr;  /* Buffer pointer */
    
    /* NX_PACKET pointer. */
    NX_PACKET      *bd_nx_packet;   /* Associate descriptor with nx_packet
                                     * (allows for chaining in RX and backoff release in TX interrupt). */
} EMAC_BD;


/* Bits in the status word of the tx/rx descriptors. */

#define BD_ACT                      0x80000000  /* TACT/RACT - Tx/Rx descriptor active. */
#define BD_DL                       0x40000000  /* TDLE/RDLE - Tx/Rx descriptor list last. */
#define BD_FP1                      0x20000000  /* TFP1/RFP1 - Tx/Rx Frame Position high bit. */
#define BD_FP0                      0x10000000  /* TFP0/RFP0 - Tx/Rx Frame Position low bit. */
#define BD_FE                       0x08000000  /* TFE/RFE - Tx/Rx Frame Error. */
#define BD_WBI                      0x04000000  /* Write-back completion interrupt (tx). */
#define BD_CSE                      0x04000000  /* Checksum value evaluation (rx). */

/* RFS - bit 26-0, bit 9, 7, 4-0 are used for error status */

#define RMAF                        0x00800000


/* EDSR register bits (write-only). */

#define EDSR_ENR                    0x00000001
#define EDSR_ENT                    0x00000002

/* EDMR register bits. */

#define EDMR_SWRR                   0x00000001
#define EDMR_SWRT                   0x00000002
#define EDMR_DE						0x00000040

/* Defines for the EESR bit values, same bit values can be used for EESIPR. */

#define EESR_TWB1                   0x80000000  /* Write back complete bit 1. */
#define EESR_TWB0                   0x40000000  /* Write back complete bit 0. */
#define EESR_TC1                    0x20000000  /* Tx complete bit 1. */
#define EESR_TUC                    0x10000000  /* Tx underflow frame write-back complete. */
#define EESR_ROC                    0x08000000  /* Rx overflow frame write-back complete. */
#define EESR_TABT                   0x04000000  /* Transmit Abort. */
#define EESR_RABT                   0x02000000  /* Receive Abort. */
#define EESR_RFCOF                  0x01000000  /* Receive frame counter overflow. */
                /*                  0x00800000     Reserved (bit 23). */
#define EESR_ECI                    0x00400000  /* EMAC Status Register interrupt source. */
#define EESR_TC0                    0x00200000  /* Tx complete bit 0. */
#define EESR_TDE                    0x00100000  /* Tx descriptor empty. */
#define EESR_TFUF                   0x00080000  /* Tx FIFO underflow. */
#define EESR_FR                     0x00040000  /* Frame received. */
#define EESR_RDE                    0x00020000  /* Rx descriptor empty. */
#define EESR_RFOF                   0x00010000  /* Rx FIFO overflow. */
                /*                  0x0000FF00     Reserved (bits 15-8). */
#define EESR_RMAF                   0x00000080  /* Receive multicast addresses frame. */
                /*                  0x00000060     Reserved (bits 6-5). */
#define EESR_RRF                    0x00000010  /* Rx residual bit frame. */
#define EESR_RTLF                   0x00000008  /* Rx too-long frame. */
#define EESR_RTSF                   0x00000004  /* Rx too-short frame. */
#define EESR_PRE                    0x00000002  /* PHY-LSI rx error. */
#define EESR_CERF                   0x00000001  /* CRC error on rx frame. */

#define EESR_TWB                    (EESR_TWB0 | EESR_TWB1)
#define EESR_TC                     (EESR_TC0 | EESR_TC1)

/* EESIPR error in packets */
//#define EESIPR_PKTERRORS (EESR_RRF | EESR_RTLF | EESR_RTSF | EESR_CERF)
//#define EESIPR_BUFERRORS (EESR_RFCOF | EESR_TDE | EESR_TFUF | EESR_RDE | EESR_RFOF)   
//#define EESIPR_COMERRORS (EESR_DLC | EESR_CD | EESR_TRO | EESR_PRE)
//#define EESIPR_ALLERRORS (EESIPR_PKTERRORS | EESIPR_BUFERRORS | EESIPR_COMERRORS)

/* current value of EESIPR. */
//#define EESIPR_CURSETTING   0x1F7F07FF          /* enable all - 0s are reserved */

/* ECSR Bits. */

#define ECSR_ICD                    0x00000001  /* Illegal carrier detect. */
#define ECSR_PFROI                  0x00000010  /* Pause frame retransmit retry over. */

#ifdef NX_STAT
/******************************************************************/
/************* Ethernet statistic collection data ****************/
struct enet_statistics {
    int rx_packets;             /* total packets received       */
    int tx_packets;             /* total packets transmitted    */
    int multicast;              /* multicast packets received   */
    int rx_errors;              /* bad packets received         */
    int tx_errors;              /* packet transmit problems     */
    int rx_dscpt_out;           /* Rx Descriptor Exhausted      */
    int tx_dscpt_out;           /* Tx Descriptor Exhausted      */
    int illegal_carrier_errors; /* Illegal carrier detection */
    int phy_lsi_errors;         /* PHY-LSI Receive Error */
    
    /* detailed rx_errors: */
    int rx_length_errors;       /* either of following errors */
                                /* Receive Too-Long Frame */
                                /* Receive too-short Frame */
    int rx_abort;               /* Receive abort */
    int rx_frame_cnt_of_errors; /* receive frame counter overflow */
    int rx_crc_errors;          /* received pkt with crc error    */
    int rx_frame_errors;        /* Receive Residual bit frame */
    int rx_fifo_of_errors;      /* recv'r fifo overflow         */

    /* detailed tx_errors */
    int tx_abort;               /* Transmit abort */
    int tx_fifo_uf_errors;      /* Tx FIFO Underflow */

    /* NX side error - NX_PACKET allocation error */
    int nx_packet_alloc_errors;
};
#endif  

/* Driver record structure - keep related info in one place */
typedef struct NX_REC_STRUCT
{
    UINT        nx_state ;   /* state of this driver */
    NX_IP       *ip_ptr;  /* NetX IP structure handling this chip */
    EMAC_BD     *driver_rx_bd;
    EMAC_BD     *driver_tx_bd;

#ifdef NX_STAT
    struct enet_statistics stat;    /* The  statistics table. */
#endif 
}   NX_REC;

/*************** External function prototypes ************************/
extern void _nx_ether_interrupt(uint32_t int_sense);
extern VOID nx_driver_renesas_rz(NX_IP_DRIVER *driver_req_ptr) ;


/* Determine if a C++ compiler is being used.  If so, complete the
 * standard C conditional started above.  */
#ifdef   __cplusplus
        }
#endif

#endif      
