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
/*    _ux_hcd_rz_periodic_schedule                        PORTABLE C      */ 
/*                                                           5.8          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Thierry Giron                                                       */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*     This function schedules new transfers from the periodic interrupt  */ 
/*     list.                                                              */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    hcd_rz                              Pointer to RZ controller        */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    None                                                                */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    _ux_hcd_rz_frame_number_get         Get frame number                */ 
/*    _ux_hcd_rz_ptd_add                  Insert PTD into list            */ 
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
VOID  _ux_hcd_rz_periodic_schedule(UX_HCD_RZ *hcd_rz)
{

UX_RZ_ED            *ed;
ULONG               frame_number;

    /* Get the current frame number.  */
    _ux_hcd_rz_frame_number_get(hcd_rz, &frame_number);

    /* Isolate the low bits to match an entry in the upper periodic entry list.  */
    frame_number &=  UX_RZ_PERIODIC_ENTRY_MASK;

    /* Get the first ED in the periodic list.  */
    ed =  hcd_rz -> ux_hcd_rz_interrupt_ed_list[frame_number];

    /* We try to find one transaction if possible that is a candidate.  */
    while (ed != UX_NULL) 
    {

        /* The ED has to be a real ED (not static) and has to have a different tail and head TD.  */
        if ((ed -> ux_rz_ed_status != UX_RZ_ED_STATIC) && (ed -> ux_rz_ed_tail_td != ed -> ux_rz_ed_head_td)
            && (ed -> ux_rz_ed_head_td -> ux_rz_td_status & UX_RZ_TD_ACK_PENDING) == 0 )
        {
            /* Insert this transfer in the list of PTDs if possible.  */
            _ux_hcd_rz_td_add(hcd_rz, ed);
            
            /* We are done here.  */
            break;

        }

        /* Point to the next ED in the list.  */
        ed =  ed -> ux_rz_ed_next_ed;
    }

    /* Return to caller.  */
    return;
}

