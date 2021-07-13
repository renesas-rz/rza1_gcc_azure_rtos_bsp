#include "mcu_board_select.h"
#if (TARGET_GUI_BPP == TARGET_GUI_32BPP)

#include    <stdio.h>
#include    <string.h>

#define DO_VIDEO	0

#include    "r_typedefs.h"
#include	"cache-l2x0.h"
#include    "dev_drv.h"

#include 	"lcd_panel.h"
#include    "r_rvapi_vdc.h"
#include 	"r_vdc_portsetting.h"
#include    "r_display_init.h"

#include 	"r_image_config.h"
#include 	"r_camera_if.h"
#include 	"r_camera_module.h"

#include	"r_os_abstraction_api.h"

#include    "gx_api.h"
#include    "gx_display.h"

extern void VDC5_CameraPortSetting(uint32_t param);

#define TODO 0

/******************************************************************************
 * Graphics Layer Configurations
 ******************************************************************************/
#define FRAME_BUFFER_BITS_PER_PIXEL_32

#if defined FRAME_BUFFER_BITS_PER_PIXEL_16
#define DATA_SIZE_PER_PIC      (2u)
#elif defined FRAME_BUFFER_BITS_PER_PIXEL_32
#define DATA_SIZE_PER_PIC      (4u)
#else
#error "Set bits per pixel"
#endif

#define FRAMEBUFFER_WIDTH	LCD_CH0_DISP_HW
#define FRAMEBUFFER_HEIGHT	LCD_CH0_DISP_VW
#define FRAMEBUFFER_STRIDE  (((FRAMEBUFFER_WIDTH * DATA_SIZE_PER_PIC) + 31u) & ~31u)

#define DISPLAY_XRES FRAMEBUFFER_WIDTH
#define DISPLAY_YRES FRAMEBUFFER_HEIGHT

#define RZ_SCREEN_HANDLE                0x525A0000

/******************************************************************************
 * Video Layer Configurations
 ******************************************************************************/
/* External input format select */


/*! Frame buffer stride: Frame buffer stride should be set to a multiple of 32 or 128
 in accordance with the frame buffer burst transfer mode. */
#define     VIDEO_BUFFER_STRIDE         (((CAP_VDC_SIZE_HW * 2u) + 31u) & ~31u)
#define     VIDEO_BUFFER_HEIGHT         (CAP_VDC_SIZE_VW)
#define     VIDEO_BUFFER_NUM            (2u)

/*! Video format: YCbCr422, 2 bytes per pixel */
#define     VIDEO_FORMAT                (VDC_RES_MD_YCBCR422)
#define VIDEO_LAYER_XRES 800
#define VIDEO_LAYER_YRES 480


static uint8_t FrameBuffer_Video[VIDEO_BUFFER_STRIDE * VIDEO_BUFFER_HEIGHT * 2u] __attribute__ ((section(".VRAM_SECTION0")));
static GX_COLOR FrameBuffer_Graphics[FRAMEBUFFER_STRIDE * FRAMEBUFFER_HEIGHT / 4] __attribute__ ((section(".VRAM_SECTION0")));

static vdc_gr_disp_sel_t gr_disp_sel[VDC_GR_TYPE_NUM] =
{
    VDC_DISPSEL_IGNORED,       /* Graphics 0 */
    VDC_DISPSEL_IGNORED,       /* Graphics 1 */
    VDC_DISPSEL_IGNORED,       /* Graphics 2 */
    VDC_DISPSEL_IGNORED,       /* Graphics 3 */
    VDC_DISPSEL_IGNORED,       /* VIN */
    VDC_DISPSEL_IGNORED        /* OIR */
};

/*! Interrupt parameter */
int vsync_count = 0;
vdc_period_rect_t      period_rect;

///////////////////////////////////////////////////////////////////////////////
static void rz_32argb_InitParameters(vdc_channel_t ch)
{
	uint32_t index;

	vsync_count = 0;

	for (index = 0; index < (uint32_t) VDC_GR_TYPE_NUM; index++)
    {
		gr_disp_sel[index] = VDC_DISPSEL_IGNORED;
	}
#if TODO
	if (ch == VDC_CHANNEL_0)
    {
		period_rect.vs_rel = (uint16_t) LCD_CH0_DISP_VS;
		period_rect.vw_rel = (uint16_t) LCD_CH0_DISP_VW;
		period_rect.hs_rel = (uint16_t) LCD_CH0_DISP_HS;
		period_rect.hw_rel = (uint16_t) LCD_CH0_DISP_HW;
	}
#endif
}


