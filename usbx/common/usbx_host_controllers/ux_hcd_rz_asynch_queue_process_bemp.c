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
/*    _ux_hcd_rz_asynch_queue_process_bemp                PORTABLE C      */ 
/*                                                           5.8          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Thierry Giron                                                       */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This function process the BEMP event that occured on a specific ED  */ 
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
VOID  _ux_hcd_rz_asynch_queue_process_bemp(UX_HCD_RZ *hcd_rz, UX_RZ_ED *ed)
{
UX_RZ_TD            *td;
UX_RZ_TD            *next_td;
UX_TRANSFER         *transfer_request;
ULONG               inbuf_flag;
ULONG               response_pid;
ULONG               rz_register;
ULONG               actual_length;

    /* Clear the status flag.  */
    hcd_rz -> ux_hcd_rz_ed_irq[ed -> ux_rz_pipe_index] = hcd_rz -> ux_hcd_rz_ed_irq[ed -> ux_rz_pipe_index] & (~(UX_RZ_HC_ED_BRDY | UX_RZ_HC_ED_BEMP));

    /* Get the TD associated with this transfer. */
    td = ed -> ux_rz_ed_head_td;
    
    /* Ensure the interrupt was valid.  */
    if ((td -> ux_rz_td_status & UX_RZ_TD_ACK_PENDING) == 0)
        return;
        
    /* Select pipe.  */
    _ux_hcd_rz_register_write( hcd_rz, UX_RZ_HC_PIPESEL, ed -> ux_rz_pipe_index);
    
    /* Get the transfer request associated with the TD.  */                          
    transfer_request =  td -> ux_rz_td_transfer_request;

    /* We have a special case for endpoint index 0.  */
    if(ed -> ux_rz_endpoint_index == 0) 
    {
    
        /* Set PIPE0 to NAK. The pipe stops sending tokens. */
        _ux_hcd_rz_register_clear(hcd_rz, UX_RZ_HC_DCPCTR, UX_RZ_HC_DCPCTR_PID_MASK);
        
        /* Get the response PID from the DCPCTR register.  */
        response_pid =  _ux_hcd_rz_register_read(hcd_rz, UX_RZ_HC_DCPCTR);

    }
    else
    {
    
        /* Set PIPEx to NAK. The pipe stops sending tokens. */
        _ux_hcd_rz_register_clear(hcd_rz,  UX_RZ_HC_PIPE1CTR + ((ed -> ux_rz_pipe_index - 1) * 2), UX_RZ_HC_DCPCTR_PID_MASK);
        
        /* Get the response PID from PIPExCTR.  */
        response_pid =  _ux_hcd_rz_register_read(hcd_rz, UX_RZ_HC_PIPE1CTR + ((ed -> ux_rz_pipe_index - 1) * 2));

    }
                
    /* Isolate the PD from the rest of the register bits.  */
    response_pid &= UX_RZ_HC_DCPCTR_PID_MASK;
    
    /* Check what response we got. It is either an error or the end of transmission.  */
    if (response_pid == UX_RZ_HC_DCPCTR_PIDSTALL || response_pid == UX_RZ_HC_DCPCTR_PIDSTALL2)
    {
        /* Set the error code to STALL.  */
        transfer_request -> ux_transfer_request_completion_code =  UX_TRANSFER_STALLED;

        /* If trace is enabled, insert this event into the trace buffer.  */
        UX_TRACE_IN_LINE_INSERT(UX_TRACE_ERROR, UX_TRANSFER_STALLED, transfer_request, 0, 0, UX_TRACE_ERRORS, 0, 0)

    }
    else
    {
        /* Check the INBUFM flag.  */
        if(ed -> ux_rz_endpoint_index == 0) 

            /* Irrelevant for Control endpoint.  */
            inbuf_flag =  0;

        else
                    
            /* Get the INBUFM flag from the PIPExCTR register.  */
            inbuf_flag =  _ux_hcd_rz_register_read(hcd_rz, UX_RZ_HC_PIPE1CTR + ((ed -> ux_rz_pipe_index - 1) * 2)) & UX_RZ_HC_PIPECTR_INBUFM;

        /* If the INBUFM flag is 0, no data to be transmitted, turn off BEMP ints.  */
        if (inbuf_flag == 0)
        
            /* Disable the Buffer empty interrupt.  */
            _ux_hcd_rz_buffer_empty_interrupt(hcd_rz, ed, UX_RZ_DISABLE);

        /* Set the error code to SUCCESS.  */
        transfer_request -> ux_transfer_request_completion_code =  UX_SUCCESS;
    }

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

    /* Adjust the length transmitted.  */
    actual_length =  td -> ux_rz_td_length;

    /* Update the transmitted length.  */
    transfer_request -> ux_transfer_request_actual_length +=  actual_length;

    /* Check the transmitted length and see if we have a short packet or if we are finished with this transfer.  */
    if ((transfer_request -> ux_transfer_request_completion_code |= UX_SUCCESS) || (transfer_request -> ux_transfer_request_actual_length == transfer_request -> ux_transfer_request_requested_length) ||
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

        /* All errors\transfer completion default to this section. The command has been repeated 3 times 
           and there is still a problem. The endpoint probably should be reset.  .  */
        if (transfer_request -> ux_transfer_request_completion_function != UX_NULL)
            transfer_request -> ux_transfer_request_completion_function(transfer_request);
    
        /* Wake up the transfer request thread.  */
        _ux_utility_semaphore_put(&transfer_request -> ux_transfer_request_semaphore);
        
    }
    else
    {

        /* Take the TD out of the Endpoint.  */
        ed -> ux_rz_ed_head_td =  td -> ux_rz_td_next_td;
    
        /* We can now free the TD used in this PTD.  */
        td -> ux_rz_td_status =  UX_UNUSED;
    
    }

}

