#include "wifi.h"
#include "../config/config.h"
#include "../config/state.h"

//WiFiClient espClient_Wifi;
WiFiClientSecure espTlsClient_Wifi;

// biến wifi để hiển thị lên màn /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
String wifiSSID[MAX_WIFI_SHOW];
int wifiRSSI[MAX_WIFI_SHOW];
int wifiCount = 0;



// biến nhập pass wifi
String selectedSSID = "";
String wifiPassword = "";
bool WAITING_PASSWORD = false;
//=============================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================






bool connectingWiFi()
{
    WiFi.begin(selectedSSID.c_str(), wifiPassword.c_str());

    const TickType_t startTick = xTaskGetTickCount();
    const TickType_t timeout = pdMS_TO_TICKS(5000);

    while (WiFi.status() != WL_CONNECTED)
    {
        if ((xTaskGetTickCount() - startTick) >= timeout)
        {
            WIFI_CONNECTING_FLAG = false;
            WIFI_STATUS_FLAG = false;
            Serial.println("[WiFi] Reconnect timeout!");
            return false;
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        return true;
    }
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
    if(connectingWiFi())
    {
        Serial.println("[WiFi] Reconnected!");
        Serial.print("[WiFi] IP: ");
        Serial.println(WiFi.localIP());

        //WIFI_CONNECTING_FLAG = false;
        WIFI_STATUS_FLAG = true;

        return true;
    }

    else
    {
        Serial.println("[WiFi] Reconnect failed!");
        //WIFI_CONNECTING_FLAG = false;
        WIFI_STATUS_FLAG = false;

        return false;
    }
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