///////////////////////////////////////////////////////////////////////////////
static void rz_32argb_SetVideoDecoder(vdc_channel_t ch, vdc_period_rect_t * cap_rect,
		vdec_adc_vinsel_t adc_vinsel)
{
	vdc_error_t error;

#if ((INPUT_SELECT == ANALOG_NTSC)||(INPUT_SELECT == ANALOG_PAL))
    {
        vdec_channel_t vdec_ch;
        vdec_adc_vinsel_t vdec_adc_vinsel;
        ana_video_col_sys_t video_col_sys;
        digital_in_t * digital;

        /* VDC5 analog video input init */
        digital = NULL;
        error = R_RVAPI_VideoControlVDC (ch, digital);

        /* Wait for Color system determination */
        R_OS_TaskSleep(500);

        /* VDEC init */
        if (VDC_CHANNEL_0 == ch)
        {
            vdec_ch = VDEC_CHANNEL_0;
        }
        else
        {
            vdec_ch = VDEC_CHANNEL_1;
        }
        vdec_adc_vinsel = VDEC_ADC_VINSEL_VIN1;
        video_col_sys = GRPH_COL_SYS_UNKNOWN;

        video_col_sys = R_RVAPI_AnalogVideDecodeDVDEC (vdec_ch, vdec_adc_vinsel);
        if (GRPH_COL_SYS_UNKNOWN == video_col_sys)
        {
            printf ("[ERROR] Color system unknown.\n");
        }
    }
#elif (INPUT_SELECT ==  CMOS_CAM)
    {
        digital_in_t digital;

        /* vdc5 digital video input init */
        digital.inp_format = VDC5_EXTIN_FORMAT_BT601;
        digital.inp_pxd_edge = VDC5_EDGE_RISING;
        digital.inp_endian_on = VDC5_OFF;
        digital.inp_swap_on = VDC5_OFF;
        digital.inp_vs_inv = VDC5_SIG_POL_NOT_INVERTED;
        digital.inp_hs_inv = VDC5_SIG_POL_NOT_INVERTED;
        digital.inp_h_edge_sel = VDC5_EXTIN_REF_H_EAV;
        digital.inp_f525_625 = VDC5_EXTIN_LINE_525;
        digital.inp_h_pos = VDC5_EXTIN_H_POS_CBYCRY;
        error = R_RVAPI_VideoControlVDC (vdc_ch, &digital);
    }

    /* vdc5 digital video input port setting */
    if (VDC5_OK == error)
    {
        R_RVAPI_VideoPortSettingVDC5 (ch, VDC5_CameraPortSetting);
    }
#endif
}


///////////////////////////////////////////////////////////////////////////////
void CopyCanvasToBackBuffer32argb(GX_CANVAS *canvas, GX_RECTANGLE *copy)
{
    GX_RECTANGLE display_size;
    GX_RECTANGLE copy_clip;
    void   *flushaddress;
    ULONG *pPutRow;
    ULONG *pGetRow;
    ULONG *pGet;
    ULONG *pPut;
    int row;
    int col;
    int copy_width;
    int copy_height;

    gx_utility_rectangle_define(&display_size, 0, 0, FRAMEBUFFER_WIDTH - 1, FRAMEBUFFER_HEIGHT - 1);
    copy_clip = *copy;

    gx_utility_rectangle_shift(&copy_clip, canvas->gx_canvas_display_offset_x, canvas->gx_canvas_display_offset_y);
    gx_utility_rectangle_overlap_detect(&copy_clip, &display_size, &copy_clip);
    copy_width = (copy_clip.gx_rectangle_right - copy_clip.gx_rectangle_left + 1);
    copy_height = copy_clip.gx_rectangle_bottom - copy_clip.gx_rectangle_top + 1;

    if (copy_width <= 0 ||
        copy_height <= 0)
    {
        return;
    }
    
    pGetRow = (ULONG *) canvas -> gx_canvas_memory;
    pGetRow += copy->gx_rectangle_top * canvas->gx_canvas_x_resolution;
    pGetRow += copy->gx_rectangle_left;

    pPutRow = FrameBuffer_Graphics;
    pPutRow += (canvas ->gx_canvas_display_offset_y + copy->gx_rectangle_top) * (FRAMEBUFFER_STRIDE / 4);
    pPutRow += (canvas ->gx_canvas_display_offset_x + copy->gx_rectangle_left);
    flushaddress = pPutRow;

    for (row = 0; row < copy_height; row++)
    {
        pGet = pGetRow;
        pPut = pPutRow;
        
        for (col = 0; col < copy_width; col++)
        {
            *pPut++ = *pGet++;
        }
        pGetRow += canvas->gx_canvas_x_resolution;
        pPutRow += (FRAMEBUFFER_STRIDE / 4);
    }
    l2x0_flush_range((uint32_t)flushaddress, (uint32_t)(flushaddress + (copy_height * FRAMEBUFFER_STRIDE)));
}

