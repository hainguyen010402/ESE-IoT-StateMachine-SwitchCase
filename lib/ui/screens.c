#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

objects_t objects;

//
// Event handlers
//

lv_obj_t *tick_value_change_obj;

//
// Screens
//

void create_screen_setup_screen() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.setup_screen = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 320);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xdfe8eb), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // header_container
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.header_container = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 240, 50);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0x375463), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_width(obj, 25, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_opa(obj, 150, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // device_status_container
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.device_status_container = obj;
                    lv_obj_set_pos(obj, 165, 7);
                    lv_obj_set_size(obj, 66, 34);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // header_label_3
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.header_label_3 = obj;
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "READY");
                        }
                        {
                            // header_label_1
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.header_label_1 = obj;
                            lv_obj_set_pos(obj, 5, 2);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Status:");
                        }
                        {
                            // device_status_led
                            lv_obj_t *obj = lv_led_create(parent_obj);
                            objects.device_status_led = obj;
                            lv_obj_set_pos(obj, 51, 7);
                            lv_obj_set_size(obj, 5, 5);
                            lv_led_set_color(obj, lv_color_hex(0x00e676));
                            lv_led_set_brightness(obj, 220);
                        }
                        {
                            // device_status_container_line
                            lv_obj_t *obj = lv_line_create(parent_obj);
                            objects.device_status_container_line = obj;
                            lv_obj_set_pos(obj, 0, 20);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            static lv_point_precise_t line_points[] = {
                                { 5, 0 },
                                { 60, 0 }
                            };
                            lv_line_set_points(obj, line_points, 2);
                            lv_obj_set_style_line_color(obj, lv_color_hex(0x212121), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_line_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
                {
                    // header_label_2
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.header_label_2 = obj;
                    lv_obj_set_pos(obj, 51, 28);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "ESP32 Ver1.1");
                }
                {
                    // header_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.header_label = obj;
                    lv_obj_set_pos(obj, 50, 1);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Gateway");
                }
                {
                    // gateway_icon_7
                    lv_obj_t *obj = lv_image_create(parent_obj);
                    objects.gateway_icon_7 = obj;
                    lv_obj_set_pos(obj, 10, 9);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_image_set_src(obj, &img_gateway_icon);
                    lv_image_set_scale(obj, 310);
                }
            }
        }
        {
            // mode_configuration_instruction_label
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.mode_configuration_instruction_label = obj;
            lv_obj_set_pos(obj, 15, 70);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_line_space(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "Please select the \nconnection type:");
        }
        {
            // ethernet_mode_btn
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.ethernet_mode_btn = obj;
            lv_obj_set_pos(obj, 60, 165);
            lv_obj_set_size(obj, 120, 40);
            lv_obj_add_event_cb(obj, action_ethernet_mode_btn_pressed, LV_EVENT_PRESSED, (void *)0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x375463), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // ethernet_mode_btn_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.ethernet_mode_btn_label = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfdfefe), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Ethernet");
                }
            }
        }
        {
            // wifi_mode_btn
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.wifi_mode_btn = obj;
            lv_obj_set_pos(obj, 60, 225);
            lv_obj_set_size(obj, 120, 40);
            lv_obj_add_event_cb(obj, action_wifi_mode_btn_pressed, LV_EVENT_PRESSED, (void *)0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x375463), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // ethernet_mode_btn_label_1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.ethernet_mode_btn_label_1 = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfdfefe), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Wi-Fi");
                }
            }
        }
    }
    
    tick_screen_setup_screen();
}

void tick_screen_setup_screen() {
}

