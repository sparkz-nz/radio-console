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
#define LOCAL_BOARDINDEX NUMIOBOARDS
#define LOCAL_NUMBOARDS 1
#define LOCAL_NUMENCODERS 0
#define LOCAL_NUMSWITCHES 2
#define MODESWITCH 0

int localSwitches[LOCAL_NUMSWITCHES] = {9, 10}; // define extra switch channels local on the control board

enum deviceTypes {
  encoders, switches
};

Configuration config;
SerialProcessor sp(&Serial);
ConfigCmdProcessor configCmdProc(&sp, &config);
SaveCmdProcessor saveCmdProc(&sp, &config);
LoadCmdProcessor loadCmdProc(&sp, &config);
DumpCmdProcessor dumpCmdProc(&sp, &config);
ClearCmdProcessor clearCmdProc(&sp, &config);
OutputCmdProcessor outputCmdProc(&sp, &config);
DataPacket dataPackets[NUMIOBOARDS + LOCAL_NUMBOARDS], lastDataPackets[NUMIOBOARDS + LOCAL_NUMBOARDS];
int dataPacketSize = sizeof(DataPacket);
int currentMode = 0;
int lastMode=0;
int totalEncoders = IOBOARD_NUMENCODERS * NUMIOBOARDS + LOCAL_NUMENCODERS;
int totalSwiches = IOBOARD_NUMSWITCHES * NUMIOBOARDS + LOCAL_NUMSWITCHES;

void test();
void readInputs();
int getMode();
void processChanges();
void initialiseLastDataPackets();
 

void setup() {
  Serial.begin(SERIAL_BAUD);
  Wire.begin();
  //while (!Serial);
  Log.begin(LOG_LEVEL_SILENT, &Serial, false);
  Log.trace("Starting..." CR);

  loadCmdProc.initCommandProcessor( NULL );

  Keyboard.begin();

  sp.registerCommand("config", &configCmdProc);
  sp.registerCommand("save", &saveCmdProc);
  sp.registerCommand("load", &loadCmdProc);
  sp.registerCommand("dump", &dumpCmdProc);
  sp.registerCommand("clear", &clearCmdProc);
  sp.registerCommand("output", &outputCmdProc);

  // initialise inputs for local switch channels
  for (int i=0; i<LOCAL_NUMSWITCHES; i++){
    pinMode(localSwitches[i], INPUT_PULLUP);
  }

  readInputs();
  initialiseLastDataPackets ();
}

void loop() {

  sp.checkSerial();

  readInputs();
  currentMode = getMode();
  if (currentMode != lastMode) {
    lastMode = currentMode;
    Log.trace(F("Mode changed to %d" CR), currentMode+1);
  }
  processChanges();

  delay(1);
}


void readInputs() {
  // read from each IO board using IIC
  for (int board = 0; board < NUMIOBOARDS; board++) {
    int addr = IOBOARD_BASEADDRESS + board;
    Wire.requestFrom(addr, dataPacketSize);

    // read encoder values into datapacket
    for (int encoder=0; encoder < IOBOARD_NUMENCODERS; encoder++) {
      if (Wire.available()) {
        dataPackets[board].encoders[encoder] = Wire.read();
      }
      else {
        Log.error(F("No IIC data available from addr %d for encoder %d" CR), addr, encoder+1);
        break;
      }
    }

    // read switch values into datapacket
    for (int switch_=0; switch_ < IOBOARD_NUMSWITCHES; switch_++) {
      if (Wire.available()) {
        dataPackets[board].switches[switch_] = Wire.read();
      }
      else {
        Log.error(F("No IIC data available from addr %d for switch %d" CR), addr, switch_+1);
        break;
      }
    }

  }

  // read from local switches
  for (int i=0; i<LOCAL_NUMSWITCHES; i++) {
    dataPackets[LOCAL_BOARDINDEX].switches[i] = digitalRead(localSwitches[i]);
  }
}

