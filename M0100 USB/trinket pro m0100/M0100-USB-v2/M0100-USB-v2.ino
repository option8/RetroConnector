 /*
 *  Converts Apple M0100 mouse to USB, using Adafruit Trinket Pro 5v. 
 * 
 *  Accelerated mouse tracking, based on speed. Faster movement
 * 
 * 
 * 
 * 
 */

 

#include <cmdline_defs.h>
#include <ProTrinketMouse.h>
#include <ProTrinketMouseC.h>
#include <usbconfig.h>
#include <Bounce2.h>


int LPIN = 9;
int RPIN = 10;
int DPIN = 4;
int UPIN = 5;

int CLICKPIN = A3;

int CALIBRATIONPIN = A5;

//int PulseState = 1;

volatile int DeltaX = 0;
volatile int DeltaY = 0;
volatile int MouseButtonDown = 0;



volatile int MouseCalibration;// = 5; // how fast does the movement translate. acceleration factor. HIGHER = FASTER

// mouse velocity = delta(axis)^2 * MouseCalibration

int PollingSpeed = 5; // how often does axis delta get computed, 5ms seems responsive. LOWER=FASTER POLLING, SLOWER MOVEMENT


Bounce MouseButtonDebounce = Bounce();



void setup() {                  
    pinMode(CLICKPIN, INPUT_PULLUP);
    MouseButtonDebounce.attach(CLICKPIN);
    MouseButtonDebounce.interval(1);

  pinMode(LPIN, INPUT);
  pinMode(RPIN, INPUT);
  pinMode(DPIN, INPUT);
  pinMode(UPIN, INPUT);

  enablePinInterupt(LPIN);
  enablePinInterupt(DPIN);

  pinMode(13, OUTPUT);

  TrinketMouse.begin();                 // initialize the mouse library

  pinMode(CALIBRATIONPIN, INPUT);
  
}



void loop() {

  MouseCalibration = map(analogRead(CALIBRATIONPIN), 0, 1023, 1, 10); // minimum 1
 
  MouseButtonDebounce.update();

  if (MouseButtonDebounce.fell()) {
    
    MouseButtonDown = 0x01;
  }
  if (MouseButtonDebounce.rose()) {
    MouseButtonDown = 0;
  }

  digitalWrite(13,MouseButtonDown);

  TrinketMouse.move(DeltaX * MouseCalibration * abs(DeltaX), DeltaY * MouseCalibration * abs(DeltaY), 0, MouseButtonDown);
    
  DeltaX = 0;
  DeltaY = 0;

    
  delay(PollingSpeed);

 
}





void enablePinInterupt(byte pin)
{
    *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));  // enable pin
    PCIFR  |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
    PCICR  |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
}



ISR (PCINT0_vect) // handle pin change interrupt for D8 to D13 here
{    
  
 if(digitalRead(LPIN)) {
  
    if(digitalRead(RPIN)) {
      
      DeltaX -= 1;
      
    } else {
      
      DeltaX += 1;
   
   }
 }
    
} 

ISR (PCINT1_vect) // handle pin change interrupt for A0 to A5 here // NAV0
{
  
}

ISR (PCINT2_vect) // handle pin change interrupt for D0 to D7 here // NAV1, NAV2
{
   if(digitalRead(DPIN)) {

  
    if(digitalRead(UPIN)) {

      DeltaY -= 1;
  
      
    } else {
      
      DeltaY += 1;
   
   }
 } 
 

}

