#ifndef SERIALPROCESSOR_H
#define SERIALPROCESSOR_H

#include <Arduino.h>
#include <ArduinoLog.h>
#include "Buffer.h"

class SerialProcessor;

class CommandProcessor {
    public:
        CommandProcessor() {};
        SerialProcessor* serProc;
        CommandProcessor(SerialProcessor *serialProc) : serProc(serialProc) {}
        virtual void processCommand(Buffer* buffer) {};
        virtual void initCommandProcessor(Buffer* buffer) {
            Log.trace(F("CommandLineProcessor::initCommandProcessor [%s](%d)" CR), buffer->getBuffer(), buffer->getIndex());
        };
    private:
};

class DefaultCmdProc : public CommandProcessor {
    public:
        void initCommandProcessor(Buffer* buffer);
        void processCommand(Buffer* buffer);
};

struct Command {
    const char* cmdString;
    CommandProcessor* processor;
    Command* next;
};

class SerialProcessor : public CommandProcessor {
    public:
        SerialProcessor(Stream* serial);
        void checkSerial();
        bool read();
        void registerCommand(const char* command, CommandProcessor* cmdProc);
        void setCommandProcessor(CommandProcessor* lineProc);
        void resetLineProcessor();
        friend class DefaultCmdProc;

    private:
        Buffer _buffer;
        Stream *_serial;
        CommandProcessor* currentCmdProc;
        Command* commandList = NULL;
        DefaultCmdProc cmdProc;
};

struct KeyPtr {
    char kkey[10];
};

#endif