void create_screen_enter_input_screen() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.enter_input_screen = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 320);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xdfe8eb), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // header_container_8
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.header_container_8 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 240, 50);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0x375463), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_width(obj, 25, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_opa(obj, 150, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // device_status_container_8
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.device_status_container_8 = obj;
                    lv_obj_set_pos(obj, 165, 7);
                    lv_obj_set_size(obj, 66, 34);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // header_label_32
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.header_label_32 = obj;
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "READY");
                        }
                        {
                            // header_label_33
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.header_label_33 = obj;
                            lv_obj_set_pos(obj, 5, 2);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Status:");
                        }
                        {
                            // device_status_led_8
                            lv_obj_t *obj = lv_led_create(parent_obj);
                            objects.device_status_led_8 = obj;
                            lv_obj_set_pos(obj, 51, 7);
                            lv_obj_set_size(obj, 5, 5);
                            lv_led_set_color(obj, lv_color_hex(0x00e676));
                            lv_led_set_brightness(obj, 220);
                        }
                        {
                            // device_status_container_line_8
                            lv_obj_t *obj = lv_line_create(parent_obj);
                            objects.device_status_container_line_8 = obj;
                            lv_obj_set_pos(obj, 0, 20);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            static lv_point_precise_t line_points[] = {
                                { 5, 0 },
                                { 60, 0 }
                            };
                            lv_line_set_points(obj, line_points, 2);
                            lv_obj_set_style_line_color(obj, lv_color_hex(0x212121), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_line_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
                {
                    // header_label_34
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.header_label_34 = obj;
                    lv_obj_set_pos(obj, 51, 28);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "ESP32 Ver1.1");
                }
                {
                    // header_label_35
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.header_label_35 = obj;
                    lv_obj_set_pos(obj, 50, 1);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Gateway");
                }
                {
                    // gateway_icon_13
                    lv_obj_t *obj = lv_image_create(parent_obj);
                    objects.gateway_icon_13 = obj;
                    lv_obj_set_pos(obj, 10, 9);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_image_set_src(obj, &img_gateway_icon);
                    lv_image_set_scale(obj, 310);
                }
            }
        }
        {
            // kb
            lv_obj_t *obj = lv_keyboard_create(parent_obj);
            objects.kb = obj;
            lv_obj_set_pos(obj, 0, 215);
            lv_obj_set_size(obj, 240, 105);
            lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
            // Add event callback
            lv_obj_add_event_cb(
                obj,
                keyboard_event_cb,
                LV_EVENT_ALL,
                NULL
            );
        }
        {
            // ta
            lv_obj_t *obj = lv_textarea_create(parent_obj);
            objects.ta = obj;
            lv_obj_set_pos(obj, 10, 160);
            lv_obj_set_size(obj, 221, 36);
            lv_textarea_set_max_length(obj, 128);
            lv_textarea_set_one_line(obj, true);
            lv_textarea_set_password_mode(obj, true);
        }
        {
            // enter_input_screen_label
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.enter_input_screen_label = obj;
            lv_obj_set_pos(obj, 0, 135);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_align(obj, LV_ALIGN_TOP_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Enter password:");
        }
        {
            // enter_input_screen_label_1
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.enter_input_screen_label_1 = obj;
            lv_obj_set_pos(obj, 0, 90);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_align(obj, LV_ALIGN_TOP_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "\"WiFi name label\"");
        }
        {
            // enter_input_screen_label_2
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.enter_input_screen_label_2 = obj;
            lv_obj_set_pos(obj, 0, 65);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_align(obj, LV_ALIGN_TOP_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Selected WiFi:");
        }
    }
    lv_keyboard_set_textarea(objects.kb, objects.ta);
    
    tick_screen_enter_input_screen();
}

void tick_screen_enter_input_screen() {
}

