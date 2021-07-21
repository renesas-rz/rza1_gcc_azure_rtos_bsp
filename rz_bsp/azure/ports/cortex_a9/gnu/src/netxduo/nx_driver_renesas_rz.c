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


/* Bring in necessary include files.  */

#include "tx_api.h"
#include "nx_api.h"
#include "nx_driver_renesas_rz.h"

#include "compiler_settings.h"

#include "iodefine.h"
#include "r_intc.h"            /* INTC Driver Header */
#include "r_port_if.h"
#include "r_gpio_if.h"
#include "phy.h"
#include "gpio_iodefine.h"
#include "ether_iodefine.h"

#include "gpio_iobitmask.h"

#include "mcu_board_select.h"
#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
/* nothing */
#elif (TARGET_BOARD == TARGET_BOARD_RSK)
#include "riic_cat9554_if.h"
#endif /* TARGET_BOARD */

/* Define Ethernet address format.  This is prepended to the incoming IP
   and ARP/RARP messages.  The frame beginning is 14 bytes, but for speed 
   purposes, we are going to assume there are 16 bytes free in front of 
   the prepend pointer and that the prepend pointer is 32-bit aligned.  

    Byte Offset     Size            Meaning

        0           6           Destination Ethernet Address
        6           6           Source Ethernet Address
        12          2           Ethernet Frame Type, where:
                                    
                                        0x0800 -> IP Datagram
                                        0x0806 -> ARP Request/Reply
                                        0x0835 -> RARP request reply
        14          2           Padding inserted by hardware
        42          18          Padding on ARP and RARP messages only.  */

#define NX_ETHERNET_IP      0x0800
#define NX_ETHERNET_ARP     0x0806
#define NX_ETHERNET_RARP    0x8035
#define NX_ETHERNET_IPV6    0x86DD
#define NX_ETHERNET_SIZE    14
#define NX_PADDING_SIZE     2

/*************** Global Variables ***************/

static NX_REC nx_record = {0,NULL,NULL,NULL};

/* The BDs must be 16-byte aligned. Don't cache BDs. */
char NO_CACHE_SECTION rx_bd_space[sizeof(EMAC_BD) * (NUM_RX_DESC+1)] __attribute__ ((aligned (16)));
char NO_CACHE_SECTION tx_bd_space[sizeof(EMAC_BD) * (NUM_TX_DESC+1)] __attribute__ ((aligned (16)));

/* These rx/tx BDs will point to the aligned BD arrays. */
static EMAC_BD *rx_bd;
static EMAC_BD *tx_bd;

/* Indexes for the BD arrays. */
static UINT current_txbd_index = 0;
static UINT current_rxbd_index = 0;

volatile static ULONG tx_in_progress = 0;

/* Transmit packet queue. */
static NX_PACKET *driver_tx_packet_queue = NULL;
static NX_PACKET *driver_tx_packet_queue_end = NULL;
volatile static ULONG tx_queued_count = 0;

/* Define receive pool.  */
#define NX_PACKET_PAYLOAD_SIZE  1536
/* Make the pool ~32 more packets larger than the number of descriptors, to ensure
 * there are enough packets. 
 */
#define NX_RECEIVE_PACKET_POOL_SIZE     ((NX_PACKET_PAYLOAD_SIZE + sizeof(NX_PACKET)) * (NUM_RX_DESC+32))
/* Add 32 to allow 32-byte alignment of rx buffers. */
/* Divide because this is a ULONG (not byte) array. */
/* Don't cache packet pool! */
ULONG	NO_CACHE_SECTION	rx_packet_pool_area[NX_RECEIVE_PACKET_POOL_SIZE/sizeof(ULONG) + 32/sizeof(ULONG)] __attribute__ ((aligned (16)));
NX_PACKET_POOL    rx_pool;

/*************** Function prototypes ***************/

static unsigned int nx_ether_write(NX_PACKET *pkt);
static ULONG        nx_ether_init(NX_REC *driver_rec);
static unsigned int nx_storage_init(NX_IP *ip_ptr);
static void         nx_multicast_join(void);
static void         nx_error_interrupt(ULONG errbits);
static void         nx_rx_interrupt(void);
static void         nx_tx_interrupt(void);
static void         nx_receive(NX_IP *ip_ptr, NX_PACKET *packet_ptr);
static unsigned int nx_ether_send(NX_PACKET *packet);

#ifdef NX_STAT
static void         nx_stat_init(struct enet_statistics *p);
#endif


