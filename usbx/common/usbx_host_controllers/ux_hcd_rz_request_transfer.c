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
/*    _ux_hcd_rz_request_transfer                         PORTABLE C      */ 
/*                                                           5.8          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Thierry Giron                                                       */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*     This function is the handler for all the transactions on the USB.  */
/*     The transfer request passed as parameter contains the endpoint and */ 
/*     the device descriptors in addition to the type of transaction de   */ 
/*     be executed. This function routes the transfer request to          */ 
/*     according to the type of transfer to be executed.                  */ 
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
/*    _ux_hcd_rz_request_bulk_transfer         Request bulk transfer      */ 
/*    _ux_hcd_rz_request_control_transfer      Request control            */ 
/*                                                    transfer            */ 
/*    _ux_hcd_rz_request_interrupt_transfer    Request interrupt          */ 
/*                                                    transfer            */ 
/*    _ux_hcd_rz_request_isochronous_transfer  Request isochronous        */ 
/*                                                    transfer            */ 
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
UINT  _ux_hcd_rz_request_transfer(UX_HCD_RZ *hcd_rz, UX_TRANSFER *transfer_request)
{

UX_ENDPOINT     *endpoint;
UINT            status;
    
    /* Get the pointer to the Endpoint.  */
    endpoint =  (UX_ENDPOINT *) transfer_request -> ux_transfer_request_endpoint;

    /* We reset the actual length field of the transfer request as a safety measure.  */
    transfer_request -> ux_transfer_request_actual_length =  0;
    
    /* Isolate the endpoint type and route the transfer request.  */
    switch ((endpoint -> ux_endpoint_descriptor.bmAttributes) & UX_MASK_ENDPOINT_TYPE)
    {

    case UX_CONTROL_ENDPOINT:

        status =  _ux_hcd_rz_request_control_transfer(hcd_rz, transfer_request);
        break;


    case UX_BULK_ENDPOINT:

        status =  _ux_hcd_rz_request_bulk_transfer(hcd_rz, transfer_request);
        break;

    case UX_INTERRUPT_ENDPOINT:

        status =  _ux_hcd_rz_request_interrupt_transfer(hcd_rz, transfer_request);
        break;

    case UX_ISOCHRONOUS_ENDPOINT:

        status =  _ux_hcd_rz_request_isochronous_transfer(hcd_rz, transfer_request);
        break;

    default:

        status =  UX_ERROR;
    }

    /* Note that it is physically impossible to have a wrong endpoint type here
       so no error checking.  */
    return(status);         
}

