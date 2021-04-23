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
/*    _ux_hcd_rz_interrupt_handler                        PORTABLE C      */ 
/*                                                           5.8          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Thierry Giron                                                       */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*     This function is the interrupt handler for the RZ interrupts.      */
/*     Normally an interrupt occurs from the controller when there is     */ 
/*     either a EOF signal and there has been transfers within the frame  */ 
/*     or when there is a change on one of the downstream ports.          */
/*                                                                        */
/*     All we need to do in the ISR is scan the controllers to find out   */ 
/*     which one has issued a IRQ. If there is work to do for this        */ 
/*     controller we need to wake up the corresponding thread to take     */ 
/*     care of the job.                                                   */ 
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
/*    _ux_hcd_rz_register_read               Read RZ HCOR register        */ 
/*    _ux_hcd_rz_register_write              Write RZ HCOR register       */ 
/*    _ux_utility_semaphore_put                Put producer semaphore     */ 
/*                                                                        */ 
/*  CALLED BY                                                             */ 
/*                                                                        */ 
/*    ThreadX Interrupt Handler                                           */ 
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
VOID  _ux_hcd_rz_interrupt_handler(UINT int_sense)
{

UINT                hcd_index;
UX_HCD              *hcd;
UX_HCD_RZ           *hcd_rz;
ULONG               bempsts, nrdysts, brdysts;
ULONG               bemp_bit, nrdy_bit, brdy_bit;
ULONG               endpoint_count;
ULONG               intenb0;
ULONG               intenb1;


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

            /* Increase the interrupt count. This indicates the controller is still alive.  */
            hcd_rz -> ux_hcd_rz_interrupt_count++;

            /* Check if the controller is operational, if not, skip it.  */
            if (hcd -> ux_hcd_status == UX_HCD_STATUS_OPERATIONAL)
            {

               /* We get the current enabled interrupt. */
                intenb0 =  _ux_hcd_rz_register_read(hcd_rz, UX_RZ_HC_INTENB0);
                intenb1 =  _ux_hcd_rz_register_read(hcd_rz, UX_RZ_HC_INTENB1);

               /* We get the current interrupt status  registers and mask them with the interrupts enabled.  */
                hcd_rz->ux_hcd_rz_intsts0 =  _ux_hcd_rz_register_read(hcd_rz, UX_RZ_HC_INTSTS0) & intenb0;
                hcd_rz->ux_hcd_rz_intsts1 =  _ux_hcd_rz_register_read(hcd_rz, UX_RZ_HC_INTSTS1) & intenb1;

                /* Examine the source of interrupts.  Check for SOF signal.  */
                if (hcd_rz->ux_hcd_rz_intsts0 & UX_RZ_HC_INTSTS0_SOFR) 
                {

                    /* Acknowledge interrupt  - wake up scheduler. */
                    _ux_hcd_rz_register_clear(hcd_rz, UX_RZ_HC_INTSTS0, UX_RZ_HC_INTSTS0_SOFR);

                    /* We have a SOF signal, we should find out what happened during the last frame and see if  there is anything to do in the next frame.  */
                    /* these interrupts are automatically acknowleged when all the pipe-buffer interrupts are handled */
                    /* Wake up the scheduler but we cannot safely mark the queue empty yet. This will be done  when we find that the scheduling queue is empty.  */
                    hcd -> ux_hcd_thread_signal++;
                    _ux_utility_semaphore_put(&_ux_system_host -> ux_system_host_hcd_semaphore);
                
                }

                /* Check if we have a BEMP interrupt.  */
                if (hcd_rz->ux_hcd_rz_intsts0 & UX_RZ_HC_INTSTS0_BEMP )
                {

                    /* Check BEMP irq flags. */
                    bempsts =  _ux_hcd_rz_register_read(hcd_rz, UX_RZ_HC_BEMPSTS);
                    bemp_bit = 1;


                    /* 10 endpoints to check.  */
                    for (endpoint_count = 0; endpoint_count <= 9; endpoint_count++)
                    {
                        /* if flag set, mark BEMP in ed status */
                        if( bempsts & bemp_bit )
                        { 
                            if (hcd_rz -> ux_hcd_rz_ed_ptr[endpoint_count])
                                hcd_rz -> ux_hcd_rz_ed_irq[endpoint_count] = hcd_rz -> ux_hcd_rz_ed_irq[endpoint_count] | UX_RZ_HC_ED_BEMP ;
                            _ux_hcd_rz_register_clear(hcd_rz, UX_RZ_HC_BEMPSTS,bemp_bit);
                        }
                        bemp_bit = bemp_bit << 1;
                    }
                    
                    /* Increment the thread signal.  */
                    hcd -> ux_hcd_thread_signal++;

                    /* And wake up the HCD thread.  */
                    _ux_utility_semaphore_put(&_ux_system_host -> ux_system_host_hcd_semaphore);
                }                   
                    
                /* Do we have a BRDY interrupt ? */
                if (hcd_rz->ux_hcd_rz_intsts0 &  UX_RZ_HC_INTSTS0_BRDY )
                {
                    
                    /* Check BRDY irq flags. */
                    brdysts =  _ux_hcd_rz_register_read(hcd_rz, UX_RZ_HC_BRDYSTS);
                    brdy_bit = 1;

                    for (endpoint_count = 0; endpoint_count <= 9; endpoint_count++)
                    {
                        
                        /* If flag set, mark BRDY in ed status. */
                        if( brdysts & brdy_bit )
                        { 
                            if (hcd_rz -> ux_hcd_rz_ed_ptr[endpoint_count] )
                                hcd_rz -> ux_hcd_rz_ed_irq[endpoint_count] = hcd_rz -> ux_hcd_rz_ed_irq[endpoint_count] | UX_RZ_HC_ED_BRDY ;
                            _ux_hcd_rz_register_clear(hcd_rz, UX_RZ_HC_BRDYSTS,brdy_bit);
                        }
                        brdy_bit = brdy_bit << 1;
                    }

                    /* Increment the thread signal.  */
                    hcd -> ux_hcd_thread_signal++;

                    /* And wake up the HCD thread.  */
                    _ux_utility_semaphore_put(&_ux_system_host -> ux_system_host_hcd_semaphore);  
                }                       
                    
                /* Do we have a NRDY interrupt ?  */
                if (hcd_rz->ux_hcd_rz_intsts0 &  UX_RZ_HC_INTSTS0_NRDY )
                {

                    /* Check NRDY irq flags. */
                    nrdysts =  _ux_hcd_rz_register_read(hcd_rz, UX_RZ_HC_NRDYSTS);
                    nrdy_bit = 1;

                    for (endpoint_count = 0; endpoint_count <= 9; endpoint_count++)
                    {
                  
                        /* If flag set, mark NRDY in ed status. */
                        if( nrdysts & nrdy_bit )
                        { 
                            if (hcd_rz -> ux_hcd_rz_ed_ptr[endpoint_count] )
                                hcd_rz -> ux_hcd_rz_ed_irq[endpoint_count] = hcd_rz -> ux_hcd_rz_ed_irq[endpoint_count] | UX_RZ_HC_ED_NRDY ;
                            _ux_hcd_rz_register_clear(hcd_rz, UX_RZ_HC_NRDYSTS, nrdy_bit);
                        }
                        nrdy_bit = nrdy_bit << 1;
                    }

                    hcd -> ux_hcd_thread_signal++;
                    _ux_utility_semaphore_put(&_ux_system_host -> ux_system_host_hcd_semaphore); 
                }                                 
            
            

                /* Check for attach signal.  */
                if (hcd_rz -> ux_hcd_rz_intsts1 & (UX_RZ_HC_INTSTS1_ATTCH) )
                {

                     /* Acknowledge interrupt. */
                    _ux_hcd_rz_register_clear(hcd_rz, UX_RZ_HC_INTSTS1, UX_RZ_HC_INTSTS1_ATTCH);
                    
                    /* Attach only if nothing was previously attached and if the port is not enabled.  */
                    if (hcd_rz -> ux_rz_device_attached == UX_FALSE && hcd_rz -> ux_hcd_rz_port_status != UX_RZ_HC_PORT_ENABLED) 
                    {
                    
                        /* Clear the ATTCH interrupt enabled.  */
                        _ux_hcd_rz_register_clear(hcd_rz, UX_RZ_HC_INTENB1, UX_RZ_HC_INTENB1_ATTCHE);
                    
                        /* Enable the DTCH interrupt.  */
                        _ux_hcd_rz_register_set(hcd_rz, UX_RZ_HC_INTENB1, UX_RZ_HC_INTENB1_DTCHE);
                    
                        /* Something happened on this port. Signal it to the root hub thread.  */
                        hcd -> ux_hcd_root_hub_signal[0]++;
                    
                        /* Something is attached now.  */
                        hcd_rz -> ux_rz_device_attached = UX_TRUE;
                        
                        /* Reset the address of the device connected to the root hub.  */
                        _ux_hcd_rz_register_set(hcd_rz, UX_RZ_HC_DCPMAXP, 0);
                        
                        /* Update the device address in the HCD structure.  We will force a resetting later. */
                        hcd_rz -> ux_hcd_rz_root_hub_device_address = 0xffffffff;
                    
                        /* We only wake up the root hub thread if there has been device insertion/extraction.  */
                        _ux_utility_semaphore_put(&_ux_system_host -> ux_system_host_enum_semaphore);
                    }
                }

                /* Is it a detach signal ? */
                if (hcd_rz -> ux_hcd_rz_intsts1 & (UX_RZ_HC_INTSTS1_DTCH ) )
                {
                     /* Acknowledge interrupt.  */
                    _ux_hcd_rz_register_clear(hcd_rz, UX_RZ_HC_INTSTS1, UX_RZ_HC_INTSTS1_DTCH);
                    
                    /* Detach only of something was previously attached and if the port is enabled
                       to avoid spurious dtach signals.  */
                    if (hcd_rz -> ux_rz_device_attached == UX_TRUE && hcd_rz -> ux_hcd_rz_port_status == UX_RZ_HC_PORT_ENABLED)
                    {

                        /* Clear the DTCH interrupt enabled.  */
                        _ux_hcd_rz_register_clear(hcd_rz, UX_RZ_HC_INTENB1, UX_RZ_HC_INTENB1_DTCHE);
                    
                        /* Enable the ATTCH interrupt.  */
                        _ux_hcd_rz_register_set(hcd_rz, UX_RZ_HC_INTENB1, UX_RZ_HC_INTENB1_ATTCHE);

                        /* Mark all endpoints as transaction timeout. This is necessary because
                           the RZ USB IP does not detect an error if the device is removed. */
                        for (endpoint_count = 0; endpoint_count <= 9; endpoint_count++)
                        {
                            /* Check endpoint.  */          
                            if (hcd_rz -> ux_hcd_rz_ed_ptr[endpoint_count] )
                                hcd_rz -> ux_hcd_rz_ed_irq[endpoint_count] = hcd_rz -> ux_hcd_rz_ed_irq[endpoint_count] | UX_RZ_HC_ED_TIMEOUT ;
                        }
        
                        /* Wake up the HCD thread.  */
                        hcd -> ux_hcd_thread_signal++;
                        _ux_utility_semaphore_put(&_ux_system_host -> ux_system_host_hcd_semaphore); 

                        /* Something happened on this port. Signal it to the root hub thread.  */
                        hcd -> ux_hcd_root_hub_signal[0]++;
                    
                        /* Something is getting detached now. */
                        hcd_rz -> ux_rz_device_attached = UX_FALSE ;
                        
                        /* We only wake up the root hub thread if there has been device insertion/extraction.  */
                        _ux_utility_semaphore_put(&_ux_system_host -> ux_system_host_enum_semaphore);
                    }
                }


                /* Check for BCHG signal.  */
                if (hcd_rz->ux_hcd_rz_intsts1 & (UX_RZ_HC_INTSTS1_BCHG) )
                {

                     /* Acknowledge interrupt. */
                    _ux_hcd_rz_register_clear(hcd_rz, UX_RZ_HC_INTSTS1, UX_RZ_HC_INTSTS1_BCHG);
                    
                    /* Attach only if nothing was previously attached and if the port is not enabled.  */
                    if (hcd_rz -> ux_rz_device_attached == UX_FALSE && hcd_rz -> ux_hcd_rz_port_status != UX_RZ_HC_PORT_ENABLED) 
                    {
                    
                        /* Clear the BCHG interrupt enabled.  */
                        _ux_hcd_rz_register_clear(hcd_rz, UX_RZ_HC_INTENB1, UX_RZ_HC_INTENB1_BCHGE);
                    
                        /* Enable the DTCH interrupt.  */
                        _ux_hcd_rz_register_set(hcd_rz, UX_RZ_HC_INTENB1, UX_RZ_HC_INTENB1_DTCHE);
                    
                        /* Something happened on this port. Signal it to the root hub thread.  */
                        hcd -> ux_hcd_root_hub_signal[0]++;
                    
                        /* Something is attached now.  */
                        hcd_rz -> ux_rz_device_attached = UX_TRUE;
                        
                        /* Reset the address of the device connected to the root hub.  */
                        _ux_hcd_rz_register_set(hcd_rz, UX_RZ_HC_DCPMAXP, 0);
                        
                        /* Update the device address in the HCD structure.  We will force a resetting later. */
                        hcd_rz -> ux_hcd_rz_root_hub_device_address = 0xffffffff;
                    
                        /* We only wake up the root hub thread if there has been device insertion/extraction.  */
                        _ux_utility_semaphore_put(&_ux_system_host -> ux_system_host_enum_semaphore);
                    }
                }
                
                /* Did we get a SACK interrupt ?  */
                if (hcd_rz->ux_hcd_rz_intsts1 &  UX_RZ_HC_INTSTS1_SACK )
                {
                    /*  Clear interrupt. */
                    _ux_hcd_rz_register_write(hcd_rz, UX_RZ_HC_INTSTS1, ~UX_RZ_HC_INTSTS1_SACK );
                    
                    /* SACK can only happen on EP0.  */
                    hcd_rz -> ux_hcd_rz_ed_irq[0] = hcd_rz -> ux_hcd_rz_ed_irq[0] | UX_RZ_HC_ED_SACK ;
                   
                    hcd -> ux_hcd_thread_signal++;
                    _ux_utility_semaphore_put(&_ux_system_host -> ux_system_host_hcd_semaphore); 
                }                                 

                /* Did we get a SIGN interrupt ?  */
                if (hcd_rz->ux_hcd_rz_intsts1 &  UX_RZ_HC_INTSTS1_SIGN )
                {
                    /*  Clear interrupt. */
                    _ux_hcd_rz_register_write(hcd_rz, UX_RZ_HC_INTSTS1, ~UX_RZ_HC_INTSTS1_SIGN );
                    
                    /* SIGN can only happen on EP0.  */
                    hcd_rz -> ux_hcd_rz_ed_irq[0] = hcd_rz -> ux_hcd_rz_ed_irq[0] | UX_RZ_HC_ED_SIGN ;
                   
                    hcd -> ux_hcd_thread_signal++;
                    _ux_utility_semaphore_put(&_ux_system_host -> ux_system_host_hcd_semaphore); 
                }                                 


                /* The sample driver does this. */
                _ux_hcd_rz_register_read(hcd_rz, UX_RZ_HC_INTSTS1);
                _ux_hcd_rz_register_read(hcd_rz, UX_RZ_HC_INTSTS1);
                _ux_hcd_rz_register_read(hcd_rz, UX_RZ_HC_INTSTS1);
                                
            }                   
        }
    }
}