void create_screen_loading_connecting_screen() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.loading_connecting_screen = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 320);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xdfe8eb), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // header_container_2
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.header_container_2 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 240, 50);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0x375463), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_width(obj, 25, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_opa(obj, 150, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // device_status_container_2
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.device_status_container_2 = obj;
                    lv_obj_set_pos(obj, 165, 7);
                    lv_obj_set_size(obj, 66, 34);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // header_label_8
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.header_label_8 = obj;
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "LOADING");
                        }
                        {
                            // header_label_9
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.header_label_9 = obj;
                            lv_obj_set_pos(obj, 5, 2);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Status:");
                        }
                        {
                            // device_status_led_2
                            lv_obj_t *obj = lv_led_create(parent_obj);
                            objects.device_status_led_2 = obj;
                            lv_obj_set_pos(obj, 51, 7);
                            lv_obj_set_size(obj, 5, 5);
                            lv_led_set_color(obj, lv_color_hex(0xffd600));
                            lv_led_set_brightness(obj, 220);
                        }
                        {
                            // device_status_container_line_2
                            lv_obj_t *obj = lv_line_create(parent_obj);
                            objects.device_status_container_line_2 = obj;
                            lv_obj_set_pos(obj, 0, 20);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            static lv_point_precise_t line_points[] = {
                                { 5, 0 },
                                { 60, 0 }
                            };
                            lv_line_set_points(obj, line_points, 2);
                            lv_obj_set_style_line_color(obj, lv_color_hex(0x212121), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_line_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
                {
                    // header_label_10
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.header_label_10 = obj;
                    lv_obj_set_pos(obj, 51, 28);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "ESP32 Ver1.1");
                }
                {
                    // header_label_11
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.header_label_11 = obj;
                    lv_obj_set_pos(obj, 50, 1);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Gateway");
                }
                {
                    // gateway_icon_5
                    lv_obj_t *obj = lv_image_create(parent_obj);
                    objects.gateway_icon_5 = obj;
                    lv_obj_set_pos(obj, 10, 9);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_image_set_src(obj, &img_gateway_icon);
                    lv_image_set_scale(obj, 310);
                }
            }
        }
        {
            // loading_screen_label
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.loading_screen_label = obj;
            lv_obj_set_pos(obj, 32, 80);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Connecting . . .");
        }
        {
            // loading_screen_spinner
            lv_obj_t *obj = lv_spinner_create(parent_obj);
            objects.loading_screen_spinner = obj;
            lv_obj_set_pos(obj, 90, 160);
            lv_obj_set_size(obj, 60, 60);
            lv_obj_set_style_arc_color(obj, lv_color_hex(0x375463), LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_opa(obj, 100, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_width(obj, 17, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_color(obj, lv_color_hex(0xd1dbde), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_width(obj, 17, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }
    
    tick_screen_loading_connecting_screen();
}

void tick_screen_loading_connecting_screen() {
}

void create_screen_dashboard_screen() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.dashboard_screen = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 320);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xdfe8eb), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // header_container_1
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.header_container_1 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 240, 50);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_width(obj, 25, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_opa(obj, 150, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // header_label_6
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.header_label_6 = obj;
                    lv_obj_set_pos(obj, 51, 28);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Dashboard");
                }
                {
                    // header_label_7
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.header_label_7 = obj;
                    lv_obj_set_pos(obj, 50, 1);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Gateway");
                }
                {
                    // device_status_container_3
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.device_status_container_3 = obj;
                    lv_obj_set_pos(obj, 160, 7);
                    lv_obj_set_size(obj, 75, 34);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // header_label_12
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.header_label_12 = obj;
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_8, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "DISCONNECTED");
                        }
                        {
                            // header_label_13
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.header_label_13 = obj;
                            lv_obj_set_pos(obj, 8, 2);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Status:");
                        }
                        {
                            // device_status_led_3
                            lv_obj_t *obj = lv_led_create(parent_obj);
                            objects.device_status_led_3 = obj;
                            lv_obj_set_pos(obj, 57, 7);
                            lv_obj_set_size(obj, 5, 5);
                            lv_led_set_color(obj, lv_color_hex(0xd00000));
                            lv_led_set_brightness(obj, 220);
                        }
                        {
                            // device_status_container_line_3
                            lv_obj_t *obj = lv_line_create(parent_obj);
                            objects.device_status_container_line_3 = obj;
                            lv_obj_set_pos(obj, 0, 20);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            static lv_point_precise_t line_points[] = {
                                { 5, 0 },
                                { 69, 0 }
                            };
                            lv_line_set_points(obj, line_points, 2);
                            lv_obj_set_style_line_color(obj, lv_color_hex(0x212121), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_line_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
                {
                    // gateway_icon_6
                    lv_obj_t *obj = lv_image_create(parent_obj);
                    objects.gateway_icon_6 = obj;
                    lv_obj_set_pos(obj, 10, 9);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_image_set_src(obj, &img_gateway_icon);
                    lv_image_set_scale(obj, 310);
                }
            }
        }
        {
            // dashboard_screen_container_1
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.dashboard_screen_container_1 = obj;
            lv_obj_set_pos(obj, 8, 60);
            lv_obj_set_size(obj, 107, 75);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xd1dbde), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_width(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_ofs_x(obj, -1, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_ofs_y(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_color(obj, lv_color_hex(0x375463), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // temp_humi_header_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.temp_humi_header_label = obj;
                    lv_obj_set_pos(obj, 18, 2);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Temp & Humi");
                }
                {
                    // dashboard_screen_container_3
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.dashboard_screen_container_3 = obj;
                    lv_obj_set_pos(obj, 14, 17);
                    lv_obj_set_size(obj, 79, 24);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xe7ecef), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_width(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, -1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0x375463), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // temperature_icon
                            lv_obj_t *obj = lv_image_create(parent_obj);
                            objects.temperature_icon = obj;
                            lv_obj_set_pos(obj, 6, 3);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_image_set_src(obj, &img_temperature_icon);
                        }
                        {
                            // temperature_value_label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.temperature_value_label = obj;
                            lv_obj_set_pos(obj, 32, 4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "27");
                        }
                        {
                            // temperature_unit_label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.temperature_unit_label = obj;
                            lv_obj_set_pos(obj, 50, 4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "°C");
                        }
                    }
                }
                {
                    // dashboard_screen_container_4
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.dashboard_screen_container_4 = obj;
                    lv_obj_set_pos(obj, 14, 45);
                    lv_obj_set_size(obj, 79, 24);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xe7ecef), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_width(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, -1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0x375463), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // humidity_value_label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.humidity_value_label = obj;
                            lv_obj_set_pos(obj, 31, 4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "70");
                        }
                        {
                            // humidity_unit_label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.humidity_unit_label = obj;
                            lv_obj_set_pos(obj, 50, 4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "%");
                        }
                        {
                            // humidity_icon
                            lv_obj_t *obj = lv_image_create(parent_obj);
                            objects.humidity_icon = obj;
                            lv_obj_set_pos(obj, 6, 3);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_image_set_src(obj, &img_humidity_icon);
                        }
                    }
                }
            }
        }
        {
            // dashboard_screen_container_2
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.dashboard_screen_container_2 = obj;
            lv_obj_set_pos(obj, 126, 60);
            lv_obj_set_size(obj, 107, 75);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xd1dbde), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_width(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_ofs_x(obj, -1, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_ofs_y(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_color(obj, lv_color_hex(0x375463), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // gpio_header_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.gpio_header_label = obj;
                    lv_obj_set_pos(obj, 25, 2);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Relay State");
                }
                {
                    // relay_switch_1
                    lv_obj_t *obj = lv_switch_create(parent_obj);
                    objects.relay_switch_1 = obj;
                    lv_obj_set_pos(obj, 50, 17);
                    lv_obj_set_size(obj, 50, 24);
                    lv_obj_add_event_cb(obj, action_relay_switch_clicked_1, LV_EVENT_CLICKED, (void *)0);
                    lv_obj_add_state(obj, LV_STATE_CHECKED);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, -1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_spread(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_opa(obj, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xe7ecef), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x375463), LV_PART_INDICATOR | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_INDICATOR | LV_STATE_CHECKED);
                }
                {
                    // relay_switch_2
                    lv_obj_t *obj = lv_switch_create(parent_obj);
                    objects.relay_switch_2 = obj;
                    lv_obj_set_pos(obj, 50, 45);
                    lv_obj_set_size(obj, 50, 24);
                    lv_obj_add_event_cb(obj, action_relay_switch_clicked_2, LV_EVENT_CLICKED, (void *)0);
                    lv_obj_add_state(obj, LV_STATE_CHECKED);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, -1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_spread(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_opa(obj, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xe7ecef), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x375463), LV_PART_INDICATOR | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_INDICATOR | LV_STATE_CHECKED);
                }
                {
                    // gpio_header_label_1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.gpio_header_label_1 = obj;
                    lv_obj_set_pos(obj, 4, 24);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Relay 1 :");
                }
                {
                    // gpio_header_label_2
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.gpio_header_label_2 = obj;
                    lv_obj_set_pos(obj, 4, 52);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Relay 2 :");
                }
            }
        }
        {
            // dashboard_screen_container_5
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.dashboard_screen_container_5 = obj;
            lv_obj_set_pos(obj, 6, 145);
            lv_obj_set_size(obj, 227, 138);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xd1dbde), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_width(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_ofs_x(obj, -1, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_ofs_y(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_color(obj, lv_color_hex(0x375463), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // y1_axis_header_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.y1_axis_header_label = obj;
                    lv_obj_set_pos(obj, 5, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xb80000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_8, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "(°C)");
                }
                {
                    // y2_axis_header_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.y2_axis_header_label = obj;
                    lv_obj_set_pos(obj, 206, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x011d8a), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_8, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "(%)");
                }
                {
                    // chart
                    lv_obj_t *obj = lv_chart_create(parent_obj);
                    objects.chart = obj;
                    lv_obj_set_pos(obj, 24, 14);
                    lv_obj_set_size(obj, 179, 103);
                    lv_obj_set_style_radius(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_opa(obj, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_line_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_line_color(obj, lv_color_hex(0x375463), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_line_opa(obj, 40, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x375463), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // chart_header_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.chart_header_label = obj;
                    lv_obj_set_pos(obj, 70, 2);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Monitoring Chart");
                }
            }
        }
        {
            // disconnect_button
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.disconnect_button = obj;
            lv_obj_set_pos(obj, 6, 287);
            lv_obj_set_size(obj, 227, 31);
            lv_obj_add_event_cb(obj, action_disconnect_button_pressed, LV_EVENT_PRESSED, (void *)0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x375463), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // disconnect_button_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.disconnect_button_label = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfdfefe), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Disconnect");
                }
            }
        }
    }
    
    tick_screen_dashboard_screen();
}

