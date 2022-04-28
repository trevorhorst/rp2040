#include "rp2040/command/command_pixel.h"

CommandPixel::CommandPixel() :
    CommandTemplate<WS2812>(COMMAND_NAME_PIXEL)
{
    mMutableMap[COMMAND_NAME_RGB] = BIND_PARAMETER(&CommandPixel::setRgb);
}

int32_t CommandPixel::setup()
{
    mControlObject = static_cast<WS2812*>(mControlObjects.back());
    return Error::NONE;
}

int32_t CommandPixel::setRgb(cJSON *json)
{
    int32_t error = Error::NONE;

    int rgb[3] = {0,0,0};

    if(cJSON_IsArray(json)) {
        int size = cJSON_GetArraySize(json);
        for(int32_t i = 0; i < size; i++) {
            cJSON *item = cJSON_GetArrayItem(json, i);
            if(cJSON_IsNumber(item)) {
                rgb[i] = item->valuedouble;
            } else {
                error = Error::PARAM_WRONG_TYPE;
            }
        }
    } else {
        error = Error::PARAM_WRONG_TYPE;
    }

    if(error == Error::NONE) {
        mControlObject->fill(WS2812::RGB(rgb[0], rgb[1], rgb[2]));
        mControlObject->show();
    }

    return error;
}
