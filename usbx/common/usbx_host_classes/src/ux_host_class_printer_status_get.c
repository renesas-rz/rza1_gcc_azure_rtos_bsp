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
/**   Printer Class                                                       */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/* Include necessary system files.  */

#define UX_SOURCE_CODE

#include "ux_api.h"
#include "ux_host_class_printer.h"
#include "ux_host_stack.h"


/**************************************************************************/ 
/*                                                                        */ 
/*  FUNCTION                                               RELEASE        */ 
/*                                                                        */ 
/*    _ux_host_class_printer_status_get                   PORTABLE C      */ 
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Chaoqiong Xiao, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */ 
/*    This function obtains the printer status. The printer status is     */ 
/*    similar to the LPT status (1284 standard).                          */
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    printer                               Pointer to printer class      */ 
/*    printer_status                        Pointer to return status      */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    Completion Status                                                   */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    _ux_host_stack_class_instance_verify  Verify the class instance     */ 
/*    _ux_host_stack_transfer_request       Process transfer request      */ 
/*    _ux_utility_long_get                  Get 32-bit long word          */ 
/*    _ux_utility_memory_allocate           Allocate memory block         */ 
/*    _ux_utility_memory_free               Free memory block             */ 
/*    _ux_utility_semaphore_get             Get protection semaphore      */ 
/*    _ux_utility_semaphore_put             Release protection semaphore  */ 
/*                                                                        */ 
/*  CALLED BY                                                             */ 
/*                                                                        */ 
/*    Application                                                         */ 
/*                                                                        */ 
/*  RELEASE HISTORY                                                       */ 
/*                                                                        */ 
/*    DATE              NAME                      DESCRIPTION             */ 
/*                                                                        */ 
/*  05-19-2020     Chaoqiong Xiao           Initial Version 6.0           */
/*  09-30-2020     Chaoqiong Xiao           Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
UINT  _ux_host_class_printer_status_get(UX_HOST_CLASS_PRINTER *printer, ULONG *printer_status)
{

UX_ENDPOINT     *control_endpoint;
UX_TRANSFER     *transfer_request;
UINT            status;
UCHAR *         printer_status_buffer;

    /* Ensure the instance is valid.  */
    if (printer -> ux_host_class_printer_state !=  UX_HOST_CLASS_INSTANCE_LIVE)
    {        

        /* If trace is enabled, insert this event into the trace buffer.  */
        UX_TRACE_IN_LINE_INSERT(UX_TRACE_ERROR, UX_HOST_CLASS_INSTANCE_UNKNOWN, printer, 0, 0, UX_TRACE_ERRORS, 0, 0)

        return(UX_HOST_CLASS_INSTANCE_UNKNOWN);
    }

    /* Protect thread reentry to this instance.  */
    status =  _ux_utility_semaphore_get(&printer -> ux_host_class_printer_semaphore, UX_WAIT_FOREVER);
    if (status != UX_SUCCESS)

        /* Return error.  */
        return(status);

    /* We need to get the default control endpoint transfer_request pointer.  */
    control_endpoint =  &printer -> ux_host_class_printer_device -> ux_device_control_endpoint;
    transfer_request =  &control_endpoint -> ux_endpoint_transfer_request;

    /* Need to allocate memory for the descriptor.  */
    printer_status_buffer =  _ux_utility_memory_allocate(UX_SAFE_ALIGN, UX_CACHE_SAFE_MEMORY, UX_HOST_CLASS_PRINTER_STATUS_LENGTH);
    if (printer_status_buffer == UX_NULL)
    {

        /* Unprotect thread reentry to this instance */
        status =  _ux_utility_semaphore_put(&printer -> ux_host_class_printer_semaphore);
        return(UX_MEMORY_INSUFFICIENT);
    }

    /* Create a transfer_request for the GET_STATUS request.  */
    transfer_request -> ux_transfer_request_data_pointer =      printer_status_buffer;
    transfer_request -> ux_transfer_request_requested_length =  UX_HOST_CLASS_PRINTER_STATUS_LENGTH;
    transfer_request -> ux_transfer_request_function =          UX_HOST_CLASS_PRINTER_GET_STATUS;
    transfer_request -> ux_transfer_request_type =              UX_REQUEST_IN | UX_REQUEST_TYPE_CLASS | UX_REQUEST_TARGET_INTERFACE;
    transfer_request -> ux_transfer_request_value =             0;
    transfer_request -> ux_transfer_request_index =             0;

    /* Send request to HCD layer.  */
    status =  _ux_host_stack_transfer_request(transfer_request);

    /* Check for correct transfer and entire descriptor returned.  */
    if ((status == UX_SUCCESS) &&
        (transfer_request -> ux_transfer_request_actual_length > 0) &&
        (transfer_request -> ux_transfer_request_actual_length <= UX_HOST_CLASS_PRINTER_STATUS_LENGTH))
    {

        /* Retrieve the printer status and translate it locally for endianness.  */
        *printer_status = printer_status_buffer[0];
        if (transfer_request -> ux_transfer_request_actual_length > 1)
            *printer_status |= (ULONG)(printer_status_buffer[1] << 8);
        if (transfer_request -> ux_transfer_request_actual_length > 2)
            *printer_status |= (ULONG)(printer_status_buffer[2] << 16);
        if (transfer_request -> ux_transfer_request_actual_length > 3)
            *printer_status |= (ULONG)(printer_status_buffer[3] << 24);

        /* If trace is enabled, insert this event into the trace buffer.  */
        UX_TRACE_IN_LINE_INSERT(UX_TRACE_HOST_CLASS_PRINTER_STATUS_GET, printer, *printer_status, 0, 0, UX_TRACE_HOST_CLASS_EVENTS, 0, 0)
    }
    else
    {

        /* Error trap. */
        _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_CLASS, status);
    }

    /* Free all used resources.  */
    _ux_utility_memory_free(printer_status_buffer);

    /* Unprotect thread reentry to this instance.  */
    status =  _ux_utility_semaphore_put(&printer -> ux_host_class_printer_semaphore);

    /* Return completion status.  */
    return(status);
}

