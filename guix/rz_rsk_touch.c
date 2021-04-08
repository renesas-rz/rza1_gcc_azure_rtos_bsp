
#include <stdio.h>
#include "r_typedefs.h"
#include "dev_drv.h"
#include "iodefine.h"
#include "rza_io_regrw.h"
#include "lcd_controller_if.h"

#include "tx_api.h"
#include "gx_api.h"

#define TP_INT_PRI              (10uL)
#define TP_TSK_PRI              (7uL)

#define TOUCH_STATE_TOUCHED  1
#define TOUCH_STATE_RELEASED 2
#define MIN_DRAG_DELTA       10

#define TOUCH_X_RANGE  800
#define TOUCH_Y_RANGE  480

static int32_t touch_get_coordinate ();
void GUIX_touch_interrupt(uint32_t int_sense);


int32_t adc_x;
int32_t adc_y;
int32_t last_pos_x;
int32_t last_pos_y;
int32_t bad_reads = 0;
TX_EVENT_FLAGS_GROUP  touch_events;

int last_touch_state;
int current_touch_state;
static uint8_t touch_raw_data[32];


/**************************************************************************/
static int32_t touch_get_coordinate ()
{
    /* the touch controller sends a 32-byte packet formatted as follows

        byte0- devide mode
        byte1- gesture id
        byte2- number of touch points
        byte3-8  first touch data
        byte9-14 second touch data
        etc etc
    */
    int32_t ret;

    ret = R_LCD_ReadCmd(LCD_SLAVE_ADDRESS, 0, touch_raw_data, 32);
    if (DEVDRV_SUCCESS == ret)
    {
        // test for invalid data
        if (touch_raw_data[0] == 0xff ||
            touch_raw_data[2] > 5)
        {
            // touch controller is not reponding. Try to reset I2C channel:
        	R_LCD_Close();
        	touch_interrupt_configure();
            return GX_FALSE;
        }
        else if (
          (touch_raw_data[0] == 0) &&
          (touch_raw_data[1] == 0) &&
          (touch_raw_data[2] == 0) &&
          (touch_raw_data[3] == 0) &&
          (touch_raw_data[4] == 0) &&
          (touch_raw_data[5] == 0) &&
          (touch_raw_data[6] == 0) &&
          (touch_raw_data[7] == 0) &&
          (touch_raw_data[8] == 0) &&
          (touch_raw_data[9] == 0) &&
          (touch_raw_data[10] == 0) &&
          (touch_raw_data[11] == 0) &&
          (touch_raw_data[12] == 0) &&
          (touch_raw_data[13] == 0) &&
          (touch_raw_data[14] == 0) &&
          (touch_raw_data[15] == 0))
        {
            // touch controller is not reponding. Try to reset I2C channel:
        	R_LCD_Close();
        	touch_interrupt_configure();
            return GX_FALSE;
        }  
            
        
        if(touch_raw_data[2] == 0)
        {
            current_touch_state = TOUCH_STATE_RELEASED;
            return GX_TRUE;
        }
        current_touch_state = TOUCH_STATE_TOUCHED;

        adc_x = touch_raw_data[3] & 0x0f;
        adc_x <<= 8;
        adc_x |= touch_raw_data[4];

        adc_x = TOUCH_X_RANGE - adc_x;

        adc_y = touch_raw_data[5] & 0x0f;
        adc_y <<= 8;
        adc_y |= touch_raw_data[6];
        adc_y = TOUCH_Y_RANGE - adc_y;

        return GX_TRUE;
    }
    return GX_FALSE;
}

/**************************************************************************/
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
VOID touch_interrupt_configure()
{
	R_LCD_Init();
	R_LCD_Open( TP_INT_PRI, 0, 0 );
	R_LCD_EventEntry( LCDEVT_ENTRY_TP, GUIX_touch_interrupt );

	/* Set the interrupt as edge trigger */
	rza_io_reg_write_16(&INTCICR1,1,INTC_ICR1_IRQ10S_SHIFT,INTC_ICR1_IRQ10S | INTC_ICR1_IRQ11S);
}

/**************************************************************************/
VOID  touch_thread_entry(ULONG thread_input)
{
    ULONG actual_flags;

    tx_event_flags_create(&touch_events, "touch_events");
    touch_interrupt_configure();
    
    while(1)
    {
        tx_event_flags_get(&touch_events, 1, TX_AND_CLEAR, &actual_flags, TX_WAIT_FOREVER);
        if (touch_get_coordinate())
        {
            if (current_touch_state == TOUCH_STATE_TOUCHED)
            {
                if (last_touch_state == TOUCH_STATE_TOUCHED)
                {
                    SendPenDragEvent();
                }
                else
                {
                    SendPenDownEvent();
                }
            }
            else
            {
                if (last_touch_state == TOUCH_STATE_TOUCHED)
                {   
                    SendPenUpEvent();
                }
            }
            tx_thread_sleep(2);
        }
        else
        {
            bad_reads++;
        }
    }   
}

void GUIX_touch_interrupt(uint32_t int_sense)
{
    /* Clear IRQ flag  */
    if (1 == rza_io_reg_read_16(&(INTCIRQRR),
    		                    INTC_IRQRR_IRQ1F_SHIFT,
    		                    (0x01 << INTC_IRQRR_IRQ1F_SHIFT)))
    {
    	rza_io_reg_write_16(&INTCIRQRR,
        		            0,
        		            INTC_IRQRR_IRQ1F_SHIFT,
        		            (0x01 << INTC_IRQRR_IRQ1F_SHIFT));
    }
    tx_event_flags_set(&touch_events, 1, TX_OR);
}
