#pragma once
#include <Arduino.h>
#include "../ethernet/ethernet.h"
#include "../wifi/wifi.h"
#include <time.h>
#include <NTPClient.h>
#include <sys/time.h>
#include <EthernetUdp.h>


// ================= NTP =================
extern const char* NTP_SERVER;

// Việt Nam: UTC+7
extern const long GMT_OFFSET_SEC;
extern const int DAYLIGHT_OFFSET_SEC;
extern struct tm timeinfo;

bool get_time_init_e();
bool get_time_init();
void get_time_HHMMSS();
String get_time_yyyymmdd_hhmmss();
