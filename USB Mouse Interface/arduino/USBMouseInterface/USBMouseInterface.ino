/*

  tracking "mode" switch on right mouse button. 

  Mac Plus: 4+
  Apple IIe (MousePaint): 6-8
  Apple IIc: ?


  Two options for emulating variable speed mouse movement: 
  
  1 - Change quadtrature wavelength/frequency based on movement speed (distance/time).
  2 - Multiply single movement by distance moved in fixed polling time.
  
  Opted for option 2 after 1 proved to be too touchy. May go back to 1 since the new hardware is more stable than prototype.
    
*/



#include <hidboot.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif



// pin assignments

int XMOVE = 3;
int XDIR = 1;
int YMOVE = 2;
int YDIR = 0;
int CLICKPIN = 4;

long XDELAY;
long YDELAY;

long MOVEMENTBASE = 20;  // "tracking speed" determines how quickly to update the quadrature square wave

int MOVEMENTFACTORX = 0;
int MOVEMENTFACTORY = 0;

int MOVEMENTTHRESHOLD = 0; // minimum movement required before triggering a movemouse action
int MOVEMENTDENOM = 8; // movement speed factor

class MouseRptParser : public MouseReportParser
{
  protected:
    void OnMouseMove	(MOUSEINFO *mi);
    void OnLeftButtonUp	(MOUSEINFO *mi);
    void OnLeftButtonDown	(MOUSEINFO *mi);
    void OnRightButtonUp	(MOUSEINFO *mi);
    void OnRightButtonDown	(MOUSEINFO *mi);
    void OnMiddleButtonUp	(MOUSEINFO *mi);
    void OnMiddleButtonDown	(MOUSEINFO *mi);
};
void MouseRptParser::OnMouseMove(MOUSEINFO *mi)
{

  MOVEMENTFACTORX = 1 + (abs(mi->dX)) / MOVEMENTDENOM; // repeat the MouseMove (movement distance)/(movement speed factor) times
  
  //Serial.println(MOVEMENTFACTOR);
  for (int i = 0; i < MOVEMENTFACTORX; i++) {

    if (mi->dX > MOVEMENTTHRESHOLD) { 
      /* if the mouse moved enough to trigger the move event. Zero threshold results in jittering, 
       *  higher thresholds mean slow movements aren't tracked at all.
      */

      delayMicroseconds(MOVEMENTBASE);
      digitalWrite(XMOVE, HIGH);
      delayMicroseconds(MOVEMENTBASE);
      digitalWrite(XDIR, HIGH);
      delayMicroseconds(MOVEMENTBASE);
      digitalWrite(XMOVE, LOW);
      delayMicroseconds(MOVEMENTBASE);
      digitalWrite(XDIR, LOW);
/*

This roughly equates to bitbanging a quadtrature square wave. It's ugly, but it works, ish.

See http://m0100.com/ for a quick explanation of how the M0100 mouse works, and what the host computer is expecting to see.

 .___.
_|   |___ X-MOVEMENT
   .___.
___|   |_ X-DIRECTION

*/
    }
    if (mi->dX < -1 * MOVEMENTTHRESHOLD) {
      delayMicroseconds(MOVEMENTBASE);
      digitalWrite(XDIR, HIGH);
      delayMicroseconds(MOVEMENTBASE);
      digitalWrite(XMOVE, HIGH);
      delayMicroseconds(MOVEMENTBASE);
      digitalWrite(XDIR, LOW);
      delayMicroseconds(MOVEMENTBASE);
      digitalWrite(XMOVE, LOW);

    }
  }
  MOVEMENTFACTORY = 1 + (abs(mi->dY)) / MOVEMENTDENOM;
  for (int i = 0; i < MOVEMENTFACTORY; i++) {
    if (mi->dY > MOVEMENTTHRESHOLD) {

      delayMicroseconds(MOVEMENTBASE);
      digitalWrite(YMOVE, HIGH);
      delayMicroseconds(MOVEMENTBASE);
      digitalWrite(YDIR, HIGH);
      delayMicroseconds(MOVEMENTBASE);
      digitalWrite(YMOVE, LOW);
      delayMicroseconds(MOVEMENTBASE);
      digitalWrite(YDIR, LOW);

    }

    if (mi->dY < -1 * MOVEMENTTHRESHOLD) {
      delayMicroseconds(MOVEMENTBASE);
      digitalWrite(YDIR, HIGH);
      delayMicroseconds(MOVEMENTBASE);
      digitalWrite(YMOVE, HIGH);
      delayMicroseconds(MOVEMENTBASE);
      digitalWrite(YDIR, LOW);
      delayMicroseconds(MOVEMENTBASE);
      digitalWrite(YMOVE, LOW);

    }

  }

};
void MouseRptParser::OnLeftButtonUp	(MOUSEINFO *mi)
{
  //  Serial.println("L Butt Up");
  digitalWrite(CLICKPIN, HIGH);
};
void MouseRptParser::OnLeftButtonDown	(MOUSEINFO *mi)
{
  //  Serial.println("L Butt Dn");
  digitalWrite(CLICKPIN, LOW);
};
void MouseRptParser::OnRightButtonUp	(MOUSEINFO *mi)
{
  //  Serial.println("R Butt Up");
};
void MouseRptParser::OnRightButtonDown	(MOUSEINFO *mi) // changes "tracking speed" variable on right click
{

  if (MOVEMENTBASE < 200) {
    MOVEMENTBASE += 20;
  } else {
    MOVEMENTBASE = 0;
  }

  //  Serial.println("R Butt Dn");
};
void MouseRptParser::OnMiddleButtonUp	(MOUSEINFO *mi)
{
  //  Serial.println("M Butt Up");
};
void MouseRptParser::OnMiddleButtonDown	(MOUSEINFO *mi)
{
  //  Serial.println("M Butt Dn");
};

USB     Usb;
USBHub     Hub(&Usb);
HIDBoot<HID_PROTOCOL_MOUSE>    HidMouse(&Usb);

uint32_t next_time;

MouseRptParser                               Prs;

void setup()
{

  pinMode(XMOVE, OUTPUT);
  pinMode(XDIR, OUTPUT);
  pinMode(YMOVE, OUTPUT);
  pinMode(YDIR, OUTPUT);
  pinMode(CLICKPIN, OUTPUT);



  /* SERIAL FOR DEBUGGING
 
 
 Serial.begin( 115200 );
  #if !defined(__MIPSEL__)
   while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
  #endif
   Serial.println("Start");
   
  */
  
  if (Usb.Init() == -1)
    Serial.println("OSC did not start.");

  delay( 200 );

  next_time = millis() + 5000;

  HidMouse.SetReportParser(0, (HIDReportParser*)&Prs);

  digitalWrite(XMOVE, LOW);
  digitalWrite(XDIR, LOW);
  digitalWrite(YMOVE, LOW);
  digitalWrite(YDIR, LOW);
  digitalWrite(CLICKPIN, HIGH);

}

void loop()
{
  Usb.Task();
}

