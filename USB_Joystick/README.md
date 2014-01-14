_Tested with generic USB gamepad:_
http://www.amazon.com/gp/product/B002NQ4820


Plug and Play
=============

Plug one end of the included 16-pin cable into the GAME port on the logic board of your Apple II. If your computer doesn't have a 16 pin port (e.g. Apple //c) use the optional 16-pin to 9-pin adapter by plugging one end of the 16-pin cable into the adapter, and the adapter into the 9-pin paddles/joystick port on the back of your computer.

Plug the other end of the 16-pin cable into the port on the RetroConnector Joystick Interface.

The 16-pin ribbon cable will have an indicator on one edge as to which is pin #1. Be sure that this edge lines up with the pin labelled #1 on the RetroConnector Joystick Interface board, or look for the notch in the DIP16 port. This notch indicates the end where pins #1 and #16 are.

Plug your USB joystick or gamepad into the USB port on the RetroConnector Joystick Interface.


Calibration
=================================

This BASIC program is helpful in calibrating the RetroConnector Joystick Interface board. Type this in at the BASIC prompt on your Apple II:
    
    10 print pdl(0) " " pdl(1)
	20 goto 10
	
Then type *RUN* and hit enter.

While the resulting numbers scroll down the screen, use a small screwdriver to adjust the trim potentiometer (labelled "TRIM") so that the first two numbers are as close to 127 as possible. They may not be equal, so try to split the difference. 

Most Apple II programs allow for a certain "dead zone" in the middle of the joystick values, so it's not necessary to be precisely 127 at center.

To test buttons along with joystick calibration, the above program can be adjusted so:

    10 print pdl(0) " " pdl(1) " " peek(-16287) " " peek(-16286)
	20 goto 10

The last two values are button 1 and 2. When pressed, they should be *above* 127, when released, *below* 127.