int8_t getSwitchValue(DataPacket dp[], int switchNumber){
  return dp[switchNumber/IOBOARD_NUMSWITCHES].switches[switchNumber % IOBOARD_NUMSWITCHES];
}

void setSwitchValue(DataPacket dp[], int switchNumber, int8_t value){
  dp[switchNumber/IOBOARD_NUMSWITCHES].switches[switchNumber % IOBOARD_NUMSWITCHES] = value;
}

int8_t getEncoderValue(DataPacket dp[], int encoderNumber){
  return dp[encoderNumber/IOBOARD_NUMENCODERS].encoders[encoderNumber % IOBOARD_NUMENCODERS];
}

void setEncoderValue(DataPacket dp[], int encoderNumber, int8_t value){
  dp[encoderNumber/IOBOARD_NUMENCODERS].encoders[encoderNumber % IOBOARD_NUMENCODERS] = value;
}

void initialiseLastDataPackets() {
  for (int board = 0; board < NUMIOBOARDS; board++) {
    for (int encoder = 0; encoder < IOBOARD_NUMENCODERS; encoder++) {
      lastDataPackets[board].encoders[encoder] = dataPackets[board].encoders[encoder];
    }
    for (int sw = 0; sw < IOBOARD_NUMSWITCHES; sw++) {
      lastDataPackets[board].switches[sw] = dataPackets[board].switches[sw];
    }
  }
}

int getMode() {
  for (int m = 0; m < NUMMODES; m++) {
    if (!getSwitchValue(dataPackets, MODESWITCH + m)) {
      return m;
    }
  }
  return 0;
}

void sendKeypress(SwitchResponse response){
  Log.trace(F("[ "));
  for (int modifier=0; modifier < MAXMODIFIERS; modifier++) {
    if (response.modifiers[modifier] != '\0') {
      Keyboard.press(response.modifiers[modifier]);
      Log.trace(F("%X "), response.modifiers[modifier]);
      delay(10);
    }
  }
  Log.trace(F("] < "));
  if (response.character != '\0') {
    Keyboard.press(response.character);
    Log.trace(F("%X "), response.character);
  }
  Log.trace(F(">"));
}

void sendKeyCombo(deviceTypes deviceType, int deviceNumber, int8_t value) {
  SwitchResponse response;
  bool hold = false;
  bool on_up;
  if (deviceType == encoders) {
    on_up = (value > 0);
    response = config.modes[currentMode].encoders[deviceNumber].response[on_up ? 0 : 1];
    Log.trace(F("E%d %s "), deviceNumber+1, on_up ? "inc" : "dec");
  }
  else {
    on_up = (value == 0);
    response = config.modes[currentMode].switches[deviceNumber].response[on_up ? 0 : 1];
    hold = (config.modes[currentMode].switches[deviceNumber].response[1].character == (uint8_t)0xFF); // if the release code is FF then we hold until released
    Log.trace(F("S%d %s %s "), deviceNumber+1, on_up ? "on " : "off", hold ? "H" : "-");
  }

  if (on_up || !hold) {
    sendKeypress(response);
    delay(30);
  }

  if ( !(on_up && hold) ) {
    Keyboard.releaseAll();
    Log.trace(F(" [release all]"));
  }
  Log.trace(CR);
}

void processChanges() {
  for (int enc = 0; enc < totalEncoders; enc++) {
    int8_t value = getEncoderValue(dataPackets, enc);
    // send keystroke if encoder value is not zero
    if (value) {
      sendKeyCombo(encoders, enc, value);
    }
  }
  for (int sw = 0; sw < totalSwiches; sw++) {
    int8_t value = getSwitchValue(dataPackets, sw);
    // send keystroke if switch value has changed
    if (value != getSwitchValue(lastDataPackets, sw)) {
      sendKeyCombo(switches, sw, value);
      setSwitchValue(lastDataPackets, sw, value);
    }
  }
}