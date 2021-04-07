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

#include "ux_api.h"
#include "ux_hcd_rz.h"
#include "ux_utility.h"

/**************************************************************************/ 
/*                                                                        */ 
/*  FUNCTION                                               RELEASE        */ 
/*                                                                        */ 
/*    _ux_hcd_rz_asynch_queue_process_brdy                PORTABLE C      */ 
/*                                                           5.8          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Thierry Giron                                                       */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This function process the BRDY event that occured on a specific ED  */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    hcd_rz                           Pointer to RZ controller           */ 
/*    ed                                 Endpoint on which the event      */ 
/*                                       happened                         */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    None                                                                */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
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
VOID  _ux_hcd_rz_asynch_queue_process_brdy(UX_HCD_RZ *hcd_rz, UX_RZ_ED  *ed)
{
UX_RZ_TD            *td;
UX_RZ_TD            *next_td;
UX_TRANSFER         *transfer_request;
ULONG               actual_length;
ULONG               rz_register;
#ifdef UX_RZ_HCD_USE_DMA
TX_INTERRUPT_SAVE_AREA
#endif


    /* Clear the status flag.  */
    hcd_rz -> ux_hcd_rz_ed_irq[ed -> ux_rz_pipe_index] = hcd_rz -> ux_hcd_rz_ed_irq[ed -> ux_rz_pipe_index] & (~(UX_RZ_HC_ED_BRDY | UX_RZ_HC_ED_BEMP));

    /* Get the TD associated with this transfer. */
    td = ed -> ux_rz_ed_head_td;
    
    /* Ensure the interrupt was valid.  */
    if ((td -> ux_rz_td_status & UX_RZ_TD_ACK_PENDING) == 0)
        return;
    
    /* Get the transfer request associated with the TD.  */                          
    transfer_request =  td -> ux_rz_td_transfer_request;

    /* We have a special case for endpoint index 0.  */
    if(ed -> ux_rz_endpoint_index == 0) 
    {
    
        /* Set to NAK. The pipe stops sending tokens. */
        _ux_hcd_rz_register_clear(hcd_rz, UX_RZ_HC_DCPCTR, UX_RZ_HC_DCPCTR_PID_MASK);
    
        /* Check the phase.  If SETUP, simply remove the TD. */
        if (td -> ux_rz_td_status & UX_RZ_TD_SETUP_PHASE)
        {
            
            /* Take the TD out of the Endpoint.  */
            ed -> ux_rz_ed_head_td =  td -> ux_rz_td_next_td;
    
            /* We can now free the TD used in this PTD.  */
            td -> ux_rz_td_status =  UX_UNUSED;

        }
    
        else
        {
            /* Check the phase.  If DATA phase, check direction. We may need to read the
               buffer into the request_transfer buffer. */
            if (td -> ux_rz_td_status & UX_RZ_TD_DATA_PHASE)
            {
                
                /* Look at the direction.  If out, take out the TD from the link. */
                if (td -> ux_rz_td_direction ==  UX_RZ_TD_OUT)
                {
    
                    /* Take the TD out of the Endpoint.  */
                    ed -> ux_rz_ed_head_td =  td -> ux_rz_td_next_td;
        
                    /* We can now free the TD used in this PTD.  */
                    td -> ux_rz_td_status =  UX_UNUSED;
                    
                    /* Update the transitted length.  */
                    transfer_request -> ux_transfer_request_actual_length +=  td -> ux_rz_td_length;
    
                }                
                else
                {
    
                    /* This is an IN transfer.  We need to read the buffer into the
                       transfer_request memory buffer.  */
                    _ux_hcd_rz_buffer_read(hcd_rz, ed);
    
                    /* We need to read the length received.  */
                    actual_length =  td -> ux_rz_td_actual_length;
    
                    /* Update the received length.  */
                    transfer_request -> ux_transfer_request_actual_length +=  actual_length;
                    
                    /* Clear the FIFO buffer memory */
                     _ux_hcd_rz_register_set(hcd_rz, UX_RZ_HC_CFIFOCTR, UX_RZ_HC_FIFOCTR_BCLR );
                     
                    /* Take the TD out of the Endpoint.  */
                    ed -> ux_rz_ed_head_td =  td -> ux_rz_td_next_td;
        
                    /* We can now free the TD used in this PTD.  */
                    td -> ux_rz_td_status =  UX_UNUSED;
                    
                    /* Point to next TD.  */
                    td =  td -> ux_rz_td_next_td;                     
    
                    /* Check the received length and see if we have a short packet.  */
                    if (transfer_request -> ux_transfer_request_actual_length != transfer_request -> ux_transfer_request_requested_length)
                    {
    
                        /* The transfer is not complete yet, check for short packet.  */
                        if (actual_length == 0 || (actual_length % ed -> ux_rz_ed_endpoint -> ux_endpoint_descriptor.wMaxPacketSize) != 0)
                        {
    
                            /* Remove all the TDs attached to a DATA phase.  */
                            while (td != ed -> ux_rz_ed_tail_td)
                            {
    
                                /* Check the TD phase.  */
                                if (td -> ux_rz_td_status & UX_RZ_TD_DATA_PHASE)
                                {
    
                                    /* Memorize this TD.  */
                                    next_td =  td -> ux_rz_td_next_td;                     
    
                                    /* This is a DATA phase, take it out of the chain.  */
                                    ed -> ux_rz_ed_head_td =  next_td;                     
    
                                    /* We can now free the TD.  */
                                    td -> ux_rz_td_status =  UX_UNUSED;
    
                                    /* Next TD.  */
                                    td =  next_td;
                                }
                                else
                                    /* Next TD.  */
                                    td =  td -> ux_rz_td_next_td;                      
                            }
                        }
                    }
                }    
            }
            else
            {

                /* Check the phase.  If STATUS phase, wake up the pending thread.  */
                if (td -> ux_rz_td_status & UX_RZ_TD_STATUS_PHASE)
                {

                    /* Check the direction.  */
                    if (td -> ux_rz_td_direction ==  UX_RZ_TD_IN)
                    
                        /* Clear the FIFO buffer memory. */
                        _ux_hcd_rz_register_set(hcd_rz, UX_RZ_HC_CFIFOCTR, UX_RZ_HC_FIFOCTR_BCLR);
                                                
                    /* Take the TD out of the Endpoint.  */
                    ed -> ux_rz_ed_head_td =  td -> ux_rz_td_next_td;
            
                    /* We can now free the TD used in this PTD.  */
                    td -> ux_rz_td_status =  UX_UNUSED;
        
                    /* Set the transfer code to completed.  */
                    transfer_request -> ux_transfer_request_completion_code =  UX_SUCCESS;
        
                    /* Check if there is a callback.  */
                    if (transfer_request -> ux_transfer_request_completion_function != UX_NULL)
            
                        /* Yes, so execute it.  */
                        transfer_request -> ux_transfer_request_completion_function(transfer_request);
        
                    /* Wake up the thread waiting for this event to complete.  */
                    _ux_utility_semaphore_put(&transfer_request -> ux_transfer_request_semaphore);               
                }
            }
        }
    }
    else
    {

        /* Select pipe.  */
        _ux_hcd_rz_register_write( hcd_rz, UX_RZ_HC_PIPESEL, ed -> ux_rz_pipe_index);
    
        /* The toggle bit expected for the next transfer has to be memorized if the toggle
           is derived from the ED value.  */
        if ( td -> ux_rz_td_toggle ==  UX_RZ_TD_TOGGLE_FROM_ED)
        {
                        
            /* Read the PIPCxCTR register.  */
            rz_register =  _ux_hcd_rz_register_read(hcd_rz, UX_RZ_HC_PIPE1CTR + ((ed -> ux_rz_pipe_index -1) * 2));
    
            /* Check the SQMON bit from the PIPExCTR register.  */
            if (rz_register & UX_RZ_HC_PIPECTR_SQMON)
            
                /* Next phase is DATA1.  */
                ed -> ux_rz_ed_toggle |= 1;
                
            else
            
                /* Next phase is DATA0.  */
                ed -> ux_rz_ed_toggle &= ~1;
                        
        }

        /* Set PIPEx to NAK. The pipe stops sending tokens. */
        _ux_hcd_rz_register_clear(hcd_rz,  UX_RZ_HC_PIPE1CTR + ((ed -> ux_rz_pipe_index - 1) * 2), UX_RZ_HC_DCPCTR_PID_MASK);
        
        /* We are dealing with a non 0 endpoint.  Check the direction.  */
        if (td -> ux_rz_td_direction ==  UX_RZ_TD_OUT)
        {
        
            /* Add the length transmitted to the total.  */
            transfer_request -> ux_transfer_request_actual_length +=  td -> ux_rz_td_length;
                        
            /* Check the transmission is complete.  */
            if (transfer_request -> ux_transfer_request_actual_length == transfer_request -> ux_transfer_request_requested_length)
            {

                /* Set the transfer code to completed.  */
                transfer_request -> ux_transfer_request_completion_code =  UX_SUCCESS;

                /* Check if there is a callback.  */
                if (transfer_request -> ux_transfer_request_completion_function != UX_NULL)
    
                    /* Yes, so execute it.  */
                    transfer_request -> ux_transfer_request_completion_function(transfer_request);

                /* Wake up the thread waiting for this event to complete.  */
                _ux_utility_semaphore_put(&transfer_request -> ux_transfer_request_semaphore);               
            }       

            /* Take the TD out of the Endpoint.  */
            ed -> ux_rz_ed_head_td =  td -> ux_rz_td_next_td;
    
            /* We can now free the TD used in this PTD.  */
            td -> ux_rz_td_status =  UX_UNUSED;
        
        }                                      
        else
        {

            /* This is an IN transfer.  We need to read the buffer into the
               transfer_request memory buffer.  */
            _ux_hcd_rz_buffer_read(hcd_rz, ed);
            
            /* We need to read the length received.  */
            actual_length =  td -> ux_rz_td_actual_length;

            /* Update the received length.  */
            transfer_request -> ux_transfer_request_actual_length +=  actual_length;

            /* Clear the FIFO buffer memory */
             _ux_hcd_rz_register_set(hcd_rz, UX_RZ_HC_CFIFOCTR, UX_RZ_HC_FIFOCTR_BCLR );

            /* Check the received length and see if we have a short packet or if we are finished with this transfer.  */
            if ((transfer_request -> ux_transfer_request_actual_length == transfer_request -> ux_transfer_request_requested_length) ||
                actual_length == 0 || (actual_length % ed -> ux_rz_ed_endpoint -> ux_endpoint_descriptor.wMaxPacketSize) != 0)
            {
                
                /* Remove all the TDs attached.  */
                while (td != ed -> ux_rz_ed_tail_td)
                {
                                            
                    /* Memorize this TD.  */
                    next_td =  td -> ux_rz_td_next_td;                     

                    /* Take it out of the chain.  */
                    ed -> ux_rz_ed_head_td =  next_td;                     

                    /* We can now free the TD.  */
                    td -> ux_rz_td_status =  UX_UNUSED;

                    /* Next TD.  */
                    td =  next_td;
                }
                
                /* Set the transfer code to completed.  */
                transfer_request -> ux_transfer_request_completion_code =  UX_SUCCESS;

#ifdef UX_RZ_HCD_USE_DMA

                TX_DISABLE

                /* Check if DMA is idle.  */
                if (hcd_rz -> ux_hcd_rz_dma_status != UX_RZ_DMA_STATUS_IDLE)
                {

                    hcd_rz -> ux_hcd_rz_transfer_request = transfer_request;
                    TX_RESTORE
                }
                else
                {
                TX_RESTORE
#endif

                    /* Check if there is a callback.  */
                    if (transfer_request -> ux_transfer_request_completion_function != UX_NULL)
                
                        /* Yes, so execute it.  */
                        transfer_request -> ux_transfer_request_completion_function(transfer_request);

                    /* Wake up the thread waiting for this event to complete.  */
                    _ux_utility_semaphore_put(&transfer_request -> ux_transfer_request_semaphore);               
#ifdef UX_RZ_HCD_USE_DMA
                }
#endif
            }
            else
            {

                /* Take the TD out of the Endpoint.  */
                ed -> ux_rz_ed_head_td =  td -> ux_rz_td_next_td;
    
                /* We can now free the TD used in this PTD.  */
                td -> ux_rz_td_status =  UX_UNUSED;
        
            }
        }
    }

    /* Return to caller.  */
    return;
}

