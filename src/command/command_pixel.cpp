#include "rp2040/command/command_pixel.h"

CommandPixel::CommandPixel() :
    CommandTemplate<WS2812>(COMMAND_NAME_PIXEL)
{

}
