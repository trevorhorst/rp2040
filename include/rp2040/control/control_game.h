#ifndef RP2040_CONTROL_GAME_H
#define RP2040_CONTROL_GAME_H

#include "rp2040/drivers/ssd1306.h"

void conwaysRun();

void conwaysSetDisplay(SSD1306 *display);
void conwaysSetReset();
void conwaysStepSpeed();

void printRamBoard(SSD1306::DisplayRam &ram);
void checkRamBoard(SSD1306::DisplayRam &ram, SSD1306::DisplayRam &newRam, bool debug = false);
void checkRamBoardNew(SSD1306::DisplayRam &ram, SSD1306::DisplayRam &newRam, bool debug = false);

#endif // RP2040_CONTROL_GAME_H
