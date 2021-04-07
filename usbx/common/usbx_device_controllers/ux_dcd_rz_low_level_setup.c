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
#include "ux_dcd_rz.h"
#include "tx_api.h"
#include "iodefine.h"
#include "r_intc.h"            /* INTC Driver Header */



/**************************************************************************/ 
/*                                                                        */ 
/*  FUNCTION                                               RELEASE        */ 
/*                                                                        */ 
/*    ux_rz_dcd_low_level_setup                           PORTABLE C      */ 
/*                                                           5.8          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Thierry Giron                                                       */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*     This function sets up the RZ DCD controller.                       */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*      None                                                              */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*      None                                                              */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*      Set_IntVect                         Set USB interrupt vector      */ 
/*                                                                        */ 
/*  CALLED BY                                                             */ 
/*                                                                        */ 
/*      _ux_hcd_rz_initialize               USBX HCD RZ initialization    */ 
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
void  _ux_dcd_rz_low_level_setup(void)
{

ULONG   iprreg;

#if UX_RZ_USB_BASE == UX_RZ_USB0_BASE
    /* turn on USB0 clock */
    //CPG.STBCR7.BIT.MSTP71 = 0;
    CPG.STBCR7 &= 0xFD;
   
    /* Register USB interrupt handler function */
    R_INTC_RegistIntFunc(INTC_ID_USBI0, (void (*)(uint32_t))_ux_dcd_rz_interrupt_handler);
    
    /* enable USB0 interrupt */
    INTC.ICDISER2 |= (1UL << 9) ;
    
    /* Set Priorities of USBIO (GROUP:4,SUB:0)*/
    iprreg = INTC.ICDIPR18;
    iprreg &= ~(0xFFUL << 8);
    iprreg |=  (0xA0UL << 8);
    INTC.ICDIPR18 = iprreg;
#else
    /* turn on USB1 clock */
    CPG.STBCR7.BIT.MSTP70 = 0;
   
    /* Register USB interrupt handler function */
    R_INTC_RegistIntFunc(INTC_ID_USBI1, _ux_dcd_rz_interrupt_handler);
    
    /* enable USB1 interrupt */
    INTC.ICDISER2 |= (1UL << 10) ;
    
    /* Set Priorities of USBI1 (GROUP:4,SUB:0)*/
    iprreg = INTC.ICDIPR18;
    iprreg &= ~(0xFFUL << 16);
    iprreg |=  (0xA0UL << 16);
    INTC.ICDIPR18 = iprreg;
#endif

#ifdef UX_RZ_DCD_USE_DMA

    /* Register DMA TX interrupt handler function */
    R_INTC_RegistIntFunc(UX_RZ_DMA_TX_INT_ID, (void (*)(uint32_t))_ux_dcd_rz_dma_tx_interrupt_handler);
    
    /* Enable DMA interrupt.  */
    INTC.ICDISER1 |= (1UL << 19) ;
    
    /* Set Priorities of DMA interrupt (GROUP:4,SUB:0)*/
    iprreg = INTC.ICDIPR12;
    iprreg &= ~(0xFFUL << 24);
    iprreg |=  (0x80UL << 24);
    INTC.ICDIPR12 = iprreg;

    /* Register DMA RX interrupt handler function */
    R_INTC_RegistIntFunc(UX_RZ_DMA_RX_INT_ID, (void (*)(uint32_t))_ux_dcd_rz_dma_rx_interrupt_handler);
    
    /* Enable DMA interrupt.  */
    INTC.ICDISER1 |= (1UL << 20) ;
    
    /* Set Priorities of DMA interrupt (GROUP:4,SUB:0)*/
    iprreg = INTC.ICDIPR13;
    iprreg &= ~(0xFFUL << 0);
    iprreg |=  (0x80UL << 0);
    INTC.ICDIPR13 = iprreg;
#endif

}

