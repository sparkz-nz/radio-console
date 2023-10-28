#include <Arduino.h>
#include "Encoder.h"

Encoder::Encoder(int pinA, int pinB) {
    this->pinA = pinA;
    this->pinB = pinB;

    pinMode(pinA, INPUT_PULLUP);
    pinMode(pinB, INPUT_PULLUP);
}

int8_t Encoder::Value() {
    int8_t v = value;
    value = 0;
    return v;
}

void Encoder::Update() {
    bool a = digitalRead(pinA);
    bool b = digitalRead(pinB);

    if (a == lastA && b == lastB) return;  // no change

    bool aFalling = (!a && lastA);
    bool aRising = (a && !lastA);
    bool bFalling = (!b && lastB);
    bool bRising = (b && !lastB);

    lastA = a;
    lastB = b;
    int nextState = state;

    switch (state) {
        case 0b11: // 11 - encoder in detent
            if (aFalling) nextState = 0b01;
            if (bFalling) nextState = 0b10;
            break;

        case 0b01:
            if (aRising) {
                nextState = 0b11;
                UpdateValue();
            } 
            if (bFalling) nextState = 0b00;
            break;

        case 0b00: // 00 half way round
            if (aRising) {
                nextState = 0b10;
                direction = 1; // turning CW
            }
            if (bRising) {
                nextState = 0b01;
                direction = -1; // turning CCW
            }
            break;

        case 0b10:
            if (bRising) {
                nextState = 0b11;
                UpdateValue();
            }
            if (aFalling) nextState = 0b00;
            break;
    }

    state = nextState;
    lastA = a;
    lastB = b;
}

void Encoder::UpdateValue() {
    value += direction;
    direction = 0;
}
