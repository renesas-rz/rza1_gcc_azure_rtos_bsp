
/**************************************************************************/
/**                                                                       */
/** DMA releated functions definition                                     */
/**   These are implemented newly, to work DMA transfer.                  */
/**                                                                       */
/**   usb_dma.c                                                           */
/**                                                                       */
/**************************************************************************/

#include "tx_api.h"	/* ULONG */
#include "ux_api.h"	/* ux_hcd_rz.h */
#include "ux_hcd_rz.h"
#include "usb_serial.h"	/* printf */

#ifdef UX_RZ_HCD_USE_DMA

/******************************************************************************
Local variables and functions
******************************************************************************/

/* whether DMA request is issued or not. */
static UINT single_requested = 0;

static UX_SEMAPHORE    ux_dma_rx_semaphore;

static UINT usb_dma_is_single_requested();


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
* Function Name: void usb_dma_init()
* Description  : Create semaphore to wait DMA completion.
*                This function never return when error occurs.
*                This function is called by main function.
* Arguments    : None
* Return Value : None
******************************************************************************/
void usb_dma_init() {
	UINT status;
	status = _ux_utility_semaphore_create(&ux_dma_rx_semaphore,
			"ux_host_transfer_dma_rx_semaphore", 0);
	if (status != UX_SUCCESS) {
		printf("%s error!\n", __func__);
		while (1) {
			tx_thread_sleep(10);
		}
	}
}

/******************************************************************************
* Function Name: void usb_dma_single_wait()
* Description  : Wait semaphore from DMA interrupt function,
*                only when DMA request is issued.
*                This function never return when error occurs.
*                This function is called by _ux_hcd_rz_entry function.
*
* Arguments    : None
* Return Value : None
******************************************************************************/
void usb_dma_single_wait() {
	UINT status;

	if (usb_dma_is_single_requested()) {

		status = _ux_utility_semaphore_get(&ux_dma_rx_semaphore,
		UX_CONTROL_TRANSFER_TIMEOUT);
		if (status != UX_SUCCESS) {
			printf("%s error!\n", __func__);
			while (1) {
				tx_thread_sleep(10);
			}
		}

		single_requested = 0;

	}
}

/******************************************************************************
* Function Name: void usb_dma_single_put_sem()
* Description  : Send semaphore to usb_dma_single_wait function.
*                This function never return when error occurs.
*                This function is called by
*                __ux_hcd_rz_dma_rx_interrupt_handler function.
* Arguments    : None
* Return Value : None
******************************************************************************/
void usb_dma_single_put_sem() {
	UINT status;

	status = _ux_utility_semaphore_put(&ux_dma_rx_semaphore);
	if (status != UX_SUCCESS) {
		printf("%s error!\n", __func__);
		while (1) {
			tx_thread_sleep(10);
		}
	}
}

/******************************************************************************
* Function Name: void usb_dma_single_requested()
* Description  : Remember DMA request is issued.
*                This function is called by
*                _ux_hcd_rz_fifo_read function.
* Arguments    : None
* Return Value : None
******************************************************************************/
void usb_dma_single_requested()
{
	single_requested = 1;
}

/******************************************************************************
* Function Name: UINT usb_dma_is_single_requested()
* Description  : Check whether DMA request is issued or not.
*                This function is called by
*                usb_dma_single_wait function.
* Arguments    : None
* Return Value : None
******************************************************************************/
UINT usb_dma_is_single_requested()
{
	if(single_requested) {
		return 1;
	}
	return 0;
}

#endif



