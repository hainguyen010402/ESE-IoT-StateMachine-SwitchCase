// Ethernet ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <SPI.h>
#include <Ethernet.h>

#define SCK 18
#define MISO 19
#define MOSI 23
// Chân CS nối với W5500
#define W5500_CS 5
// Chân RESET (nếu có nối, không dùng thì để -1)
#define RST 4

// ------------ W5500 
// DHCP_STATUS 0 --> DÙNG IP TĨNH TỰ TẠO 
// DHCP_STATUS 1 --> DÙNG IP GATEWAY CẤP
bool DHCP_STATUS = 1;
// Địa chỉ MAC - tự đặt, miễn là không trùng thiết bị khác trong mạng LAN
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// Cấu hình IP tĩnh (dùng khi không có DHCP)
IPAddress ip(192, 168, 1, 177);
IPAddress dns(192, 168, 1, 1);
// IP gateway (router)
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

EthernetServer server(8080);  // Mở server ở cổng 8080


bool EthernetMode = 0;// 0->wifi, 1->ethernet
bool EthernetConnectStatus = 0; // 0->ko kết nối, 1->đã kết nối
//=========================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================














//  MÀN /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <lvgl.h>
#if LV_USE_TFT_ESPI
#include <TFT_eSPI.h>
#endif
#include "ui.h"
#include "images.h"
#include "actions.h"

// ani 3 chấm connecting... cho màn
unsigned long mqttAnimMillis = 0;
uint8_t mqttDotCount = 0;
//=============================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================

//biến TFT //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Set to your screen resolution and rotation*/
#define TFT_HOR_RES   240
#define TFT_VER_RES   240
#define TFT_ROTATION  LV_DISPLAY_ROTATION_0

/*LVGL draw into this buffer, 1/10 screen size usually works well. The size is in bytes*/
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE / 4];
//=============================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================


// ĐỒ THỊ


#define CHART_POINTS 10

// vị trí/kích thước chart (khớp với code EEZ của bạn)
#define CH_X 24
#define CH_Y 14
#define CH_W 179
#define CH_H 56

static lv_obj_t *chart;
static lv_obj_t *scale_x;
static lv_chart_series_t *ser_temp;
static lv_chart_series_t *ser_hum;

static uint32_t ts[CHART_POINTS];          // giây trong ngày, 0 = chưa có điểm
static char xbuf[4][6];                    // 4 nhãn "hh:mm"
static const char *xlabels[] = { xbuf[0], xbuf[1], xbuf[2], xbuf[3], NULL };  // phải kết thúc bằng NULL

static lv_obj_t *make_scale(lv_obj_t *parent, lv_scale_mode_t mode,
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












//  WIFI /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <WiFi.h>
//=============================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================













// MQTT Client //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <PubSubClient.h>
// MQTT CLIENT 
WiFiClient espClient_Wifi;
EthernetClient espClient_Eth;
PubSubClient client;
// mqtt server
String mqtt_server = "192.168.100.2";
// mqtt paras
#define DEVICE_ID "ESP32_001"
#define NAME "GATEWAY 001"
#define LOCATION "ROOM 001"
#define ONLINE "true"
#define RELAY1_STATUS "true"
#define RELAY2_STATUS "false"
#define TEMP "27.0"
#define HUM "75.0"

#define MQTT_ID "esp32_01"
#define MQTT_PW "123"

// mqtt topic
/*
#define TOPIC_TELEMETRY_INFO "device/{device_id}/info"
#define TOPIC_TELEMETRY_MEASUREMENT "device/{device_id}/measurement"
#define TOPIC_TELEMETRY_RELAY_STATUS "device/{device_id}/relay/status"
*/

#define TOPIC_MAX_LEN 128

char TOPIC_TELEMETRY_INFO[TOPIC_MAX_LEN];
char TOPIC_TELEMETRY_MEASUREMENT[TOPIC_MAX_LEN];
char TOPIC_TELEMETRY_RELAYSTATUS[TOPIC_MAX_LEN];
char TOPIC_CMD_RELAYSET[TOPIC_MAX_LEN];

// PROTOCOL CMD
#define CMD_GPIO1_WRITE              "gpio1.write"
#define CMD_GPIO2_WRITE              "gpio2.write"
//=============================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================














// freeRTOS ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

static SemaphoreHandle_t lvgl_mutex;
static SemaphoreHandle_t mqtt_mutex;

static TaskHandle_t taskDisplayHandle = NULL;
static TaskHandle_t taskProcessingHandle = NULL;

TickType_t lastWakeTime;

uint32_t lastPublish;
//=============================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================











//json ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <ArduinoJson.h>
JsonDocument doc;
//=============================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================











// GPIO ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define GPIO_PIN1 26
#define GPIO_PIN2 27

struct GpioConfig
{
    int gpio;
    bool state;
    const char* key;
    int id;
};

// thêm IO thì chỉ cần thêm ở đây
GpioConfig esp32_gpio_config[] =
{
    {GPIO_PIN1, true, "gpio_key1", 1},
    {GPIO_PIN2, true, "gpio_key2", 2},
};
//=============================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================













// biến của sensor //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float temp = 21.5;
float hum = 30.0;
char text2[32];
char text1[32];
//=============================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================
lv_chart_series_t *temp_series;
lv_chart_series_t *hum_series;












// biến flag để khiển //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MODE_CONFIGURATED = false;

bool WIFI_SEARCHING_FLAG = true;
bool WIFI_CONNECTING_FLAG = false;
bool WIFI_STATUS_FLAG = false;
bool WIFI_SCAN_DONE = false;
bool WIFI_PASSWORD_ENTER_SCREEN_FLAG = false;
bool WIFI_SHOW_WIFI_LIST = false;

bool MQTT_IP_ENTER_SCREEN_FLAG = false;
bool MQTT_IP_CONFIGURATED = false;
bool MQTT_STATUS_FLAG = false;
bool MQTT_CONNECTING_FLAG = false;

bool ETHERNET_MODE_FLAG = false;
bool ETHERNET_CONNECTING_FLAG = false;
bool ETHERNET_STATUS_FLAG = false;

bool DASHBOARD_SCREEN_FLAG = false;
//=============================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================


enum uiDisplay
{
  UI_MODE_CONFIGURATION = 0,
  UI_LOADING_SCAN_WIFI,
  UI_LOADING_CONNECTING,
  UI_WIFI_LIST,
  UI_ENTER_WIFI_PASSWORD,
  UI_ENTER_BROKER_IP,
  UI_DASHBOARD
};
uiDisplay UI_DISPLAY = UI_MODE_CONFIGURATION;










// biến processing state //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum processingState
{
  MODE_CONFIGURATION = 0,
  ETHERNET,
  WIFI,
  MQTT,
  TELEMETRY
};

processingState PROCESSING_STATE = MODE_CONFIGURATION;
//=============================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================











// biến wifi để hiển thị lên màn /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MAX_WIFI_SHOW 10
String wifiSSID[MAX_WIFI_SHOW];
int wifiRSSI[MAX_WIFI_SHOW];
int wifiCount = 0;



// biến nhập pass wifi
String selectedSSID = "";
String wifiPassword = "";
bool WAITING_PASSWORD = false;
//=============================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================











void resetW5500() {
  if (RST >= 0) {
    pinMode(RST, OUTPUT);
    digitalWrite(RST, LOW);
    delay(50);
    digitalWrite(RST, HIGH);
    delay(200);
  }
}


void ethernet_DHCP_handling()
{
  switch (DHCP_STATUS){

    case 1:
    {
      Serial.print("[ETHERNET] Connecting to Ethernet... Please wait...");


      if (Ethernet.begin(mac) != 0)
      {
        EthernetConnectStatus = 1;
      }
      else
      {
        EthernetConnectStatus = 0;
      }

      //Ethernet.begin(mac);
      if (Ethernet.localIP() == IPAddress(0, 0, 0, 0))
      {
        // chưa kết nối dc ethernet
        EthernetConnectStatus = 0;
        Serial.println("[ETHERNET] DHCP MODE FAILED: Ethernet Initialization Failure - Cannot get IP Address from DHCP.");
      }
      
      //Ethernet.begin(mac, ip, dns, gateway, subnet);

      else if (Ethernet.localIP() != IPAddress(0, 0, 0, 0))
      {
        //đã kết nối
        Serial.println("[ETHERNET] DHCP MODE SUCCESS: Successfully getting the IP Address from DHCP!");
        EthernetConnectStatus = 1;
      }
      break;
    }
      
      

    case 0:
      Serial.println("[ETHERNET] NOW RUNNING ON LOCAL IP MODE: Running on setted local IP Address.");
      Ethernet.begin(mac, ip, dns, gateway, subnet);
      break;
  }
}

void ethernet_checking()
{
  // Kiểm tra phần cứng W5500 có được nhận diện không
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("[ETHERNET] ERR: Cannot detect the W5500 module!");
  }

  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("[ETHERNET] WARN: Ethernet cable is not connected!.");
  }

  server.begin();
  if (Ethernet.localIP() == IPAddress(0, 0, 0, 0))
  {
    Serial.println("[ETHERNET] Ethernet Initialization Failure! Retry connecting to Wifi...");
  }
  else
  {
    Serial.print("[ETHERNET] Ethernet Initialization Successful! Server is running at IP Address: ");
    Serial.println(Ethernet.localIP());
    Serial.println("[ETHERNET] Connect Telnet/TCP to gate 8080 for testing.");
    client.setClient(espClient_Eth);
  }
}




