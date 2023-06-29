/*
Octave 	Note
        C	C#	D	D#	E	F	F#	G	G#	A	A#	B
0	0	1	2	3       4	5	6	7	8	9	10	11
1	12	13	14	15	16	17	18	19	20	21	22	23
2	24	25	26	27	28	29	30	31	32	33	34	35
3	36	37	38	39	40	41	42	43	44	45	46	47
4	48	49	50	51	52	53	54	55	56	57	58	59
5	60	61	62	63	64	65	66	67	68	69	70	71
6	72	73	74	75	76	77	78	79	80	81	82	83
7	84	85	86	87	88	89	90	91	92	93	94	95
8	96	97	98	99	100	101	102	103	104	105	106	107
9	108	109	110	111	112	113	114	115	116	117	118	119
10	120	121	122	123	124	125	126	127


A2.DMS
        C	C#	D	D#	E	F	F#	G	G#	A	A#	B



48       z      s       x       d      c        v      g        b       h       n       j        m      
60       ,      l       .       ;      /


                                                                        ESC      TAB    1        q  
60       w      3       e       4       r       t       6       y        7       u      8        i
72       o      0       p       -      [        ]       DEL     \



in A2DMS "w" is the same note as ","


IIe keyboard matrix

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
Y4      4$      R       F       B   |   2       6       .       RETURN
                                    |
Y5      6"      Y       G       N   |   3       7       +       ,
                                    +----------------------------------
Y6      5%      T       J       M       \|      `~      RETURN  DELETE

Y7      7&      U       K       ,<      +=       P      UP       DOWN

Y8      8*      I       ;:      .>      0)       [{     SPACE   LEFT

Y9      9(      O       L       /?      -_       ]}      '"      RIGHT



IIe  Col/Row  
1     Y0        
2     Y1	
3     +5V       
4     Y2	
5     SW1/SAPL*  
6     Y3	
7     SW0/OAPL*  
8     Y4	
9     CAPLOCK*  
10     Y5	
11     CNTL*    
12     Y8	
13     GND      
14     X0	
15     RESET*   
16     X2     
17     X7     
18     X1	
19     X5      
20     X3	
21     X4      
22     Y9	
23     Y6      
24     SHFT*	
25     Y7      
26     X6






*/

#define HWSERIAL Serial1

  
  // change on MIDI channel 1-8 = DMS, 9 = drum
  int MIDI_CHANNEL = 1;
  /*
  1 - piano
  2 - Bass
  3 - Banjo
  4 - Guitar
  5 - Clarinet
  6 - Trumpet
  7 - Square Wave
  8 - Sawtooth
  
  9 - switch to "drum" mode for DMS Drummer
  */

  boolean SERIAL_MODE = true ;
  // serial or keyboard control?


  int ledPin = 11; //teensy status LED

  int EXIT_PIN = 12; // press for "X" to return to keyboard control.

  int ENABLE_PIN =   0;
  int S0_PIN = 3;
  int S1_PIN = 4;
  int S2_PIN = 2;
  int S3_PIN = 1;

// 4051 pins
  int S0_4051 = 5;
  int S1_4051 = 6;
  int S2_4051 = 7;

  int SHIFT_PIN = 14;
  int CONTROL_PIN = 15;
  int OPEN_APPLE_PIN = 8;
  int CLOSED_APPLE_PIN = 16;
  int CAPS_LOCK_PIN = 17;
  int RESET_PIN = 1;

  int r0;
  int r1;
  int r2;

  int c0;
  int c1;
  int c2;
  int c3;
  
  int count= 0;
  
  int CAPS_LOCK_ACTIVE = 0;
  
  
  const int COLUMNS = 8;
  const int ROWS = 10;
  

  uint32_t next_time;


  int octave = 3; // W key == MIDI 60 == middle C
  int RangeC = 48; // initial range is 1 octave down (_C)
  int RangeG = 79; // to 1.5 octaves up (^G)


  volatile int CURRENT_NOTE; // store the currently playing note, to check if it's ending.
  volatile long NOTE_OFF_TIMER;
  int NOTE_OFF_INTERVAL = 5;


// note integer to QWERTY translation

