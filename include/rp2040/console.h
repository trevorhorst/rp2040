#ifndef RP2040_CONSOLE_H
#define RP2040_CONSOLE_H

#include "string.h"
#include "pico/stdio.h"

#include "rp2040/logger.h"

#define CONSOLE_INPUT_LENGTH    1024

extern char console_input[CONSOLE_INPUT_LENGTH];

void console_flush_input();
void console_run();

// class Console
// {
// public:
//     Console();
//
//     void run();
//
// private:
//     bool mDone;
// };

#endif // RP2040_CONSOLE_H
