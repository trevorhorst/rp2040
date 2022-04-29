#ifndef RP2040_DRIVER_I2C_H
#define RP2040_DRIVER_I2C_H

#include <stdio.h>

#include "hardware/i2c.h"

#include "rp2040/control/control_template.h"

class I2C :
        public ControlTemplate<I2C>
{
public:
    I2C(i2c_inst_t *bus);

    static bool isReservedAddress(uint8_t address);
    static void detect(i2c_inst_t *bus);


    int32_t applySpeed(uint32_t baud);

private:
    i2c_inst_t *mBus;

};

#endif // RP2040_DRIVER_I2C_H
