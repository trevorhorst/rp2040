#ifndef RP2040_APPLICATION_H
#define RP2040_APPLICATION_H

#include <stdio.h>

#include "pico/stdlib.h"

#include "rp2040/drivers/ws2812.h"
#include "rp2040/drivers/ssd1306.h"

class Application
{
    static const uint32_t neopixel_pin;
    static const uint32_t neopixel_num_leds;
    static const uint32_t neopixel_max_brightness;
    static const uint32_t i2c1_sda_pin;
    static const uint32_t i2c1_scl_pin;
    static const uint32_t heartbeat_ms;

public:
    Application();

    int32_t run();
    void initialize();
    void initializeI2C();
    void initializeDisplay();

    bool reserved_addr(uint8_t addr)
    {
        return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
    }

    void i2cBusScan(i2c_inst_t *bus);

private:
    i2c_inst_t *mI2C1;
    WS2812 mNeopixel;
    SSD1306 mDisplay;
};

#endif // RP2040_APPLICATION_H