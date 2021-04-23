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
/*    _ux_hcd_rz_control_td_add                           PORTABLE C      */ 
/*                                                           5.8          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Thierry Giron                                                       */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*     This function adds a transfer descriptor to an Bulk or INT ED      */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    hcd_rz                              Pointer to RZ controller        */ 
/*    ed                                    Pointer to ED                 */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    Completion Status                                                   */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    _ux_utility_memory_copy               Copy memory block             */ 
/*    _ux_utility_memory_set                Set memory block              */ 
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
UINT  _ux_hcd_rz_bulk_int_td_add(UX_HCD_RZ *hcd_rz, UX_RZ_ED *ed)
{

UX_RZ_TD        *current_td;
UINT            status;
    
    /* Get transmit decriptors. */
    current_td =  ed->ux_rz_ed_head_td;
    
    /* Set TD into response pending state. */
    current_td -> ux_rz_td_status |=  UX_RZ_TD_ACK_PENDING;
    
    /* Set PIPEx FIFO in in status. */
    _ux_hcd_rz_register_write(hcd_rz, UX_RZ_HC_PIPESEL, ed -> ux_rz_pipe_index);

    /* Set PID to NAK. */
    _ux_hcd_rz_register_clear(hcd_rz, UX_RZ_HC_PIPE1CTR + ((ed -> ux_rz_pipe_index -1) * 2), UX_RZ_HC_PIPECTR_PID_MASK);
        
    /* Clear the BRDY and BEMP status for this pipe.  */
    _ux_hcd_rz_register_clear(hcd_rz, UX_RZ_HC_BEMPSTS, (1 << ed -> ux_rz_pipe_index));
    _ux_hcd_rz_register_clear(hcd_rz, UX_RZ_HC_BRDYSTS, (1 << ed -> ux_rz_pipe_index));
         
    /* Check the direction of the transfer. */
    if (current_td -> ux_rz_td_direction == UX_RZ_TD_IN)
    {
            
        /* Set DATA0-DATA1 toggle. We take this value from the ED for bulk and interrupt. */
        if (ed  -> ux_rz_ed_toggle == 0)
        
            /* Set Toggle to DATA0.  */
            _ux_hcd_rz_register_set(hcd_rz, UX_RZ_HC_PIPE1CTR + ((ed -> ux_rz_pipe_index -1) * 2),UX_RZ_HC_PIPECTR_DATA0);
        
        else

            /* Set Toggle to DATA1.  */
            _ux_hcd_rz_register_set(hcd_rz, UX_RZ_HC_PIPE1CTR + ((ed -> ux_rz_pipe_index -1) * 2),UX_RZ_HC_PIPECTR_DATA1);

        /* Set the current endpoint fifo.  */
        _ux_hcd_rz_current_endpoint_change(hcd_rz, ed, 0);
    
        /* Enable the BRDY interrupt.  */
        _ux_hcd_rz_buffer_ready_interrupt(hcd_rz, ed, UX_RZ_ENABLE);
           
        /* Enable the NRDY interrupt.  */
        _ux_hcd_rz_buffer_notready_interrupt(hcd_rz, ed, UX_RZ_ENABLE);
    
        /* Start transmission - set PID to NAK  then  set PID to BUF. */
        _ux_hcd_rz_register_set(hcd_rz, UX_RZ_HC_PIPE1CTR + ((ed -> ux_rz_pipe_index -1) * 2), UX_RZ_HC_PIPECTR_PIDBUF);
    
        /* Set the ED in the ed array.  */
        hcd_rz -> ux_hcd_rz_ed_ptr[ed -> ux_rz_pipe_index] = ed;
    }       
    
    else 
    {
                            
        /* Must be an OUT now.  */                              
        if ( current_td -> ux_rz_td_direction == UX_RZ_TD_OUT )
        {

           /* Set DATA0-DATA1 toggle. We take this value from the ED for bulk and interrupt. */
            if (ed  -> ux_rz_ed_toggle == 0)

                /* Set Toggle to DATA0.  */
                _ux_hcd_rz_register_set(hcd_rz, UX_RZ_HC_PIPE1CTR + ((ed -> ux_rz_pipe_index -1) * 2),UX_RZ_HC_PIPECTR_DATA0);
        
            else

                /* Set Toggle to DATA1.  */
                _ux_hcd_rz_register_set(hcd_rz, UX_RZ_HC_PIPE1CTR + ((ed -> ux_rz_pipe_index -1) * 2),UX_RZ_HC_PIPECTR_DATA1);
            
            /* Write the buffer to the pipe.  */
            status =  _ux_hcd_rz_buffer_write(hcd_rz, ed); 

            /* Check status.  */
            if (status != UX_ERROR)         

                /* If no error, we can set the pipe PID_BUF flag.  */
                _ux_hcd_rz_register_set(hcd_rz, UX_RZ_HC_PIPE1CTR + ((ed -> ux_rz_pipe_index -1) * 2), UX_RZ_HC_PIPECTR_PIDBUF);
    
        }
    }        

    /* Return successful completion.  */
    return(UX_SUCCESS);
}

