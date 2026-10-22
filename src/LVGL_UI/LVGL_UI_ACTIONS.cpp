#include "LVGL_UI.h"

#include "../config/config.h"
#include "../config/state.h"

#include "../wifi/wifi.h"

#include "../gpio/gpio.h"

#include "../freeRTOS_tasks/freeRTOS_tasks.h"

void action_ethernet_mode_btn_pressed(lv_event_t * e)
{
  Serial.println("[LVGL EVENT] LVGL Event Triggered! Ethernet Mode Button pressed!");
  Serial.println("[MODE CONFIGURATION] Selected Mode: Ethernet");


  MODE_CONFIGURATED = true;
  ETHERNET_CONNECTING_FLAG = true;
  ETHERNET_MODE_FLAG = true;
}



void action_wifi_mode_btn_pressed(lv_event_t * e)
{
  WIFI_SEARCHING_FLAG = true; 
  
  Serial.println("[LVGL EVENT] LVGL Event Triggered! WiFi Mode Button pressed!");
  Serial.println("[MODE CONFIGURATION] Selected Mode: WiFi");
  
  MODE_CONFIGURATED = true;

  ETHERNET_MODE_FLAG = false;
}


void action_scanned_wifi_label_pressed_1(lv_event_t *e) {
  Serial.println("[LVGL EVENT] LVGL Event Triggered! Wifi label 1 pressed!");
  select_wifi(0);
  WIFI_SHOW_WIFI_LIST = false;
  WIFI_PASSWORD_ENTER_SCREEN_FLAG = true;
}

void action_scanned_wifi_label_pressed_2(lv_event_t *e) {
  Serial.println("[LVGL EVENT] LVGL Event Triggered! Wifi label 2 pressed!");
  select_wifi(1);
  WIFI_SHOW_WIFI_LIST = false;
  WIFI_PASSWORD_ENTER_SCREEN_FLAG = true;
}

void action_scanned_wifi_label_pressed_3(lv_event_t *e) {
  Serial.println("[LVGL EVENT] LVGL Event Triggered! Wifi label 3 pressed!");
  select_wifi(2);
  WIFI_SHOW_WIFI_LIST = false;
  WIFI_PASSWORD_ENTER_SCREEN_FLAG = true;
}

void action_scanned_wifi_label_pressed_4(lv_event_t *e) {
  Serial.println("[LVGL EVENT] LVGL Event Triggered! Wifi label 4 pressed!");
  select_wifi(3);
  WIFI_SHOW_WIFI_LIST = false;
  WIFI_PASSWORD_ENTER_SCREEN_FLAG = true;
}

void action_scanned_wifi_label_pressed_5(lv_event_t *e) {
  Serial.println("[LVGL EVENT] LVGL Event Triggered! Wifi label 5 pressed!");
  select_wifi(4);
  WIFI_SHOW_WIFI_LIST = false;
  WIFI_PASSWORD_ENTER_SCREEN_FLAG = true;
}

void action_scanned_wifi_label_pressed_6(lv_event_t *e) {
  Serial.println("[LVGL EVENT] LVGL Event Triggered! Wifi label 6 pressed!");
  select_wifi(5);
  WIFI_SHOW_WIFI_LIST = false;
  WIFI_PASSWORD_ENTER_SCREEN_FLAG = true;
}

void action_scanned_wifi_label_pressed_7(lv_event_t *e) {
  Serial.println("[LVGL EVENT] LVGL Event Triggered! Wifi label 7 pressed!");
  select_wifi(6);
  WIFI_SHOW_WIFI_LIST = false;
  WIFI_PASSWORD_ENTER_SCREEN_FLAG = true;
}

void action_scanned_wifi_label_pressed_8(lv_event_t *e) {
  Serial.println("[LVGL EVENT] LVGL Event Triggered! Wifi label 8 pressed!");
  select_wifi(7);
  WIFI_SHOW_WIFI_LIST = false;
  WIFI_PASSWORD_ENTER_SCREEN_FLAG = true;
}

void action_scanned_wifi_label_pressed_9(lv_event_t *e) {
  Serial.println("[LVGL EVENT] LVGL Event Triggered! Wifi label 9 pressed!");
  select_wifi(8);
  WIFI_SHOW_WIFI_LIST = false;
  WIFI_PASSWORD_ENTER_SCREEN_FLAG = true;
}

