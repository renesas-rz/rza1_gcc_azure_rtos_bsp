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


/**************************************************************************/ 
/*                                                                        */ 
/*  FUNCTION                                               RELEASE        */ 
/*                                                                        */ 
/*    _ux_hcd_rz_periodic_tree_create                     PORTABLE C      */ 
/*                                                           5.8          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Thierry Giron                                                       */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*     This function creates the periodic static tree for the interrupt   */ 
/*     and isochronous eds.                                               */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    hcd_rz                              Pointer to RZ controller        */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    Completion Status                                                   */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    _ux_hcd_rz_ed_obtain                Obtain RZ ED                    */ 
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

UINT  _ux_hcd_rz_periodic_tree_create(UX_HCD_RZ *hcd_rz)
{

UX_RZ_ED            *ed;
UINT                list_index;
UINT                list_entries;
UINT                current_list_entry;
UX_RZ_ED            *ed_list[32];
UX_RZ_ED            *ed_start_list[32];
    
    
    /* Start with the 1st list - it has 32 entries.  */
    list_entries =  32;

    /* Create each list one by one starting from the 32ms list.  */
    for (list_index = 0; list_index < 6; list_index++)
    {

        for (current_list_entry = 0; current_list_entry < list_entries; current_list_entry++)
        {
            /* In each list, insert an static ED as the anchor. There should not
               be any errors when obtaining a new ED, still we do a sanity check.  */
            ed =  _ux_hcd_rz_ed_obtain(hcd_rz);
            if (ed == UX_NULL)
                return(UX_NO_ED_AVAILABLE);

            /* Mark this anchor ED as static by putting it as SKIPPED, the RZ  controller will 
               not look into its tail and head list and will simply jump to the next ED.  */
            ed -> ux_rz_ed_status =  UX_RZ_ED_STATIC;

            /* Either we hook this new ED to the start list for further processing
               or we hook it to the 2 successive entries in the previous list.  */
            if (list_index == 0)
            {

                ed_start_list[current_list_entry] =  ed;
            }
            else
            {

                ed_list[current_list_entry * 2] -> ux_rz_ed_next_ed =  ed;
                ed_list[(current_list_entry * 2) + 1] -> ux_rz_ed_next_ed =  ed;
            }

            /* Memorize this ED in the local list. We do this operation now, otherwise
               we would erase the previous list eds.  */
            ed_list[current_list_entry] =  ed;
        }

        /*  Shift the number of entries in the next list by 1 (i.e. divide by 2).  */
        list_entries =  list_entries >> 1;
    }

    /* The tree has been completed but the entries in the interrupt list are in the wrong order.
       We need to swap each entry according to the RZ specified entry order list 
       so that we have a fair interval frequency for each periodic ED. The primary eds are 
       fetched from the start list, and stored in the interrupt list.  */
    for (current_list_entry = 0; current_list_entry < 32; current_list_entry++)
    {

        ed =  ed_start_list[_ux_system_host_hcd_periodic_tree_entries[current_list_entry]];
        hcd_rz -> ux_hcd_rz_interrupt_ed_list[current_list_entry] =  ed;
    }

    /* Return successful completion.  */
    return(UX_SUCCESS);
}

