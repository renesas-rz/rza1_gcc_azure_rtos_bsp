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
#include "ux_utility.h"

/**************************************************************************/ 
/*                                                                        */ 
/*  FUNCTION                                               RELEASE        */ 
/*                                                                        */ 
/*    _ux_hcd_rz_control_td_add                           PORTABLE C      */ 
/*                                                           5.8          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Thierry Giron                                                       */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*     This function adds a transfer descriptor to an ED                  */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    hcd_rz                              Pointer to RZ controller        */ 
/*    ed                                    Pointer to ED                 */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    Completion Status                                                   */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    _ux_utility_memory_copy               Copy memory block             */ 
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
UINT  _ux_hcd_rz_control_td_add(UX_HCD_RZ *hcd_rz, UX_RZ_ED *ed)
{

UX_RZ_TD        *current_td;
USHORT          register_value;
ULONG           device_address;
UINT            status;
ULONG           device_address_register_value;

    /* Get transmit decriptors. */
    current_td =  ed->ux_rz_ed_head_td;
    
    /* Set TD into response pending state. */
    current_td -> ux_rz_td_status |=  UX_RZ_TD_ACK_PENDING;
         
    /* Check whether setup, data , status. */
    if (current_td -> ux_rz_td_status &  UX_RZ_TD_SETUP_PHASE)
    {
        
        /* We are processing a SETUP phase.  
           Set the device address register if different. */
        device_address = ed -> ux_rz_ed_endpoint -> ux_endpoint_device -> ux_device_address;

        /* Is the device address different from the one stored in the root hub  ?  */ 
        if (device_address != hcd_rz -> ux_hcd_rz_root_hub_device_address)
        {
            /* The device address has changed, so read it again.  */                
            device_address_register_value = _ux_hcd_rz_register_read(hcd_rz, UX_RZ_HC_DEVADD0);

            /* And store it.  Note that the device address is not an index.  */
            _ux_hcd_rz_register_write(hcd_rz, (UX_RZ_HC_DEVADD0 + ( 2 * device_address)), device_address_register_value);

            /* Clear the address field first.  */
            _ux_hcd_rz_register_clear(hcd_rz, UX_RZ_HC_DCPMAXP, UX_RZ_HC_DCPMAXP_DEVADDR_MASK);

            /* Store the new address but leave the MPS fild intact.  */
            _ux_hcd_rz_register_set(hcd_rz, UX_RZ_HC_DCPMAXP, (device_address << UX_RZ_HC_DCPMAXP_DEVADDR_SHIFT) );
            
            /* Update the device address in the HCD structure.  */
            hcd_rz -> ux_hcd_rz_root_hub_device_address = device_address;
            
         }
         
        /* Set the buffer address to be accessed.  */
         _ux_hcd_rz_register_set(hcd_rz, UX_RZ_HC_DCPCTR, UX_RZ_HC_DCPCTR_DATA0);
        
        /* Copy the payload of the control transfer into each register : Request.  */
        register_value =  _ux_utility_short_get( (current_td -> ux_rz_td_buffer));
        _ux_hcd_rz_register_write(hcd_rz, UX_RZ_HC_USBREQ, (ULONG) register_value);
        
        /* Copy the payload of the control transfer into each register : Value.  */
        register_value =  _ux_utility_short_get( (current_td -> ux_rz_td_buffer) + 2 );
        _ux_hcd_rz_register_write(hcd_rz, UX_RZ_HC_USBVAL, (ULONG) register_value);
        
        /* Copy the payload of the control transfer into each register : Index.  */
        register_value =  _ux_utility_short_get( (current_td -> ux_rz_td_buffer) + 4 );
        _ux_hcd_rz_register_write(hcd_rz, UX_RZ_HC_USBINDX, (ULONG) register_value);
        
        /* Copy the payload of the control transfer into each register : Length.  */
        register_value =  _ux_utility_short_get( (current_td -> ux_rz_td_buffer) + 6 );
        _ux_hcd_rz_register_write(hcd_rz, UX_RZ_HC_USBLENG, (ULONG) register_value);
        
        /* DCP -  PIPE0 */
        hcd_rz -> ux_hcd_rz_ed_ptr[0] = ed;
    
        /* Start transmission. */
        _ux_hcd_rz_register_set(hcd_rz, UX_RZ_HC_DCPCTR, UX_RZ_HC_DCPCTR_SUREQ );
        
    }
    else     
    {
            
         /* We are processing data/status stage of control transfer.  Check direction now.  */
        if (current_td -> ux_rz_td_direction == UX_RZ_TD_IN)
        {
        
            /* This is an IN. Reset the PID mask register.  */
            _ux_hcd_rz_register_clear(hcd_rz, UX_RZ_HC_DCPCTR, UX_RZ_HC_DCPCTR_PID_MASK);

            /* Set PIPE0 FIFO in in status. */
            _ux_hcd_rz_current_endpoint_change(hcd_rz, ed, 0);

            /* Set DATA0-DATA1 toggle. */
            if (current_td -> ux_rz_td_toggle == 0)

                /* Set Toggle to DATA0.  */
                 _ux_hcd_rz_register_set(hcd_rz, UX_RZ_HC_DCPCTR,UX_RZ_HC_DCPCTR_DATA0);

            else

                /* Set Toggle to DATA1.  */
                 _ux_hcd_rz_register_set(hcd_rz, UX_RZ_HC_DCPCTR,UX_RZ_HC_DCPCTR_DATA1);
                    
            /* We are doing a read. Reset the Direction bit in the DCPCFG register.  */
            _ux_hcd_rz_register_clear(hcd_rz, UX_RZ_HC_DCPCFG, UX_RZ_HC_DCPCFG_DIR);
                    
            /* Clear the FIFO buffer memory. */
            _ux_hcd_rz_register_write(hcd_rz, UX_RZ_HC_CFIFOCTR, UX_RZ_HC_FIFOCTR_BCLR);

            /* Enable the Buffer empty interrupt.  */
            _ux_hcd_rz_buffer_empty_interrupt(hcd_rz, ed, UX_RZ_ENABLE);

            /* Enable the Ready interrupt.  */
            _ux_hcd_rz_buffer_ready_interrupt(hcd_rz, ed, UX_RZ_ENABLE);
            
            /* Start transmission - set PID to NAK  then  set PID to BUF. */
            _ux_hcd_rz_register_set(hcd_rz, UX_RZ_HC_DCPCTR, UX_RZ_HC_DCPCTR_PIDBUF);

        
        }
        else 
        {
                            
            /* Must be an OUT now.  */                              
            if ( current_td -> ux_rz_td_direction == UX_RZ_TD_OUT )
            {

                /* Clear the FIFO buffer memory. */
                _ux_hcd_rz_register_set(hcd_rz, UX_RZ_HC_CFIFOCTR, UX_RZ_HC_FIFOCTR_BCLR);
                
                /* Set PID to NAK.  */
                _ux_hcd_rz_register_clear(hcd_rz, UX_RZ_HC_DCPCTR, UX_RZ_HC_DCPCTR_PID_MASK);

                /* Set DATA0-DATA1 toggle. */
                if (current_td -> ux_rz_td_toggle == 0)

                    /* Set Toggle to DATA0.  */
                     _ux_hcd_rz_register_set(hcd_rz, UX_RZ_HC_DCPCTR,UX_RZ_HC_DCPCTR_DATA0);
                
                else

                    /* Set Toggle to DATA1.  */
                     _ux_hcd_rz_register_set(hcd_rz, UX_RZ_HC_DCPCTR,UX_RZ_HC_DCPCTR_DATA1);

                /* Write the buffer to the pipe.  */
                status =  _ux_hcd_rz_buffer_write(hcd_rz, ed); 

                /* Check status.  */
                if (status != UX_ERROR)         

                    /* If no error, we can set the pipe PID_BUF flag.  */
                    _ux_hcd_rz_register_set(hcd_rz, UX_RZ_HC_DCPCTR, UX_RZ_HC_DCPCTR_PIDBUF);
         
            }
        }
    }       

    /* Return successful completion.  */
    return(UX_SUCCESS);
}

