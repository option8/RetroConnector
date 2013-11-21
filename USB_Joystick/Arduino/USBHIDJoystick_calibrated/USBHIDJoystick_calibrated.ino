

#include <SPI.h>

// set pin 4 as the slave select for the digital pot:
const int slaveSelectPin = A0;  //4;
//const float foo = .21;
const int TrimPin = A3;

const int Butt0Pin = A4;
const int Butt1Pin = A5;


#include <avr/pgmspace.h>

#include <avrpins.h>
#include <max3421e.h>
#include <usbhost.h>
#include <usb_ch9.h>
#include <Usb.h>
#include <usbhub.h>
#include <avr/pgmspace.h>
#include <address.h>
#include <hid.h>
#include <hiduniversal.h>

#include "hidjoystickrptparser.h"

#include <printhex.h>
#include <message.h>
#include <hexdump.h>
#include <parsetools.h>

USB                                             Usb;
USBHub                                          Hub(&Usb);
HIDUniversal                                    Hid(&Usb);
JoystickEvents                                  JoyEvents;
JoystickReportParser                            Joy(&JoyEvents);

void setup()
{
  // set the slaveSelectPin as an output:
  pinMode (slaveSelectPin, OUTPUT);
  

  pinMode (Butt0Pin, OUTPUT);
  pinMode (Butt1Pin, OUTPUT);

  
  // initialize SPI:
  SPI.begin(); 



  Serial.begin( 115200 );
  Serial.println("Start");

  if (Usb.Init() == -1)
      Serial.println("OSC did not start.");
      
  delay( 200 );

  if (!Hid.SetReportParser(0, &Joy))
      ErrorMessage<uint8_t>(PSTR("SetReportParser"), 1  ); 




}

void loop()
{
    Usb.Task();

    float foo = float(analogRead(TrimPin))/1023.000;

    
    int joyX = JoyEvents.X;
    int channel = 5;

      digitalPotWrite(channel, joyX);
      digitalPotWrite(channel - 1, joyX * foo);

    int joyY = JoyEvents.Y;
     channel = 3;

      digitalPotWrite(channel, joyY);
      digitalPotWrite(channel - 1, joyY * foo);


    digitalWrite(Butt0Pin,JoyEvents.Butt0);
    digitalWrite(Butt1Pin,JoyEvents.Butt1);
    
    


}



void digitalPotWrite(int address, int value) {
  // take the SS pin low to select the chip:
  digitalWrite(slaveSelectPin,LOW);
  //  send in the address and value via SPI:
  SPI.transfer(address);
  SPI.transfer(value);
  // take the SS pin high to de-select the chip:
  digitalWrite(slaveSelectPin,HIGH); 
}