/**************************************************************************/ 
/*                                                                        */ 
/*  FUNCTION                                               RELEASE        */ 
/*                                                                        */ 
/*    nx_driver_renesas_rz                                PORTABLE C      */ 
/*                                                           5.0          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Scott Larson, Microsoft Corporation                                 */
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This is the entry point of the zero-copy Ethernet driver for the    */ 
/*    Renesas RZ processor.                                               */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    driver_req_ptr        - Pointer to driver request structure         */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    None                                                                */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    nx_stat_init                                                        */ 
/*    nx_storage_init                                                     */ 
/*    nx_ether_init                                                       */ 
/*    nx_packet_transmit_release                                          */ 
/*    NX_BACKOFF_RELEASE                                                  */ 
/*    nx_ether_send                                                       */ 
/*    nx_multicast_join                                                   */ 
/*                                                                        */ 
/*  CALLED BY                                                             */ 
/*                                                                        */ 
/*    IP thread.                                                          */ 
/*                                                                        */ 
/*  RELEASE HISTORY                                                       */ 
/*                                                                        */ 
/*    DATE              NAME                      DESCRIPTION             */ 
/*                                                                        */ 
/*  01-01-2014       Scott Larson           Initial release               */ 
/*                                                                        */ 
/**************************************************************************/ 
VOID nx_driver_renesas_rz(NX_IP_DRIVER *driver_req_ptr)
{

UINT            status;
NX_PACKET       *packet_ptr;
ULONG           *ethernet_frame_ptr;
NX_IP           *ip_ptr;
UINT            driver_command;
    
    
    /* Setup the IP pointer from the driver request. */
    ip_ptr = driver_req_ptr -> nx_ip_driver_ptr;

    /* Default to successful return. */
    driver_req_ptr -> nx_ip_driver_status = NX_SUCCESS;

    /* Extract driver command. */
    driver_command = driver_req_ptr -> nx_ip_driver_command;
    
    /* Process the driver request. */
    switch (driver_command)
    {
        case NX_LINK_INTERFACE_ATTACH:
        /* This command is used in multi homed devices - return NX_SUCCESS by default. */
            break;
        
        case NX_LINK_INITIALIZE:
        {   /* Process driver initialization. */
            if (nx_record.nx_state == NX_NOT_INITIALIZED)
            {
                #ifdef NX_STAT
                /* Initialize statistics */
                nx_stat_init(&nx_record.stat);
                #endif
                
                /*  Initialize BDs. */
                status = nx_storage_init(ip_ptr);
                
                if (status != NX_SUCCESS)
                {
                    nx_record.nx_state = NX_CANNOT_INITSTORAGE;
                    driver_req_ptr -> nx_ip_driver_status =  NX_NOT_SUCCESSFUL;
                    break;
                }
                
                /* Initialize the ETHER/E-DMAC hardware. */
                status = nx_ether_init(&nx_record);
                
                if (status != NX_SUCCESS)
                {
                    nx_record.nx_state = NX_CANNOT_INITHARDWARE;
                    driver_req_ptr -> nx_ip_driver_status =  NX_NOT_SUCCESSFUL;
                    break;
                }
                
                /* Setup the link maximum IP layer transfer unit.  */
                ip_ptr -> nx_ip_driver_mtu = NX_ENET_MAX_MTU - NX_ETHERNET_SIZE - NX_PADDING_SIZE;
                
                /* Setup the physical address of this IP instance. */
                ip_ptr -> nx_ip_arp_physical_address_msw = ETHERNET_MAC_H; 
                ip_ptr -> nx_ip_arp_physical_address_lsw =  ETHERNET_MAC_L;
                ip_ptr -> nx_ip_driver_mapping_needed =  NX_TRUE;
                ip_ptr -> nx_ip_driver_link_up =  NX_FALSE;
                
                nx_record.ip_ptr = driver_req_ptr -> nx_ip_driver_ptr;
                nx_record.nx_state = nx_record.nx_state  | NX_INITIALIZE_DONE;
                break;
            }
            else 
               break; /* Accidental reinitialization: don't do anything. */
        }
    
        case NX_LINK_ENABLE:
        {
             /* See if we can honor the NX_LINK_ENABLE request. */
            if (!(nx_record.nx_state & NX_INITIALIZE_DONE))
            {
                driver_req_ptr -> nx_ip_driver_status =  NX_NOT_SUCCESSFUL;
                break;
            }            
            if (nx_record.nx_state & NX_LINK_ENABLED)
            {
                driver_req_ptr -> nx_ip_driver_status =  NX_ALREADY_ENABLED;
            }
            else
            {
                nx_record.nx_state = nx_record.nx_state | NX_LINK_ENABLED ;
                driver_req_ptr -> nx_ip_driver_status =  NX_SUCCESS;
            }
            
            /* Set link_up, configure Rx, Tx, and error interrupts. */
            ip_ptr -> nx_ip_driver_link_up =  NX_TRUE;
            
            ETHER.EESIPR0 |= 0x20640000;	/* SET TC0IP, TC1IP, FRIP, ECIIP */
            ETHER.ECSIPR0 |= 0x1;			/* Set illegal carrier detect */
            break;
        
        }

        case NX_LINK_DISABLE:
        {
            /* Make sure we are in the right state for NX_LINK_DISABLE. */
            if ( !(nx_record.nx_state & NX_LINK_ENABLED))
            {
                driver_req_ptr -> nx_ip_driver_status =  NX_NOT_ENABLED;
                break;
            }                
            /* Disable all ether interrupts. */
            ETHER.EESIPR0 = 0;
            ETHER.ECSIPR0 = 0;
            
            /* Clear link enabled flag. */
            nx_record.nx_state =  nx_record.nx_state & ~(NX_LINK_ENABLED);
            
            /* Clear the link up flag. */
            ip_ptr -> nx_ip_driver_link_up =  NX_FALSE;
            
            break;
        } /* End case NX_LINK_DISABLE. */

        case NX_LINK_PACKET_SEND:
        case NX_LINK_PACKET_BROADCAST:
        case NX_LINK_ARP_SEND:
        case NX_LINK_ARP_RESPONSE_SEND:
        case NX_LINK_RARP_SEND:
        {
            /* Check if link is enabled. */
            if ( !(nx_record.nx_state & NX_LINK_ENABLED))
            {
                driver_req_ptr -> nx_ip_driver_status =  NX_NOT_ENABLED;
                /* Free the packet that we will not send. */
                nx_packet_transmit_release(driver_req_ptr -> nx_ip_driver_packet);
                break;
            }                

            /* Process driver send packet. */
            /* Place the ethernet frame at the front of the packet. */
            packet_ptr =  driver_req_ptr -> nx_ip_driver_packet;

            /* Adjust the prepend pointer back by NX_ETHERNET_SIZE. */
            packet_ptr -> nx_packet_prepend_ptr -= NX_ETHERNET_SIZE;

            /* Adjust the packet length by NX_ETHERNET_SIZE. */
            packet_ptr -> nx_packet_length += NX_ETHERNET_SIZE;

            /* Setup the ethernet frame pointer to build the ethernet frame. Backup another 2
             * bytes to get 32-bit word alignment. */
            ethernet_frame_ptr =  (ULONG*) (packet_ptr -> nx_packet_prepend_ptr - 2);
            
            /* Build the actual ethernet frame. */
            
            /* Set destination MAC. */
            if(driver_command==NX_LINK_PACKET_SEND || driver_command==NX_LINK_ARP_RESPONSE_SEND)
            {
                *ethernet_frame_ptr     =  driver_req_ptr -> nx_ip_driver_physical_address_msw;
                *(ethernet_frame_ptr+1) =  driver_req_ptr -> nx_ip_driver_physical_address_lsw;
            }
            else
            {
                *ethernet_frame_ptr     =  0xFFFF;          /* Broadcast! */
                *(ethernet_frame_ptr+1) =  0xFFFFFFFFUL;    
            }
            
            /* Set our MAC. */
            *(ethernet_frame_ptr+2) =  (ip_ptr -> nx_ip_arp_physical_address_msw << 16) | 
                                       (ip_ptr -> nx_ip_arp_physical_address_lsw >> 16);
            
            /* Set our MAC and frame type. */
            switch(driver_command)
            {
                case NX_LINK_PACKET_SEND:
                case NX_LINK_PACKET_BROADCAST:
                {
                    *(ethernet_frame_ptr + 3) = (ip_ptr -> nx_ip_arp_physical_address_lsw << 16);
                    
                    #ifdef FEATURE_NX_IPV6
                    if(packet_ptr->nx_packet_ip_version == 4)
                    #endif /* FEATURE_NX_IPV6 */
                        *(ethernet_frame_ptr + 3) |= NX_ETHERNET_IP;
                    #ifdef FEATURE_NX_IPV6
                    else if(packet_ptr -> nx_packet_ip_version == 6)
                        *(ethernet_frame_ptr + 3) |= NX_ETHERNET_IPV6;         
                    else 
                    {   /* Unknown IP version */
                        NX_BACKOFF_RELEASE(packet_ptr)
                        break;
                    }
                    #endif /* FEATURE_NX_IPV6 */
                    
                    break;
                }
                
                case NX_LINK_ARP_SEND:
                case NX_LINK_ARP_RESPONSE_SEND:
                {
                    *(ethernet_frame_ptr + 3) = (ip_ptr -> nx_ip_arp_physical_address_lsw << 16) | NX_ETHERNET_ARP;
                    break;
                }
                
                default:
                {
                    *(ethernet_frame_ptr + 3) = (ip_ptr -> nx_ip_arp_physical_address_lsw << 16) | NX_ETHERNET_RARP;
                    break;
                }
            }
            
            
            /* Endian swapping if necessary. */
            NX_CHANGE_ULONG_ENDIAN(*(ethernet_frame_ptr));
            NX_CHANGE_ULONG_ENDIAN(*(ethernet_frame_ptr + 1));
            NX_CHANGE_ULONG_ENDIAN(*(ethernet_frame_ptr + 2));
            NX_CHANGE_ULONG_ENDIAN(*(ethernet_frame_ptr + 3));

            /* Put the frame on the wire. */
            driver_req_ptr -> nx_ip_driver_status = nx_ether_send(packet_ptr);
            break;  
        } /* End cases NX_LINK_PACKET_SEND, NX_LINK_PACKET_BROADCAST,
           * NX_LINK_ARP_SEND, NX_LINK_ARP_RESPONSE_SEND, NX_LINK_RARP_SEND. */

        case NX_LINK_MULTICAST_JOIN:
        {
            /* Enable Multicast. */
            if ( !(nx_record.nx_state & NX_LINK_ENABLED))
            {
                driver_req_ptr -> nx_ip_driver_status = NX_NOT_ENABLED;
                break;
            }                
            nx_multicast_join();
            break;
        }


        case NX_LINK_MULTICAST_LEAVE:
        {
            driver_req_ptr -> nx_ip_driver_status = NX_UNHANDLED_COMMAND;
            *(driver_req_ptr -> nx_ip_driver_return_ptr) = 0;
            break;
        }

        case NX_LINK_GET_STATUS:
        {
            *(driver_req_ptr -> nx_ip_driver_return_ptr) = ip_ptr -> nx_ip_driver_link_up;
            break;
        }

#ifdef NX_STAT
        case NX_LINK_GET_ERROR_COUNT:
        {
            if ( !(nx_record.nx_state & NX_LINK_ENABLED) )
            {
                driver_req_ptr -> nx_ip_driver_status = NX_NOT_ENABLED;
                break;
            }                
            /* Return the error count (rx + tx) in the supplied return pointer. */
            *(driver_req_ptr -> nx_ip_driver_return_ptr) = nx_record.stat.rx_errors +
                                                            nx_record.stat.tx_errors; 
            
            break;
        }

        case NX_LINK_GET_RX_COUNT:
        {
            if ( !(nx_record.nx_state & NX_LINK_ENABLED) )
            {
                driver_req_ptr -> nx_ip_driver_status = NX_NOT_ENABLED;
                break;
            }
            /* Return the rx packet count in the supplied return pointer. */
            *(driver_req_ptr -> nx_ip_driver_return_ptr) = nx_record.stat.rx_packets;
            break;
        }


        case NX_LINK_GET_TX_COUNT:
        {
            if ( !(nx_record.nx_state & NX_LINK_ENABLED) )
            {
                driver_req_ptr -> nx_ip_driver_status = NX_NOT_ENABLED;
                break;
            }                
            /* Return the error count (rx + tx) in the supplied return pointer.  */
            *(driver_req_ptr -> nx_ip_driver_return_ptr) = nx_record.stat.tx_packets;
            break;
        }
#else
        case NX_LINK_GET_ERROR_COUNT:
        case NX_LINK_GET_RX_COUNT:
        case NX_LINK_GET_TX_COUNT:
        {
            /* Return the unhandled command status. */
            driver_req_ptr -> nx_ip_driver_status = NX_UNHANDLED_COMMAND;

            /* Return NULL in the supplied return pointer. */
            *(driver_req_ptr -> nx_ip_driver_return_ptr) = 0;
            break;
        }            

#endif  /* NX_STAT */

        case NX_LINK_GET_ALLOC_ERRORS:
        case NX_LINK_GET_SPEED:
        case NX_LINK_GET_DUPLEX_TYPE:
        case NX_LINK_USER_COMMAND :
        {
            driver_req_ptr -> nx_ip_driver_status = NX_UNHANDLED_COMMAND;
            /* Return the link status in the supplied return pointer. */
            *(driver_req_ptr -> nx_ip_driver_return_ptr) = 0;
            break;
        }
        
        case NX_LINK_DEFERRED_PROCESSING:
        {
            if(driver_req_ptr -> nx_ip_driver_ptr -> nx_ip_additional_link_info)
            {
                *(driver_req_ptr -> nx_ip_driver_return_ptr) = 0;
            }
            break;      
        }
        default:
        {
            driver_req_ptr -> nx_ip_driver_status = NX_UNHANDLED_COMMAND;
            break;
        }
    }
}   /* nx_driver_renesas_rz() */


