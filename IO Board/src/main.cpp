#include <Arduino.h>
#include <ArduinoLog.h>
#include <Wire.h>
#include "Encoder.h"

#define SERIAL_BAUD 9600
#define ADDR 

// Encoder e1(2,3);
// Encoder e2(4,5);
// Encoder e3(6,7);
// Encoder encoders[] = {e1, e2, e3};

#define NUMENCODERS 3
Encoder encoders[] = {
  Encoder(2,3),
  Encoder(4,5),
  Encoder(6,7)
};

#define NUMSWITCHES 9
int switches[] = {8, 9, 10, 11, 12, A0, A1, A2, A3};
const int totalDevices = NUMENCODERS + NUMSWITCHES;
int values[3];

void iicRequest();

void setup() {
  Serial.begin(SERIAL_BAUD);
  while (!Serial);
  Log.begin(LOG_LEVEL_VERBOSE, &Serial, false);

  for (int i=0; i<NUMSWITCHES; i++) {
    pinMode(switches[i], INPUT_PULLUP);
  }

  int addr = (analogRead(A7) > 512) ? 8 : 9;
  Wire.begin(addr);
  Wire.onRequest(iicRequest);
  Log.trace("Starting IO board on address %d" CR, addr);


  PCICR  |= 0b00000100;
  PCMSK2 |= 0b11111100;

}

void loop() {
  // bool changed = false;
  // for (int i=0; i<3; i++){
  //   int v = encoders[i].Value();
  //   if (v != 0) {
  //     values[i] += v;
  //     changed = true;
  //   }
  // }
  // if (changed) Log.trace("%d %d %d" CR, values[0], values[1], values[2]);
  iicRequest();

  delay(100);
}

ISR (PCINT2_vect) 
{
  for (int i=0; i<3; i++){
    encoders[i].Update();
  }
}

void iicRequest() {
  uint8_t buffer[totalDevices];
  buffer[0] = encoders[0].Value();
  buffer[1] = encoders[1].Value(); 
  buffer[2] = encoders[2].Value();

  for (int i = 0; i < NUMSWITCHES; i++) {
    buffer[i+NUMENCODERS] = digitalRead(switches[i]);
  }

  Wire.write(buffer, 12);

  for (int i=0; i<totalDevices-1; i++) {
    Log.trace("%X ", buffer[i]);
  }
  Log.trace("%X" CR, buffer[11]);
}