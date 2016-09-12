/* requires keypad library by Mark Stanley, Alexander Brevig from:
http://www.arduino.cc/playground/Code/Keypad

Uses the PJRC Teensy++ 2.0 and Teensyduino libraries, specifically USB HID and Keyboard():
http://www.pjrc.com/teensy/teensyduino.html
*/

#include <Keypad.h>


/* 
Declares the matrix rows/cols of the Apple IIe keyboard. 

More information here:
http://apple2.info/wiki/index.php?title=Pinouts#Apple_.2F.2Fe_Motherboard_keyboard_connector

*/

// Prevent compiler warning about casts from unsigned ints to chars
#define C(x) ((char) (x))

const byte ROWS = 10; // rows
const byte COLS = 8; // columns
char keys[ROWS][COLS] = {

  
  { C(KEY_ESC), C(KEY_TAB), C(KEY_A), C(KEY_Z), C(KEYPAD_SLASH), 0, C(KEYPAD_ASTERIX), C(KEY_ESC)},
  
  { C(KEY_1), C(KEY_Q), C(KEY_D), C(KEY_X),  0,  0,  0,  0},
  
  { C(KEY_2), C(KEY_W), C(KEY_S), C(KEY_C), C(KEYPAD_0), C(KEYPAD_4), C(KEYPAD_8),  0},
  
  { C(KEY_3), C(KEY_E), C(KEY_H), C(KEY_V), C(KEYPAD_1), C(KEYPAD_5), C(KEYPAD_9), C(KEYPAD_MINUS)},
  
  { C(KEY_4), C(KEY_R), C(KEY_F), C(KEY_B), C(KEYPAD_2), C(KEYPAD_6), C(KEYPAD_PERIOD), C(KEYPAD_ENTER)},
  
  { C(KEY_6), C(KEY_Y), C(KEY_G), C(KEY_N), C(KEYPAD_3), C(KEYPAD_7), C(KEYPAD_PLUS),  0},
  
  { C(KEY_5), C(KEY_T), C(KEY_J), C(KEY_M), C(KEY_BACKSLASH), C(KEY_TILDE), C(KEY_ENTER), C(KEY_BACKSPACE)},
  
  { C(KEY_7), C(KEY_U), C(KEY_K), C(KEY_COMMA), C(KEY_EQUAL), C(KEY_P), C(KEY_UP), C(KEY_DOWN) },
  
  { C(KEY_8), C(KEY_I), C(KEY_SEMICOLON), C(KEY_PERIOD), C(KEY_0), C(KEY_LEFT_BRACE), C(KEY_SPACE), C(KEY_LEFT) },
  
  { C(KEY_9), C(KEY_O), C(KEY_L), C(KEY_SLASH), C(KEY_MINUS), C(KEY_RIGHT_BRACE), C(KEY_QUOTE), C(KEY_RIGHT) },

};


char Fkeys[2][10] = {
  {C(KEY_1), C(KEY_2), C(KEY_3), C(KEY_4), C(KEY_5), C(KEY_6), C(KEY_7), C(KEY_8), C(KEY_9), C(KEY_0)},
  {C(KEY_F1), C(KEY_F2), C(KEY_F3), C(KEY_F4), C(KEY_F5), C(KEY_F6), C(KEY_F7), C(KEY_F8), C(KEY_F9), C(KEY_F10)}
};




/*
        XO      X1      X2      X3  |   X4      X5      X6      X7
------------------------------------+--------------------------------
YO      ESC     TAB     A       Z   |   /       )       *       ESC
                                    |
Y1      1!      Q       D       X   |   DOWN    UP      LEFT    RIGHT
                                    |
Y2      20      W       S       C   |   0       4       8       (
                                    |
Y3      34      E       H       V   |   1       5       9       -
                                    |
Y4      4$      R       F       S   |   2       6       .       RETURN
                                    |
Y5      6"      Y       G       N   |   3       7       +       ,
                                    +----------------------------------
Y6      5%      T       J       M       \|      `~      RETURN  DELETE

Y7      7&      U       K       ,<      +=       P      UP       DOWN

Y8      8*      I       ;:      .>      0)       [{     SPACE   LEFT

Y9      9(      O       L       /?      -_       ]}      '"      RIGHT


The RetroConnector IIe maps the 26 Apple IIe pins to Teensy++ pins

IIe  Col/Row  Teensy
1     Y0        10
2     Y1	45
3     +5V       +5v
4     Y2	44
5     SW1/CAPL  9
6     Y3	43
7     SW0/OAPL  8
8     Y4	42
9     CAPLOCK*  7
10     Y5	41
11     CNTL*    5
12     Y8	40
13     GND      GND
14     X0	39
15     RESET*  4
16     X2      38
17     X7      3
18     X1	18
19     X5       2
20     X3	19
21     X4       1
22     Y9	20
23     Y6      0
24     SHFT*	21
25     Y7      27
26     X6	22

*/

