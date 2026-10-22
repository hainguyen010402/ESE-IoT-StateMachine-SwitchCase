#ifndef EEZ_LVGL_UI_EVENTS_H
#define EEZ_LVGL_UI_EVENTS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void action_ethernet_mode_btn_pressed(lv_event_t * e);
extern void action_wifi_mode_btn_pressed(lv_event_t * e);
extern void action_scanned_wifi_label_pressed_1(lv_event_t * e);
extern void action_scanned_wifi_label_pressed_2(lv_event_t * e);
extern void action_scanned_wifi_label_pressed_3(lv_event_t * e);
extern void action_scanned_wifi_label_pressed_4(lv_event_t * e);
extern void action_scanned_wifi_label_pressed_5(lv_event_t * e);
extern void action_scanned_wifi_label_pressed_6(lv_event_t * e);
extern void action_scanned_wifi_label_pressed_7(lv_event_t * e);
extern void action_scanned_wifi_label_pressed_8(lv_event_t * e);
extern void action_scanned_wifi_label_pressed_9(lv_event_t * e);
extern void action_scanned_wifi_label_pressed_10(lv_event_t * e);
extern void action_relay_switch_clicked_1(lv_event_t * e);
extern void action_relay_switch_clicked_2(lv_event_t * e);
extern void action_confirm_btn_pressed(lv_event_t * e);
extern void action_disconnect_button_pressed(lv_event_t * e);
extern void action_back_button_pressed(lv_event_t * e);
extern void keyboard_event_cb(lv_event_t *e);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_EVENTS_H*/