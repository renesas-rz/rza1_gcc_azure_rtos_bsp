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
#include "rza_io_regrw.h"      /* Low level register read/write header */
#include "r_gpio_if.h"
#include "mcu_board_select.h"

#define USB_PRI_DMA_TX (23)
#define USB_PRI_DMA_RX (24)
#define USB_PRI_USBI0 (28)
#define USB_PRI_USBI1 (27)

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
CPG.STBCR7 &= (uint8_t)~(CPG_STBCR7_MSTP71);

/* Register USB interrupt handler function */
R_INTC_RegistIntFunc(INTC_ID_USBI0,  (void (*)(uint32_t))_ux_dcd_rz_interrupt_handler);

/* Set interrupt priority */
R_INTC_SetPriority(INTC_ID_USBI0, USB_PRI_USBI0);

/* A/D end interrupt enable */
R_INTC_Enable(INTC_ID_USBI0);

#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
	gpio_init(P7_1);
	gpio_dir(P7_1, PIN_OUTPUT);
	gpio_write(P7_1, 1);
#endif

#else
    /* turn on USB1 clock */
   // CPG.STBCR7.BIT.MSTP70 = 0;

    /* turn on USB1 clock */
	//CPG.STBCR7 &= 0xFE;
	CPG.STBCR7 &= (uint8_t)~(CPG_STBCR7_MSTP70|CPG_STBCR7_MSTP71);
	dummy = CPG.STBCR7;

    /* Register ETHER interrupt handler function */
    R_INTC_RegistIntFunc(INTC_ID_USBI1, (void (*)(uint32_t))_ux_dcd_rz_interrupt_handler);

    /* Set interrupt priority */
	R_INTC_SetPriority(INTC_ID_USBI1, USB_PRI_USBI1);

	/* A/D end interrupt enable */
	R_INTC_Enable(INTC_ID_USBI1);
#endif

#ifdef UX_RZ_DCD_USE_DMA

    /* Register DMA TX interrupt handler function */
    R_INTC_RegistIntFunc(UX_RZ_DMA_TX_INT_ID, (void (*)(uint32_t))_ux_dcd_rz_dma_tx_interrupt_handler);
    
    /* Set interrupt priority */
	R_INTC_SetPriority(UX_RZ_DMA_TX_INT_ID, USB_PRI_DMA_TX);

	/* A/D end interrupt enable */
	R_INTC_Enable(UX_RZ_DMA_TX_INT_ID);


    /* Register DMA RX interrupt handler function */
    R_INTC_RegistIntFunc(UX_RZ_DMA_RX_INT_ID, (void (*)(uint32_t))_ux_dcd_rz_dma_rx_interrupt_handler);
    
    /* Set interrupt priority */
	R_INTC_SetPriority(UX_RZ_DMA_RX_INT_ID, USB_PRI_DMA_RX);

	 /* Enable DMA interrupt.  */
	R_INTC_Enable(UX_RZ_DMA_RX_INT_ID);
#endif

}