byte rowPins[ROWS] = { // Y0 - Y9
  10, 45, 44, 43, 42, 41, 0, 27, 40, 20}; //connect to the row pinouts of the keypad

byte colPins[COLS] = { // X0 - X7
   39, 18, 38, 19, 1, 2, 22, 3}; //connect to the column pinouts of the keypad

Keypad KPD = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


// these pins are special in that they are dis/connected to ground, instead of to a row/col
const int  SHIFTPin = 21;    // the pin that the shift key is attached to
const int  CTRLPin = 5;    // the pin that the control key is attached to
const int  APPLEPin1 = 8;    // the pin that the open-apple key is attached to
const int  APPLEPin2 = 9;    // the pin that the closed-apple key is attached to
const int  CAPSPin = 7;

#define LED  6

uint16_t modifierKeys[4];


#define KEY_CAPS_UNLOCK  0

boolean resetCapsLock = false;  // Allows one caps unlock signal.
unsigned long dTime = 0;
boolean CAPSLock = false;  // Initialize this to a reasonable value.
   
boolean FKEYS = false; // used to set numbers to F-Key equivalent. currently tied to caps lock


void setup()
{
  pinMode(SHIFTPin, INPUT_PULLUP);
  digitalWrite(SHIFTPin, HIGH);

  pinMode(CTRLPin, INPUT_PULLUP);
  digitalWrite(CTRLPin, HIGH);

  pinMode(APPLEPin1, INPUT);
  digitalWrite(APPLEPin1, LOW);

  pinMode(APPLEPin2, INPUT);
  digitalWrite(APPLEPin2, LOW);

  pinMode(CAPSPin, INPUT_PULLUP);
  digitalWrite(CAPSPin, HIGH);
 
// DEBUG
//  Serial.begin(115200);
  pinMode(LED, OUTPUT);

  // This gives time for the keyboard to hook up to the PC.
  // Otherwise the caps lock state may be incorrect.
  delay(1000);

}


