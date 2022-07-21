#ifndef RP2040_APPLICATION_H
#define RP2040_APPLICATION_H

#include <stdio.h>
#include <time.h>

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/mutex.h"
#include "hardware/gpio.h"

#include "rp2040/control/control_game.h"

#include "rp2040/command/command_handler.h"
#include "rp2040/command/command_help.h"
#include "rp2040/command/command_i2c.h"
#include "rp2040/command/command_pixel.h"

#include "rp2040/console.h"
#include "rp2040/drivers/ws2812.h"
#include "rp2040/drivers/ssd1306.h"
// #include "rp2040/font/font.h"
#include "rp2040/logger.h"

class Application
{
    static const uint32_t neopixel_pin;
    static const uint32_t neopixel_num_leds;
    static const uint32_t neopixel_max_brightness;
    static const uint32_t i2c1_sda_pin;
    static const uint32_t i2c1_scl_pin;
    static const uint32_t heartbeat_ms;

    static const uint8_t ssd1306_display_addr;

public:
    Application();

    int32_t run();
    void initialize();
    void initializeI2C();
    void initializeDisplay();
    void initializeConsole();

    bool reserved_addr(uint8_t addr)
    {
        return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
    }

    void i2cBusScan(i2c_inst_t *bus);

    void printRamBoard(SSD1306::DisplayRam &ram);
    void checkRamBoard(SSD1306::DisplayRam &ram, SSD1306::DisplayRam &newRam, bool debug = false);
    void checkRamBoardNew(SSD1306::DisplayRam &ram, SSD1306::DisplayRam &newRam, bool debug = false);

private:
    mutex_t mMutex;
    i2c_inst_t *mI2C1;
    WS2812 mNeopixel;
    SSD1306 mDisplay;

    CommandHandler mHandler;
    CommandHelp mCmdHelp;
    CommandPixel mCmdPixel;
    CommandI2CDetect mCmdI2CDetect;

};

#endif // RP2040_APPLICATION_H
