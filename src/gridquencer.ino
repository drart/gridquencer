/*-----------------------------------------
// griquencer teensy 3.5,3.6,4.0,4.1
// In Arduino IDE Set Serial Mode to MIDI
//-----------------------------------------*/
#include "USBHost_t36.h"
#include <MIDI.h>
#include <TimerThree.h>
#include <vector>
// todo #include "ArduinoJSON.h"

#include "Grid.h"
#include "Sequencer.h"

IntervalTimer myTimer;

USBHost myusb;
USBHub hub1(myusb);
//MIDIDevice_BigBuffer midicontroller(myusb); //// https://forum.pjrc.com/threads/66148-Teensy-3-6-USBHost-interfacing-Ableton-Push2
MIDIDevice midicontroller(myusb); // Push 1 is not a USB2.0 full speed device

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, midi_module_output); // used for hardware MIDI output

const int LEDPIN = LED_BUILTIN;
int ledState = LOW;
const int LEDPIN1 = 2;
const int LEDPIN2 = 3;

Sequencer sequencer(60.0f, 480);  // bpm, ticks per beat. 480 allows for good resolution of subdivisions up to 20

std::vector<Cell> padsDown;
Grid grid;

void setup()
{
//  while (!Serial) ; // wait for Arduino Serial Monitor
  Serial.begin(115200);
  Serial.println("USB Host Testing");
  digitalWrite(LEDPIN, HIGH);

  myusb.begin();
  midi_module_output.begin();
  midicontroller.setHandleNoteOff(OnNoteOff);
  midicontroller.setHandleNoteOn(OnNoteOn);
  midicontroller.setHandleControlChange(OnControlChange); 
  //midi.setHandleAfterTouch(); // to be added later

  //sequencer.start();  // todo should this take a function as an argument? 
  sequencer.bpm(120.0); // for now this just sets a value and does not update the intervaltimer
  myTimer.begin(seqfun, (double) sequencer._period);

  Serial.print("Sequencer BPM: ");
  Serial.println(sequencer._bpm);
  Serial.print("Sequencer beat period: ");
  Serial.println(sequencer._beatPeriod);
  Serial.print("Sequencer period: ");
  Serial.println(sequencer._period);

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);

  digitalWrite(2, HIGH);
  delay(400);

  String productname = (char*)midicontroller.product();
  Serial.print("Reported name of connected USB device: ");
  Serial.println(productname);
  // todo check for null? 

  blankGridDisplay();  
  // push2midispec.md
  //#define ABLETON_VENDOR_ID 0x2982
  //#define PUSH2_PRODUCT_ID  0x1967
  if(midicontroller.idVendor() == 0x2982 || midicontroller.idVendor() == 0x09E8){
    Serial.println("USB device made by Ableton");
    digitalWrite(3, HIGH);
  }
  if(midicontroller.idProduct() == 0x15){
    Serial.println("USB Device is Push 1");
    // https://github.com/Carlborg/hardpush/blob/master/hardpush.ino#L436
    byte sysexUserModeMessage[] = {240, 71, 127, 21, 98, 0, 1, 0, 247};
    midicontroller.sendSysEx(9, sysexUserModeMessage, true, 1);

    uint8_t line1[] = {240,71,127,21,28,0,0,247};
    uint8_t line2[] = {240,71,127,21,29,0,0,247};
    uint8_t line3[] = {240,71,127,21,30,0,0,247};
    uint8_t line4[] = {240,71,127,21,31,0,0,247};
    midicontroller.sendSysEx(sizeof(line1)/sizeof(uint8_t), line1, true);
    midicontroller.sendSysEx(sizeof(line2)/sizeof(uint8_t), line2, true);
    midicontroller.sendSysEx(sizeof(line3)/sizeof(uint8_t), line3, true);
    midicontroller.sendSysEx(sizeof(line4)/sizeof(uint8_t), line4, true);


    // PUSH SYSEX Spec
    // 240,71,127,21,<24+line(0-3)>,0,<Nchars+1>,<Offset>,<Chars>,247
    // 240,71,127,21,25,0,14,4,"Hello World",247 /// string is length 13
    uint8_t helloworldlcd[] = {240,71,127,21,25,0,14,0,34,104,101,108,108,111,32,119,111,114,108,100,34,247};
    midicontroller.sendSysEx(sizeof(helloworldlcd)/sizeof(uint8_t), helloworldlcd, true);



    digitalWrite(3, HIGH);
  }
  if(midicontroller.idProduct() == 0x1967){
    Serial.println("USB Device is Push 2");
    digitalWrite(3, HIGH);
  }
  /// launchpad pro mk3 productID 0x123
  if(midicontroller.idProduct() == 0x123){ 
    Serial.println("USB Device is LaunchPad");
    digitalWrite(3, HIGH);
  }
  // TODO make this a debug menu? 
  Serial.print( "Product ID of MIDI controller: ");
  Serial.println( (uint16_t) midicontroller.idProduct() );
  Serial.print( "Vendor ID of MIDI controller: ");
  Serial.println( (uint16_t) midicontroller.idVendor() );
}


