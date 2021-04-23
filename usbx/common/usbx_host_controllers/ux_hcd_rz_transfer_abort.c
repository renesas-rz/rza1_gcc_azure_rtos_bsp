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


/**************************************************************************/ 
/*                                                                        */ 
/*  FUNCTION                                               RELEASE        */ 
/*                                                                        */ 
/*    _ux_hcd_rz_transfer_abort                           PORTABLE C      */ 
/*                                                           5.8          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Thierry Giron                                                       */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*     This function will abort transactions attached to a transfer       */ 
/*     request.                                                           */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    hcd_rz                              Pointer to RZ controller        */ 
/*    transfer_request                      Pointer to transfer request   */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    Completion Status                                                   */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    _ux_host_stack_delay_ms               Delay                         */ 
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
UINT  _ux_hcd_rz_transfer_abort(UX_HCD_RZ *hcd_rz, UX_TRANSFER *transfer_request)
{
                    
UX_ENDPOINT     *endpoint;
UX_RZ_ED        *ed;
UX_RZ_TD        *head_td;
UX_RZ_TD        *tail_td;
ULONG           rz_register;

TX_INTERRUPT_SAVE_AREA
    

    /* Get the pointer to the endpoint associated with the transfer request.  */
    endpoint =  (UX_ENDPOINT *) transfer_request -> ux_transfer_request_endpoint;
    
    /* From the endpoint container, get the address of the physical endpoint.  */
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

    /* Remove the reset bit in the ED.  */
    ed -> ux_rz_ed_status =  ~UX_RZ_ED_SKIP;

    /* Critical section, even ISR cannot be disrupted.  */
    TX_DISABLE

    /* Check if we are using endpoint 0.  */
    if (ed -> ux_rz_endpoint_index == 0)
    {
    
        /* Set the current fifo port on endpoint 0.  */
        _ux_hcd_rz_current_endpoint_change(hcd_rz, ed, UX_RZ_HC_CFIFOSEL_ISEL);

        /* Set the BCLR flag.  */
        _ux_hcd_rz_register_set(hcd_rz, UX_RZ_HC_CFIFOCTR, UX_RZ_HC_FIFOCTR_BCLR);

    }
    
    else
    {    
    
        /* Select pipe.  */
        _ux_hcd_rz_register_write(hcd_rz, UX_RZ_HC_PIPESEL, ed -> ux_rz_pipe_index);

        /* Read PIPE CTR for current FIFO.  */
        rz_register =  _ux_hcd_rz_register_read(hcd_rz, UX_RZ_HC_PIPE1CTR + ((ed -> ux_rz_pipe_index -1) * 2));
    
        /* Check if PID BUF on.  */
        if ((rz_register & UX_RZ_HC_DCPCTR_PID_MASK) == UX_RZ_HC_DCPCTR_PIDBUF)
        {

            /* Set PID to NAK. */
            _ux_hcd_rz_register_clear(hcd_rz, UX_RZ_HC_PIPE1CTR + ((ed -> ux_rz_pipe_index -1) * 2), UX_RZ_HC_DCPCTR_PID_MASK);

            /* Set ACLRM bit to 1 to clear the FIFO buffer. */
            _ux_hcd_rz_register_set(hcd_rz, UX_RZ_HC_PIPE1CTR + ((ed -> ux_rz_pipe_index -1) * 2), UX_RZ_HC_PIPECTR_ACLRM);

            /* Now reset the ACLRM bit. */
            _ux_hcd_rz_register_clear(hcd_rz, UX_RZ_HC_PIPE1CTR + ((ed -> ux_rz_pipe_index -1) * 2), UX_RZ_HC_PIPECTR_ACLRM);

            /* Set the current fifo port.  */
            _ux_hcd_rz_current_endpoint_change(hcd_rz, ed, 0);

            /* Read FIFO CTR for current FIFO.  */
            rz_register =  _ux_hcd_rz_register_read(hcd_rz, UX_RZ_HC_CFIFOCTR);
            
            /* Now check the status of the FRDY bit.  */
            if ((rz_register & UX_RZ_HC_FIFOCTR_FRDY) == UX_RZ_HC_FIFOCTR_FRDY)
            {
                
                /* Set the BCLR flag.  */
                _ux_hcd_rz_register_set(hcd_rz, UX_RZ_HC_CFIFOCTR, UX_RZ_HC_FIFOCTR_BCLR);

            }
        }
    }
    
    /* Free the INTs now.  */
    TX_RESTORE

    /* Return successful completion.  */
    return(UX_SUCCESS);         
}