bool ethernet_init(){
  Serial.println("[ETHERNET] Ethernet (W5500) initiating...");
  SPI.begin(SCK, MISO, MOSI, W5500_CS);
  Ethernet.init(W5500_CS);
  resetW5500();
  ethernet_DHCP_handling();
  ethernet_checking();
  return EthernetConnectStatus;
}


void ethernet_reconnect(){
  Serial.println("[ETHERNET] Ethernet (W5500) disconnected! Try Reconnecting...");
  ethernet_init();
}


void ethernet_maintain(){
  Ethernet.maintain();
}

void action_ethernet_mode_btn_pressed(lv_event_t * e)
{
  Serial.println("[LVGL EVENT] LVGL Event Triggered! Ethernet Mode Button pressed!");
  Serial.println("[MODE CONFIGURATION] Selected Mode: Ethernet");


  MODE_CONFIGURATED = true;
  
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






bool reconnectWiFi()
{
    Serial.println("[WiFi] WiFi disconnected! Try reconnecting...");

    WIFI_STATUS_FLAG = false;
    //WIFI_CONNECTING_FLAG = true;

    // Ngắt kết nối cũ, giữ cấu hình WiFi
    WiFi.disconnect(false, false);
    vTaskDelay(pdMS_TO_TICKS(200));

    // Kết nối lại bằng SSID và password đã lưu
    WiFi.begin(selectedSSID.c_str(), wifiPassword.c_str());

    const TickType_t startTick = xTaskGetTickCount();
    const TickType_t timeout = pdMS_TO_TICKS(15000);

    while (WiFi.status() != WL_CONNECTED)
    {
        if ((xTaskGetTickCount() - startTick) >= timeout)
        {
            Serial.println("[WiFi] Reconnect timeout!");

            //WIFI_CONNECTING_FLAG = false;
            WIFI_STATUS_FLAG = false;
            return false;
        }

        vTaskDelay(pdMS_TO_TICKS(250));
    }

    Serial.println("[WiFi] Reconnected!");
    Serial.print("[WiFi] IP: ");
    Serial.println(WiFi.localIP());

    //WIFI_CONNECTING_FLAG = false;
    WIFI_STATUS_FLAG = true;

    return true;
}




void select_wifi(int index)
{
    if (index < 0 || index >= wifiCount) return;

    selectedSSID = wifiSSID[index];
    wifiPassword = "";
    WAITING_PASSWORD = true;

    Serial.println();
    Serial.println("Selected Wifi SSID: " + selectedSSID);
    Serial.println("Enter the Selected Wifi's Password:");
}








void protocol_cmdGPIOWrite(GpioConfig &config, JsonDocument& request)
{
    Serial.println("CMD: GPIO WRITE");
    JsonObject params = request["params"];
    digitalWrite(config.gpio, params["state"]);

    /*

    JsonDocument response;

    response["id"] = request["id"];
    response["result"] = "ok";

    JsonObject data = response["data"].to<JsonObject>();
    data["state"] = gpio_get(config.gpio);

    mqtt_publish(TOPIC_STT1, response);
    */
}




// hàm build mqtt topics
void mqtt_buildTopics(const char* device_id)
{
    snprintf(TOPIC_TELEMETRY_INFO,
             sizeof(TOPIC_TELEMETRY_INFO),
             "device/%s/info", device_id);

    snprintf(TOPIC_TELEMETRY_MEASUREMENT,
             sizeof(TOPIC_TELEMETRY_MEASUREMENT),
             "device/%s/measurement", device_id);

    snprintf(TOPIC_TELEMETRY_RELAYSTATUS,
             sizeof(TOPIC_TELEMETRY_RELAYSTATUS),
             "device/%s/relay/status", device_id);

    snprintf(TOPIC_CMD_RELAYSET,
             sizeof(TOPIC_CMD_RELAYSET),
             "device/%s/relay/set", device_id);
}












void JsonPayload_buildTelemetry(JsonDocument& doc)
{
    doc.clear();
    

    /*

     //---------------- Device ----------------
    JsonObject device = doc["device"].to<JsonObject>();

    device["id"] = DEVICE_ID1;
    device[""] = FW_VERSION;
    device["uptime"] = millis();

     //---------------- Sensor ----------------
    JsonObject sensor = doc["sensor"].to<JsonObject>();

    sensor["temp1"] = temp1;
    sensor["temp2"] = temp2;

    //---------------- GPIO ----------------
    JsonObject gpio = doc["gpio"].to<JsonObject>();

    gpio["gpio1"] = gpio_get(esp32_gpio_config[0].gpio);
    gpio["gpio2"] = gpio_get(esp32_gpio_config[1].gpio);

    */

    // ---------------- INFO ----------------
    JsonObject info = doc["info"].to<JsonObject>();

    info["device_id"] = DEVICE_ID;
    info["name"]       = NAME;
    info["location"]   = LOCATION;
    info["online"]     = MQTT_STATUS_FLAG;

    // ---------------- MEASUREMENT ----------------
    JsonObject measurement = doc["measurement"].to<JsonObject>();

    measurement["temperature"] = temp;
    measurement["humidity"]    = hum;

    // ---------------- RELAY ----------------
    JsonObject relay = doc["relay"].to<JsonObject>();

    relay["relay_1"] = !esp32_gpio_config[0].state;
    relay["relay_2"] = !esp32_gpio_config[1].state;

}




bool mqttPublish(JsonDocument& doc)
{
    char buffer_topicInfo[256];

    serializeJson(doc["info"], buffer_topicInfo);

    bool result_topicInfo = client.publish(TOPIC_TELEMETRY_INFO, buffer_topicInfo);

    if (result_topicInfo == 0 || client.connected() == 0)
    {
      Serial.print("[MQTT FAILED] ");
    }

    if (result_topicInfo == 1 && client.connected() == 1)
    {
      Serial.print("[MQTT SUCCEED] ");
    }

    Serial.print("Publish ");
    Serial.print(TOPIC_TELEMETRY_INFO);
    Serial.print(" : ");
    Serial.println(buffer_topicInfo);









    char buffer_topicMeasurement[256];

    serializeJson(doc["measurement"], buffer_topicMeasurement);

    bool result_topicMeasurement = client.publish(TOPIC_TELEMETRY_MEASUREMENT, buffer_topicMeasurement);

    if (result_topicMeasurement == 0 || client.connected() == 0)
    {
      Serial.print("[MQTT FAILED] ");
    }

    if (result_topicMeasurement == 1 && client.connected() == 1)
    {
      Serial.print("[MQTT SUCCEED] ");
    }

    Serial.print("Publish ");
    Serial.print(TOPIC_TELEMETRY_MEASUREMENT);
    Serial.print(" : ");
    Serial.println(buffer_topicMeasurement);










    char buffer_topicRelay[256];

    serializeJson(doc["relay"], buffer_topicRelay);

    bool result_topicRelay = client.publish(TOPIC_TELEMETRY_RELAYSTATUS, buffer_topicRelay);

    if (result_topicRelay == 0 || client.connected() == 0)
    {
      Serial.print("[MQTT FAILED] ");
    }

    if (result_topicRelay == 1 && client.connected() == 1)
    {
      Serial.print("[MQTT SUCCEED] ");
    }

    Serial.print("Publish ");
    Serial.print(TOPIC_TELEMETRY_RELAYSTATUS);
    Serial.print(" : ");
    Serial.println(buffer_topicRelay);


    bool result = result_topicInfo && result_topicMeasurement && result_topicRelay;



    return result;
}





void protocol_processCommand(JsonDocument& doc)
{
    String cmd = doc["cmd"];

    JsonObject params = doc["params"];

    if(cmd == CMD_GPIO1_WRITE)
        protocol_cmdGPIOWrite(esp32_gpio_config[0], doc);

    else if(cmd == CMD_GPIO2_WRITE)
        protocol_cmdGPIOWrite(esp32_gpio_config[1], doc);

}




void mqtt_callback(char* topic, byte* payload, unsigned int length)
{
    JsonDocument doc;

    DeserializationError err = deserializeJson(doc, payload, length);

    if (err)
    {
        Serial.print("JSON Error: ");
        Serial.println(err.c_str());
        return;
    }


    if (strcmp(topic, TOPIC_CMD_RELAYSET) == 0)
    {
        protocol_processCommand(doc);
    }
}














void gpio_init()
{
    pinMode(GPIO_PIN1, OUTPUT);
    digitalWrite(GPIO_PIN1, LOW);

    pinMode(GPIO_PIN2, OUTPUT);
    digitalWrite(GPIO_PIN2, HIGH);
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

    /*For example ("my_..." functions needs to be implemented by you)
    uint32_t w = lv_area_get_width(area);
    uint32_t h = lv_area_get_height(area);

    my_set_window(area->x1, area->y1, w, h);
    my_draw_bitmaps(px_map, w * h);
     */

    /*Call it to tell LVGL you are ready*/
    lv_display_flush_ready(disp);
}

/*Read the touchpad*/
void my_touchpad_read( lv_indev_t * indev, lv_indev_data_t * data )
{
    /*For example  ("my_..." functions needs to be implemented by you)
    int32_t x, y;
    bool touched = my_get_touch( &x, &y );

    if(!touched) {
        data->state = LV_INDEV_STATE_RELEASED;
    } else {
        data->state = LV_INDEV_STATE_PRESSED;

        data->point.x = x;
        data->point.y = y;
    }
     */
}

/*use Arduinos millis() as tick source*/
static uint32_t my_tick(void)
{
    return millis();
}


void chart_setup(void)
{
    chart = objects.chart;
    lv_obj_t *parent = lv_obj_get_parent(chart);

    lv_obj_set_style_pad_all(chart, 0, 0);     // để tick khớp mép chart
    lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
    lv_chart_set_point_count(chart, CHART_POINTS);                // tối đa 10 điểm
    lv_chart_set_update_mode(chart, LV_CHART_UPDATE_MODE_SHIFT);  // đầy thì đẩy điểm cũ ra
    lv_chart_set_axis_range(chart, LV_CHART_AXIS_PRIMARY_Y,   0, 50);   // °C
    lv_chart_set_axis_range(chart, LV_CHART_AXIS_SECONDARY_Y, 0, 100);  // %
    lv_chart_set_div_line_count(chart, 5, CHART_POINTS - 1);
    lv_obj_set_style_size(chart, 0, 0, LV_PART_INDICATOR);        // bỏ chấm tròn mỗi điểm

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
    lv_scale_set_major_tick_every(scale_x, 3);
    lv_scale_set_text_src(scale_x, xlabels);
    for (int i = 0; i < 4; i++) xbuf[i][0] = '\0';
}


void lvgl_init()
{
  String LVGL_Arduino = "LVGL Hello ESP32! ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
  Serial.println( LVGL_Arduino );


 

 



  lv_init();

  /*Set a tick source so that LVGL will know how much time elapsed. */
  lv_tick_set_cb(my_tick);




   /* register print function for debugging */
  #if LV_USE_LOG != 0
    lv_log_register_print_cb( my_print );
  #endif

  lv_display_t * disp;
  #if LV_USE_TFT_ESPI
    /*TFT_eSPI can be enabled lv_conf.h to initialize the display in a simple way*/
    disp = lv_tft_espi_create(TFT_HOR_RES, TFT_VER_RES, draw_buf, sizeof(draw_buf));
    lv_display_set_rotation(disp, TFT_ROTATION);

  #else
    /*Else create a display yourself*/
    disp = lv_display_create(TFT_HOR_RES, TFT_VER_RES);
    lv_display_set_flush_cb(disp, my_disp_flush);
    lv_display_set_buffers(disp, draw_buf, NULL, sizeof(draw_buf), LV_DISPLAY_RENDER_MODE_PARTIAL);
  #endif

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






void ui_mode_configuration()
{
  /*
  // màn chọn mode - mode configuration
  lv_obj_add_flag(objects.loading_spinner, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(objects.wifi_label, LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_flag(objects.screen_1_header_label, LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_flag(objects.screen_1_panel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(objects.screen_1_container_2, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(objects.screen_1_container, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(objects.screen_1_container_1, LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_flag(objects.led, LV_OBJ_FLAG_HIDDEN);

  lv_label_set_text_static(objects.screen_1_header_label, "MODE CONFIGURATION");
  lv_led_set_color(objects.led, lv_color_hex(0x3fd700));
  */
}

void ui_loading_connecting()
{
  /*
  // màn loading đang kết nối 
  lv_label_set_text_static(objects.wifi_label, "CONNECTING...");
  lv_obj_clear_flag(objects.loading_spinner, LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_flag(objects.wifi_label, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(objects.screen_1_header_label, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(objects.screen_1_panel, LV_OBJ_FLAG_HIDDEN);
  lv_led_set_color(objects.led, lv_color_hex(0xF2BB27));
  */
}


void ui_loading_searching_lans()
{
  /*
  // màn loading searching lans
  lv_label_set_text_static(objects.wifi_label, "SEARCHING LANs...");
  lv_obj_clear_flag(objects.loading_spinner, LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_flag(objects.wifi_label, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(objects.screen_1_header_label, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(objects.screen_1_panel, LV_OBJ_FLAG_HIDDEN);
  lv_led_set_color(objects.led, lv_color_hex(0xF2BB27));
  */
}

void ui_dashboard()
{
  /*
  // màn dashboard
  lv_obj_add_flag(objects.loading_spinner, LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_flag(objects.wifi_label, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(objects.screen_1_header_label, LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_flag(objects.screen_1_panel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(objects.screen_1_container_2, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(objects.screen_1_container, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(objects.screen_1_container_1, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(objects.mqtt_status_label, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(objects.led_status_mqtt, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(objects.lan_status_label, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(objects.led_status_lan, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(objects.sensor_1_label, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(objects.sensor_2_label, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(objects.gpio_1_label, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(objects.gpio_2_label, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(objects.switch_gpio_1, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(objects.switch_gpio_2, LV_OBJ_FLAG_HIDDEN);

  lv_obj_clear_flag(objects.led, LV_OBJ_FLAG_HIDDEN);

  lv_led_set_color(objects.led_status_lan, lv_color_hex(0x0eff00));

  lv_label_set_text_static(objects.wifi_label, "MQTT Dashboard");
  lv_label_set_text(objects.sensor_1_label, "SEN1: ");
  lv_label_set_text(objects.sensor_2_label, "SEN2: ");          
  lv_label_set_text(objects.gpio_1_label, "GPIO1");    
  lv_label_set_text(objects.gpio_2_label, "GPIO2");    
  lv_led_set_color(objects.led, lv_color_hex(0x3fd700));


  lv_label_set_text(objects.sensor_1_label, text1);

  lv_label_set_text(objects.sensor_2_label, text2);


  lv_label_set_text(objects.gpio_1_label, "GPIO1");
  if (!esp32_gpio_config[0].state)
  {
    lv_obj_add_state(objects.switch_gpio_1, LV_STATE_CHECKED);
  }
  else
  {
    lv_obj_clear_state(objects.switch_gpio_1, LV_STATE_CHECKED);
  }
  


  lv_label_set_text(objects.gpio_2_label, "GPIO2");
  if (!esp32_gpio_config[1].state)
  {
    lv_obj_add_state(objects.switch_gpio_2, LV_STATE_CHECKED);
  }
  else
  {
    lv_obj_clear_state(objects.switch_gpio_2, LV_STATE_CHECKED);
  }
  */
}


// update ani chấm chấm chờ kết nối mqtt và led chuyển vàng (trạng thái chờ)
void ui_update_mqtt_connecting_ani_dots_label_yellow_led()
{
  /*
  lv_led_set_color(objects.led_status_mqtt, lv_color_lighten(lv_color_hex(0xf2bb27), 92));

  if (millis() - mqttAnimMillis >= 500)
  {
      mqttAnimMillis = millis();

      mqttDotCount++;
      if (mqttDotCount > 5)
      {
          mqttDotCount = 1;
      }
  }

  char mqttText[32];

  snprintf(mqttText, sizeof(mqttText),
          "MQTT STATUS : %.*s",
          mqttDotCount,
          ".....");

  lv_label_set_text(objects.mqtt_status_label, mqttText);
  */
}

void ui_update_mqtt_status_label_led()
{
  /*
  // đã kết nối dc mqtt + wf
  if (MQTT_STATUS_FLAG)
  {                   
    lv_led_set_color(objects.led_status_mqtt, lv_color_hex(0x0eff00));
    lv_label_set_text(objects.mqtt_status_label, "MQTT STATUS : OK");
  }
  // chưa kết nối dc mqtt, đã kết nối dc wf
  else
  {
    lv_led_set_color(objects.led_status_mqtt, lv_color_hex(0xff0000));
    lv_label_set_text(objects.mqtt_status_label, "MQTT STATUS : NC");
  }
  */
}


void ui_update_lan_connecting_ani_dots_label_yellow_led()
{
  /*
  lv_led_set_color(objects.led_status_lan, lv_color_lighten(lv_color_hex(0xf2bb27), 92));

  if (millis() - mqttAnimMillis >= 500)
  {
      mqttAnimMillis = millis();

      mqttDotCount++;
      if (mqttDotCount > 5)
      {
          mqttDotCount = 1;
      }
  }

  char mqttText[32];

  snprintf(mqttText, sizeof(mqttText),
          "LAN STATUS    : %.*s",
          mqttDotCount,
          ".....");

  lv_label_set_text(objects.lan_status_label, mqttText);
  */
}


// màn nhập wifi password
void ui_enter_wifi_password()
{
  /*
  lv_label_set_text_static(objects.wifi_label, "ENTER PASSWORD");
  lv_obj_add_flag(objects.loading_spinner, LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_flag(objects.wifi_label, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(objects.screen_1_header_label, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(objects.screen_1_panel, LV_OBJ_FLAG_HIDDEN);

  lv_led_set_color(objects.led, lv_color_hex(0x3fd700));
  */
}

// màn wifi list hiển thị sau khi scan

void ui_wifi_list_update()
{
  lv_obj_t* wifiLabels[MAX_WIFI_SHOW] = {
    objects.scanned_wifi_label_1, objects.scanned_wifi_label_10,
    objects.scanned_wifi_label_2, objects.scanned_wifi_label_3,
    objects.scanned_wifi_label_4, objects.scanned_wifi_label_5,
    objects.scanned_wifi_label_6, objects.scanned_wifi_label_7,
    objects.scanned_wifi_label_9, objects.scanned_wifi_label_8
  };

  char buf[64];
  for (int i = 0; i < MAX_WIFI_SHOW; i++) {
    if (i < wifiCount) {
      snprintf(buf, sizeof(buf), "%d. %s (%d dBm)", (i+1),wifiSSID[i].c_str(), wifiRSSI[i]);
      lv_label_set_text(wifiLabels[i], buf);
      lv_obj_clear_flag(wifiLabels[i], LV_OBJ_FLAG_HIDDEN);
      
    } else {
      lv_label_set_text(wifiLabels[i], ""); // không đủ 10 mạng thì để trống
    }
  }
}



void ui_wifi_list()
{
  /*
  // màn hiển thị danh sách wifi
  lv_obj_t* wifiLabels[MAX_WIFI_SHOW] = {
  objects.wifi_label_1, objects.wifi_label_2, objects.wifi_label_3,
  objects.wifi_label_4, objects.wifi_label_5, objects.wifi_label_6,
  objects.wifi_label_7, objects.wifi_label_8, objects.wifi_label_9,
  objects.wifi_label_10
  };

  char buf[64];
  for (int i = 0; i < MAX_WIFI_SHOW; i++) {
    if (i < wifiCount) {
      snprintf(buf, sizeof(buf), "%s (%d dBm)", wifiSSID[i].c_str(), wifiRSSI[i]);
      lv_label_set_text(wifiLabels[i], buf);
      lv_obj_clear_flag(wifiLabels[i], LV_OBJ_FLAG_HIDDEN);
      
    } else {
      lv_label_set_text(wifiLabels[i], ""); // không đủ 10 mạng thì để trống
    }
  }

  
  lv_obj_add_flag(objects.loading_spinner, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(objects.wifi_label, LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_flag(objects.screen_1_header_label, LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_flag(objects.screen_1_panel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(objects.screen_1_container_2, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(objects.screen_1_container, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(objects.screen_1_container_1, LV_OBJ_FLAG_HIDDEN); 
  lv_obj_clear_flag(objects.led, LV_OBJ_FLAG_HIDDEN);

  lv_led_set_color(objects.led, lv_color_hex(0x3fd700));
  */
}






void ui_update_dashboard()
{
  /*
  // đang connecting mqtt
  if (MQTT_CONNECTING_FLAG)
  {
    ui_update_mqtt_connecting_ani_dots_label_yellow_led();
  }
  
  else
  {
    ui_update_mqtt_status_label_led();

  }

  if (!ETHERNET_MODE_FLAG)
  {
    if (!WIFI_STATUS_FLAG)
    {
      ui_update_lan_connecting_ani_dots_label_yellow_led();

    }
    else
    {
      lv_led_set_color(objects.led_status_lan, lv_color_hex(0x0eff00));
      lv_label_set_text(objects.lan_status_label, "LAN STATUS    : OK");
    }
  }

  else if (!ETHERNET_STATUS_FLAG)
  {
    ui_update_lan_connecting_ani_dots_label_yellow_led();

  }
  else
  {
    lv_led_set_color(objects.led_status_lan, lv_color_hex(0x0eff00));
    lv_label_set_text(objects.lan_status_label, "LAN STATUS    : OK");
  }
  */
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ui_selected_wifi_name_update()
{

  lv_label_set_text(objects.enter_password_screen_label_3, selectedSSID.c_str());
      
}


void ui_dashboard_update()
{
  if (MQTT_STATUS_FLAG)
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
      }
    }                                    
  }
}




