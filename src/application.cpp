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
    srand(time(nullptr));
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

    // int count = 0;
    // int incrementer = 0;
    // while(true) {
    //     count += incrementer;
    //     if(count == 0) {
    //         incrementer = 1;
    //     } else if(count == neopixel_max_brightness) {
    //         incrementer = -1;
    //     }

    //     // mNeopixel.fill(WS2812::RGB(0, count, 0));
    //     // mNeopixel.show();
    //     sleep_ms(heartbeat_ms / neopixel_max_brightness);
    // }

    SSD1306::DisplayRam ram[2];
    printf("filling display\n");
    mDisplay.fill_display_random(ram[0]);
    printf("writing display\n");
    mDisplay.write_buffer(ram[0]);
    printRamBoard(ram[0]);

    uint32_t gen = 0;
    SSD1306::DisplayRam *cur = &ram[0];
    SSD1306::DisplayRam *nxt = &ram[1];
    do {
        // mDisplay.reset_cursor();
        printf("Generation: %d\n", gen++);
        // printRamBoard(*cur);
        checkRamBoard(*cur, *nxt);
        // Print the current generation and the board
        mDisplay.write_buffer(*nxt);
        // printRamBoard(*nxt);

        // Next board becomes the current and the current becomes the container for
        // our next generation
        SSD1306::DisplayRam *temp = cur;
        cur = nxt;
        nxt = temp;
        mDisplay.fill_display(*nxt);

        // Sleep so the results are easily viewable
        sleep_us(250000);
    } while(true);

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

    // Flash display so we know it's alive
    mDisplay.fill_screen(0xFF);
    sleep_ms(500);
    mDisplay.fill_screen(0x00);
}

void Application::initializeConsole()
{
    mCmdHelp.addControlObject(&mHandler);
    mCmdPixel.addControlObject(&mNeopixel);

    mHandler.addCommand(&mCmdHelp);
    mHandler.addCommand(&mCmdPixel);
    mHandler.addCommand(&mCmdI2CDetect);

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

void Application::printRamBoard(SSD1306::DisplayRam &ram)
{
    for(uint32_t page = 0; page < OLED_PAGE_HEIGHT; page++) {
        for(uint32_t shift = 0; shift < 8; shift++) {
            uint32_t bit = 1 << shift;
            for(uint32_t column = 0; column < OLED_WIDTH; column++) {
                if((ram[page][column] & bit) == 0) {
                    printf("  ");
                } else {
                    // printf("%01X ", ram[page][column] & 0xF);
                    printf("o ");
                }
            }
            printf("\n");
        }
    }
}

void Application::checkRamBoard(SSD1306::DisplayRam &ram, SSD1306::DisplayRam &newRam, bool debug)
{
    int32_t column_bits = 8;
    for(int32_t page = 0; page < OLED_PAGE_HEIGHT; page++) {
        for(int32_t shift = 0; shift < column_bits; shift++) {
            int32_t bit = 1 << shift;
            for(int32_t column = 0; column < OLED_WIDTH; column++) {
                int32_t neighbors = 0;
                bool leftValid = false;
                bool rightValid = false;
                bool upValid = false;
                bool upperPage = false;
                bool downValid = false;
                bool lowerPage = false;

                if((column - 1) >= 0) {
                    // Check left
                    neighbors += ((ram[page][column - 1] & bit) > 0);
                    leftValid = true;
                }

                if((column + 1) < OLED_WIDTH) {
                    // Check right
                    neighbors += ((ram[page][column + 1] & bit) > 0);
                    rightValid = true;
                }

                if((shift - 1) >= 0) {
                    // Check up
                    neighbors += ((ram[page][column] & (1 << (shift - 1))) > 0);
                    upValid = true;
                } else if((page - 1) >= 0) {
                    // If there is another page before this one, check the last
                    // bit in that column
                    neighbors += ((ram[page - 1][column] & (1 << (column_bits - 1))) > 0);
                    upValid = true;
                    upperPage = true;
                }

                if((shift + 1) < column_bits) {
                    // Check down
                    neighbors += ((ram[page][column] & (1 << (shift + 1))) > 0);
                    downValid = true;
                } else if((page + 1) < OLED_PAGE_HEIGHT) {
                    // If there is another page after this one, check the first
                    // bit in that column
                    neighbors += ((ram[page + 1][column] & (1 << 0)) > 0);
                    downValid = true;
                    lowerPage = true;
                }

                if(leftValid && upValid) {
                    if(upperPage) {
                        neighbors += ((ram[page - 1][column - 1] & (1 << (column_bits - 1))) > 0);
                    } else {
                        neighbors += ((ram[page][column - 1] & (1 << (shift - 1))) > 0);
                    }
                }

                if(rightValid && upValid) {
                    if(upperPage) {
                        neighbors += ((ram[page - 1][column + 1] & (1 << (column_bits - 1))) > 0);
                    } else {
                        neighbors += ((ram[page][column + 1] & (1 << (shift - 1))) > 0);
                    }
                }

                if(rightValid && downValid) {
                    if(lowerPage) {
                        neighbors += ((ram[page + 1][column + 1] & (1 << 0)) > 0);
                    } else {
                        neighbors += ((ram[page][column + 1] & (1 << (shift + 1))) > 0);
                    }
                }

                if(leftValid && downValid) {
                    if(lowerPage) {
                        neighbors += ((ram[page + 1][column - 1] & (1 << 0)) > 0);
                    } else {
                        neighbors += ((ram[page][column - 1] & (1 << (shift + 1))) > 0);
                    }
                }

                if(neighbors == 2) {
                    // Cell survives
                    newRam[page][column] |= (ram[page][column] & bit);
                } else if(neighbors == 3) {
                    // Cell survives / becomes living
                    newRam[page][column] |= bit;
                } else {
                    // Cell dies
                    newRam[page][column] &= ~bit;
                }
                if(debug){ printf("%d ", neighbors); }
            }
            if(debug){ printf("\n"); }
        }
    }
}
