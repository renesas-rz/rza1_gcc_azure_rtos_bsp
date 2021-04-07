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
/*    _ux_hcd_rz_port_reset                               PORTABLE C      */ 
/*                                                           5.8          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Thierry Giron                                                       */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*     This function will reset a specific port attached to the root      */ 
/*     HUB.                                                               */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    hcd_rz                              Pointer to RZ controller        */ 
/*    port_index                            Port index                    */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    Completion Status                                                   */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    _ux_hcd_rz_hcor_register_read       Read RZ register                */ 
/*    _ux_hcd_rz_hcor_register_write      Write RZ register               */ 
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
UINT  _ux_hcd_rz_port_reset(UX_HCD_RZ *hcd_rz, ULONG port_index)
{

ULONG       rz_register_port_status;
ULONG       reset_in_progress;

    /* Check to see if this port is valid on this controller.  */
    if (hcd_rz -> ux_hcd_rz_nb_root_hubs < port_index)
    {

        /* Error trap. */
        _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_HCD, UX_PORT_INDEX_UNKNOWN);

        /* If trace is enabled, insert this event into the trace buffer.  */
        UX_TRACE_IN_LINE_INSERT(UX_TRACE_ERROR, UX_PORT_INDEX_UNKNOWN, port_index, 0, 0, UX_TRACE_ERRORS, 0, 0)

        return(UX_PORT_INDEX_UNKNOWN);
    }
    
    /* Ensure that the downstream port has a device attached. It is unnatural
    to perform a port reset if there is no device.  */
    rz_register_port_status =  _ux_hcd_rz_port_status_get(hcd_rz, port_index);
    
    /* Check Device Connection Status.  */
    if ((rz_register_port_status &  UX_PS_CCS) == 0)
    {
    
        /* If trace is enabled, insert this event into the trace buffer.  */
        UX_TRACE_IN_LINE_INSERT(UX_TRACE_ERROR, UX_NO_DEVICE_CONNECTED, port_index, 0, 0, UX_TRACE_ERRORS, 0, 0)

        return(UX_NO_DEVICE_CONNECTED);
    }

    /* Now we can safely issue a RESET command to this port.  */
    _ux_hcd_rz_register_set(hcd_rz, UX_RZ_HC_DVSTCTR0, UX_RZ_HC_DVSTCTR0_USBRST);
    _ux_hcd_rz_register_clear(hcd_rz, UX_RZ_HC_DVSTCTR0, UX_RZ_HC_DVSTCTR0_UACT);

    /* Perform the necessary delay before resetting the port.  */
    tx_thread_sleep(50 * UX_PERIODIC_RATE / 1000);  
     
    /* Now we can safely complete a RESET command to this port.  */
    _ux_hcd_rz_register_set(hcd_rz, UX_RZ_HC_DVSTCTR0, UX_RZ_HC_DVSTCTR0_UACT);
    _ux_hcd_rz_register_clear(hcd_rz, UX_RZ_HC_DVSTCTR0, UX_RZ_HC_DVSTCTR0_USBRST); 
    
    tx_thread_sleep(20 * UX_PERIODIC_RATE / 1000);  
    
    reset_in_progress = _ux_hcd_rz_register_read(hcd_rz, UX_RZ_HC_DVSTCTR0);
    
    /* Check if reset is completed.  */
    while (reset_in_progress & UX_RZ_HC_DVSTCTR0_RESET_IN_PROGRESS)
    {
        
        /* Not yet, so wait a bit.  */
        tx_thread_sleep(10 * UX_PERIODIC_RATE / 1000); 

        /* Read the reset status again.  */
        reset_in_progress = _ux_hcd_rz_register_read(hcd_rz, UX_RZ_HC_DVSTCTR0);
    
    }
    
    return(UX_SUCCESS);       
}