/**************************************************************************/ 
/*                                                                        */ 
/*  FUNCTION                                               RELEASE        */ 
/*                                                                        */ 
/*    _nx_ether_interrupt                                 PORTABLE C      */ 
/*                                                           5.0          */ 
/*                                                                        */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Scott Larson, Microsoft Corporation                                 */
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This function is the main renesas interrupt handler.                */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    None                                                                */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    None                                                                */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    nx_rx_interrupt                                                     */ 
/*    nx_tx_interrupt                                                     */ 
/*    nx_error_interrupt                                                  */ 
/*                                                                        */ 
/*  CALLED BY                                                             */ 
/*                                                                        */ 
/*    Interrupt handler                                                   */ 
/*                                                                        */ 
/*                                                                        */ 
/*  RELEASE HISTORY                                                       */ 
/*                                                                        */ 
/*    DATE              NAME                      DESCRIPTION             */ 
/*                                                                        */ 
/*  01-01-2014       Scott Larson           Initial release               */ 
/*                                                                        */ 
/**************************************************************************/ 
void _nx_ether_interrupt(uint32_t int_sense)   
{

ULONG   eesr;
ULONG   ecsr;
    
    
    /* Read ETHER and EDMAC status registers. */
	eesr = ETHER.EESR0;
	ecsr = ETHER.ECSR0;
     
    /* Clear interrupts. */
    ETHER.EESR0 = eesr;
    ETHER.ECSR0 = ecsr;
    
    /* Receive. */
    if(eesr & (EESR_FR | EESR_RMAF | EESR_RRF | EESR_RTLF | EESR_RTSF | EESR_PRE | EESR_CERF))
    {
        nx_rx_interrupt();
    }
    
    /* Transmit. */
    if(eesr & EESR_TC)
    {
        nx_tx_interrupt();
    }
    
    /* Error. */
    if(eesr & (EESR_TABT | EESR_RABT | EESR_RFCOF | EESR_TDE | EESR_TFUF | EESR_RDE | 
               EESR_RFOF | EESR_RRF | EESR_RTLF | EESR_RTSF | EESR_PRE | EESR_CERF))
    {
        nx_error_interrupt(eesr);
    }
    
    /* Multicast Frame received. */ 
    if(eesr & EESR_RMAF) 
    {
        #ifdef NX_STAT
        nx_record.stat.multicast++;
        #endif
    }
    
    /* Illegal carrier detection. */
    if(ecsr & ECSR_ICD)
    {
        /* Clear link enabled flag - cable unplugged. */
        nx_record.nx_state = nx_record.nx_state & ~(NX_LINK_ENABLED);
        
        #ifdef NX_STAT
        nx_record.stat.illegal_carrier_errors++;
        #endif
        
        /* To report link status changes to the application,
         * add a semaphore put or event flag set to the above statement.
         */
    }
    
    return;
} /* End _nx_ether_interrupt(). */


/**************************************************************************/ 
/*                                                                        */ 
/*  FUNCTION                                               RELEASE        */ 
/*                                                                        */ 
/*    nx_ether_send                                      PORTABLE C       */ 
/*                                                           5.0          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Scott Larson, Microsoft Corporation                                 */
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This function decides whether to queue or send the packet           */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    NX_PACKET *pkt  - pointer to the data packet                        */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    NX_SUCCESS or                                                       */ 
/*    NX_TX_QUEUE_DEPTH                                                   */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*   NX_BACKOFF_RELEASE                                                   */ 
/*   nx_ether_write                                                       */ 
/*                                                                        */ 
/*  CALLED BY                                                             */ 
/*                                                                        */ 
/*    nx_driver_renesas_rz                                                */ 
/*                                                                        */ 
/*                                                                        */ 
/*  RELEASE HISTORY                                                       */ 
/*                                                                        */ 
/*    DATE              NAME                      DESCRIPTION             */ 
/*                                                                        */ 
/*  01-01-2014       Scott Larson           Initial release               */ 
/*                                                                        */ 
/**************************************************************************/ 
static unsigned int nx_ether_send(NX_PACKET *packet)
{

TX_INTERRUPT_SAVE_AREA;

    
    /* Check if link is enabled. */
    if( nx_record.nx_state & NX_LINK_ENABLED )
    {
        
        /* If packets are already queued or device is transmitting we need to
         * add the packet to the end of the queue. */
        
        /* Disable interrupts while messing with queue pointers. */
        TX_DISABLE
        if( (tx_in_progress!= 0) || (driver_tx_packet_queue!=NULL) )
        {
            /* Queue this packet at the front of the queue,
             * or drop if queue depth exceeded. */
            if(tx_queued_count < TX_QUEUE_DEPTH)
            {
                packet->nx_packet_queue_next= NULL;
                if(driver_tx_packet_queue==NULL)
                {   /* Queue is empty. */
                    driver_tx_packet_queue = packet;
                    driver_tx_packet_queue_end = packet;
                } 
                else 
                {   /* Add to end of queue. */
                    driver_tx_packet_queue_end->nx_packet_queue_next = packet;
                    driver_tx_packet_queue_end = packet;
                }
                tx_queued_count++;
                TX_RESTORE
                return NX_SUCCESS;
            }
            else    /* Queue full, drop packet. */
            {
                TX_RESTORE
                NX_BACKOFF_RELEASE(packet);
                return NX_TX_QUEUE_DEPTH;
            }
        }
        else    /* No need to queue, just send the packet. */
        {
            nx_ether_write(packet);
            TX_RESTORE
        }
    }
    else    /* Link not enabled. */
    {
            NX_BACKOFF_RELEASE(packet);
            return NX_NOT_ENABLED;
    }
    
    return NX_SUCCESS;
}   /* End nx_ether_send. */


/**************************************************************************/ 
/*                                                                        */ 
/*  FUNCTION                                               RELEASE        */ 
/*                                                                        */ 
/*    nx_ether_write                                     PORTABLE C       */ 
/*                                                           5.0          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Scott Larson, Microsoft Corporation                                 */
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This function writes the data to the TX FIFO buffer descriptor      */
/*    This function can only be called when there is no active            */
/*    transmission.                                                       */
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    NX_PACKET *pkt  - pointer to the data packet                        */
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    NX_SUCCESS                                                          */
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    None                                                                */ 
/*                                                                        */ 
/*  CALLED BY                                                             */ 
/*                                                                        */ 
/*    nx_ether_send                                                       */ 
/*    nx_tx_interrupt                                                     */
/*                                                                        */ 
/*  RELEASE HISTORY                                                       */ 
/*                                                                        */ 
/*    DATE              NAME                      DESCRIPTION             */ 
/*                                                                        */ 
/*  01-01-2014       Scott Larson           Initial release               */
/*                                                                        */ 
/**************************************************************************/ 
static unsigned int nx_ether_write(NX_PACKET *packet)
{

NX_PACKET           *current_packet;
INT                 j;
ULONG               length;
UCHAR               *new_prepend_ptr;
UCHAR               *new_append_ptr;
ULONG               bd_count = 0;

    /* Setup current packet pointer.  */
    current_packet = packet;
    
    /* Check for valid packet. */
    if(!current_packet)
    {
        return NX_INVALID_PACKET;
    }

    while (current_packet)
    {
        /* Set up a transfer descriptor for each NX_PACKET in the chain. */
        /* Calculate length. */
        length = current_packet->nx_packet_append_ptr - current_packet->nx_packet_prepend_ptr;
        
        /* Alignment requirement to 5 bits for data 16 bytes or less,
           nx_packets need to be minimum 32+16 in size. */
        if (length < 17)
        {
            /* Check alignment. */
            if( ((ULONG)(current_packet->nx_packet_prepend_ptr)) & 0x1F)
            {
                new_prepend_ptr = (UCHAR*) (((ULONG)(current_packet->nx_packet_prepend_ptr) + 0x1F) & 0xFFFFFFE0) ; 
                new_append_ptr = current_packet->nx_packet_append_ptr + (new_prepend_ptr - current_packet->nx_packet_prepend_ptr) ; 
                
                /* Loop from back to front to copy data in the NX_PACKET. */
                for(j = length; j > 0; j--)
                {
                    new_prepend_ptr[j-1] = current_packet->nx_packet_prepend_ptr[j-1];
                }

                /* Set new prepend and append pointers. */
                current_packet->nx_packet_prepend_ptr = new_prepend_ptr;
                current_packet->nx_packet_append_ptr = new_append_ptr;
            }
        }
        
        if (bd_count == 0)
        {

            /* Set FP1 on the first descriptor of the frame. */
            tx_bd[current_txbd_index].bd_status = BD_FP1;

        }
        else
        {

            /* Clear BD status. */
            tx_bd[current_txbd_index].bd_status = 0;
        }

        /* Set up the transmit descriptor length, data buffer pointer, and packet pointer. */
        tx_bd[current_txbd_index].bd_bufsize = (USHORT) length;
        tx_bd[current_txbd_index].bd_buffer_ptr = current_packet->nx_packet_prepend_ptr;
        tx_bd[current_txbd_index].bd_nx_packet = packet;
        
        /* Get next packet in the chain. */
        current_packet =  current_packet->nx_packet_next;
        
        if(current_packet)
        {
            if(current_txbd_index == (NUM_TX_DESC - 1) )
            {
                tx_bd[current_txbd_index].bd_status |= (BD_ACT | BD_DL);
            }
            else
            {
                tx_bd[current_txbd_index].bd_status |= BD_ACT;
            }
        }
        else  
        {
            /* Check if this is the last BD. */
            if( current_txbd_index == (NUM_TX_DESC - 1) )
                tx_bd[current_txbd_index].bd_status |= (BD_FP0 | BD_ACT | BD_DL);
            else
                tx_bd[current_txbd_index].bd_status |= (BD_FP0 | BD_ACT);
            
            #ifdef NX_STAT
            /* Increment transmit packet statistic. */
            nx_record.stat.tx_packets++;
            #endif
        }
        
        /* Increment the buffer descriptor table index. */
        current_txbd_index = ((current_txbd_index + 1) % NUM_TX_DESC);
        
        bd_count ++;

    } /* End while loop. */
    
    /* Set flag. */
    tx_in_progress = 1;
    
    /* Start transmission. */
    ETHER.EDTRR0 = 0x00000003;
    
    return NX_SUCCESS;
}   /* nx_ether_write */


