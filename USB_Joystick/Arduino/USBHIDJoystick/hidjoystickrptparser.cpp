



#include "hidjoystickrptparser.h"

JoystickReportParser::JoystickReportParser(JoystickEvents *evt) :
	joyEvents(evt),
	oldHat(0xDE),
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
		for (uint8_t i=0; i<0x0C; i++)
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

X = evt->X;
Y = evt->Y;
  	Serial.print("X: ");
  	Serial.print(X);
	//PrintHex<uint8_t>(evt->X, 0x80);
	Serial.print("\tY: ");
	Serial.print(Y);
//	PrintHex<uint8_t>(evt->Y, 0x80);
	Serial.println("");


}


void JoystickEvents::OnButtonUp(uint8_t but_id)
{
	Serial.print("Up: ");
	Serial.println(but_id);
      if(but_id == 1) {
        Butt0 = false;
      }

      if(but_id == 2) {
        Butt1 = false;
      }

}

void JoystickEvents::OnButtonDn(uint8_t but_id)
{
	Serial.print("Dn: ");
	Serial.println(but_id);


      if(but_id == 1) {
        Butt0 = true;
      }

      if(but_id == 2) {
        Butt1 = true;
      }
}


