#include <Arduino.h>
#include "Keyboard.h"


const char sBsl[]  PROGMEM = "\\";         const byte kBsl   PROGMEM = '\\';
const char scln[]  PROGMEM = ":";          const byte kcln   PROGMEM = ':';
const char sLC[]   PROGMEM = "LC";         const byte kLC    PROGMEM = KEY_LEFT_CTRL;
const char sLS[]   PROGMEM = "LS";         const byte kLS    PROGMEM = KEY_LEFT_SHIFT;
const char sLA[]   PROGMEM = "LA";         const byte kLA    PROGMEM = KEY_LEFT_ALT;
const char sLG[]   PROGMEM = "LG";         const byte kLG    PROGMEM = KEY_LEFT_GUI;
const char sRC[]   PROGMEM = "RC";         const byte kRC    PROGMEM = KEY_RIGHT_CTRL;
const char sRS[]   PROGMEM = "RS";         const byte kRS    PROGMEM = KEY_RIGHT_SHIFT;
const char sRA[]   PROGMEM = "RA";         const byte kRA    PROGMEM = KEY_RIGHT_ALT;
const char sRG[]   PROGMEM = "RG";         const byte kRG    PROGMEM = KEY_RIGHT_GUI;
const char sHold[] PROGMEM = "HOLD";       const byte kHold  PROGMEM = 0xFF;

// Misc keys
const char sUp[]      PROGMEM = "UP";			const byte kUp     PROGMEM = KEY_UP_ARROW;
const char sDown[]    PROGMEM = "DOWN";			const byte kDown   PROGMEM = KEY_DOWN_ARROW;
const char sLeft[]    PROGMEM = "LEFT";			const byte kLeft   PROGMEM = KEY_LEFT_ARROW;
const char sRight[]   PROGMEM = "RIGHT";		const byte kRight  PROGMEM = KEY_RIGHT_ARROW;
const char sBS[]      PROGMEM = "BS";			const byte kBS     PROGMEM = KEY_BACKSPACE;
const char sTab[]     PROGMEM = "TAB";			const byte kTab    PROGMEM = KEY_TAB;
const char sRet[]     PROGMEM = "RET";			const byte kRet    PROGMEM = KEY_RETURN;
const char sMenu[]    PROGMEM = "MENU";			const byte kMenu   PROGMEM = KEY_MENU;
const char sEsc[]     PROGMEM = "ESC";			const byte kEsc    PROGMEM = KEY_ESC;
const char sIns[]     PROGMEM = "INS";			const byte kIns    PROGMEM = KEY_INSERT;
const char sDel[]     PROGMEM = "DEL";			const byte kDel    PROGMEM = KEY_DELETE;
const char sPgUp[]    PROGMEM = "PUP";			const byte kPgUp   PROGMEM = KEY_PAGE_UP;
const char sPgDn[]    PROGMEM = "PDN";			const byte kPgDn   PROGMEM = KEY_PAGE_DOWN;
const char sHome[]    PROGMEM = "HOME";			const byte kHome   PROGMEM = KEY_HOME;
const char sEnd[]     PROGMEM = "END";			const byte kEnd    PROGMEM = KEY_END;
const char sCaps[]    PROGMEM = "CAPS";			const byte kCaps   PROGMEM = KEY_CAPS_LOCK;
const char sPrt[]     PROGMEM = "PRT";			const byte kPrt    PROGMEM = KEY_PRINT_SCREEN;
const char sSL[]      PROGMEM = "SCRL";			const byte kSL     PROGMEM = KEY_SCROLL_LOCK;
const char sPs[]      PROGMEM = "PAUSE";		const byte kPs     PROGMEM = KEY_PAUSE;

// Numeric keypad
const char sNumLk[]   PROGMEM = "NLK";			const byte kNumLk  PROGMEM =  KEY_NUM_LOCK;
const char sNslash[]  PROGMEM = "NUM/";			const byte kNslash PROGMEM =  KEY_KP_SLASH;
const char sNstar[]   PROGMEM = "NUM*";			const byte kNstar  PROGMEM =  KEY_KP_ASTERISK;
const char sNminus[]  PROGMEM = "NUM-";			const byte kNminus PROGMEM =  KEY_KP_MINUS;
const char sNplus[]   PROGMEM = "NUM+";			const byte kNplus  PROGMEM =  KEY_KP_PLUS;
const char sNenter[]  PROGMEM = "ENT";			const byte kNenter PROGMEM =  KEY_KP_ENTER;
const char sN1[]      PROGMEM = "NUM1";			const byte kN1     PROGMEM =  KEY_KP_1;
const char sN2[]      PROGMEM = "NUM2";			const byte kN2     PROGMEM =  KEY_KP_2;
const char sN3[]      PROGMEM = "NUM3";			const byte kN3     PROGMEM =  KEY_KP_3;
const char sN4[]      PROGMEM = "NUM4";			const byte kN4     PROGMEM =  KEY_KP_4;
const char sN5[]      PROGMEM = "NUM5";			const byte kN5     PROGMEM =  KEY_KP_5;
const char sN6[]      PROGMEM = "NUM6";			const byte kN6     PROGMEM =  KEY_KP_6;
const char sN7[]      PROGMEM = "NUM7";			const byte kN7     PROGMEM =  KEY_KP_7;
const char sN8[]      PROGMEM = "NUM8";			const byte kN8     PROGMEM =  KEY_KP_8;
const char sN9[]      PROGMEM = "NUM9";			const byte kN9     PROGMEM =  KEY_KP_9;
const char sN0[]      PROGMEM = "NUM0";			const byte kN0     PROGMEM =  KEY_KP_0;
const char sNpt[]     PROGMEM = "NUM.";			const byte kNpt    PROGMEM =  KEY_KP_DOT;

