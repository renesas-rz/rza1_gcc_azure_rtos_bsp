
/**************************************************************************/
/**                                                                       */
/** Non cached global variables                                           */
/**    sd_mem :   memory for SD driver workarea                           */
/**    fx_mem :   filex application read/write buffer                     */
/**                                                                       */
/**   usb_mem.c                                                           */
/**                                                                       */
/**************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "usb_mem.h"

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
/* Define non-cache memory space below */
/* When using cache memory space,
 * comment out "__attribute__((section(".VRAM_SECTION0")))"
 */

#ifdef __GNUC__

/* buffer for transfer sector data, used FileX application. */
ALIGN_32BYTES(
unsigned long test_usb_rw_buff[ FX_MEM_SIZE / sizeof(unsigned long) ]
__attribute__((section(".VRAM_SECTION0")))
);

#endif /* __GNUC__ */