void tick_screen_dashboard_screen() {
}

void create_screen_wifi_list_screen() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.wifi_list_screen = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 320);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xdfe8eb), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // header_container_3
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.header_container_3 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 240, 50);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0x375463), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // device_status_container_1
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.device_status_container_1 = obj;
                    lv_obj_set_pos(obj, 165, 7);
                    lv_obj_set_size(obj, 66, 34);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // header_label_4
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.header_label_4 = obj;
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "READY");
                        }
                        {
                            // header_label_5
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.header_label_5 = obj;
                            lv_obj_set_pos(obj, 5, 2);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Status:");
                        }
                        {
                            // device_status_led_1
                            lv_obj_t *obj = lv_led_create(parent_obj);
                            objects.device_status_led_1 = obj;
                            lv_obj_set_pos(obj, 51, 7);
                            lv_obj_set_size(obj, 5, 5);
                            lv_led_set_color(obj, lv_color_hex(0x00e676));
                            lv_led_set_brightness(obj, 220);
                        }
                        {
                            // device_status_container_line_1
                            lv_obj_t *obj = lv_line_create(parent_obj);
                            objects.device_status_container_line_1 = obj;
                            lv_obj_set_pos(obj, 0, 20);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            static lv_point_precise_t line_points[] = {
                                { 5, 0 },
                                { 60, 0 }
                            };
                            lv_line_set_points(obj, line_points, 2);
                            lv_obj_set_style_line_color(obj, lv_color_hex(0x212121), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_line_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
                {
                    // header_label_14
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.header_label_14 = obj;
                    lv_obj_set_pos(obj, 51, 28);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "ESP32 Ver1.1");
                }
                {
                    // header_label_15
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.header_label_15 = obj;
                    lv_obj_set_pos(obj, 50, 1);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Gateway");
                }
                {
                    // gateway_icon_8
                    lv_obj_t *obj = lv_image_create(parent_obj);
                    objects.gateway_icon_8 = obj;
                    lv_obj_set_pos(obj, 10, 9);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_image_set_src(obj, &img_gateway_icon);
                    lv_image_set_scale(obj, 310);
                }
            }
        }
        {
            // wifi_list_screen_container
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.wifi_list_screen_container = obj;
            lv_obj_set_pos(obj, 17, 92);
            lv_obj_set_size(obj, 206, 186);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfdfefe), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // scanned_wifi_label_1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.scanned_wifi_label_1 = obj;
                    lv_obj_set_pos(obj, 4, 7);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_add_event_cb(obj, action_scanned_wifi_label_pressed_1, LV_EVENT_CLICKED, (void *)0);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "1. WF1 (-20dB)");
                }
                {
                    // scanned_wifi_label_3
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.scanned_wifi_label_3 = obj;
                    lv_obj_set_pos(obj, 4, 59);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_add_event_cb(obj, action_scanned_wifi_label_pressed_3, LV_EVENT_CLICKED, (void *)0);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "3. WF1 (-20dB)");
                }
                {
                    // scanned_wifi_label_4
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.scanned_wifi_label_4 = obj;
                    lv_obj_set_pos(obj, 4, 85);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_add_event_cb(obj, action_scanned_wifi_label_pressed_4, LV_EVENT_CLICKED, (void *)0);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "4. WF1 (-20dB)");
                }
                {
                    // scanned_wifi_label_5
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.scanned_wifi_label_5 = obj;
                    lv_obj_set_pos(obj, 4, 111);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_add_event_cb(obj, action_scanned_wifi_label_pressed_5, LV_EVENT_CLICKED, (void *)0);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "5. WF1 (-20dB)");
                }
                {
                    // scanned_wifi_label_6
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.scanned_wifi_label_6 = obj;
                    lv_obj_set_pos(obj, 4, 136);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_add_event_cb(obj, action_scanned_wifi_label_pressed_6, LV_EVENT_CLICKED, (void *)0);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "6. WF1 (-20dB)");
                }
                {
                    // scanned_wifi_label_7
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.scanned_wifi_label_7 = obj;
                    lv_obj_set_pos(obj, 4, 162);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_add_event_cb(obj, action_scanned_wifi_label_pressed_7, LV_EVENT_CLICKED, (void *)0);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "7. WF1 (-20dB)");
                }
                {
                    // scanned_wifi_label_8
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.scanned_wifi_label_8 = obj;
                    lv_obj_set_pos(obj, 4, 188);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_add_event_cb(obj, action_scanned_wifi_label_pressed_8, LV_EVENT_CLICKED, (void *)0);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "8. WF1 (-20dB)");
                }
                {
                    // scanned_wifi_label_10
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.scanned_wifi_label_10 = obj;
                    lv_obj_set_pos(obj, 4, 240);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_add_event_cb(obj, action_scanned_wifi_label_pressed_10, LV_EVENT_CLICKED, (void *)0);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "10. WF1 (-20dB)");
                }
                {
                    // scanned_wifi_label_9
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.scanned_wifi_label_9 = obj;
                    lv_obj_set_pos(obj, 4, 214);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_add_event_cb(obj, action_scanned_wifi_label_pressed_9, LV_EVENT_CLICKED, (void *)0);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "9. WF1 (-20dB)");
                }
                {
                    // scanned_wifi_label_2
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.scanned_wifi_label_2 = obj;
                    lv_obj_set_pos(obj, 4, 33);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_add_event_cb(obj, action_scanned_wifi_label_pressed_2, LV_EVENT_CLICKED, (void *)0);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "2. WF1 (-20dB)");
                }
            }
        }
        {
            // wifi_list_screen_subheader_label
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.wifi_list_screen_subheader_label = obj;
            lv_obj_set_pos(obj, 15, 60);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "Scanned Wi-Fi(s):");
        }
        {
            // back_button
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.back_button = obj;
            lv_obj_set_pos(obj, 15, 287);
            lv_obj_set_size(obj, 210, 31);
            lv_obj_add_event_cb(obj, action_back_button_pressed, LV_EVENT_PRESSED, (void *)0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x375463), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // back_button_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.back_button_label = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfdfefe), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Back");
                }
            }
        }
    }
    
    tick_screen_wifi_list_screen();
}

