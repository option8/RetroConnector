/*

M0100 pins:

LPIN = D0
RPIN = D1


if LPIN & RPIN are HIGH or LPIN & RPIN are LOW

  if LPIN goes HIGH to LOW or LPIN goes LOW to HIGH
    
    moving right, X++
    
  if RPIN goes HIGH to LOW or RPIN goes LOW to HIGH

    moving left, X--
  
  /
/

Mouse.move(X, Y), where X and Y range from -127 to +127. Positive X moves to the right. Positive Y moves downwards. 

*/


int LPIN = 5;
int RPIN = 6;
int DPIN = 7;
int UPIN = 8;

//int PulseState = 1;

volatile int DeltaX = 0;
volatile int DeltaY = 0;

int MouseCalibration = 4; // how fast does the movement translate (3 - 6 seems okay, 10 is too fast) 


#include <Bounce.h>

Bounce button4 = Bounce(4, 10);



void setup() {                  
    pinMode(4, INPUT_PULLUP);
//  Serial.begin(38400);

  pinMode(LPIN, INPUT);
  pinMode(RPIN, INPUT);
  pinMode(DPIN, INPUT);
  pinMode(UPIN, INPUT);

      attachInterrupt(0, LPINChange, CHANGE);    
      attachInterrupt(1, RPINChange, CHANGE);    
      attachInterrupt(2, DPINChange, CHANGE);    
      attachInterrupt(3, UPINChange, CHANGE);    


}


void loop()                     
{
     
    if( (DeltaX != 0) || (DeltaY != 0) )  {
      Mouse.move(DeltaX * MouseCalibration, DeltaY * MouseCalibration);
      DeltaX = 0;
      DeltaY = 0;
    }
    
   // delay(15);
   
   
  button4.update();
  if (button4.fallingEdge()) {
    Mouse.set_buttons(1, 0, 0);
  }
  if (button4.risingEdge()) {
    Mouse.set_buttons(0, 0, 0);
  }
  
  
}
   
   
  
void LPINChange() 
{ 
  if (  digitalRead(LPIN) == digitalRead(RPIN) ) {
      DeltaX = -1;
    }
  //  Serial.println(".");
}

void RPINChange() 
{
  if (  digitalRead(LPIN) == digitalRead(RPIN) ) {
      DeltaX = 1;
  }
 //   Serial.println("-");
}

void DPINChange() 
{ 
  if (  digitalRead(DPIN) == digitalRead(UPIN) ) {
      DeltaY = 1;
    }
}

void UPINChange() 
{
  if (  digitalRead(DPIN) == digitalRead(UPIN) ) {
      DeltaY = -1;
  }
}




