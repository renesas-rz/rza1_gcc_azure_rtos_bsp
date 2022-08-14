
/**************************************************************************/
/**                                                                       */
/** USB application transfer buffer                                       */
/**                                                                       */
/**   usb_mem.h                                                           */
/**                                                                       */
/**************************************************************************/

#ifndef _USB_MEM_H_
#define _USB_MEM_H_

/******************************************************************************
Macro definitions
******************************************************************************/
#include "mcu_board_select.h"
#if (TARGET_BOARD == TARGET_BOARD_RSK)
/* USB transfer buffer size */
#define FX_MEM_SIZE	( 1 * 1024 * 1024 )
#elif  (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
/* USB transfer buffer size */
#define FX_MEM_SIZE	( 1 * 512 * 1024 )
#endif

/* convert byte to MB */
#define MB(bytes)	(bytes/1024/1024)

#if defined   (__GNUC__)        /* GNU Compiler */
  #define ALIGN_32BYTES(buf)  buf __attribute__ ((aligned (32)))
#endif

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
extern
unsigned long test_usb_rw_buff[ FX_MEM_SIZE / sizeof(unsigned long) ];

#endif	/* _USB_MEM_H_ */
