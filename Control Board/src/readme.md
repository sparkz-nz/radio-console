# Radio Console Control Board

## Radio Console Overview
The Radio Console project provides a configurable keyboard input device driven by a mix of rotary encoders and switches. The opening and closing of each switch, and CW or CCW rotation of each encoder, can be configured to send a keystroke as a usb HID keyboard device.

The electronics comprises three arduino boards:

- one Pro Micro - the Control Board  
this is used primarily for the usb HID capability. It has limited IO and so extends the IO channels by connecting to additonal Nano boards via I2C
- two Nanos - the IO Boards  
these contain the code to read the encoders and switches and report status to the control board over I2C

## Control Board Overview

