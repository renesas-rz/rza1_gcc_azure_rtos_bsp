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
/*    _ux_hcd_rz_fifo_port_change                         PORTABLE C      */ 
/*                                                           5.8          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Thierry Giron                                                       */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This function change the port of the FIFO                           */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    hcd_rz                              Pointer to RZ controller        */ 
/*    ed                                    Register to the ed            */ 
/*    direction                             Direction to switch           */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    fifo_access_status                                                  */ 
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
ULONG  _ux_hcd_rz_fifo_port_change(UX_HCD_RZ *hcd_rz, UX_RZ_ED *ed, ULONG direction)
{

ULONG   loop_index;
ULONG   rz_register;

    /* Set the current fifo port.  */
    _ux_hcd_rz_current_endpoint_change(hcd_rz, ed, direction);

    /* According to the SH7671 spec, it make take several tries to perform this function.  */
    for (loop_index = 0; loop_index < 4; loop_index++) 
    {

        /* Select the fifo.  */
        switch (ed -> ux_rz_fifo_index)
        {
    
            case UX_RZ_HC_FIFO_D0       :
                
                /* Read the current endpoint index (pipe) in the D0FIFOSEL register.  */
                rz_register =  _ux_hcd_rz_register_read(hcd_rz, UX_RZ_HC_D0FIFOCTR);
                break;        
            
            case UX_RZ_HC_FIFO_D1       :
            
                /* Read the current endpoint index (pipe) in the D1FIFOSEL register.  */
                rz_register =  _ux_hcd_rz_register_read(hcd_rz, UX_RZ_HC_D1FIFOCTR);
                break;        
    
            case UX_RZ_HC_FIFO_C        :
    
                /* Set the current endpoint index (pipe) in the CFIFOSEL register.  */
                rz_register =  _ux_hcd_rz_register_read(hcd_rz, UX_RZ_HC_CFIFOCTR);
                break;
    
            default                     :

                /* We should never get here.  */
                rz_register = 0;
            
        }
        
        /* Now check the status of the FRDY bit.  */
        if ((rz_register & UX_RZ_HC_FIFOCTR_FRDY) == UX_RZ_HC_FIFOCTR_FRDY)
            
            return(rz_register);

    }

    /* Return to caller with an error. */
    return(UX_ERROR);
}

