#pragma once


extern bool MODE_CONFIGURATED;

extern bool WIFI_SEARCHING_FLAG;
extern bool WIFI_CONNECTING_FLAG;
extern bool WIFI_STATUS_FLAG;
extern bool WIFI_SCAN_DONE;
extern bool WIFI_PASSWORD_ENTER_SCREEN_FLAG;
extern bool WIFI_SHOW_WIFI_LIST;
extern bool WIFI_PASSWORD_NOTIFY;
extern bool WIFI_PASSWORD_ENTERED;

extern bool MQTT_IP_ENTER_SCREEN_FLAG;
extern bool MQTT_IP_CONFIGURATED;
extern bool MQTT_STATUS_FLAG;
extern bool MQTT_CONNECTING_FLAG;
extern bool MQTT_IP_ENTERED;
extern bool MQTT_FIRST_TIME_CONNECTED;

extern bool ETHERNET_MODE_FLAG; 
extern bool ETHERNET_CONNECTING_FLAG;
extern bool ETHERNET_STATUS_FLAG; // biến này k dùng, state ethernet dùng biến EthernetConnectStatus
extern bool ETHERNET_CONNECT_FAILED_NOTIFY;

extern bool DASHBOARD_SCREEN_FLAG;

extern bool MQTT_IP_CONFIGURATED_FIRST_TIME;
extern bool MQTT_PUBLISH_SUCCESS;
extern bool MQTT_SUBSCRIBE_SUCCESS;

extern bool DISCONNECT_ALL;

extern bool WATCHDOG_TRIGGERED;



