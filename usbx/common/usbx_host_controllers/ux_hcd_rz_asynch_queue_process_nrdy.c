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
/*    _ux_hcd_rz_asynch_queue_process_nrdy                PORTABLE C      */ 
/*                                                           5.8          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Thierry Giron                                                       */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This function process the NRDY event that occured on a specific ED  */ 
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
VOID  _ux_hcd_rz_asynch_queue_process_nrdy(UX_HCD_RZ *hcd_rz, UX_RZ_ED  *ed)
{
UX_RZ_TD            *td;
UX_RZ_TD            *next_td;
UX_TRANSFER         *transfer_request;
ULONG               response_pid;

    /* Clear the status flag.  */
    hcd_rz -> ux_hcd_rz_ed_irq[ed -> ux_rz_pipe_index] = hcd_rz -> ux_hcd_rz_ed_irq[ed -> ux_rz_pipe_index] & (~UX_RZ_HC_ED_NRDY);

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
    
        /* Get the response PID from the DCPCTR register.  */
        response_pid =  _ux_hcd_rz_register_read(hcd_rz, UX_RZ_HC_DCPCTR);
        
        /* Set PIPE0 to NAK. The pipe stops sending tokens. */
        _ux_hcd_rz_register_clear(hcd_rz, UX_RZ_HC_DCPCTR, UX_RZ_HC_DCPCTR_PID_MASK);

    }
    else
    {
        /* Select pipe.  */
        _ux_hcd_rz_register_write( hcd_rz, UX_RZ_HC_PIPESEL, ed -> ux_rz_pipe_index);
        
        /* Get the response PID from PIPExCTR.  */
        response_pid =  _ux_hcd_rz_register_read(hcd_rz, UX_RZ_HC_PIPE1CTR + ((ed -> ux_rz_pipe_index - 1) * 2));

        /* Set PIPEx to NAK. The pipe stops sending tokens. */
        _ux_hcd_rz_register_clear(hcd_rz,  UX_RZ_HC_PIPE1CTR + ((ed -> ux_rz_pipe_index - 1) * 2), UX_RZ_HC_DCPCTR_PID_MASK);
        
    }
        
    /* Isolate the PD from the rest of the register bits.  */
    response_pid &= UX_RZ_HC_DCPCTR_PID_MASK;
    
    /* Check what response we got. Obviously a bad one.  */
    if (response_pid == UX_RZ_HC_DCPCTR_PIDSTALL || response_pid == UX_RZ_HC_DCPCTR_PIDSTALL2)
    {
        /* Set the error code to STALL.  */
        transfer_request -> ux_transfer_request_completion_code =  UX_TRANSFER_STALLED;

        /* If trace is enabled, insert this event into the trace buffer.  */
        UX_TRACE_IN_LINE_INSERT(UX_TRACE_ERROR, UX_TRANSFER_STALLED, transfer_request, 0, 0, UX_TRACE_ERRORS, 0, 0)

    }
    else
    {
    
        /* Check the number of consecutive errors.  */
        if (td -> ux_rz_td_error_count++ <= UX_RZ_TD_MAX_ERROR)
        {
        
            /* We have a special case for endpoint index 0.  */
            if(ed -> ux_rz_endpoint_index == 0) 
            
                /* Set PIPE0 to BUF. */
                _ux_hcd_rz_register_set(hcd_rz, UX_RZ_HC_DCPCTR, UX_RZ_HC_DCPCTR_PIDBUF);
            else
                /* Set PIPEx to BUF.  */
                _ux_hcd_rz_register_set(hcd_rz,  UX_RZ_HC_PIPE1CTR + ((ed -> ux_rz_pipe_index - 1) * 2), UX_RZ_HC_DCPCTR_PIDBUF);
        

            /* We are done here.  */
            return;
            
        }
        else
        {

            /* Set error to NO ANSWER.  */    
            transfer_request -> ux_transfer_request_completion_code =  UX_TRANSFER_NO_ANSWER;

        }
    }
        
    /* Clean the tds attached to the ED.  */
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

    
    /* All errors default to this section. The command has been repeated 3 times 
       and there is still a problem. The endpoint probably should be reset  */
    if (transfer_request -> ux_transfer_request_completion_function != UX_NULL)
        transfer_request -> ux_transfer_request_completion_function(transfer_request);
    _ux_utility_semaphore_put(&transfer_request -> ux_transfer_request_semaphore);
    
    /* Return to caller.  */
    return;
}

