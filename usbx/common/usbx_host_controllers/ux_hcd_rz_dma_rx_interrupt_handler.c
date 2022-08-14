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

#define UX_SOURCE_CODE


/* Include necessary system files.  */

#include "ux_api.h"
#include "ux_hcd_rz.h"
#include "ux_system.h"

#include "usb_dma.h"	/* grape */

#ifdef UX_RZ_HCD_USE_DMA
/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                                 RELEASE      */
/*                                                                        */
/*    _ux_hcd_rz_dma_rx_interrupt_handler                   PORTABLE C    */
/*                                                             5.8        */
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Thierry Giron                                                       */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function is the interrupt handler for the RZ controller.       */
/*    The controller will trigger an interrupt when something happens on  */
/*    an endpoint whose mask has been set in the interrupt enable         */
/*    register.                                                           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    _ux_dcd_rz_initialize_complete      Complete initialization         */ 
/*    _ux_dcd_rz_register_read            Read register                   */ 
/*    _ux_dcd_rz_register_write           Write register                  */ 
/*    _ux_dcd_rz_transfer_callback        Process callback                */ 
/*    _ux_device_stack_disconnect           Disconnect device             */ 
/*                                                                        */ 
/*  CALLED BY                                                             */ 
/*                                                                        */ 
/*    USBX Device Stack                                                   */ 
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

VOID  _ux_hcd_rz_dma_rx_interrupt_handler(UINT int_sense)
{

UINT                hcd_index;
UX_HCD              *hcd;
UX_HCD_RZ           *hcd_rz;

    /* We need to parse the controller driver table to find all controllers that 
      are registered as RZ.  */
    for (hcd_index = 0; hcd_index < _ux_system_host -> ux_system_host_registered_hcd; hcd_index++)
    {


        /* Check type of controller.  */
        if (_ux_system_host -> ux_system_host_hcd_array[hcd_index].ux_hcd_controller_type == UX_RZ_CONTROLLER)
        {

            /* Get the pointers to the generic HCD and RZ specific areas.  */
            hcd =  &_ux_system_host -> ux_system_host_hcd_array[hcd_index];
            hcd_rz =  (UX_HCD_RZ *) hcd -> ux_hcd_controller_hardware;

            /* Clear DMA flags.  */
            _ux_hcd_rz_dma_register_write(hcd_rz, UX_RZ_DMA_CHCTRL(UX_RZ_DMA_RX_CH),
                                          UX_RZ_DMA_CHCTRL_CLREN | UX_RZ_DMA_CHCTRL_SWRST);

            /* Set status to idle.  */
            hcd_rz -> ux_hcd_rz_dma_status = UX_RZ_DMA_STATUS_IDLE;

#if 1	/* grape */
            usb_dma_single_put_sem();
#endif
            /* Check if we need to put a semaphore.  */
            if (hcd_rz -> ux_hcd_rz_transfer_request != UX_NULL)
            {
                /* Check if there is a callback.  */
                if (hcd_rz -> ux_hcd_rz_transfer_request -> ux_transfer_request_completion_function != UX_NULL)
            
                    /* Yes, so execute it.  */
                    hcd_rz -> ux_hcd_rz_transfer_request -> ux_transfer_request_completion_function(hcd_rz -> ux_hcd_rz_transfer_request);

                /* Wake up the thread waiting for this event to complete.  */
                _ux_utility_semaphore_put(&hcd_rz -> ux_hcd_rz_transfer_request -> ux_transfer_request_semaphore);

                hcd_rz -> ux_hcd_rz_transfer_request = UX_NULL;
            }
        }
    }
}

#endif
