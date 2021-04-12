
#include    <stdio.h>
#include    <string.h>
#include    <fcntl.h>
#include	"r_typedefs.h"
#include    "iodefine_cfg.h"
#include 	"intc_iodefine.h"
#include 	"r_intc.h"
#include    "r_devlink_wrapper.h"
#include    "r_riic_drv_sc_cfg.h"
/* Low level register read/write header */
#include "rza_io_regrw.h"

#include    "Renesas_RZ_A1.h"

#include "tx_api.h"
#include "gx_api.h"

/* FocalTech CTP Controller  */
#define RZA1H_APP_I2C_FT5216    (0x70)
/* TI PanelBus� DIGITAL TRANSMITTER */
#define RZA1H_APP_I2C_TFP410    (0x78)

#define TOUCH_STATE_TOUCHED  1
#define TOUCH_STATE_RELEASED 2
#define MIN_DRAG_DELTA       10

#define TOUCH_X_RANGE  800
#define TOUCH_Y_RANGE  480

static int32_t touch_get_coordinate ();
void GUIX_touch_interrupt(uint32_t int_sense);
static void Int_SetPort( void );

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

int8_t R_RIIC_rza1h_rsk_read(uint32_t channel,
		                      uint8_t d_adr,
		                      uint16_t r_adr,
		                      uint32_t r_byte,
		                      uint8_t * r_buffer) {

	int32_t nRet;

	st_r_drv_riic_config_t i2c_read;

	/* cast to uint8_t */
	i2c_read.device_address = (uint8_t)d_adr;
	i2c_read.sub_address = r_adr;
	i2c_read.number_of_bytes = r_byte;
	i2c_read.p_data_buffer = r_buffer;

	nRet = control(hi2c0, CTL_RIIC_READ, &i2c_read);

	return nRet;
}

int32_t R_RIIC_rza1h_rsk_write(uint32_t channel, uint8_t d_adr,
		                       uint16_t w_adr, uint32_t w_byte, uint8_t * w_buffer) {
	int_t nRet;

	st_r_drv_riic_config_t i2c_write;

	/* cast to uint8_t */
	i2c_write.device_address = (uint8_t)d_adr;
	i2c_write.sub_address = w_adr;
	i2c_write.number_of_bytes = w_byte;

	/* cast to uint8_t pointer */
	i2c_write.p_data_buffer = (uint8_t *) w_buffer;

	nRet = control(hi2c0, CTL_RIIC_WRITE, &i2c_write);

	return (uint8_t)nRet;
}
void R_RIIC_rza1h_rsk_init(void) {

	int_t nRet;
	int_t error = -1;
	st_r_drv_riic_create_t riic_clock;
	uint8_t  data[0x2];

	/* set the clock frequency for the I2C channel */
	riic_clock.frequency = RIIC_FREQUENCY_100KHZ;

	hi2c0 = open(DEVICE_INDENTIFIER "iic0", O_RDWR);

	if (( -1) == hi2c0)
	{
		nRet = -1;
	}
	else
	{
		/* Create the I2C channel */
		error = control(hi2c0, CTL_RIIC_CREATE, &riic_clock);

		if (0 > error)
		{
			nRet = -1;
		}

		tx_thread_sleep(5);

		R_RIIC_rza1h_rsk_read(DEVDRV_CH_0, RZA1H_APP_I2C_TFP410, 0x08, 0x2, data);

		data[0] = 0xbf;

		R_RIIC_rza1h_rsk_write(DEVDRV_CH_0, RZA1H_APP_I2C_TFP410, 0x08, 0x2, data);

		data[0] = 0x00;

		R_RIIC_rza1h_rsk_read(DEVDRV_CH_0, RZA1H_APP_I2C_TFP410, 0x08, 0x2, data);
	}
}

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

    ret = R_RIIC_rza1h_rsk_read(DEVDRV_CH_0, RZA1H_APP_I2C_FT5216, 0, 32, touch_raw_data);
    if (DEVDRV_SUCCESS == ret)
    {
        // test for invalid data
        if (touch_raw_data[0] == 0xff ||
            touch_raw_data[2] > 5)
        {
            // touch controller is not reponding. Try to reset I2C channel:
            R_RIIC_rza1h_rsk_init();
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
            R_RIIC_rza1h_rsk_init();
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
	R_INTC_Disable(INTC_ID_IRQ1);
	R_INTC_SetPriority(INTC_ID_IRQ1, 10uL);

	Int_SetPort();

    /* Assign the IRQ1 function */
    R_INTC_RegistIntFunc(INTC_ID_IRQ1, GUIX_touch_interrupt);

    /* Set the interrupt as edge trigger */
    rza_io_reg_write_16(&INTCICR1,1,INTC_ICR1_IRQ10S_SHIFT,INTC_ICR1_IRQ10S | INTC_ICR1_IRQ11S);

    /* Enable the touch panel interrupt */
    R_INTC_Enable(INTC_ID_IRQ1);
}

/**************************************************************************/
VOID  touch_thread_entry(ULONG thread_input)
{
    ULONG actual_flags;
    R_RIIC_rza1h_rsk_init();
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

void Int_SetPort( void )
{
    int32_t was_masked;

#if defined (__ICCARM__)
    was_masked = __disable_irq_iar();
#else
    was_masked = __disable_irq();
#endif

	/* Initialise Touch Panel Interrupt. Set TP_INT (P4_9) as IRQ1 */
    /* P4_9=IRQ1(Alternative Mode 8,Input:PFCAE1,PFCE1,PFC1) */

    /* Port initialize */
    GPIO.PIBC4  &= (uint16_t)~(GPIO_BIT_N9);   /* Input buffer disable */
    GPIO.PBDC4  &= (uint16_t)~(GPIO_BIT_N9);   /* Bidirectional mode disable */
    GPIO.PM4    |= (uint16_t) (GPIO_BIT_N9);   /* Input mode */
    GPIO.PMC4   &= (uint16_t)~(GPIO_BIT_N9);   /* Port mode */
    GPIO.PIPC4  &= (uint16_t)~(GPIO_BIT_N9);   /* Selected by PM bit */

    /* Port input function setting */
    GPIO.PBDC4  &= (uint16_t)~(GPIO_BIT_N9);   /* Bidirectional mode disable */
    GPIO.PFC4   |= (uint16_t) (GPIO_BIT_N9);   /* 8th convertible */
    GPIO.PFCE4  |= (uint16_t) (GPIO_BIT_N9);   /* 8th convertible */
    GPIO.PFCAE4 |= (uint16_t) (GPIO_BIT_N9);   /* 8th convertible */

    /* Port setting */
    GPIO.PIPC4  |= (uint16_t) (GPIO_BIT_N9);   /* Selected by peripheral */
    GPIO.PMC4   |= (uint16_t) (GPIO_BIT_N9);   /* Convertible mode */

    if (0 == was_masked)
    {
        __enable_irq();
    }
}
#
