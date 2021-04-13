#include "common_hardware_code.h"

#include <stdint.h>
#include "r_intc.h"

/* Define the display dimensions specific to this implemenation.  */
#define DEMO_CANVAS_WIDTH   800
#define DEMO_CANVAS_HEIGHT  480

/* Define the number of pixels on the canvas */
#define DEFAULT_CANVAS_PIXELS  (DEMO_CANVAS_WIDTH * DEMO_CANVAS_HEIGHT)

/* Create memory for the root's canvas. */
EXTERNAL_RAM static GX_COLOR default_canvas_memory[DEFAULT_CANVAS_PIXELS];

/* Create stack and control block for demo thread. */
static TX_THREAD demo_thread;
static ULONG demo_thread_stack[1024];

/* Create stack and control block for touch thread. */
static TX_THREAD touch_thread;
static ULONG touch_thread_stack[1024];

/* Define prototypes.   */
extern VOID demo_thread_entry(ULONG thread_input);
extern VOID touch_thread_entry(ULONG thread_input);
extern UINT gx_studio_display_configure(USHORT display, UINT (*driver)(GX_DISPLAY *), 
                                        USHORT language, USHORT theme, GX_WINDOW_ROOT **return_root);


VOID CreateDemoThread()
{
    tx_thread_create(&demo_thread, "Demo Thread", demo_thread_entry, 0,  
                     demo_thread_stack, sizeof(demo_thread_stack), 
                     GX_SYSTEM_THREAD_PRIORITY + 1,
                     GX_SYSTEM_THREAD_PRIORITY + 1, TX_NO_TIME_SLICE, TX_AUTO_START);
}

VOID CreateInputThread(void)
{
	tx_thread_create(&touch_thread, "GUIX Touch Thread", touch_thread_entry, 0,
					 touch_thread_stack, sizeof(touch_thread_stack),
					 GX_SYSTEM_THREAD_PRIORITY + 1,
					 GX_SYSTEM_THREAD_PRIORITY + 1, TX_NO_TIME_SLICE, TX_AUTO_START);
}

GX_WINDOW_ROOT *CreateDisplay(INT display, UINT (*driver_setup)(GX_DISPLAY *), INT language, INT theme)
{
    GX_WINDOW_ROOT *root;

    gx_studio_display_configure(display, driver_setup, language, theme, &root);
    root->gx_window_root_canvas->gx_canvas_memory = default_canvas_memory;

    return root;
}