/**************************************************************************/ 
/*                                                                        */ 
/*  FUNCTION                                               RELEASE        */ 
/*                                                                        */ 
/*    nx_hw_init_rz                                       PORTABLE C      */ 
/*                                                           5.0          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Scott Larson, Microsoft Corporation                                 */
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This function initializes the RZ Ethernet Controller (ETHER) and    */ 
/*    the PHY.                                                            */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    nx_record                         Pointer to Ethernet record        */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    Completion Status                                                   */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    tx_thread_sleep                                                     */ 
/*    Phy_Init                                                            */ 
/*    Phy_GetLinkStatus                                                   */ 
/*    Phy_SetAutonegotiate                                                */ 
/*                                                                        */ 
/*  CALLED BY                                                             */ 
/*                                                                        */ 
/*    nx_ether_init                                                       */ 
/*                                                                        */ 
/*  RELEASE HISTORY                                                       */ 
/*                                                                        */ 
/*    DATE              NAME                      DESCRIPTION             */ 
/*                                                                        */ 
/*  01-01-2014       Scott Larson           Initial release               */ 
/*                                                                        */ 
/**************************************************************************/ 
static UINT  nx_hw_init_rz(NX_REC* nx_record)
{

SHORT       phydata;
USHORT      link_speed_duplex;
USHORT      local_pause_bits;
USHORT      partner_pause_bits;
    
    
    /* Take ETHER out of standby. */
	CPG.STBCR7 &= 0xEF;
    
    /* ETHER software reset. */
    ETHER.ARSTR |= 1;

    /* The ARSTR SW Reset requires 256 BCLK cycles, so sleep for a tick. */
    tx_thread_sleep(1);
    
    /* Activate ETHER module according to procedure in datasheet (26.4.8). */
    /* (1) Reset. */
    /* 1. Perform a power-on reset. */
    /* 2. Start the E-DMAC tx and rx. */
    ETHER.EDSR0 = EDSR_ENT | EDSR_ENR;
    
    /* 3. Perform a software reset of the ETHER module by writing 1 to
     * bits SWRR and SWRT simultaneously. */
    ETHER.EDMR0 = EDMR_SWRT | EDMR_SWRR;
    
    /* 4. Init descriptor entry table - this is done elsewhere. */
    /* The EDMR SW Reset requires 64 BCLK cycles, so sleep for a tick. */
    tx_thread_sleep(1);
    
    /* 5. Check that SW reset successful (both bits clear). */
    while((ETHER.EDMR0 & EDMR_SWRR) || (ETHER.EDMR0 & EDMR_SWRT))
    {
        /* After the tx_thread_sleep above, the SW reset should have had
         * plenty of time to clear - we should never loop here. */
    }
    
    
    /* Set E-DMAC related registers. */
    
    /* EDMR defaults to 16-byte descriptor length. */
#ifdef NX_LITTLE_ENDIAN
    ETHER.EDMR0 |= EDMR_DE; /* Little endian. */
#endif
    /* EESIPR is set elsewhere. */
    ETHER.TRSCER0   = 0x00000000;       /* copy-back status is RFE & TFE only */
    ETHER.TFTR0     = 0x00000008;       /* threshold of TX FIFO 32 bytes */
    ETHER.FDR0      = 0x00000107;       /* TX FIFO/RX FIFO are 512/2048 bytes */
    ETHER.RMCR0     = 0x00000001;       /* RR in EDRRR is under driver control */
    ETHER.RPADIR0   = 0x0002000E;       /* RX padding insertion: 2 bytes after 14th byte. */
    ETHER.FCFTR0    = 0x000F0003;       /* 16 frame threshold, 1024-32 byte threshold. */
    
    /* Set E-MAC related registers. */
    /* More ECMR bits are set below. */
    /* Not going to use receive hardware checksum calculation. */
    ETHER.ECMR0 &= 0xFF7FFFFF;           /* Disable checksum calculation. */

    /* Set MAC address. */
    ETHER.MAHR0     = (ETHERNET_MAC_H << 16) | ((ETHERNET_MAC_L >> 16) & 0x0000FFFF);
    ETHER.MALR0     = ETHERNET_MAC_L & 0x0000FFFF;
    ETHER.RFLR0     = NX_ENET_MAX_MTU;  /* Ether payload is 1500+ CRC. */
    
    /* APR not set up. */
    /* MPR not set up. */
    /* TPAUSER not set up, default to unlimited PAUSE frame retransmit count. */
    
    ETHER.EESR0     = 0xFF7F07FF;       /* Clear all ETHERC and EDMAC status flags. */
    ETHER.ECSR0     = 0x00000011;       /* Clear ETHER status PFROI & ICD flags. */
    
    /* (3) Registration of Descriptor Ring. */
    /* Initialize Rx/Tx Descriptor List Address. */
    ETHER.RDLAR0 = (ULONG) (&nx_record->driver_rx_bd[0]);
    ETHER.TDLAR0 = (ULONG) (&nx_record->driver_tx_bd[0]);
    
    /* Initialize Rx/Tx Descriptor Fetch Address. */
    ETHER.RDFAR0 = (ULONG) (&nx_record->driver_rx_bd[0]);
    ETHER.TDFAR0 = (ULONG) (&nx_record->driver_tx_bd[0]);
    
    /* Initialize Rx/Tx Descriptor Finished Address. */
    ETHER.RDFXR0 = (ULONG) (&nx_record->driver_rx_bd[NUM_RX_DESC - 1]);
    ETHER.TDFXR0 = (ULONG) (&nx_record->driver_tx_bd[NUM_TX_DESC - 1]);
    
    /* Initialize Rx/Tx Descriptor Final Flag. */
    ETHER.RDFFR0 = 0x00000001;
    ETHER.TDFFR0 = 0x00000001;
    
    /* Initialize PHY. */
    phydata = Phy_Init();

    if (phydata == R_PHY_ERROR)
    {
        return NX_NOT_SUCCESSFUL;
    }

    /* 100ms poll for link to come up. */
    do
    {
        tx_thread_sleep(10);  
        phydata =  Phy_GetLinkStatus();
    } while (phydata == R_PHY_ERROR);
    
    /* Start PHY auto negotiation. */ 
    phydata = Phy_SetAutonegotiate(&link_speed_duplex, 
                                   &local_pause_bits,
                                   &partner_pause_bits);
    
    switch(link_speed_duplex)
    {
        /* Half duplex link. */
        case PHY_LINK_100H:
        case PHY_LINK_10H:
        {
            ETHER.ECMR0 &= 0xFFFFFFFD;
            break;
        }
        
        /* Full duplex link. */
        case PHY_LINK_100F:
        case PHY_LINK_10F:
        {
            ETHER.ECMR0 |= 0x2;

            break;
        }
        default:
        {
            return NX_NOT_SUCCESSFUL;
            break;
        }
    }
    
    tx_thread_sleep(10); /* wait for PHY to settle */
    
    /* for IPv6 mode promiscuous mode is required */
    /* ETHERC.ECMR.BIT.PRM = 1; */
    
    /* Ethernet ports and interrupt set up in bsp.c. */
    
    /* (5) Activation. */
    
    /* Enable E-DMAC receive. */
    ETHER.EDRRR0 |= 0x1;
    
    /* Enable ETHER receive and transmit. */
    ETHER.ECMR0 |= 0x40;
    ETHER.ECMR0 |= 0x20;
    
    return NX_SUCCESS;
}


