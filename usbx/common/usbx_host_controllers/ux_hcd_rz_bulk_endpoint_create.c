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


/**************************************************************************/ 
/*                                                                        */ 
/*  FUNCTION                                               RELEASE        */ 
/*                                                                        */ 
/*    _ux_hcd_rz_bulk_endpoint_create                     PORTABLE C      */ 
/*                                                           5.8          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Thierry Giron                                                       */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This function will create a bulk endpoint.                          */
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    hcd_rz                              Pointer to RZ controller        */ 
/*    endpoint                              Pointer to endpoint           */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    Completion Status                                                   */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    _ux_hcd_rz_ed_obtain                Obtain RZ ED                    */ 
/*    _ux_hcd_rz_regular_td_obtain        Obtain RZ regular TD            */ 
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
UINT  _ux_hcd_rz_bulk_endpoint_create(UX_HCD_RZ *hcd_rz, UX_ENDPOINT *endpoint)
{

UX_RZ_ED            *ed;
UX_RZ_ED            *head_ed;
UX_RZ_TD            *td;
ULONG               pipe_index ;
ULONG               current_pipe_index ;
ULONG               endpoint_number;
ULONG               endpoint_direction;
ULONG               pipe_type;
ULONG               device_address;
ULONG               max_packet_size;

TX_INTERRUPT_SAVE_AREA

    /* Obtain the first available pipe. */
    current_pipe_index = 1;
    pipe_index = 0;
    
    while( ( current_pipe_index <= 5 ) && ( pipe_index ==  0 ) )
    {
    
        /* Select pipe 1 - 5.  */
        _ux_hcd_rz_register_write( hcd_rz, UX_RZ_HC_PIPESEL, current_pipe_index);
       
        /* Check whether pipe is in use.  */
        pipe_type =  ( (UX_RZ_HC_PIPECFG_TYPE_MASK & _ux_hcd_rz_register_read(hcd_rz, UX_RZ_HC_PIPECFG)) >> 14 ) ;
    
        /* Not in use, choose this one.  */
        if (pipe_type == 0)
            pipe_index = current_pipe_index;  
            
        /* Increment the current index.  */
        current_pipe_index++;
    }

    /* Check if we found an available pipe for bulk endpoint.  */
    if (pipe_index == 0)
        return(UX_ERROR);

    /* We need to take into account the nature of the HCD to define the max size
       of any transfer in the transfer request.  */
    endpoint -> ux_endpoint_transfer_request.ux_transfer_request_maximum_length =  UX_RZ_MAX_BULK_PAYLOAD;
    
    /* Obtain a ED for this new endpoint. This ED will live as long as the endpoint is active 
       and will be the container for the tds.  */
    ed =  _ux_hcd_rz_ed_obtain(hcd_rz);
    if (ed == UX_NULL)
        return(UX_NO_ED_AVAILABLE);

    /* Store the pipe index.  */
    ed -> ux_rz_pipe_index = pipe_index;
    
    /* Obtain a dummy TD for terminating the ED transfer chain.  */
    td =  _ux_hcd_rz_regular_td_obtain(hcd_rz);
    if (td == UX_NULL)
    {

        /* No more TD.  */
        ed -> ux_rz_ed_status =  UX_UNUSED;
        return(UX_NO_TD_AVAILABLE);
    }

    /* Attach the ED to the endpoint container.  */
    endpoint -> ux_endpoint_ed =  (VOID *) ed;

    /* Now do the opposite, attach the ED container to the physical ED.  */
    ed -> ux_rz_ed_endpoint =  endpoint;
    
    /* Hook the TD to both the tail and head of the ED.  */
    ed -> ux_rz_ed_tail_td =  td;
    ed -> ux_rz_ed_head_td =  td;
    
    /* Attach this ED to the bulk list.  */
    head_ed =  hcd_rz -> ux_hcd_rz_asynch_head_ed;
    ed -> ux_rz_ed_next_ed =  head_ed;
    hcd_rz -> ux_hcd_rz_asynch_head_ed =  ed;

    /* Build the back chaining pointer. The previous head ED needs to know about the
       inserted ED. */
    if (head_ed != UX_NULL)
        head_ed -> ux_rz_ed_previous_ed =  ed;
        
    /* Configure the  endpoint  hardware, pick up buffer size & number. */
    ed -> ux_rz_buffer_size =  UX_RZ_MAX_BULK_PAYLOAD / UX_RZ_HC_MAX_BUF_SIZE;
    
    /* Get the endpoint number.  */
    endpoint_number =  (endpoint -> ux_endpoint_descriptor.bEndpointAddress & 0xF ) ;
    
    /* Store the endpoint index for quick retrieval.  */
    ed -> ux_rz_endpoint_index = endpoint_number;
    
    /* Get the endpoint direction.  */
    endpoint_direction = endpoint -> ux_endpoint_descriptor.bEndpointAddress & UX_ENDPOINT_DIRECTION;
    
    /* Check if IN or OUT.  */
    if (endpoint_direction == UX_ENDPOINT_IN)
    
        /* Reset the endpoint direction as bit 4 of PIPECFG is 0 for IN.  */
        endpoint_direction = 0;
        
    else
            
        /* Set the direction to OUT.  */
        endpoint_direction = UX_RZ_HC_PIPECFG_DIR;
     
    /* Set PID to NAK. */
    _ux_hcd_rz_register_clear(hcd_rz, UX_RZ_HC_PIPE1CTR + (ed -> ux_rz_pipe_index * 2), UX_RZ_HC_DCPCTR_PID_MASK);
     
    /* Critical section, even ISR cannot be disrupted.  */
    TX_DISABLE

    /* Select the endpoint register to map all registers.  */
    _ux_hcd_rz_register_write(hcd_rz, UX_RZ_HC_PIPESEL, pipe_index);

    /* Set Pipe configuration registers.  */
    _ux_hcd_rz_register_write(hcd_rz, UX_RZ_HC_PIPECFG, (UX_RZ_HC_PIPECFG_TYPE_BULK | UX_RZ_HC_PIPECFG_SHTNAK | endpoint_direction | endpoint_number));

    /* Get the device address. It needs to be stored in the PIPEMAXP register.  */
    device_address = ed -> ux_rz_ed_endpoint -> ux_endpoint_device -> ux_device_address << UX_RZ_HC_PIPEMAXP_DEVSEL_SHIFT;

    /* Get the max packet size for this endpoint.  */
    max_packet_size = ed -> ux_rz_ed_endpoint -> ux_endpoint_descriptor.wMaxPacketSize;
    
    /* Inititalize the PIPEMAX register.  */
    _ux_hcd_rz_register_write(hcd_rz, UX_RZ_HC_PIPEMAXP, device_address | max_packet_size);

    /* Free the INTs now.  */
    TX_RESTORE

    /* For bulk and interrupt, use FIFO_D0.  */
    ed -> ux_rz_fifo_index =  UX_RZ_HC_FIFO_D0; 
    
    /* Memorise the endpoint in the HCD structure. */
    hcd_rz -> ux_hcd_rz_ed_ptr[ed -> ux_rz_pipe_index] = ed;

    /* Return successful completion.  */
    return(UX_SUCCESS);         
}

