#pragma once

#define SCK 18
#define MISO 19
#define MOSI 23


#define W5500_CS 5
#define TFT_CS 12
#define TOUCH_CS 22
#define SD_CS 16

// Chân RESET (nếu có nối, không dùng thì để -1)
#define W5500_RST 4
#define TFT_RST 17
#define TOUCH_RST -1
#define SD_RST -1

#define TLS_ENTROPY_PIN 2 // chân cho TLS ethernet