int KEYS_ARRAY[ROWS][COLUMNS] = {
{  56,  57,  -1,  48,  64,  -1,  -1,  56 },
{  58,  59,  51,  50,  -1,  -1,  -1,  -1 },
{  -1,  60,  49,  52,  73,  63,  70,  -1 },
{  61,  62,  56,  53,  58,  -1,  -1,  75 },
{  63,  64,  -1,  55,  -1,  66,  62,  -1 }, 
{  66,  67,  54,  57,  61,  68,  -1,  60 },
{  -1,  65,  58,  59,  79,  -1,  -1,  78 },
{  68,  69,  -1,  60,  -1,  74,  -1,  -1 },
{  70,  71,  63,  62,  73,  76,  -1,  -1 },
{  -1,  72,  61,  64,  75,  77,  -1,  -1 }
};

/*
A2.DMS
        C	C#	D	D#	E	F	F#	G	G#	A	A#	B

 48      z      s       x       d      c        v      g        b       h       n       j        m     59 
 60     ,      l       .       ;      /


                                                                        ESC      TAB    1        q    59
 60     w      3       e       4       r       t       6       y        7       u      8        i     71
 72     o      0       p        -      [        ]       DEL     \






*/

int CONTROLS_ARRAY[ROWS][COLUMNS] = {

  {  -1,    -1,  -1,  -1,   -1,  -1,   -1,  -1 },
  {   49,   -1,  -1,  50,   -1,  -1,   -1,  -1 },
  {   50,   -1,  -1,  -1,   -1,  -1,   -1,  -1 },
  {   51,   -1,  -1,  -1,   -1,  -1,   -1,  -1 },
  {   52,   -1,  -1,  -1,   -1,  -1,   -1,  -1 },
  {   54,   -1,  -1,  -1,   -1,  -1,   -1,  -1 },
  {   53,   -1,  -1,  -1,   -1,  -1,   -1,  -1 },
  {   55,   -1,  107,  -1,   -1,  -1,   48,  57 },
  {   56,   -1,  -1,  -1,   -1,  -1,   -1,  -1 },
  {  -1,   -1,  -1,  -1,   -1,  -1,   -1,  -1 }

};


// Shift 0 - 8 = instrument change, 
// shift + 9 = down octave
// shift + 0 = up octave





int DRUMKIT_ARRAY[ROWS][COLUMNS] = {
  {  -1,   -1,  60,  48,   -1,  -1,   -1,  -1 },
  {   84,   72,  64,  50,  -1,   -1,   -1,  -1 },
  {   86,   74,  62,  52,   -1,  -1,   -1,  -1 },
  {   88,   76,  69,  53,   -1,  -1,   -1,  -1 },
  {   89,   77,  65,  55,   -1,  -1,   -1,  -1 },
  {   93,   81,  67,  57,   -1,  -1,   -1,  -1 },
  {   91,   79,  71,  59,   -1,  -1,   -1,  -1 },
  {   95,   83,  61,  49,   -1,  -1,   -1,  -1 },
  {   85,   73,  -1,  -1,   -1,  -1,   -1,  -1 },
  {  -1,   -1,  -1,  -1,   -1,  -1,   -1,  -1 }
};

/* DMS DRUMMER


     Kick    Laser   Rimshot         Clap   Snare            Tom            Closed Hat      Open Hat
        C	C#	D	D#	E	F	F#	G	G#	A	A#	B

48      z       ,       x               c       v               b               n               m    59

60      a       k       s               d       f               g               h               j    71

72      q       i       w               e       r               t               y               u    83

84      1       8       2               3       4               5               6               7    95




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
Y4      4$      R       F       B   |   2       6       .       RETURN
                                    |
Y5      6"      Y       G       N   |   3       7       +       ,
                                    +----------------------------------
Y6      5%      T       J       M       \|      `~      RETURN  DELETE

Y7      7&      U       K       ,<      +=       P      UP       DOWN

Y8      8*      I       ;:      .>      0)       [{     SPACE   LEFT

Y9      9(      O       L       /?      -_       ]}      '"      RIGHT



*/

