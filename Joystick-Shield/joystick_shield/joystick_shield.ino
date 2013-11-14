/*
Thanks to Dagen Brock:

BASIC program to test joystick in Apple II emulators:

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


*/




/* Read Joystick
*/

  
  int XPin = A8;
  int YPin = A7;

  int Button0Pin = 9;
  int Button1Pin = 10;

int JoystickX;
int JoystickY;

int XMax = 0;
int YMax = 0;
int XMin = 1023;
int YMin = 1023;


/*

adjust CalibrationMax up if the range on your joystick is too restricted to calibrate

adjust down if your joystick "jumps" around a lot. 800 seems to be a pretty good spot for the sticks i've tested.

*/
int CalibrationMax = 800; // allows for weird "jumpy" joystick potentiometers. these things are 30 years old, after all.




 void setup() {
//  Serial.begin(9600);
  
  pinMode(Button0Pin, INPUT);
  pinMode(Button1Pin, INPUT);

  Joystick.useManualSend(true);
  
 }
  
 void loop() {

   
   /* auto calibrate:
   
   save X, Y. 
   compare them to historical high/low values.
   if higher/lower, reset highest/lowest variable
   
   highest == 1023
   lowest == 0
   midway between now == 512
   
   
   */
 JoystickX = 1023 - analogRead(XPin);  // needs to be inverted!
 JoystickY = 1023 - analogRead(YPin);
  
   /* auto calibrate:
   
   save X, Y. 
   compare them to historical high/low values.
   if higher/lower, reset highest/lowest variable
   
   highest == 1023
   lowest == 0
   midway between now == 512
   
   
   */
     // record the maximum sensor value
   if ((JoystickX < CalibrationMax) && (JoystickX > XMax)) {
     XMax = JoystickX;
   }

   // record the minimum sensor value
   if (JoystickX < XMin) {
     XMin = JoystickX;
   }
     // record the maximum sensor value
   if ((JoystickY < CalibrationMax) && (JoystickY > YMax)) {
     YMax = JoystickY;
   }

   // record the minimum sensor value
   if (JoystickY < YMin) {
     YMin = JoystickY;
   }
   
   
  JoystickX =  map(JoystickX, XMin, XMax, 0, 1023);
  JoystickY =  map(JoystickY, YMin, YMax, 0, 1023);
   

  Joystick.X(JoystickX);
  Joystick.Y(JoystickY);

  Joystick.button(1, digitalRead(Button0Pin));
  Joystick.button(2, digitalRead(Button1Pin));

  Joystick.send_now();

/*
  Serial.print(XMin);
  Serial.print(" -- ");
  Serial.print(JoystickX);
  Serial.print(" -- ");
  Serial.print(XMax);
  Serial.print(" ---- ");
  Serial.print(YMin);
  Serial.print(" -- ");
  Serial.print(JoystickY);
  Serial.print(" -- ");
  Serial.println(YMax);
*/


/*
  Serial.print(JoystickX);
  Serial.print(" -- ");
  Serial.print(JoystickY);
 
  Serial.print(" -- ");

  Serial.print(digitalRead(Button0Pin));
    Serial.print(" -- ");
  Serial.print(digitalRead(Button1Pin));
   Serial.println();

    delay(100);
*/
}
