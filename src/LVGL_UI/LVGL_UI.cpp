#include "LVGL_UI.h"
#include "esp_heap_caps.h"



#include "../config/config.h"
#include "../config/state.h"

#include "../wifi/wifi.h"

#include "../gpio/gpio.h"

#include "../sensor_handler/sensor_handler.h"

#include "../get_time/get_time.h"

#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();     // tự tạo, không lấy từ lv_tft_espi_get_tft nữa (do dùng dma)

TFT_eSPI *touch_tft = nullptr;

//TFT_eSPI * lv_tft_espi_get_tft(lv_display_t * disp);



uiDisplay UI_DISPLAY = UI_MODE_CONFIGURATION;

//uint32_t draw_buf[DRAW_BUF_SIZE / 4];

uint8_t *draw_buf1 = nullptr;
uint8_t *draw_buf2 = nullptr;




bool initDrawBuffer()
{
    draw_buf1 = (uint8_t *)heap_caps_malloc(DRAW_BUF_SIZE, MALLOC_CAP_INTERNAL | MALLOC_CAP_DMA);
    //draw_buf2 = (uint8_t *)heap_caps_malloc(DRAW_BUF_SIZE, MALLOC_CAP_INTERNAL | MALLOC_CAP_DMA);

    //if (!draw_buf1 || !draw_buf2) {
    if (!draw_buf1) {
        Serial.println("Draw buffer allocation failed!");
        return false;
    }
    return true;
}




// ani 3 chấm connecting... cho màn
unsigned long mqttAnimMillis = 0;
uint8_t mqttDotCount = 0;

// ĐỒ THỊ /////////////////////////////////////////////////////////////////
lv_obj_t *chart;
lv_obj_t *scale_x;
lv_chart_series_t *ser_temp;
lv_chart_series_t *ser_hum;

uint32_t ts[CHART_POINTS];          // giây trong ngày, 0 = chưa có điểm
char xbuf[5][9];                   // 5 nhãn "hh:mm"
const char *xlabels[] = { xbuf[0], xbuf[1], xbuf[2], xbuf[3], xbuf[4], NULL };  // phải kết thúc bằng NULL