int KEYMAP[81]= { 0x00,
0x21,0x40,0x23,0x24,0x25,0x5e,0x26,0x2a,0x0B,0x0A,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x7a,      0x73,       0x78,      0x64,     0x63,       0x76,      0x67,       0x62,       0x68,     0x6e,      0x6a,        0x6d,
0x77,      0x33,       0x65,      0x34,      0x72,       0x74,      0x36,       0x79,       0x37,      0x75,      0x38,        0x69, 
0x6f,      0x30,       0x70,      0x2d,      0x5b,       0x5d,       0x7f,     0x5c };



int DRUMMAP[96]= { 0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x7A, 0x2C, 0x78, 0x00, 0x63, 0x76, 0x00, 0x62, 0x00, 0x6E, 0x00, 0x6D, 
0x61, 0x6B, 0x73, 0x00, 0x64, 0x66, 0x00, 0x67, 0x00, 0x68, 0x00, 0x6A, 
0x71, 0x69, 0x77, 0x00, 0x65, 0x72, 0x00, 0x74, 0x00, 0x79, 0x00, 0x75, 

0x31, 0x38, 0x32, 0x00, 0x33, 0x34, 0x00, 0x35, 0x00, 0x36, 0x00, 0x37  };

// read MIDI

/*

specials:
KEYMAP[9] = 0x0A = down arrow
KEYMAP[10] = 0x0B = up arrow


*/


void OnNoteOn(byte channel, byte note, byte velocity)
{ 
    CURRENT_NOTE = note;
     if(SERIAL_MODE) { 
        NOTE_OFF_TIMER = 0;
     }
  
  digitalWrite(ledPin, HIGH);
  digitalWrite(SHIFT_PIN, HIGH);


   if(channel != MIDI_CHANNEL) {  // switched channels
    
      Serial.print("Channel: ");
      Serial.println(channel);
    
    
       MIDI_CHANNEL = channel;
       
     if(SERIAL_MODE) { 
        HWSERIAL.write(char(KEYMAP[channel]));
        Serial.println(char(KEYMAP[channel]));

      } else {
       sendControl(channel + 48);
     }
       
   }  
 
 
 
     Serial.print("DN ");
     Serial.println(note);
    
    
    
    
    //  Serial.println(note);
      
      /*
      note returns as integer 0 to 127
      
      
      lookup note in A2.DMS keymap - returns column/row
      
    
    "top" octave  
      0  24 - 35
      1  36 - 47
      2  48 - 59
      3  60 - 71
      4  72 - 83
      5  84 - 95
      
      
      
      
      bottom C to top G:
      
      0  12 - 43
      1  24 - 55
      2  36 - 67
      3  48 - 79
      4  60 - 91
      5  72 - 103
      
      
      
      
     RangeC = (octave + 1) * 12
     RangeG = RangeC + 31 
      
      
      noteOctave = floor( note/12 ) - 2
      
      do octaveSWITCH( octave - noteOctave )
    
      */
    
    
      int SEARCH_COLUMN = -1;
      int SEARCH_ROW = -1;
    
      
      if( MIDI_CHANNEL > 8 ) {  // DMS Drummer drum kit
     
     
         if(SERIAL_MODE) {
           
              HWSERIAL.write(DRUMMAP[note]);
              Serial.println(DRUMMAP[note], BYTE);
           
         } else {
     
             for (int row=0; row<ROWS; row++) {
                
                for (int column=0; column<COLUMNS; column++) {
                   if (note == DRUMKIT_ARRAY[row][column]) {
                     SEARCH_COLUMN = row;
                     SEARCH_ROW = column;
                     
                     Serial.print(SEARCH_COLUMN);
                     Serial.print(" : ");
                     Serial.println(SEARCH_ROW);
                     
                     break;
                   }
                }        
            }  // for rows
            
            SendKeyStroke(SEARCH_ROW, SEARCH_COLUMN, note);

         } // serial mode
     
     
      
      } else {  // A2 DMS MIDI piano
      
            
           RangeC = (octave + 1) * 12;
           RangeG = RangeC + 31;
           
            
           if (note > RangeG) {
             octaveSwitch(1);
           } else if (note < RangeC) {
             octaveSwitch(-1);
           }
           
     
      
      
    /*  int  noteOctave = floor( note/12 ) - 2;
      
      if (noteOctave != octave) {
        octaveSwitch(noteOctave - octave);
        delay(100);
      } 
    
    
    */
    
    
      
      /*
      if ( note >= (octave + 3) * 12 ) { // higher than current range ( normal == octave 3 )
       
         SendKeyStroke(6,7); // up key
         octave ++;    
         Serial.println(octave);
       delay(10);
         
      } else if ( note < (octave + 2) * 12 ) {
      
    
         SendKeyStroke(7,7); // down key
         octave -= 1;    
         Serial.println(octave);
       delay(10);
       
      } //else {    
        */
        
    
        
        // transpose note for octave switch
        
            note = note - 12 * (octave - 3);
            
            Serial.println(note);
           
      
          
            
            for (int row=0; row<ROWS; row++) {
                
                for (int column=0; column<COLUMNS; column++) {
                   if (note == KEYS_ARRAY[row][column]) {
                     SEARCH_COLUMN = row;
                     SEARCH_ROW = column;
                     break;
                   }
                }        
            }  // for rows
    
    
        SendKeyStroke(SEARCH_ROW, SEARCH_COLUMN, note);
      }  // if Drum/piano
    
    
    

          
} //OnNoteOn

  
  
