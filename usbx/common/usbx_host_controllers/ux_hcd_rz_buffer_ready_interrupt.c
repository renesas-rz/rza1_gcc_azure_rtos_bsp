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
/*    _ux_hcd_rz_buffer_ready_interrupt                   PORTABLE C      */ 
/*                                                           5.8          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Thierry Giron                                                       */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This function enable or disable the BRDY interrupt for the pipe     */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    hcd_rz                           Pointer to RZ controller           */ 
/*    ed                                 Endpoint                         */ 
/*    flag                               Enable or Disable                */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    none                                                                */ 
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
VOID  _ux_hcd_rz_buffer_ready_interrupt(UX_HCD_RZ *hcd_rz, UX_RZ_ED *ed, ULONG flag)
{

ULONG       rz_register;
ULONG       rz_brdy_register;

    /* Read the Interrupt Enable register 0.  */
    rz_register = _ux_hcd_rz_register_read(hcd_rz, UX_RZ_HC_INTENB0);

    /* Reset the BEMPE, NRDYE, BRDYE bits.  */
    _ux_hcd_rz_register_clear(hcd_rz, UX_RZ_HC_INTENB0, UX_RZ_HC_INTENB0_BEMPE | UX_RZ_HC_INTENB0_NRDYE | UX_RZ_HC_INTENB0_BRDYE);

    /* Read the BRDY register.  */
    rz_brdy_register = _ux_hcd_rz_register_read(hcd_rz, UX_RZ_HC_BRDYENB);

    /* Enable or Disable ?  */
    if (flag == UX_RZ_ENABLE)

        /* Set the appropriate endpoint number.  */
        rz_brdy_register |= (1 << ed -> ux_rz_pipe_index);

    else
    
        /* Reset the appropriate endpoint number.  */
        rz_brdy_register &= ~(1 << ed -> ux_rz_pipe_index);

    /* Wrie the BRDY register.  */
    _ux_hcd_rz_register_write(hcd_rz, UX_RZ_HC_BRDYENB, rz_brdy_register);

    /* Restore the INTENB0 register.  */
    _ux_hcd_rz_register_write(hcd_rz, UX_RZ_HC_INTENB0, rz_register);
    
    return;
}

