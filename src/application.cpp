#include "rp2040/application.h"

const uint32_t Application::neopixel_pin = 16;
const uint32_t Application::neopixel_num_leds = 1;
const uint32_t Application::neopixel_max_brightness = 25;

const uint32_t Application::heartbeat_ms = 1000;

/**
 * @brief Construct a new Application:: Application object
 * 
 */
Application::Application()
    : mNeopixel(
        neopixel_pin,
        neopixel_num_leds,
        pio0,
        0,
        WS2812::DataFormat::FORMAT_GRB
    )
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

        mNeopixel.fill( WS2812::RGB(0, count, 0));
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
    mNeopixel.fill(WS2812::RGB(1, 0, 0));
    mNeopixel.show();
    sleep_ms(1000);

    printf("Booting...\n");
}