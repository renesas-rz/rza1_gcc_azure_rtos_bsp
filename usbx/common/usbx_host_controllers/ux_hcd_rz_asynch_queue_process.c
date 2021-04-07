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
/*    _ux_hcd_rz_asynch_queue_process                     PORTABLE C      */ 
/*                                                           5.8          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Thierry Giron                                                       */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This function process the asynchronous transactions that happened   */ 
/*    in the last frame. We read the ATL buffer and search for completed  */ 
/*    PTDs and reflect the completion in the higher level ED/TD.          */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    hcd_rz                               Pointer to RZ controller       */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    None                                                                */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    (ux_transfer_request_completion_function) Transfer req completion   */ 
/*    _ux_hcd_rz_atl_buffer_read            Read to the ATL buffer        */ 
/*    _ux_hcd_rz_ed_td_clean                Clean RZ TD                   */ 
/*    _ux_hcd_rz_hcer_register_read         Read RZ HCER read             */ 
/*    _ux_utility_memory_copy               Copy memory block             */ 
/*    _ux_utility_semaphore_put             Put semaphore                 */ 
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
VOID  _ux_hcd_rz_asynch_queue_process(UX_HCD_RZ *hcd_rz)
{
UX_RZ_ED            *ed;
ULONG               ed_index;
UX_RZ_TD            *td;
UX_RZ_TD            *next_td;
UX_TRANSFER         *transfer_request;


    /* Find the endpoint responsible for interrupt, map that to ed 
       find td that was scheduled, find out what happened on the transaction
       update the transaction. */
    
    /* Parse through the list of EDs */
    for (ed_index = 0 ; ed_index <UX_RZ_HC_ED_CNT ; ed_index++)
    {

        /* Check for an  event on the transaction. */
        if(hcd_rz -> ux_hcd_rz_ed_irq[ed_index])
        { 
        
            /* There was something for the endpoint.  Load the ED that had a transmission.  */
            ed = hcd_rz -> ux_hcd_rz_ed_ptr[ed_index];

            /* Check if the irq  was a Buffer Ready.  */
            if( hcd_rz -> ux_hcd_rz_ed_irq[ed_index] & UX_RZ_HC_ED_BRDY)

                /* Process buffer ready interrupt. */
                _ux_hcd_rz_asynch_queue_process_brdy(hcd_rz, ed);


            if( hcd_rz -> ux_hcd_rz_ed_irq[ed_index] & UX_RZ_HC_ED_NRDY )

                /* Process buffer not ready interrupt. */
                _ux_hcd_rz_asynch_queue_process_nrdy(hcd_rz, ed);


            if( hcd_rz -> ux_hcd_rz_ed_irq[ed_index] & UX_RZ_HC_ED_BEMP )

                /* Process buffer empty interrupt. */
                _ux_hcd_rz_asynch_queue_process_bemp(hcd_rz, ed);

            if( hcd_rz -> ux_hcd_rz_ed_irq[ed_index] & UX_RZ_HC_ED_EOFERR )
            {
            }

            if( hcd_rz -> ux_hcd_rz_ed_irq[ed_index] & UX_RZ_HC_ED_SIGN  )
            
                /* Process SIGN interrupt : Error on SETUP transfer. */
                _ux_hcd_rz_asynch_queue_process_sign(hcd_rz, ed);

            if( hcd_rz -> ux_hcd_rz_ed_irq[ed_index] &  UX_RZ_HC_ED_SACK )
            {
                
                /* Clear the status flag. */
                hcd_rz -> ux_hcd_rz_ed_irq[ed_index] = hcd_rz -> ux_hcd_rz_ed_irq[ed_index] & (~UX_RZ_HC_ED_SACK);


                /* Get the TD associated with this transfer. */
                td = ed -> ux_rz_ed_head_td;
 
                /* Take the TD out of the Endpoint.  */
                ed -> ux_rz_ed_head_td =  td -> ux_rz_td_next_td;
                
                /* We can now free the TD used in this PTD.  */
                td -> ux_rz_td_status =  UX_UNUSED; 
                
            }
            
            if( hcd_rz -> ux_hcd_rz_ed_irq[ed_index] &  UX_RZ_HC_ED_TIMEOUT )
            {
                
                /* Clear the status flag. */
                hcd_rz -> ux_hcd_rz_ed_irq[ed_index] = hcd_rz -> ux_hcd_rz_ed_irq[ed_index] & (~UX_RZ_HC_ED_TIMEOUT);

                /* Get the TD associated with this transfer. */
                td = ed -> ux_rz_ed_head_td;

                /* Get the transfer request associated with the TD.  */                          
                transfer_request =  td -> ux_rz_td_transfer_request;

                /* Ensure the interrupt was valid.  */
                if (td -> ux_rz_td_status & UX_RZ_TD_ACK_PENDING)
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
        
        
                    /* Set the error code to TIMEOUT.  */
                    transfer_request -> ux_transfer_request_completion_code =  UX_TRANSFER_NO_ANSWER;
                    
                    /* Wake up the class if necessary.  */
                    if (transfer_request -> ux_transfer_request_completion_function != UX_NULL)
                        transfer_request -> ux_transfer_request_completion_function(transfer_request);
            
                    /* Wake up the transfer request thread.  */
                    _ux_utility_semaphore_put(&transfer_request -> ux_transfer_request_semaphore);
                } 
            }
            

        } 

    } 

    /* Return to caller.  */
    return;
}

