#ifndef IOBOARD_H
#define IOBOARD_H

#include <Arduino.h>

#define IOBOARD_NUMENCODERS 3
#define IOBOARD_NUMSWITCHES 9

struct DataPacket {
    uint8_t encoders[IOBOARD_NUMENCODERS];
    uint8_t switches[IOBOARD_NUMSWITCHES];
};

struct IOBoard {
    IOBoard(uint8_t addr) : address(addr) {}
    uint8_t address;
};

#endif