///////////////////////////////////////////////////////////////////////////////
void rz_32argb_buffer_toggle(GX_CANVAS *canvas, GX_RECTANGLE *dirty)
{
    GX_RECTANGLE Limit;
    GX_RECTANGLE Copy;
        
    gx_utility_rectangle_define(&Limit, 0, 0,
        canvas->gx_canvas_x_resolution -1,
        canvas->gx_canvas_y_resolution -1);
    
    if (gx_utility_rectangle_overlap_detect(&Limit, &canvas->gx_canvas_dirty_area, &Copy))
    {
        /* copy our canvas to the back buffer */
        CopyCanvasToBackBuffer32argb(canvas, &Copy);
    }
}

///////////////////////////////////////////////////////////////////////////////
void fill_graphics_buffer_32argb(unsigned long color)
{
    unsigned long *put_start = (unsigned long *) FrameBuffer_Graphics;
    unsigned long *put;

    int row;
    int col;

    for (row = 0; row < FRAMEBUFFER_HEIGHT; row++)
    {
        put = put_start;

        for (col = 0; col < FRAMEBUFFER_WIDTH; col++)
        {
            *put++ = color;
        }
        put_start += FRAMEBUFFER_STRIDE / 4;
    }
}


///////////////////////////////////////////////////////////////////////////////
void ConfigureGUIXDisplayHardware32argb(GX_DISPLAY *display)
{
	vdc_error_t error;
	vdc_channel_t vdc_ch = VDC_CHANNEL_0;
	v_surface_config_t v_cnf;
	v_surface_disp_config_t v_disp_cnf;

	GX_COLOR                *put;

	/***********************************************************************/
	/* display init (VDC output setting) */
	/***********************************************************************/
	{
		error = r_display_init (vdc_ch);
	}

	/***********************************************************************/
	/* Graphic Layer 2 CLUT8 */
	/***********************************************************************/
	if (error == VDC_OK)
	{
		gr_surface_disp_config_t gr_disp_cnf;
		uint32_t  clut_table[4] = {
				0x00000000, /* No.0 transparent color  */
				0xFF000000, /* No.1 black */
				0xFF00FF00, /* No.2 green */
				0xFFFF0000  /* No.3 red */
		};

		/* buffer clear */
		// Set frame buffer to black
		memset((void*)FrameBuffer_Graphics, 0x00, FRAMEBUFFER_STRIDE * FRAMEBUFFER_HEIGHT);

	#if (1) /* not use camera captured layer */
	        gr_disp_cnf.layer_id         = VDC_LAYER_ID_1_RD;
	#else   /* blend over camera captured image */
	        gr_disp_cnf.layer_id         = VDC_LAYER_ID_2_RD;
	#endif
	        gr_disp_cnf.disp_area.hs_rel = 0;
	        gr_disp_cnf.disp_area.hw_rel = FRAMEBUFFER_WIDTH;
	        gr_disp_cnf.disp_area.vs_rel = 0;
	        gr_disp_cnf.disp_area.vw_rel = FRAMEBUFFER_HEIGHT;
	        gr_disp_cnf.fb_buff          = &FrameBuffer_Graphics[0];
	        gr_disp_cnf.fb_stride        = FRAMEBUFFER_STRIDE;
	        gr_disp_cnf.read_format      = VDC_GR_FORMAT_CLUT8;
	#if defined FRAME_BUFFER_BITS_PER_PIXEL_16
	        gr_disp_cnf.read_format      = VDC_GR_FORMAT_RGB565;
	#elif defined FRAME_BUFFER_BITS_PER_PIXEL_32
	        gr_disp_cnf.read_format      = VDC_GR_FORMAT_RGB888;
	#endif
	        gr_disp_cnf.clut_table       = clut_table;
	        gr_disp_cnf.read_ycc_swap    = VDC_GR_YCCSWAP_CBY0CRY1;
	        gr_disp_cnf.read_swap        = VDC_WR_RD_WRSWA_32BIT;
	#if (1) /* not use camera captured data */
	        gr_disp_cnf.disp_mode        = VDC_DISPSEL_CURRENT;
	#else   /* blend over camera captured image */
	        gr_disp_cnf.disp_mode        = VDC_DISPSEL_BLEND;
	#endif
	        error = R_RVAPI_GraphCreateSurfaceVDC(vdc_ch, &gr_disp_cnf);
	    }
#if DO_VIDEO

		/* Capture area */
		v_cnf.cap_area.vs = CAP_VDC_VS;
		v_cnf.cap_area.vw = CAP_VDC_VW;
		v_cnf.cap_area.hs = CAP_VDC_HS;
		v_cnf.cap_area.hw = CAP_VDC_HW;

		/* Write */
		v_cnf.layer_id = VDC_LAYER_ID_0_WR; /* Layer ID */
		v_cnf.fb_buff = FrameBuffer_Video; /* Video frame buffer address  */
		v_cnf.fb_stride = VIDEO_BUFFER_STRIDE; /* Write buffer stride[byte]   */
		v_cnf.fb_offset = VIDEO_BUFFER_STRIDE * VIDEO_BUFFER_HEIGHT; /* Write buffer offset[byte] */
		v_cnf.fb_num = VIDEO_BUFFER_NUM; /* Frame buffer num            */
		v_cnf.write_format = CAP_VDC_WFORMAT; /* Write format               */
		v_cnf.write_swap = VDC_WR_RD_WRSWA_NON; /* Write swap 8bit/16bit/32bit */
		v_cnf.write_fb_vw = CAP_VDC_SIZE_VW; /* 1/1 Capture (480u/2u) -> scale down -> 240u out */
		v_cnf.write_fb_hw = CAP_VDC_SIZE_HW; /* 1/2 Capture (720u*2u) -> scale down -> 720u out */
		v_cnf.res_inter = CAP_VDC_FIELD; /* Field operating mode select */
		v_cnf.write_rot = IMGC_ROTATION_MODE; /* Field operating mode select */

		/* display area */
		if (CAP_VDC_SIZE_HW < LCD_CH0_DISP_HW)
		{
			v_disp_cnf.disp_area.vs_rel = 0u;
			v_disp_cnf.disp_area.vw_rel = CAP_VDC_SIZE_VW * 2;
			v_disp_cnf.disp_area.hs_rel = 0u;
			v_disp_cnf.disp_area.hw_rel = CAP_VDC_SIZE_HW;
		}
		else
		{
			v_disp_cnf.disp_area.vs_rel = 0u;
			v_disp_cnf.disp_area.vw_rel = LCD_CH0_DISP_VW;
			v_disp_cnf.disp_area.hs_rel = 0u;
			v_disp_cnf.disp_area.hw_rel = LCD_CH0_DISP_HW;
		}

		/* read */
		v_disp_cnf.read_ycc_swap = VDC_GR_YCCSWAP_CBY0CRY1; /* Read Swap for YCbCr422 */
		v_disp_cnf.read_swap = VDC_WR_RD_WRSWA_NON; /* Read Swap 8bit/16bit/32bit */
		error = R_RVAPI_VideoCreateSurfaceVDC (vdc_ch, &v_cnf, &v_disp_cnf);
#endif
	    /* Image Quality Adjustment */
	    if (VDC_OK == error)
	    {
	        error = r_image_quality_adjustment(vdc_ch);
	    }

	    /* Enable signal output */
	    if (VDC_OK == error)
	    {
	        /* Wait for register update */
	        R_OS_TaskSleep(5);

	        R_RVAPI_DispPortSettingVDC(vdc_ch, &VDC_LcdPortSetting);
	    }

	    fill_graphics_buffer_32argb(0);
}

///////////////////////////////////////////////////////////////////////////////
UINT rz_graphics_driver_setup_32argb(GX_DISPLAY *display)
{
    _gx_display_driver_32argb_setup(display, GX_NULL, rz_32argb_buffer_toggle);
    display -> gx_display_handle =                       RZ_SCREEN_HANDLE;
    
    ConfigureGUIXDisplayHardware32argb(display);
    return GX_SUCCESS;
}
    
#endif
