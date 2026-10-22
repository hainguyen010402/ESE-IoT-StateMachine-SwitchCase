#include "LVGL_UI.h"
#include <string.h>

void secToTime(uint32_t sec_of_day, char *buf, size_t buf_size)
{
    uint32_t hour = (sec_of_day / 3600) % 24;
    uint32_t min  = (sec_of_day / 60) % 60;
    uint32_t sec  = sec_of_day % 60;

    lv_snprintf(
        buf,
        buf_size,
        "%02u:%02u:%02u",
        (unsigned)hour,
        (unsigned)min,
        (unsigned)sec
    );
}


void chart_setup(void)
{
    chart = objects.chart;
    lv_obj_t *parent = lv_obj_get_parent(chart);

    lv_obj_set_style_pad_all(chart, 0, 0);     // để tick khớp mép chart
    lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
    lv_chart_set_point_count(chart, CHART_POINTS);                // tối đa CHART_POINTS điểm
    lv_chart_set_update_mode(chart, LV_CHART_UPDATE_MODE_SHIFT);  // đầy thì đẩy điểm cũ ra
    lv_chart_set_axis_range(chart, LV_CHART_AXIS_PRIMARY_Y,   0, 50);   // °C
    lv_chart_set_axis_range(chart, LV_CHART_AXIS_SECONDARY_Y, 0, 100);  // %
    lv_chart_set_div_line_count(chart, 5, CHART_POINTS - 1);
    lv_obj_set_style_size(chart, 4, 4, LV_PART_INDICATOR);        // bỏ chấm tròn mỗi điểm

    lv_color_t c_t = lv_palette_main(LV_PALETTE_RED);
    lv_color_t c_h = lv_palette_main(LV_PALETTE_BLUE);

    ser_temp = lv_chart_add_series(chart, c_t, LV_CHART_AXIS_PRIMARY_Y);
    ser_hum  = lv_chart_add_series(chart, c_h, LV_CHART_AXIS_SECONDARY_Y);
    lv_chart_set_all_values(chart, ser_temp, LV_CHART_POINT_NONE);
    lv_chart_set_all_values(chart, ser_hum,  LV_CHART_POINT_NONE);

    // Trục Y trái: nhiệt độ 0..50, 11 tick, cứ 2 tick có 1 nhãn -> 0,10,...,50
    lv_obj_t *sy1 = make_scale(parent, LV_SCALE_MODE_VERTICAL_LEFT,
                               CH_X - 22, CH_Y, 22, CH_H, c_t);
    lv_scale_set_range(sy1, 0, 50);
    lv_scale_set_total_tick_count(sy1, 11);
    lv_scale_set_major_tick_every(sy1, 2);

    // Trục Y phải: độ ẩm 0..100 -> 0,20,...,100
    lv_obj_t *sy2 = make_scale(parent, LV_SCALE_MODE_VERTICAL_RIGHT,
                               CH_X + CH_W, CH_Y, 26, CH_H, c_h);
    lv_scale_set_range(sy2, 0, 100);
    lv_scale_set_total_tick_count(sy2, 11);
    lv_scale_set_major_tick_every(sy2, 2);

    // Trục X: 10 tick = 10 điểm, nhãn ở tick 0,3,6,9 (major_every = 3)
    scale_x = make_scale(parent, LV_SCALE_MODE_HORIZONTAL_BOTTOM,
                         CH_X, CH_Y + CH_H, CH_W, 14, lv_color_hex(0x375463));
    lv_scale_set_range(scale_x, 0, CHART_POINTS - 1);
    lv_scale_set_total_tick_count(scale_x, CHART_POINTS);
    lv_scale_set_major_tick_every(scale_x, 7);
    lv_scale_set_text_src(scale_x, xlabels);
    for (int i = 0; i < 5; i++) xbuf[i][0] = '\0';
}




// gọi mỗi lần có mẫu mới
void chart_push(float temp, float hum)
{
    static uint32_t start_time = millis();

    uint32_t sec_of_day = (millis() - start_time) / 1000;

    for (int i = 0; i < CHART_POINTS - 1; i++) {
        ts[i] = ts[i + 1];
    }

    if (sec_of_day != 0)
      ts[CHART_POINTS - 1] = sec_of_day;
    else
      ts[CHART_POINTS - 1] = 1;

    lv_chart_set_next_value(chart, ser_temp, (int32_t)temp);
    lv_chart_set_next_value(chart, ser_hum,  (int32_t)hum);

    update_x_labels();
}








void update_x_labels(void)
{
    get_time_HHMMSS(); // cập nhật biến timeinfo
    int HH = timeinfo.tm_hour;
    int MM = timeinfo.tm_min;
    int SS = timeinfo.tm_sec;
    int sec_of_day = HH * 3600 + MM * 60 + SS;
    
    const int idx[5] = {0, 7, 14, 21, 28};           // điểm tương ứng với tick chính
    for (int i = 0; i < 5; i++) {
        uint32_t s = ts[idx[i]];
        if (s == 0) xbuf[i][0] = '\0';
        else 
        {

          secToTime(sec_of_day - (28 - idx[i]), xbuf[i], sizeof(xbuf[i]));
          
        }
    }
    lv_obj_invalidate(scale_x);
}

void chart_clear_data(void)
{
    if (chart == NULL || ser_temp == NULL || ser_hum == NULL)
        return;

    // Xoá dữ liệu hai series
    lv_chart_set_all_value(chart, ser_temp, LV_CHART_POINT_NONE);
    lv_chart_set_all_value(chart, ser_hum,  LV_CHART_POINT_NONE);

    // Xoá dữ liệu thời gian
    memset(ts, 0, sizeof(ts));

    // Xoá nhãn trục X
    for (int i = 0; i < 5; i++)
    {
        xbuf[i][0] = '\0';
    }

    lv_chart_refresh(chart);
    lv_obj_invalidate(scale_x);
}








