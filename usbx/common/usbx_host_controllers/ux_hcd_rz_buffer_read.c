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
#include "ux_system.h"
#include "ux_utility.h"


/**************************************************************************/ 
/*                                                                        */ 
/*  FUNCTION                                               RELEASE        */ 
/*                                                                        */ 
/*    _ux_hcd_rz_buffer_read                              PORTABLE C      */ 
/*                                                           5.8          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Thierry Giron                                                       */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This function reads from a specified pipe into a buffer.            */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    hcd_rz                              Pointer to RZ controller        */ 
/*    ed                                    Register to the ed            */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    status                                                              */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    None                                                                */ 
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
UINT  _ux_hcd_rz_buffer_read(UX_HCD_RZ *hcd_rz, UX_RZ_ED *ed)
{

UINT    status;
    
    
    /* Read from the FIFO C or D0 or D1.  */
    status = _ux_hcd_rz_fifo_read(hcd_rz, ed);

    /* Analyze return status.  */
    switch (status)
    {
    
        case UX_RZ_HC_FIFO_READING      :
        
            /* Enable the Ready interrupt.  */
            _ux_hcd_rz_buffer_ready_interrupt(hcd_rz, ed, UX_RZ_ENABLE);
            break;      

        case UX_RZ_HC_FIFO_READ_END     :
        case UX_RZ_HC_FIFO_READ_SHORT   :

            /* Disable the Ready interrupt.  */
            _ux_hcd_rz_buffer_ready_interrupt(hcd_rz, ed, UX_RZ_DISABLE);
            
            /* Enable the Buffer empty interrupt.  */
            _ux_hcd_rz_buffer_empty_interrupt(hcd_rz, ed, UX_RZ_ENABLE);
            
            break;      
        
        case UX_RZ_HC_FIFO_READ_DMA     :
            break;
            
        case UX_RZ_HC_FIFO_READ_ERROR   :
        default                         :
        
            /* Disable the Ready interrupt.  */
            _ux_hcd_rz_buffer_ready_interrupt(hcd_rz, ed, UX_RZ_DISABLE);
            
            /* Disable the Buffer empty interrupt.  */
            _ux_hcd_rz_buffer_empty_interrupt(hcd_rz, ed, UX_RZ_DISABLE);
            
            break;      

    }

    /* Return to caller. */
    return(status);
}

