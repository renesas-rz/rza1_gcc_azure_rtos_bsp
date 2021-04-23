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
/*    _ux_hcd_rz_frame_number_get                         PORTABLE C      */ 
/*                                                           5.8          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Thierry Giron                                                       */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This function will return the frame number currently used by the    */
/*    controller. This function is mostly used for isochronous purposes   */ 
/*    and for timing.                                                     */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    hcd_rz                              Pointer to RZ controller        */ 
/*    frame_number                          Frame number to set           */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    Completion Status                                                   */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    _ux_hcd_rz_register_read            Read RZ register                */ 
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
UINT  _ux_hcd_rz_frame_number_get(UX_HCD_RZ *hcd_rz, ULONG *frame_number)
{

ULONG               rz_register_port_status;
volatile ULONG      index_loop;
volatile ULONG      dummy_access;
    
    /* Read the true port status.  */            
    rz_register_port_status =  _ux_hcd_rz_register_read(hcd_rz, UX_RZ_HC_DVSTCTR0);
                                
    /* Check if port is transmitting SOFs. 
       There is a catch with the SH2 processor USB cell. If the downstream port is not sending SOFs
       the frame number is not incremented. So we use a local loop.  */
    if ((rz_register_port_status & UX_RZ_HC_DVSTCTR0_UACT) == 0)
    {

        /* There s no SOF generated. Do 1 ms loop locally.  */
        for (index_loop = 0; index_loop < 200000; index_loop++)
            dummy_access++;

        /* Now increment the frame number in the HCD structure.  */
        hcd_rz -> ux_hcd_rz_frame_number++;
    }

    else
           
        /* Pickup the frame number.  */
        hcd_rz -> ux_hcd_rz_frame_number =  _ux_hcd_rz_register_read(hcd_rz, UX_RZ_HC_FRMNUM) & UX_RZ_HC_FRMNUM_FRNM_MASK;

    /* Store the frame # in the caller's parameters.  */
    *frame_number = hcd_rz -> ux_hcd_rz_frame_number;

    return(UX_SUCCESS);
}

