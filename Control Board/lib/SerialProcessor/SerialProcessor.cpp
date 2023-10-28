#include <Arduino.h>
#include <ArduinoLog.h>
#include <SerialProcessor.h>
#include <Buffer.h>

#define ECHO_ON 1

void printCmdList(Command* list);

SerialProcessor::SerialProcessor(Stream* serial):
    _serial(serial) {
        cmdProc.serProc = this;
        resetLineProcessor();
}

/** --- checkSerial()
 * If a character is available it is read into the buffer
 * If end of line, then process the line with the current line processor
*/
void SerialProcessor::checkSerial() {
    if (read()) {
        currentCmdProc->processCommand(&_buffer);
        _buffer.clear();
    }
}

/** --- read
 * reads a character if available into the buffer, returns true on newline 
*/
bool SerialProcessor::read() {
    if (_serial->available()) {
        char c = _serial->read();

        switch (c) {
            case '\b':
                if (_buffer.deleteLast() && ECHO_ON) _serial->write("\b \b");
                break;

            case '\r':
                if (_serial->peek() == '\n') {
                    _serial->read();
                }

            case '\n':
                if (ECHO_ON) _serial->write('\n');
                return true;

            default:
                if (ECHO_ON) _serial->write(c);
                _buffer.addChar(c);
        }
    }
    return false;
}

/** --- DefaultCmdProc::processLine
 * Default command processor for top level commands.
 * Calls the user supplied callback for the command.
*/
void DefaultCmdProc::processCommand(Buffer *buffer) {
    // walk through commandList and compare to start of buffer
    // if found, 
    //   set the current command processor to the one supplied in the command list 
    //   and call the initCommandProcessor method with the buffer as a parameter

    Log.trace(F("DefaultCmdProc::processCommands - buffer: '%s' [%d]" CR), buffer->getBuffer(), strlen(buffer->getBuffer()));

    Command* cmdPtr = serProc->commandList;
    while (cmdPtr) {
        if (strncmp(cmdPtr->cmdString, buffer->getBuffer(), strlen(cmdPtr->cmdString)) == 0) {
            Log.trace(F("Found command %s, setting processor..." CR), cmdPtr->cmdString);
            buffer->setIndex(strlen(cmdPtr->cmdString)); // buffer index to next char after command token
            while (buffer->peekNext() == ' ') buffer->getNext(); // consume any spaces
            serProc->setCommandProcessor(cmdPtr->processor);
            break;
        }
        cmdPtr = cmdPtr->next;
    }
    serProc->currentCmdProc->initCommandProcessor(buffer);
};

void prompt() {
    Log.notice(F(">"));
}

void DefaultCmdProc::initCommandProcessor(Buffer* buffer) {
    prompt();
}

/** --- registerCommand
 * Adds a command to the commands linked list
*/
void SerialProcessor::registerCommand(const char* command, CommandProcessor* cmdLineProc) {
    struct Command *cmd = new Command {command, cmdLineProc, NULL};
    if (commandList == NULL) {
        commandList = cmd;
    }
    else {
        Command* current = commandList;
        while (current->next != NULL ) {
            current = current->next;
            }
        current->next = cmd;
        current = cmd;
    }
}

void SerialProcessor::setCommandProcessor(CommandProcessor* cmdProc) {
    currentCmdProc = cmdProc;
}

void SerialProcessor::resetLineProcessor() {
    currentCmdProc = (CommandProcessor*)&cmdProc;
    Log.trace(F("currentLineProcessor has been reset to cmdProc" CR));
    prompt();
}