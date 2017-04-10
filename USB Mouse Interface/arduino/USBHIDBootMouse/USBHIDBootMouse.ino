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
long MOVEMENTBASE = 50;
int MOVEMENTFACTORX = 0;
int MOVEMENTFACTORY = 0;
int MOVEMENTTHRESHOLD = 1;
int MOVEMENTDENOM = 12;

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

  //  XDELAY =  MOVEMENTBASE + (1 - (abs(mi->dX) / 255.000)) * MOVEMENTFACTOR;
  // YDELAY =  MOVEMENTBASE + (1 - (abs(mi->dY) / 127.000)) * MOVEMENTFACTOR;
  //  Serial.print("dx=");
  //    Serial.println(XDELAY);
  // Serial.print(" dy=");
  //   Serial.println(YDELAY);
  // delayMicroseconds(XDELAY);


  MOVEMENTFACTORX = 1 + (abs(mi->dX)) / MOVEMENTDENOM;
  //Serial.println(MOVEMENTFACTOR);
  for (int i = 0; i < MOVEMENTFACTORX; i++) {

    if (mi->dX > MOVEMENTTHRESHOLD) {

      digitalWrite(XMOVE, HIGH);
      delayMicroseconds(MOVEMENTBASE);
      digitalWrite(XDIR, HIGH);
      delayMicroseconds(MOVEMENTBASE);
      digitalWrite(XMOVE, LOW);
      delayMicroseconds(MOVEMENTBASE);
      digitalWrite(XDIR, LOW);

    }
    if (mi->dX < -1 * MOVEMENTTHRESHOLD) {
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

      digitalWrite(YMOVE, HIGH);
      delayMicroseconds(MOVEMENTBASE);
      digitalWrite(YDIR, HIGH);
      delayMicroseconds(MOVEMENTBASE);
      digitalWrite(YMOVE, LOW);
      delayMicroseconds(MOVEMENTBASE);
      digitalWrite(YDIR, LOW);

    }

    if (mi->dY < -1 * MOVEMENTTHRESHOLD) {
      digitalWrite(YDIR, HIGH);
      delayMicroseconds(MOVEMENTBASE);
      digitalWrite(YMOVE, HIGH);
      delayMicroseconds(MOVEMENTBASE);
      digitalWrite(YDIR, LOW);
      delayMicroseconds(MOVEMENTBASE);
      digitalWrite(YMOVE, LOW);

    }

  }
  /*
  XDIR = XMOVE = LOW

      if X > 0
        XMOVE HIGH
        delay
        XDIR HIGH
        delay
        XMOVE LOW
        delay
        XDIR LOW

      else if X < 0
        XDIR HIGH
        delay
        XMOVE HIGH
        delay
        XDIR LOW
        delay
        XMOVE LOW





  */



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
void MouseRptParser::OnRightButtonDown	(MOUSEINFO *mi)
{
  if (MOVEMENTTHRESHOLD == 1) {
    MOVEMENTTHRESHOLD = 0;
  } else {
    MOVEMENTTHRESHOLD = 1;
  }

  if (MOVEMENTDENOM == 12) {
    MOVEMENTDENOM = 8;
  } else {
    MOVEMENTDENOM = 12;
  }

  if (MOVEMENTBASE == 50) {
    MOVEMENTBASE = 100;
  } else {
    MOVEMENTBASE = 50;
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



  /* Serial.begin( 115200 );
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

