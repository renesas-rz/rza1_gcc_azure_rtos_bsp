
#include    <stdio.h>
#include    <string.h>

#include    "r_typedefs.h"
#include	"cache-l2x0.h"

#include 	"lcd_panel.h"
#include 	"r_vdc_portsetting.h"
#include 	"r_rvapi_header.h"
#include 	"r_display_init.h"

#include	"r_os_abstraction_api.h"

#include    "gx_api.h"
#include    "gx_display.h"

#define     GRAPHICS_CHANNEL            (VDC_CHANNEL_0)

#define FRAME_BUFFER_BITS_PER_PIXEL_16

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
#define FRAME_BUFFER_SIZE (DISPLAY_XRES * DISPLAY_YRES * 2)

#define DISPLAY_BACKGROUND_COLOR        0x8418

#if defined FRAME_BUFFER_BITS_PER_PIXEL_16
static USHORT frame_buffer1[DISPLAY_XRES * DISPLAY_YRES / 2] __attribute__ ((section(".VRAM_SECTION0")));
#elif defined FRAME_BUFFER_BITS_PER_PIXEL_32
static ULONG frame_buffer1[DISPLAY_XRES * DISPLAY_YRES / 2] __attribute__ ((section(".VRAM_SECTION0")));
#endif

void  BSP_DCache_FlushRange       (void *, unsigned long);

/*****************************************************************************/
void CopyCanvasToBackBuffer565rgb(GX_CANVAS *canvas, GX_RECTANGLE *copy)
{
    GX_RECTANGLE display_size;
    GX_RECTANGLE copy_clip;
    void   *flushaddress;
    USHORT *pPutRow;
    USHORT *pGetRow;
    USHORT *pGet;
    USHORT *pPut;
    int row;
    int col;
    int copy_width;
    int copy_height;

    gx_utility_rectangle_define(&display_size, 0, 0, DISPLAY_XRES - 1, DISPLAY_YRES - 1);
    copy_clip = *copy;
    copy_clip.gx_rectangle_left &= 0xfffe;
    copy_clip.gx_rectangle_right += 1;

    gx_utility_rectangle_shift(&copy_clip, canvas->gx_canvas_display_offset_x, canvas->gx_canvas_display_offset_y);
    gx_utility_rectangle_overlap_detect(&copy_clip, &display_size, &copy_clip);
    copy_width = (copy_clip.gx_rectangle_right - copy_clip.gx_rectangle_left + 1) / 2;
    copy_height = copy_clip.gx_rectangle_bottom - copy_clip.gx_rectangle_top + 1;

    if (copy_width <= 0 ||
        copy_height <= 0)
    {
        return;
    }
    
    pGetRow = (USHORT *) canvas -> gx_canvas_memory;
    pGetRow += copy->gx_rectangle_top * (canvas->gx_canvas_x_resolution / 2);
    pGetRow += copy->gx_rectangle_left / 2;

    pPutRow = frame_buffer1;    // copy into buffer 1

    pPutRow += (canvas ->gx_canvas_display_offset_y + copy->gx_rectangle_top) * (DISPLAY_XRES / 2);
    pPutRow += (canvas ->gx_canvas_display_offset_x + copy->gx_rectangle_left) / 2;;
    flushaddress = pPutRow;

    for (row = 0; row < copy_height; row++)
    {
        pGet = pGetRow;
        pPut = pPutRow;
        
        for (col = 0; col < copy_width; col++)
        {
            *pPut++ = *pGet++;
        }
        pGetRow += canvas->gx_canvas_x_resolution / 2;
        pPutRow += DISPLAY_XRES / 2;
    }
    l2x0_flush_range((uint32_t)flushaddress, (uint32_t)(flushaddress + (copy_height * DISPLAY_XRES * 2)));

}

/*****************************************************************************/
void rz_565rgb_buffer_toggle(GX_CANVAS *canvas, GX_RECTANGLE *dirty)
{
    GX_RECTANGLE Limit;
    GX_RECTANGLE Copy;

    gx_utility_rectangle_define(&Limit, 0, 0,
        canvas->gx_canvas_x_resolution -1,
        canvas->gx_canvas_y_resolution -1);
    
    if (gx_utility_rectangle_overlap_detect(&Limit, &canvas->gx_canvas_dirty_area, &Copy))
    {
        /* copy our canvas to the back buffer */
        CopyCanvasToBackBuffer565rgb(canvas, &Copy);
    }
}

/*****************************************************************************/
void ConfigureGUIXDisplayHardware565rgb(GX_DISPLAY *display)
{
	vdc_error_t error;
	vdc_channel_t vdc_ch = VDC_CHANNEL_0;
#if defined FRAME_BUFFER_BITS_PER_PIXEL_16
	USHORT                *put;
#elif defined FRAME_BUFFER_BITS_PER_PIXEL_32
	ULONG                *put;
#endif

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
		memset((void*)frame_buffer1, 0x00, FRAMEBUFFER_STRIDE * FRAMEBUFFER_HEIGHT);

#if (1) /* not use camera captured layer */
		gr_disp_cnf.layer_id         = VDC_LAYER_ID_1_RD;
#else   /* blend over camera captured image */
		gr_disp_cnf.layer_id         = VDC_LAYER_ID_2_RD;
#endif
		gr_disp_cnf.disp_area.hs_rel = 0;
		gr_disp_cnf.disp_area.hw_rel = FRAMEBUFFER_WIDTH;
		gr_disp_cnf.disp_area.vs_rel = 0;
		gr_disp_cnf.disp_area.vw_rel = FRAMEBUFFER_HEIGHT;
		gr_disp_cnf.fb_buff          = &frame_buffer1[0];
		gr_disp_cnf.fb_stride        = FRAMEBUFFER_STRIDE;
		gr_disp_cnf.read_format      = VDC_GR_FORMAT_CLUT8;
#if defined FRAME_BUFFER_BITS_PER_PIXEL_16
		gr_disp_cnf.read_format      = VDC_GR_FORMAT_RGB565;
#elif defined FRAME_BUFFER_BITS_PER_PIXEL_32
		gr_disp_cnf.read_format      = VDC_GR_FORMAT_RGB888;
#endif
		gr_disp_cnf.clut_table       = clut_table;
		gr_disp_cnf.read_ycc_swap    = VDC_GR_YCCSWAP_CBY0CRY1;
		gr_disp_cnf.read_swap        = VDC_WR_RD_WRSWA_32_16BIT;
#if (1) /* not use camera captured data */
		gr_disp_cnf.disp_mode        = VDC_DISPSEL_CURRENT;
#else   /* blend over camera captured image */
		gr_disp_cnf.disp_mode        = VDC_DISPSEL_BLEND;
#endif
		error = R_RVAPI_GraphCreateSurfaceVDC(vdc_ch, &gr_disp_cnf);
	}

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


    put = (USHORT *) frame_buffer1;
    for (int loop = 0; loop < DISPLAY_XRES * DISPLAY_YRES; loop++)
    {
        *put++ = DISPLAY_BACKGROUND_COLOR;
    }
}

/*****************************************************************************/
UINT rz_graphics_driver_setup_565rgb(GX_DISPLAY *display)
{
    _gx_display_driver_565rgb_setup(display, GX_NULL, rz_565rgb_buffer_toggle);
    display -> gx_display_handle =                       RZ_SCREEN_HANDLE;
    
    ConfigureGUIXDisplayHardware565rgb(display);
    return GX_SUCCESS;
}
    