void tick_screen_wifi_list_screen() {
}

void create_screen_loading_scanning_wifi_screen() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.loading_scanning_wifi_screen = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 320);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xdfe8eb), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // header_container_6
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.header_container_6 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 240, 50);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0x375463), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_width(obj, 25, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_opa(obj, 150, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // device_status_container_6
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.device_status_container_6 = obj;
                    lv_obj_set_pos(obj, 165, 7);
                    lv_obj_set_size(obj, 66, 34);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // header_label_24
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.header_label_24 = obj;
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "LOADING");
                        }
                        {
                            // header_label_25
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.header_label_25 = obj;
                            lv_obj_set_pos(obj, 5, 2);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Status:");
                        }
                        {
                            // device_status_led_6
                            lv_obj_t *obj = lv_led_create(parent_obj);
                            objects.device_status_led_6 = obj;
                            lv_obj_set_pos(obj, 51, 7);
                            lv_obj_set_size(obj, 5, 5);
                            lv_led_set_color(obj, lv_color_hex(0xffd600));
                            lv_led_set_brightness(obj, 220);
                        }
                        {
                            // device_status_container_line_6
                            lv_obj_t *obj = lv_line_create(parent_obj);
                            objects.device_status_container_line_6 = obj;
                            lv_obj_set_pos(obj, 0, 20);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            static lv_point_precise_t line_points[] = {
                                { 5, 0 },
                                { 60, 0 }
                            };
                            lv_line_set_points(obj, line_points, 2);
                            lv_obj_set_style_line_color(obj, lv_color_hex(0x212121), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_line_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
                {
                    // header_label_26
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.header_label_26 = obj;
                    lv_obj_set_pos(obj, 51, 28);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "ESP32 Ver1.1");
                }
                {
                    // header_label_27
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.header_label_27 = obj;
                    lv_obj_set_pos(obj, 50, 1);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Gateway");
                }
                {
                    // gateway_icon_11
                    lv_obj_t *obj = lv_image_create(parent_obj);
                    objects.gateway_icon_11 = obj;
                    lv_obj_set_pos(obj, 10, 9);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_image_set_src(obj, &img_gateway_icon);
                    lv_image_set_scale(obj, 310);
                }
            }
        }
        {
            // loading_screen_label_1
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.loading_screen_label_1 = obj;
            lv_obj_set_pos(obj, 42, 80);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Searching . . .");
        }
        {
            // loading_screen_spinner_1
            lv_obj_t *obj = lv_spinner_create(parent_obj);
            objects.loading_screen_spinner_1 = obj;
            lv_obj_set_pos(obj, 90, 160);
            lv_obj_set_size(obj, 60, 60);
            lv_obj_set_style_arc_color(obj, lv_color_hex(0x375463), LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_opa(obj, 100, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_width(obj, 17, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_color(obj, lv_color_hex(0xd1dbde), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_width(obj, 17, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }
    
    tick_screen_loading_scanning_wifi_screen();
}

void tick_screen_loading_scanning_wifi_screen() {
}

void create_screen_notify_screen() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.notify_screen = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 320);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xdfe8eb), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // header_container_7
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.header_container_7 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 240, 50);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0x375463), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_width(obj, 25, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_opa(obj, 150, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // device_status_container_7
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.device_status_container_7 = obj;
                    lv_obj_set_pos(obj, 165, 7);
                    lv_obj_set_size(obj, 66, 34);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // header_label_28
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.header_label_28 = obj;
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "READY");
                        }
                        {
                            // header_label_29
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.header_label_29 = obj;
                            lv_obj_set_pos(obj, 5, 2);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Status:");
                        }
                        {
                            // device_status_led_7
                            lv_obj_t *obj = lv_led_create(parent_obj);
                            objects.device_status_led_7 = obj;
                            lv_obj_set_pos(obj, 51, 7);
                            lv_obj_set_size(obj, 5, 5);
                            lv_led_set_color(obj, lv_color_hex(0x00e676));
                            lv_led_set_brightness(obj, 220);
                        }
                        {
                            // device_status_container_line_7
                            lv_obj_t *obj = lv_line_create(parent_obj);
                            objects.device_status_container_line_7 = obj;
                            lv_obj_set_pos(obj, 0, 20);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            static lv_point_precise_t line_points[] = {
                                { 5, 0 },
                                { 60, 0 }
                            };
                            lv_line_set_points(obj, line_points, 2);
                            lv_obj_set_style_line_color(obj, lv_color_hex(0x212121), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_line_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
                {
                    // header_label_30
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.header_label_30 = obj;
                    lv_obj_set_pos(obj, 51, 28);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "ESP32 Ver1.1");
                }
                {
                    // header_label_31
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.header_label_31 = obj;
                    lv_obj_set_pos(obj, 50, 1);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Gateway");
                }
                {
                    // gateway_icon_12
                    lv_obj_t *obj = lv_image_create(parent_obj);
                    objects.gateway_icon_12 = obj;
                    lv_obj_set_pos(obj, 10, 9);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_image_set_src(obj, &img_gateway_icon);
                    lv_image_set_scale(obj, 310);
                }
            }
        }
        {
            // notify_screen_label_container
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.notify_screen_label_container = obj;
            lv_obj_set_pos(obj, 29, 100);
            lv_obj_set_size(obj, 182, 162);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // confirm_button
                    lv_obj_t *obj = lv_button_create(parent_obj);
                    objects.confirm_button = obj;
                    lv_obj_set_pos(obj, 31, 103);
                    lv_obj_set_size(obj, 120, 40);
                    lv_obj_add_event_cb(obj, action_confirm_btn_pressed, LV_EVENT_PRESSED, (void *)0);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x375463), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // confirm_button_label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.confirm_button_label = obj;
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xfdfefe), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Confirm");
                        }
                    }
                }
                {
                    // notify_screen_label_container_1
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.notify_screen_label_container_1 = obj;
                    lv_obj_set_pos(obj, 20, 12);
                    lv_obj_set_size(obj, 143, 87);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // notify_screen_label_1
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.notify_screen_label_1 = obj;
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x022235), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "[W] Try again.");
                        }
                    }
                }
            }
        }
    }
    
    tick_screen_notify_screen();
}