lv_obj_t *make_scale(lv_obj_t *parent, lv_scale_mode_t mode,
                            int32_t x, int32_t y, int32_t w, int32_t h,
                            lv_color_t color)
{ 
    lv_obj_t *s = lv_scale_create(parent);
    lv_scale_set_mode(s, mode);
    lv_obj_set_pos(s, x, y);
    lv_obj_set_size(s, w, h);
    lv_scale_set_label_show(s, true);
    lv_obj_remove_flag(s, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_remove_flag(s, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_set_style_pad_all(s, 0, 0);
    lv_obj_set_style_text_font(s, &lv_font_montserrat_8, 0);
    lv_obj_set_style_text_color(s, color, 0);

    // đường trục + tick chính (INDICATOR) + tick phụ (ITEMS)
    lv_obj_set_style_line_color(s, color, LV_PART_MAIN);
    lv_obj_set_style_line_width(s, 1, LV_PART_MAIN);
    lv_obj_set_style_line_color(s, color, LV_PART_INDICATOR);
    lv_obj_set_style_line_width(s, 1, LV_PART_INDICATOR);
    lv_obj_set_style_length(s, 4, LV_PART_INDICATOR);
    lv_obj_set_style_line_color(s, color, LV_PART_ITEMS);
    lv_obj_set_style_line_width(s, 1, LV_PART_ITEMS);
    lv_obj_set_style_length(s, 2, LV_PART_ITEMS);
    return s;
}

#if LV_USE_LOG != 0
void my_print( lv_log_level_t level, const char * buf )
{
    LV_UNUSED(level);
    Serial.println(buf);
    Serial.flush();
}
#endif

/* LVGL calls it when a rendered image needs to copied to the display*/
void my_disp_flush( lv_display_t *disp, const lv_area_t *area, uint8_t * px_map)
{
    /*Copy `px map` to the `area`*/

    /*For example ("my_..." functions needs to be implemented by you)*/
    uint32_t w = lv_area_get_width(area);
    uint32_t h = lv_area_get_height(area);

    //my_set_window(area->x1, area->y1, w, h);
    //my_draw_bitmaps(px_map, w * h);

    tft.dmaWait();                         // chờ lượt DMA trước xong

    lv_draw_sw_rgb565_swap(px_map, w * h); // đổi byte order RGB565 (LVGL 9 không còn LV_COLOR_16_SWAP)

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushPixelsDMA((uint16_t *)px_map, w * h);
    tft.endWrite();
     

    /*Call it to tell LVGL you are ready*/
    lv_display_flush_ready(disp);
}


// đọc cảm ứng 
/*Read the touchpad*/
void my_touchpad_read( lv_indev_t * indev, lv_indev_data_t * data )
{
    /*For example  ("my_..." functions needs to be implemented by you)*/

    //Serial.printf("my_touchpad_read active!");

    uint16_t x, y;
    //bool touched = my_get_touch( &x, &y );
    bool touched = false;

    if (touch_tft != nullptr) {
        //touch_tft->dmaWait();
        touched = touch_tft->getTouch(&x, &y);
    }



    if(!touched) {
        data->state = LV_INDEV_STATE_RELEASED;
        //Serial.printf("nothing touch!");
    } else {
        data->state = LV_INDEV_STATE_PRESSED;



        data->point.x = (240-int(y*240/300));
        data->point.y = (320-int(x*320/240));

        Serial.printf("Touch X: %u, Y: %u\n", (240-int(y*240/320)), (320-int(x*320/240)));
    }
    
     
}

/*use Arduinos millis() as tick source*/
uint32_t my_tick(void)
{
    return millis();
}



void lvgl_init()
{
  String LVGL_Arduino = "LVGL Hello ESP32! ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
  Serial.println( LVGL_Arduino );


 
  if (!initDrawBuffer()) {
      Serial.println("Cannot initialize draw buffer!");
      return;
  }


  tft.init();
  tft.setRotation(TFT_ROTATION);  // rotation của TFT_eSPI, đừng nhầm với lv_display_set_rotation
  if (!tft.initDMA()) {
    Serial.println("initDMA FAILED");
  }
  // tft.setTouch(calData);           // nếu có calib cho touch
 



  lv_init();
  /*Set a tick source so that LVGL will know how much time elapsed. */
  lv_tick_set_cb(my_tick);




   /* register print function for debugging */
  #if LV_USE_LOG != 0
    lv_log_register_print_cb( my_print );
  #endif

  lv_display_t * disp = nullptr;
  
  
  #if LV_USE_TFT_ESPI
    //TFT_eSPI can be enabled lv_conf.h to initialize the display in a simple way
      disp = lv_tft_espi_create(
      TFT_HOR_RES,
      TFT_VER_RES,
      draw_buf,
      DRAW_BUF_SIZE
    );
    lv_display_set_rotation(disp, TFT_ROTATION); 

  #else
    /*Else create a display yourself*/
    disp = lv_display_create(TFT_HOR_RES, TFT_VER_RES);
    lv_display_set_flush_cb(disp, my_disp_flush);
    lv_display_set_buffers(disp, draw_buf1, NULL, DRAW_BUF_SIZE,
                       LV_DISPLAY_RENDER_MODE_PARTIAL);
  #endif
  
  
  



  touch_tft = &tft;   // để my_touchpad_read dùng tiếp

  if (touch_tft == nullptr) 
  {
    Serial.println("ERROR: TFT object is NULL");
  } 

  else 
  {
    Serial.println("OK: Using LVGL's TFT object");
  }


  /*Initialize the (dummy) input device driver*/
  lv_indev_t * indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER); /*Touchpad should have POINTER type*/
  lv_indev_set_read_cb(indev, my_touchpad_read);

  /* Create a simple label
    * ---------------------
    lv_obj_t *label = lv_label_create( lv_screen_active() );
    lv_label_set_text( label, "Hello Arduino, I'm LVGL!" );
    lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );

    * Try an example. See all the examples
    *  - Online: https://docs.lvgl.io/master/examples.html
    *  - Source codes: https://github.com/lvgl/lvgl/tree/master/examples
    * ----------------------------------------------------------------

    lv_example_btn_1();

    * Or try out a demo. Don't forget to enable the demos in lv_conf.h. E.g. LV_USE_DEMO_WIDGETS
    * -------------------------------------------------------------------------------------------

    lv_demo_widgets();
    */




  ui_init();

  chart_setup();



}

// màn wifi list hiển thị sau khi scan

void ui_wifi_list_update()
{
  lv_obj_t* wifiLabels[MAX_WIFI_SHOW] = {
    objects.scanned_wifi_label_1, objects.scanned_wifi_label_2,
    objects.scanned_wifi_label_3, objects.scanned_wifi_label_4,
    objects.scanned_wifi_label_5, objects.scanned_wifi_label_6,
    objects.scanned_wifi_label_7, objects.scanned_wifi_label_8,
    objects.scanned_wifi_label_9, objects.scanned_wifi_label_10
  };

  char buf[64];
  for (int i = 0; i < MAX_WIFI_SHOW; i++) {
    if (i < wifiCount) {
      snprintf(buf, sizeof(buf), "%d. %s (%d dBm)", (i+1), wifiSSID[i].c_str(), wifiRSSI[i]);
      lv_label_set_text(wifiLabels[i], buf);
      lv_obj_clear_flag(wifiLabels[i], LV_OBJ_FLAG_HIDDEN);
      
    } else {
      lv_label_set_text(wifiLabels[i], ""); // không đủ 10 mạng thì để trống
    }
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ui_selected_wifi_name_update()
{

  lv_label_set_text(objects.enter_input_screen_label_1, selectedSSID.c_str());
  //lv_label_set_text(objects.enter_password_screen_label_4, "");
}

void ui_enter_broker_ip_update()
{
  if (WIFI_STATUS_FLAG || EthernetConnectStatus)
  {
    // đã kết nối đc lan
    lv_label_set_text(objects.enter_input_screen_label_2, "LAN Connected!");
    lv_label_set_text(objects.enter_input_screen_label, "Enter Broker IP:");
    lv_textarea_set_password_mode(objects.ta, false);
    lv_keyboard_set_mode(objects.kb, LV_KEYBOARD_MODE_NUMBER);

    if (WIFI_STATUS_FLAG)
    {
      lv_label_set_text(objects.enter_input_screen_label_1, selectedSSID.c_str());
    }

    else if (EthernetConnectStatus)
    {
      lv_label_set_text(objects.enter_input_screen_label_1, "EtherneT");
    }

  }
}


void ui_notify_update()
{
  if (WIFI_PASSWORD_NOTIFY)
  {
    lv_label_set_text(objects.notify_screen_label_1, "[W] Try again.");
    lv_obj_align(objects.notify_screen_label_1, LV_ALIGN_CENTER, 0, 0);
  }
  else if (ETHERNET_CONNECT_FAILED_NOTIFY)
  {
    lv_label_set_text(objects.notify_screen_label_1, "[E] Try again.");
    lv_obj_align(objects.notify_screen_label_1, LV_ALIGN_CENTER, 0, 0);
  }
}



void ui_dashboard_update()
{
  if (MQTT_STATUS_FLAG && MQTT_SUBSCRIBE_SUCCESS)
  {
    lv_label_set_text(objects.header_label_12, "CONNECTED");
    lv_led_set_color(objects.device_status_led_3, lv_color_hex(0x00e676));
  }
  else
  {
    lv_label_set_text(objects.header_label_12, "DISCONNECTED");
    lv_led_set_color(objects.device_status_led_3, lv_color_hex(0xd00000));
  }
    

  lv_label_set_text(objects.temperature_value_label, text1);
  lv_label_set_text(objects.humidity_value_label, text2);

  if (!esp32_gpio_config[0].state)
  {
    lv_obj_add_state(objects.relay_switch_1, LV_STATE_CHECKED);
  }
  else lv_obj_clear_state(objects.relay_switch_1, LV_STATE_CHECKED);

  if (!esp32_gpio_config[1].state)
  {
    lv_obj_add_state(objects.relay_switch_2, LV_STATE_CHECKED);
  }
  else lv_obj_clear_state(objects.relay_switch_2, LV_STATE_CHECKED);

  static uint32_t last_chart_push = 0;
  uint32_t now = millis();

  if (now - last_chart_push >= 2000) {
      last_chart_push = now;
      chart_push(temp, hum);
  }
}

uiDisplay ui_display_processing()
{
  // màn chọn mode (ethernet, wifi)
  if (!MODE_CONFIGURATED)
  {
    return UI_MODE_CONFIGURATION;
  }
  
  // không phải màn chọn mode -> biến flag mode configurated đã từ false -> true
  else
  {
    if (ETHERNET_MODE_FLAG) // nếu chọn mode ethernet
    {
      if (ETHERNET_CONNECTING_FLAG) // flag ethernet connecting của màn loading
      {
        return UI_LOADING_CONNECTING;
      }
      else  // không phải màn loading connecting
      {
        if (EthernetConnectStatus)  // nếu ethernet đã kết nối 
        {
          if (!MQTT_IP_CONFIGURATED) // nếu chưa config broker ip
          {
            return UI_ENTER_BROKER_IP;  // màn nhập broker ip
          }

          else if (DASHBOARD_SCREEN_FLAG) // nếu đã config broker ip
          {
            return UI_DASHBOARD; // dashboard
          }
        }

        else  
        {
          if (!EthernetConnectStatus) return UI_NOTIFY; // nếu ethernet chưa kết nối
        }
      } 
    }

    else // nếu chọn mode wifi
    {
        // màn searching lans chỉ hiện wifi_label, spinner và led
      if (WIFI_SEARCHING_FLAG)
      {
        return UI_LOADING_SCAN_WIFI;
      }
      // không phải màn searching lans
      else
      {
        if (WIFI_SCAN_DONE && WIFI_SHOW_WIFI_LIST)
        {
          return UI_WIFI_LIST; // màn hiển thị wifi list quét đc
        }

        else if (WIFI_PASSWORD_ENTER_SCREEN_FLAG)
        {
          return UI_ENTER_WIFI_PASSWORD; // màn nhập password wifi
        }

        else if (WIFI_CONNECTING_FLAG)
        {
          return UI_LOADING_CONNECTING; // màn loading connecting
        }

        

        else if (WIFI_STATUS_FLAG) // đã kết nối đc với wifi 
        {
          if (!MQTT_IP_CONFIGURATED) // nếu chưa config broker ip
          {
            return UI_ENTER_BROKER_IP;  // màn nhập broker ip
          }

          else if (DASHBOARD_SCREEN_FLAG) // nếu đã config broker ip
          {
            return UI_DASHBOARD; // dashboard
          }

        }

        else  // nếu chưa kết nối được wifi
        {
            if (WIFI_PASSWORD_NOTIFY) return UI_NOTIFY;    // màn thông báo      
        }

      }
    }                                    
  }
}









void chart_refresh()
{
  lv_chart_set_all_value(chart, ser_temp, LV_CHART_POINT_NONE);
  lv_chart_set_all_value(chart, ser_hum,  LV_CHART_POINT_NONE);

  lv_chart_refresh(chart);
}







static int16_t lastDisplay = -1;



void ui_display_load_screen()
{
    if (UI_DISPLAY != lastDisplay)
    {
        lastDisplay = UI_DISPLAY;

        switch (UI_DISPLAY)
        {
            case UI_MODE_CONFIGURATION:
                chart_clear_data();
                loadScreen(SCREEN_ID_SETUP_SCREEN);
                break;

            case UI_LOADING_SCAN_WIFI:
                chart_clear_data();
                loadScreen(SCREEN_ID_LOADING_SCANNING_WIFI_SCREEN);
                break;

            case UI_LOADING_CONNECTING:
                chart_clear_data();
                loadScreen(SCREEN_ID_LOADING_CONNECTING_SCREEN);
                break;

            case UI_WIFI_LIST:
                chart_clear_data();
                loadScreen(SCREEN_ID_WIFI_LIST_SCREEN);
                break;

            case UI_ENTER_WIFI_PASSWORD:
                chart_clear_data();
                // Xóa screen cũ nếu còn tồn tại
                if (objects.enter_input_screen != NULL) {
                    lv_obj_delete(objects.enter_input_screen);
                    objects.enter_input_screen = NULL;
                }
                create_screen_enter_input_screen();
                loadScreen(SCREEN_ID_ENTER_INPUT_SCREEN);
                break;

            case UI_ENTER_BROKER_IP:
                chart_clear_data();
                // Xóa screen cũ nếu còn tồn tại
                if (objects.enter_input_screen != NULL) {
                    lv_obj_delete(objects.enter_input_screen);
                    objects.enter_input_screen = NULL;
                }
                create_screen_enter_input_screen();
                loadScreen(SCREEN_ID_ENTER_INPUT_SCREEN);
                break;

            case UI_DASHBOARD:
                chart_clear_data();
                loadScreen(SCREEN_ID_DASHBOARD_SCREEN);
                break;

            case UI_NOTIFY:
                chart_clear_data();
                loadScreen(SCREEN_ID_NOTIFY_SCREEN);
                break;

        }
    }
}



void ui_display_update()
{
  
    // Những thứ cần update liên tục để riêng ra
    switch (UI_DISPLAY)
    {
        case UI_WIFI_LIST:
            ui_wifi_list_update();
            break;

        case UI_ENTER_WIFI_PASSWORD:
            ui_selected_wifi_name_update();
            break;

        case UI_ENTER_BROKER_IP:
            ui_enter_broker_ip_update();
            break;

        case UI_NOTIFY:
            ui_notify_update();
            break;

        case UI_DASHBOARD:
            ui_dashboard_update();
            break;
    }
}




void ui_display()
{
    UI_DISPLAY = ui_display_processing();
    ui_display_update();
}
