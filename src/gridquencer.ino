/*-----------------------------------------
// griquencer teensy 3.5,3.6,4.0,4.1
// In Arduino IDE Set Serial Mode to MIDI
//-----------------------------------------*/
#include "USBHost_t36.h"
#include <MIDI.h>
#include <TimerThree.h>
#include <vector>
// todo #include "ArduinoJSON.h"

#include "Cell.h"
#include "Region.h" 
#include "Grid.h"
#include "Sequence.h"
#include "Sequencer.h"

IntervalTimer myTimer;

USBHost myusb;
USBHub hub1(myusb);
MIDIDevice_BigBuffer midi1(myusb); //// https://forum.pjrc.com/threads/66148-Teensy-3-6-USBHost-interfacing-Ableton-Push2

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI); // used for hardware MIDI output

const int LEDPIN = LED_BUILTIN;
int ledState = LOW;
const int LEDPIN1 = 2;
const int LEDPIN2 = 3;
float bpm = 60.0f;

std::vector<Cell> padsDown;
Grid grid;

Sequencer sequencer(60.0f, 480); 

void setup()
{
//  while (!Serial) ; // wait for Arduino Serial Monitor
  Serial.begin(115200);
  Serial.println("USB Host Testing");

  myusb.begin();
  MIDI.begin();
  midi1.setHandleNoteOff(OnNoteOff);
  midi1.setHandleNoteOn(OnNoteOn);
  midi1.setHandleControlChange(OnControlChange);
  //midi.setHandleAfterTouch(); // to be added later

  //sequencer.start();  // todo should this take a function as an argument? 
  myTimer.begin(seqfun, sequencer._microsPerSecond / sequencer._period);

  Serial.println(sequencer._beatPeriod);
  Serial.println(sequencer._bpm);
  Serial.println(sequencer._period);

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  

  delay(400);
  blankGridDisplay();  
  // push2midispec.md
  //#define ABLETON_VENDOR_ID 0x2982
  //#define PUSH2_PRODUCT_ID  0x1967
  if(midi1.idVendor() == 0x2982){
    Serial.println("USB device made by Ableton");
    digitalWrite(2, HIGH);
  }

  if(midi1.idProduct() == 0x113){
    Serial.println("USB Device is LaunchPad");
  }
  if(midi1.idProduct() == 0x1967){
    Serial.println("USB Device is Push 2");
    digitalWrite(3, HIGH);
  }

}

void seqfun(){
  sequencer.tick();
    // service notes for tick index
    // update visual feedback on grid 
  if(sequencer._tickTime % 480 == 0){
    // Serial.println("beat");
    MIDI.sendClock();
    //MIDI.sendNoteOn(74,126,10);
    //midi1.sendControlChange(120,120,1);
  }
  for(Sequence * seq : sequencer._sequences ){
    // check each note in each sequence to see if now is its time to shine
    for(Note n : seq->_notes){
      if(seq->_tickTime == n.index){
        //Serial.println("Sending note out" );
        // todo put the channels into the sequences
        // todo 
        MIDI.sendNoteOn(100,100,10);
        // MIDI.sendNoteOn(n.pitch, n.velocity, 10);
        // Serial.println(n.pitch);
      }
    }
  }
} 

void loop() {
  myusb.Task();
  midi1.read();
}

void OnNoteOn(byte channel, byte note, byte velocity) {
  Serial.print("Note On, ch=");
  Serial.print(channel);
  Serial.print(", note=");
  Serial.print(note);
  Serial.print(", velocity=");
  Serial.print(velocity);
  Serial.println();
  midi1.sendNoteOn(note, 10, channel);
  usbMIDI.sendNoteOn(note, velocity, channel);


  // Convert Notes to Cell notation zero indexed from bottom left of grid 

  // if(note < 30){return;} // don't listen to knob touches on push
  // padsDown.push_back( pushNoteToCell(note) );
  padsDown.push_back( LPPNoteToCell(note) );

  if(padsDown.size() == 2){ // TODO handle region of size 1
    Region newRegion(padsDown[0], padsDown[1] );

    if ( grid.addRegion( newRegion ) ){
      Serial.println("Region added to grid");

      std::vector<int> regionvec = newRegion.regionToVector(); 
      // TODO deallocate memory at appropriate time
      Sequence * newSequence = new Sequence(regionvec);
      sequencer.queueSequence(newSequence); 
      for(Note &n : newSequence->_notes){
        Serial.println(n.index);
      }

      // todo convert to JSON and print to console? 
      updateGridDisplay();
    }else{
      Serial.println("Region not added to grid");
    }
  }
}

void OnNoteOff(byte channel, byte note, byte velocity) {
  Serial.print("Note Off, ch=");
  Serial.print(channel);
  Serial.print(", note=");
  Serial.print(note);
  Serial.println();

  midi1.sendNoteOff(note,10,channel);

  if(!padsDown.empty()){
    padsDown.clear();
  }
  updateGridDisplay();
}

void OnControlChange(byte channel, byte control, byte value) {
  Serial.print("Control Change, ch=");
  Serial.print(channel);
  Serial.print(", control=");
  Serial.print(control);
  Serial.print(", value=");
  Serial.print(value);
  Serial.println();
  if(control == 30 ){ // setup button
    if(value == 127){
      blankGridDisplay();
    }else{
      updateGridDisplay();
    }
  }
  /// todo causes crash
  /*
  if(control == 45 && value == 127){
    bool ttt = grid.requestMoveRegion(grid._selectedRegion, 1,0);
  }
  */
}

Cell LPPNoteToCell( byte note ){
  byte y = (note / 10) - 1;
  byte x = (note % 10) - 1;
  Serial.println( y);
  Serial.println( x);
  Cell newcell(x, y);
  return newcell;
}

Cell pushNoteToCell( byte note ){
  byte y = floor( (note - 36) / 8 );
  byte x = (note - 36 ) % 8;
  Cell newcell(x, y);
  return newcell;
}

void LPPM3NoteOn(byte channel, byte note, byte velocity){
  Serial.println(note);
}


// TODO move to grid or region? 
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
      sendGridLPP( x, y, 0 );
    }
  }
}

void sendGridLPP( char x, char y, char col){
  char note = ( y * 10) + 11 + x;
  midi1.sendNoteOn( note, col, 1); 
}

void sendGrid( char x, char y, char col){
  char note = (y * 8) + x + 36;
  midi1.sendNoteOn( note, col, 1); 
}

void updateGridDisplay(){
 for ( GridCell &cell : grid.grid ){
   if( !cell.memberOf.empty() ){
     sendGridLPP( cell.cell._x, cell.cell._y, 15 ); //  TODO add colour defined by region
   }else{
     sendGridLPP( cell.cell._x, cell.cell._y, 0 ); 
   }
 } 
}