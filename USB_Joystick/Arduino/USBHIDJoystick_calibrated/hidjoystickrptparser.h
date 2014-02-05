#if !defined(__HIDJOYSTICKRPTPARSER_H__)
#define __HIDJOYSTICKRPTPARSER_H__

#include <inttypes.h>
#include <avr/pgmspace.h>
#include "avrpins.h"
#include "max3421e.h"
#include "usbhost.h"
#include "usb_ch9.h"
#include "Usb.h"

#if defined(ARDUINO) && ARDUINO >=100
#include "Arduino.h"
#else
#include <WProgram.h>
#endif

#include "printhex.h"
#include "hexdump.h"
#include "message.h"
#include "confdescparser.h"
#include "hid.h"

struct GamePadEventData
{
	uint8_t axis0, axis1, axis2, axis3, axis4; // read first 5 axes
};

class JoystickEvents
{
public:
	virtual void OnGamePadChanged(const GamePadEventData *evt);
	virtual void OnButtonUp(uint8_t but_id);
	virtual void OnButtonDn(uint8_t but_id);

uint8_t X;
uint8_t Y;

int axes[5];
int axesMin[5];
int axesMax[5];
int axesDelta[5];

int activeX;
int activeY;
//arrays of min and max values
// var with which axis is X/Y

boolean initialized;

boolean Butt0;
boolean Butt1;

};

#define RPT_GEMEPAD_LEN		5

class JoystickReportParser : public HIDReportParser
{
	JoystickEvents		*joyEvents;

	uint8_t				oldPad[RPT_GEMEPAD_LEN];
	uint16_t			oldButtons;

public:
	JoystickReportParser(JoystickEvents *evt);

	virtual void Parse(HID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf);
};

#endif // __HIDJOYSTICKRPTPARSER_H__
