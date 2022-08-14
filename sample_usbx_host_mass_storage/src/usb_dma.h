
/**************************************************************************/
/**                                                                       */
/** DMA releated macros and functions definition                          */
/**   These are implemented newly, to work DMA transfer.                  */
/**                                                                       */
/**   usb_dma.h                                                           */
/**                                                                       */
/**************************************************************************/

#ifndef _USB_DMA_H_
#define _USB_DMA_H_

#include "ux_hcd_rz.h"

/******************************************************************************
Macro definitions
******************************************************************************/

#if 0
/* These macros are implemented for continuous DMA.
 * At this moment, continuous DMA is not implemented.
 */

#define UX_RZ_HC_D0FIFOB0                           (UX_RZ_USB_BASE + 0x160)
#define UX_RZ_HC_D0FIFOB1                           (UX_RZ_USB_BASE + 0x164)
#define UX_RZ_HC_D0FIFOB2                           (UX_RZ_USB_BASE + 0x168)
#define UX_RZ_HC_D0FIFOB3                           (UX_RZ_USB_BASE + 0x16C)
#define UX_RZ_HC_D0FIFOB4                           (UX_RZ_USB_BASE + 0x170)
#define UX_RZ_HC_D0FIFOB5                           (UX_RZ_USB_BASE + 0x174)
#define UX_RZ_HC_D0FIFOB6                           (UX_RZ_USB_BASE + 0x178)
#define UX_RZ_HC_D0FIFOB7                           (UX_RZ_USB_BASE + 0x17C)

#define UX_RZ_HC_D1FIFOB0                           (UX_RZ_USB_BASE + 0x180)
#define UX_RZ_HC_D1FIFOB1                           (UX_RZ_USB_BASE + 0x184)
#define UX_RZ_HC_D1FIFOB2                           (UX_RZ_USB_BASE + 0x188)
#define UX_RZ_HC_D1FIFOB3                           (UX_RZ_USB_BASE + 0x18C)
#define UX_RZ_HC_D1FIFOB4                           (UX_RZ_USB_BASE + 0x190)
#define UX_RZ_HC_D1FIFOB5                           (UX_RZ_USB_BASE + 0x194)
#define UX_RZ_HC_D1FIFOB6                           (UX_RZ_USB_BASE + 0x198)
#define UX_RZ_HC_D1FIFOB7                           (UX_RZ_USB_BASE + 0x19C)

#define UX_RZ_HC_DFBCFG_DFACC_CYCLE                (0<<12)
#define UX_RZ_HC_DFBCFG_DFACC_16                   (1<<12)
#define UX_RZ_HC_DFBCFG_DFACC_32                   (2<<12)
#define UX_RZ_HC_DFBCFG_DFACC_MASK                 (3<<12)

#define UX_RZ_HC_DFBCFG_TENDE_NOSMP                (0<<4)
#define UX_RZ_HC_DFBCFG_TENDE_SMP                  (1<<4)

#endif


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

void usb_dma_init();
void usb_dma_single_wait();
void usb_dma_single_put_sem();
void usb_dma_single_requested();


#endif	/* _USB_DMA_H_ */
