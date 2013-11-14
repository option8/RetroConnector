

// inslude the SPI library:
#include <SPI.h>


// set pin 10 as the slave select for the digital pot:
const int slaveSelectPin = 4;

void setup() {
  // set the slaveSelectPin as an output:
  pinMode (slaveSelectPin, OUTPUT);
  // initialize SPI:
  SPI.begin(); 
}

void loop() {

   int channel = 5;
   float foo = .21;



    delay(1000);


// change the resistance on this channel from min to max:
    for (int level = 0; level < 255; level++) {
      digitalPotWrite(channel, level);
      digitalPotWrite(channel - 1, level * foo);
      delay(10);
    }
    // wait a second at the top:
    delay(1000);
    // change the resistance on this channel from max to min:
    for (int level = 0; level < 255; level++) {
      digitalPotWrite(channel, 255 - level);
      digitalPotWrite(channel - 1, (255 - level) * foo);
      delay(10);
    }

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