void octaveSwitch(int diff) {
  
  Serial.print("OCTAVE DIFF = ");
  Serial.println(diff);
  
  for (int i=0; i< abs(diff); i++ ) {
 
      if(diff > 0 ) {
             SendKeyStroke(6,7,9); // up key
             octave ++;   
             Serial.print("OCTAVE ") ;
             Serial.println(octave);
             delay(20);
             SendKeyUp();
      } else {
             SendKeyStroke(7,7,10); // down key
             octave -= 1;    
             Serial.print("OCTAVE ") ;
             Serial.println(octave);
             delay(20);
             SendKeyUp();
      } 
    
  }
  
}
  
  
  
void SendKeyStroke ( int keyRow, int keyCol, char note) {

  
  if(SERIAL_MODE) {
      HWSERIAL.write(KEYMAP[note]);
      
      Serial.println(KEYMAP[note], BYTE);
      
  } else {
  
    digitalWrite(S0_4051, LOW);
    digitalWrite(S1_4051, LOW);
    digitalWrite(S2_4051, LOW);

  
//select the column bits  
    c0 = bitRead(keyCol,0);        
    c1 = bitRead(keyCol,1);       
    c2 = bitRead(keyCol,2);   
    c3 = bitRead(keyCol,3);     

// set the column signals to match selected column
    digitalWrite(S0_PIN, c0);
    digitalWrite(S1_PIN, c1);
    digitalWrite(S2_PIN, c2);
    digitalWrite(S3_PIN, c3);

//select the row bits  
    r0 = bitRead(keyRow,0);        
    r1 = bitRead(keyRow,1);       
    r2 = bitRead(keyRow,2);   

// set the column signals to match selected column
    digitalWrite(S0_4051, r0);
    digitalWrite(S1_4051, r1);
    digitalWrite(S2_4051, r2);

// enable the common IO
    digitalWrite(ENABLE_PIN, LOW);
  }




}  
  
  
void OnControlChange(byte channel, byte control, byte value) {
  Serial.print( "CONTROL ");
  Serial.print(control);
  Serial.print( " : ");
  Serial.println(value);
  
  if (control == 123) { // "all notes off"

         
       if(SERIAL_MODE) {
         HWSERIAL.write("+");
       } else {
         
        SendKeyUp();
        delay(10);
        
        SendKeyUp();
        delay(10);
        
        SendKeyUp();
        delay(10);
       }


      if(channel < 9) {  // to reset to default octave, down to 0, then up to 3
   
        octaveSwitch(-1);
        delay(10);
        octaveSwitch(-1);
        delay(10);
        octaveSwitch(-1);
        delay(10);
        octaveSwitch(-1);
        delay(10);
        octaveSwitch(-1);
        delay(10);
        octaveSwitch(-1);
        delay(10);
        octaveSwitch(1);
        delay(10);
        octaveSwitch(1);
        delay(10);
        octaveSwitch(1);
   // octave == 3
        octave = 3; 


     }
  }
  
}

