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
/*    _ux_hcd_rz_isochronous_td_obtain                    PORTABLE C      */ 
/*                                                           5.8          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Thierry Giron                                                       */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*     This function obtains a free TD from the isochronous TD list.      */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    hcd_rz                           Pointer to RZ controller           */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    UX_RZ_ISO_TD *                   Pointer to RZ ISO ED               */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    _ux_utility_memory_set                Set memory block              */ 
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
UX_RZ_ISO_TD  *_ux_hcd_rz_isochronous_td_obtain(UX_HCD_RZ *hcd_rz)
{
                    
UX_RZ_ISO_TD        *td;
ULONG               td_index;


    /* Start the search from the beginning of the list.  */
    td =  hcd_rz -> ux_hcd_rz_iso_td_list;
    for (td_index = 0; td_index < _ux_system_host -> ux_system_host_max_iso_td; td_index++)
    {

        /* Check the TD status, a free TD is marked with the UX_USED flag.  */
        if (td -> ux_rz_iso_td_status == UX_UNUSED)
        {

            /* The TD may have been used, so we reset all fields.  */
            _ux_utility_memory_set(td, 0, sizeof(UX_RZ_ISO_TD));

            /* This TD is now marked as USED.  */
            td -> ux_rz_iso_td_status = UX_USED;

            /* Success, return pointer to TD.  */
            return(td);
        }

        /* Move to next TD.  */
        td++;
    }

    /* There is no available TD in the TD list, return a NULL.  */
    return(UX_NULL);
}

