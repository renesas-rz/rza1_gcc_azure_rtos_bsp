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


/**************************************************************************/
/**************************************************************************/
/**                                                                       */ 
/** USBX Component                                                        */ 
/**                                                                       */
/**   RZ Controller Driver                                                */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/* Include necessary system files.  */

#define UX_SOURCE_CODE
#include "cache-l2x0.h"
#include "ux_api.h"
#include "ux_hcd_rz.h"


/**************************************************************************/ 
/*                                                                        */ 
/*  FUNCTION                                               RELEASE        */ 
/*                                                                        */ 
/*    _ux_hcd_rz_fofod_write                              PORTABLE C      */ 
/*                                                           5.8          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Thierry Giron                                                       */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This function writes a buffer to FIFOD0 or FIFOD1                   */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    hcd_rz                           Pointer to RZ controller           */ 
/*    ed                                    Register to the ed            */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    status                                                              */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    None                                                                */ 
/*                                                                        */ 
/*  CALLED BY                                                             */ 
/*                                                                        */ 
/*    RZ Controller Driver                                                */
/*                                                                        */ 
/*  RELEASE HISTORY                                                       */ 
/*                                                                        */ 
/*    DATE              NAME                      DESCRIPTION             */ 
/*                                                                        */ 
/*  06-01-2014     TCRG                     Initial Version 5.7           */ 
/*  06-01-2017     TCRG                     Modified comment(s),          */ 
/*                                            resulting in version 5.8    */ 
/*                                                                        */ 
/**************************************************************************/ 
UINT  _ux_hcd_rz_fifod_write(UX_HCD_RZ *hcd_rz, UX_RZ_ED *ed)
{

UX_RZ_TD        *td;
ULONG           fifo_access_status;
ULONG           max_packet_size;
ULONG           data_buffer_size;
ULONG           payload_length;
UINT            status;
UCHAR           *payload_buffer;
ULONG           fifo_sel;
#ifdef UX_RZ_HCD_USE_DMA
ULONG            fifo_addr;
#else
VOID             *fifo_addr;
#endif
ULONG           fifo_ctrl;
ULONG           transfer_length_width;
#ifndef UX_RZ_HCD_USE_DMA
ULONG           rz_register;
#endif


    /* Get the Fifo access status.  */
    fifo_access_status =  _ux_hcd_rz_fifo_port_change(hcd_rz, ed, 0);
    
    /* Check Status.  */
    if (fifo_access_status == UX_ERROR)
        
        /* We have an error. Abort.  */
        return(fifo_access_status);

    /* Get the data buffer size.  */
    data_buffer_size = _ux_hcd_rz_data_buffer_size(hcd_rz, ed);

    /* Get the max packet size for this endpoint.  */
    max_packet_size = ed -> ux_rz_ed_endpoint -> ux_endpoint_descriptor.wMaxPacketSize;
    
    /* Get the TD used for this transfer.  */
    td =  ed->ux_rz_ed_head_td;
    
    /* Check if this transfer takes more than one packet.  */
    if (td -> ux_rz_td_length <= max_packet_size) 
    {

        /* Set the payload to the TD payload length.  */
        payload_length =  td -> ux_rz_td_length;

        /* Set Status to write ends.  */
        status = UX_RZ_HC_FIFO_WRITE_END;
        
        /* Check for 0 packet.  */
        if ((td -> ux_rz_td_length == 0) || (td -> ux_rz_td_length % max_packet_size ) != 0)

            /* Set Status to write short.  */
            status = UX_RZ_HC_FIFO_WRITE_SHORT;
    }
    else
    {
        
        /* We are doing a multi write.  */
        status = UX_RZ_HC_FIFO_WRITING;
                
        /* Payload length is the buffer size.  */
        payload_length = data_buffer_size;
        
    }      
    
    /* We need to select the FIFO registers.  */
    if (ed -> ux_rz_fifo_index == UX_RZ_HC_FIFO_D0)
    {

        /* Set fifo_sel and fifo_addr fields to FIFO_D0 */
        fifo_sel  =  UX_RZ_HC_D0FIFOSEL;
#ifdef UX_RZ_HCD_USE_DMA
        fifo_addr =  UX_RZ_HC_D0FIFO;

        /* Select trigger for D0.  */
        _ux_hcd_rz_dma_register_clear(hcd_rz, UX_RZ_DMA_RS5, (1<<2));
#else
        fifo_addr =  (VOID *) UX_RZ_HC_D0FIFO;
#endif
        fifo_ctrl =  UX_RZ_HC_D0FIFOCTR;
    }
    else
    {

        /* Set fifo_sel and fifo_addr fields to FIFO_D1 */
        fifo_sel  =  UX_RZ_HC_D1FIFOSEL;
#ifdef UX_RZ_HCD_USE_DMA
        fifo_addr =  UX_RZ_HC_D1FIFO;

        /* Select trigger for D1.  */
        _ux_hcd_rz_dma_register_set(hcd_rz, UX_RZ_DMA_RS5, (1<<2));
#else
        fifo_addr =  (VOID *) UX_RZ_HC_D1FIFO;
#endif
        fifo_ctrl =  UX_RZ_HC_D1FIFOCTR;
    
    }

    /* Get the payload buffer address.  */
    payload_buffer =  td -> ux_rz_td_buffer;

#ifdef UX_RZ_HCD_USE_DMA

    /* Clear DMA data size bits.  */
    _ux_hcd_rz_dma_register_clear(hcd_rz, UX_RZ_DMA_CHCFG(UX_RZ_DMA_TX_CH),
                                  UX_RZ_DMA_CHCFG_DDS_MASK | UX_RZ_DMA_CHCFG_SDS_MASK);
#endif

    /* Select a transfer size.  */
    if ((payload_length & 0x1 ) || ( (ULONG) payload_buffer & 0x1))   
    {
        
        /* We use 8 bits to transfer.  */
        transfer_length_width = UX_RZ_HC_DFIFOSEL_MBW_8;
#ifdef UX_RZ_HCD_USE_DMA

        /* Configure data size to 8 bits.  */
        _ux_hcd_rz_dma_register_set(hcd_rz, UX_RZ_DMA_CHCFG(UX_RZ_DMA_TX_CH),
                                    UX_RZ_DMA_CHCFG_DDS_8 | UX_RZ_DMA_CHCFG_SDS_8);
        fifo_addr += 3;
#else
        fifo_addr = (VOID*)((ULONG)fifo_addr + 3);
#endif
    }
    else if ((payload_length & 0x2 )  || ( (ULONG) payload_buffer & 0x2))
    {
        
        /* We use 16 bits to transfer.  */
        transfer_length_width = UX_RZ_HC_DFIFOSEL_MBW_16;
#ifdef UX_RZ_HCD_USE_DMA

        /* Configure data size to 8 bits.  */
        _ux_hcd_rz_dma_register_set(hcd_rz, UX_RZ_DMA_CHCFG(UX_RZ_DMA_TX_CH),
                                    UX_RZ_DMA_CHCFG_DDS_16 | UX_RZ_DMA_CHCFG_SDS_16);
        fifo_addr += 2;
#else
        fifo_addr = (VOID*)((ULONG)fifo_addr + 2);
#endif
    }
    else
    {
        
        /* We use 32 bits to transfer.  */
        transfer_length_width = UX_RZ_HC_DFIFOSEL_MBW_32;

#ifdef UX_RZ_HCD_USE_DMA

        /* Configure data size to 8 bits.  */
        _ux_hcd_rz_dma_register_set(hcd_rz, UX_RZ_DMA_CHCFG(UX_RZ_DMA_TX_CH),
                                    UX_RZ_DMA_CHCFG_DDS_32 | UX_RZ_DMA_CHCFG_SDS_32);
#endif
    }
#ifdef UX_RZ_HCD_USE_DMA

    /* Set the width to x bits and DMA request bit. */
    _ux_hcd_rz_register_set(hcd_rz, fifo_sel, transfer_length_width | UX_RZ_HC_DFIFOSEL_DREQE);
    
    /* Configure source and destination addresses, set transfer length.  */
    _ux_hcd_rz_dma_register_write(hcd_rz, UX_RZ_DMA_N0SA(UX_RZ_DMA_TX_CH), (ULONG)payload_buffer);
    _ux_hcd_rz_dma_register_write(hcd_rz, UX_RZ_DMA_N0DA(UX_RZ_DMA_TX_CH), (ULONG)fifo_addr);
    _ux_hcd_rz_dma_register_write(hcd_rz, UX_RZ_DMA_N0TB(UX_RZ_DMA_TX_CH), (ULONG)payload_length);

    /* Flush cache.  */
    l2x0_flush_range((uint32_t)payload_buffer, (uint32_t)(payload_buffer + payload_length));

    /* Check if we have a short packet.  */
    if (payload_length % max_packet_size)
    {

        /* Update status to indicate a short packet.  */
        hcd_rz -> ux_hcd_rz_dma_status = UX_RZ_DMA_STATUS_WRITING_SHORT;
    }
    else
    {

        /* Update status.  */
        hcd_rz -> ux_hcd_rz_dma_status = UX_RZ_DMA_STATUS_WRITING;
    }

    /* Save the FIFO control register.  */
    hcd_rz -> ux_hcd_rz_current_fifo_ctrl = fifo_ctrl;

    /* Enable DMA channel.  */
    _ux_hcd_rz_dma_register_write(hcd_rz, UX_RZ_DMA_CHCTRL(UX_RZ_DMA_TX_CH), UX_RZ_DMA_CHCTRL_SETEN);

#else
    
    /* Set the width to x bits. */
    _ux_hcd_rz_register_set(hcd_rz, fifo_sel, transfer_length_width);
    
    /* Write according to the width.  */
    switch (transfer_length_width)
    {
    
        case UX_RZ_HC_DFIFOSEL_MBW_32  : 

            /* Write 32 bits values.  */
            while (payload_length != 0)                  
            {              
                /* Read the 32 bit value.  */
                *((ULONG *)fifo_addr) =  *((ULONG *)(payload_buffer));
                
                /* Update the payload buffer address.  */
                payload_buffer += sizeof(ULONG);
                
                /* And the remaining length.  */
                payload_length -= sizeof(ULONG);
            }
            break;

        case UX_RZ_HC_DFIFOSEL_MBW_16  : 

            /* Write 16 bits values.  */
            while (payload_length != 0)                  
            {
                
                /* Read the 16 bit value.  */
                *((USHORT *)fifo_addr) =  _ux_utility_short_get(payload_buffer);

                /* Update the payload buffer address.  */
                payload_buffer += sizeof(USHORT);
            
                /* And the remaining length.  */
                payload_length -= sizeof(USHORT);
            }
            break;
            

        case UX_RZ_HC_DFIFOSEL_MBW_8  : 

            /* Write 16 bits values.  */
            while (payload_length != 0)                  
            {
                
                /* Read the 8 bit value.  */
                *((UCHAR *)fifo_addr) =  *payload_buffer;

                /* Update the payload buffer address.  */
                payload_buffer += sizeof(UCHAR);
            
                /* And the remaining length.  */
                payload_length -= sizeof(UCHAR);
                
            }
            break;
    }
    
    /* Check status. If we have a short packet, we need to set 
       the BVAL flag  in the Fifo controller.  */
    if (status != UX_RZ_HC_FIFO_WRITING)
    {

        /* Read the current FIFO control register.  */
        rz_register = _ux_hcd_rz_register_read(hcd_rz, fifo_ctrl);

        /* Check if the BVAL bit is already set.  */
        if ((rz_register & UX_RZ_HC_FIFOCTR_BVAL) == 0)

            /* No so set it.  This will enable the Buffer Memory Valid flag.  
               This bit is set to 1 when data has been written to the FIFO and
               this is a short packet or zero packet or a full packet but not the
               end of the transmission.  */
            _ux_hcd_rz_register_set(hcd_rz, fifo_ctrl, UX_RZ_HC_FIFOCTR_BVAL);
    }       

#endif

    /* Return status about buffer transfer.  */
    return(status);
}

