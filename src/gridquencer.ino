/*-----------------------------------------
// griquencer teensy 
// Set Serial Mode to MIDI
//
//
//-----------------------------------------*/
#include "USBHost_t36.h"
#include <MIDI.h>
#include <TimerThree.h>
#include <vector>

#include "Cell.h"
#include "Region.h" 
#include "Grid.h"
#include "Sequence.h"
#include "Sequencer.h"

IntervalTimer myTimer;

USBHost myusb;
MIDIDevice_BigBuffer midi1(myusb);
//// https://forum.pjrc.com/threads/66148-Teensy-3-6-USBHost-interfacing-Ableton-Push2

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI); // used for hardware MIDI output

const int LEDPIN = LED_BUILTIN;
int ledState = LOW;
float bpm = 60.0f;
long microsPerSecond = 1000000;
long period;
long tickPeriod;

Cell test( 10, 2);
std::vector<Cell> padsDown;
Grid grid;

// Sequencer sequencer;

void setup()
{
//  while (!Serial) ; // wait for Arduino Serial Monitor
  Serial.begin(9600);
  Serial.println("USB Host Testing");
  myusb.begin();

  MIDI.begin(MIDI_CHANNEL_OMNI);

  midi1.setHandleNoteOff(OnNoteOff);
  midi1.setHandleNoteOn(OnNoteOn);
  midi1.setHandleControlChange(OnControlChange);
  //midi.setHandleAfterTouch(); // to be added later

  // push2midispec.md
  //#define ABLETON_VENDOR_ID 0x2982
  //#define PUSH2_PRODUCT_ID  0x1967
  Serial.println( midi1.idVendor() );
  //Serial.println( String((char)midi1.product()).c_str() );

  period = (60 / bpm ) * microsPerSecond;  
  tickPeriod = period / 480;
//  Serial.print("Default period: "); 
//  Serial.println(period);
  // myTimer.begin(sequencer, period);
  
}



/*
void sequencer(){

 if (ledState == LOW) {
    ledState = HIGH;
//    midi1.sendNoteOn( 60, 100, 1 );
  } else {
    ledState = LOW;
//    midi1.sendNoteOff( 60, 100, 1 );
  }
  digitalWrite(LEDPIN, ledState);
  
  // sequencer.tick();
  
} 
*/

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
    Region newRegion(padsDown[0], padsDown[1] );
    // Serial.println( newRegion.numberOfSteps() );
    if (grid.addRegion( padsDown[0], padsDown[1] ) ){
      std::vector<int> steps = newRegion.regionToVector();
      // for(int i = 0; i < (int)newRegion.cells.size(); i++){
      //   Serial.print(newRegion.cells[i]._x);
      //   Serial.print(",");
      //   Serial.println(newRegion.cells[i]._y);
      // }
      // Serial.println((int)steps.size());
      Sequence newSequence( steps );
      // create a sequence and add to sequencer
    }
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



void moveRegion(byte channel, byte number, byte value){
  if(number == 45) {
    grid.requestMoveRegion(grid._selectedRegion,1,0);
  }
  else if(number == 46) {
    grid.requestMoveRegion(grid._selectedRegion,-1,0);
  }
  else if(number == 47) {
    grid.requestMoveRegion(grid._selectedRegion,0,1);
  }
  else if(number == 48) {
    grid.requestMoveRegion(grid._selectedRegion,0,1);
  }
}


void blankGridDisplay(){
  for ( char x = 0; x < 8; x++){
    for ( char y = 0; y < 8; y++ ){
      sendGrid( x, y, 0 );
    }
  }
}

void sendGrid( char x, char y, char col){
  char note = (y * 8) + x + 36;
  midi1.sendNoteOn( note, col, 1); 
}


void updateGridDisplay(){
  Serial.println(grid.grid.size() );
  Serial.println(grid.grid[8].cell._x );
//  for ( GridCell &cell : grid.grid ){
//    Serial.println( cell.cell._x );
//    if(cell._region != NULL){
//      sendGrid( cell.cell._x, cell.cell._y, 15 ); //  TODO add colour defined by region
//    }
//  } 
}