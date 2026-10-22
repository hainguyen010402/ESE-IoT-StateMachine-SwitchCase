#include "gpio.h"
#include "../config/config.h"



// thêm IO thì chỉ cần thêm ở đây
GpioConfig esp32_gpio_config[] =
{
    {GPIO_PIN1, true, "gpio_key1", 1},
    {GPIO_PIN2, true, "gpio_key2", 2},
};
//=============================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================

void gpio_init()
{
    pinMode(GPIO_PIN1, OUTPUT);
    digitalWrite(GPIO_PIN1, LOW);

    pinMode(GPIO_PIN2, OUTPUT);
    digitalWrite(GPIO_PIN2, HIGH);
}

bool gpio_change_check(uint8_t pin, uint32_t debounceMs)
{
    static uint8_t  stableState[GPIO_NUM_MAX];
    static uint8_t  lastRead[GPIO_NUM_MAX];
    static uint32_t lastChangeTime[GPIO_NUM_MAX];
    static bool     initialized[GPIO_NUM_MAX] = {false};

    if (pin >= GPIO_NUM_MAX) return false;

    uint8_t  reading = digitalRead(pin);
    uint32_t now     = millis();

    if (!initialized[pin])
    {
        stableState[pin]    = reading;
        lastRead[pin]       = reading;
        lastChangeTime[pin] = now;
        initialized[pin]    = true;
        return false;
    }

    // Đọc khác lần trước -> đang nhiễu/bounce, reset đồng hồ
    if (reading != lastRead[pin])
    {
        lastRead[pin]       = reading;
        lastChangeTime[pin] = now;
    }

    // Giữ ổn định đủ lâu và khác trạng thái đã chốt -> thay đổi thật
    if ((now - lastChangeTime[pin]) >= debounceMs && reading != stableState[pin])
    {
        stableState[pin] = reading;
        return true;
    }

    return false;
}