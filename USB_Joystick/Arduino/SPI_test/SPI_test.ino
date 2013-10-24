//--- SPI code
 
#define DATAOUT     11
#define SPICLOCK    13
#define SLAVESELECT 4
 
void SPIInitialize()
{
  byte clr;
 
  pinMode(DATAOUT, OUTPUT);
  pinMode(SPICLOCK,OUTPUT);
  pinMode(SLAVESELECT,OUTPUT);
 
  digitalWrite(SLAVESELECT,HIGH); //disable device
 
  SPCR = (1<<SPE)|(1<<MSTR);
  clr=SPSR;
  clr=SPDR;
  delay(10);
}
 
char SPITransfer(volatile char data)
{
  SPDR = data;                    // Start the transmission
  while (!(SPSR & (1<<SPIF)))     // Wait the end of the transmission
  {
  };
  return SPDR;                    // return the received byte
}
 
//--- MCP42100 code
 
byte SetPot(int address, int value)
{
  // Slave Select set low to allow commands
  digitalWrite(SLAVESELECT, LOW);
 
  // 2 byte command
  SPITransfer(0x10 + address);   // 0x10 = 'set pot' command
  SPITransfer(value);            // Value to set pot
 
  // Release chip, signal end transfer
  digitalWrite(SLAVESELECT, HIGH);
}
 
//--- Application code
 
void setup()
{
  SPIInitialize();          // Initialize the SPI interface
  SetPot(1,255);
  SetPot(0,255);
}
 
void loop()
{


  for (int level = 0; level < 255; level++) {
        SetPot(1,level);;
        SetPot(2,255 - level);;
      delay(100);
    }


}
