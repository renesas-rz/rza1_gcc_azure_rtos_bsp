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
/*    _ux_hcd_rz_fifo_read                                PORTABLE C      */ 
/*                                                           5.8          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Thierry Giron                                                       */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This function reads a buffer from FIFO C D0 or D1                   */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    hcd_rz                           Pointer to RZ controller           */ 
/*    ed                                 Register to the ed               */ 
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
UINT  _ux_hcd_rz_fifo_read(UX_HCD_RZ *hcd_rz, UX_RZ_ED *ed)
{

UX_RZ_TD        *td;
ULONG           fifo_access_status;
ULONG           max_packet_size;
ULONG           payload_length;
UINT            status;
UCHAR           *payload_buffer;
ULONG           transfer_length_width;
ULONG           fifo_sel;
VOID            *fifo_addr;
ULONG           fifo_ctrl;
#ifdef UX_RZ_HCD_USE_DMA
ULONG           use_dma;
#endif


    /* We need to select the FIFO registers.  */
    switch (ed -> ux_rz_fifo_index)
    {

        case  UX_RZ_HC_FIFO_C       :

            /* Set fifo_sel and fifo_addr fields to FIFO_C */
            fifo_sel  =  UX_RZ_HC_CFIFOSEL;
            fifo_addr =  (VOID *)UX_RZ_HC_CFIFO;
            fifo_ctrl =  UX_RZ_HC_CFIFOCTR;
#ifdef UX_RZ_HCD_USE_DMA

            /* Do not use DMA for control endpoint.  */
            use_dma = UX_FALSE;
#endif
            break;

        case  UX_RZ_HC_FIFO_D0      :
        
            /* Set fifo_sel and fifo_addr fields to FIFO_D0 */
            fifo_sel  =  UX_RZ_HC_D0FIFOSEL;
            fifo_addr =  (VOID *)UX_RZ_HC_D0FIFO;
            fifo_ctrl =  UX_RZ_HC_D0FIFOCTR;
#ifdef UX_RZ_HCD_USE_DMA

            /* Use DMA for other endpoints.  */
            use_dma = UX_TRUE;

            /* Select trigger for D0.  */
            _ux_hcd_rz_dma_register_clear(hcd_rz, UX_RZ_DMA_RS5, 1<<18);
#endif
            break; 

        case  UX_RZ_HC_FIFO_D1      :

            /* Set fifo_sel and fifo_addr fields to FIFO_D1 */
            fifo_sel  =  UX_RZ_HC_D1FIFOSEL;
            fifo_addr =  (VOID *)UX_RZ_HC_D1FIFO;
            fifo_ctrl =  UX_RZ_HC_D1FIFOCTR;
#ifdef UX_RZ_HCD_USE_DMA

            /* Use DMA for other endpoints.  */
            use_dma = UX_TRUE;

            /* Select trigger for D1.  */
            _ux_hcd_rz_dma_register_set(hcd_rz, UX_RZ_DMA_RS5, 1<<18);
#endif
            break;
    }

    
    /* Get the Fifo access status for the endpoint.  */
    fifo_access_status =  _ux_hcd_rz_fifo_port_change(hcd_rz, ed, 0);
    
    /* Check Status.  */
    if (fifo_access_status == UX_ERROR)
        
        /* We have an error. Abort.  */
        return(UX_ERROR);

    /* Get the max packet size for this endpoint.  */
    max_packet_size = ed -> ux_rz_ed_endpoint -> ux_endpoint_descriptor.wMaxPacketSize;
    
    /* Isolate the payload length.  */
    payload_length = fifo_access_status & UX_RZ_HC_FIFOCTR_DTLN;
    
    /* Get the TD used for this transfer.  */
    td =  ed->ux_rz_ed_head_td;
    
    /* Save the payload length in the TD. This is needed to check for end of transfer.  */
    td -> ux_rz_td_actual_length =  payload_length;

    /* Set NAK.  */
    _ux_hcd_rz_endpoint_nak_set(hcd_rz, ed);

    /* Check for overwrite.  */
    if (td -> ux_rz_td_length < payload_length)
    {

        /* Set Status to read overflow.  */
        status = UX_RZ_HC_FIFO_READ_OVER;
        
        /* Set the payload length to the size wanted by the caller.  */
        payload_length = td -> ux_rz_td_length;

    }

    else
    {

        /* Check for short packet.  */
        if (payload_length == 0 || payload_length % max_packet_size != 0)
        
            /* We have a short packet.  */
            status = UX_RZ_HC_FIFO_READ_SHORT;

        else
        
            /* Continue reading.  */
            status = UX_RZ_HC_FIFO_READING;
        

    }

    /* Check for 0 length packet.  */
    if (payload_length == 0)

        /* Set the BCLR flag.  */
        _ux_hcd_rz_register_set(hcd_rz, fifo_ctrl, UX_RZ_HC_FIFOCTR_BCLR);
        
    else
    {

        /* Get the payload buffer address.  */
        payload_buffer =  td -> ux_rz_td_buffer;
       
        /* Select a transfer size.  */
        if ((payload_length & 0x1 ) || ( (ULONG) payload_buffer & 0x1))
            /* We use 8 bits to transfer .  */
            transfer_length_width = UX_RZ_HC_DFIFOSEL_MBW_8;  
        else if ((payload_length & 0x2 )|| ((ULONG) payload_buffer & 0x2)) 
            /* We use 16 bits to transfer.  */
            transfer_length_width = UX_RZ_HC_DFIFOSEL_MBW_16;       
        else
            /* We use 32 bits to transfer.  */
            transfer_length_width = UX_RZ_HC_DFIFOSEL_MBW_32;
#ifdef UX_RZ_HCD_USE_DMA
        if (use_dma)
        {
            
            /* Set FIFO transfer width and enable DMA request.  */
            _ux_hcd_rz_register_set(hcd_rz, fifo_sel, transfer_length_width | UX_RZ_HC_DFIFOSEL_DREQE);
            
            /* Clear DMA data size bits.  */
            _ux_hcd_rz_dma_register_clear(hcd_rz, UX_RZ_DMA_CHCFG(UX_RZ_DMA_RX_CH),
                                          UX_RZ_DMA_CHCFG_DDS_MASK | UX_RZ_DMA_CHCFG_SDS_MASK);

            /* Configure DMA channel according to the width.  */
            switch (transfer_length_width)
            {
                case UX_RZ_HC_DFIFOSEL_MBW_8: 

                    /* Configure data size to 8 bits.  */
                    _ux_hcd_rz_dma_register_set(hcd_rz, UX_RZ_DMA_CHCFG(UX_RZ_DMA_RX_CH),
                                                UX_RZ_DMA_CHCFG_DDS_8 | UX_RZ_DMA_CHCFG_SDS_8);
                    fifo_addr = (VOID*)((ULONG)fifo_addr + 3);
                    break;
                case UX_RZ_HC_DFIFOSEL_MBW_16:

                    /* Configure data size to 16 bits.  */
                    _ux_hcd_rz_dma_register_set(hcd_rz, UX_RZ_DMA_CHCFG(UX_RZ_DMA_RX_CH),
                                                UX_RZ_DMA_CHCFG_DDS_16 | UX_RZ_DMA_CHCFG_SDS_16);
                    fifo_addr = (VOID*)((ULONG)fifo_addr + 2);
                    break;

                case UX_RZ_HC_DFIFOSEL_MBW_32:

                    /* Configure data size to 32 bits.  */
                    _ux_hcd_rz_dma_register_set(hcd_rz, UX_RZ_DMA_CHCFG(UX_RZ_DMA_RX_CH),
                                                UX_RZ_DMA_CHCFG_DDS_32 | UX_RZ_DMA_CHCFG_SDS_32);

                    break;
            }

            /* Configure source and destination addresses, set transfer length.  */
            _ux_hcd_rz_dma_register_write(hcd_rz, UX_RZ_DMA_N0DA(UX_RZ_DMA_RX_CH), (ULONG)payload_buffer);
            _ux_hcd_rz_dma_register_write(hcd_rz, UX_RZ_DMA_N0SA(UX_RZ_DMA_RX_CH), (ULONG)fifo_addr);
            _ux_hcd_rz_dma_register_write(hcd_rz, UX_RZ_DMA_N0TB(UX_RZ_DMA_RX_CH), (ULONG)payload_length);
            
            /* Flush and invalidate cache.  */
            l2x0_flush_range((uint32_t)payload_buffer, (uint32_t)(payload_buffer + payload_length));
            l2x0_inv_range((uint32_t)payload_buffer, (uint32_t)(payload_buffer + payload_length));
            
            /* Update status.  */
            hcd_rz->ux_hcd_rz_dma_status = UX_RZ_DMA_STATUS_READING;

            /* Enable DMA channel.  */
            _ux_hcd_rz_dma_register_write(hcd_rz, UX_RZ_DMA_CHCTRL(UX_RZ_DMA_RX_CH), UX_RZ_DMA_CHCTRL_SETEN);
        }
        else
        {
#endif
            /* Set the width to x bits. */
            _ux_hcd_rz_register_set(hcd_rz, fifo_sel, transfer_length_width);
            
            /* Write according to the width.  */
            switch (transfer_length_width)
            {
                  case UX_RZ_HC_DFIFOSEL_MBW_32  : 
        
                    /* Write 32 bits values.  */
                    while (payload_length != 0)                  
                    {
     
                        *((ULONG *) payload_buffer) =  *((ULONG *) fifo_addr);

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
                        _ux_utility_short_put(payload_buffer, *((ULONG *) fifo_addr));

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
                        *payload_buffer =  *((UCHAR *) fifo_addr);
        
                        /* Update the payload buffer address.  */
                        payload_buffer += sizeof(UCHAR);
                    
                        /* And the remaining length.  */
                        payload_length -= sizeof(UCHAR);
                        
                    }
                    break;
            }
#ifdef UX_RZ_HCD_USE_DMA
        }
#endif
    }
        
    /* Return status about buffer transfer.  */
    return(status);
}

