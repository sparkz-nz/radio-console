#include <Arduino.h>
#include <ArduinoLog.h>
#include <EEPROM.h>
#include <SerialProcessor.h>
#include <Config.h>


/**
 * Config
*/
class ConfigCmdProcessor : public CommandLineProcessor {
    public:
        ConfigCmdProcessor(SerialProcessor *serialProc, Configuration *configuration)
         : CommandLineProcessor(serialProc) {
            config=configuration;
        }

        void processLine(Buffer* buffer) {
            Log.trace(F("configCmdProcessor::processLine buffer='%s'" CR), buffer->getBuffer());
            
            // if empty line, return to command processor 
            if (buffer->atEnd()) {
                serProc->resetLineProcessor();
                Log.trace(F("configCmdProcessor::processLine - buffer empty, resetting back to cmdProc" CR));
            }

            config->processLine(buffer);
        };

    private:
        Configuration* config;
};

/**
 * Save
*/
class SaveCmdProcessor : public CommandLineProcessor {
    public:
        SaveCmdProcessor(SerialProcessor *serialProc, Configuration *configuration)
        : CommandLineProcessor(serialProc) {
            config=configuration;
        }

        void initProcess(Buffer* buffer) override {
            Log.trace(F("Saving... "));
            config->save();
            Log.trace(F("done. Returning to cmdProc" CR));
            serProc->resetLineProcessor();
        }

        void processLine(Buffer* buffer) {
            Log.trace(F("SaveCmdProcessor::processLine buffer='%s' - this should never be called" CR), buffer->getBuffer());
            // when all done reset processor 
            if (buffer->atEnd()) {
                serProc->resetLineProcessor();
                Log.trace(F("SaveCmdProcessor::processLine - buffer empty, resetting back to cmdProc" CR));
                }
        }
    private:
        Configuration* config;
};

/**
 * Load
*/
class LoadCmdProcessor : public CommandLineProcessor {
    public:
        LoadCmdProcessor(SerialProcessor *serialProc, Configuration *configuration)
        : CommandLineProcessor(serialProc) {
            config=configuration;
        }

        void initProcess(Buffer* buffer) override {
            Log.trace(F("Loading... "));
            config->load();
            Log.trace(F("done. Returning to cmdProc" CR));
            serProc->resetLineProcessor();
        }

        void processLine(Buffer* buffer) {
            Log.trace(F("LoadCmdProcessor::processLine buffer='%s' - this should never be called" CR), buffer->getBuffer());
            // when all done reset processor 
            if (buffer->atEnd()) {
                serProc->resetLineProcessor();
                Log.trace(F("LoadCmdProcessor::processLine - buffer empty, resetting back to cmdProc" CR));
                }
        }
    private:
        Configuration* config;
};


/**
 * Dump
*/
class DumpCmdProcessor : public CommandLineProcessor {
    public:
        DumpCmdProcessor(SerialProcessor *serialProc, Configuration *configuration)
         : CommandLineProcessor(serialProc) {
            config=configuration;
        }

        void initProcess(Buffer *buffer) override {
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
                    Log.notice("{%x}", int(c));
                }
                for (int mod = 0; (sc.response[response].modifiers[mod] != 0) && (mod < MAXMODIFIERS); mod++){
                    switch (uint8_t(sc.response[response].modifiers[mod])) {
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
                        default: Log.notice("Modifier not found %x", uint8_t(sc.response[response].modifiers[mod]), KEY_LEFT_CTRL);
                    }
                }
                if (response == 0) Log.notice(":");
            }
            Log.notice(CR);
        }
};