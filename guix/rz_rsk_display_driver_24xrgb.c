
#include    <stdio.h>
#include    <string.h>

#include    "r_typedefs.h"

#include    "r_vdc5.h"
#include    "r_vdc_user.h"

#include    "dev_drv.h"

#include 	"lcd_panel.h"
#include 	"r_vdc_portsetting.h"
#include 	"compiler_settings.h"
#include 	"r_rvapi_header.h"
#include 	"r_display_init.h"
#include    "gx_api.h"
#include    "gx_display.h"

#define DISPLAY_XRES 800
#define DISPLAY_YRES 480
#define RZ_SCREEN_HANDLE                0x525A0000
#define GRAPHICS_CHANNEL                (VDC_CHANNEL_0)
void  BSP_DCache_FlushRange       (void *, unsigned long);

#define DISPLAY_BACKGROUND_COLOR        0x00505090
#pragma data_alignment=32
static GX_COLOR frame_buffer1[DISPLAY_XRES * DISPLAY_YRES];

void CopyCanvasToBackBuffer24xrgb(GX_CANVAS *canvas, GX_RECTANGLE *copy)
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

    gx_utility_rectangle_define(&display_size, 0, 0, DISPLAY_XRES - 1, DISPLAY_YRES - 1);
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

    pPutRow = frame_buffer1;
    pPutRow += (canvas ->gx_canvas_display_offset_y + copy->gx_rectangle_top) * DISPLAY_XRES;
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
        pPutRow += DISPLAY_XRES;
    }
    BSP_DCache_FlushRange(flushaddress, copy_height * DISPLAY_XRES * 4);

}

void rz_24xrgb_buffer_toggle(GX_CANVAS *canvas, GX_RECTANGLE *dirty)
{
    GX_RECTANGLE Limit;
    GX_RECTANGLE Copy;
        
    gx_utility_rectangle_define(&Limit, 0, 0,
        canvas->gx_canvas_x_resolution -1,
        canvas->gx_canvas_y_resolution -1);
    
    if (gx_utility_rectangle_overlap_detect(&Limit, &canvas->gx_canvas_dirty_area, &Copy))
    {
        /* copy our canvas to the back buffer */
        CopyCanvasToBackBuffer24xrgb(canvas, &Copy);
    }
}

void ConfigureGUIXDisplayHardware24xrgb(GX_DISPLAY *display)
{
    vdc_channel_t          ch;
    vdc_gr_format_t        gr_format;
    uint32_t                fb_stride;
    vdc_pd_disp_rect_t     period_rect;
    vdc_layer_id_t         layer_id;
    vdc_graphics_type_t    graphics_type;
    void                   *framebuff;      
    int                     loop;

    GX_COLOR                *put;
    
    vdc_gr_disp_sel_t      gr_disp_sel[VDC_GR_TYPE_NUM] =
    {
        VDC_DISPSEL_IGNORED,       /* Graphics 0 */
        VDC_DISPSEL_IGNORED,       /* Graphics 1 */
        VDC_DISPSEL_IGNORED,       /* Graphics 2 */
        VDC_DISPSEL_IGNORED,       /* Graphics 3 */
        VDC_DISPSEL_IGNORED,       /* VIN */
        VDC_DISPSEL_IGNORED        /* OIR */
    };

    vdc_error_t            error;
 
    ch = GRAPHICS_CHANNEL;

    gr_format       = VDC_GR_FORMAT_RGB888;
    fb_stride       = (uint32_t)(DISPLAY_XRES * 4);     /* bytes per pixel */  
    layer_id        = VDC_LAYER_ID_1_RD;
    graphics_type   = VDC_GR_TYPE_GR1;
    
    /* Frame buffer stride should be set to a multiple of 32 or 128
       in accordance with the frame buffer burst transfer mode. */
    fb_stride       += 31u;
    fb_stride       &= ~31u;
    
    framebuff = frame_buffer1;
    
    /* VDC5 Initialization */
        error = r_display_init(ch);

    if (error != VDC_OK)
    {
        printf("VDC5 GrpDrv_Init Error.\n");
        return;
    }
    if (GRAPHICS_CHANNEL == VDC_CHANNEL_0)
    {
        period_rect.vs_rel  = (uint16_t)LCD_CH0_DISP_VS;
        period_rect.vw_rel  = (uint16_t)LCD_CH0_DISP_VW;
        period_rect.hs_rel  = (uint16_t)LCD_CH0_DISP_HS;
        period_rect.hw_rel  = (uint16_t)LCD_CH0_DISP_HW;
    }
    else
    {
        period_rect.vs_rel  = (uint16_t)LCD_CH1_DISP_VS;
        period_rect.vw_rel  = (uint16_t)LCD_CH1_DISP_VW;
        period_rect.hs_rel  = (uint16_t)LCD_CH1_DISP_HS;
        period_rect.hw_rel  = (uint16_t)LCD_CH1_DISP_HW;
    }
    

    gr_surface_disp_config_t disp_config = {
			layer_id,
			period_rect,
			&framebuff,
			fb_stride,
			gr_format,
			NULL,
			VDC_WR_RD_WRSWA_NON,
			VDC_DISPSEL_CURRENT
	};
	/* Create graphics surface */
	error = R_RVAPI_GraphCreateSurfaceVDC(ch, &disp_config );
   
    put = (GX_COLOR *) framebuff;

    for (loop = 0; loop < DISPLAY_XRES * DISPLAY_YRES; loop++)
    {
        *put++ = DISPLAY_BACKGROUND_COLOR;
    }
}

UINT rz_graphics_driver_setup_24xrgb(GX_DISPLAY *display)
{
    _gx_display_driver_24xrgb_setup(display, GX_NULL, rz_24xrgb_buffer_toggle);
    display -> gx_display_handle =                       RZ_SCREEN_HANDLE;
    
    ConfigureGUIXDisplayHardware24xrgb(display);
    return GX_SUCCESS;
}
    
