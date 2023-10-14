#include <Arduino.h>
#include <ArduinoLog.h>
#include <Wire.h>
#include "Keyboard.h"
#include <Config.h>
#include <SerialProcessor.h>
#include <IOBoard.h>
#include "CommandProcessors.h"

#define SERIAL_BAUD 9600

#define NUMIOBOARDS 2


Configuration config;
SerialProcessor sp(&Serial);
ConfigCmdProcessor configCmdProc(&sp, &config);
SaveCmdProcessor saveCmdProc(&sp, &config);
LoadCmdProcessor loadCmdProc(&sp, &config);
DumpCmdProcessor dumpCmdProc(&sp, &config);
DataPacket dataPackets[NUMIOBOARDS];
int dataPacketSize = sizeof(DataPacket);

void printMsg();
void test();

void setup() {
  Serial.begin(SERIAL_BAUD);
  Wire.begin();
  while (!Serial);
  Log.begin(LOG_LEVEL_VERBOSE, &Serial, false);
  Log.trace("Starting..." CR);

  sp.registerCommand("config", &configCmdProc);
  sp.registerCommand("save", &saveCmdProc);
  sp.registerCommand("load", &loadCmdProc);
  sp.registerCommand("dump", &dumpCmdProc);


}

void loop() {

  sp.checkSerial();

  // check Board 1 
  int addr = 9;
  Log.trace("Requesting %d bytes from addr %d" CR, dataPacketSize, addr);
  Wire.requestFrom(addr, dataPacketSize);

  for (int i=0; i < IOBOARD_NUMENCODERS; i++) {
    if (Wire.available()) {
      dataPackets[0].encoders[i] = Wire.read();
    }
    else {
      Log.error(F("No IIC data available from addr %d for encoder %d" CR), addr, i+1);
      break;
    }
  }

  for (int i=0; i < IOBOARD_NUMSWITCHES; i++) {
    if (Wire.available()) {
      dataPackets[0].switches[i] = Wire.read();
    }
    else {
      Log.error(F("No IIC data available from addr %d for switch %d" CR), addr, i+1);
      break;
    }
  }

  test();

  delay(500);
}


void test() {
  for (int i=0; i<NUMIOBOARDS; i++) {
    for (int enc=0; enc < IOBOARD_NUMENCODERS; enc++) {
      Log.trace("%X ", dataPackets[i].encoders[enc]);
    }
    for (int sw=0; sw < IOBOARD_NUMENCODERS; sw++) {
      Log.trace("%X ", dataPackets[i].switches[sw]);
    }
    Log.trace(CR);
  }

}