void tick_screen_notify_screen() {
}

typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_setup_screen,
    tick_screen_enter_input_screen,
    tick_screen_loading_connecting_screen,
    tick_screen_dashboard_screen,
    tick_screen_wifi_list_screen,
    tick_screen_loading_scanning_wifi_screen,
    tick_screen_notify_screen,
};
void tick_screen(int screen_index) {
    if (screen_index >= 0 && screen_index < 7) {
        tick_screen_funcs[screen_index]();
    }
}
void tick_screen_by_id(enum ScreensEnum screenId) {
    tick_screen(screenId - 1);
}

//
// Fonts
//

ext_font_desc_t fonts[] = {
#if LV_FONT_MONTSERRAT_8
    { "MONTSERRAT_8", &lv_font_montserrat_8 },
#endif
#if LV_FONT_MONTSERRAT_10
    { "MONTSERRAT_10", &lv_font_montserrat_10 },
#endif
#if LV_FONT_MONTSERRAT_12
    { "MONTSERRAT_12", &lv_font_montserrat_12 },
#endif
#if LV_FONT_MONTSERRAT_14
    { "MONTSERRAT_14", &lv_font_montserrat_14 },
#endif
#if LV_FONT_MONTSERRAT_16
    { "MONTSERRAT_16", &lv_font_montserrat_16 },
#endif
#if LV_FONT_MONTSERRAT_18
    { "MONTSERRAT_18", &lv_font_montserrat_18 },
#endif
#if LV_FONT_MONTSERRAT_20
    { "MONTSERRAT_20", &lv_font_montserrat_20 },
#endif
#if LV_FONT_MONTSERRAT_22
    { "MONTSERRAT_22", &lv_font_montserrat_22 },
#endif
#if LV_FONT_MONTSERRAT_24
    { "MONTSERRAT_24", &lv_font_montserrat_24 },
#endif
#if LV_FONT_MONTSERRAT_26
    { "MONTSERRAT_26", &lv_font_montserrat_26 },
#endif
#if LV_FONT_MONTSERRAT_28
    { "MONTSERRAT_28", &lv_font_montserrat_28 },
#endif
#if LV_FONT_MONTSERRAT_30
    { "MONTSERRAT_30", &lv_font_montserrat_30 },
#endif
#if LV_FONT_MONTSERRAT_32
    { "MONTSERRAT_32", &lv_font_montserrat_32 },
#endif
#if LV_FONT_MONTSERRAT_34
    { "MONTSERRAT_34", &lv_font_montserrat_34 },
#endif
#if LV_FONT_MONTSERRAT_36
    { "MONTSERRAT_36", &lv_font_montserrat_36 },
#endif
#if LV_FONT_MONTSERRAT_38
    { "MONTSERRAT_38", &lv_font_montserrat_38 },
#endif
#if LV_FONT_MONTSERRAT_40
    { "MONTSERRAT_40", &lv_font_montserrat_40 },
#endif
#if LV_FONT_MONTSERRAT_42
    { "MONTSERRAT_42", &lv_font_montserrat_42 },
#endif
#if LV_FONT_MONTSERRAT_44
    { "MONTSERRAT_44", &lv_font_montserrat_44 },
#endif
#if LV_FONT_MONTSERRAT_46
    { "MONTSERRAT_46", &lv_font_montserrat_46 },
#endif
#if LV_FONT_MONTSERRAT_48
    { "MONTSERRAT_48", &lv_font_montserrat_48 },
#endif
};

//
// Color themes
//

uint32_t active_theme_index = 0;

//
//
//

void create_screens() {

// Set default LVGL theme
    lv_display_t *dispp = lv_display_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
    lv_display_set_theme(dispp, theme);
    
    // Initialize screens
    // Create screens
    create_screen_setup_screen();
    create_screen_enter_input_screen();
    create_screen_loading_connecting_screen();
    create_screen_dashboard_screen();
    create_screen_wifi_list_screen();
    create_screen_loading_scanning_wifi_screen();
    create_screen_notify_screen();
}