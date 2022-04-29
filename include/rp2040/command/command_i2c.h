#ifndef RP2040_COMMAND_I2C_H
#define RP2040_COMMAND_I2C_H

#include "rp2040/command/command_template.h"
#include "rp2040/drivers/i2c.h"

#define COMMAND_NAME_I2CDETECT  "i2cdetect"
#define COMMAND_NAME_BUS        "bus"
#define COMMAND_NAME_DETECT     "detect"

class CommandI2CDetect :
        public CommandTemplate<i2c_inst_t>
{
public:
    CommandI2CDetect();
    int32_t setBus(cJSON *json);

    int32_t getDetect(cJSON *json);

protected:
};

#endif // RP2040_COMMAND_I2C_H
