
#ifndef _USB_MISC_H_	/* grape */
#define _USB_MISC_H_

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "tx_api.h"	/* TX_TIMER_TICKS_PER_SECOND */

/******************************************************************************
Macro definitions
******************************************************************************/
#define TICK_MS (1000/TX_TIMER_TICKS_PER_SECOND)
#define delay(x)	tx_thread_sleep( x / TICK_MS )

#endif	/* _USB_MISC_H_ */
