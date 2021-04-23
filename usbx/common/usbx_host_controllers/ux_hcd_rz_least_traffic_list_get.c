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
/*    _ux_hcd_rz_least_traffic_list_get                   PORTABLE C      */ 
/*                                                           5.8          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Thierry Giron                                                       */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*     This function return a pointer to the first ED in the periodic     */ 
/*     tree that has the least traffic registered.                        */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    hcd_rz                              Pointer to RZ controller        */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    UX_RZ_ED *                          Pointer to RZ ED                */ 
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
UX_RZ_ED  *_ux_hcd_rz_least_traffic_list_get(UX_HCD_RZ *hcd_rz)
{

UX_RZ_ED            *min_bandwidth_ed;
UX_RZ_ED            *begin_ed;
UX_RZ_ED            *ed;
UX_ENDPOINT         *endpoint;
UINT                list_index;
ULONG               min_bandwidth_used;
ULONG               bandwidth_used;


    /* Set the min bandwidth used to a arbitrary maximum value.  */
    min_bandwidth_used =  0xffffffff;

    /* The first ED is the list candidate for now.  */
    min_bandwidth_ed =  hcd_rz -> ux_hcd_rz_interrupt_ed_list[0];
    
    /* All list will be scanned.  */
    for (list_index = 0; list_index < 32; list_index++)
    {

        /* Reset the bandwidth for this list.  */
        bandwidth_used =  0;

        /* Get the ED of the beginning of the list we parse now.  */
        ed =  hcd_rz -> ux_hcd_rz_interrupt_ed_list[list_index];

        /* We keep track of the first ED for the current list.  */
        begin_ed =  ed;

        /* Parse the eds in the list.  */
        while (ed -> ux_rz_ed_next_ed != UX_NULL)
        {

            /* Check if this is a valid endpoint or an anchor only.  */
            if ((ed -> ux_rz_ed_status & UX_RZ_ED_STATIC) == 0)
            {

                /* Get the endpoint pointer from the physical ED.  */
                endpoint =  ed -> ux_rz_ed_endpoint;

                /* Add to the bandwidth used the max packet size pointed by this ED.  */
                bandwidth_used +=  (ULONG) endpoint -> ux_endpoint_descriptor.wMaxPacketSize;
            }
            
            /* Move to next ED.  */           
            ed =  ed -> ux_rz_ed_next_ed;
        }

        /* We have processed a list, check the bandwidth used by this list.
           If this bandwidth is the minimum, we memorize the ED.  */        
        if (bandwidth_used < min_bandwidth_used)
        {

            /* We have found a better list with a lower used bandwidth, memorize the bandwidth 
               for this list.  */
            min_bandwidth_used =  bandwidth_used;
            
            /* Memorize the begin ED for this list.  */
            min_bandwidth_ed =  begin_ed;
        }
    }
    
    /* Return the ED list with the lowest bandwidth.  */
    return(min_bandwidth_ed);   
}