/**************************************************************************/ 
/*                                                                        */ 
/*  FUNCTION                                               RELEASE        */ 
/*                                                                        */ 
/*    nx_ether_init                                       PORTABLE C      */ 
/*                                                           5.0          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Scott Larson, Microsoft Corporation                                 */
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This function initializes the ETHERC and some E-DMAC                */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    driver_record                     Pointer to Ethernet record        */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    Completion Status                                                   */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    Set_IntVect                                                         */ 
/*    nx_hw_init_rz                                                       */ 
/*                                                                        */ 
/*  CALLED BY                                                             */ 
/*                                                                        */ 
/*    nx_driver_renesas_rz                                                */ 
/*                                                                        */ 
/*  RELEASE HISTORY                                                       */ 
/*                                                                        */ 
/*    DATE              NAME                      DESCRIPTION             */ 
/*                                                                        */ 
/*  01-01-2014       Scott Larson           Initial release               */ 
/*                                                                        */ 
/**************************************************************************/ 
static ULONG nx_ether_init(NX_REC* driver_rec)
{   

ULONG   iprreg;

#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
#if 0
	/*
	Port_bit    Alt.    Desc.   I/O
	--------    ---     ----    ---
	P8_0        2nd     TXD0    out
	P8_1        2nd     TXD1    out
	P8_2        2nd     TXD2    out
	P8_3        2nd     TXD3    out
	P8_4        2nd     TXCLK   in
	P8_5        2nd     TXER    out
	P8_6        2nd     TXEN    out
	P8_7        2nd     RXD0    in
	P8_8        2nd     RXD1    in
	P8_9        2nd     RXD2    in
	P8_10       2nd     RXD3    in
	P8_14       2th     COL     in
	P8_15       2nd     CRS     in
	P9_0        2nd     MDC     out
	P9_1        2nd     MDIO    in/out
	P9_2        2nd     RXCLK   in
	P9_3        2nd     RXER    in
	P9_4        2nd     RXDV    in
	P2_7		gpio    RST    in
	*/
	st_port_config_t pFunc[18] = {
		{P8_0, FUNCTION_MODE2, PIN_IO},
		{P8_1, FUNCTION_MODE2, PIN_IO},
		{P8_2, FUNCTION_MODE2, PIN_IO},
		{P8_3, FUNCTION_MODE2, PIN_IO},
		{P8_4, FUNCTION_MODE2, PIN_IO},
		{P8_5, FUNCTION_MODE2, PIN_IO},
		{P8_6, FUNCTION_MODE2, PIN_IO},
		{P8_7, FUNCTION_MODE2,  PIN_IO},
		{P8_8, FUNCTION_MODE2,  PIN_IO},
		{P8_9, FUNCTION_MODE2,  PIN_IO},
		{P8_10, FUNCTION_MODE2, PIN_IO},
		{P8_14, FUNCTION_MODE2, PIN_IO},
		{P8_15, FUNCTION_MODE2, PIN_IO},
		{P9_0, FUNCTION_MODE2,  PIN_IO},
		{P9_1, FUNCTION_MODE2,  PIN_IO},
		{P9_2, FUNCTION_MODE2,  PIN_IO},
		{P9_3, FUNCTION_MODE2,  PIN_IO},
		{P9_4, FUNCTION_MODE2,  PIN_IO},
	};

	for ( int i = 0; i < 19; i++) {
		set_pin_function(&pFunc[i]);
	}

	/* Setup the PHY Reset Pin for GPIO */
	gpio_init(P2_7);
	gpio_dir(P2_7, PIN_OUTPUT);

	/* Reset the PHY transceiver */
	gpio_write(P2_7, 0);
	R_OS_TaskSleep(10);

	/* Take the PHY transceiver out of reset */
	gpio_write(P2_7, 1);
	R_OS_TaskSleep(10);
#else
	//Configure ET_TXD0 (P8_0)
	GPIO.PMC8 |= GPIO_PMC8_PMC80;    // PMC8.0 = 1
	GPIO.PFC8 |= GPIO_PFC8_PFC80;    // PFC8.0 = 1
	GPIO.PFCE8 &= (uint16_t)(~GPIO_PFCE8_PFCE80);    // PFCE8.0 = 0
	GPIO.PFCAE8 &= (uint16_t)(~GPIO_PFCAE8_PFCAE80);    // PFCAE8.0 = 0
	GPIO.PIPC8 |= GPIO_PIPC8_PIPC80;    // PIPC8.0 = 1

	//Configure ET_TXD1 (P8_1)
	GPIO.PMC8 |= GPIO_PMC8_PMC81;   // PMC8.1 = 1
	GPIO.PFC8 |= GPIO_PFC8_PFC81;   // PFC8.1 = 1
	GPIO.PFCE8 &= (uint16_t)(~GPIO_PFCE8_PFCE81); // PFCE8.1 = 0
	GPIO.PFCAE8 &= (uint16_t)(~GPIO_PFCAE8_PFCAE81);  // PFCAE8.1 = 0
	GPIO.PIPC8 |= GPIO_PIPC8_PIPC81;    // PIPC8.1 = 1



	//Configure ET_TXD2 (P8_2)
	GPIO.PMC8 |= GPIO_PMC8_PMC82;   // PMC8.2 = 1
	GPIO.PFC8 |= GPIO_PFC8_PFC82;   // PFC8.2 = 1
	GPIO.PFCE8 &= (uint16_t)(~GPIO_PFCE8_PFCE82); // PFCE8.2 = 0
	GPIO.PFCAE8 &= (uint16_t)(~GPIO_PFCAE8_PFCAE82);  // PFCAE8.2 = 0
	GPIO.PIPC8 |= GPIO_PIPC8_PIPC82;    // PIPC8.2 = 1


	//Configure ET_TXD3 (P8_3)
	GPIO.PMC8 |= GPIO_PMC8_PMC83;   // PMC8.3 = 1
	GPIO.PFC8 |= GPIO_PFC8_PFC83;   // PFC8.3 = 1
	GPIO.PFCE8 &= (uint16_t)(~GPIO_PFCE8_PFCE83); // PFCE8.3 = 0
	GPIO.PFCAE8 &= (uint16_t)(~GPIO_PFCAE8_PFCAE83);  // PFCAE8.3 = 0
	GPIO.PIPC8 |= GPIO_PIPC8_PIPC83;    // PIPC8.3 = 1

	//Configure ET_TXCLK (P8_4)
	GPIO.PMC8 |= GPIO_PMC8_PMC84;    // PMC8.4 = 1
	GPIO.PFC8 |= GPIO_PFC8_PFC84;   // PFC8.4 = 1
	GPIO.PFCE8 &= (uint16_t)(~GPIO_PFCE8_PFCE84); // PFCE8.4 = 0
	GPIO.PFCAE8 &= (uint16_t)(~GPIO_PFCAE8_PFCAE84);  // PFCAE8.4 = 0
	GPIO.PIPC8 |= GPIO_PIPC8_PIPC84;    // PIPC8.4 = 1


	//Configure ET_TXER (P8_5)
	GPIO.PMC8 |= GPIO_PMC8_PMC85;   // PMC8.5 = 1
	GPIO.PFC8 |= GPIO_PFC8_PFC85;   // PFC8.5 = 1
	GPIO.PFCE8 &= (uint16_t)(~GPIO_PFCE8_PFCE85); // PFCE8.5 = 0
	GPIO.PFCAE8 &= (uint16_t)(~GPIO_PFCAE8_PFCAE85);  // PFCAE8.5 = 0
	GPIO.PIPC8 |= GPIO_PIPC8_PIPC85;    // PIPC8.5 = 1

	//Configure ET_TXEN (P8_6)
	GPIO.PMC8 |= GPIO_PMC8_PMC86;   // PMC8.6 = 1
	GPIO.PFC8 |= GPIO_PFC8_PFC86;   // PFC8.6 = 1
	GPIO.PFCE8 &= (uint16_t)(~GPIO_PFCE8_PFCE86); // PFCE8.6 = 0
	GPIO.PFCAE8 &= (uint16_t)(~GPIO_PFCAE8_PFCAE86);  // PFCAE8.6 = 0
	GPIO.PIPC8 |= GPIO_PIPC8_PIPC86;    // PIPC8.6 = 1


	//Configure ET_RXD0 (P8_7)
	GPIO.PMC8 |= GPIO_PMC8_PMC87;   // PMC8.7 = 1
	GPIO.PFC8 |= GPIO_PFC8_PFC87;   // PFC8.7 = 1
	GPIO.PFCE8 &= (uint16_t)(~GPIO_PFCE8_PFCE87); // PFCE8.7 = 0
	GPIO.PFCAE8 &= (uint16_t)(~GPIO_PFCAE8_PFCAE87);  // PFCAE8.7 = 0
	GPIO.PIPC8 |= GPIO_PIPC8_PIPC87;    // PIPC8.7 = 1


	//Configure ET_RXD1 (P8_8)
	GPIO.PMC8 |= GPIO_PMC8_PMC88;   // PMC8.8 = 1
	GPIO.PFC8 |= GPIO_PFC8_PFC88;   // PFC8.8 = 1
	GPIO.PFCE8 &= (uint16_t)(~GPIO_PFCE8_PFCE88); // PFCE8.8 = 0
	GPIO.PFCAE8 &= (uint16_t)(~GPIO_PFCAE8_PFCAE88);  // PFCAE8.8 = 0
	GPIO.PIPC8 |= GPIO_PIPC8_PIPC88;    // PIPC8.8 = 1

	//Configure ET_RXD2 (P8_9)
	GPIO.PMC8 |= GPIO_PMC8_PMC89;   // PMC8.9 = 1
	GPIO.PFC8 |= GPIO_PFC8_PFC89;   // PFC8.9 = 1
	GPIO.PFCE8 &= (uint16_t)(~GPIO_PFCE8_PFCE89); // PFCE8.9 = 0
	GPIO.PFCAE8 &= (uint16_t)(~GPIO_PFCAE8_PFCAE89);  // PFCAE8.9 = 0
	GPIO.PIPC8 |= GPIO_PIPC8_PIPC89;    // PIPC8.9 = 1


	//Configure ET_RXD3 (P8_10)
	GPIO.PMC8 |= GPIO_PMC8_PMC810;  // PMC8.10 = 1
	GPIO.PFC8 |= GPIO_PFC8_PFC810;  // PFC8.10 = 1
	GPIO.PFCE8 &= (uint16_t)(~GPIO_PFCE8_PFCE810);    // PFCE8.10 = 0
	GPIO.PFCAE8 &= (uint16_t)(~GPIO_PFCAE8_PFCAE810); // PFCAE8.10 = 0
	GPIO.PIPC8 |= GPIO_PIPC8_PIPC810;   // PIPC8.10 = 1


	//Configure ET_COL (P8_14)
	GPIO.PMC8 |= GPIO_PMC8_PMC814;  // PMC8.14 = 1
	GPIO.PFC8 |= GPIO_PFC8_PFC814;  // PFC8.14 = 1
	GPIO.PFCE8 &= (uint16_t)(~GPIO_PFCE8_PFCE814);    // PFCE8.14 = 0
	GPIO.PFCAE8 &= (uint16_t)(~GPIO_PFCAE8_PFCAE814); // PFCAE8.14 = 0
	GPIO.PIPC8 |= GPIO_PIPC8_PIPC814;   // PIPC8.14 = 1


	//Configure ET_CRS (P8_15)
	GPIO.PMC8 |= GPIO_PMC8_PMC815;  // PMC8.15 = 1
	GPIO.PFC8 |= GPIO_PFC8_PFC815;  // PFC8.15 = 1
	GPIO.PFCE8 &= (uint16_t)(~GPIO_PFCE8_PFCE815);    // PFCE8.15 = 0
	GPIO.PFCAE8 &= (uint16_t)(~GPIO_PFCAE8_PFCAE815); // PFCAE8.15 = 0
	GPIO.PIPC8 |= GPIO_PIPC8_PIPC815;   // PIPC8.15 = 1


	//Configure ET_MDC (P9_0)
	GPIO.PMC9 |= GPIO_PMC9_PMC90;   // PMC9.0 = 1
	GPIO.PFC9 |= GPIO_PFC9_PFC90;   // PFC9.0 = 1
	GPIO.PFCE9 &= (uint16_t)(~GPIO_PFCE9_PFCE90); // PFCE9.0 = 0
	GPIO.PFCAE9 &= (uint16_t)(~GPIO_PFCAE9_PFCAE90);  // PFCAE9.0 = 0
	GPIO.PIPC9 |= GPIO_PIPC9_PIPC90;    // PIPC9.0 = 1


	//Configure ET_MDIO (P9_1)
	GPIO.PMC9 |= GPIO_PMC9_PMC91;   // PMC9.1 = 1
	GPIO.PFC9 |= GPIO_PFC9_PFC91;   // PFC9.1 = 1
	GPIO.PFCE9 &= (uint16_t)(~GPIO_PFCE9_PFCE91); // PFCE9.1 = 0
	GPIO.PFCAE9 &= (uint16_t)(~GPIO_PFCAE9_PFCAE91);  // PFCAE9.1 = 0
	GPIO.PIPC9 |= GPIO_PIPC9_PIPC91;    // PIPC9.1 = 1

	//Configure ET_RXCLK (P9_2)
	GPIO.PMC9 |= GPIO_PMC9_PMC92;   // PMC9.2 = 1
	GPIO.PFC9 |= GPIO_PFC9_PFC92;   // PFC9.2 = 1
	GPIO.PFCE9 &= (uint16_t)(~GPIO_PFCE9_PFCE92); // PFCE9.2 = 0
	GPIO.PFCAE9 &= (uint16_t)(~GPIO_PFCAE9_PFCAE92);  // PFCAE9.2 = 0
	GPIO.PIPC9 |= GPIO_PIPC9_PIPC92;    // PIPC9.2 = 1


	//Configure ET_RXER (P9_3)
	GPIO.PMC9 |= GPIO_PMC9_PMC93;   // PMC9.3 = 1
	GPIO.PFC9 |= GPIO_PFC9_PFC93;   // PFC9.3 = 1
	GPIO.PFCE9 &= (uint16_t)(~GPIO_PFCE9_PFCE93); // PFCE9.3 = 0
	GPIO.PFCAE9 &= (uint16_t)(~GPIO_PFCAE9_PFCAE93);  // PFCAE9.3 = 0
	GPIO.PIPC9 |= GPIO_PIPC9_PIPC93;    // PIPC9.3 = 1


	//Configure ET_RXDV (P9_4)
	GPIO.PMC9 |= GPIO_PMC9_PMC94;   // PMC9.4 = 1
	GPIO.PFC9 |= GPIO_PFC9_PFC94;   // PFC9.4 = 1
	GPIO.PFCE9 &= (uint16_t)(~GPIO_PFCE9_PFCE94); // PFCE9.4 = 0
	GPIO.PFCAE9 &= (uint16_t)(~GPIO_PFCAE9_PFCAE94);  // PFCAE9.4 = 0
	GPIO.PIPC9 |= GPIO_PIPC9_PIPC94;    // PIPC9.4 = 1

	//Configure PHY_RST (P2_7)
	GPIO.PMC2 &= (uint16_t)(~GPIO_PMC2_PMC27);    // PMC2.7 = 0
	GPIO.PIPC2 &= (uint16_t)(~GPIO_PIPC2_PIPC27); // PIPC2.7 = 0
	GPIO.PM2 &= (uint16_t)(~GPIO_PM2_PM27);   // PM2.7 = 0


	//Reset the PHY transceiver
	GPIO.P2 &= (uint16_t)(~GPIO_P2_P27);   // P2.7 = 0
	R_OS_TaskSleep(10);

	//Take the PHY transceiver out of reset
	GPIO.P2 |= GPIO_P2_P27;  // P2.7 = 1

	R_OS_TaskSleep(10);

#endif


#elif (TARGET_BOARD == TARGET_BOARD_RSK)
    /* Set up IO for ethernet. */
	uint8_t px_addr, px_data, px_config;

	/* Port Expander 2 setting */
	/* PX1 EN1: High = Ethernet */
	R_RIIC_CAT9554_Open();
	px_addr = CAT9554_I2C_PX2;
	px_data = PX2_PX1_EN1;
	px_config = PX2_PX1_EN1;
	R_RIIC_CAT9554_Write(px_addr, px_data, px_config);
	R_RIIC_CAT9554_Close();

    /* 
    Port_bit    Alt.    Desc.   I/O
    --------    ---     ----    ---
    P1_14       4th     COL     in
    P1_15       0       int     in
    P2_0        2nd     TXCLK   out
    P2_1        2nd     TXER    out
    P2_2        2nd     TXEN    out
    P2_3        2nd     CRS     in
    P2_4        2nd     TXD0    out
    P2_5        2nd     TXD1    out
    P2_6        2nd     TXD2    out
    P2_7        2nd     TXD3    out
    P2_8        2nd     RXD0    in
    P2_9        2nd     RXD1    in
    P2_10       2nd     RXD2    in
    P2_11       2nd     RXD3    in
    P3_3        2nd     MDIO    in/out
    P3_4        2nd     RXCLK   in
    P3_5        2nd     RXER    in
    P3_6        2nd     RXDV    in
    P5_9        2nd     MDC     out
    */

	st_port_config_t pFunc[19] = {
		{P1_14, FUNCTION_MODE4, PIN_IO},
		/*{P1_15, FUNCTION_MODE1, PIN_INPUT},*/
		{P2_0,  FUNCTION_MODE2, PIN_IO},
		{P2_1,  FUNCTION_MODE2, PIN_IO},
		{P2_2,  FUNCTION_MODE2, PIN_IO},
		{P2_3,  FUNCTION_MODE2, PIN_IO},
		{P2_4,  FUNCTION_MODE2, PIN_IO},
		{P2_5,  FUNCTION_MODE2, PIN_IO},
		{P2_6,  FUNCTION_MODE2, PIN_IO},
		{P2_7,  FUNCTION_MODE2, PIN_IO},
		{P2_8,  FUNCTION_MODE2, PIN_IO},
		{P2_9,  FUNCTION_MODE2, PIN_IO},
		{P2_10, FUNCTION_MODE2, PIN_IO},
		{P2_11, FUNCTION_MODE2, PIN_IO},
		{P3_3,  FUNCTION_MODE2, PIN_IO},
		{P3_4,  FUNCTION_MODE2, PIN_IO},
		{P3_5,  FUNCTION_MODE2, PIN_IO},
		{P3_6,  FUNCTION_MODE2, PIN_IO},
		{P5_9,  FUNCTION_MODE2, PIN_IO},
	};
	for ( int i = 0; i < 19; i++) {
		set_pin_function(&pFunc[i]);
	}

#else
    /* Custom Board  pin settings
     * Use the Smart Configurator to setup the iebus pin settings
     */
    set_pins_function( &GPIO_SC_INIT_iebus);
#endif
    
    /* Ethernet interrupt setup. */
    
    /* Register ETHER interrupt handler function */
    R_INTC_RegistIntFunc(INTC_ID_ETHERI, _nx_ether_interrupt);

    /* Set priority of ETHER (GROUP:6,SUB:0). */
	R_INTC_SetPriority(INTC_ID_ETHERI, 25);

	/* interrupt enable */
	R_INTC_Enable(INTC_ID_ETHERI);
  
    
    /* Initialize ETHERC and EDMAC. */
    return nx_hw_init_rz(driver_rec);
    
}   /* nx_ether_init() */


