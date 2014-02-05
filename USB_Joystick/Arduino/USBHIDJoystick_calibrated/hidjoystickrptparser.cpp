#include "hidjoystickrptparser.h"

JoystickReportParser::JoystickReportParser(JoystickEvents *evt) :
	joyEvents(evt),
	oldButtons(0)
{
	for (uint8_t i=0; i<RPT_GEMEPAD_LEN; i++)
		oldPad[i]	= 0xD;
}

void JoystickReportParser::Parse(HID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf)
{
	bool match = true;

	// Checking if there are changes in report since the method was last called
	for (uint8_t i=0; i<RPT_GEMEPAD_LEN; i++)
		if (buf[i] != oldPad[i])
		{
			match = false;
			break;
		}

	// Calling Game Pad event handler
	if (!match && joyEvents)
	{
		joyEvents->OnGamePadChanged((const GamePadEventData*)buf);

		for (uint8_t i=0; i<RPT_GEMEPAD_LEN; i++) oldPad[i] = buf[i];
	}

	uint16_t buttons = (0x0000 | buf[6]);
	buttons <<= 4;
	buttons |= (buf[5] >> 4);
	uint16_t changes = (buttons ^ oldButtons);

	// Calling Button Event Handler for every button changed
	if (changes)
	{
//		for (uint8_t i=0; i<0x0C; i++) // 12 buttons 0x0C == 12
		for (uint8_t i=0; i<0x0F; i++) // up to 16 buttons 
		{
			uint16_t mask = (0x0001 << i);

			if (((mask & changes) > 0) && joyEvents)
				if ((buttons & mask) > 0)
					joyEvents->OnButtonDn(i+1);
				else
					joyEvents->OnButtonUp(i+1);
		}
		oldButtons = buttons;
	}
}

void JoystickEvents::OnGamePadChanged(const GamePadEventData *evt)
{
  // got to find a better way to make an array from this structured vars
  axes[0] =  evt->axis0; 
  axes[1] =  evt->axis1; 
  axes[2] =  evt->axis2; 
  axes[3] =  evt->axis3; 
  axes[4] =  evt->axis4; 


  
  if(initialized) {
  
  
    for(int i =0; i<5 ; i++ ){   
      
      if (axes[i] < axesMin[i] ) {
        axesMin[i] = axes[i];
      }
      
      if (axes[i] > axesMax[i]) {
        axesMax[i] = axes[i];
      }
      
      axesDelta[i] = axesMax[i] - axesMin[i]; 

    }
  

      for(int j =0; j<5 ; j++ ){   
      
        if(axesDelta[j] > 0) {
          X = axes[j];
          Y = axes[j+1];
          break;
        }
        
      }


  } else {

    for(int i =0; i<5 ; i++ ){         
        axesMin[i] = axes[i];
        axesMax[i] = axes[i];
    }
    
    initialized = true;
    // don't bother with the first set of data, as joystick initializes.
    
  }

Serial.print(X);
Serial.print("   ");
Serial.println(Y);


/*
  
keep track of axes 0 - 4 (5 axes)

min and max values. max-min = range

first axis with range, and first +1 are X and Y

*/

}


void JoystickEvents::OnButtonUp(uint8_t but_id)
{
	Serial.print("Up: ");
	Serial.println(but_id);
      if(but_id % 2 == 0) { // all even numbered buttons = button 0
        Butt0 = false;
      }

      if(but_id % 2 == 1) { // all odd buttons = button 1
        Butt1 = false;
      }

}

void JoystickEvents::OnButtonDn(uint8_t but_id)
{
	Serial.print("Dn: ");
	Serial.println(but_id);


      if(but_id % 2 == 0) { // all even numbered buttons = button 0
        Butt0 = true;
      }

      if(but_id % 2 == 1) { // all odd buttons = button 1
        Butt1 = true;
      }
}