static void update_x_labels(void)
{
    const int idx[4] = {0, 3, 6, 9};           // điểm tương ứng với tick chính
    for (int i = 0; i < 4; i++) {
        uint32_t s = ts[idx[i]];
        if (s == 0) xbuf[i][0] = '\0';
        else lv_snprintf(xbuf[i], sizeof(xbuf[i]), "%02u:%02u",
                         (unsigned)((s / 3600) % 24), (unsigned)((s / 60) % 60));
    }
    lv_obj_invalidate(scale_x);
}

// gọi mỗi lần có mẫu mới
void chart_push(float temp, float hum, uint32_t sec_of_day)
{
    for (int i = 0; i < CHART_POINTS - 1; i++) ts[i] = ts[i + 1];
    ts[CHART_POINTS - 1] = sec_of_day ? sec_of_day : 1;

    lv_chart_set_next_value(chart, ser_temp, (int32_t)temp);
    lv_chart_set_next_value(chart, ser_hum,  (int32_t)hum);
    update_x_labels();
}







void ui_display()
{
  UI_DISPLAY = ui_display_processing();

  switch (UI_DISPLAY)
  {
    case (UI_MODE_CONFIGURATION):
      //
      // màn chọn mode - mode configuration
      loadScreen(SCREEN_ID_SETUP_SCREEN);
      break;
    
    case (UI_LOADING_SCAN_WIFI):
      //
      loadScreen(SCREEN_ID_LOADING_SCANNING_WIFI_SCREEN);
      break;

    case (UI_LOADING_CONNECTING):
      //
      loadScreen(SCREEN_ID_LOADING_CONNECTING_SCREEN);
      break;

    case (UI_WIFI_LIST):
      //
      loadScreen(SCREEN_ID_WIFI_LIST_SCREEN);
      ui_wifi_list_update();
      break;

    case (UI_ENTER_WIFI_PASSWORD):
      //
      loadScreen(SCREEN_ID_ENTER_PASSWORD_SCREEN);
      ui_selected_wifi_name_update();
      break;

    case (UI_ENTER_BROKER_IP):
      //
      loadScreen(SCREEN_ID_ENTER_BROKER_IP_SCREEN);
      break;

    case (UI_DASHBOARD):
      //
      loadScreen(SCREEN_ID_DASHBOARD_SCREEN);
      ui_dashboard_update();
      static uint32_t last_push = 0;
      uint32_t now = millis();
      if (now - last_push >= 1000) {
          last_push = now;
          chart_push(temp, hum, (now / 1000) % 86400);
      }
      break;
  }
}
