void action_scanned_wifi_label_pressed_10(lv_event_t *e) {
  Serial.println("[LVGL EVENT] LVGL Event Triggered! Wifi label 10 pressed!");
  select_wifi(9);
  WIFI_SHOW_WIFI_LIST = false;
  WIFI_PASSWORD_ENTER_SCREEN_FLAG = true;
}

void action_relay_switch_clicked_1(lv_event_t * e)
{
  Serial.println("[LVGL EVENT] LVGL Event Triggered! Switch GPIO 1 pressed!");
  digitalWrite(GPIO_PIN1, !digitalRead(GPIO_PIN1));
}


void action_relay_switch_clicked_2(lv_event_t * e)
{
  Serial.println("[LVGL EVENT] LVGL Event Triggered! Switch GPIO 2 pressed!");
  digitalWrite(GPIO_PIN2, !digitalRead(GPIO_PIN2));
}

void action_confirm_btn_pressed(lv_event_t * e)
{
  switch (PROCESSING_STATE)
  {
    case (WIFI):
      Serial.println("[LVGL EVENT] LVGL Event Triggered! Confirm button pressed!");
      WIFI_PASSWORD_NOTIFY = false;
      WIFI_SHOW_WIFI_LIST = true;
      WIFI_STATUS_FLAG = false;
      DASHBOARD_SCREEN_FLAG = false;
      PROCESSING_STATE = WIFI;
      Serial.println("WIFI CONNECT FAILED!");
      Serial.println("Re-enter 'w1' to 'w10' to retry.");
      break;

    case (ETHERNET):
      ETHERNET_CONNECT_FAILED_NOTIFY = false;
      MODE_CONFIGURATED = false;
      PROCESSING_STATE = MODE_CONFIGURATION;
      break;
  }
  
}

void keyboard_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_READY)
    {

      if (WIFI_PASSWORD_ENTER_SCREEN_FLAG)
      {
        WIFI_PASSWORD_ENTERED = true;
        Serial.println("[LVGL EVENT - WF PW] LVGL Event Triggered! Enter button pressed!");
      }

      else if (MQTT_IP_ENTER_SCREEN_FLAG)
      {
        MQTT_IP_ENTERED = true;
        Serial.println("[LVGL EVENT - MQTT IP] LVGL Event Triggered! Enter button pressed!");
      }     
    }

    if (code == LV_EVENT_VALUE_CHANGED) 
    {
      uint32_t id = lv_buttonmatrix_get_selected_button(
          lv_event_get_target_obj(e)
      );

      const char *txt = lv_buttonmatrix_get_button_text(
          lv_event_get_target_obj(e), id
      );

      if (txt && strcmp(txt, LV_SYMBOL_LEFT) == 0) 
      {

          Serial.println("\"<\" pressed!");
          
          // Xử lý khi nhấn phím <

          //back màn enter pass
          if (WIFI_PASSWORD_ENTER_SCREEN_FLAG)
          {
            WIFI_PASSWORD_ENTER_SCREEN_FLAG = false;
            WIFI_SHOW_WIFI_LIST = true;
            WIFI_SCAN_DONE = true;
          }

          //back màn enter broker ip
          else if (!MQTT_IP_CONFIGURATED)
          {
              MODE_CONFIGURATED = false;

              WIFI_SCAN_DONE = false;

              WIFI_SHOW_WIFI_LIST = false;

              PROCESSING_STATE = MODE_CONFIGURATION;
          }
        
      }
      
    }
}

void action_disconnect_button_pressed(lv_event_t * e)
{
  Serial.println("[LVGL EVENT] LVGL Event Triggered! Disconnect button pressed!");

  DISCONNECT_ALL = true;
}


void action_back_button_pressed(lv_event_t * e)
{
  Serial.println("[LVGL EVENT] LVGL Event Triggered! Back button pressed!");

  MODE_CONFIGURATED = false;

  WIFI_SCAN_DONE = false;

  WIFI_SHOW_WIFI_LIST = false;

  PROCESSING_STATE = MODE_CONFIGURATION;
}