void OnProgramChange(byte channel, byte program) {
  Serial.print( "PROGRAM ");
  Serial.print(program);
}


  
  

void OnNoteOff(byte channel, byte note, byte velocity)
{
   digitalWrite(ledPin, LOW);
   
   
   if (MIDI_CHANNEL < 9) {
  
    if( (SERIAL_MODE) && (CURRENT_NOTE == note) && (Serial.available()==false) ) {
  
  // start loop timer
          NOTE_OFF_TIMER = millis();
  
  
     } else if( (SERIAL_MODE == false) && (CURRENT_NOTE == note) ) {
  
     Serial.print("UP ");
     Serial.println(note);
     
      SendKeyUp();  
     }

   }
   
} //OnNoteOff



void SendKeyUp() {
   // disable the common IO 
   digitalWrite(ENABLE_PIN, HIGH);
   digitalWrite(SHIFT_PIN, HIGH);
   

}





void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = Serial.read(); //
    
    
    
    if(inChar > 47 && inChar < 58) {  // #1 - 10
     // channel/instrument change
       MIDI_CHANNEL = inChar - 48;
  
      Serial.print("Channel: ");
      Serial.println(MIDI_CHANNEL);
 
        if(SERIAL_MODE) {
        
          HWSERIAL.write( char(KEYMAP[MIDI_CHANNEL]) );
          Serial.println(char(KEYMAP[MIDI_CHANNEL]));
          
        }


      if(inChar == 57) {
             SendKeyStroke(6,7,9); // up key

             Serial.print("OCTAVE ") ;
             Serial.println(octave);
             delay(20);
             SendKeyUp();
      } 
      if(inChar == 48) {
             SendKeyStroke(7,7,10); // down key
             Serial.print("OCTAVE ") ;
             Serial.println(octave);
             delay(20);
             SendKeyUp();
      } 


      if(inChar == 75) { // K for keyboard performanxce mode in drummer
    
          digitalWrite(SHIFT_PIN, LOW); // shift +
             SendKeyStroke(2,7,0); // k
             Serial.print("Keyboard mode") ;
             delay(20);
             SendKeyUp();
          digitalWrite(SHIFT_PIN, HIGH); // shift off
     }




    } else {

    sendControl(inChar);

    }
  
  
      
  }
  
  
/*  while (HWSERIAL.available()) {
    // get the new byte:
    char inChar = HWSERIAL.read(); 
    Serial.println(inChar);
    
  }
*/
  
}



void sendControl(char inChar) {

    if(SERIAL_MODE) {
          HWSERIAL.write(inChar);
          
          Serial.println(inChar, HEX);
          
    } else {
  
          digitalWrite(SHIFT_PIN, LOW);
      
          Serial.print(inChar);
          Serial.print("  ");
      
      //    Serial.print(inChar, BIN);
      //    Serial.print("  ");
          
      //    Serial.print(inChar, OCT);
      //    Serial.print("  ");
          
          Serial.println(inChar, DEC);
          
      //    Serial.print(inChar, HEX);
      //    Serial.print("  ");   
          
        
         
          int SEARCH_COLUMN = -1;
          int SEARCH_ROW = -1;
          
          for (int row=0; row<ROWS; row++) {
              
              for (int column=0; column<COLUMNS; column++) {
                 if (inChar == CONTROLS_ARRAY[row][column]) {
                   SEARCH_COLUMN = row;
                   SEARCH_ROW = column;
                   break;
                 }
              }        
          }  // for rows
      
            SendKeyStroke(SEARCH_ROW, SEARCH_COLUMN, inChar);
            delay(100);
            SendKeyUp();
            
    } // if in serial mode
}





// keyboard parser pieces

//#include <avr/pgmspace.h>
//#include <avrpins.h>
//#include <max3421e.h>
//#include <address.h>
//#include <printhex.h>
//#include <message.h>
//#include <hexdump.h>
//#include <parsetools.h>
//#include <hidboot.h>




// write to keystrokes

