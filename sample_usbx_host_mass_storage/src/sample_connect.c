/**************************************************************************/
/**                                                                       */
/** Functions for waiting USB memory connection or disconnection.         */
/**                                                                       */
/**   sample_connect.h                                                    */
/**                                                                       */
/**************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdio.h>
#include "tx_api.h"
#include "fx_api.h"
#include "usb_serial.h"	/* printf */


/******************************************************************************
Macro definitions
******************************************************************************/


/******************************************************************************
External variables and functions
******************************************************************************/
extern FX_MEDIA *media;


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
* Function Name: void wait_connect()
* Description  : Wait USB memory is connected.
* Arguments    : None
* Return Value : None
******************************************************************************/
void wait_connect() {
	while (NULL == media) {
		tx_thread_sleep(100);
	}
	printf("USB memory is connected.\n\n");
}


/******************************************************************************
* Function Name: void wait_connect()
* Description  : Wait USB memory is disconnected.
* Arguments    : None
* Return Value : None
******************************************************************************/
void wait_disconnect() {
	while (NULL != media) {
		tx_thread_sleep(100);
	}
	printf("USB memory is disconnected.\n");
}