/**************************************************************************/ 
/*                                                                        */ 
/*  FUNCTION                                               RELEASE        */ 
/*                                                                        */ 
/*    nx_storage_init                                     PORTABLE C      */ 
/*                                                           5.0          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Scott Larson, Microsoft Corporation                                 */
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This function initializes the receive packet pool and list of       */ 
/*    rx & tx descriptors.                                                */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    ip_ptr                                                              */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    NX_SUCCESS                                                          */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    nx_packet_pool_create                                               */ 
/*    nx_packet_allocate                                                  */ 
/*    memset                                                              */ 
/*                                                                        */ 
/*  CALLED BY                                                             */ 
/*                                                                        */ 
/*     nx_driver_renesas_rz                                               */ 
/*                                                                        */ 
/*  RELEASE HISTORY                                                       */ 
/*                                                                        */ 
/*    DATE              NAME                      DESCRIPTION             */ 
/*                                                                        */ 
/*  01-01-2014       Scott Larson           Initial release               */ 
/*                                                                        */ 
/**************************************************************************/ 
static unsigned int nx_storage_init(NX_IP *ip_ptr)
{

UINT    i;
UINT    status;
    
    
    /* All memory is assumed to be allocated at link time in the .bss section. */
    /* Avoid memory allocation errors at runtime. */

    /* Ensure 16-byte alignment for BDs. */
    rx_bd = (EMAC_BD*) ((ULONG)(rx_bd_space + 0xF) & 0xFFFFFFF0); 
    tx_bd = (EMAC_BD*) ((ULONG)(tx_bd_space + 0xF) & 0xFFFFFFF0); 
    nx_record.driver_rx_bd = rx_bd;
    nx_record.driver_tx_bd = tx_bd; 

    /* Initialize receive pool, descriptors and buffers, set receive BDs ready to receive. */
    /* Create a packet pool, ensure area is 32-byte aligned. */
    status =  nx_packet_pool_create(&rx_pool,
                                    "NetX RX Packet Pool",
                                    NX_PACKET_PAYLOAD_SIZE,
                                    (ULONG*)(((int)rx_packet_pool_area + 31) & ~31),
                                    NX_RECEIVE_PACKET_POOL_SIZE);
    
    /* Return if there is an error when creating pool. */
    if(status)
    {
        return status;
    }
        
    /* Allocate packets from the pool to be used for reception. */
    for(i = 0; i < NUM_RX_DESC; i++)
    {
        status = nx_packet_allocate(&rx_pool, &rx_bd[i].bd_nx_packet, NX_RECEIVE_PACKET, 0);
        
        /* Return if there is an error when allocating a packet. */
        if(status)
        {
            return status;
        }
        
        /* Set buffer length and address, init status. */
        rx_bd[i].bd_bufsize     = rx_pool.nx_packet_pool_payload_size;
        rx_bd[i].bd_buffer_ptr  = rx_bd[i].bd_nx_packet->nx_packet_prepend_ptr;
        rx_bd[i].bd_status      = BD_ACT;
    }

    /* Set DL bit in last descriptor. */
    rx_bd[NUM_RX_DESC-1].bd_status |= BD_DL;

    /* Initialize send descriptor area to 0. */
    memset(tx_bd, 0, sizeof(EMAC_BD) * NUM_TX_DESC);

    return NX_SUCCESS;
}   /* nx_storage_init */