/*void SetMods(uint8_t m, uint8_t key)	
{
    MODIFIERKEYS mod;
    *((uint8_t*)&mod) = m;

// send APPLE keys as key presses, not just as modifiers. 

  if(mod.bmLeftAlt  == 1) {
      digitalWrite(OPEN_APPLE_PIN, HIGH);
  } else {
      digitalWrite(OPEN_APPLE_PIN, LOW);
  }

  if(mod.bmRightAlt  == 1) {
      digitalWrite(CLOSED_APPLE_PIN, HIGH);
  } else {
      digitalWrite(CLOSED_APPLE_PIN, LOW);
  }

  if((mod.bmLeftShift  == 1) || (mod.bmRightShift  == 1) ) {
      digitalWrite(SHIFT_PIN, LOW);
  } else {
      digitalWrite(SHIFT_PIN, HIGH);
  }

  if((mod.bmRightCtrl  == 1) || (mod.bmLeftCtrl  == 1) ) {
      digitalWrite(CONTROL_PIN, LOW);
  } else {
      digitalWrite(CONTROL_PIN, HIGH);
  }

};

*/




void setup()
{
  pinMode(ledPin, OUTPUT);
  pinMode(EXIT_PIN, INPUT_PULLUP);
  
  usbMIDI.setHandleNoteOff(OnNoteOff);
  usbMIDI.setHandleNoteOn(OnNoteOn) ;
  
  usbMIDI.setHandleControlChange(OnControlChange);
  usbMIDI.setHandleProgramChange(OnProgramChange);
  
  
  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);
  delay(1000);
  
  Serial.begin(9600);

  if(SERIAL_MODE) {
    HWSERIAL.begin(9600);
  }

  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(S0_PIN, OUTPUT);
  pinMode(S1_PIN, OUTPUT);
  pinMode(S2_PIN, OUTPUT);
  pinMode(S3_PIN, OUTPUT);
  pinMode(SHIFT_PIN, OUTPUT);
  pinMode(CONTROL_PIN, OUTPUT);
  pinMode(OPEN_APPLE_PIN, OUTPUT);
  pinMode(CLOSED_APPLE_PIN, OUTPUT);
  pinMode(CAPS_LOCK_PIN, OUTPUT);
  pinMode(RESET_PIN, OUTPUT);
  pinMode(S0_4051, OUTPUT);
  pinMode(S1_4051, OUTPUT);
  pinMode(S2_4051, OUTPUT);
  
    
  digitalWrite(ENABLE_PIN, HIGH); // write low to enable, high to disable
  digitalWrite(S0_PIN, LOW);
  digitalWrite(S1_PIN, LOW);
  digitalWrite(S2_PIN, LOW);
  digitalWrite(S3_PIN, LOW);

  digitalWrite(CAPS_LOCK_PIN, HIGH);
  digitalWrite(RESET_PIN, HIGH);
  
  digitalWrite(OPEN_APPLE_PIN, LOW);
  digitalWrite(CLOSED_APPLE_PIN, LOW);

  // modifier keys - HIGH == not pressed. LOW == pressed.
  digitalWrite(SHIFT_PIN, HIGH);
  digitalWrite(CONTROL_PIN, HIGH);



    digitalWrite(S0_4051, LOW);
    digitalWrite(S1_4051, LOW);
    digitalWrite(S2_4051, LOW);

}




void loop()
{
   if( usbMIDI.read() ) {
//  Serial.println( usbMIDI.getType() ); 




 };

  serialEvent();

  if(SERIAL_MODE) { // no need for this with keyboard mode.

// check loop timer for noteOff
    if(( NOTE_OFF_TIMER ) && ( millis() > NOTE_OFF_TIMER + NOTE_OFF_INTERVAL )) {
// when loop timer ends, if a new note has been sent, ignore the noteOff

// else, if offnote hasn't been replaced, send noteOff
        HWSERIAL.write("+");

        Serial.println(NOTE_OFF_TIMER);
        NOTE_OFF_TIMER = 0;
    }
    
    
  }


        if (digitalRead(EXIT_PIN) == LOW) {
         // send "X"
          HWSERIAL.write("X");    
          Serial.println("RESET BUTTON");
          //delay(500);
        }


}







