#include "rp2040/command/command_i2c.h"

CommandI2CDetect::CommandI2CDetect() :
    CommandTemplate<i2c_inst_t>(COMMAND_NAME_I2CDETECT)
{
    mRequiredMap[COMMAND_NAME_BUS] = BIND_PARAMETER(&CommandI2CDetect::setBus);

    mAccessableMap[COMMAND_NAME_DETECT] = BIND_PARAMETER(&CommandI2CDetect::getDetect);
}

int32_t CommandI2CDetect::setBus(cJSON *json)
{
    int32_t error = Error::NONE;

    if(cJSON_IsNumber(json)) {
        uint32_t bus = static_cast<uint32_t>(json->valuedouble);
        if(bus == 0 && (i2c0)) {
            mControlObject = i2c0;
        } else if(bus == 1 && (i2c1)) {
            mControlObject = i2c1;
        } else {
            error = Error::PARAM_OUT_OF_RANGE;
        }
    } else {
        error = Error::PARAM_WRONG_TYPE;
    }

    return error;
}

int32_t CommandI2CDetect::getDetect(cJSON *json)
{
    int32_t error = Error::NONE;
    I2C::detect(mControlObject);
    return error;
}
