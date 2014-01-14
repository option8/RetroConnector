_Uses the PJRC Teensy++ 2.0 and Teensyduino libraries, specifically USB HID:_
http://www.pjrc.com/teensy/teensyduino.html

Calibration
================================
Plug your Apple II joystick into the RetroConnector Joystick Shield, in either the 16-pin or 9-pin plug. If your joystick has a 16-pin plug, be sure the indicator on the cable for pin #1 (usually a red or blue line on the cable itself) lines up with pin #1 on the 16-pin port on the RetroConnector Joystick Shield.

Plug one end of a mini-USB cable into the RetroConnector Joystick Shield, and the other into a free USB port on your computer. 

Move the joystick up, down, left and right as far as it will go. Do this a few times. The RetroConnector Joystick Shield will automatically calibrate what it sends to the computer. You can see the calibrated values and test buttons in the Game Controllers control panel on a Windows PC, or download Joystick Show for Mac (https://itunes.apple.com/us/app/joystick-show/id515886877?mt=12 $.99 in the Mac App Store) 



BASIC program to test joystick in Apple II emulators:
================================

Copy and paste this into the BASIC prompt in an Apple II emulator (e.g. Virtual ][).
_Thanks to Dagen Brock. https://github.com/digarok_

```
5 DP = .10: REM Deadzone Percentage
10 XLEFT = 0:YUP = 0:XRIGHT = 279:YBOT = 159: REM Screen Bounds 
15 REM Set target box coordinates
20 HALF = XRIGHT / 2:D1 = INT (HALF - (HALF * DP)):D2 = INT (HALF + (HALF * DP))
25 HALF = YBOT / 2:D3 = INT (HALF - (HALF * DP)):D4 = INT (HALF + (HALF * DP))
30 HGR : HCOLOR= 3
35 REM Draw edge box
40 X1 = XLEFT:Y1 = YUP:X2 = XRIGHT:Y2 = YBOT: GOSUB 2000:X1 = X1 + 1:X2 = X2 - 1:Y1 = Y1 + 1:Y2 = Y2 - 1: GOSUB 2000
50 P0 = PDL (0):P1 = PDL (1): GOSUB 3000
55 PRINT P0,P1
60 GOTO 50
2000 HPLOT X1,Y1 TO X1,Y2: HPLOT X1,Y2 TO X2,Y2: HPLOT X2,Y2 TO X2,Y1: HPLOT X2,Y1 TO X1,Y1
2002 RETURN 
3000 X = INT (P0 * 274 / 255 + 2):Y = INT (P1 * 154 / 255 + 2)
3010 X1 = X:X2 = X + 1:Y1 = Y:Y2 = Y1 + 1: GOSUB 2000
3020 GOSUB 4000: HCOLOR= 0: GOSUB 2000: HCOLOR= 3
3050 RETURN 
4000 HPLOT D1,D3 TO D1,D4: HPLOT D1,D4 TO D2,D4: HPLOT D2,D4 TO D2,D3: HPLOT D2,D3 TO D1,D3
4010 RETURN : REM End Draw Target Box
```


