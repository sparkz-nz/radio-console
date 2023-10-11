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
        currentLineProc->processLine(&_buffer);
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

/** --- CmdProc::processLine
 * Default command processor for top level commands.
 * Calls the user supplied callback for the command.
*/
void CmdProc::processLine(Buffer *buffer) {
    // walk through commandList and compare to start of buffer
    // if found, call the callback for that command with the buffer as a parameter
    Log.trace(F("CmdProc::processCommands - buffer: '%s' [%d]" CR), buffer->getBuffer(), strlen(buffer->getBuffer()));

    Command* cmdPtr = serProc->commandList;
    while (cmdPtr) {
        Log.trace(F("checking %d chars of cmdPtr->cmdString '%s' against buffer '%s'" CR), strlen(cmdPtr->cmdString), cmdPtr->cmdString, buffer->getBuffer());
        if (strncmp(cmdPtr->cmdString, buffer->getBuffer(), strlen(cmdPtr->cmdString)) == 0) {
            Log.trace(F("Found command %s, setting processor..." CR), cmdPtr->cmdString);
            buffer->setIndex(strlen(cmdPtr->cmdString)); // buffer index to next char after command token
            while (buffer->peekNext() == ' ') buffer->getNext(); // consume any spaces
            serProc->setLineProcessor(cmdPtr->processor);
            cmdPtr->processor->initProcess(buffer);
            break;
        }
        cmdPtr = cmdPtr->next;
    }


    // if (buffer->getNext() == 'c') {
    //     Log.trace(F("Found 'c', switching to config" CR));
    //     serProc->setLineProcessor(serProc->commandList->processor);
    // }
};

/** --- registerCommand
 * Adds a command to the commands linked list
*/
void SerialProcessor::registerCommand(const char* command, CommandLineProcessor* cmdLineProc) {
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

void printCmdList(Command* list){
    Command* current = list;
    int i = 0;
    do {
        Log.trace(F("'%s' [%X]" CR), current->cmdString, current->next);
        current = current->next;
        delay(50);
    } while (current != NULL && i++ < 10);
}

void SerialProcessor::setLineProcessor(CommandLineProcessor* lineProc) {
    currentLineProc = lineProc;
}

void SerialProcessor::resetLineProcessor() {
    currentLineProc = (CommandLineProcessor*)&cmdProc;
    Log.trace(F("currentLineProcessor has been reset to cmdProc" CR));
}

void SerialProcessor::debug() {

    Log.trace("debug - Buffer: %s" CR, _buffer.getBuffer());
    _buffer.clear();
}
