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
/*    _ux_hcd_rz_port_status_get                          PORTABLE C      */ 
/*                                                           5.8          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Thierry Giron                                                       */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*     This function will return the status for each port attached to the */
/*     root HUB.                                                          */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    hcd_rz                               Pointer to RZ controller       */ 
/*    port_index                             Port index                   */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*  RZ Port Status                                                        */ 
/*                                                                        */ 
/*    Where port status has the following format:                         */
/*                                                                        */ 
/*               bit 0         device connection status                   */
/*                             if 0 : no device connected                 */
/*                             if 1 : device connected to the port        */
/*               bit 1         port enable status                         */
/*                             if 0 : port disabled                       */
/*                             if 1 : port enabled                        */
/*               bit 2         port suspend status                        */
/*                             if 0 : port is not suspended               */
/*                             if 1 : port is suspended                   */
/*               bit 3         port overcurrent status                    */
/*                             if 0 : port has no overcurrent condition   */
/*                             if 1 : port has overcurrent condition      */
/*               bit 4         port reset status                          */
/*                             if 0 : port is not in reset                */
/*                             if 1 : port is in reset                    */
/*               bit 5         port power status                          */
/*                             if 0 : port power is off                   */
/*                             if 1 : port power is on                    */
/*               bit 6-7       device attached speed                      */
/*                             if 00 : low speed device attached          */
/*                             if 01 : full speed device attached         */
/*                             if 10 : high speed device attached         */
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    _ux_hcd_rz_register_read               Read RZ register             */ 
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
ULONG  _ux_hcd_rz_port_status_get(UX_HCD_RZ *hcd_rz, ULONG port_index)
{

ULONG       rz_register_port_status;
ULONG       port_status;
ULONG       speed;

    /* Check to see if this port is valid on this controller.  */
    if (hcd_rz -> ux_hcd_rz_nb_root_hubs < port_index)
    {

        /* Error trap. */
        _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_HCD, UX_PORT_INDEX_UNKNOWN);

        /* If trace is enabled, insert this event into the trace buffer.  */
        UX_TRACE_IN_LINE_INSERT(UX_TRACE_ERROR, UX_PORT_INDEX_UNKNOWN, port_index, 0, 0, UX_TRACE_ERRORS, 0, 0)

        return(UX_PORT_INDEX_UNKNOWN);
    }
    
    /* The port is valid, build the status mask for this port. This function
       returns a controller agnostic bit field.  */ 
    port_status =  0;
    
    /* The device attachment signal is quite erratic for this chip and it may be that we have multiple 
       insertion/extraction very quickly.  We wait a little. */
     tx_thread_sleep(100 * UX_PERIODIC_RATE / 1000);  

    /* Now check the low level filter.  */
    if (hcd_rz -> ux_rz_device_attached == UX_TRUE)     
    {
            
        /* Get the Device State Control Register.  */
        rz_register_port_status =  _ux_hcd_rz_register_read(hcd_rz, UX_RZ_HC_DVSTCTR0);
                        
        /* Isolate speed.  */
        speed =  rz_register_port_status & UX_RZ_HC_DVSTCTR0_RHST;
            
        /* Detect Port Device Attached speed.   */
        switch(speed)
        {
            case UX_RZ_HC_DVSTCTR0_SPEED_LOW:
                port_status |=  UX_PS_DS_LS;
                break;
        
            case UX_RZ_HC_DVSTCTR0_SPEED_FULL:
                port_status |=  UX_PS_DS_FS;
                break;
                
            case UX_RZ_HC_DVSTCTR0_SPEED_HIGH:
                port_status |=  UX_PS_DS_HS;
                break;
        }      
        
        /*  Device connection detected .  */
        port_status |=  UX_PS_CCS;
        
        /* Port Enabled.  */
        hcd_rz -> ux_hcd_rz_port_status = UX_RZ_HC_PORT_ENABLED;
        
        /* Port Enable Status.  */
        if (rz_register_port_status & UX_RZ_HC_DVSTCTR0_UACT)
            port_status |=  UX_PS_PES;

        /* Port Reset Status.  */
        if (rz_register_port_status & UX_RZ_HC_DVSTCTR0_USBRST)
            port_status |=  UX_PS_PRS;
                                   
    }    
    else
        /* And the port is disabled.  */
        hcd_rz -> ux_hcd_rz_port_status = UX_RZ_HC_PORT_DISABLED;

    /* Return port status.  */
    return(port_status);            
}