/**************************************************************************/ 
/*                                                                        */ 
/*  FUNCTION                                               RELEASE        */ 
/*                                                                        */ 
/*    nx_multicast_join                                   PORTABLE C      */ 
/*                                                           5.0          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Scott Larson, Microsoft Corporation                                 */
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This function sets the device to accept the multicast packets.      */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    None                                                                */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    None                                                                */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    None                                                                */ 
/*                                                                        */ 
/*  CALLED BY                                                             */ 
/*                                                                        */ 
/*    nx_driver_renesas_rz                                                */ 
/*                                                                        */ 
/*  NOTE                                                                  */ 
/*                                                                        */ 
/*    There is no hardware supported multicast address hashing table for  */ 
/*    this device. So we will not implement the command explicitly.       */ 
/*                                                                        */ 
/*  RELEASE HISTORY                                                       */ 
/*                                                                        */ 
/*    DATE              NAME                      DESCRIPTION             */ 
/*                                                                        */ 
/*  01-01-2014       Scott Larson           Initial release               */ 
/*                                                                        */ 
/**************************************************************************/ 
static void nx_multicast_join(void)
{
    
    /* Just make sure the RX is enabled */
    ETHER.ECMR0 |= 0x40;

    return;

}   /* nx_multicast_join() */


/**************************************************************************/ 
/*                                                                        */ 
/*  FUNCTION                                               RELEASE        */ 
/*                                                                        */ 
/*    nx_error_interrupt                                  PORTABLE C      */ 
/*                                                           5.0          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Scott Larson, Microsoft Corporation                                 */
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This function simply handles all the error cases (RX/TX/PHY).       */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    ULONG errbits     - Error bits extracted from the Status word in    */ 
/*                        the descriptor                                  */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    None                                                                */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    None                                                                */ 
/*                                                                        */ 
/*  CALLED BY                                                             */ 
/*                                                                        */ 
/*    _nx_ether_interrupt                                                 */ 
/*                                                                        */ 
/*  RELEASE HISTORY                                                       */ 
/*                                                                        */ 
/*    DATE              NAME                      DESCRIPTION             */ 
/*                                                                        */ 
/*  01-01-2014       Scott Larson           Initial release               */ 
/*                                                                        */ 
/**************************************************************************/ 
static void nx_error_interrupt(ULONG errbits)
{
#ifdef NX_STAT
    
    if (errbits & EESR_TABT)
    {
        nx_record.stat.tx_abort++;
        nx_record.stat.tx_errors++;
    }
    
    if (errbits & EESR_RABT)
    {
        nx_record.stat.rx_abort++;
        nx_record.stat.rx_errors++;
    }
    
    if (errbits & EESR_RFCOF)
    {
        nx_record.stat.rx_frame_cnt_of_errors++;
        nx_record.stat.rx_errors++;
    }
    
    if (errbits & EESR_TDE)
    {
        nx_record.stat.tx_dscpt_out++;
        nx_record.stat.tx_errors++;
    }
    
    if (errbits & EESR_TFUF)
    {
        nx_record.stat.tx_fifo_uf_errors++;
        nx_record.stat.tx_errors++;
    }
    
    if (errbits & EESR_RDE)
    {
        nx_record.stat.rx_dscpt_out++;
        nx_record.stat.rx_errors++;
    }
    
    if (errbits & EESR_RFOF)
    {
        nx_record.stat.rx_fifo_of_errors++;
        nx_record.stat.rx_errors++;
    }
    
    if (errbits & EESR_RRF)
    {
        nx_record.stat.rx_frame_errors++;
        nx_record.stat.rx_errors++;
    }
    
    if (errbits & (EESR_RTLF | EESR_RTSF))
    {
        nx_record.stat.rx_length_errors++;
        nx_record.stat.rx_errors++;
    }
    
    if (errbits & EESR_PRE)
    {
        nx_record.stat.phy_lsi_errors++;
    }
    
    if (errbits & EESR_CERF)
    {
        nx_record.stat.rx_crc_errors++;
        nx_record.stat.rx_errors++;
    }
        
#endif
    return;

}   /* nx_error_interrupt() */


/**************************************************************************/ 
/*                                                                        */ 
/*  FUNCTION                                               RELEASE        */ 
/*                                                                        */ 
/*    nx_tx_interrupt                                     PORTABLE C      */ 
/*                                                           5.0          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Scott Larson, Microsoft Corporation                                 */
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This function handles the Tx done interrupt.                        */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    None                                                                */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    None                                                                */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    NX_BACKOFF_RELEASE                                                  */ 
/*    nx_ether_write                                                      */ 
/*                                                                        */ 
/*  CALLED BY                                                             */ 
/*                                                                        */ 
/*    nx_ether_interrupt                                                  */ 
/*                                                                        */ 
/*  RELEASE HISTORY                                                       */ 
/*                                                                        */ 
/*    DATE              NAME                      DESCRIPTION             */ 
/*                                                                        */ 
/*  01-01-2014       Scott Larson           Initial release               */ 
/*                                                                        */ 
/**************************************************************************/ 
static void nx_tx_interrupt(void)
{

NX_PACKET   *next_packet;
    
    
    /* If ACT bit set, return. */
    if(tx_bd[(current_txbd_index + NUM_TX_DESC - 1) % NUM_TX_DESC].bd_status & BD_ACT)
    {
        return;
    }
    
    /* Release the packet that was just transmitted. */
    NX_BACKOFF_RELEASE(tx_bd[(current_txbd_index + NUM_TX_DESC - 1) % NUM_TX_DESC].bd_nx_packet);
    /* Clear flag. */
    tx_in_progress = 0;
    
    /* Check if there's a packet in the queue. */
    if(driver_tx_packet_queue || tx_queued_count)
    {
        /* Update queue pointers. */
        next_packet = driver_tx_packet_queue;
        driver_tx_packet_queue = next_packet -> nx_packet_queue_next;
        
        /* Check if last packet in queue - reset queue. */
        if(driver_tx_packet_queue == NULL)
        {
            driver_tx_packet_queue_end = NULL;
        }
        
        /* Update queue count. */
        tx_queued_count--;
        
        /* Send packet. */       
        nx_ether_write(next_packet); 
    }
    
}   /* nx_tx_interrupt */


