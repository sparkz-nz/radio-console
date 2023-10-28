# Radio Console Control Board

## Radio Console Overview
The Radio Console project provides a configurable keyboard input device driven by a mix of rotary encoders and switches. The opening and closing of each switch, and CW or CCW rotation of each encoder, can be configured to send a keystroke as a usb HID keyboard device.

The electronics comprises three arduino boards:

- one Pro Micro - the Control Board  
this is used primarily for the usb HID capability. It has limited IO and so extends the IO channels by connecting to additonal Nano boards via I2C
- two Nanos - the IO Boards  
these contain the code to read the encoders and switches and report status to the control board over I2C

## Control Board Overview


## Serial Port CLI

A very basic command line interface is available over the serial port connection established by connecting by USB. Port settings are standard 9600-none-8-one.

Enter a command followed by Enter (CR, LF, or both) from the following list:

* `config` - enter config mode
* `load` - load saved configuration from eeprom (this is done automtatically on power-up)
* `save` - save current configuration to eeprom
* `dump` - output current configuration (note hex values are output instead of key macros)

These commands can be placed in a text file and sent to the serial port using a terminal program such as TeraTerm or Putty

## Configuration

Enter configuration mode with the command `config`

Each line starts with a mode number. Switches 1-5 are used to select the mode on the console, and depending on which mode is selected each switch can be configured to send different keystrokes.

Each line configures a switch or encoder with two keystroke definitions. The first keystroke is sent when the switch is pressed or the encoder is incremented, and the second is sent when the switch is released or the encoder is decremented. If the keystroke is not specified then nothing is sent when the key or encoder is activated.

Normally the keystroke is sent as a key-press immediately followed by a key-release (the equivalent of pressing an releasing a key on the keyboard). In order to 'hold down' a key during the time a switch is pressed, specify `\HOLD\` for the second keystroke. The first keystroke will be sent (but not released) when the key is pressed, and released when the switch is released. This is useful to emulate modifier keys such as shift or control on the keyboard. This is not applicable to encoders.

The syntax of each line is as follows:

`<mode>:<switch>:<keystroke-definition>[:<keystroke-definition>]`

Where

`<mode>:` is a number from 1 to 5 followed by a `:` - eg. `1:`

`<switch>: = {s|e}<n>:` is the switch or encoder number followed by a `:` - eg. `s13:` or `e4`

`<keystroke-definition> = {<key>|:}[<modifiers>..]`

`<key>` is a character from the keyboard or a key macro, or `:` for no character, eg. `a` or `=` or `\F2\` or `\ESC\`

`[<modifierss>..]` is an optional list of modifier keys as two character pairs, up to four pairs, eg. `LSLC`

* `LS` - Left Shift
* `LC` - Left Control
* `LA` - Left Alt
* `LG` - Left GUI (Windows key or Mac equivalent)
* `R..` - the right key equivalent of these



`mode>:{s|e}<n>:{<key>|:}[<mod>..][:{<key>|:}[<mod>..]]`

Each part of the line is as follows:

`<mode>:` - a number from 1 to 5 followed by a `:`

`{s|e}<n>:` - switch or encoder number followed by a `:`

`{<key>|:}` - a key code or a `:` if no key is to be pressed