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
/*    _ux_hcd_rz_request_interrupt_transfer               PORTABLE C      */ 
/*                                                           5.8          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Thierry Giron                                                       */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*     This function performs an interrupt transfer request. An interrupt */ 
/*     transfer can only be as large as the MaxpacketField in the         */ 
/*     endpoint descriptor. This was verified at the USB layer and does   */ 
/*     not need to be reverified here.                                    */ 
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
/*    _ux_hcd_rz_regular_td_obtain        Obtain regular TD               */ 
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
UINT  _ux_hcd_rz_request_interrupt_transfer(UX_HCD_RZ *hcd_rz, UX_TRANSFER *transfer_request)
{

UX_ENDPOINT     *endpoint;
UX_RZ_ED        *ed;
UX_RZ_TD        *data_td;
UX_RZ_TD        *tail_td;

    /* Get the pointer to the Endpoint.  */
    endpoint =  (UX_ENDPOINT *) transfer_request -> ux_transfer_request_endpoint;

    /* Now get the physical ED attached to this endpoint.  */
    ed =  endpoint -> ux_endpoint_ed;

    /* Use the TD pointer by ed -> tail for the first TD of this transfer
        and chain from this one on.  */
    data_td =  ed -> ux_rz_ed_tail_td;

    /* Set the direction of the transfer.  */
    if ((transfer_request -> ux_transfer_request_type & UX_REQUEST_DIRECTION) == UX_REQUEST_IN)
        data_td -> ux_rz_td_direction =  UX_RZ_TD_IN;
    else
        data_td -> ux_rz_td_direction =  UX_RZ_TD_OUT;

    /* Mark the TD with the DATA phase.  */
    data_td -> ux_rz_td_status |=  UX_RZ_TD_DATA_PHASE;

    /* The Toggle value is in the ED.  */
    data_td -> ux_rz_td_toggle =  UX_RZ_TD_TOGGLE_FROM_ED;

    /* Store the beginning of the buffer address in the TD.  */
    data_td -> ux_rz_td_buffer =  transfer_request -> ux_transfer_request_data_pointer;

    /* Update the length of the transfer for this TD.  */
    data_td -> ux_rz_td_length =  transfer_request -> ux_transfer_request_requested_length;

    /* Attach the endpoint and transfer request to the TD.  */
    data_td -> ux_rz_td_transfer_request =  transfer_request;
    data_td -> ux_rz_td_ed =  ed;

    /* At this stage, the Head and Tail in the ED are still the same and
       the RZ controller will skip this ED until we have hooked the new
       tail TD.  */
    tail_td =  _ux_hcd_rz_regular_td_obtain(hcd_rz);
    if (tail_td == UX_NULL)
        return(UX_NO_TD_AVAILABLE);

    /* Attach the tail TD to the last data TD.  */
    data_td -> ux_rz_td_next_td =  tail_td;

    /* Store the new tail TD.  */
    ed -> ux_rz_ed_tail_td =  tail_td;

    /* Now we can tell the scheduler to wake up.  */
    hcd_rz -> ux_hcd_rz_queue_empty =  UX_FALSE;

    /* There is no need to wake up the RZ controller on this transfer
       since periodic transactions will be picked up when the interrupt
       tree is scanned.  */
    return(UX_SUCCESS);           
}

