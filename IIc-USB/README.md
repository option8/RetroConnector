FAQ
===


###What is it?

The RetroConnector Shield is a breakout board for the PJRC Teensy Arduino clone that, along with the RetroConnector firmware, connects to an Apple II keyboard and turns it into a USB keyboard.

###What does it do?

The RetroConnector Shield turns the built-in keyboard of an Apple //c or //e into a standard USB keyboard that is recognized by any modern computer.

###What can I do with it?

Type on your iPad, Mac, PC or Raspberry Pi with a the 30 year old, super clicky keyboard of your youth. Raspberry Pi RetroConnector Demo

###Where do I buy one?

Currently available at http://retroconnector.com

###What do I need?

The full version, currently $45, requires only a working //e or //c keyboard and the computer you intend to plug it into. Even if all you have left of the Apple II is a keyboard and a ribbon cable, you’re all set.

The bare board, $18, requires the following parts/skills:

- Teensy ++ 2.0
- header pins
- female headers
- box header (34-pins for //c, 26-pins for //e)
- USB mini cable
- basic soldering


###Can I hack it?

Yes. All the software and hardware are open source. Everything you need is on github.

To update or modify the firmware on the Teensy, you will also need:

- RetroConnector firmware https://github.com/option8/retroconnector
- Arduino software http://arduino.cc/en/Main/Software
- teensyduino library http://www.pjrc.com/teensy/td_download.html
- keypad.h library http://playground.arduino.cc/Code/Keypad

Download and install the Arduino IDE software.

Download and install the Teensy Arduino libraries (Teensyduino).

Download the keypad.h library. Put the unzipped Keypad folder in “arduino\libraries\”.

Download the updated Arduino sketch for your RetroConnector (IIe version, IIc version) and open the .INO file with the Arduino IDE.
In the Arduino application, select Tools > Board > Teensy++ 2.0 and Tools > USB Type > Keyboard + Mouse + Joystick

Verify and upload the new sketch.

###How does it work?

The board is basically a breakout for the Teensy pins to connect to the proper pins on the Apple II keyboard connector. The rest is software, sending the right signal down the different pins based on keystrokes. The Teensy does all the heavy lifting, and includes the USB HID functionality out of the box.

###What model Apple IIs will these work with?

The //c version works with the Apple //c,and //c plus.

The //e version will work with all versions of the //e, including those with the extended keyboard, which adds a numeric keypad to the standard keyboard of earlier models.

###Will there be more models available?

If there is sufficient demand, a version for Commodore 64 or other vintage computers with built-in keyboards may be developed as well.

###Do lower-case characters function? I can’t remember if that was a hardware limitation or something to do w/ BASIC.

Upper and lower case character input works as you would expect from a modern keyboard. As do modifier keys like shift, control, and caps-lock. Since the Apple //e and //c lack alt keys, the RetroConnector firmware maps “closed apple” key to alt. The “open apple” key is the default “GUI” key (command on the Mac, Windows key on PC). These default mappings can be changed by editing the Arduino sketch running on the Teensy board attached to the shield.

###How about Function Keys and other special keys?

The Shift and Control keys are mapped to their modern counterparts. The left Apple key is Command/Windows key, the right is Alt.

To get Function keys (F1-F10) press CAPS-LOCK and the corresponding number key.

###My //c doesn’t work – does the keyboard (or Teensy for that matter) require it’s own power source? Regards.

All you need in working order from the //c is the keyboard and its ribbon cable. The RetroConnector and Teensy get power from the computer you hook it to via USB.

###Can the //c keyboard’s ‘disk use’ and ‘power’ LEDs be programmatically controlled via the RetroConnector? Conversely, can the keyboard LEDs be wired directly into the Pi’s status LED pads with the RetroConnector installed? Thanks for such a geeky-cool product!

To save space on the board, the RetroConnector Shield doesn’t make any connections to the //c Disk Activity LED. As for the power indictor, when power is applied, the power indicator LED should light up. It’s not an addressable pin on the Teensy, though, so it can’t be programmed in the current version. If you’re handy with a soldering iron, it would be an easy matter to connect a couple of lines to pin 11 (Disk Activity) and 25 (Power), and cut the current connection to pin 25. If it’s a popular feature, I’ll add those connections in to the next revision.

###Why?

My original, hand-wired version was for the G4 Apple //c project. I decided to make it available to see what other people would do with it.

###Thanks

Special thanks go to

- Fritzing.org for their excellent circuit layout software and the Fritzing Fab prototypying service. http://fritzing.org/
- David Schmidt (of ADTPro fame) for early testing, debugging and advice. http://adtpro.sourceforge.net/
