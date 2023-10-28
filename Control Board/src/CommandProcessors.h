#include <Arduino.h>
#include <ArduinoLog.h>
#include <SerialProcessor.h>
#include <Config.h>


/**
 * Config
*/
class ConfigCmdProcessor : public CommandProcessor {
    public:
        ConfigCmdProcessor(SerialProcessor *serialProc, Configuration *configuration)
         : CommandProcessor(serialProc) {
            config=configuration;
        }

        void initCommandProcessor(Buffer* buffer) override {
            Log.notice(F("Config> "));
        }

        void processCommand(Buffer* buffer) override {
            Log.trace(F("configCmdProcessor::processCommand buffer='%s'" CR), buffer->getBuffer());
            
            // if empty line, return to command processor 
            if (buffer->atEnd()) {
                serProc->resetLineProcessor();
                Log.trace(F("configCmdProcessor::processCommand - buffer empty, resetting back to cmdProc" CR));
                return;
            }
            config->processLine(buffer);
            Log.notice(F("Config> "));
        };

    private:
        Configuration* config;
};

/**
 * Save
*/
class SaveCmdProcessor : public CommandProcessor {
    public:
        SaveCmdProcessor(SerialProcessor *serialProc, Configuration *configuration)
        : CommandProcessor(serialProc) {
            config=configuration;
        }

        void initCommandProcessor(Buffer* buffer) override {
            Log.notice(F("Saving... "));
            config->save();
            Log.notice(F("done." CR));
            serProc->resetLineProcessor();
        }

        void processCommand(Buffer* buffer) override {
            Log.trace(F("SaveCmdProcessor::processCommand buffer='%s' - this should never be called" CR), buffer->getBuffer());
            // when all done reset processor 
            if (buffer->atEnd()) {
                serProc->resetLineProcessor();
                Log.trace(F("SaveCmdProcessor::processCommand - buffer empty, resetting back to cmdProc" CR));
                }
        }
    private:
        Configuration* config;
};

/**
 * Load
*/
class LoadCmdProcessor : public CommandProcessor {
    public:
        LoadCmdProcessor(SerialProcessor *serialProc, Configuration *configuration)
        : CommandProcessor(serialProc) {
            config=configuration;
        }

        void initCommandProcessor(Buffer* buffer) override {
            Log.notice(F("Loading... "));
            config->load();
            Log.notice(F("done." CR));
            serProc->resetLineProcessor();
        }

        void processCommand(Buffer* buffer) override {
            Log.trace(F("LoadCmdProcessor::processCommand buffer='%s' - this should never be called" CR), buffer->getBuffer());
            // when all done reset processor 
            if (buffer->atEnd()) {
                serProc->resetLineProcessor();
                Log.trace(F("LoadCmdProcessor::processCommand - buffer empty, resetting back to cmdProc" CR));
                }
        }
    private:
        Configuration* config;
};

/**
 * Clear
*/
class ClearCmdProcessor : public CommandProcessor {
    public:
        ClearCmdProcessor(SerialProcessor *serialProc, Configuration *configuration)
        : CommandProcessor(serialProc) {
            config=configuration;
        }

        void initCommandProcessor(Buffer* buffer) override {
            Log.notice(F("Clearing all config... "));
            config->clearConfig();
            Log.notice(F("done." CR));
            serProc->resetLineProcessor();
        }

        void processCommand(Buffer* buffer) override {
            Log.trace(F("ClearCmdProcessor::processCommand buffer='%s' - this should never be called" CR), buffer->getBuffer());
            // when all done reset processor 
            if (buffer->atEnd()) {
                serProc->resetLineProcessor();
                Log.trace(F("ClearCmdProcessor::processCommand - buffer empty, resetting back to cmdProc" CR));
                }
        }
    private:
        Configuration* config;
};

/**
 * Output
*/
class OutputCmdProcessor : public CommandProcessor {
    public:
        OutputCmdProcessor(SerialProcessor *serialProc, Configuration *configuration)
        : CommandProcessor(serialProc) {
            config=configuration;
        }

        void initCommandProcessor(Buffer* buffer) override {
            Log.trace(F("OutputCmdProcessor::processCommand buffer='%s'" CR), buffer->getBuffer());

            if (strncasecmp("on", buffer->getBuffer()+7, 2) == 0) {
                Log.setLevel(LOG_LEVEL_NOTICE);
                Log.notice(F("Serial output enabled" CR));
            }
            if (strncasecmp("of", buffer->getBuffer()+7, 2) == 0) {
                Log.notice(F("Turning serial output off" CR));
                Log.setLevel(LOG_LEVEL_SILENT);
            }
            if (strncasecmp("tr", buffer->getBuffer()+7, 2) == 0) {
                Log.setLevel(LOG_LEVEL_TRACE);
                Log.notice(F("Serial output trace enables" CR));
            }
            serProc->resetLineProcessor();
        }

        void processCommand(Buffer* buffer) override {
            Log.trace(F("OutputCmdProcessor::processCommand buffer='%s' - this should never be called" CR), buffer->getBuffer());
            // when all done reset processor 
            if (buffer->atEnd()) {
                serProc->resetLineProcessor();
                Log.trace(F("OutputCmdProcessor::processCommand - buffer empty, resetting back to cmdProc" CR));
                }
        }
    private:
        Configuration* config;
};


/**
 * Dump
*/
class DumpCmdProcessor : public CommandProcessor {
    public:
        DumpCmdProcessor(SerialProcessor *serialProc, Configuration *configuration)
         : CommandProcessor(serialProc) {
            config=configuration;
        }

        void initCommandProcessor(Buffer *buffer) override {
            Log.notice(F("config" CR));
            for (int m = 0; m < NUMMODES; m++) {
                for (int s = 0; s < NUMSWITCHES; s++) {
                    SwitchConfig sc = config->modes[m].switches[s];
                    printConfig(m, 's', s, sc);
                }
                for (int e = 0; e < NUMENCODERS; e++) {
                    SwitchConfig sc = config->modes[m].encoders[e];
                    printConfig(m, 'e', e, sc);
                }
            }
            serProc->resetLineProcessor();
        }

    private:
        Configuration* config;
        void printConfig(int mode, char type, int num, SwitchConfig sc) {
            Log.notice(F("%d:%c%d:"), mode+1, type, num+1);
            for (int response = 0; response < 2; response++) {
                char c = sc.response[response].character;
                if (c > ' ' && c <= 'z') {
                    Log.notice("%c", c);
                }
                else {
                    Log.trace("{%x}", (uint8_t)c);
                }
                for (int mod = 0; mod < MAXMODIFIERS; mod++){
                    switch (uint8_t(sc.response[response].modifiers[mod])) {
                        case 0:
                            break; // no more modifiers
                        case KEY_LEFT_CTRL:
                            Log.notice("LC"); break;
                        case KEY_LEFT_SHIFT:
                            Log.notice("LS"); break;
                        case KEY_LEFT_ALT:
                            Log.notice("LA"); break;
                        case KEY_LEFT_GUI:
                            Log.notice("LG"); break;
                        case KEY_RIGHT_CTRL:
                            Log.notice("RC"); break;
                        case KEY_RIGHT_SHIFT:
                            Log.notice("RS"); break;
                        case KEY_RIGHT_ALT:
                            Log.notice("RA"); break;
                        case KEY_RIGHT_GUI:
                            Log.notice("RG"); break;
                        default: Log.notice("Modifier not found %x", uint8_t(sc.response[response].modifiers[mod]));
                    }
                }
                if (response == 0) Log.notice(":");
            }
            Log.notice(CR);
        }
};