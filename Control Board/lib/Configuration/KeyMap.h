#include <Arduino.h>
#include "Keyboard.h"


const char sbs[]   PROGMEM = "\\";         const byte kbs    PROGMEM = '\\';
const char scln[]  PROGMEM = ":";          const byte kcln   PROGMEM = ':';
const char sLC[]   PROGMEM = "LC";         const byte kLC    PROGMEM = KEY_LEFT_CTRL;
const char sLS[]   PROGMEM = "LS";         const byte kLS    PROGMEM = KEY_LEFT_SHIFT;
const char sLA[]   PROGMEM = "LA";         const byte kLA    PROGMEM = KEY_LEFT_ALT;
const char sLG[]   PROGMEM = "LG";         const byte kLG    PROGMEM = KEY_LEFT_GUI;
const char sRC[]   PROGMEM = "RC";         const byte kRC    PROGMEM = KEY_RIGHT_CTRL;
const char sRS[]   PROGMEM = "RS";         const byte kRS    PROGMEM = KEY_RIGHT_SHIFT;
const char sRA[]   PROGMEM = "RA";         const byte kRA    PROGMEM = KEY_RIGHT_ALT;
const char sRG[]   PROGMEM = "RG";         const byte kRG    PROGMEM = KEY_RIGHT_GUI;
const char sF1[]   PROGMEM = "F1";         const byte kF1    PROGMEM = KEY_F1;
const char sF2[]   PROGMEM = "F2";         const byte kF2    PROGMEM = KEY_F2;
const char sESC[]  PROGMEM = "ESC";        const byte kESC   PROGMEM = KEY_ESC;
const char sHold[] PROGMEM = "HOLD";       const byte kHold  PROGMEM = 0xFF;
//const char s_[] PROGMEM = "";           const byte kc_ PROGMEM = ;
/*const char s_[] PROGMEM = "";           const byte kc_RG PROGMEM = ;
const char s_[] PROGMEM = "";           const byte kc_RG PROGMEM = ;
const char s_[] PROGMEM = "";           const byte kc_RG PROGMEM = ;
const char s_[] PROGMEM = "";           const byte kc_RG PROGMEM = ;
const char s_[] PROGMEM = "";           const byte kc_RG PROGMEM = ;
const char s_[] PROGMEM = "";           const byte kc_RG PROGMEM = ;
const char s_[] PROGMEM = "";           const byte kc_RG PROGMEM = ;
const char s_[] PROGMEM = "";           const byte kc_RG PROGMEM = ;
const char s_[] PROGMEM = "";           const byte kc_RG PROGMEM = ;
const char s_[] PROGMEM = "";           const byte kc_RG PROGMEM = ;
const char s_[] PROGMEM = "";           const byte kc_RG PROGMEM = ;
const char s_[] PROGMEM = "";           const byte kc_RG PROGMEM = ;*/
const char sEOL[] PROGMEM = "EOL";         const byte kEOL PROGMEM = 0;




const char* const stringTable[] PROGMEM = {sbs, scln, sLC, sLS, sLA, sLG, sRC, sRS, sRA, sRG, sF1, sF2, sESC, sHold, sEOL};
const byte keycodeTable[] PROGMEM =       {kbs, kcln, kLC, kLS, kLA, kLG, kRC, kRS, kRA, kRG, kF1, kF2, kESC, kHold, kEOL};