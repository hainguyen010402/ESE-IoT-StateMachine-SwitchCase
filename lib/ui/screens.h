#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Screens

enum ScreensEnum {
    _SCREEN_ID_FIRST = 1,
    SCREEN_ID_SETUP_SCREEN = 1,
    SCREEN_ID_ENTER_INPUT_SCREEN = 2,
    SCREEN_ID_LOADING_CONNECTING_SCREEN = 3,
    SCREEN_ID_DASHBOARD_SCREEN = 4,
    SCREEN_ID_WIFI_LIST_SCREEN = 5,
    SCREEN_ID_LOADING_SCANNING_WIFI_SCREEN = 6,
    SCREEN_ID_NOTIFY_SCREEN = 7,
    _SCREEN_ID_LAST = 7
};

typedef struct _objects_t {
    lv_obj_t *setup_screen;
    lv_obj_t *enter_input_screen;
    lv_obj_t *loading_connecting_screen;
    lv_obj_t *dashboard_screen;
    lv_obj_t *wifi_list_screen;
    lv_obj_t *loading_scanning_wifi_screen;
    lv_obj_t *notify_screen;
    lv_obj_t *header_container;
    lv_obj_t *device_status_container;
    lv_obj_t *header_label_3;
    lv_obj_t *header_label_1;
    lv_obj_t *device_status_led;
    lv_obj_t *device_status_container_line;
    lv_obj_t *header_label_2;
    lv_obj_t *header_label;
    lv_obj_t *gateway_icon_7;
    lv_obj_t *mode_configuration_instruction_label;
    lv_obj_t *ethernet_mode_btn;
    lv_obj_t *ethernet_mode_btn_label;
    lv_obj_t *wifi_mode_btn;
    lv_obj_t *ethernet_mode_btn_label_1;
    lv_obj_t *header_container_8;
    lv_obj_t *device_status_container_8;
    lv_obj_t *header_label_32;
    lv_obj_t *header_label_33;
    lv_obj_t *device_status_led_8;
    lv_obj_t *device_status_container_line_8;
    lv_obj_t *header_label_34;
    lv_obj_t *header_label_35;
    lv_obj_t *gateway_icon_13;
    lv_obj_t *kb;
    lv_obj_t *ta;
    lv_obj_t *enter_input_screen_label;
    lv_obj_t *enter_input_screen_label_1;
    lv_obj_t *enter_input_screen_label_2;
    lv_obj_t *header_container_2;
    lv_obj_t *device_status_container_2;
    lv_obj_t *header_label_8;
    lv_obj_t *header_label_9;
    lv_obj_t *device_status_led_2;
    lv_obj_t *device_status_container_line_2;
    lv_obj_t *header_label_10;
    lv_obj_t *header_label_11;
    lv_obj_t *gateway_icon_5;
    lv_obj_t *loading_screen_label;
    lv_obj_t *loading_screen_spinner;
    lv_obj_t *header_container_1;
    lv_obj_t *header_label_6;
    lv_obj_t *header_label_7;
    lv_obj_t *device_status_container_3;
    lv_obj_t *header_label_12;
    lv_obj_t *header_label_13;
    lv_obj_t *device_status_led_3;
    lv_obj_t *device_status_container_line_3;
    lv_obj_t *gateway_icon_6;
    lv_obj_t *dashboard_screen_container_1;
    lv_obj_t *temp_humi_header_label;
    lv_obj_t *dashboard_screen_container_3;
    lv_obj_t *temperature_icon;
    lv_obj_t *temperature_value_label;
    lv_obj_t *temperature_unit_label;
    lv_obj_t *dashboard_screen_container_4;
    lv_obj_t *humidity_value_label;
    lv_obj_t *humidity_unit_label;
    lv_obj_t *humidity_icon;
    lv_obj_t *dashboard_screen_container_2;
    lv_obj_t *gpio_header_label;
    lv_obj_t *relay_switch_1;
    lv_obj_t *relay_switch_2;
    lv_obj_t *gpio_header_label_1;
    lv_obj_t *gpio_header_label_2;
    lv_obj_t *dashboard_screen_container_5;
    lv_obj_t *y1_axis_header_label;
    lv_obj_t *y2_axis_header_label;
    lv_obj_t *chart;
    lv_obj_t *chart_header_label;
    lv_obj_t *disconnect_button;
    lv_obj_t *disconnect_button_label;
    lv_obj_t *header_container_3;
    lv_obj_t *device_status_container_1;
    lv_obj_t *header_label_4;
    lv_obj_t *header_label_5;
    lv_obj_t *device_status_led_1;
    lv_obj_t *device_status_container_line_1;
    lv_obj_t *header_label_14;
    lv_obj_t *header_label_15;
    lv_obj_t *gateway_icon_8;
    lv_obj_t *wifi_list_screen_container;
    lv_obj_t *scanned_wifi_label_1;
    lv_obj_t *scanned_wifi_label_3;
    lv_obj_t *scanned_wifi_label_4;
    lv_obj_t *scanned_wifi_label_5;
    lv_obj_t *scanned_wifi_label_6;
    lv_obj_t *scanned_wifi_label_7;
    lv_obj_t *scanned_wifi_label_8;
    lv_obj_t *scanned_wifi_label_10;
    lv_obj_t *scanned_wifi_label_9;
    lv_obj_t *scanned_wifi_label_2;
    lv_obj_t *wifi_list_screen_subheader_label;
    lv_obj_t *back_button;
    lv_obj_t *back_button_label;
    lv_obj_t *header_container_6;
    lv_obj_t *device_status_container_6;
    lv_obj_t *header_label_24;
    lv_obj_t *header_label_25;
    lv_obj_t *device_status_led_6;
    lv_obj_t *device_status_container_line_6;
    lv_obj_t *header_label_26;
    lv_obj_t *header_label_27;
    lv_obj_t *gateway_icon_11;
    lv_obj_t *loading_screen_label_1;
    lv_obj_t *loading_screen_spinner_1;
    lv_obj_t *header_container_7;
    lv_obj_t *device_status_container_7;
    lv_obj_t *header_label_28;
    lv_obj_t *header_label_29;
    lv_obj_t *device_status_led_7;
    lv_obj_t *device_status_container_line_7;
    lv_obj_t *header_label_30;
    lv_obj_t *header_label_31;
    lv_obj_t *gateway_icon_12;
    lv_obj_t *notify_screen_label_container;
    lv_obj_t *confirm_button;
    lv_obj_t *confirm_button_label;
    lv_obj_t *notify_screen_label_container_1;
    lv_obj_t *notify_screen_label_1;
} objects_t;

extern objects_t objects;

void create_screen_setup_screen();
void tick_screen_setup_screen();

void create_screen_enter_input_screen();
void tick_screen_enter_input_screen();

void create_screen_loading_connecting_screen();
void tick_screen_loading_connecting_screen();

void create_screen_dashboard_screen();
void tick_screen_dashboard_screen();

void create_screen_wifi_list_screen();
void tick_screen_wifi_list_screen();

void create_screen_loading_scanning_wifi_screen();
void tick_screen_loading_scanning_wifi_screen();

void create_screen_notify_screen();
void tick_screen_notify_screen();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/