#ifndef RP2040_APPLICATION_H
#define RP2040_APPLICATION_H

#include <stdio.h>

#include "pico/stdlib.h"

#include "rp2040/drivers/ws2812.h"

class Application
{
    static const uint32_t neopixel_pin;
    static const uint32_t neopixel_num_leds;
    static const uint32_t neopixel_max_brightness;
    static const uint32_t heartbeat_ms;

public:
    Application();

    int32_t run();
    void initialize();

private:
    WS2812 mNeopixel;
};

#endif // RP2040_APPLICATION_H