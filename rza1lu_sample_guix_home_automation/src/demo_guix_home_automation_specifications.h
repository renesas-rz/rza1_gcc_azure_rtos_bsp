/*******************************************************************************/
/*  This file is auto-generated by Azure RTOS GUIX Studio. Do not edit this    */
/*  file by hand. Modifications to this file should only be made by running    */
/*  the Azure RTOS GUIX Studio application and re-generating the application   */
/*  specification file(s). For more information please refer to the Azure RTOS */
/*  GUIX Studio User Guide, or visit our web site at azure.com/rtos            */
/*                                                                             */
/*  GUIX Studio Revision 6.1.7.0                                               */
/*  Date (dd.mm.yyyy): 23. 6.2021   Time (hh:mm): 19:28                        */
/*******************************************************************************/


#ifndef _DEMO_GUIX_HOME_AUTOMATION_SPECIFICATIONS_H_
#define _DEMO_GUIX_HOME_AUTOMATION_SPECIFICATIONS_H_

#include "gx_api.h"

/* Determine if C++ compiler is being used, if so use standard C.              */
#ifdef __cplusplus
extern   "C" {
#endif

/* Define widget ids                                                           */

#define ID_RADIAL_SLIDER 1
#define ID_CLOSE 2
#define ID_OPEN 3
#define ID_THERMOSTAT_SCREEN 4
#define ID_PAGINATION_1 5
#define ID_PAGINATION_2 6
#define ID_PAGINATION_3 7
#define ID_PAGINATION_4 8
#define ID_LIGHTS_SCREEN 9
#define ID_STATUS 10
#define ID_BUTTON_ENABLE_DISABLE 11
#define ID_BUTTON_LOCK_UNLOCK_ALL 12
#define ID_LOCK_BUTTON 13
#define ID_LOCKS_SCREEN 14
#define ID_WEATHER_BG 15
#define ID_TITLE 16
#define ID_WEATHER_WIN 17
#define ID_WIND_WIN 18
#define ID_THU_WIN 19
#define ID_FRI_WIN 20
#define ID_SAT_WIN 21
#define ID_SUN_WIN 22
#define ID_BOTTOM_INFO_WIN 23
#define ID_V_LINE 24
#define ID_WEATHER_SCREEN 25
#define ID_NUM_0 26
#define ID_NUM_1 27
#define ID_NUM_2 28
#define ID_NUM_3 29
#define ID_NUM_4 30
#define ID_NUM_5 31
#define ID_NUM_6 32
#define ID_NUM_7 33
#define ID_NUM_8 34
#define ID_NUM_9 35
#define ID_CLEAR 36
#define ID_OK 37
#define ID_ADD 38
#define ID_FOOTER_THERMOSTAT 39
#define ID_FOOTER_LOCKS 40
#define ID_HOME 41
#define ID_SETTING 42
#define ID_FOOTER_WEATHER 43
#define ID_FOOTER_LIGHTS 44
#define ID_LIGHTS 45
#define ID_THERMOSTAT 46
#define ID_WEATHER 47
#define ID_LOCKS 48


/* Define animation ids                                                        */

#define GX_NEXT_ANIMATION_ID 1


/* Define user event ids                                                       */

enum user_defined_events{
    USER_EVENT_FADE_OUT_HOME_WINDOW = GX_FIRST_USER_EVENT,
    USER_EVENT_FADE_IN_HOME_WINDOW,
    GX_NEXT_USER_EVENT_ID
};

#define GX_ACTION_FLAG_DYNAMIC_TARGET 0x01
#define GX_ACTION_FLAG_DYNAMIC_PARENT 0x02
#define GX_ACTION_FLAG_POP_TARGET     0x04
#define GX_ACTION_FLAG_POP_PARENT     0x08

typedef struct GX_STUDIO_ACTION_STRUCT
{
    GX_UBYTE opcode;
    GX_UBYTE flags;
    GX_CONST VOID *parent;
    GX_CONST VOID *target;
    GX_CONST GX_ANIMATION_INFO  *animation;
} GX_STUDIO_ACTION;

typedef struct GX_STUDIO_EVENT_ENTRY_STRUCT
{
    ULONG event_type;
    USHORT event_sender;
    GX_CONST GX_STUDIO_ACTION *action_list;
} GX_STUDIO_EVENT_ENTRY;

typedef struct GX_STUDIO_EVENT_PROCESS_STRUCT 
{
    GX_CONST GX_STUDIO_EVENT_ENTRY *event_table;
    UINT (*chain_event_handler)(GX_WIDGET *, GX_EVENT *);
} GX_STUDIO_EVENT_PROCESS;

/* Declare properties structures for each utilized widget type                 */

typedef struct GX_STUDIO_WIDGET_STRUCT
{
   GX_CHAR *widget_name;
   USHORT  widget_type;
   USHORT  widget_id;
   #if defined(GX_WIDGET_USER_DATA)
   INT   user_data;
   #endif
   ULONG style;
   ULONG status;
   ULONG control_block_size;
   GX_RESOURCE_ID normal_fill_color_id;
   GX_RESOURCE_ID selected_fill_color_id;
   GX_RESOURCE_ID disabled_fill_color_id;
   UINT (*create_function) (GX_CONST struct GX_STUDIO_WIDGET_STRUCT *, GX_WIDGET *, GX_WIDGET *);
   void (*draw_function) (GX_WIDGET *);
   UINT (*event_function) (GX_WIDGET *, GX_EVENT *);
   GX_RECTANGLE size;
   GX_CONST struct GX_STUDIO_WIDGET_STRUCT *next_widget;
   GX_CONST struct GX_STUDIO_WIDGET_STRUCT *child_widget;
   ULONG control_block_offset;
   GX_CONST void *properties;
} GX_STUDIO_WIDGET;

typedef struct
{
    GX_CONST GX_STUDIO_WIDGET *widget_information;
    GX_WIDGET        *widget;
} GX_STUDIO_WIDGET_ENTRY;

typedef struct
{
    GX_RESOURCE_ID string_id; 
    GX_RESOURCE_ID font_id;
    GX_RESOURCE_ID normal_text_color_id;
    GX_RESOURCE_ID selected_text_color_id;
    GX_RESOURCE_ID disabled_text_color_id;
} GX_ML_TEXT_BUTTON_PROPERTIES;

typedef struct
{
    GX_RESOURCE_ID string_id; 
    GX_RESOURCE_ID font_id;
    GX_RESOURCE_ID normal_text_color_id;
    GX_RESOURCE_ID selected_text_color_id;
    GX_RESOURCE_ID disabled_text_color_id;
    GX_RESOURCE_ID off_pixelmap_id;
    GX_RESOURCE_ID on_pixelmap_id;
    GX_RESOURCE_ID off_disabled_pixelmap_id;
    GX_RESOURCE_ID on_disabled_pixelmap_id;
} GX_RADIO_BUTTON_PROPERTIES;

typedef struct
{
    GX_RESOURCE_ID normal_pixelmap_id;
    GX_RESOURCE_ID selected_pixelmap_id;
    GX_RESOURCE_ID disabled_pixelmap_id;
} GX_PIXELMAP_BUTTON_PROPERTIES;

typedef struct
{
    GX_RESOURCE_ID normal_pixelmap_id;
    GX_RESOURCE_ID selected_pixelmap_id;
} GX_ICON_PROPERTIES;

typedef struct
{
    GX_VALUE       xcenter;
    GX_VALUE       ycenter;
    USHORT         radius;
    USHORT         track_width;
    GX_VALUE       current_angle;
    GX_VALUE       min_angle;
    GX_VALUE       max_angle;
    GX_RESOURCE_ID background_pixelmap;
    GX_RESOURCE_ID needle_pixelmap;
    USHORT         animation_total_steps;
    USHORT         animation_delay;
    USHORT         animation_style;
    VOID         (*animation_update_callback)(struct GX_RADIAL_SLIDER_STRUCT *slider);
} GX_RADIAL_SLIDER_PROPERTIES;

typedef struct
{
    GX_RESOURCE_ID string_id;
    GX_RESOURCE_ID font_id;
    GX_RESOURCE_ID normal_text_color_id;
    GX_RESOURCE_ID selected_text_color_id;
    GX_RESOURCE_ID disabled_text_color_id;
} GX_PROMPT_PROPERTIES;

typedef struct
{
    GX_RESOURCE_ID string_id;
    GX_RESOURCE_ID font_id;
    GX_RESOURCE_ID normal_text_color_id;
    GX_RESOURCE_ID selected_text_color_id;
    GX_RESOURCE_ID disabled_text_color_id;
    VOID (*format_func)(GX_NUMERIC_PROMPT *, INT);
    INT            numeric_prompt_value;
} GX_NUMERIC_PROMPT_PROPERTIES;

typedef struct
{
    GX_RESOURCE_ID wallpaper_id;
} GX_WINDOW_PROPERTIES;

typedef struct
{
   GX_CONST GX_STUDIO_WIDGET *base_info;
   UINT (*base_create_function) (GX_CONST struct GX_STUDIO_WIDGET_STRUCT *, GX_WIDGET *, GX_WIDGET *);
   GX_RECTANGLE size;
} GX_TEMPLATE_PROPERTIES;


/* Declare top-level control blocks                                            */

typedef struct WEATHER_BASE_CONTROL_BLOCK_STRUCT
{
    GX_WINDOW_MEMBERS_DECLARE
    GX_WINDOW weather_base_window_bg;
    GX_PROMPT weather_base_city_name;
    GX_NUMERIC_PROMPT weather_base_temperature;
    GX_PROMPT weather_base_degree_lable;
    GX_WINDOW weather_base_weather_win;
    GX_ICON weather_base_weather_icon;
    GX_PROMPT weather_base_weather_text;
    GX_WINDOW weather_base_wind_win;
    GX_ICON weather_base_wind_icon;
    GX_PROMPT weather_base_prompt_14_1;
    GX_PROMPT weather_base_prompt_14_3;
    GX_NUMERIC_PROMPT weather_base_wind_speed;
    GX_WINDOW weather_base_thu_win;
    GX_PROMPT weather_base_thu;
    GX_ICON weather_base_thu_wicon;
    GX_NUMERIC_PROMPT weather_base_thu_hi_value;
    GX_NUMERIC_PROMPT weather_base_thu_low_value;
    GX_PROMPT weather_base_prompt_17;
    GX_PROMPT weather_base_prompt_17_1;
    GX_WINDOW weather_base_fri_win;
    GX_PROMPT weather_base_prompt_14_5;
    GX_ICON weather_base_fri_wicon;
    GX_NUMERIC_PROMPT weather_base_fri_hi_value;
    GX_NUMERIC_PROMPT weather_base_fri_low_value;
    GX_PROMPT weather_base_prompt_17_2;
    GX_PROMPT weather_base_prompt_17_3;
    GX_WINDOW weather_base_sat_win;
    GX_PROMPT weather_base_prompt_14_6;
    GX_ICON weather_base_sat_wicon;
    GX_NUMERIC_PROMPT weather_base_sat_hi_value;
    GX_NUMERIC_PROMPT weather_base_sat_low_value;
    GX_PROMPT weather_base_prompt_17_4;
    GX_PROMPT weather_base_prompt_17_5;
    GX_WINDOW weather_base_sun_win;
    GX_PROMPT weather_base_prompt_14_7;
    GX_ICON weather_base_sun_wicon;
    GX_NUMERIC_PROMPT weather_base_sun_hi_value;
    GX_NUMERIC_PROMPT weather_base_sun_low_value;
    GX_PROMPT weather_base_prompt_17_6;
    GX_PROMPT weather_base_prompt_17_7;
    GX_WINDOW weather_base_bottom_info_win;
    GX_PROMPT weather_base_hi_label;
    GX_NUMERIC_PROMPT weather_base_low_value;
    GX_PROMPT weather_base_low_label;
    GX_NUMERIC_PROMPT weather_base_hi_value;
    GX_WINDOW weather_base_v_line_left;
    GX_WINDOW weather_base_v_line_right;
} WEATHER_BASE_CONTROL_BLOCK;

typedef struct PASSCODE_SCREEN_CONTROL_BLOCK_STRUCT
{
    GX_WINDOW_MEMBERS_DECLARE
    GX_WINDOW passcode_screen_num_pad;
    GX_PIXELMAP_BUTTON passcode_screen_num_0;
    GX_PROMPT passcode_screen_prompt_11_10;
    GX_PIXELMAP_BUTTON passcode_screen_num_1;
    GX_PROMPT passcode_screen_prompt_11;
    GX_PIXELMAP_BUTTON passcode_screen_num_2;
    GX_PROMPT passcode_screen_prompt_11_1;
    GX_PIXELMAP_BUTTON passcode_screen_num_3;
    GX_PROMPT passcode_screen_prompt_11_2;
    GX_PIXELMAP_BUTTON passcode_screen_num_4;
    GX_PROMPT passcode_screen_prompt_11_3;
    GX_PIXELMAP_BUTTON passcode_screen_num_5;
    GX_PROMPT passcode_screen_prompt_11_4;
    GX_PIXELMAP_BUTTON passcode_screen_num_6;
    GX_PROMPT passcode_screen_prompt_11_5;
    GX_PIXELMAP_BUTTON passcode_screen_num_7;
    GX_PROMPT passcode_screen_prompt_11_6;
    GX_PIXELMAP_BUTTON passcode_screen_num_8;
    GX_PROMPT passcode_screen_prompt_11_7;
    GX_PIXELMAP_BUTTON passcode_screen_num_9;
    GX_PROMPT passcode_screen_prompt_11_8;
    GX_PIXELMAP_BUTTON passcode_screen_num_clear;
    GX_PROMPT passcode_screen_prompt_11_9;
    GX_PIXELMAP_BUTTON passcode_screen_ok;
    GX_PROMPT passcode_screen_prompt_11_11;
    GX_PROMPT passcode_screen_title;
    GX_PIXELMAP_BUTTON passcode_screen_close;
    GX_ICON passcode_screen_passcode_1;
    GX_ICON passcode_screen_passcode_4;
    GX_ICON passcode_screen_passcode_3;
    GX_ICON passcode_screen_passcode_2;
} PASSCODE_SCREEN_CONTROL_BLOCK;

typedef struct CONTROLLER_BASE_CONTROL_BLOCK_STRUCT
{
    GX_WINDOW_MEMBERS_DECLARE
    GX_PROMPT controller_base_title;
} CONTROLLER_BASE_CONTROL_BLOCK;

typedef struct SCREEN_BASE_CONTROL_BLOCK_STRUCT
{
    GX_WINDOW_MEMBERS_DECLARE
    GX_PIXELMAP_BUTTON screen_base_button_thermostat;
    GX_PIXELMAP_BUTTON screen_base_thermostat;
    GX_PROMPT screen_base_thermostat_text;
    GX_PIXELMAP_BUTTON screen_base_button_locks;
    GX_PIXELMAP_BUTTON screen_base_locks;
    GX_PROMPT screen_base_locks_text;
    GX_PIXELMAP_BUTTON screen_base_home;
    GX_PIXELMAP_BUTTON screen_base_setting;
    GX_WINDOW screen_base_slide_win;
    GX_PIXELMAP_BUTTON screen_base_add;
    GX_PROMPT screen_base_add_text;
    GX_ICON screen_base_icon_6;
    GX_PIXELMAP_BUTTON screen_base_button_weather;
    GX_PROMPT screen_base_weather_text;
    GX_PIXELMAP_BUTTON screen_base_weather;
    GX_PIXELMAP_BUTTON screen_base_button_light;
    GX_PIXELMAP_BUTTON screen_base_light;
    GX_PROMPT screen_base_light_text;
} SCREEN_BASE_CONTROL_BLOCK;

typedef struct MAIN_SCREEN_CONTROL_BLOCK_STRUCT
{
    GX_WINDOW_MEMBERS_DECLARE
    GX_WINDOW main_screen_home_window;
    GX_PIXELMAP_BUTTON main_screen_lights;
    GX_PROMPT main_screen_prompt;
    GX_PIXELMAP_BUTTON main_screen_pixelmap_button_4;
    GX_PIXELMAP_BUTTON main_screen_thermostat;
    GX_PROMPT main_screen_prompt_1;
    GX_PIXELMAP_BUTTON main_screen_pixelmap_button_5;
    GX_PIXELMAP_BUTTON main_screen_weather;
    GX_PROMPT main_screen_prompt_3;
    GX_PIXELMAP_BUTTON main_screen_pixelmap_button_7;
    GX_PIXELMAP_BUTTON main_screen_locks;
    GX_PROMPT main_screen_prompt_2;
    GX_PIXELMAP_BUTTON main_screen_pixelmap_button_6;
    GX_WINDOW main_screen_time_window;
    GX_PROMPT main_screen_am_pm;
    GX_PROMPT main_screen_date;
    GX_NUMERIC_PROMPT main_screen_hour;
    GX_NUMERIC_PROMPT main_screen_minute;
    GX_ICON main_screen_upper_dot;
    GX_ICON main_screen_lower_dot;
    GX_WINDOW main_screen_window;
    GX_PROMPT main_screen_weather_text;
    GX_ICON main_screen_weather_icon;
    GX_NUMERIC_PROMPT main_screen_t_value;
    GX_NUMERIC_PROMPT main_screen_low_value;
    GX_PROMPT main_screen_low_lable;
    GX_PROMPT main_screen_hi_lable;
    GX_NUMERIC_PROMPT main_screen_hi_value;
    GX_PROMPT main_screen_dot;
    GX_ICON main_screen_logo;
} MAIN_SCREEN_CONTROL_BLOCK;

typedef struct THERMOSTAT_BASE_CONTROL_BLOCK_STRUCT
{
    CONTROLLER_BASE_CONTROL_BLOCK base;
    GX_ICON thermostat_base_ruller_thermostat;
    GX_PROMPT thermostat_base_prompt_16;
    GX_PROMPT thermostat_base_prompt_16_1;
    GX_PROMPT thermostat_base_prompt_16_2;
    GX_PROMPT thermostat_base_prompt_16_3;
    GX_PROMPT thermostat_base_prompt_16_4;
    GX_PROMPT thermostat_base_prompt_16_5;
    GX_PROMPT thermostat_base_prompt_16_6;
    GX_PROMPT thermostat_base_prompt_16_7;
    GX_PROMPT thermostat_base_prompt_16_8;
    GX_RADIAL_SLIDER thermostat_base_radial_slider;
    GX_PROMPT thermostat_base_cold_heat_text;
    GX_NUMERIC_PROMPT thermostat_base_center_value;
    GX_PROMPT thermostat_base_dot_lable;
    GX_PROMPT thermostat_base_prompt_15;
    GX_ICON thermostat_base_cold_heat_icon;
    GX_PROMPT thermostat_base_bottom_value;
    GX_PIXELMAP_BUTTON thermostat_base_button_close;
    GX_PIXELMAP_BUTTON thermostat_base_button_open;
} THERMOSTAT_BASE_CONTROL_BLOCK;

typedef struct THERMOSTAT_SCREEN_CONTROL_BLOCK_STRUCT
{
    SCREEN_BASE_CONTROL_BLOCK base;
    GX_RADIO_BUTTON thermostat_screen_pagination_1;
    GX_RADIO_BUTTON thermostat_screen_pagination_2;
    GX_RADIO_BUTTON thermostat_screen_pagination_3;
    GX_RADIO_BUTTON thermostat_screen_pagination_4;
    GX_WINDOW thermostat_screen_info_win;
    GX_PROMPT thermostat_screen_on_lable;
    GX_ICON thermostat_screen_icon;
    GX_NUMERIC_PROMPT thermostat_screen_num_on;
    GX_ICON thermostat_screen_icon_1;
    GX_NUMERIC_PROMPT thermostat_screen_num_off;
    GX_PROMPT thermostat_screen_off_lable;
    GX_PROMPT thermostat_screen_kwh_lable;
    GX_PROMPT thermostat_screen_kwh_lable_2;
    GX_NUMERIC_PROMPT thermostat_screen_overall_today;
    GX_NUMERIC_PROMPT thermostat_screen_energy_in_use;
    GX_PROMPT thermostat_screen_energy_in_use_lable;
    GX_PROMPT thermostat_screen_overall_today_lable;
} THERMOSTAT_SCREEN_CONTROL_BLOCK;

typedef struct LIGHTS_BASE_CONTROL_BLOCK_STRUCT
{
    CONTROLLER_BASE_CONTROL_BLOCK base;
    GX_PIXELMAP_BUTTON lights_base_button_open;
    GX_ICON lights_base_icon_ruller;
    GX_PROMPT lights_base_prompt_16;
    GX_PROMPT lights_base_prompt_16_2;
    GX_PROMPT lights_base_prompt_16_3;
    GX_PROMPT lights_base_prompt_16_4;
    GX_PROMPT lights_base_prompt_16_5;
    GX_PROMPT lights_base_prompt_16_6;
    GX_PROMPT lights_base_prompt_16_7;
    GX_PROMPT lights_base_prompt_16_8;
    GX_PROMPT lights_base_prompt_16_9;
    GX_PROMPT lights_base_prompt_16_10;
    GX_PROMPT lights_base_prompt_16_1;
    GX_PIXELMAP_BUTTON lights_base_button_close;
    GX_RADIAL_SLIDER lights_base_radial_slider;
    GX_NUMERIC_PROMPT lights_base_power_value;
    GX_PROMPT lights_base_power_lable;
    GX_PROMPT lights_base_precent_lable;
    GX_NUMERIC_PROMPT lights_base_power_percent_value;
} LIGHTS_BASE_CONTROL_BLOCK;

typedef struct LIGHTS_SCREEN_CONTROL_BLOCK_STRUCT
{
    SCREEN_BASE_CONTROL_BLOCK base;
    GX_RADIO_BUTTON lights_screen_pagination_1;
    GX_RADIO_BUTTON lights_screen_pagination_2;
    GX_RADIO_BUTTON lights_screen_pagination_3;
    GX_RADIO_BUTTON lights_screen_pagination_4;
} LIGHTS_SCREEN_CONTROL_BLOCK;

typedef struct LOCKS_DOOR_BASE_CONTROL_BLOCK_STRUCT
{
    CONTROLLER_BASE_CONTROL_BLOCK base;
    GX_ICON locks_door_base_status_icon;
    GX_PROMPT locks_door_base_status_text;
    GX_PIXELMAP_BUTTON locks_door_base_button_lock;
    GX_ICON locks_door_base_lock_icon;
    GX_PROMPT locks_door_base_lock_text;
    GX_PIXELMAP_BUTTON locks_door_base_button_close;
    GX_PIXELMAP_BUTTON locks_door_base_button_open;
} LOCKS_DOOR_BASE_CONTROL_BLOCK;

typedef struct LOCKS_SCREEN_CONTROL_BLOCK_STRUCT
{
    SCREEN_BASE_CONTROL_BLOCK base;
    GX_RADIO_BUTTON locks_screen_pagination_1;
    GX_RADIO_BUTTON locks_screen_pagination_2;
    GX_RADIO_BUTTON locks_screen_pagination_3;
} LOCKS_SCREEN_CONTROL_BLOCK;

typedef struct WIN_SAN_DIEGO_CONTROL_BLOCK_STRUCT
{
    WEATHER_BASE_CONTROL_BLOCK base;
} WIN_SAN_DIEGO_CONTROL_BLOCK;

typedef struct WIN_NEW_YORK_CONTROL_BLOCK_STRUCT
{
    WEATHER_BASE_CONTROL_BLOCK base;
} WIN_NEW_YORK_CONTROL_BLOCK;

typedef struct WIN_LAS_VEGAS_CONTROL_BLOCK_STRUCT
{
    WEATHER_BASE_CONTROL_BLOCK base;
} WIN_LAS_VEGAS_CONTROL_BLOCK;

typedef struct WEATHER_SCREEN_CONTROL_BLOCK_STRUCT
{
    SCREEN_BASE_CONTROL_BLOCK base;
    GX_RADIO_BUTTON weather_screen_pagination_1;
    GX_RADIO_BUTTON weather_screen_pagination_2;
    GX_RADIO_BUTTON weather_screen_pagination_3;
} WEATHER_SCREEN_CONTROL_BLOCK;

typedef struct ADD_BASE_CONTROL_BLOCK_STRUCT
{
    CONTROLLER_BASE_CONTROL_BLOCK base;
    GX_PIXELMAP_BUTTON add_base_add;
} ADD_BASE_CONTROL_BLOCK;

typedef struct THERMOSTAT_PAGE_4_CONTROL_BLOCK_STRUCT
{
    GX_WINDOW_MEMBERS_DECLARE
    THERMOSTAT_BASE_CONTROL_BLOCK thermostat_page_4_office;
    ADD_BASE_CONTROL_BLOCK thermostat_page_4_add_room;
} THERMOSTAT_PAGE_4_CONTROL_BLOCK;

typedef struct THERMOSTAT_PAGE_3_CONTROL_BLOCK_STRUCT
{
    GX_WINDOW_MEMBERS_DECLARE
    THERMOSTAT_BASE_CONTROL_BLOCK thermostat_page_3_dinning_room;
    THERMOSTAT_BASE_CONTROL_BLOCK thermostat_page_3_outdoor_patio;
} THERMOSTAT_PAGE_3_CONTROL_BLOCK;

typedef struct THERMOSTAT_PAGE_2_CONTROL_BLOCK_STRUCT
{
    GX_WINDOW_MEMBERS_DECLARE
    THERMOSTAT_BASE_CONTROL_BLOCK thermostat_page_2_kids_bedroom;
    THERMOSTAT_BASE_CONTROL_BLOCK thermostat_page_2_living_room;
} THERMOSTAT_PAGE_2_CONTROL_BLOCK;

typedef struct THERMOSTAT_PAGE_1_CONTROL_BLOCK_STRUCT
{
    GX_WINDOW_MEMBERS_DECLARE
    THERMOSTAT_BASE_CONTROL_BLOCK thermostat_page_1_kitchen;
    THERMOSTAT_BASE_CONTROL_BLOCK thermostat_page_1_master_bedroom;
} THERMOSTAT_PAGE_1_CONTROL_BLOCK;

typedef struct LIGHTS_PAGE_3_CONTROL_BLOCK_STRUCT
{
    GX_WINDOW_MEMBERS_DECLARE
    LIGHTS_BASE_CONTROL_BLOCK lights_page_3_dinning_room;
    LIGHTS_BASE_CONTROL_BLOCK lights_page_3_outdoor_patio;
} LIGHTS_PAGE_3_CONTROL_BLOCK;

typedef struct LIGHTS_PAGE_2_CONTROL_BLOCK_STRUCT
{
    GX_WINDOW_MEMBERS_DECLARE
    LIGHTS_BASE_CONTROL_BLOCK lights_page_2_kids_bedroom;
    LIGHTS_BASE_CONTROL_BLOCK lights_page_2_living_room;
} LIGHTS_PAGE_2_CONTROL_BLOCK;

typedef struct LIGHTS_PAGE_4_CONTROL_BLOCK_STRUCT
{
    GX_WINDOW_MEMBERS_DECLARE
    LIGHTS_BASE_CONTROL_BLOCK lights_page_4_office;
    ADD_BASE_CONTROL_BLOCK lights_page_4_add_light;
} LIGHTS_PAGE_4_CONTROL_BLOCK;

typedef struct LIGHTS_PAGE_1_CONTROL_BLOCK_STRUCT
{
    GX_WINDOW_MEMBERS_DECLARE
    LIGHTS_BASE_CONTROL_BLOCK lights_page_1_kitchen;
    LIGHTS_BASE_CONTROL_BLOCK lights_page_1_master_bedroom;
} LIGHTS_PAGE_1_CONTROL_BLOCK;

typedef struct LOCKS_PAGE_3_CONTROL_BLOCK_STRUCT
{
    GX_WINDOW_MEMBERS_DECLARE
    LOCKS_DOOR_BASE_CONTROL_BLOCK locks_page_3_lower_deck_door;
    ADD_BASE_CONTROL_BLOCK locks_page_3_add_lock;
} LOCKS_PAGE_3_CONTROL_BLOCK;

typedef struct LOCKS_PAGE_2_CONTROL_BLOCK_STRUCT
{
    GX_WINDOW_MEMBERS_DECLARE
    LOCKS_DOOR_BASE_CONTROL_BLOCK locks_page_2_upper_deck_door;
    LOCKS_DOOR_BASE_CONTROL_BLOCK locks_page_2_patio_door;
} LOCKS_PAGE_2_CONTROL_BLOCK;

typedef struct LOCKS_PAGE_1_CONTROL_BLOCK_STRUCT
{
    GX_WINDOW_MEMBERS_DECLARE
    CONTROLLER_BASE_CONTROL_BLOCK locks_page_1_security_alarm;
    GX_ICON locks_page_1_icon_3;
    GX_PROMPT locks_page_1_prompt_15_1;
    GX_PIXELMAP_BUTTON locks_page_1_button_enable_diable;
    GX_ICON locks_page_1_icon_5;
    GX_PROMPT locks_page_1_enable_disable_text;
    GX_PIXELMAP_BUTTON locks_page_1_button_lock;
    GX_ICON locks_page_1_lock_unlock_icon;
    GX_MULTI_LINE_TEXT_BUTTON locks_page_1_lock_unlock_text;
    GX_ICON locks_page_1_icon_1;
    GX_ICON locks_page_1_icon_2;
    GX_PROMPT locks_page_1_prompt_15_5;
    GX_MULTI_LINE_TEXT_BUTTON locks_page_1_multi_line_button;
    LOCKS_DOOR_BASE_CONTROL_BLOCK locks_page_1_front_door;
} LOCKS_PAGE_1_CONTROL_BLOCK;


/* extern statically defined control blocks                                    */

#ifndef GUIX_STUDIO_GENERATED_FILE
extern THERMOSTAT_PAGE_4_CONTROL_BLOCK thermostat_page_4;
extern THERMOSTAT_PAGE_3_CONTROL_BLOCK thermostat_page_3;
extern THERMOSTAT_PAGE_2_CONTROL_BLOCK thermostat_page_2;
extern THERMOSTAT_PAGE_1_CONTROL_BLOCK thermostat_page_1;
extern THERMOSTAT_BASE_CONTROL_BLOCK thermostat_base;
extern THERMOSTAT_SCREEN_CONTROL_BLOCK thermostat_screen;
extern LIGHTS_PAGE_3_CONTROL_BLOCK lights_page_3;
extern LIGHTS_PAGE_2_CONTROL_BLOCK lights_page_2;
extern LIGHTS_PAGE_4_CONTROL_BLOCK lights_page_4;
extern LIGHTS_PAGE_1_CONTROL_BLOCK lights_page_1;
extern LIGHTS_BASE_CONTROL_BLOCK lights_base;
extern LIGHTS_SCREEN_CONTROL_BLOCK lights_screen;
extern LOCKS_PAGE_3_CONTROL_BLOCK locks_page_3;
extern LOCKS_PAGE_2_CONTROL_BLOCK locks_page_2;
extern LOCKS_PAGE_1_CONTROL_BLOCK locks_page_1;
extern LOCKS_DOOR_BASE_CONTROL_BLOCK locks_door_base;
extern LOCKS_SCREEN_CONTROL_BLOCK locks_screen;
extern WIN_SAN_DIEGO_CONTROL_BLOCK win_San_Diego;
extern WIN_NEW_YORK_CONTROL_BLOCK win_New_York;
extern WIN_LAS_VEGAS_CONTROL_BLOCK win_Las_Vegas;
extern WEATHER_BASE_CONTROL_BLOCK weather_base;
extern WEATHER_SCREEN_CONTROL_BLOCK weather_screen;
extern PASSCODE_SCREEN_CONTROL_BLOCK passcode_screen;
extern ADD_BASE_CONTROL_BLOCK add_base;
extern CONTROLLER_BASE_CONTROL_BLOCK controller_base;
extern SCREEN_BASE_CONTROL_BLOCK screen_base;
extern MAIN_SCREEN_CONTROL_BLOCK main_screen;
#endif

/* Declare event process functions, draw functions, and callback functions     */

UINT thermostat_base_event_process(GX_WINDOW *widget, GX_EVENT *event_ptr);
VOID thermostat_radial_slider_value_update(GX_RADIAL_SLIDER *slider);
VOID custom_radial_slider_draw(GX_RADIAL_SLIDER *widget);
VOID power_value_format(GX_NUMERIC_PROMPT *, INT);
UINT lights_base_event_process(GX_WINDOW *widget, GX_EVENT *event_ptr);
VOID radial_slider_value_update(GX_RADIAL_SLIDER *slider);
UINT locks_page_1_event_process(GX_WINDOW *widget, GX_EVENT *event_ptr);
UINT locks_door_base_event_process(GX_WINDOW *widget, GX_EVENT *event_ptr);
UINT passcode_screen_event_process(GX_WINDOW *widget, GX_EVENT *event_ptr);
UINT screen_base_event_process(GX_WINDOW *widget, GX_EVENT *event_ptr);
UINT slide_win_event_process(GX_WINDOW *widget, GX_EVENT *event_ptr);
UINT main_screen_event_process(GX_WINDOW *widget, GX_EVENT *event_ptr);
VOID main_screen_draw(GX_WINDOW *widget);
VOID time_format(GX_NUMERIC_PROMPT *, INT);

/* Declare the GX_STUDIO_DISPLAY_INFO structure                                */


typedef struct GX_STUDIO_DISPLAY_INFO_STRUCT 
{
    GX_CONST GX_CHAR *name;
    GX_CONST GX_CHAR *canvas_name;
    GX_CONST GX_THEME **theme_table;
    GX_CONST GX_STRING **language_table;
    USHORT   theme_table_size;
    USHORT   language_table_size;
    UINT     string_table_size;
    UINT     x_resolution;
    UINT     y_resolution;
    GX_DISPLAY *display;
    GX_CANVAS  *canvas;
    GX_WINDOW_ROOT *root_window;
    GX_COLOR   *canvas_memory;
    ULONG      canvas_memory_size;
} GX_STUDIO_DISPLAY_INFO;


/* Declare Studio-generated functions for creating top-level widgets           */

UINT gx_studio_multi_line_text_button_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
UINT gx_studio_radio_button_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
UINT gx_studio_pixelmap_button_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
UINT gx_studio_icon_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
UINT gx_studio_radial_slider_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
UINT gx_studio_prompt_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
UINT gx_studio_numeric_prompt_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
UINT gx_studio_window_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
UINT gx_studio_template_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
GX_WIDGET *gx_studio_widget_create(GX_BYTE *storage, GX_CONST GX_STUDIO_WIDGET *definition, GX_WIDGET *parent);
UINT gx_studio_named_widget_create(char *name, GX_WIDGET *parent, GX_WIDGET **new_widget);
UINT gx_studio_display_configure(USHORT display, UINT (*driver)(GX_DISPLAY *), GX_UBYTE language, USHORT theme, GX_WINDOW_ROOT **return_root);
UINT gx_studio_auto_event_handler(GX_WIDGET *widget, GX_EVENT *event_ptr, GX_CONST GX_STUDIO_EVENT_PROCESS *record);

/* Determine if a C++ compiler is being used.  If so, complete the standard
  C conditional started above.                                                 */
#ifdef __cplusplus
}
#endif

#endif                                       /* sentry                         */
