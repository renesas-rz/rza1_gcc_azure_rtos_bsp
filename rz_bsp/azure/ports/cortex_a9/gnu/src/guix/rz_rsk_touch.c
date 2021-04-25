
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#include "r_typedefs.h"
#include "lcd_panel.h"

#include "tp_if.h"
#include "r_intc.h"
#include "rza_io_regrw.h"
#include "r_task_priority.h"

#include "tx_api.h"
#include "gx_api.h"

#define GRAPHICS_LAYER_WIDTH    (LCD_CH0_DISP_HW)
#define GRAPHICS_LAYER_HEIGHT   (LCD_CH0_DISP_VW)

#define TP_INT_PRI              (10uL)
#define TP_TSK_PRI              (GX_SYSTEM_THREAD_PRIORITY+1)

#define TOUCH_STATE_TOUCHED  1
#define TOUCH_STATE_RELEASED 2
#define MIN_DRAG_DELTA       10

static int32_t touch_get_coordinate ();
void GUIX_touch_interrupt(uint32_t int_sense);
static void tpevt_cb_func( int_t nId, TP_TouchEvent_st* psTouchEvt );

static int_t hi2c0 = ( -1);
static int32_t adc_x;
static int32_t adc_y;
static int32_t last_pos_x;
static int32_t last_pos_y;
static int32_t bad_reads = 0;
static TX_EVENT_FLAGS_GROUP  touch_events;

int last_touch_state;
int current_touch_state;
static uint8_t touch_raw_data[32];

/**********************************************************************/
VOID SendPenDownEvent(VOID)
{
    GX_EVENT event;
    event.gx_event_type = GX_EVENT_PEN_DOWN;
    event.gx_event_payload.gx_event_pointdata.gx_point_x = adc_x;
    event.gx_event_payload.gx_event_pointdata.gx_point_y = adc_y;
    event.gx_event_sender = 0;
    event.gx_event_target = 0;
    event.gx_event_display_handle = 0;
    gx_system_event_send(&event);
    last_pos_x = adc_x;
    last_pos_y = adc_y;
    last_touch_state = TOUCH_STATE_TOUCHED;
}

/**************************************************************************/
VOID SendPenDragEvent(VOID)
{
    GX_EVENT event;
    int x_delta = abs(adc_x - last_pos_x);
    int y_delta = abs(adc_y - last_pos_y);

    if (x_delta > MIN_DRAG_DELTA || y_delta > MIN_DRAG_DELTA)
    {
        event.gx_event_type = GX_EVENT_PEN_DRAG;
        event.gx_event_payload.gx_event_pointdata.gx_point_x = adc_x;
        event.gx_event_payload.gx_event_pointdata.gx_point_y = adc_y;
        event.gx_event_sender = 0;
        event.gx_event_target = 0;
        event.gx_event_display_handle = 0;
        last_pos_x = adc_x;
        last_pos_y = adc_y;
    
        gx_system_event_fold(&event);
    }
}

/**************************************************************************/
VOID SendPenUpEvent(VOID)
{
    GX_EVENT event;
    event.gx_event_type = GX_EVENT_PEN_UP;
    event.gx_event_payload.gx_event_pointdata.gx_point_x = adc_x;
    event.gx_event_payload.gx_event_pointdata.gx_point_y = adc_y;
    event.gx_event_sender = 0;
    event.gx_event_target = 0;
    event.gx_event_display_handle = 0;
    last_pos_x = adc_x;
    last_pos_y = adc_y;
    gx_system_event_send(&event);
    last_touch_state = TOUCH_STATE_RELEASED;
}


/**************************************************************************/
VOID  touch_thread_entry(ULONG thread_input)
{
	/* initialise capacitive touch screen */
	TouchPanel_Init();

	/* open the I2C channel1 driver */
	if(0 <= TouchPanel_Open(GRAPHICS_LAYER_WIDTH, GRAPHICS_LAYER_HEIGHT, TP_INT_PRI, TP_TSK_PRI, 4000))
	{
		/* Register a Touch event handler */
		TouchPanel_EventEntry(TPEVT_ENTRY_ALL, 0, 0, GRAPHICS_LAYER_WIDTH, GRAPHICS_LAYER_HEIGHT, &tpevt_cb_func);

		while(1)
		{
			tx_thread_sleep(2000);
		}

	}
}


/****************************************************************************/
/* Function Name : tpevt_cb_func                                            */
/* Explanation   : touch event callback function                            */
/* PARAMETERS    : [IN]nId        : event ID                                */
/*               : [IN]psTouchEvt : touch information                       */
/* RETURNS       : none                                                     */
/* NOTES         : Especially, none.                                        */
/****************************************************************************/
static void tpevt_cb_func( int_t nId, TP_TouchEvent_st* psTouchEvt )
{
    UNUSED_PARAM(nId);
    adc_x = psTouchEvt->sFinger[0].unPosX;
    adc_y = psTouchEvt->sFinger[0].unPosY;

    switch(psTouchEvt->sFinger[0].eState)
    {
        case TPEVT_ENTRY_UP:
        	SendPenUpEvent();
            break;
        case TPEVT_ENTRY_DOWN:
        	SendPenDownEvent();
            break;
        case TPEVT_ENTRY_MOVE:
        	SendPenDragEvent();
            break;
        default:
            break;
    }
}
/***********************************************************************************************************************
 End of function touch_task
 ***********************************************************************************************************************/