// Function keys
const char sF1[]      PROGMEM = "F1";			const byte kF1     PROGMEM =  KEY_F1;
const char sF2[]      PROGMEM = "F2";			const byte kF2     PROGMEM =  KEY_F2;
const char sF3[]      PROGMEM = "F3";			const byte kF3     PROGMEM =  KEY_F3;
const char sF4[]      PROGMEM = "F4";			const byte kF4     PROGMEM =  KEY_F4;
const char sF5[]      PROGMEM = "F5";			const byte kF5     PROGMEM =  KEY_F5;
const char sF6[]      PROGMEM = "F6";			const byte kF6     PROGMEM =  KEY_F6;
const char sF7[]      PROGMEM = "F7";			const byte kF7     PROGMEM =  KEY_F7;
const char sF8[]      PROGMEM = "F8";			const byte kF8     PROGMEM =  KEY_F8;
const char sF9[]      PROGMEM = "F9";			const byte kF9     PROGMEM =  KEY_F9;
const char sF10[]     PROGMEM = "F10";			const byte kF10    PROGMEM =  KEY_F10;
const char sF11[]     PROGMEM = "F11";			const byte kF11    PROGMEM =  KEY_F11;
const char sF12[]     PROGMEM = "F12";			const byte kF12    PROGMEM =  KEY_F12;
const char sF13[]     PROGMEM = "F13";			const byte kF13    PROGMEM =  KEY_F13;
const char sF14[]     PROGMEM = "F14";			const byte kF14    PROGMEM =  KEY_F14;
const char sF15[]     PROGMEM = "F15";			const byte kF15    PROGMEM =  KEY_F15;
const char sF16[]     PROGMEM = "F16";			const byte kF16    PROGMEM =  KEY_F16;
const char sF17[]     PROGMEM = "F17";			const byte kF17    PROGMEM =  KEY_F17;
const char sF18[]     PROGMEM = "F18";			const byte kF18    PROGMEM =  KEY_F18;
const char sF19[]     PROGMEM = "F19";			const byte kF19    PROGMEM =  KEY_F19;
const char sF20[]     PROGMEM = "F20";			const byte kF20    PROGMEM =  KEY_F20;
const char sF21[]     PROGMEM = "F21";			const byte kF21    PROGMEM =  KEY_F21;
const char sF22[]     PROGMEM = "F22";			const byte kF22    PROGMEM =  KEY_F22;
const char sF23[]     PROGMEM = "F23";			const byte kF23    PROGMEM =  KEY_F23;
const char sF24[]     PROGMEM = "F24";			const byte kF24    PROGMEM =  KEY_F24;

// End of list marker
const char sEOL[]     PROGMEM = "EOL";          const byte kEOL    PROGMEM = 0;

const char* const stringTable[] PROGMEM = {
    sBsl, scln, sLC, sLS, sLA, sLG, sRC, sRS, sRA, sRG, sHold,
    sUp, sDown, sLeft, sRight, sBS, sTab, sRet, sMenu, sEsc, sIns, sDel, sPgUp, sPgDn, sHome, sEnd, sCaps, sPrt, sSL, sPs,
    sNumLk, sNslash, sNstar, sNminus, sNplus, sNenter, sN1, sN2, sN3, sN4, sN5, sN6, sN7, sN8, sN9, sN0, sNpt,
    sF1, sF2, sF3, sF4, sF5, sF6, sF7, sF8, sF9, sF10, sF11, sF12, sF13, sF14, sF15, sF16, sF17, sF18, sF19, sF20, sF21, sF22, sF23, sF24,
    sEOL};

const byte keycodeTable[] PROGMEM = {
    kBsl, kcln, kLC, kLS, kLA, kLG, kRC, kRS, kRA, kRG, kHold,
    kUp, kDown, kLeft, kRight, kBS, kTab, kRet, kMenu, kEsc, kIns, kDel, kPgUp, kPgDn, kHome, kEnd, kCaps, kPrt, kSL, kPs,
    kNumLk, kNslash, kNstar, kNminus, kNplus, kNenter, kN1, kN2, kN3, kN4, kN5, kN6, kN7, kN8, kN9, kN0, kNpt,
    kF1, kF2, kF3, kF4, kF5, kF6, kF7, kF8, kF9, kF10, kF11, kF12, kF13, kF14, kF15, kF16, kF17, kF18, kF19, kF20, kF21, kF22, kF23, kF24,
    kEOL};