/**************************************************************************/ 
/*                                                                        */ 
/*  FUNCTION                                               RELEASE        */ 
/*                                                                        */ 
/*    nx_rx_interrupt                                     PORTABLE C      */ 
/*                                                           5.0          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Scott Larson, Microsoft Corporation                                 */
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This function handles the RX interrupt.                             */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    None                                                                */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    None                                                                */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    nx_packet_allocate                                                  */ 
/*    nx_receive                                                          */ 
/*                                                                        */ 
/*  CALLED BY                                                             */ 
/*                                                                        */ 
/*    _nx_ether_interrupt                                                 */ 
/*                                                                        */ 
/*                                                                        */ 
/*  RELEASE HISTORY                                                       */ 
/*                                                                        */ 
/*    DATE              NAME                      DESCRIPTION             */ 
/*                                                                        */ 
/*  01-01-2014       Scott Larson           Initial release               */ 
/*                                                                        */ 
/**************************************************************************/ 
static void nx_rx_interrupt(void)
{

ULONG           rxbd_status;
NX_PACKET       *packet_ptr=NULL;
NX_PACKET       *last_packet;
ULONG           bds_in_chain;
UINT            status;
UINT            i;
UINT            index;
UINT            buffer_size = 0;
UINT            remaining_bytes = 0;
    
    
    /* Do for every complete frame in the receive buffer */
    
    /* Get the status of the current BD. */
    rxbd_status = rx_bd[current_rxbd_index].bd_status;
    
    do
    {
        /* Initialize count. */
        bds_in_chain = 1;
        
        /* Count how many BDs are in current packet. The last BD in a chain is 
         * indicated by rxbd_status.FP0 = 1. */
        while( !(rxbd_status & BD_FP0) && !(rxbd_status & BD_ACT))
        {
            bds_in_chain++;     /* Increment counter. */
            
            /* Check if we've exceeded the number of descriptors defined. */
            if (bds_in_chain > NUM_RX_DESC)
            {
                return; /* We looped around, no more BDs to process. */
            }
            
            /* Get next BD status, ensure proper roll-over of the index. */
            rxbd_status = rx_bd[(current_rxbd_index + bds_in_chain - 1 ) % NUM_RX_DESC].bd_status;
        }
        
        
        /* There are 2 possible cases for this chain:
         * 1. Valid chain, (ACT=0, FP0=1).
         * 2. Still receiving frame.
         */
        /* Case 1: valid reception BD chain (ACT=0, FP0=1). */
        if( !(rxbd_status & BD_ACT) && (rxbd_status & BD_FP0) ) 
        {
            /* Get total packet length by reading the length in the last BD. */
            buffer_size = rx_bd[(current_rxbd_index + bds_in_chain - 1 ) % NUM_RX_DESC].bd_rxdatalength;
            buffer_size += NX_PADDING_SIZE;     /* Padding is not included in length. */
            remaining_bytes = buffer_size;
            
            /* Get first and last packet pointers. */
            packet_ptr = rx_bd[current_rxbd_index].bd_nx_packet;
            last_packet = rx_bd[(current_rxbd_index + bds_in_chain - 1 ) % NUM_RX_DESC].bd_nx_packet;
            
            /* For each BD in chain, set length, _next and _last pointers. */
            for(i = 0; i < bds_in_chain; i++)
            {
                /* Update and rollover index. */
                index = (current_rxbd_index + i) % NUM_RX_DESC;
                
                /* Adjust prepend ptr to point to the start of the packet. */
                rx_bd[index].bd_nx_packet->nx_packet_prepend_ptr = rx_bd[index].bd_buffer_ptr;
                
                /* Adjust append pointer to point to the end of the packet. */
                if( rx_bd[index].bd_status & BD_FP0 )
                {
                    /* Last BD. */
                    rx_bd[index].bd_nx_packet->nx_packet_append_ptr = 
                        rx_bd[index].bd_nx_packet->nx_packet_prepend_ptr + remaining_bytes;
                }
                else
                {
                    /* Intermediate BD holds RX_BUF_SIZE. */
                    rx_bd[index].bd_nx_packet->nx_packet_append_ptr = 
                        rx_bd[index].bd_nx_packet->nx_packet_prepend_ptr + RX_BUF_SIZE;
                    
                    /* Update remaining_bytes. */
                    remaining_bytes -= RX_BUF_SIZE;
                }
                
                /* Set length. */
                rx_bd[index].bd_nx_packet->nx_packet_length = buffer_size;
                
                /* Set _last pointer to point to last packet in chain. */
                rx_bd[index].bd_nx_packet->nx_packet_last = last_packet;
                
                /* Chain packet (set _next pointer). */
                rx_bd[index].bd_nx_packet->nx_packet_next = rx_bd[(index + 1) % NUM_RX_DESC].bd_nx_packet;
                
                
                /* Prepare BD for new reception:
                   - Allocate a new packet for the BD.
                   - Set pointer.
                   - Set ACT bit.
                */
                status = nx_packet_allocate(&rx_pool, &rx_bd[index].bd_nx_packet, NX_RECEIVE_PACKET, 0);
                
                if(status)
                {
                    /* To do: handle packet allocation error. */
                    
                    #if NX_STAT
                    nx_record.stat.nx_packet_alloc_errors++;
                    #endif
                }
                else
                {
                    rx_bd[index].bd_buffer_ptr = rx_bd[index].bd_nx_packet->nx_packet_prepend_ptr;
                    rx_bd[index].bd_status |= BD_ACT;
                }
            }
            
            /* Set last packet's _next and _last pointers to NULL. */
            last_packet->nx_packet_next = NULL;
            last_packet->nx_packet_last = NULL;
            
        }
        else /* Case 2: frame is still receiving, exit loop. */
        {
           break; /* Exit the receive loop. */
        }
        
        if(packet_ptr)
        {   /* Successful transfer of receive buffer(s) into NetX packet(s),
               send up the stack. */
            nx_receive(nx_record.ip_ptr, packet_ptr);
            
            #ifdef NX_STAT
            /* Increment received packet statistic. */
            nx_record.stat.rx_packets++;
            #endif
            
        }

        /* Advance the index for next reception. */
        current_rxbd_index = (current_rxbd_index + bds_in_chain) % NUM_RX_DESC;
        
        /* Move to next receive frame BD chain */
        rxbd_status = rx_bd[current_rxbd_index].bd_status;
        
        /* Make sure ACT bit is clear. */
    } while( !(rxbd_status & BD_ACT) );
    
    if(ETHER.EDRRR0 == 0)
    {
        ETHER.EDRRR0 = 1;
    }
    
    return;

}   /* nx_rx_interrupt() */


/**************************************************************************/ 
/*                                                                        */ 
/*  FUNCTION                                               RELEASE        */ 
/*                                                                        */ 
/*    nx_receive                                          PORTABLE C      */ 
/*                                                           5.0          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Scott Larson, Microsoft Corporation                                 */
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This function processes incoming packets.  This routine is          */ 
/*    called from the receive packet ISR.                                 */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    ip_ptr                                Pointer to IP protocol block  */ 
/*    packet_ptr                            Packet pointer                */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    _nx_ip_packet_receive                 IP receive packet processing  */ 
/*    _nx_ip_packet_deferred_receive                                      */ 
/*    _nx_arp_packet_deferred_receive                                     */ 
/*    _nx_rarp_packet_deferred_receive                                    */ 
/*    nx_packet_release                                                   */ 
/*                                                                        */ 
/*  CALLED BY                                                             */ 
/*                                                                        */ 
/*    Receive interrupt handler                                           */ 
/*                                                                        */ 
/*  RELEASE HISTORY                                                       */ 
/*                                                                        */ 
/*    DATE              NAME                      DESCRIPTION             */ 
/*                                                                        */ 
/*  01-01-2014       Scott Larson           Initial release               */ 
/*                                                                        */ 
/**************************************************************************/
void nx_receive(NX_IP *ip_ptr, NX_PACKET *packet_ptr)
{

USHORT    packet_type;


    /* Pickup the packet header to determine where the packet needs to be sent. */
    packet_type =  (((UINT) (*(packet_ptr -> nx_packet_prepend_ptr+12))) << 8) | 
                    ((UINT) (*(packet_ptr -> nx_packet_prepend_ptr+13)));
    
    /* Clean off the Ethernet header & padding. */
    packet_ptr -> nx_packet_prepend_ptr = packet_ptr -> nx_packet_prepend_ptr + NX_ETHERNET_SIZE + NX_PADDING_SIZE;
    /* Adjust the packet length (sub length of padding that was appended). */
    packet_ptr -> nx_packet_length = packet_ptr -> nx_packet_length - NX_ETHERNET_SIZE - NX_PADDING_SIZE;   
    
    /* Route the incoming packet according to its ethernet type. */
    switch(packet_type)
    {
        case NX_ETHERNET_IP:
        {
#ifndef NX_DRIVER_DEFERRED_PROCESSING
            _nx_ip_packet_receive(ip_ptr, packet_ptr);
#else
            _nx_ip_packet_deferred_receive(ip_ptr, packet_ptr);
#endif
            break;
        }
        
        case NX_ETHERNET_ARP:
        {
            _nx_arp_packet_deferred_receive(ip_ptr, packet_ptr);
            break;
        }
        
        case NX_ETHERNET_RARP:
        {
            _nx_rarp_packet_deferred_receive(ip_ptr, packet_ptr);
            break;
        }
        
        default:
        {
            /* Invalid ethernet header: release the packet. */
            nx_packet_release(packet_ptr);
        }
    }
    
    return;
}   /* nx_receive() */

#ifdef NX_STAT

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */ 
/*    nx_stat_init                                        PORTABLE C      */
/*                                                           5.0          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Scott Larson, Microsoft Corporation                                 */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function initializes the statistic information in preparation  */
/*    of data collection.                                                 */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    enet_statistics *p     - pointer to the statistic structure         */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    nx_record_init                                                      */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  01-01-2014       Scott Larson           Initial release               */
/*                                                                        */
/**************************************************************************/
static void nx_stat_init(struct enet_statistics *p)
{

    p->rx_packets = 0;
    p->tx_packets = 0;
    p->multicast = 0;
    p->rx_errors = 0;
    p->tx_errors = 0;
    p->rx_dscpt_out = 0;
    p->tx_dscpt_out = 0;
    p->illegal_carrier_errors = 0;
    p->phy_lsi_errors = 0;
    
    /* Detailed rx_errors. */
    p->rx_length_errors = 0; 
    p->rx_abort = 0; 
    p->rx_frame_cnt_of_errors = 0; 
    p->rx_crc_errors = 0;
    p->rx_frame_errors = 0;
    p->rx_fifo_of_errors = 0;
    
    /* Detailed tx_errors. */
    p->tx_abort = 0;
    p->tx_fifo_uf_errors = 0;
    
    
    p->nx_packet_alloc_errors = 0;
    
    return;
}   /* nx_stat_init */
#endif  /* NX_STAT */


