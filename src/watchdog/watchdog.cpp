
#include <Arduino.h>
#include "esp_task_wdt.h"
#include "esp_err.h"

void watchdog_init()
{
    // Timeout = 10 giây, panic khi watchdog timeout
    esp_err_t err = esp_task_wdt_init(100, true);

    if (err == ESP_OK)
    {
        Serial.println("[WDT] Initialized successfully");
    }
    else if (err == ESP_ERR_INVALID_STATE)
    {
        Serial.println("[WDT] Already initialized");
    }
    else
    {
        Serial.printf("[WDT] Init failed: %s\n",
                      esp_err_to_name(err));
    }
}
