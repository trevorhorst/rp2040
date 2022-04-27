#ifndef RP2040_COMMAND_PIXEL_H
#define RP2040_COMMAND_PIXEL_H

#include "rp2040/command/command_template.h"
#include "rp2040/drivers/ws2812.h"

#define COMMAND_NAME_PIXEL  "pixel"

class CommandPixel :
        public CommandTemplate<WS2812>
{
public:
    CommandPixel();

};

#endif // RP2040_COMMAND_PIXEL_H
