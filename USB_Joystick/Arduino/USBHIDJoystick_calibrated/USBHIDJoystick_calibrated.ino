

#include <SPI.h>

// set pin A0 as the slave select for the digital pot:
const int slaveSelectPin = A0;
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
    int channelX = 5;
    int channelY = 3;
    

    float calibrate = float(analogRead(TrimPin))/1023.000;

    // need to determine if x or x2 is giving values
    int joyX = JoyEvents.X;
    
    // need to determine if y or y2 is giving values
    int joyY = JoyEvents.Y;
 
 
    // write the values to the POTs
      digitalPotWrite(channelX, joyX * calibrate);
      digitalPotWrite(channelX - 1, joyX * calibrate);

      digitalPotWrite(channelY, joyY * calibrate);
      digitalPotWrite(channelY - 1, joyY * calibrate);


    // write the buttons
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

