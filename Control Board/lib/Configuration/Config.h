#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <Buffer.h>
#include "KeyMap.h"

#define MAXMODIFIERS 2

#define LEFTSHIFT  0b00100000
#define LEFTCTRL   0b00010000
#define LEFTALT    0b00001000
#define RIGHTSHIFT 0b00000100
#define RIGHTCTRL  0b00000010
#define RIGHTALT   0b00000001

#define NUMENCODERS 6
#define NUMSWITCHES 20
#define NUMMODES    5

#define EEPROM_CONFIG_ADDR 0x0


struct SwitchResponse {
  uint8_t character = '\0';
  uint8_t modifiers[MAXMODIFIERS] = {0};
};

struct SwitchConfig {
  SwitchResponse response[2];
};

struct Mode {
  SwitchConfig encoders[NUMENCODERS];
  SwitchConfig switches[NUMSWITCHES];
};

class Configuration {
  public:
    Mode modes[NUMMODES];
    void save();
    void load();
    void processLine(Buffer *buffer);
    void clearConfig();


};

#endif