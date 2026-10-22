#include "freeRTOS_tasks.h"

#include "../config/config.h"

#include "../config/state.h"

#include "../sensor_handler/sensor_handler.h"

#include "../gpio/gpio.h"

#include "../wifi/wifi.h"

#include "../ethernet/ethernet.h"

#include "../mqtt/mqtt.h"

#include "../LVGL_UI/LVGL_UI.h"

#include  "../get_time/get_time.h"

#include "../watchdog/watchdog.h"

#include "../ca_cert/ca_cert.h"




SemaphoreHandle_t lvgl_mutex;
SemaphoreHandle_t mqtt_mutex;
SemaphoreHandle_t spi_mutex;




TaskHandle_t taskDisplayHandle = NULL;
TaskHandle_t taskProcessingHandle = NULL;
TaskHandle_t taskSerialHandle = NULL;
TaskHandle_t taskReadingSensorGpioHandle = NULL;




TickType_t lastWakeTime;



uint32_t lastPublish_info = 0;
uint32_t lastPublish_measurement = 0;
uint32_t lastPublish_relay = 0;


processingState PROCESSING_STATE = MODE_CONFIGURATION;







void taskReadingSensorGpio(void *pvParameter)
{
  esp_err_t err = esp_task_wdt_add(NULL);

  if (err != ESP_OK)
  {
      Serial.printf("[WDT] taskReadingSensorGpio add failed: %s\n",
                    esp_err_to_name(err));
  }
  bool wdt_enabled = (err == ESP_OK);

  if (!wdt_enabled)
  {
      Serial.printf("[WDT] taskReadingSensorGpio: %s\n",
                    esp_err_to_name(err));
  }
  for (;;)
  {
    uint32_t now = millis();
    if (now - lastSensorRead >= 1000)
    {
      lastSensorRead = now;
      temp = random(150,350)/10.0;
      hum = random(600,800)/10.0;
    }
    

    // Sensor 1
    snprintf(text1, sizeof(text1), "%d", int(temp));

    // Sensor 2
    snprintf(text2, sizeof(text2), "%d", int(hum));

    esp32_gpio_config[0].state = digitalRead(GPIO_PIN1);
    
    esp32_gpio_config[1].state = digitalRead(GPIO_PIN2);

    if (wdt_enabled)
    {
      esp_task_wdt_reset();
    }
            

    vTaskDelay(pdMS_TO_TICKS(10));
  }
}









void modeConfigurating()
{
  vTaskDelay(pdMS_TO_TICKS(1000));
  
  Serial.println("Mode Configurating Task");

  if (!MODE_CONFIGURATED) return;

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
  
}