void taskReadingSensorGpio(void *pvParameter)
{
  for (;;)
  {
    temp = random(150,350)/10.0;
    hum = random(150,350)/10.0;

    // Sensor 1
    snprintf(text1, sizeof(text1), "%d", int(temp));

    // Sensor 2
    snprintf(text2, sizeof(text2), "%d", int(hum));

    esp32_gpio_config[0].state = digitalRead(GPIO_PIN1);
    
    esp32_gpio_config[1].state = digitalRead(GPIO_PIN2);

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}





void modeConfigurating()
{
  Serial.println("Mode Configurating Task");
  if (!MODE_CONFIGURATED)
  {

    /*
    if (ETHERNET_CONNECTING_FLAG)
    {
      ethernet_init();
      if (EthernetConnectStatus)
      {
        MODE_CONFIGURATED = true;
        ETHERNET_MODE_FLAG = true;
        ETHERNET_CONNECTING_FLAG = false;
        DASHBOARD_SCREEN_FLAG = true;
        MQTT_CONNECTING_FLAG = true;
        ETHERNET_STATUS_FLAG = true;
        PROCESSING_STATE = MQTT;
      }
      else
      {
        MODE_CONFIGURATED = false;
        ETHERNET_MODE_FLAG = false;        
        ETHERNET_CONNECTING_FLAG = false;  
        DASHBOARD_SCREEN_FLAG = false;  
        MQTT_CONNECTING_FLAG = false; 
        ETHERNET_STATUS_FLAG = false; 
        PROCESSING_STATE = MODE_CONFIGURATION;  
      }
    }
    */
  }
  else
  {
    if (ETHERNET_MODE_FLAG)
    {
      PROCESSING_STATE = ETHERNET;
    }
    else
    {
      PROCESSING_STATE = WIFI;
    }
  }
  vTaskDelay(pdMS_TO_TICKS(1000));
}

void connectingEthernet()
{
  Serial.println("Connecting Ethernet Task");

  ETHERNET_CONNECTING_FLAG = true;

  if (ethernet_init())
  {
    ETHERNET_CONNECTING_FLAG = false;
    
    PROCESSING_STATE = MQTT;
  }
  else
  {
    ETHERNET_CONNECTING_FLAG = false;

    MODE_CONFIGURATED = false;

    PROCESSING_STATE = MODE_CONFIGURATION;
  }
}
void searchingWifi()
{
  vTaskDelay(pdMS_TO_TICKS(1000));
  Serial.println("Searching WiFi Task");
  if (MODE_CONFIGURATED && !ETHERNET_MODE_FLAG && !WIFI_SCAN_DONE)
  {
    WIFI_SEARCHING_FLAG = true;
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    vTaskDelay(pdMS_TO_TICKS(100));

    Serial.println("Scanning WiFi...");
    
    int n = WiFi.scanNetworks();

    if (n == 0) 
    {
      Serial.println("Cannot found any WiFi.");
      if (xSemaphoreTake(lvgl_mutex, portMAX_DELAY) == pdTRUE) 
      {
        wifiCount = 0;
        xSemaphoreGive(lvgl_mutex);
      }
    } 
    else 
    {
      Serial.printf("Found %d WiFi:\n", n);
      int maxNetworks = min(n, MAX_WIFI_SHOW);

      // đọc dữ liệu từ WiFi lib ra biến tạm trước (không giữ mutex lâu)
      String tempSSID[MAX_WIFI_SHOW];
      int tempRSSI[MAX_WIFI_SHOW];

      for (int i = 0; i < maxNetworks; i++) {
        tempSSID[i] = WiFi.SSID(i);
        tempRSSI[i] = WiFi.RSSI(i);

        Serial.printf("%2d: %-32s (RSSI: %d dBm) %s\n",
                      i + 1,
                      tempSSID[i].c_str(),
                      tempRSSI[i],
                      (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? "Open" : "Encrypted");
      }

      // ghi vào biến chung, có mutex bảo vệ vì taskDisplay cũng đọc biến này
      if (xSemaphoreTake(lvgl_mutex, portMAX_DELAY) == pdTRUE) {
        wifiCount = maxNetworks;
        for (int i = 0; i < maxNetworks; i++) {
          wifiSSID[i] = tempSSID[i];
          wifiRSSI[i] = tempRSSI[i];
        }
        xSemaphoreGive(lvgl_mutex);
      }
    }

    Serial.println("--------------------------------------------------");
    WIFI_SEARCHING_FLAG = false;
    WIFI_SCAN_DONE = true;
    WIFI_SHOW_WIFI_LIST = true;
    WiFi.scanDelete();
  }
  else return;
}
void connectingMQTT()
{
  Serial.println("Connecting MQTT Task");
  if (!MQTT_IP_CONFIGURATED)
  {

    
    MQTT_IP_ENTER_SCREEN_FLAG = true;


    Serial.println("Enter MQTT Server IP:");


  }

  else
  {
    
    if (!ETHERNET_MODE_FLAG) //mode wifi
    {
      Serial.println("[Mode: WiFi] Setting WiFi Client as the Client for Server...");
      client.setClient(espClient_Wifi);
    }
    else //mode ethernet
    {
      Serial.println("[Mode: Ethernet] Setting Ethernet Client as the Client for Server...");
      client.setClient(espClient_Eth);
    }

    

    Serial.println("Client setting Server...");
    client.setServer(mqtt_server.c_str(), 1883);
    client.setCallback(mqtt_callback);

    
    Serial.println("Client connecting to Server...");
    if (client.connect(DEVICE_ID))
    {
      MQTT_IP_CONFIGURATED = true;
      MQTT_IP_ENTER_SCREEN_FLAG = false;
      MQTT_CONNECTING_FLAG = false;
      MQTT_STATUS_FLAG = true;
      PROCESSING_STATE = TELEMETRY;
      Serial.println("MQTT Connected");
      mqtt_buildTopics(DEVICE_ID);
      client.subscribe(TOPIC_TELEMETRY_INFO);
      Serial.print("Subscribe: ");
      Serial.println(TOPIC_TELEMETRY_INFO);
      client.subscribe(TOPIC_TELEMETRY_MEASUREMENT);
      Serial.print("Subscribe: ");
      Serial.println(TOPIC_TELEMETRY_MEASUREMENT);
      client.subscribe(TOPIC_TELEMETRY_RELAYSTATUS);
      Serial.print("Subscribe: ");
      Serial.println(TOPIC_TELEMETRY_RELAYSTATUS);
      client.subscribe(TOPIC_CMD_RELAYSET);
      Serial.print("Subscribe: ");
      Serial.println(TOPIC_CMD_RELAYSET);
    }
    else
    {
      MQTT_IP_CONFIGURATED = false;
      MQTT_IP_ENTER_SCREEN_FLAG = true;
      MQTT_CONNECTING_FLAG = false;
      MQTT_STATUS_FLAG = false;
      PROCESSING_STATE = MQTT;
      Serial.print("MQTT Failed: ");
      Serial.println(client.state());
      
    }
  }

  vTaskDelay(pdMS_TO_TICKS(100));


}


void mqttTelemetry()
{
  //Serial.println("Telemetry Task");


  // WiFi reconnect
  if (WiFi.status() != WL_CONNECTED)
  {
      if (selectedSSID.length() > 0)
          reconnectWiFi();

      vTaskDelay(pdMS_TO_TICKS(10));
      return;
  }

  // MQTT reconnect
  if (!client.connected())
  {
      MQTT_STATUS_FLAG = false;
      MQTT_CONNECTING_FLAG = true;
      PROCESSING_STATE = MQTT;

      vTaskDelay(pdMS_TO_TICKS(10));
      return;
  }

  MQTT_STATUS_FLAG = true;
  MQTT_CONNECTING_FLAG = false;

  // Xử lý MQTT liên tục
  client.loop();

  // Publish mỗi 1 giây
  uint32_t now = millis();

  if (now - lastPublish >= 1000)
  {
      lastPublish = now;

      JsonPayload_buildTelemetry(doc);
      mqttPublish(doc);
  }

  vTaskDelay(pdMS_TO_TICKS(10));
}


void taskProcessing(void *pvParameters) 
{
  
  for (;;)
  {
    /*Serial.printf(
      "[DEBUG task processing] state=%d\n",
      PROCESSING_STATE
    );*/

    switch (PROCESSING_STATE)
    {


      case MODE_CONFIGURATION:
        modeConfigurating();
        break;

      
      case ETHERNET:
        connectingEthernet();
        break;



      case WIFI:
        searchingWifi();
        break;



      case MQTT:
        connectingMQTT();
        break;



      case TELEMETRY:
        mqttTelemetry();
        break;



    }
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}












void taskStatusChecking(void *pvParameters)
{
  for (;;)
  {
    if (PROCESSING_STATE == TELEMETRY)
    {

    }
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}







void taskErrorHandling(void *pvParameters)
{
  for (;;)
  {
    /*
    MQTT_STATUS_FLAG = client.connected();
    if (PROCESSING_STATE == TELEMETRY)
    {
      if (!MQTT_STATUS_FLAG)
      {
          Serial.println("MQTT Disconnected. Try reconnecting MQTT...");

          //if (client.connect(DEVICE_ID1, MQTT_ID1, MQTT_PW1))
          if (client.connect(DEVICE_ID_1))
          {
              Serial.println("MQTT Connected");

              client.subscribe(TOPIC_TELEMETRY_1);
              Serial.print("Subscribe: ");
              Serial.println(TOPIC_TELEMETRY_1);
          }
          else
          {
              Serial.print("MQTT Failed: ");
              Serial.println(client.state());
              vTaskDelay(pdMS_TO_TICKS(2000));  
          }
      }
    }
    */
    
    vTaskDelay(pdMS_TO_TICKS(5000));  
  }
  
}





void taskDisplay(void *pvParameters) 
{                          

  for (;;) 
  {

    if (xSemaphoreTake(lvgl_mutex, portMAX_DELAY) == pdTRUE) 
    {
      ui_display();
    
      lv_timer_handler();
      ui_tick();
      xSemaphoreGive(lvgl_mutex);
    }
    vTaskDelay(pdMS_TO_TICKS(5));
  }
}






















void taskSerialInput(void *pvParameters)
{
    String input = "";

    for (;;)
    {
        while (Serial.available())
        {
            char c = Serial.read();

            if (c == '\n' || c == '\r')
            {
                input.trim();

                if (input.length() == 0)
                    continue;

                // Đang chờ nhập mật khẩu
                if (WAITING_PASSWORD)
                {
                    wifiPassword = input;
                    WAITING_PASSWORD = false;
                    WIFI_PASSWORD_ENTER_SCREEN_FLAG = false;

                    Serial.println("Connecting to: " + selectedSSID);

                    WIFI_CONNECTING_FLAG = true;

                    WiFi.begin(selectedSSID.c_str(),
                               wifiPassword.c_str());

                    unsigned long start = millis();

                    while (WiFi.status() != WL_CONNECTED &&
                           millis() - start < 15000)
                    {
                        vTaskDelay(pdMS_TO_TICKS(250));
                    }

                    WIFI_CONNECTING_FLAG = false;

                    if (WiFi.status() == WL_CONNECTED)
                    {
                        WIFI_SHOW_WIFI_LIST = false;
                        WIFI_STATUS_FLAG = true;
                        DASHBOARD_SCREEN_FLAG = true;
                        PROCESSING_STATE = MQTT;
                        Serial.println("WIFI CONNECTED!");
                        Serial.print("IP: ");
                        Serial.println(WiFi.localIP());
                        //Serial.println(PROCESSING_STATE);

                        MQTT_CONNECTING_FLAG = true;        

                    }
                    else
                    {
                        WIFI_SHOW_WIFI_LIST = true;
                        WIFI_STATUS_FLAG = false;
                        DASHBOARD_SCREEN_FLAG = false;
                        PROCESSING_STATE = WIFI;
                        Serial.println("WIFI CONNECT FAILED!");
                        Serial.println("Re-enter 'w1' to 'w10' to retry.");
                        
                    }
                }


                
                else if (!MQTT_IP_CONFIGURATED && (PROCESSING_STATE == MQTT))
                {
                  mqtt_server = input;
                  MQTT_IP_CONFIGURATED = true;


                  Serial.printf("IP set: [%s], len=%d\n",
                  mqtt_server.c_str(),
                  mqtt_server.length());

                  Serial.printf("state=%d, connecting=%d\n",
                                PROCESSING_STATE,
                                MQTT_CONNECTING_FLAG);

                  Serial.println("MQTT Server IP set to: " + mqtt_server);
                }

                // Chọn WiFi bằng w1-w10
                else if (input[0] == 'w')
                {
                    int index = input.substring(1).toInt();

                    if (index < 1 || index > MAX_WIFI_SHOW)
                    {
                        Serial.println("Enter 'w1' to 'w10'!");
                    }
                    else if (xSemaphoreTake(lvgl_mutex, portMAX_DELAY) == pdTRUE)
                    {
                        if (index <= wifiCount)
                        {
                          
                            lv_obj_t* wifiLabels[MAX_WIFI_SHOW] = {
                                objects.scanned_wifi_label_1, objects.scanned_wifi_label_2,
                                objects.scanned_wifi_label_3, objects.scanned_wifi_label_4,
                                objects.scanned_wifi_label_5, objects.scanned_wifi_label_6,
                                objects.scanned_wifi_label_7, objects.scanned_wifi_label_8,
                                objects.scanned_wifi_label_9, objects.scanned_wifi_label_10
                            };

                            lv_obj_send_event(
                                wifiLabels[index - 1],
                                LV_EVENT_CLICKED,
                                NULL
                            );
                            WIFI_SHOW_WIFI_LIST = false;
                            WIFI_PASSWORD_ENTER_SCREEN_FLAG = true;
                            
                        }
                        else
                        {
                            Serial.println("Wifi does not exist!");
                            WIFI_PASSWORD_ENTER_SCREEN_FLAG = false;
                        }

                        xSemaphoreGive(lvgl_mutex);
                    }
                }


                // ------- mấy lệnh dùng để mô phỏng cảm ứng ------- //

                // 'dc' -> disconnect all
                else if (input[0] == 'd' && input[1] == 'c')
                {
                  

                  WIFI_STATUS_FLAG = false;
                  MQTT_STATUS_FLAG = false;
                  ETHERNET_STATUS_FLAG = false;
                  DASHBOARD_SCREEN_FLAG = false;
                  MODE_CONFIGURATED = false;
                  ETHERNET_MODE_FLAG = false;
                  EthernetConnectStatus = false;
                  MQTT_IP_CONFIGURATED = false;
                  WIFI_SCAN_DONE = false;
                  PROCESSING_STATE = MODE_CONFIGURATION;
                  Serial.println("Disconnected!");
                  WiFi.disconnect();
                  resetW5500();
                  client.disconnect();

                }
                else if (input[0] == '.' && input[1] == 'w' && input[2] == 'd')
                {

                  WiFi.disconnect();
                  Serial.println("Wifi Disconnected!");
                  
                }
                else if (input[0] == '.' && input[1] == 'm' && input[2] == 'd')
                {

                  client.disconnect();
                  Serial.println("MQTT Disconnected!");
                  
                }

                // '.d' scroll panel chứa label wifi xuống 
                else if (input[0] == '.' && input[1] == 'd')
                {
                  
                  Serial.println("Scrolling down!");
                  if (xSemaphoreTake(lvgl_mutex, portMAX_DELAY) == pdTRUE)
                  {
                    lv_obj_scroll_by(objects.wifi_list_screen_container, 0, -100, LV_ANIM_ON);
                    xSemaphoreGive(lvgl_mutex);
                  }
                }
                // '.u' scroll panel chứa label wifi lên
                else if (input[0] == '.' && input[1] == 'u')
                {
                  
                  Serial.println("Scrolling up!");
                  if (xSemaphoreTake(lvgl_mutex, portMAX_DELAY) == pdTRUE)
                  {
                    lv_obj_scroll_by(objects.wifi_list_screen_container, 0, 100, LV_ANIM_ON);
                    xSemaphoreGive(lvgl_mutex);
                  }
                  
                }

                else if (input[0] == '.' && input[1] == '1')
                {
                  Serial.println("Toggled GPIO1!");
                  digitalWrite(GPIO_PIN1, !digitalRead(GPIO_PIN1));
                }
                else if (input[0] == '.' && input[1] == '2')
                {
                  Serial.println("Toggled GPIO2!");
                  digitalWrite(GPIO_PIN2, !digitalRead(GPIO_PIN2));
                }
                else if (input[0] == '.' && input[1] == 's' && input[2] == '1')
                {
                  Serial.println("Switch GPIO 1 pressed!");
                  if (xSemaphoreTake(lvgl_mutex, portMAX_DELAY) == pdTRUE)
                  {
                    
                    lv_obj_send_event(objects.relay_switch_1, LV_EVENT_CLICKED, NULL);
                    xSemaphoreGive(lvgl_mutex);
                    
                  }
                }
                else if (input[0] == '.' && input[1] == 's' && input[2] == '2')
                {
                  Serial.println("Switch GPIO 2 pressed!");
                  if (xSemaphoreTake(lvgl_mutex, portMAX_DELAY) == pdTRUE)
                  {
                    
                    lv_obj_send_event(objects.relay_switch_2, LV_EVENT_CLICKED, NULL);
                    xSemaphoreGive(lvgl_mutex);
                    
                  }
                }
                else if (input[0] == '.' && input[1] == 'm' && input[2] == 'e')
                {
                  Serial.println("Ethernet Mode Button pressed!");
                  if (xSemaphoreTake(lvgl_mutex, portMAX_DELAY) == pdTRUE)
                  {
                    
                    lv_obj_send_event(objects.ethernet_mode_btn, LV_EVENT_PRESSED, NULL);
                    xSemaphoreGive(lvgl_mutex);
                    
                  }
                }
                else if (input[0] == '.' && input[1] == 'm' && input[2] == 'w')
                {
                  Serial.println("WiFi Mode Button pressed!");
                  if (xSemaphoreTake(lvgl_mutex, portMAX_DELAY) == pdTRUE)
                  {
                    lv_obj_send_event(objects.wifi_mode_btn, LV_EVENT_PRESSED, NULL);
                    xSemaphoreGive(lvgl_mutex);
                    
                  }
                }
                else if (input[0] == '.' && input[1] == 'm' && input[2] != 'c')
                {
                  ethernet_init();
                }
                else if (input[0] == '.' && input[1] == 'm' && input[2] == 'c')
                {

                  Serial.println("\n========== ETHERNET INFO ==========");

                  Serial.print("Hardware: ");
                  Serial.println((int)Ethernet.hardwareStatus());

                  Serial.print("Link: ");
                  Serial.println((int)Ethernet.linkStatus());

                  Serial.print("MAC: ");
                  byte mac[6];
                  Ethernet.MACAddress(mac);

                  for (int i = 0; i < 6; i++)
                  {
                      if (i > 0) Serial.print(":");
                      if (mac[i] < 16) Serial.print("0");
                      Serial.print(mac[i], HEX);
                  }
                  Serial.println();

                  Serial.print("IP: ");
                  Serial.println(Ethernet.localIP());

                  Serial.print("Subnet: ");
                  Serial.println(Ethernet.subnetMask());

                  Serial.print("Gateway: ");
                  Serial.println(Ethernet.gatewayIP());

                  Serial.print("DNS: ");
                  Serial.println(Ethernet.dnsServerIP());

                  Serial.println("===================================\n");
                }
                else if (input[0] == 'c')
                {

                    MODE_CONFIGURATED = !MODE_CONFIGURATED;
                    Serial.print("MODE_CONFIGURATED: ");
                    Serial.println(MODE_CONFIGURATED);

                }
                else if (input[0] == '.' && input[1] == 'c' && input[2] == 'e')
                {
                  Serial.println("Selected Mode: Ethernet");
                  Serial.println("Ethernet Initialization...");
                  ETHERNET_CONNECTING_FLAG = true;
                  ethernet_init();
                  if (EthernetConnectStatus)
                  {
                    MODE_CONFIGURATED = true;
                    ETHERNET_MODE_FLAG = true;
                    ETHERNET_CONNECTING_FLAG = false;
                  }
                  else
                  {
                    MODE_CONFIGURATED = false;
                    ETHERNET_MODE_FLAG = false;        
                    ETHERNET_CONNECTING_FLAG = false;        
                  }
                }

                else if (input[0] == '.' && input[1] == 'c' && input[2] == 'w')
                {
                    Serial.println("Selected Mode: WiFi");
                    MODE_CONFIGURATED = true;
                    ETHERNET_MODE_FLAG = false;
                }

                
                
                else
                {
                    Serial.println("[E] Invalid UART command!");
                }

                input = "";
            }
            else
            {
                input += c;

                if (input.length() > 128)
                {
                    input = "";
                    Serial.println("[E] Input exceeds 128 characters!");
                }
            }
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

















void freertos_init()
{
  lvgl_mutex = xSemaphoreCreateMutex();
  if (lvgl_mutex == NULL)
  {
    Serial.println("lvgl_mutex create failed!");
  }


  mqtt_mutex = xSemaphoreCreateMutex();
  if (mqtt_mutex == NULL)
  {
    Serial.println("MQTT Mutex create failed!");
  }



  xTaskCreatePinnedToCore(taskDisplay, "TaskDisplay", 8192, NULL, 3, &taskDisplayHandle, 1);
  xTaskCreatePinnedToCore(taskProcessing, "TaskProcessing", 8192, NULL, 3, &taskProcessingHandle, 0);
  xTaskCreatePinnedToCore(taskSerialInput,"TaskSerialInput",4096,NULL,1,NULL,0);
  xTaskCreatePinnedToCore(taskReadingSensorGpio,"taskReadingSensorGpio",4096,NULL,2,NULL,0);
  //xTaskCreatePinnedToCore(taskStatusChecking,"taskStatusChecking",2048,NULL,1,NULL,0);
  //xTaskCreatePinnedToCore(taskErrorHandling,"taskErrorHandling",2048,NULL,1,NULL,0);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {

  Serial.begin(115200);

  lvgl_init();
  Serial.println( "[LVGL] TFT DISPLAY setup done!" );

  freertos_init();
  Serial.println( "[FREERTOS] FREERTOS setup done!" );

  gpio_init();
  Serial.println( "[GPIO] GPIO setup done!" );




}
//=============================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() 
{
  vTaskDelete(NULL); // không cần loop mặc định nữa, việc đã có taskS lo
}
//=============================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================



void action_scanned_wifi_label_pressed_1(lv_event_t *e) {
  Serial.println("[LVGL EVENT] LVGL Event Triggered! Wifi label 1 pressed!");
  select_wifi(0);
}
void action_scanned_wifi_label_pressed_2(lv_event_t *e) {
  Serial.println("[LVGL EVENT] LVGL Event Triggered! Wifi label 2 pressed!");
  select_wifi(1);
}
void action_scanned_wifi_label_pressed_3(lv_event_t *e) {
  Serial.println("[LVGL EVENT] LVGL Event Triggered! Wifi label 3 pressed!");
  select_wifi(2);
}
void action_scanned_wifi_label_pressed_4(lv_event_t *e) {
  Serial.println("[LVGL EVENT] LVGL Event Triggered! Wifi label 4 pressed!");
  select_wifi(3);
}
void action_scanned_wifi_label_pressed_5(lv_event_t *e) {
  Serial.println("[LVGL EVENT] LVGL Event Triggered! Wifi label 5 pressed!");
  select_wifi(4);
}
void action_scanned_wifi_label_pressed_6(lv_event_t *e) {
  Serial.println("[LVGL EVENT] LVGL Event Triggered! Wifi label 6 pressed!");
  select_wifi(5);
}
void action_scanned_wifi_label_pressed_7(lv_event_t *e) {
  Serial.println("[LVGL EVENT] LVGL Event Triggered! Wifi label 7 pressed!");
  select_wifi(6);
}
void action_scanned_wifi_label_pressed_8(lv_event_t *e) {
  Serial.println("[LVGL EVENT] LVGL Event Triggered! Wifi label 8 pressed!");
  select_wifi(7);
}
void action_scanned_wifi_label_pressed_9(lv_event_t *e) {
  Serial.println("[LVGL EVENT] LVGL Event Triggered! Wifi label 9 pressed!");
  select_wifi(8);
}
void action_scanned_wifi_label_pressed_10(lv_event_t *e) {
  Serial.println("[LVGL EVENT] LVGL Event Triggered! Wifi label 10 pressed!");
  select_wifi(9);
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





