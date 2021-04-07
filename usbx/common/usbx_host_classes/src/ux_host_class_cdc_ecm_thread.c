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
/**   CDC_ECM Class                                                       */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/* Include necessary system files.  */

#define UX_SOURCE_CODE

#include "ux_api.h"
#include "ux_host_class_cdc_ecm.h"
#include "ux_host_stack.h"

/**************************************************************************/ 
/*                                                                        */ 
/*  FUNCTION                                               RELEASE        */ 
/*                                                                        */ 
/*    _ux_host_class_cdc_ecm_thread                       PORTABLE C      */ 
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Chaoqiong Xiao, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */ 
/*    This is the CDC ECM thread that monitors the link change flag,      */
/*    receives data from the device, and passes the data to the NetX-USB  */
/*    broker.                                                             */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    cdc_ecm                               CDC ECM instance              */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    Completion Status                                                   */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    _ux_host_class_cdc_ecm_transmit_queue_clean                         */
/*                                          Clean transmit queue          */
/*    _ux_host_stack_transfer_request       Transfer request              */
/*    _ux_utility_semaphore_get             Get semaphore                 */
/*    _ux_utility_semaphore_put             Put semaphore                 */
/*    _ux_utility_short_get_big_endian      Get 16-bit big endian         */
/*    _ux_network_driver_link_up            Set state link up             */
/*    _ux_network_driver_link_down          Set state link down           */
/*    _ux_network_driver_packet_received    Process received packet       */
/*    nx_packet_allocate                    Allocate NetX packet          */
/*    nx_packet_release                     Free NetX packet              */
/*                                                                        */ 
/*  CALLED BY                                                             */ 
/*                                                                        */ 
/*    CDC ECM class initialization                                        */ 
/*                                                                        */ 
/*  RELEASE HISTORY                                                       */ 
/*                                                                        */ 
/*    DATE              NAME                      DESCRIPTION             */ 
/*                                                                        */ 
/*  05-19-2020     Chaoqiong Xiao           Initial Version 6.0           */
/*  09-30-2020     Chaoqiong Xiao           Modified comment(s),          */
/*                                            prefixed UX to MS_TO_TICK,  */
/*                                            used UX prefix to refer to  */
/*                                            TX symbols instead of using */
/*                                            them directly,              */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
VOID  _ux_host_class_cdc_ecm_thread(ULONG parameter)
{

UX_HOST_CLASS_CDC_ECM       *cdc_ecm;
UX_TRANSFER                 *transfer_request;
NX_PACKET                   *packet;
ULONG                       ip_given_length;
UINT                        status;

    /* Cast the parameter passed in the thread into the cdc_ecm pointer.  */
    UX_THREAD_EXTENSION_PTR_GET(cdc_ecm, UX_HOST_CLASS_CDC_ECM, parameter)

    /* Loop forever waiting for changes signaled through the semaphore. */     
    while (1)
    {   

        /* Wait for the semaphore to be put by the cdc_ecm interrupt event.  */
        _ux_utility_semaphore_get(&cdc_ecm -> ux_host_class_cdc_ecm_interrupt_notification_semaphore, UX_WAIT_FOREVER);

        /* Check the link state. It is either pending up or down.  */
        if (cdc_ecm -> ux_host_class_cdc_ecm_link_state == UX_HOST_CLASS_CDC_ECM_LINK_STATE_PENDING_UP)
        {

            /* Now the link is up.  */
            cdc_ecm -> ux_host_class_cdc_ecm_link_state = UX_HOST_CLASS_CDC_ECM_LINK_STATE_UP;

            /* Communicate the state with the network driver.  */
            _ux_network_driver_link_up(cdc_ecm -> ux_host_class_cdc_ecm_network_handle);
            
            /* As long as we are connected, configured and link up ... do some work.... */
            while ((cdc_ecm -> ux_host_class_cdc_ecm_link_state == UX_HOST_CLASS_CDC_ECM_LINK_STATE_UP) &&
                   (cdc_ecm -> ux_host_class_cdc_ecm_device -> ux_device_state == UX_DEVICE_CONFIGURED))                             
            {

                /* We can accept reception. Get a NX Packet. */
                status =  nx_packet_allocate(&cdc_ecm -> ux_host_class_cdc_ecm_packet_pool, &packet, 
                                             NX_RECEIVE_PACKET, UX_MS_TO_TICK(UX_HOST_CLASS_CDC_ECM_PACKET_POOL_WAIT));

                if (status == NX_SUCCESS)
                {

                    /* Adjust the prepend pointer to take into account the non 3 bit alignment of the ethernet header.  */
                    packet -> nx_packet_prepend_ptr += sizeof(USHORT);
    
                    /* We have a packet.  Link this packet to the reception transfer request on the bulk in endpoint. */
                    transfer_request =  &cdc_ecm -> ux_host_class_cdc_ecm_bulk_in_endpoint -> ux_endpoint_transfer_request;
                    
                    /* Set the data pointer.  */                
                    transfer_request -> ux_transfer_request_data_pointer     =  packet -> nx_packet_prepend_ptr;
    
                    /* And length.  */
                    transfer_request -> ux_transfer_request_requested_length =  UX_HOST_CLASS_CDC_ECM_NX_PAYLOAD_SIZE;
                    transfer_request -> ux_transfer_request_actual_length =     0;
    
                    /* Store the packet that owns this transaction.  */
                    transfer_request -> ux_transfer_request_user_specific = packet;
                
                    /* Reset the queue pointer of this packet.  */
                    packet -> nx_packet_queue_next =  UX_NULL;

                    /* We're arming the transfer now.  */
                    cdc_ecm -> ux_host_class_cdc_ecm_bulk_in_transfer_check_and_arm_in_process =  UX_TRUE;

                    /* Is the link up?  */
                    if (cdc_ecm -> ux_host_class_cdc_ecm_link_state == UX_HOST_CLASS_CDC_ECM_LINK_STATE_UP)
                    {

                        /* Ask USB to schedule a reception.  */
                        status =  _ux_host_stack_transfer_request(transfer_request);

                        /* Signal that we are done arming and resume waiting thread if necessary.  */
                        cdc_ecm -> ux_host_class_cdc_ecm_bulk_in_transfer_check_and_arm_in_process =  UX_FALSE;
                        if (cdc_ecm -> ux_host_class_cdc_ecm_bulk_in_transfer_waiting_for_check_and_arm_to_finish == UX_TRUE)
                            _ux_utility_semaphore_put(&cdc_ecm -> ux_host_class_cdc_ecm_bulk_in_transfer_waiting_for_check_and_arm_to_finish_semaphore);
    
                        /* Check if the transaction was armed successfully.  */
                        if (status == UX_SUCCESS)
                        {

                            /* Wait for the completion of the transfer request.  */
                            _ux_utility_semaphore_get(&transfer_request -> ux_transfer_request_semaphore, UX_WAIT_FOREVER);

                            /* Check the transfer status. If there is a transport error, we ignore the packet
                               and restart it. */
                            if (transfer_request -> ux_transfer_request_completion_code == UX_SUCCESS)
                            {

                                /* Get the packet length. */
                                packet -> nx_packet_length = transfer_request -> ux_transfer_request_actual_length;        
                        
                                /* Adjust the prepend, length, and append fields.  */ 
                                packet -> nx_packet_append_ptr =
                                    packet->nx_packet_prepend_ptr + transfer_request -> ux_transfer_request_actual_length;
                            
                                /* Calculate the accurate packet length from ip header */ 
                                if ((*(packet -> nx_packet_prepend_ptr + 12) == 0x08) && 
                                    (*(packet -> nx_packet_prepend_ptr + 13) == 0))
                                {

                                    ip_given_length = _ux_utility_short_get_big_endian(packet -> nx_packet_prepend_ptr + 16) + UX_HOST_CLASS_CDC_ECM_ETHERNET_SIZE;
                                    packet->nx_packet_length = ip_given_length ;
                                    packet->nx_packet_append_ptr =  packet->nx_packet_prepend_ptr + ip_given_length;
                                }
                        
                                /* Send that packet to the NetX USB broker.  */
                                _ux_network_driver_packet_received(cdc_ecm -> ux_host_class_cdc_ecm_network_handle, packet);
                            }
                            else
                            {

                                /* Free the packet that was not successfully received.  */
                                nx_packet_release(packet);
                            }
                        }
                        else
                        {

                            /* Error arming transfer.  */

                            /* Release packet.  */
                            nx_packet_release(packet);
                        }
                    }
                    else
                    {

                        /* Link is down.  */

                        /* Signal that we are done arming and resume waiting thread if necessary.  */
                        cdc_ecm -> ux_host_class_cdc_ecm_bulk_in_transfer_check_and_arm_in_process =  UX_FALSE;
                        if (cdc_ecm -> ux_host_class_cdc_ecm_bulk_in_transfer_waiting_for_check_and_arm_to_finish == UX_TRUE)
                            _ux_utility_semaphore_put(&cdc_ecm -> ux_host_class_cdc_ecm_bulk_in_transfer_waiting_for_check_and_arm_to_finish_semaphore);

                        /* Release packet.  */
                        nx_packet_release(packet);
                    }
                }
                else
                {

                    /* Packet allocation timed out. Note that the timeout value is
                       configurable.  */

                    /* Error trap. No need for trace, since NetX does it.  */
                    _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_CLASS, UX_MEMORY_INSUFFICIENT);
                }
            }
        }
        else
        {

            /* The link state is pending down. We need to free the xmit queue.  */
            _ux_host_class_cdc_ecm_transmit_queue_clean(cdc_ecm);

            /* Link state can now be set to down.  */

            /* Notify the network driver.  */
            _ux_network_driver_link_down(cdc_ecm -> ux_host_class_cdc_ecm_network_handle);

            /* Set the link state.  */
            cdc_ecm -> ux_host_class_cdc_ecm_link_state =  UX_HOST_CLASS_CDC_ECM_LINK_STATE_DOWN;
        }
    }    
}
