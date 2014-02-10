FAQ
====

###What is it?

The RetroConnector USB Interface is an adapter designed to fit inside the Apple //e, modifying it to enable connecting a USB keyboard.

###What does it do?

The RetroConnector USB Interface connects to the keyboard connector on the //e logic board using the //e’s existing keyboard cable, and acts as a USB host for a keyboard. Onboard the RetroConnector board is the Circuits@Home.com USB Host Shield for Arduino Pro Mini and an Atmel ATMega328, which translate USB keyboard keystrokes into keyboard matrix connections the //e keyboard ROM understands to produce keystrokes on the //e.

###What can I do with it?

Connect an inexpensive USB keyboard to your //e (or IIgs, see below) to replace a damaged or missing //e keyboard. Perhaps you prefer to type on an ergonomic keyboard, on a flat surface, or pull the keyboard away from the display for comfort. Those are all things you can’t do with the built-in keyboard of the //e, but can do easily with the USB Interface board.

###Where do I buy one?

Currently available at http://retroconnector.com

###What do I need?

A working Apple //e and its keyboard ribbon cable. Replacement cables will be available on my Tindie store shortly.
A USB keyboard.

###Does it take up a slot in the IIe?

No. The Interface card will mount into the IIe in an unused slot, but it doesn’t require any connections from the logic board except the ribbon cable. The expansion slot holds the card in place, but it will work just as well outside the case, as long as the keyboard ribbon cable reaches.

_Note: Repeated insertion and removal of the board into a slot may scrape off the solder mask on the RetroConnector. This could expose copper fill beneath to the pins on the logic board. If you plan on using your RetroConnector in a slot on your Apple IIe, be sure to check the board before inserting it for exposed copper. If you see any, put tape or something else non-conductive on that part of the board before inserting it into your IIe again._

###Will it work with an Apple USB keyboard?

Yes! (Firmware updated September 30, 2013).

###Will it work with a wireless keyboard?

Yes. I have tested with a couple of USB wireless keyboards, the type with a small dongle that attaches to the USB port. They worked without any problems.

###Can I hack it?

Yes. All the software and hardware are open source. Everything you need is on github.

To update or modify the firmware on the ATMega328, you will also need:

- RetroConnector firmware
- Arduino software
- circuits@home code library
- An AVR programmer

###What models of Apple II will this work with?

The RetroConnector USB Interface will work in any model IIe.

I am still testing with the IIgs via the keyboard port available on some logic boards, intended to be used as upgrades from IIe to IIgs. There are some differences in the IIe and GS with regard to the keyboard passing the self-test at startup. Once those are worked out, I will either release a new version of the Interface board, or publish the necessary modifications to make the current revision work.

###How do I do CTRL+Open-Apple+RESET?

There’s no “reset” button on modern keyboards, so that has been mapped to F12.

The two Apple keys have been mapped to the ALT keys – left is Open-Apple. Right is Closed-Apple.

So: Press F12 to do CTRL+RESET, left ALT+F12 to do CTRL+Open-Apple+RESET.

###Why?

There are many Apple IIs on ebay or other sites with broken keyboards or missing keys, with no way to connect an external keyboard. Now, instead of searching for a replacement, or hunting down key caps to get up and running, all you need is a cheap USB keyboard.

###Thanks

Special thanks go to

- Fritzing.org for their excellent circuit layout software and the Fritzing Fab prototypying service.
- David Schmidt (of ADTPro fame) for early testing, debugging and advice.





Function Keys
=============

The F12 key sends CTRL+Reset.


Macros
======

There are currently three handy macros included in the Keyboard Interface firmware. The are triggered by the F-keys F1 through F3.


_F1: enter the Monitor_
    CALL -151 
    
_F2: WeaknessPoint Minus by Martin Haye_
    8:20 35 FD 20 ED FD 4C 8 0 8G
    
This is Martin's minimal presentation interface (i.e. "Not PowerPoint")

To clear the screen and begin typing, hit ESC, then @.

To navigate the screen, hit ESC, then I,J,K, or M. Then ESC again to edit.    

    
_F3: Calibrate joysticks_
    10 print pdl(0) " " pdl(1) " " peek(-16287) " " peek(-16286) : goto 10
    
Center on your joystick should read (approximately) 127 127.

The third and fourth numbers in the display are the joystick buttons. Pressed, they should read higher than 128, and lower than 128 when not pressed.
    
    

Hardware Buttons
================

There are two switches on the RetroConnector Keyboard Interface board. They are labelled "Apple" and "CTRL+RESET"

Pressing the "CTRL+RESET" switch sends CTRL+Reset to reset the system. Hold the "Apple" switch while pressing "CTRL+RESET" will reboot the computer.