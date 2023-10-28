#ifndef IOBOARD_H
#define IOBOARD_H

#include <Arduino.h>

#define IOBOARD_NUMENCODERS 3
#define IOBOARD_NUMSWITCHES 9
#define IOBOARD_BASEADDRESS 8

struct DataPacket {
    int8_t encoders[IOBOARD_NUMENCODERS];
    int8_t switches[IOBOARD_NUMSWITCHES];
};

struct IOBoard {
    IOBoard(uint8_t addr) : address(addr) {}
    uint8_t address;
};

#endif