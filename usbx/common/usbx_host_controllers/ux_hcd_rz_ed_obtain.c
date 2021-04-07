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
/*    _ux_hcd_rz_ed_obtain                                PORTABLE C      */ 
/*                                                           5.8          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Thierry Giron                                                       */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This function obtains a free ED from the ED list.                   */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    hcd_rz                                 Pointer to RZ controller     */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    UX_RZ_ED *                             Pointer to ED                */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    _ux_utility_memory_set                   Set memory block           */ 
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
UX_RZ_ED  *_ux_hcd_rz_ed_obtain(UX_HCD_RZ *hcd_rz)
{

UX_RZ_ED      *ed;
ULONG          ed_index;

    /* Start the search from the beginning of the list.  */
    ed =  hcd_rz -> ux_hcd_rz_ed_list;
    for(ed_index = 0; ed_index < _ux_system_host -> ux_system_host_max_ed; ed_index++)
    {

        /* Check the ED status, a free ED is marked with the UNUSED flag.  */
        if (ed -> ux_rz_ed_status == UX_UNUSED)
        {

            /* The ED may have been used, so we reset all fields.  */
            _ux_utility_memory_set(ed, 0, sizeof(UX_RZ_ED));

            /* This ED is now marked as USED.  */
            ed -> ux_rz_ed_status =  UX_USED;

            /* Return ED pointer.  */
            return(ed);
        }

        /* Point to the next ED.  */
        ed++;
    }

    /* There is no available ED in the ED list.  */
    return(UX_NULL);
}

