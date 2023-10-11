#include <Arduino.h>
#include <ArduinoLog.h>
#include <EEPROM.h>
#include <Buffer.h>
#include <Config.h>


enum SwitchTypes {
    None, Switch, Encoder
};

struct TokenizedLine {
    int mode = 0;
    SwitchTypes switchType = None;
    int switchNumber = 0;
    SwitchResponse response[2];
};

void FlushUntilEOL();
uint8_t parseEscapeSequence(Buffer* buffer);
uint8_t parseModifier(Buffer* buffer);
TokenizedLine tokenizeLine(Buffer* buffer);
uint8_t searchStringTable(char* charBuf, int len);

void Configuration::save() {
  EEPROM.put(EEPROM_CONFIG_ADDR, modes);
}

void Configuration::load() {
  EEPROM.get(EEPROM_CONFIG_ADDR, modes);
}

void Configuration::processLine(Buffer *buffer) {
    if (buffer->atEnd()) {
        Log.error(F("Config line processor - no data" CR));
        return;
    }

    TokenizedLine tLine = tokenizeLine(buffer);

    if (tLine.switchType == None) {
        Log.trace(F("Invalid configuration line '%s' ignored" CR), buffer->getBuffer());
        return;
    }

    if (tLine.switchType == Switch) {
        if (tLine.switchNumber >= 0 && tLine.switchNumber < NUMSWITCHES) {
            modes[tLine.mode].switches[tLine.switchNumber-1].response[0] = tLine.response[0];
            modes[tLine.mode].switches[tLine.switchNumber-1].response[1] = tLine.response[1];
        }
        else Log.error(F("Switch number %d out of bounds in line '%s'" CR), tLine.switchNumber, buffer->getBuffer());
    }
    else {
        if (tLine.switchNumber >= 0 && tLine.switchNumber < NUMENCODERS) {
            modes[tLine.mode].encoders[tLine.switchNumber-1].response[0] = tLine.response[0];
            modes[tLine.mode].encoders[tLine.switchNumber-1].response[1] = tLine.response[1];
        }
        else Log.error(F("Encoder number %d out of bounds in line '%s'" CR), tLine.switchNumber, buffer->getBuffer());
    }

    // debug print stuff
    Log.trace(F("Tokenized Line: Mode %d <%d>%d: "), tLine.mode, tLine.switchType, tLine.switchNumber);
    for (int i=0; i<2; i++){
        char c = tLine.response[i].character;
        if (c > ' ' && c <= 'z') {
            Log.trace("%c (", c);
        }
        else {
            Log.trace("%x (", uint8_t(c));
        }
        for (int m = 0; tLine.response[i].modifiers[m] != '\0'; m++) {
            Log.trace(" %X", uint8_t(tLine.response[i].modifiers[m]));
        }
        if (i == 0) {
            Log.trace(") : ");
        }
        else {
            Log.trace(")" CR);
        }
    }


}

void Configuration::clearConfig() {

}

TokenizedLine tokenizeLine(Buffer* buffer) {
    TokenizedLine result;

    // first character is numeric mode 1..NUMMODES
    char c = buffer->getNext();
    int m = c - '1';
    if (m < 0 || m >= NUMMODES) {
        Log.error(F("Invalid mode %c in line '%s'" CR), c, buffer->getBuffer());
        return result;
    }
    result.mode = m;

    // expect : next
    if (buffer->getNext() != ':') {
        Log.error(F("Expected : after mode in line '%s'" CR), buffer->getBuffer());
        return result;
    }

    // next character - switch type s or e
    switch (buffer->getNext()) {
        case 's':
        case 'S':
            result.switchType = Switch;
            Log.trace(F("Found s" CR));
            break;

        case 'e':
        case 'E':
            result.switchType = Encoder;
            Log.trace(F("Found e" CR));
            break;
        
        default:
            Log.error(F("Invalid switch type in line '%s'" CR), buffer->getBuffer());
            return result;
    }

    // next one or two chars - switch number
    if (isdigit(buffer->peekNext())) {
        result.switchNumber = buffer->parseInt();
        Log.trace(F("Found number %d" CR), result.switchNumber);
    }
    else {
        result.switchType = None;
        Log.error(F("Invalid switch number in line '%s'" CR), buffer->getBuffer());
        return result;
    }
    
    // expect : next
    if (buffer->getNext() != ':') {
        result.switchType = None;
        Log.error(F("Expected : after switch number in line '%s'" CR), buffer->getBuffer());
        return result;
    }

    // next should be two response definitions (one for press/incr, one for release/decr)
    for (int i=0; i < 2; i++) {
        // first part is the char or an escape sequence or a ':' if nothing defined
        char c = buffer->getNext();
        if (c == ':') {
            result.response[i].character = '\0'; // null indicates undefined action
            continue;
        }
        else {
            if (c == '\\') c = parseEscapeSequence(buffer);
        }
        result.response[i].character = c;

        // next part should be zero or more modifiers terminated by ':' or '\0'
        int numModifiers = 0;
        while (buffer->peekNext() != ':' && buffer->peekNext() != '\0' && numModifiers < MAXMODIFIERS) {
            result.response[i].modifiers[numModifiers++] = parseModifier(buffer);
        }
        buffer->getNext(); // consume the terminator
        Log.trace(F("added %d modifiers" CR), numModifiers);
    }

    return result;
}

uint8_t parseEscapeSequence(Buffer* buffer) {
    int startIndex = buffer->getIndex();
    char nextChar;
    Log.trace(F("parse escape sequence..." CR));
    // an escape sequence is terminated by '\' so walk the buffer looking for '\'
    do {
        nextChar = buffer->getNext();
        Log.trace("[%c]", nextChar);
    } while (nextChar != '\\' && nextChar != '\0');
    Log.trace(CR);

    if (nextChar == '\0') return nextChar; // reached end of buffer with no '\' so return null

    int lenEscSeq = buffer->getIndex() - startIndex;
    uint8_t keyCode = searchStringTable(buffer->getBuffer() + startIndex, lenEscSeq - 1);
    Log.trace("parseEscapeSequence returns %X" CR, keyCode);
    return keyCode;
}

uint8_t parseModifier(Buffer* buffer) {
    uint8_t keycode = searchStringTable(buffer->getBuffer() + buffer->getIndex(), 2); // modifiers are two characters
    Log.trace(F("parseModifier %s found %X" CR), buffer->getBuffer() + buffer->getIndex(), keycode);
    buffer->getNext(); buffer->getNext(); // consume the two modifier characters, as searchStringTable does not
    return keycode;
}

uint8_t searchStringTable(char* charBuf, int len) {
    Log.trace("Searching string table for %d chars of %s" CR, len, charBuf);
    int i = 0;
    uint8_t val = 0xFF;
    bool found = false;
    do {
        found = (strncmp_P(charBuf, (const char*)pgm_read_word(&(stringTable[i])), len) == 0);
        val = pgm_read_byte(&keycodeTable[i++]); // read keycode, 0 at end
        if (found) {
            Log.trace(F("Found string in string table at index %d, keycode %X" CR), i-1, val);
        }
    } while (!found && val != 0); // until we find it or reach the end
    Log.trace("seachStringTable returns %X" CR, val);
    return val;
}





