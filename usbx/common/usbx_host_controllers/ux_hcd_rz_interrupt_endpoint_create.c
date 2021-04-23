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
/*    _ux_hcd_rz_interrupt_endpoint_create                PORTABLE C      */ 
/*                                                           5.8          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Thierry Giron                                                       */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This function will create an interrupt endpoint. The interrupt      */ 
/*    endpoint has an interval of operation from 1 to 255. The RZ         */
/*    has no hardware scheduler but we still build an interrupt tree      */ 
/*    similar to the OHCI controller.                                     */
/*                                                                        */
/*    This routine will match the best interval for the RZ                */ 
/*    hardware. It will also determine the best node to hook the          */ 
/*    endpoint based on the load that already exists on the horizontal    */ 
/*    ED chain.                                                           */
/*                                                                        */
/*    For the ones curious about this coding. The tricky part is to       */
/*    understand how the interrupt matrix is constructed. We have used    */ 
/*    eds with the skip bit on to build a frame of anchor eds. Each ED    */ 
/*    creates a node for an appropriate combination of interval frequency */ 
/*    in the list.                                                        */ 
/*                                                                        */
/*    After obtaining a pointer to the list with the lowest traffic, we   */
/*    traverse the list from the highest interval until we reach the      */ 
/*    interval required. At that node, we anchor our real ED to the node  */ 
/*    and link the ED that was attached to the node to our ED.            */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    hcd_rz                              Pointer to RZ Controller        */ 
/*    endpoint                              Pointer to endpoint           */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    Completion Status                                                   */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    _ux_hcd_rz_ed_obtain                Obtain ED                       */ 
/*    _ux_hcd_rz_regular_td_obtain        Obtain regular TD               */ 
/*    _ux_hcd_rz_least_traffic_list_get   Get least traffic list          */ 
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
UINT  _ux_hcd_rz_interrupt_endpoint_create(UX_HCD_RZ *hcd_rz, UX_ENDPOINT *endpoint)
{

UX_RZ_ED            *ed;
UX_RZ_ED            *ed_list;
UX_RZ_ED            *next_ed;
UX_RZ_TD            *td;
UINT                interval;
UINT                interval_index;
UINT                interval_rz;
ULONG               buffer_index;
ULONG               pipe_index ;
ULONG               current_pipe_index ;
ULONG               endpoint_number;
ULONG               endpoint_direction;
ULONG               pipe_type;
ULONG               device_address;
ULONG               max_packet_size;
TX_INTERRUPT_SAVE_AREA

    /* Set first buffer number. */
    buffer_index = 4;
    pipe_index = 0;
    
    /* Browse Pipes from 6 to 10 which can be bulk.  */
    for (current_pipe_index = 6; current_pipe_index < 11; current_pipe_index++)
    {
    
        /* Select pipe 6 - 9.  */
        _ux_hcd_rz_register_write( hcd_rz, UX_RZ_HC_PIPESEL, current_pipe_index);
       
        /* Check whether pipe is in use.  */
        pipe_type =  ( (UX_RZ_HC_PIPECFG_TYPE_MASK & _ux_hcd_rz_register_read(hcd_rz, UX_RZ_HC_PIPECFG)) >> 14 ) ;
    
        /* Not in use, choose this one.  */
        if (pipe_type == 0)
        {

            /* Set the pipe index.  */
            pipe_index = current_pipe_index;

            /* We are done with pipe selection.  */
            break;
        }            

        /* Next buffer.  */
        buffer_index++;

    }

    /* Check if we found an available pipe for interrupt endpoint.  */
    if (pipe_index == 0)
        return(UX_ERROR);

    /* We need to take into account the nature of the HCD to define the max size
       of any transfer in the transfer request.  */
    endpoint -> ux_endpoint_transfer_request.ux_transfer_request_maximum_length =  UX_RZ_MAX_BULK_PAYLOAD;
    
    /* Obtain a ED for this new endpoint. This ED will live as long as
       the endpoint is active and will be the container for the tds.  */
    ed =  _ux_hcd_rz_ed_obtain(hcd_rz);
    if (ed == UX_NULL)
        return(UX_NO_ED_AVAILABLE);

    /* Store the pipe index.  */
    ed -> ux_rz_pipe_index = pipe_index;
    
    /* Obtain a dummy TD for terminating the ED transfer chain.  */
    td =  _ux_hcd_rz_regular_td_obtain(hcd_rz);
    if (td == UX_NULL)
    {

        ed -> ux_rz_ed_status =  UX_UNUSED;
        return(UX_NO_TD_AVAILABLE);
    }

    /* Attach the ED to the endpoint container.  */
    endpoint -> ux_endpoint_ed =  (VOID *)ed;
    
    /* Now do the opposite, attach the ED container to the physical ED.  */
    ed -> ux_rz_ed_endpoint =  endpoint;

    /* Hook the TD to both the tail and head of the ED.  */
    ed -> ux_rz_ed_tail_td =  td;
    ed -> ux_rz_ed_head_td =  td;

    /* Get the list index with the least traffic.  */
    ed_list =  _ux_hcd_rz_least_traffic_list_get(hcd_rz);
    
    /* Get the interval for the endpoint and match it to a RZ list. We match anything 
       that is > 32ms to the 32ms interval list, the 32ms list is list 0, 16ms list is 1...
       the 1ms list is number 5.  */
    interval =          endpoint -> ux_endpoint_descriptor.bInterval;
    interval_index =  0x10;
    interval_rz =   1;

    /* Do a sanity check if the frequency is 0. That should not happen, so treat it as 1.  */
    if (interval == 0)
    {

        interval =  1;
    }

    /* If the frequency is beyond the OHCI framework, make it the maximum of 32.  */
    if (interval >= 32)
    {

        interval_rz =  0;
    }
    else
    {

        /* We parse the interval from the high bits. This gives us the first power of 2 entry in the tree.  */
        while (interval_index != 0)
        {

            /* When we find the first bit of the interval the current value of interval_rz is set to the the list index.  */
            if (interval & interval_index)
                break;
                
            /* Go down the tree one entry.  */
            interval_rz++;
            
            /* And shift the bit of the device interval to check.  */
            interval_index =  interval_index >> 1;
        }
    }

    /* Now we need to scan the list of eds from the lowest load entry until we reach 
       the appropriate interval node. The depth index is the interval_rz value 
       and the 1st entry is pointed by the ED list entry.  */
    while (interval_rz--)
    {

        ed_list =  ed_list -> ux_rz_ed_next_ed;
        while (!(ed_list -> ux_rz_ed_status & UX_RZ_ED_STATIC))
            ed_list =  ed_list -> ux_rz_ed_next_ed;
    }       

    /* We found the node entry of the ED pointer that will be the anchor for this interrupt 
       endpoint. Now we attach this endpoint to the anchor and rebuild the chain .  */
    next_ed =                               ed_list -> ux_rz_ed_next_ed;

    /* Check for end of tree which happens for devices with interval of 1. In this case
       there might not be a next_ed.  */
    if (next_ed != UX_NULL)
        next_ed -> ux_rz_ed_previous_ed =  ed;

    ed -> ux_rz_ed_next_ed =           next_ed;
    ed -> ux_rz_ed_previous_ed =       ed_list;
    ed_list -> ux_rz_ed_next_ed =      ed;

    /* Configure the  endpoint  hardware, pick up buffer size. Number is fixed. */
    ed -> ux_rz_buffer_number =  buffer_index;
    ed -> ux_rz_buffer_size =  0;
    
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
    _ux_hcd_rz_register_set(hcd_rz, UX_RZ_HC_PIPE1CTR + (ed -> ux_rz_pipe_index * 2), UX_RZ_HC_DCPCTR_PIDNAK);
     
    /* Critical section, even ISR cannot be disrupted.  */
    TX_DISABLE

    /* Select the endpoint register to map all registers.  */
    _ux_hcd_rz_register_write(hcd_rz, UX_RZ_HC_PIPESEL, pipe_index);

    /* Set Pipe configuration registers.  */
    _ux_hcd_rz_register_write(hcd_rz, UX_RZ_HC_PIPECFG, (UX_RZ_HC_PIPECFG_TYPE_INTERRUPT | UX_RZ_HC_PIPECFG_SHTNAK | endpoint_direction | endpoint_number));

    /* Get the device address. It needs to be stored in the PIPEMAXP register.  */
    device_address = ed -> ux_rz_ed_endpoint -> ux_endpoint_device -> ux_device_address << UX_RZ_HC_PIPEMAXP_DEVSEL_SHIFT;

    /* Get the max packet size for this endpoint.  */
    max_packet_size = ed -> ux_rz_ed_endpoint -> ux_endpoint_descriptor.wMaxPacketSize;
    
    /* Inititalize the PIPEMAX register.  */
    _ux_hcd_rz_register_write(hcd_rz, UX_RZ_HC_PIPEMAXP, device_address | max_packet_size);
    
    /* Free the INTs now.  */
    TX_RESTORE

    /* For bulk and interrupt, use FIFO_C.  */
    ed -> ux_rz_fifo_index =  UX_RZ_HC_FIFO_C; 
    
    /* Memorise the endpoint in the HCD structure. */
    hcd_rz -> ux_hcd_rz_ed_ptr[ed -> ux_rz_pipe_index] = ed;

    /* There is activity in the periodic tree, the scheduler has to be active all the time.  */
    hcd_rz -> ux_hcd_rz_periodic_scheduler_active++;

    /* Return successful completion.  */
    return(UX_SUCCESS);         
}