void connectingEthernet()
{
  if (ETHERNET_CONNECT_FAILED_NOTIFY) return;

  else
  {
    ETHERNET_CONNECTING_FLAG = true;

    Serial.println("Connecting Ethernet Task");

    if (ethernet_init())
    {
      ETHERNET_CONNECTING_FLAG = false;

      ETHERNET_CONNECT_FAILED_NOTIFY = false;

      get_time_init_e();
      
      PROCESSING_STATE = MQTT;
    }

    else
    {
      ETHERNET_CONNECTING_FLAG = false;

      ETHERNET_CONNECT_FAILED_NOTIFY = true;

      PROCESSING_STATE = ETHERNET;
    }

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

  else if (WIFI_PASSWORD_ENTERED)
  {
    WIFI_PASSWORD_ENTERED = false;
    // biến wifiPassword để lưu mật khẩu wifi


    if (xSemaphoreTake(lvgl_mutex, portMAX_DELAY) == pdTRUE)
    {
      wifiPassword = lv_textarea_get_text(objects.ta);
      xSemaphoreGive(lvgl_mutex);
    }


    
    WAITING_PASSWORD = false;
    WIFI_PASSWORD_ENTER_SCREEN_FLAG = false;

    Serial.println("Connecting to: " + selectedSSID);

    WIFI_CONNECTING_FLAG = true;

    connectingWiFi();

    WIFI_CONNECTING_FLAG = false;

    if (WiFi.status() == WL_CONNECTED)
    {
        WIFI_PASSWORD_NOTIFY = false;
        WIFI_SHOW_WIFI_LIST = false;
        WIFI_STATUS_FLAG = true;
        DASHBOARD_SCREEN_FLAG = true;
        
        Serial.println("WIFI CONNECTED!");
        Serial.print("IP: ");
        Serial.println(WiFi.localIP());
        get_time_init();
        //Serial.println(PROCESSING_STATE);

        MQTT_CONNECTING_FLAG = true;  
        
        PROCESSING_STATE = MQTT;

    }
    else
    {
        //WIFI_SHOW_WIFI_LIST = true;
        WIFI_PASSWORD_NOTIFY = true;
        WIFI_STATUS_FLAG = false;
        //DASHBOARD_SCREEN_FLAG = false;
        //PROCESSING_STATE = WIFI;
        //Serial.println("WIFI CONNECT FAILED!");
        //Serial.println("Re-enter 'w1' to 'w10' to retry.");
        
    }
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
    // nhập IP MQTT với biến flag MQTT_IP_CONFIGURATED
    if (MQTT_IP_ENTERED)
    {
      MQTT_IP_ENTERED = false;

      if (xSemaphoreTake(lvgl_mutex, portMAX_DELAY) == pdTRUE)
      {
        mqtt_server = lv_textarea_get_text(objects.ta);
        xSemaphoreGive(lvgl_mutex);
      }

      // biến mqtt_server để cấu hình ip mqtt broker
      
      MQTT_IP_CONFIGURATED = true;


      Serial.printf("IP set: [%s], len=%d\n",
      mqtt_server.c_str(),
      mqtt_server.length());

      Serial.printf("state=%d, connecting=%d\n",
            PROCESSING_STATE,
            MQTT_CONNECTING_FLAG);

      Serial.println("MQTT Server IP set to: " + mqtt_server);
    }
  }

  else
  {

      // code
      if (!ETHERNET_MODE_FLAG) //mode wifi
      {
        Serial.println("[Mode: WiFi] Setting WiFi Client as the Client for Server...");
        mqttClient.setClient(espTlsClient_Wifi);
        espTlsClient_Wifi.setCACert(MQTT_CA_CERT);
        //espTlsClient_Wifi.setInsecure();
        espTlsClient_Wifi.setHandshakeTimeout(5);   // đơn vị: GIÂY (không phải ms)
        // 1) TCP connect + đọc/ghi socket (WiFiClientSecure)
        espTlsClient_Wifi.setTimeout(5);            // giây (xem lưu ý bên dưới)
      }
      else //mode ethernet
      {
        Serial.println("[Mode: Ethernet] Setting Ethernet Client as the Client for Server...");
        mqttClient.setClient(espTlsClient_Eth);
        espTlsClient_Eth.setTimeout(5000);
      }

      

      Serial.println("Client setting Server...");

      time_t t = time(nullptr);
      Serial.printf("epoch=%ld  %s", (long)t, ctime(&t));

      

      




      




      mqttClient.setServer(mqtt_server.c_str(), 8883);
      mqttClient.setCallback(mqtt_callback);

      mqttClient.setBufferSize(1024);
      mqttClient.setKeepAlive(30);
      // Chờ CONNACK từ broker và các lần đọc của PubSubClient
      mqttClient.setSocketTimeout(5);              // giây

      
      Serial.println("Client connecting to Server...");

      Serial.printf("Free=%u largest=%u\n", ESP.getFreeHeap(),
                heap_caps_get_largest_free_block(MALLOC_CAP_8BIT));

      mqtt_connect();
      

  }


  if (DISCONNECT_ALL)
  {
    DISCONNECT_ALL = false;

    MQTT_IP_CONFIGURATED_FIRST_TIME = true;
    WIFI_STATUS_FLAG = false;
    MQTT_STATUS_FLAG = false;
    ETHERNET_STATUS_FLAG = false;
    DASHBOARD_SCREEN_FLAG = false;
    MODE_CONFIGURATED = false;
    ETHERNET_MODE_FLAG = false;
    EthernetConnectStatus = false;
    MQTT_IP_CONFIGURATED = false;
    WIFI_SCAN_DONE = false;
    
    Serial.println("Disconnected!");
    WiFi.disconnect();
    resetW5500();

    if (xSemaphoreTake(mqtt_mutex, pdMS_TO_TICKS(5000)) == pdTRUE)
    {
      // code
    
      mqttClient.disconnect();
      xSemaphoreGive(mqtt_mutex);
    }

    

    PROCESSING_STATE = MODE_CONFIGURATION;
  }

  vTaskDelay(pdMS_TO_TICKS(100));

}













void mqttTelemetry()
{
  //Serial.println("Telemetry Task");

    // code

    if (!ETHERNET_MODE_FLAG)
    {
        // WiFi reconnect
      if (WiFi.status() != WL_CONNECTED)
      {
          if (selectedSSID.length() > 0)
              reconnectWiFi();

          vTaskDelay(pdMS_TO_TICKS(2000));
          return;
      }
    }



    else
    {
        // Ethernet reconnect
      if (!EthernetConnectStatus)
      {
          ethernet_reconnect();
          vTaskDelay(pdMS_TO_TICKS(2000));
          return;
      }
    }



    // MQTT reconnect
    if (!mqttClient.connected() || !MQTT_SUBSCRIBE_SUCCESS)
    {
        Serial.println("MQTT RECONNECTING...");
        MQTT_STATUS_FLAG = false;
        MQTT_CONNECTING_FLAG = true;

        mqtt_connect();

        vTaskDelay(pdMS_TO_TICKS(2000));
        return;
    }



    MQTT_STATUS_FLAG = true;
    MQTT_CONNECTING_FLAG = false;

    // Xử lý MQTT liên tục
    mqttClient.loop();

    
    

    uint32_t now_info = millis();

    if (now_info - lastPublish_info >= 60000)
    {
      /*
      buildJsonPayload_then_mqttPublish(doc);                                // cả 3
      buildJsonPayload_then_mqttPublish(doc, TELE_RELAY);                    // chỉ relay (vd: khi relay đổi trạng thái)
      buildJsonPayload_then_mqttPublish(doc, TELE_MEASUREMENT | TELE_INFO);  // 2 cái
      */

      lastPublish_info = now_info;

      if (!buildJsonPayload_then_mqttPublish(doc, TELE_INFO))
      {
        MQTT_PUBLISH_SUCCESS = false;
      }

      else MQTT_PUBLISH_SUCCESS = true;
      

    }

    uint32_t now_measurement = millis();

    if (now_measurement - lastPublish_measurement >= 2000)
    {
      /*
      buildJsonPayload_then_mqttPublish(doc);                                // cả 3
      buildJsonPayload_then_mqttPublish(doc, TELE_RELAY);                    // chỉ relay (vd: khi relay đổi trạng thái)
      buildJsonPayload_then_mqttPublish(doc, TELE_MEASUREMENT | TELE_INFO);  // 2 cái
      */

      lastPublish_measurement = now_measurement;

      if (!buildJsonPayload_then_mqttPublish(doc, TELE_MEASUREMENT))
      {
        MQTT_PUBLISH_SUCCESS = false;
      }

      else MQTT_PUBLISH_SUCCESS = true;
      

    }

    /*
    uint32_t now_relay = millis();

    if (now_relay - lastPublish_relay >= 5000)
    {

      lastPublish_relay = now_relay;

      if (!buildJsonPayload_then_mqttPublish(doc, TELE_RELAY))
      {
        MQTT_PUBLISH_SUCCESS = false;
      }

      else MQTT_PUBLISH_SUCCESS = true;
      
    } 
      */
    

    if (MQTT_FIRST_TIME_CONNECTED
        || gpio_change_check(esp32_gpio_config[0].gpio, 20) 
        || gpio_change_check(esp32_gpio_config[1].gpio, 20))
    {
      MQTT_FIRST_TIME_CONNECTED = false;

      vTaskDelay(pdMS_TO_TICKS(10));

      if (!buildJsonPayload_then_mqttPublish(doc, TELE_RELAY))
      {
        MQTT_PUBLISH_SUCCESS = false;
      }

      else MQTT_PUBLISH_SUCCESS = true;
    }


    
    

    if (DISCONNECT_ALL)
    {
      DISCONNECT_ALL = false;

      MQTT_IP_CONFIGURATED_FIRST_TIME = true;
      WIFI_STATUS_FLAG = false;
      MQTT_STATUS_FLAG = false;
      ETHERNET_STATUS_FLAG = false;
      DASHBOARD_SCREEN_FLAG = false;
      MODE_CONFIGURATED = false;
      ETHERNET_MODE_FLAG = false;
      EthernetConnectStatus = false;
      MQTT_IP_CONFIGURATED = false;
      WIFI_SCAN_DONE = false;
      
      Serial.println("Disconnected!");
      WiFi.disconnect();
      resetW5500();
      mqttClient.disconnect();

      WiFi.disconnect(true);
      WiFi.mode(WIFI_OFF);

      PROCESSING_STATE = MODE_CONFIGURATION;
    }
  
    

}















void taskProcessing(void *pvParameters) 
{

  esp_err_t err = esp_task_wdt_add(NULL);

  if (err != ESP_OK)
  {
      Serial.printf("[WDT] taskProcessing add failed: %s\n",
                    esp_err_to_name(err));
  }

  bool wdt_enabled = (err == ESP_OK);

  if (!wdt_enabled)
  {
      Serial.printf("[WDT] taskProcessing: %s\n",
                    esp_err_to_name(err));
  }
  
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

    if (wdt_enabled)
    {
      esp_task_wdt_reset();
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
  //Serial.println("da vao taskdisplay");    

  esp_err_t err = esp_task_wdt_add(NULL);

  if (err != ESP_OK)
  {
      Serial.printf("[WDT] taskDisplay add failed: %s\n",
                    esp_err_to_name(err));
  }    

  bool wdt_enabled = (err == ESP_OK);

  if (!wdt_enabled)
  {
      Serial.printf("[WDT] taskDisplay: %s\n",
                    esp_err_to_name(err));
  }

  for (;;) 
  {
    //Serial.println("da vao vong lap for taskdisplay");

    if (xSemaphoreTake(lvgl_mutex, pdMS_TO_TICKS(5000)) == pdTRUE) 
    {
      //Serial.println("da vao lvgl mutex");
      
        //Serial.println("da vao spi mutex");
      ui_display_load_screen();
      ui_display();
      
        if (xSemaphoreTake(spi_mutex, pdMS_TO_TICKS(5000)) == pdTRUE)
        {
          lv_timer_handler();
          ui_tick();
          //Serial.println("da chay ham ui_display");
        xSemaphoreGive(spi_mutex);
        }
       //else
      //{
      //  Serial.println("[DISP] spi_mutex timeout");
      //}
      xSemaphoreGive(lvgl_mutex);
    }
      
      
        //Serial.println("da chay ham lv_timer_hndler");
        //tft.dmaWait();   
        //Serial.println("da chay ham tft.dmaWait");              

     
      


    else
    {
      Serial.println("[DISP] lvgl_mutex timeout");
    }

    if (wdt_enabled)
    {
      esp_task_wdt_reset();
    }

    vTaskDelay(pdMS_TO_TICKS(5));
  }
}





void taskDisplayProcessing(void *pvParameters) 
{       
  //Serial.println("da vao taskdisplay");    

  esp_err_t err = esp_task_wdt_add(NULL);

  if (err != ESP_OK)
  {
      Serial.printf("[WDT] taskDisplayProcessing add failed: %s\n",
                    esp_err_to_name(err));
  }    

  bool wdt_enabled = (err == ESP_OK);

  if (!wdt_enabled)
  {
      Serial.printf("[WDT] taskDisplayProcessing: %s\n",
                    esp_err_to_name(err));
  }

  for (;;) 
  {
    
    if (xSemaphoreTake(lvgl_mutex, pdMS_TO_TICKS(500)) == pdTRUE)
    {
      
      xSemaphoreGive(lvgl_mutex);
    }
    else Serial.println("[LVGL] lvgl_mutex timeout");
     

    if (wdt_enabled)
    {
      esp_task_wdt_reset();
    }

    vTaskDelay(pdMS_TO_TICKS(5));
  }
}












/*
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

                // Nhập mật khẩu wifi với biến flag WAITING_PASSWORD
                if (WAITING_PASSWORD)
                {
                    // biến wifiPassword để lưu mật khẩu wifi
                    wifiPassword = input;
                    WAITING_PASSWORD = false;
                    WIFI_PASSWORD_ENTER_SCREEN_FLAG = false;

                    Serial.println("Connecting to: " + selectedSSID);

                    WIFI_CONNECTING_FLAG = true;

                    connectingWiFi();

                    WIFI_CONNECTING_FLAG = false;

                    if (WiFi.status() == WL_CONNECTED)
                    {
                        WIFI_PASSWORD_NOTIFY = false;
                        WIFI_SHOW_WIFI_LIST = false;
                        WIFI_STATUS_FLAG = true;
                        DASHBOARD_SCREEN_FLAG = true;
                        PROCESSING_STATE = MQTT;
                        Serial.println("WIFI CONNECTED!");
                        Serial.print("IP: ");
                        Serial.println(WiFi.localIP());
                        get_time_init();
                        //Serial.println(PROCESSING_STATE);

                        MQTT_CONNECTING_FLAG = true;        

                    }
                    else
                    {
                        //WIFI_SHOW_WIFI_LIST = true;
                        WIFI_PASSWORD_NOTIFY = true;
                        WIFI_STATUS_FLAG = false;
                        //DASHBOARD_SCREEN_FLAG = false;
                        //PROCESSING_STATE = WIFI;
                        //Serial.println("WIFI CONNECT FAILED!");
                        //Serial.println("Re-enter 'w1' to 'w10' to retry.");
                        
                    }
                }


                // nhập IP MQTT với biến flag MQTT_IP_CONFIGURATED
                else if (!MQTT_IP_CONFIGURATED && (PROCESSING_STATE == MQTT))
                {
                  // biến mqtt_server để cấu hình ip mqtt broker
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
                  
                  MQTT_IP_CONFIGURATED_FIRST_TIME = true;
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
                  mqttClient.disconnect();

                }
                else if (input[0] == '.' && input[1] == 'w' && input[2] == 'd')
                {

                  WiFi.disconnect();
                  Serial.println("Wifi Disconnected!");
                  
                }
                else if (input[0] == '.' && input[1] == 'm' && input[2] == 'd')
                {

                  mqttClient.disconnect();
                  Serial.println("MQTT Disconnected!");
                  
                }

                // '.d' scroll panel chứa label wifi xuống 
                else if (input[0] == '.' && input[1] == 'd')
                {
                  
                  Serial.println("Scrolling down!");
                  if (xSemaphoreTake(lvgl_mutex, portMAX_DELAY) == pdTRUE)
                  {
                    lv_obj_scroll_by(objects.wifi_list_screen_container, 0, -50, LV_ANIM_ON);
                    xSemaphoreGive(lvgl_mutex);
                  }
                }
                // '.u' scroll panel chứa label wifi lên
                else if (input[0] == '.' && input[1] == 'u')
                {
                  
                  Serial.println("Scrolling up!");
                  if (xSemaphoreTake(lvgl_mutex, portMAX_DELAY) == pdTRUE)
                  {
                    lv_obj_scroll_by(objects.wifi_list_screen_container, 0, 50, LV_ANIM_ON);
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

                else if (input[0] == '.' && input[1] == 'c')
                {
                  Serial.println("Confirm Button pressed!");
                  if (xSemaphoreTake(lvgl_mutex, portMAX_DELAY) == pdTRUE)
                  {
                    lv_obj_send_event(objects.confirm_button, LV_EVENT_PRESSED, NULL);
                    xSemaphoreGive(lvgl_mutex);
                    
                  }
                }
                
                // 't'
                else if (input[0] == 't')
                {
                  Serial.println("Test!");
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
*/


  









void taskSerial(void *pvParameters) 
{

  esp_err_t err = esp_task_wdt_add(NULL);

  if (err != ESP_OK)
  {
      Serial.printf("[WDT] taskSerial add failed: %s\n",
                    esp_err_to_name(err));
  }

  bool wdt_enabled = (err == ESP_OK);

  if (!wdt_enabled)
  {
      Serial.printf("[WDT] taskSerial: %s\n",
                    esp_err_to_name(err));
  }
  
  for (;;)
  {

    
    Serial.printf("Free heap: %u\n", ESP.getFreeHeap());

    Serial.printf("Display stack watermark: %u bytes\n",
    uxTaskGetStackHighWaterMark(taskDisplayHandle));

    Serial.printf("Processing stack watermark: %u bytes\n",
    uxTaskGetStackHighWaterMark(taskProcessingHandle));

    Serial.printf("Serial stack watermark: %u bytes\n",
    uxTaskGetStackHighWaterMark(taskSerialHandle));

    Serial.printf("Reading sensor and GPIO stack watermark: %u bytes\n",
    uxTaskGetStackHighWaterMark(taskReadingSensorGpioHandle));


    
    /*

    Serial.printf("Largest block: %u\n",
        heap_caps_get_largest_free_block(MALLOC_CAP_8BIT));

    Serial.printf("Minimum free heap: %u\n",
        ESP.getMinFreeHeap());
    */

    if (wdt_enabled)
    {
      esp_task_wdt_reset();
    }

    vTaskDelay(pdMS_TO_TICKS(1000));
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

  spi_mutex = xSemaphoreCreateMutex();
   if (spi_mutex == NULL)
  {
    Serial.println("SPI Mutex create failed!");
  }


  //xTaskCreatePinnedToCore(taskDisplayProcessing, "TaskDisplayProcessing", 8192, NULL, 3, &taskDisplayHandle, 1);
  xTaskCreatePinnedToCore(taskDisplay, "TaskDisplay", 5500, NULL, 2, &taskDisplayHandle, 1);
  xTaskCreatePinnedToCore(taskProcessing, "TaskProcessing", 6144, NULL, 3, &taskProcessingHandle, 0);
  xTaskCreatePinnedToCore(taskSerial,"taskSerial",2048,NULL,1,&taskSerialHandle,0);
  xTaskCreatePinnedToCore(taskReadingSensorGpio,"taskReadingSensorGpio",2048,NULL,2,&taskReadingSensorGpioHandle,0);
  //xTaskCreatePinnedToCore(taskStatusChecking,"taskStatusChecking",2048,NULL,1,NULL,0);
  //xTaskCreatePinnedToCore(taskErrorHandling,"taskErrorHandling",2048,NULL,1,NULL,0);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////









