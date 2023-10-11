#ifndef SERIALPROCESSOR_H
#define SERIALPROCESSOR_H

#include <Arduino.h>
#include <ArduinoLog.h>
#include "Buffer.h"

class SerialProcessor;

class CommandLineProcessor {
    public:
        CommandLineProcessor() {};
        SerialProcessor* serProc;
        CommandLineProcessor(SerialProcessor *serialProc) : serProc(serialProc) {}
        virtual void processLine(Buffer* buffer) {};
        virtual void initProcess(Buffer* buffer) {
            Log.trace(F("CommandLineProcessor::initProcess [%s](%d)" CR), buffer->getBuffer(), buffer->getIndex());
        };
    private:
};

class CmdProc : public CommandLineProcessor {
    public:
        void processLine(Buffer* buffer);
};

struct Command {
    const char* cmdString;
    CommandLineProcessor* processor;
    Command* next;
};

class SerialProcessor : public CommandLineProcessor {
    public:
        SerialProcessor(Stream* serial);
        void checkSerial();
        bool read();
        void debug();
        void registerCommand(const char* command, CommandLineProcessor* cmdProc);
        void setLineProcessor(CommandLineProcessor* lineProc);
        void resetLineProcessor();
        friend class CmdProc;

    private:
        Buffer _buffer;
        Stream *_serial;
        CommandLineProcessor* currentLineProc;
        Command* commandList = NULL;
        CmdProc cmdProc;
};



struct KeyPtr {
    char kkey[10];
};

#endif