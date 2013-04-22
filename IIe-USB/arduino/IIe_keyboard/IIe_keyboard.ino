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

const byte ROWS = 10; // rows
const byte COLS = 8; // columns
char keys[ROWS][COLS] = {


{      KEY_ESC,     KEY_TAB,     KEY_A,       KEY_Z,  KEYPAD_SLASH , 0 ,  KEYPAD_ASTERIX,  KEY_ESC},

{      KEY_1,      KEY_Q,       KEY_D,       KEY_X,  0,  0,  0,  0},

{      KEY_2,      KEY_W,       KEY_S,       KEY_C,  KEYPAD_0,  KEYPAD_4,  KEYPAD_8,  0},

{      KEY_3,      KEY_E,       KEY_H,       KEY_V,  KEYPAD_1,  KEYPAD_5,  KEYPAD_9,  KEYPAD_MINUS},

{      KEY_4,      KEY_R,       KEY_F,       KEY_B,  KEYPAD_2,  KEYPAD_6,  KEYPAD_PERIOD,  KEYPAD_ENTER},

{      KEY_6,      KEY_Y,       KEY_G,       KEY_N,  KEYPAD_3,  KEYPAD_7,  KEYPAD_PLUS,  0},

{      KEY_5,      KEY_T,       KEY_J,       KEY_M,       KEY_BACKSLASH,     KEY_TILDE,      KEY_ENTER,  KEY_BACKSPACE},

{      KEY_7,      KEY_U,       KEY_K,       KEY_COMMA,      KEY_EQUAL,       KEY_P,      KEY_UP,       KEY_DOWN },

{      KEY_8,      KEY_I,       KEY_SEMICOLON,      KEY_PERIOD,      KEY_0,       KEY_LEFT_BRACE,     KEY_SPACE,   KEY_LEFT },

{      KEY_9,      KEY_O,       KEY_L,       KEY_SLASH,      KEY_MINUS,      KEY_RIGHT_BRACE,      KEY_QUOTE,      KEY_RIGHT },

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


const int  SHIFTPin = 21;    // the pin that the shift key is attached to
const int  CTRLPin = 5;    // the pin that the control key is attached to
const int  APPLEPin1 = 8;    // the pin that the open-apple key is attached to
const int  APPLEPin2 = 9;    // the pin that the closed-apple key is attached to
// these pins are special in that they are dis/connected to ground, instead of to a row/col
const int CAPSPin = 7;



char modifierKeys[4];


void setup(){

  pinMode(SHIFTPin, INPUT);
  pinMode(CTRLPin, INPUT);
  pinMode(APPLEPin1, INPUT);
  pinMode(APPLEPin2, INPUT);

digitalWrite(APPLEPin1, LOW);
digitalWrite(APPLEPin2, LOW);

digitalWrite(SHIFTPin, HIGH);
digitalWrite(CTRLPin, HIGH);
  
  pinMode(CAPSPin, INPUT);
  digitalWrite(CAPSPin, HIGH);

}

void loop()
{
//probably should be on an interrupt, to catch high->low transition 
// as it is, caps lock key needs to be pressed twice

char CAPSState = digitalRead(CAPSPin);
    if (CAPSState == LOW) {
      Keyboard.set_key6(KEY_CAPS_LOCK);
    } else {
      Keyboard.set_key6(0);
    }

   char SHIFTState = digitalRead(SHIFTPin);

    if (SHIFTState == LOW) {
      modifierKeys[0] = MODIFIERKEY_SHIFT;
      digitalWrite(SHIFTPin, HIGH);
    } else {
      digitalWrite(SHIFTPin, HIGH);
      modifierKeys[0] = 0;
    }

   char CTRLState = digitalRead(CTRLPin);

    if (CTRLState == LOW) {
      modifierKeys[1] = MODIFIERKEY_CTRL;
      digitalWrite(CTRLPin, HIGH);
    } else {
      modifierKeys[1] = 0;
      digitalWrite(CTRLPin, HIGH);
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
      digitalWrite(APPLEPin1, LOW);
    } else {
      modifierKeys[2] = 0;
      digitalWrite(APPLEPin1, LOW);
    }

    if (CAPPLEState == HIGH) {
      modifierKeys[3] =  MODIFIERKEY_ALT;
      digitalWrite(APPLEPin2, LOW);
    } else {
      modifierKeys[3] = 0;
      digitalWrite(APPLEPin2, LOW);
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
	Keyboard.set_key6(0);

	// Update keyboard keys to active values.
	if( KPD.key[0].kchar && ( KPD.key[0].kstate==PRESSED || KPD.key[0].kstate==HOLD ))
		Keyboard.set_key1( KPD.key[0].kchar );

	if( KPD.key[1].kchar && ( KPD.key[1].kstate==PRESSED || KPD.key[1].kstate==HOLD ))
		Keyboard.set_key2( KPD.key[1].kchar );

	if( KPD.key[2].kchar && ( KPD.key[2].kstate==PRESSED || KPD.key[2].kstate==HOLD ))
		Keyboard.set_key3( KPD.key[2].kchar );

	if( KPD.key[3].kchar && ( KPD.key[3].kstate==PRESSED || KPD.key[3].kstate==HOLD ))
		Keyboard.set_key4( KPD.key[3].kchar );

	if( KPD.key[4].kchar && ( KPD.key[4].kstate==PRESSED || KPD.key[4].kstate==HOLD ))
		Keyboard.set_key5( KPD.key[4].kchar );

	if( KPD.key[5].kchar && ( KPD.key[5].kstate==PRESSED || KPD.key[5].kstate==HOLD ))
		Keyboard.set_key6( KPD.key[5].kchar );

	Keyboard.send_now();
	Keyboard.set_modifier(0);

}




