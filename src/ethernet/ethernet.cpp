#include "ethernet.h"

#include "../config/config.h"
#include "../mqtt/mqtt.h"



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

EthernetClient espClient_Eth;
SSLClient      espTlsClient_Eth(espClient_Eth, TAs, (size_t)TAs_NUM, TLS_ENTROPY_PIN, 1, SSLClient::SSL_INFO);
//=========================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================


void resetW5500() {
  if (W5500_RST >= 0) {
    pinMode(W5500_RST, OUTPUT);
    digitalWrite(W5500_RST, LOW);
    delay(50);
    digitalWrite(W5500_RST, HIGH);
    delay(200);
  }
}

void ethernet_DHCP_handling()
{
  switch (DHCP_STATUS){

    case 1:
    {
      Serial.print("[ETHERNET] Connecting to Ethernet... Please wait...");

      unsigned long DHCP_TIMEOUT = 5000;
      unsigned long DHCP_RESPONSE_TIMEOUT = 500;

      int ethernet_begin = Ethernet.begin(
          mac,
          DHCP_TIMEOUT,
          DHCP_RESPONSE_TIMEOUT
      );


      if (ethernet_begin != 0)
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

      // code
      mqttClient.setClient(espTlsClient_Eth);
  
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