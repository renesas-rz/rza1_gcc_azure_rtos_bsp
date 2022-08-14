
#ifndef _USB_SERIAL_H_	/* grape */
#define _USB_SERIAL_H_

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "tx_api.h"	/* TX_TIMER_TICKS_PER_SECOND */

/******************************************************************************
Macro definitions
******************************************************************************/
#define printf		usb_printf
#define scanf		usb_scanf

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
int usb_printf( const char *fmt, ... );
int usb_scanf( const char *fmt, ... );
char *usb_gets( char* buffer, int max );
void usb_dump( char* label, unsigned char* buf, int bytes );
void usb_sample();

#endif	/* _USB_SERIAL_H_ */
