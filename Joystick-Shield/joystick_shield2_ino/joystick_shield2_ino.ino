/* Complete USB Joystick Example
   Teensy becomes a USB joystick with 16 or 32 buttons and 6 axis input

   You must select Joystick from the "Tools > USB Type" menu

   Pushbuttons should be connected between the digital pins and ground.
   Potentiometers should be connected to analog inputs 0 to 5.

   This example code is in the public domain.
*/

// Configure the number of buttons.  Be careful not
// to use a pin for both a digital button and analog
// axis.  The pullup resistor will interfere with
// the analog voltage.
const int numButtons = 2;  // 16 for Teensy, 32 for Teensy++


  int XPin = A7;
  int YPin = A8;

  int Button0Pin = 9;
  int Button1Pin = 10;

void setup() {
  // you can print to the serial monitor while the joystick is active!
  Serial.begin(9600);
  // configure the joystick to manual send mode.  This gives precise
  // control over when the computer receives updates, but it does
  // require you to manually call Joystick.send_now().
  Joystick.useManualSend(true);
  for (int i=Button0Pin; i<numButtons; i++) {
    pinMode(i, INPUT);
  }
  Serial.println("Begin Complete Joystick Test");
}

byte allButtons[numButtons];
byte prevButtons[numButtons];
int angle=0;

void loop() {
  // read 6 analog inputs and use them for the joystick axis
  Joystick.X(analogRead(XPin));
  Joystick.Y(analogRead(YPin));
//  Joystick.Z(analogRead(2));
//  Joystick.Zrotate(analogRead(3));
//  Joystick.sliderLeft(analogRead(4));
//  Joystick.sliderRight(analogRead(5));
  
  // read digital pins and use them for the buttons
  for (int i=Button0Pin; i<numButtons; i++) {
    if (digitalRead(i)) {
      // when a pin reads high, the button is not pressed
      // the pullup resistor creates the "on" signal
      allButtons[i] = 0;
    } else {
      // when a pin reads low, the button is connecting to ground.
      allButtons[i] = 1;
    }
    Joystick.button(i + 1, allButtons[i]);
  }


/*
  // make the hat switch automatically move in a circle
  angle = angle + 1;
  if (angle >= 360) angle = 0;
  Joystick.hat(angle);
*/  
  // Because setup configured the Joystick manual send,
  // the computer does not see any of the changes yet.
  // This send_now() transmits everything all at once.
  Joystick.send_now();
  
  // check to see if any button changed since last time
  boolean anyChange = false;
  for (int i=Button0Pin; i<numButtons; i++) {
    if (allButtons[i] != prevButtons[i]) anyChange = true;
    prevButtons[i] = allButtons[i];
  }
  
  // if any button changed, print them to the serial monitor
  if (anyChange) {
    Serial.print("Buttons: ");
    for (int i=0; i<numButtons; i++) {
      Serial.print(allButtons[i], DEC);
    }
    Serial.println();
  }
  
  // a brief delay, so this runs "only" 200 times per second
  delay(5);
}

