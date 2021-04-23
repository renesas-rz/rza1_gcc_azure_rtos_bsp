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
/*    _ux_hcd_rz_asynch_schedule                          PORTABLE C      */ 
/*                                                           5.8          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Thierry Giron                                                       */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This function schedules new transfers from the control/bulk lists.  */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    hcd_rz                                 Pointer to RZ controller     */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    None                                                                */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
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
VOID  _ux_hcd_rz_asynch_schedule(UX_HCD_RZ *hcd_rz)
{

UX_RZ_ED       *ed;
UX_RZ_ED       *first_ed;
UINT           status;

    /* Get the pointer to the current ED in the asynchronous list.  */
    ed =  hcd_rz -> ux_hcd_rz_asynch_current_ed;

    /* Check if there is any eds candidate in the asynch list.  */
    if (ed == UX_NULL)
    {

        /* Check if there is any ED in the asynch list. If none, nothing to do.  */
        if (hcd_rz -> ux_hcd_rz_asynch_head_ed == UX_NULL)
            return;
        else
        
            /* We use the first ED of the asynch list.  */
            ed = hcd_rz -> ux_hcd_rz_asynch_head_ed;
    }

    /* Remember this ED.  */
    first_ed =  ed;

    /* Parse the list of eds in search for scheduling candidates. */
    do 
    {

            /* Check if this ED has a tail and head TD different, and no TD in progress */
            if ( (ed -> ux_rz_ed_tail_td != ed -> ux_rz_ed_head_td) && (ed -> ux_rz_ed_head_td -> ux_rz_td_status & UX_RZ_TD_ACK_PENDING) == 0 )
            {
    
                /* Insert this transfer in the list of TDs if possible.  */
                status =  _ux_hcd_rz_td_add(hcd_rz, ed);
    
                /* If the TD has been added to the list, we can memorize this ED has 
                   being served and make the next ED as the one to be first scanned 
                   at the next SOF.  */
                if (status == UX_SUCCESS)
                {
    
                    /* If this is the end of the list ?  */
                    if (ed -> ux_rz_ed_next_ed == UX_NULL)
                    
                        /* Restart from the beginning.  */
                        hcd_rz -> ux_hcd_rz_asynch_current_ed =  hcd_rz -> ux_hcd_rz_asynch_head_ed;
    
                    else            
    
                        /* This is not the end, so use the next ED as the next candidate.  */
                        hcd_rz -> ux_hcd_rz_asynch_current_ed =  ed -> ux_rz_ed_next_ed;
                }
            }
    

        /* Point to the next ED in the list. Check if at end of list.  */
        if (ed -> ux_rz_ed_next_ed == UX_NULL)
    
            /* We are at the end. restart from the beginning.  */
            ed =  hcd_rz -> ux_hcd_rz_asynch_head_ed;
    
        else            

            /* Proceed to the next ED.  */
            ed =  ed -> ux_rz_ed_next_ed;

    } while ((ed) && (ed != first_ed));

}

