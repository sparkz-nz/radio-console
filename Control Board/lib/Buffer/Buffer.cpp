#include <Arduino.h>
#include <ArduinoLog.h>
#include "Buffer.h"

Buffer::Buffer() {
    clear();
}

void Buffer::clear() {
    _buffer[0] = '\0';  // pointer to start of buffer array
    _index = 0;         // current position for reading through
    _end = 0;           // index of null terminator
}

void Buffer::addChar(char c) {
    if (_end < BUFFERSIZE) {
        _buffer[_end] = c;
        _buffer[++_end] = '\0';
    }
}

char Buffer::getNext() {
    if (_buffer[_index] != '\0') {
        return _buffer[_index++];
    }
    return '\0';
}

char Buffer::getFirst() {
    _index = 0;
    return getNext();
}

char* Buffer::getBuffer() {
    return _buffer;
}

bool Buffer::atEnd() {
    return (_buffer[_index] == '\0');
}

bool Buffer::deleteLast() {
    if (_end > 0) {
        _buffer[--_end] = '\0';
        return true;
    }
    return false;
}

int Buffer::setIndex(int index) {
    if (index <= _end) {
        _index = index;
    }
    else {
        _index = _end;
    }
    return _index;
}

char Buffer::peekNext() {
    return(_buffer[_index]);
}

int Buffer::getIndex() {
    return _index;
}

int Buffer::parseInt() {
    int result = 0;
    while (isdigit(_buffer[_index])) {
        Log.trace("[%d]'%c' ", _index, _buffer[_index]);
        int d = getNext() - '0';
        result = (result * 10) + d;
        Log.trace("%d -> %d" CR, d, result);
    }
    return result;
}

void Buffer::addString(const char* str) {
    int i = 0;
    while (str[i]) {
        addChar(str[i++]);
    }
}