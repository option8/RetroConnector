/*



CANT HAVE ANYTHING HOOKED UP TO PINS 3 OR 4 AND DO USB. FFFFUUUUUU


M0100 pins:


if LPIN & RPIN are HIGH or LPIN & RPIN are LOW

  if LPIN goes HIGH to LOW or LPIN goes LOW to HIGH
    
    moving right, X++
    
  if RPIN goes HIGH to LOW or RPIN goes LOW to HIGH

    moving left, X--
  
  /
/

Mouse.move(X, Y), where X and Y range from -127 to +127. Positive X moves to the right. Positive Y moves downwards. 

*/


// mouse is essentially two rotary encoders. one for L/R one for U/D

int LPIN = 0;
int RPIN = 4;
int led = 1; // blink 'digital' pin 1 - AKA the built in red LED

static uint8_t X_prev_pos = 0;
static uint8_t X_flags      = 0;


int DPIN = 2;
int UPIN = 3;

static uint8_t Y_prev_pos = 0;
static uint8_t Y_flags      = 0;

// using comparison to previous read to check direction instead of interrupts.

volatile int DeltaX = 0;
volatile int DeltaY = 0;

int MouseButtonPin = 1; 

boolean MouseButton = 0;

int MouseCalibration = 4; // how fast does the movement translate (3 - 6 seems okay, 10 is too fast) 

#include <TrinketMouse.h>
//#include <Bounce.h>

//Bounce button4 = Bounce(4, 10);



void setup() {                  
    pinMode(MouseButtonPin, INPUT_PULLUP);
  pinMode(led, OUTPUT);

  pinMode(LPIN, INPUT);
//  pinMode(RPIN, INPUT);
//  pinMode(UPIN, INPUT);
//  pinMode(DPIN, INPUT);

  digitalWrite(LPIN, HIGH);
//  digitalWrite(RPIN, HIGH);
//  digitalWrite(UPIN, HIGH);
//  digitalWrite(DPIN, HIGH);


      TrinketMouse.begin(); 

  // get an initial reading on the encoder pins
/*  if (digitalRead(LPIN) == LOW) {
    X_prev_pos |= (1 << 0);
    
  }
  if (digitalRead(RPIN) == LOW) {
    X_prev_pos |= (1 << 1);
  }

  if (digitalRead(UPIN) == LOW) {
    Y_prev_pos |= (1 << 0);
  }
  if (digitalRead(DPIN) == LOW) {
    Y_prev_pos |= (1 << 1);
  }
*/

}



void loop()
{

/*

  int8_t X_action = 0; // 1 or -1 if moved, sign is direction

  uint8_t X_cur_pos = 0;
  // read in the encoder state first


  if ( digitalRead(LPIN) ) {
    X_cur_pos |= (1 << 0);
  }
  if ( digitalRead(RPIN) ) {
    X_cur_pos |= (1 << 1);
  }

  // if any rotation at all
  if (X_cur_pos != X_prev_pos)
  {
    if (X_prev_pos == 0x00)
    {
      // this is the first edge
      if (X_cur_pos == 0x01) {
        X_flags |= (1 << 0);
      }
      else if (X_cur_pos == 0x02) {
        X_flags |= (1 << 1);
      }
    }

    if (X_cur_pos == 0x03)
    {
      // this is when the Xoder is in the middle of a "step"
      X_flags |= (1 << 4);
    }
    else if (X_cur_pos == 0x00)
    {
      // this is the final edge
      if (X_prev_pos == 0x02) {
        X_flags |= (1 << 2);
      }
      else if (X_prev_pos == 0x01) {
        X_flags |= (1 << 3);
      }

      // check the first and last edge
      // or maybe one edge is missing, if missing then require the middle state
      // this will reject bounces and false movements
      if (bit_is_set(X_flags, 0) && (bit_is_set(X_flags, 2) || bit_is_set(X_flags, 4))) {
        X_action = 1;
      }
      else if (bit_is_set(X_flags, 2) && (bit_is_set(X_flags, 0) || bit_is_set(X_flags, 4))) {
        X_action = 1;
      }
      else if (bit_is_set(X_flags, 1) && (bit_is_set(X_flags, 3) || bit_is_set(X_flags, 4))) {
        X_action = -1;
      }
      else if (bit_is_set(X_flags, 3) && (bit_is_set(X_flags, 1) || bit_is_set(X_flags, 4))) {
        X_action = -1;
      }

      X_flags = 0; // reset for next time
    }
  }

  X_prev_pos = X_cur_pos;


  DeltaX = X_action;


// now do Y...


    if( (DeltaX != 0) || (DeltaY != 0) )  {
      TrinketMouse.move(DeltaX * MouseCalibration, DeltaY * MouseCalibration, 0, MouseButton);
      DeltaX = 0;
      DeltaY = 0;
    }
*/

    digitalWrite(led, digitalRead(LPIN)); 
    TrinketMouse.move(1,0,0,0);


}


   
