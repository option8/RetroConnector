/*
  Digital Pot Control
  
  Based on the original sketch for AD5206....
  
  This example controls a Microchip digital potentiometer.
  The MCP42 has 2 potentiometer channels. Each channel's pins are labeled
  PAx - connect this to voltage
  PWx - this is the pot's wiper, which changes when you set it
  PBx - connect this to ground.
 
 The MCP42 is SPI-compatible,and to command it, you send two bytes: 
 
 The first byte is the Command Byte which has this format when
 the next byte is to be data: XX01XXpp
 note these bits ...............^^....  the 01 means the next byte is data
 (where pp = potentiometer selection, X= don't care)
 pp= 00 = dummy code, no pot selected
 pp= 01 = pot0
 pp= 10 = pot1
 pp= 11 = both pots
 
 Simplest case is to have X= 0 so the Command Byte will be:
 pp= 00: 00010000 = 16 
 pp= 01: 00010001 = 17
 pp= 10: 00010010 = 18
 pp= 11: 00010011 = 19
 
 The second byte is the resistance value for the channel (0 - 255).  
 
 The circuit:
  * All PA pins of MCP42 connected to +5V
  * All PB pins of MCP42 connected to ground
  * An LED and a 220-ohm resisor in series connected from each PW pin to ground
  * CS - to digital pin 10  (SS pin)
  * SI - to digital pin 11 (MOSI pin)
  * SCK - to digital pin 13 (SCK pin)
 
 created 10 Aug 2010 
 by Tom Igoe
 
 Thanks to Heather Dewey-Hagborg for the original tutorial, 2005
 
 Version for MCP42xx April 2013, Jim Brown
 
*/


// include the SPI library:
#include <SPI.h>


// set pin 4 as the slave select for the digital pot:
const int slaveSelectPin = 4;

void setup() {
  // set the slaveSelectPin as an output:
  pinMode (slaveSelectPin, OUTPUT);
  // initialize SPI:
  SPI.begin(); 
}

void loop() {
  // go through pp=01, 10, 11  for pot0, pot1 and both together of the digital pot:
  for (int CommandByte = 17; CommandByte < 19; CommandByte++) {    // 17, 18 and 19
    // change the resistance on this pot from min to max:
    for (int level = 0; level < 255; level++) {
      digitalPotWrite(CommandByte, level);
      delay(10);
    }
    // wait at the top:
    delay(100);
    // change the resistance on this channel from max to min:
    for (int level = 0; level < 255; level++) {
      digitalPotWrite(CommandByte, 255 - level);
      delay(10);
    }
  }

}

int digitalPotWrite(int CommandByte, int value) {
  // take the SS pin low to select the chip:
  digitalWrite(slaveSelectPin,LOW);
  //  send in the address and value via SPI:
  SPI.transfer(CommandByte); 
  SPI.transfer(value);
  // take the SS pin high to de-select the chip:
  digitalWrite(slaveSelectPin,HIGH); 
}
