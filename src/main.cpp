#include <Arduino.h>
#include "config/config.h"
#include "ethernet/ethernet.h"
#include "freeRTOS_tasks/freeRTOS_tasks.h"
#include "gpio/gpio.h"
#include "LVGL_UI/LVGL_UI.h"
#include "mqtt/mqtt.h"
#include "sensor_handler/sensor_handler.h"
#include "wifi/wifi.h"
#include "SD_card/SD_card.h"
#include "watchdog/watchdog.h"
#include "ca_cert/ca_cert.h"

// biến flag để khiển //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MODE_CONFIGURATED = false;

bool WIFI_SEARCHING_FLAG = true;
bool WIFI_CONNECTING_FLAG = false;
bool WIFI_STATUS_FLAG = false;
bool WIFI_SCAN_DONE = false;
bool WIFI_PASSWORD_ENTER_SCREEN_FLAG = false;
bool WIFI_SHOW_WIFI_LIST = false;
bool WIFI_PASSWORD_NOTIFY = false;
bool WIFI_PASSWORD_ENTERED = false;

bool MQTT_IP_ENTER_SCREEN_FLAG = false;
bool MQTT_IP_CONFIGURATED = false;
bool MQTT_STATUS_FLAG = false;
bool MQTT_CONNECTING_FLAG = false;
bool MQTT_IP_CONFIGURATED_FIRST_TIME = true;
bool MQTT_PUBLISH_SUCCESS = false;
bool MQTT_SUBSCRIBE_SUCCESS = false;
bool MQTT_IP_ENTERED = false;
bool MQTT_FIRST_TIME_CONNECTED = false;

bool ETHERNET_MODE_FLAG = false;
bool ETHERNET_CONNECTING_FLAG = false;
bool ETHERNET_STATUS_FLAG = false;
bool ETHERNET_CONNECT_FAILED_NOTIFY = false;

bool DASHBOARD_SCREEN_FLAG = false;

bool DISCONNECT_ALL = false;

bool WATCHDOG_TRIGGERED = false;







void setup() {
  Serial.begin(115200);







  
  //pinMode(SD_CS, OUTPUT);
  gpio_init();
  Serial.println( "[GPIO] GPIO setup done!" );

  delay(100);

  

  

  sdSPI_init();

  sdSPI_test();

  delay(100);




  watchdog_init();

  delay(100);

  

  lvgl_init();
  Serial.println( "[LVGL] TFT DISPLAY setup done!" );
  delay(100);


  freertos_init();
  Serial.println( "[FREERTOS] FREERTOS setup done!" );
  delay(100);

  








}







void loop() 
{

  vTaskDelete(NULL); // không cần loop mặc định nữa, việc đã có taskS lo

}
//=============================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================

