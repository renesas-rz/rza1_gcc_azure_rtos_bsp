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
#include "ux_host_stack.h"
#include "ux_utility.h"


/**************************************************************************/ 
/*                                                                        */ 
/*  FUNCTION                                               RELEASE        */ 
/*                                                                        */ 
/*    _ux_hcd_rz_asynchronous_endpoint_destroy            PORTABLE C      */ 
/*                                                           5.8          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Thierry Giron                                                       */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This function will destroy an asynchronous endpoint. The control    */
/*    and bulk endpoints fall into this category.                         */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    hcd_rz                                 Pointer to RZ controller     */ 
/*    endpoint                                 Pointer to endpoint        */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    Completion Status                                                   */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    _ux_utility_virtual_address              Get virtual address        */ 
/*                                                                        */ 
/*  CALLED BY                                                             */ 
/*                                                                        */ 
/*    RZ Controller Driver                                                */
/*                                                                        */ 
/*  RELEASE HISTORY                                                       */ 
/*                                                                        */ 
/*    DATE              NAME                      DESCRIPTION             */ 
/*                                                                        */ 
/*  09-03-2012     TCRG                     Initial Version 5.7           */ 
/*  06-01-2017     TCRG                     Modified comment(s),          */ 
/*                                            resulting in version 5.8    */ 
/*                                                                        */ 
/**************************************************************************/ 
UINT  _ux_hcd_rz_asynchronous_endpoint_destroy(UX_HCD_RZ *hcd_rz, UX_ENDPOINT *endpoint)
{
                
UX_RZ_ED      *ed;
UX_RZ_ED      *previous_ed;
UX_RZ_ED      *next_ed;
UX_RZ_TD      *tail_td;
UX_RZ_TD      *head_td;
    
    /* From the endpoint container fetch the RZ ED descriptor.  */
    ed =  (UX_RZ_ED *) endpoint -> ux_endpoint_ed;

    /* Check if this physical endpoint has been initialized properly!  */
    if (ed == UX_NULL)
    {

        /* Error trap. */
        _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_HCD, UX_ENDPOINT_HANDLE_UNKNOWN);

        /* If trace is enabled, insert this event into the trace buffer.  */
        UX_TRACE_IN_LINE_INSERT(UX_TRACE_ERROR, UX_ENDPOINT_HANDLE_UNKNOWN, endpoint, 0, 0, UX_TRACE_ERRORS, 0, 0)

        return(UX_ENDPOINT_HANDLE_UNKNOWN);
    }

    /* The endpoint may be active. If so, set the skip bit.  */
    ed -> ux_rz_ed_status |=  UX_RZ_ED_SKIP;
    
    /* Wait for the controller to finish the current frame processing.  */
    _ux_host_stack_delay_ms(1);

    /* Get the previous ED in the list for this ED.  */
    previous_ed =  ed -> ux_rz_ed_previous_ed;

    /* Get the next ED in the list for this ED.  */
    next_ed =  ed -> ux_rz_ed_next_ed;

    /* If the previous ED is NULL, we are at trying to remove the head ED.  */
    if (previous_ed == UX_NULL)

        /* Store the new endpoint in the Control list.  */
        hcd_rz -> ux_hcd_rz_asynch_head_ed =  next_ed;
    else

        /* The previous ED points now to the ED after the ED we are removing.  */
        previous_ed -> ux_rz_ed_next_ed =  next_ed;

    /* Reset the current ed processed and force the scheduler to restart from the beginning
       of the list.  */
    hcd_rz -> ux_hcd_rz_asynch_current_ed =  UX_NULL;       

    /* Update the previous ED pointer in the next ED if exists.  */
    if (next_ed != UX_NULL)
        next_ed -> ux_rz_ed_previous_ed =  previous_ed;

    /* Remove all the tds from this ED and leave the head and tail pointing to the dummy TD.  */
    head_td =  ed -> ux_rz_ed_head_td;
    tail_td =  ed -> ux_rz_ed_tail_td;

    /* Free all tds attached to the ED.  */
    while (head_td != tail_td)
    {

        /* Mark the current head TD as free. */
        head_td -> ux_rz_td_status =  UX_UNUSED;

        /* Update the head TD with the next TD.  */
        ed -> ux_rz_ed_head_td =  head_td -> ux_rz_td_next_td;

        /* Now the new head_td is the next TD in the chain.  */
        head_td =  ed -> ux_rz_ed_head_td;
    }

    /* We need to free the dummy TD that was attached to the ED.  */
    tail_td -> ux_rz_td_status =  UX_UNUSED;

   /* Select pipe.  */
    _ux_hcd_rz_register_write( hcd_rz, UX_RZ_HC_PIPESEL, ed -> ux_rz_pipe_index);
    
    /* Disable endpoint in the CFG register.  */
    _ux_hcd_rz_register_clear(hcd_rz, UX_RZ_HC_PIPECFG, UX_RZ_HC_PIPECFG_TYPE_MASK);

    /* Now we can safely make the ED free.  */
    ed -> ux_rz_ed_status =  UX_UNUSED;

    /* Return successful completion.  */
    return(UX_SUCCESS);         
}