void checkDevices(){

}

void seqfun(){
  sequencer.tick();
    // service notes for tick index
    // update visual feedback on grid 
  if(sequencer._tickTime % 480 == 0){
    Serial.println("beat");
    midi_module_output.sendClock();
    midi_module_output.sendNoteOn(74,126,10);
    //midi1.sendControlChange(120,120,1);
  }
  for(Sequence * seq : sequencer._sequences ){
    for(uint8_t i = 0; i < seq->_notes.size(); i++){
      Note * n = &seq->_notes.at(i);
      if(seq->_tickTime == n->startIndex){
        n->playing = true;
      }
    }
    for(Note n : seq->_notes){
      if(seq->_tickTime == n.startIndex){
        n.playing = true;
        // todo put the channels into the sequences
        midi_module_output.sendNoteOn(n.pitch,n.duration,10);
        Serial.println(n.pitch); 

      }
    }
  }
} 

void loop() {
  myusb.Task();
  midicontroller.read();
  updateGridDisplay(); // refactor this function to look for changes from last call? 
} 

void OnNoteOn(byte channel, byte note, byte velocity) {
  Serial.print("Note On, ch=");
  Serial.print(channel);
  Serial.print(", note=");
  Serial.print(note);
  Serial.print(", velocity=");
  Serial.print(velocity);
  Serial.println();
  midicontroller.sendNoteOn(note, 10, channel);

  // Convert Notes to Cell notation zero indexed from bottom left of grid 

  if(note < 30){return;} // TODO don't listen to knob touches on push - need a better solution for this
  padsDown.push_back( pushNoteToCell(note) );
  //padsDown.push_back( LPPNoteToCell(note) ); // todo make a set of classes for different controllers? 

  if(padsDown.size() == 2){ // TODO handle region of size 1
    Region newRegion(padsDown[0], padsDown[1] );

    if ( grid.addRegion( newRegion ) ){
      Serial.println("Region added to grid");

      std::vector<int> regionvec = newRegion.regionToVector(); 
      Sequence * newSequence = new Sequence(regionvec); // TODO deallocate memory at appropriate time

      for(uint8_t i = 0; i < newRegion.cells.size(); i++){
        GridCell * location = grid.getCell( newRegion.cells.at(i) );
        location->note = &newSequence->_notes.at(i);
        location->note->pitch = random(127);
        location->_sequence = newSequence;
      }

      sequencer.queueSequence(newSequence); 
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

  midicontroller.sendNoteOff(note,10,channel);

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
  for ( uint8_t x = 0; x < 8; x++){
    for ( uint8_t y = 0; y < 8; y++ ){
      //sendGridLPP( x, y, 0 );
      sendGrid( x, y, 0 );
    }
  }
}

void sendGridLPP( uint8_t x, uint8_t y, uint8_t col){
  uint8_t note = ( y * 10) + 11 + x;
  midicontroller.sendNoteOn( note, col, 1); 
}

void sendGrid( uint8_t x, uint8_t y, uint8_t col){
  uint8_t note = (y * 8) + x + 36;
  midicontroller.sendNoteOn( note, col, 1); 
}

/*
void updateGridDisplay(){
  // Serial.println("updategriddisplay ");
 for ( GridCell &cell : grid.grid ){
   if( !cell.memberOf.empty() ){
     sendGrid( cell.cell._x, cell.cell._y, 100 ); //  TODO add colour defined by region
     if(cell.note != NULL){
      if(cell.note->playing == true){
        // sendGrid( cell.cell._x, cell.cell._y, 110 ); 
        Serial.println("playing");
      }else{
        // sendGrid( cell.cell._x, cell.cell._y, 100 ); 
      }
     }
   }else{
     sendGrid( cell.cell._x, cell.cell._y, 0 ); 
   }
 } 
}
*/

void updateGridDisplay() {
  for (GridCell &cell : grid.grid) {
    if (cell.note != NULL) {
      Serial.println("not null");
      if (cell.note->playing == true) {
        sendGrid(cell.cell._x, cell.cell._y, 110);
        Serial.println("playing");
      } else {
        sendGrid(cell.cell._x, cell.cell._y, 100);
      }
    } else {
      sendGrid(cell.cell._x, cell.cell._y, 0);
    }
  }
}

// PUSH SYSEX Spec
// 240,71,127,21,<24+line(0-3)>,0,<Nchars+1>,<Offset>,<Chars>,247
// 240,71,127,21,25,0,13,4,"Hello World",247
void lcdClearLine(uint8_t line)
{
  // todo check that line is between 0 and 3 (or 1 and 4?)
  line = 0;
  // uint8_t message[] = {240,71,127,21,28+line,0,0,247};
  // midicontroller.sendSysEx(10, message, true);
}