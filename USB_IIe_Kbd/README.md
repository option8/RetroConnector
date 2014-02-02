FAQ
====

http://mentalhygiene.com/retroconnector/#interface


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