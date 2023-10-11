#ifndef BUFFER_H
#define BUFFER_H

#include <Arduino.h>
#include <ArduinoLog.h>

#define BUFFERSIZE 80

class Buffer {
    public:
        Buffer();
        void addChar(char c);
        char getNext();
        char getFirst();
        int parseInt();
        void clear();
        bool atEnd();
        char* getBuffer();
        bool deleteLast();
        int setIndex(int index);
        char peekNext();
        int getIndex();
        void addString(const char*);

    private:
        char _buffer[BUFFERSIZE + 1];
        int _index = 0;
        int _end = 0;

};


#endif