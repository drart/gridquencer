/*-----------------------------------------
// griquencer teensy 
// Set Serial Mode to MIDI
//
//
//-----------------------------------------*/
#include "USBHost_t36.h"
#include <TimerThree.h>
#include <list>

#include "Cell.h"
#include "Region.h" 
#include "Grid.h"

IntervalTimer myTimer;

USBHost myusb;
MIDIDevice_BigBuffer midi1(myusb);
//// https://forum.pjrc.com/threads/66148-Teensy-3-6-USBHost-interfacing-Ableton-Push2

const int LEDPIN = LED_BUILTIN;
int ledState = LOW;

Cell test( 10, 2);
std::list<Cell> padsDown;

void setup()
{
//  while (!Serial) ; // wait for Arduino Serial Monitor
  Serial.println("USB Host Testing");
  myusb.begin();

  midi1.setHandleNoteOff(OnNoteOff);
  midi1.setHandleNoteOn(OnNoteOn);
  midi1.setHandleControlChange(OnControlChange);
  //midi.setHandleAfterTouch();

  // push2midispec.md
  //#define ABLETON_VENDOR_ID 0x2982
  //#define PUSH2_PRODUCT_ID  0x1967
  Serial.println( midi1.idVendor() );
  //Serial.println( String((char)midi1.product()).c_str() );

//  Timer3.initialize(150000);
//  Timer3.attachInterrupt(sequencer);
}



void sequencer(){

 if (ledState == LOW) {
    ledState = HIGH;
  } else {
    ledState = LOW;
  }
  digitalWrite(LEDPIN, ledState);
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
  usbMIDI.sendNoteOn(note, velocity, channel);
  
  padsDown.push_back( pushNoteToCell(note) );
  if(padsDown.size() == 2){
       
  
  }
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
  usbMIDI.sendNoteOff(note,10,channel);
  padsDown.clear();
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


Cell pushNoteToCell( byte note ){
  byte y = floor( (note - 36) / 8 );
  byte x = (note - 36 ) % 8;
  Cell newcell(x, y);
  return newcell;
}
