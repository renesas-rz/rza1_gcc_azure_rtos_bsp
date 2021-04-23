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
/*    This function change the endpoint in the FIFO                       */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    hcd_rz                              Pointer to RZ controller        */ 
/*    ed                                    Register to the ed            */ 
/*    direction                             Direction to switch           */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    None                                                                */ 
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
VOID  _ux_hcd_rz_current_endpoint_change(UX_HCD_RZ *hcd_rz, UX_RZ_ED *ed, ULONG direction)
{

ULONG   rz_register;
ULONG   rz_register_read_back;


    /* Select the fifo.  */
    switch (ed -> ux_rz_fifo_index)
    {

        case UX_RZ_HC_FIFO_D0       :
            
            /* Set the current endpoint index (pipe) in the D0FIFOSEL register.  */
            _ux_hcd_rz_register_write(hcd_rz, UX_RZ_HC_D0FIFOSEL, ed -> ux_rz_pipe_index);
            break;        
        
        
        case UX_RZ_HC_FIFO_D1       :
        
            /* Set the current endpoint index (pipe) in the D1FIFOSEL register.  */
            _ux_hcd_rz_register_write(hcd_rz, UX_RZ_HC_D1FIFOSEL, ed -> ux_rz_pipe_index);
            break;        

        case UX_RZ_HC_FIFO_C        :

            /* Read the current CFIFOSEL register.  */
            rz_register =  _ux_hcd_rz_register_read(hcd_rz, UX_RZ_HC_CFIFOSEL);

            /* Clear the ISEL field, the reading/writing width and the previous endpoint index.  */
            rz_register &= ~(UX_RZ_HC_CFIFOSEL_CURPIPE_MASK | UX_RZ_HC_CFIFOSEL_ISEL | UX_RZ_HC_CFIFOSEL_MBW_MASK);

            /* Add the current direction and the new endpoint index.  */
            rz_register |= (ed -> ux_rz_pipe_index | direction);

            /* Write back the register.  */
            _ux_hcd_rz_register_write(hcd_rz, UX_RZ_HC_CFIFOSEL, rz_register);

            do {
            
                /* Read the register again.  Ensuring the value was set properly.  */
                rz_register_read_back =  _ux_hcd_rz_register_read(hcd_rz, UX_RZ_HC_CFIFOSEL) & UX_RZ_HC_CFIFOSEL_CURPIPE_MASK;
                
                } while (rz_register_read_back != ed -> ux_rz_pipe_index);

            break;

        default                     :
            break;
        
    }
    
    return;
}

