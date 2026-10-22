#include "get_time.h"


// ================= NTP =================
const char* NTP_SERVER = "time.google.com";

// Việt Nam: UTC+7
const long GMT_OFFSET_SEC = 7 * 3600;

//const long GMT_OFFSET_SEC = 0;
const int DAYLIGHT_OFFSET_SEC = 0;



// ----- Cấu hình NTP ----- ethernet
#define NTP_SERVER      "pool.ntp.org"
#define TZ_VIETNAM      "ICT-7"     // UTC+7, không có DST
#define NTP_MIN_VALID   1700000000  // ~ 11/2023

EthernetUDP ntpUDP;
NTPClient   timeClient(ntpUDP, NTP_SERVER, 0, 60000);  // offset 0 -> lấy UTC






tm timeinfo;





// --------------------------------------------------------- ethernet
bool get_time_init_e()
{
    Serial.println("[NTP] Initializing time synchronization...");
    timeClient.begin();

    bool ok = false;
    for (int i = 1; i <= 5; i++) {
        Serial.printf("[NTP] Attempt %d/5...\n", i);
        if (timeClient.forceUpdate()) {
            ok = true;
            break;
        }
        delay(1000);
    }
    if (!ok) {
        Serial.println("[NTP] Timeout! Kiem tra DNS / firewall UDP 123");
        return false;
    }

    // Đưa epoch (UTC) vào đồng hồ hệ thống của ESP32
    time_t epoch = (time_t)timeClient.getEpochTime();
    if (epoch < NTP_MIN_VALID) {
        Serial.println("[NTP] Epoch khong hop le");
        return false;
    }

    struct timeval tv = { .tv_sec = epoch, .tv_usec = 0 };
    settimeofday(&tv, nullptr);

    // Đặt múi giờ để localtime()/getLocalTime() ra giờ Việt Nam
    setenv("TZ", TZ_VIETNAM, 1);
    tzset();

    //struct tm timeinfo;
    if (!getLocalTime(&timeinfo, 2000)) {
        Serial.println("[NTP] getLocalTime failed");
        return false;
    }

    Serial.println("[NTP] Time synchronized successfully!");
    return true;
}



// wifi
bool get_time_init()
{
    Serial.println("[NTP SERVER TIME] Initializing time synchronization...");
    // Đồng bộ thời gian từ NTP
    configTime(
        GMT_OFFSET_SEC,
        DAYLIGHT_OFFSET_SEC,
        NTP_SERVER,
        "asia.pool.ntp.org"
    );

    Serial.print("Syncing time");
    time_t now = time(nullptr);
    while (now < 1700000000) {   // ~ tháng 11/2023, tức là đã sync
        delay(200);
        Serial.print(".");
        now = time(nullptr);
    }
    Serial.println();
    Serial.println(ctime(&now));

    while (!getLocalTime(&timeinfo))
    {
        Serial.println("[NTP SERVER TIME] Waiting for time synchronization...");
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    Serial.println();
    Serial.println("[NTP SERVER TIME]Time from NTP Server synchronized successfully!");
    return true;
}















void get_time_HHMMSS()
{

    if (getLocalTime(&timeinfo))
    {
        Serial.printf(
            "%02d:%02d:%02d\n",

            timeinfo.tm_hour,
            timeinfo.tm_min,
            timeinfo.tm_sec
        );
    }

    else
    {
        Serial.println("Failed to obtain time from NTP server!");
        return;
    }
}



String get_time_yyyymmdd_hhmmss() {

    if (!getLocalTime(&timeinfo)) return "";

    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
    Serial.println(buffer);
    return String(buffer);

}