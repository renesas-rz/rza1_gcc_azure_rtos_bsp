#ifndef COMMON_HARDWARE_CODE_HEADER
#define COMMON_HARDWARE_CODE_HEADER

#include "tx_api.h"
#include "gx_api.h"

#define EXTERNAL_RAM

/* Define prototypes. */
VOID hardware_setup();
VOID CreateDemoThread();
VOID CreateInputThread();
GX_WINDOW_ROOT *CreateDisplay(INT display, UINT (*driver_setup)(GX_DISPLAY *), INT language, INT theme);

#endif
