#pragma once

#include <SPI.h>
#include <Ethernet.h>

#include <Arduino.h>

#include "../freeRTOS_tasks/freeRTOS_tasks.h"

#include <SSLClient.h>

#include "../ca_cert/trust_anchor.h"

// ------------ W5500 
// DHCP_STATUS 0 --> DÙNG IP TĨNH TỰ TẠO 
// DHCP_STATUS 1 --> DÙNG IP GATEWAY CẤP
extern bool DHCP_STATUS;
// Địa chỉ MAC - tự đặt, miễn là không trùng thiết bị khác trong mạng LAN
extern byte mac[];
// Cấu hình IP tĩnh (dùng khi không có DHCP)
extern IPAddress ip;
extern IPAddress dns;
// IP gateway (router)
extern IPAddress gateway;
extern IPAddress subnet;

extern EthernetServer server;  // Mở server ở cổng 8080


extern bool EthernetMode;// 0->wifi, 1->ethernet
extern bool EthernetConnectStatus; // 0->ko kết nối, 1->đã kết nối
extern EthernetClient espClient_Eth;
extern SSLClient espTlsClient_Eth;
//=========================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================






void resetW5500();
void ethernet_DHCP_handling();
void ethernet_checking();
bool ethernet_init();
void ethernet_reconnect();
void ethernet_maintain();