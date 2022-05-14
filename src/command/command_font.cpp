#include "rp2040/command/command_font.h"

Font::Font() :
    ControlTemplate<Font>()
{
}

uint32_t Font::printSymbol(uint8_t symbol)
{
    for(uint32_t row = 0; row < 5; row++) {
        uint8_t symbolBit = 0x80;
        for(uint32_t bit = 0; bit < 8; bit++) {
            if(symbols[symbol][row] & symbolBit) {
                printf("%c", 'O');
            } else {
                printf("%c", ' ');
            }
            symbolBit = symbolBit >> 1;
        }
        printf("row: %d\n", row);
    }
    return 0;
}

CommandFont::CommandFont() :
    CommandTemplate<Font>(COMMAND_NAME_FONT)
{
    mRequiredMap[COMMAND_NAME_SYMBOL] = BIND_PARAMETER(&CommandFont::setSymbol);
}

int32_t CommandFont::setup()
{
    mControlObject = static_cast<Font*>(mControlObjects.back());
    return Error::NONE;
}

int32_t CommandFont::setSymbol(cJSON *json)
{
    int32_t error = Error::NONE;
    if(cJSON_IsNumber(json)) {
        mControlObject->printSymbol(static_cast<uint8_t>(json->valuedouble));
    }
    return error;
}
