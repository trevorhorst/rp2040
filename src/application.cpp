#include "rp2040/application.h"

const uint32_t Application::neopixel_pin = 16;
const uint32_t Application::neopixel_num_leds = 1;
const uint32_t Application::neopixel_max_brightness = 25;

const uint32_t Application::i2c1_sda_pin  = 26;
const uint32_t Application::i2c1_scl_pin  = 27;

const uint32_t Application::heartbeat_ms = 1000;

const uint8_t Application::ssd1306_display_addr = 0x3D;

#define I2C_BUS_SPEED_KHZ(x) x * 1000

/**
 * @brief Construct a new Application:: Application object
 * 
 */
Application::Application()
    : mI2C1(i2c1)
    , mNeopixel(
        neopixel_pin,
        neopixel_num_leds,
        pio0,
        0,
        WS2812::DataFormat::FORMAT_GRB
    )
    , mDisplay(mI2C1, ssd1306_display_addr)
{
    initialize();
}

/**
 * @brief Main application thread
 * 
 * @return int32_t 
 */
int32_t Application::run()
{
    int32_t error = 0;

    int count = 0;
    int incrementer = 0;
    while(true) {
        count += incrementer;
        if(count == 0) {
            incrementer = 1;
        } else if(count == neopixel_max_brightness) {
            incrementer = -1;
        }

        mNeopixel.fill(WS2812::RGB(0, count, 0));
        mNeopixel.show();
        sleep_ms(heartbeat_ms / neopixel_max_brightness);
    }

    return error;
}

/**
 * @brief Initialize various hardware to defaults
 * 
 */
void Application::initialize()
{
    log_set_level(LOG_TRACE);

    mNeopixel.fill(WS2812::RGB(1, 0, 0));
    mNeopixel.show();
    sleep_ms(1000);

    LOG_TRACE("Booting...\n");
    initializeI2C();
    initializeDisplay();
    initializeConsole();

}

void Application::initializeI2C()
{
    LOG_TRACE("Initializing I2C...\n");
    i2c_init(mI2C1, I2C_BUS_SPEED_KHZ(1000));
    gpio_set_function(i2c1_sda_pin, GPIO_FUNC_I2C);
    gpio_set_function(i2c1_scl_pin, GPIO_FUNC_I2C);
    gpio_pull_up(i2c1_sda_pin);
    gpio_pull_up(i2c1_scl_pin);

    i2cBusScan(mI2C1);
}

void Application::initializeDisplay()
{
    LOG_TRACE("Initializing display...\n");
    LOG_TRACE("Display @ 0x%02X\n", ssd1306_display_addr);
    mDisplay.initialize();

    for(int i = 0; i < 3; i++) {
        mDisplay.fill_screen(0xFF);
        sleep_ms(500);
        mDisplay.fill_screen(0x00);
        sleep_ms(500);
    }

    mDisplay.write_buffer(symbols[static_cast<uint8_t>('H')], 5);
    mDisplay.write_buffer(symbols[static_cast<uint8_t>('e')], 5);
    mDisplay.write_buffer(symbols[static_cast<uint8_t>('l')], 5);
    mDisplay.write_buffer(symbols[static_cast<uint8_t>('l')], 5);
    mDisplay.write_buffer(symbols[static_cast<uint8_t>('o')], 5);
    mDisplay.write_buffer(symbols[static_cast<uint8_t>(' ')], 5);
    mDisplay.write_buffer(symbols[static_cast<uint8_t>('W')], 5);
    mDisplay.write_buffer(symbols[static_cast<uint8_t>('o')], 5);
    mDisplay.write_buffer(symbols[static_cast<uint8_t>('r')], 5);
    mDisplay.write_buffer(symbols[static_cast<uint8_t>('l')], 5);
    mDisplay.write_buffer(symbols[static_cast<uint8_t>('d')], 5);
    mDisplay.write_buffer(symbols[static_cast<uint8_t>('!')], 5);
}

void Application::initializeConsole()
{
    mCmdHelp.addControlObject(&mHandler);
    mHandler.addCommand(&mCmdHelp);

    console_set_command_handler(&mHandler);
    multicore_launch_core1(&console_run);
}

void Application::i2cBusScan(i2c_inst_t *bus)
{
    uint8_t rxdata = 0;

    printf("\nI2C Bus Scan\n");
    printf("   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");

    for (int addr = 0; addr < (1 << 7); ++addr) {
        if (addr % 16 == 0) {
            printf("%02x ", addr);
        }

        // Perform a 1-byte dummy read from the probe address. If a slave
        // acknowledges this address, the function returns the number of bytes
        // transferred. If the address byte is ignored, the function returns
        // -1.

        // Skip over any reserved addresses.
        int ret;
        uint8_t rxdata;
        if (reserved_addr(addr))
            ret = PICO_ERROR_GENERIC;
        else
            ret = i2c_read_blocking(bus, addr, &rxdata, 1, false);

        printf(ret < 0 ? "." : "@");
        printf(addr % 16 == 15 ? "\n" : "  ");
    }
    printf("Done.\n");
}
