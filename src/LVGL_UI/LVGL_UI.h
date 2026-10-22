#pragma once
#include <Arduino.h>

//  MÀN /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <lvgl.h>

#include <TFT_eSPI.h>

#include "ui.h"
#include "images.h"
#include "actions.h"

#include "../get_time/get_time.h"


//biến size TFT //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Set to your screen resolution and rotation*/
#define TFT_HOR_RES   240
#define TFT_VER_RES   320
#define TFT_ROTATION  LV_DISPLAY_ROTATION_0

/*LVGL draw into this buffer, 1/10 screen size usually works well. The size is in bytes*/
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES /40 * (LV_COLOR_DEPTH / 8))
//extern uint32_t draw_buf[DRAW_BUF_SIZE / 4];
extern TFT_eSPI tft;

extern uint8_t *draw_buf1;

extern uint8_t *draw_buf2;

//=============================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================


// ani 3 chấm connecting... cho màn
extern unsigned long mqttAnimMillis;

extern uint8_t mqttDotCount;



enum uiDisplay
{
    UI_MODE_CONFIGURATION = 0,
    UI_LOADING_SCAN_WIFI,
    UI_LOADING_CONNECTING,
    UI_WIFI_LIST,
    UI_ENTER_WIFI_PASSWORD,
    UI_ENTER_BROKER_IP,
    UI_DASHBOARD,
    UI_ENTER_INPUT,
    UI_NOTIFY
};
extern uiDisplay UI_DISPLAY;




// ĐỒ THỊ /////////////////////////////////////////////////////////////////
#define CHART_POINTS 30

// vị trí/kích thước chart (khớp với code EEZ gen ra)
#define CH_X 24
#define CH_Y 14
#define CH_W 179
#define CH_H 103

extern  lv_obj_t *chart;
extern lv_obj_t *scale_x;
extern lv_chart_series_t *ser_temp;
extern lv_chart_series_t *ser_hum;

extern uint32_t ts[CHART_POINTS];          // giây trong ngày, 0 = chưa có điểm
extern char xbuf[5][9];                    // 5 nhãn "hh:mm:ss"
extern const char *xlabels[];  // phải kết thúc bằng NULL

extern lv_obj_t *make_scale(lv_obj_t *parent, lv_scale_mode_t mode,
                            int32_t x, int32_t y, int32_t w, int32_t h,
                            lv_color_t color);



void my_print( lv_log_level_t level, const char * buf );
void my_disp_flush( lv_display_t *disp, const lv_area_t *area, uint8_t * px_map);
void my_touchpad_read( lv_indev_t * indev, lv_indev_data_t * data );
extern uint32_t my_tick(void);

void lvgl_init();
void ui_wifi_list_update();
void ui_selected_wifi_name_update();
void ui_dashboard_update();


uiDisplay ui_display_processing();
void ui_display();

void chart_setup(void);
void chart_push(float temp, float hum);
void update_x_labels(void);
void chart_clear_data(void);
void ui_display_load_screen();



