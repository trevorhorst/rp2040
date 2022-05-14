#ifndef RP2040_COMMAND_FONT_H
#define RP2040_COMMAND_FONT_H

#include <stdio.h>

#include "rp2040/font/font.h"
#include "rp2040/command/command_template.h"
#include "rp2040/control/control_template.h"

#define COMMAND_NAME_FONT "font"
#define COMMAND_NAME_SYMBOL "symbol"

class Font :
        public ControlTemplate<Font>
{
public:
    Font();

    uint32_t printSymbol(uint8_t value);

};

class CommandFont :
        public CommandTemplate<Font>
{
public:
    CommandFont();

    int32_t setSymbol(cJSON *json);

protected:
    int32_t setup() override;
};

#endif // RP2040_COMMAND_FONT_H
