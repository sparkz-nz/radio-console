#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>

class Encoder {
    public:
        Encoder(int pinA, int pinB);
        void Update();
        int Value();

    private:
        int state, value, direction;
        bool aRising, aFalling, bRising, bFalling;
        int pinA, pinB;
        bool lastA, lastB;
        void UpdateValue();
};

#endif