void loop()
{
//probably should be on an interrupt, to catch high->low transition 

  // Only do something if the pin is different from previous state.
  boolean newCaps = digitalRead(CAPSPin) ? false : true;

  if ( (CAPSLock != newCaps) && !resetCapsLock)
  {
    CAPSLock = newCaps;    // Remember new CAPSLock.
    Keyboard.set_key6((uint8_t) KEY_CAPS_LOCK);    // Send KEY_CAPS_LOCK.
    dTime = millis();                    // Reset delay timer.
    resetCapsLock = true;

//    Serial.print("Caps = ");
//    Serial.println(CAPSLock);

    // Turn on the LED for caps lock.
    digitalWrite(LED, CAPSLock ? HIGH : LOW);
  
  }

  if ( resetCapsLock && (millis()-dTime) > 10)
  {
    Keyboard.set_key6(KEY_CAPS_UNLOCK);
    resetCapsLock = false;
  }

  // If caps lock is set, then turn number keys into function keys.
  FKEYS = CAPSLock;

/*char CAPSLock = digitalRead(CAPSPin);
    if (CAPSLock == LOW) {
      Keyboard.set_key6(KEY_CAPS_LOCK);
    } else {
      Keyboard.set_key6(0);89
    }

  */
   char SHIFTState = digitalRead(SHIFTPin);

    if (SHIFTState == LOW) {
      modifierKeys[0] = MODIFIERKEY_SHIFT;
    } else {
      modifierKeys[0] = 0;
    }

   char CTRLState = digitalRead(CTRLPin);

    if (CTRLState == LOW) {
      modifierKeys[1] = MODIFIERKEY_CTRL;
    } else {
      modifierKeys[1] = 0;
    }

   char OAPPLEState = digitalRead(APPLEPin1);
   char CAPPLEState = digitalRead(APPLEPin2);

/*    if (OAPPLEState == HIGH) {
      modifierKeys[2] =  MODIFIERKEY_GUI;
    } else if (CAPPLEState == HIGH) {
      modifierKeys[2] =  MODIFIERKEY_GUI;
    } else {
      modifierKeys[2] = 0;
    }
      digitalWrite(APPLEPin1, LOW);
      digitalWrite(APPLEPin2, LOW);
*/


// *** NOW USING CLOSED APPLE AS ALT/OPTION
    if (OAPPLEState == HIGH) {
      modifierKeys[2] =  MODIFIERKEY_GUI;
    } else {
      modifierKeys[2] = 0;
    }

    if (CAPPLEState == HIGH) {
      modifierKeys[3] =  MODIFIERKEY_ALT;
    } else {
      modifierKeys[3] = 0;
    }


// to use the TILDE key as ALT/OPTION
/*    modifierKeys[3] = 0;
    if( KPD.isPressed(KEY_TILDE) ) {
      modifierKeys[3] = MODIFIERKEY_ALT;
    }
*/

// *** NOW USING CLOSED APPLE AS ALT/OPTION

  Keyboard.set_modifier( modifierKeys[0] | modifierKeys[1] | modifierKeys[2] | modifierKeys[3] );

  KPD.getKeys(); // Scan for all pressed keys. 6 Max, + 4 modifiers. Should be plenty, but can be extended to 10+

	// Set keyboard keys to default values.
	Keyboard.set_key1(0);
	Keyboard.set_key2(0);
	Keyboard.set_key3(0);
	Keyboard.set_key4(0);
	Keyboard.set_key5(0);
	//Keyboard.set_key6(0);

        
  /* based on suggestion from Craig Brooks <s.craig.brooks@gmail.com>
     uses CAPS LOCK to turn number keys into F-Key equivalent.
  */


	// Update keyboard keys to active values.
	if ( KPD.key[0].kchar && ( KPD.key[0].kstate==PRESSED || KPD.key[0].kstate==HOLD ))
	{
    //Serial.println(FKEYS);
 
    if (FKEYS)
    {
      // number keys 1 through 0 for f1 - f10
      if ((KPD.key[0].kchar >= 0x1E) &&  (KPD.key[0].kchar <= 0x27))
      {
        KPD.key[0].kchar += 0x1C;
//      Serial.println( KPD.key[0].kchar, HEX );
      }
      else if ( KPD.key[0].kchar == 0x2D || KPD.key[0].kchar == 0x2E )
      {
//      - and = for f11 and f12
        KPD.key[0].kchar += 0x17;
      }
    }
    
    Keyboard.set_key1( KPD.key[0].kchar );  
  }


	if ( KPD.key[1].kchar && ( KPD.key[1].kstate==PRESSED || KPD.key[1].kstate==HOLD ))
		Keyboard.set_key2( KPD.key[1].kchar );

	if( KPD.key[2].kchar && ( KPD.key[2].kstate==PRESSED || KPD.key[2].kstate==HOLD ))
		Keyboard.set_key3( KPD.key[2].kchar );

	if( KPD.key[3].kchar && ( KPD.key[3].kstate==PRESSED || KPD.key[3].kstate==HOLD ))
		Keyboard.set_key4( KPD.key[3].kchar );

	if( KPD.key[4].kchar && ( KPD.key[4].kstate==PRESSED || KPD.key[4].kstate==HOLD ))
		Keyboard.set_key5( KPD.key[4].kchar );

	//if( KPD.key[5].kchar && ( KPD.key[5].kstate==PRESSED || KPD.key[5].kstate==HOLD ))
	//	Keyboard.set_key6( KPD.key[5].kchar );


	Keyboard.send_now();
	Keyboard.set_modifier(0);

}




