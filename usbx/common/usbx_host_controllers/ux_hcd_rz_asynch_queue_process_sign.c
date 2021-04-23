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
/*    _ux_hcd_rz_asynch_queue_process_sign                PORTABLE C      */ 
/*                                                           5.8          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Thierry Giron                                                       */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This function process the SIGN event. Timeout on SETUP transfer     */ 
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
VOID  _ux_hcd_rz_asynch_queue_process_sign(UX_HCD_RZ *hcd_rz, UX_RZ_ED  *ed)
{
UX_RZ_TD            *td;
UX_RZ_TD            *next_td;
UX_TRANSFER         *transfer_request;

    /* Get the TD associated with this transfer. */
    td = ed -> ux_rz_ed_head_td;
    
    /* Ensure the interrupt was valid.  */
    if ((td -> ux_rz_td_status & UX_RZ_TD_ACK_PENDING) == 0)
        return;
 
    /* Get the transfer request associated with the TD.  */                          
    transfer_request =  td -> ux_rz_td_transfer_request;
                
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
    
    /* Set the transfer code to NO ANSWER.  */
    transfer_request -> ux_transfer_request_completion_code =  UX_TRANSFER_NO_ANSWER;

    /* Check if there is a callback.  */
    if (transfer_request -> ux_transfer_request_completion_function != UX_NULL)
    
        /* Yes, so execute it.  */
        transfer_request -> ux_transfer_request_completion_function(transfer_request);
    
    /* If trace is enabled, insert this event into the trace buffer.  */
    UX_TRACE_IN_LINE_INSERT(UX_TRACE_ERROR, UX_TRANSFER_NO_ANSWER, transfer_request, 0, 0, UX_TRACE_ERRORS, 0, 0)

    /* Wake up the thread waiting for this event to complete.  */
    _ux_utility_semaphore_put(&transfer_request -> ux_transfer_request_semaphore);               
            
    /* Return to caller.  */
    return;
}

