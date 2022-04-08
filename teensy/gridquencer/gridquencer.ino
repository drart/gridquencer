// Simple test of USB Host
//
// This example is in the public domain

#include "USBHost_t36.h"

USBHost myusb;
MIDIDevice_BigBuffer midi1(myusb);
//// https://forum.pjrc.com/threads/66148-Teensy-3-6-USBHost-interfacing-Ableton-Push2



void setup()
{
	while (!Serial) ; // wait for Arduino Serial Monitor
	Serial.println("USB Host Testing");
	myusb.begin();

	midi1.setHandleNoteOff(OnNoteOff);
	midi1.setHandleNoteOn(OnNoteOn);
	midi1.setHandleControlChange(OnControlChange);
  //midi.setHandleAfterTouch();
}


void loop()
{
	myusb.Task();
	midi1.read();
}


void OnNoteOn(byte channel, byte note, byte velocity)
{
	Serial.print("Note On, ch=");
	Serial.print(channel);
	Serial.print(", note=");
	Serial.print(note);
	Serial.print(", velocity=");
	Serial.print(velocity);
	Serial.println();
  midi1.sendNoteOn(note, 10, channel);
}

void OnNoteOff(byte channel, byte note, byte velocity)
{
	Serial.print("Note Off, ch=");
	Serial.print(channel);
	Serial.print(", note=");
	Serial.print(note);
	//Serial.print(", velocity=");
	//Serial.print(velocity);
	Serial.println();
 midi1.sendNoteOff(note,10,channel);
}

void OnControlChange(byte channel, byte control, byte value)
{
	Serial.print("Control Change, ch=");
	Serial.print(channel);
	Serial.print(", control=");
	Serial.print(control);
	Serial.print(", value=");
	Serial.print(value);
	Serial.println();
}
