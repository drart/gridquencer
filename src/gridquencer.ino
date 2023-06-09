/*-----------------------------------------
// griquencer teensy 3.5,3.6,4.0,4.1
// In Arduino IDE Set Serial Mode to MIDI
//-----------------------------------------*/
#include <Arduino.h>
#include <USBHost_t36.h>
#include <MIDI.h>
#include <TimerThree.h>
#include <vector>
// todo #include "ArduinoJSON.h"

#include "Grid.h"
#include "Sequencer.h"
#include "RegionSequenceMediator.h"

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
RegionSequenceMediator mediator;

mode subdivisionMode = mode::SIXTEENTH_TUPLET; 
enum class entryMode{
  NEW,
  MUTE,
  DELETE
  // MOVE?
};


enum PUSH2COLOURS { // https://gist.github.com/adamjmurray/21d7d3ae1f2ef8c66a19
  OFF,
  DARK_GREY,
  GREY,
  WHITE,
  WHITE_RED,
  RED_BRIGHT,
  RED,
  RED_DIM,
  RED_AMBER,
  AMBER_BRIGHT,
  AMBER,
  AMBER_DIM,
  AMBER_YELLOW,
  YELLOW_BRIGHT,
  YELLOW,
  YELLOW_DIM,
  YELLOW_LIME,
  LIME_BRIGHT,
  LIME,
  LIME_DIM,
  LIME_GREEN,
  GREEN_BRIGHT,
  GREEN,
  GREEN_DIM,
  GREEN_SPRING,
  SPRING_BRIGHT,
  SPRING,
  SPRING_DIM,
  SPRING_TURQUOISE,
  TURQUOISE_BRIGHT,
  TURQUOISE,
  TURQUOISE_DIM,
  TURQUOISE_CYAN,
  CYAN_BRIGHT,
  CYAN,
  CYAN_DIM,
  CYAN_SKY,
  SKY_BRIGHT,
  SKY,
  SKY_DIM,
  SKY_OCEAN,
  OCEAN_BRIGHT,
  OCEAN,
  OCEAN_DIM,
  OCEAN_BLUE,
  BLUE_BRIGHT,
  BLUE,
  BLUE_DIM,
  BLUE_ORCHID,
  ORCHID_BRIGHT,
  ORCHID,
  ORCHID_DIM,
  ORCHID_MAGENTA,
  MAGENTA_BRIGHT,
  MAGENTA,
  MAGENTA_DIM,
  MAGENTA_PINK,
  PINK_BRIGHT,
  PINK,
  PINK_DIM,
  ORANGE_BRIGHT,
  ORANGE,
  ORANGE_DIM
};

entryMode regionMode = entryMode::NEW;
entryMode noteMode   = entryMode::NEW;

void setup()
{
  Serial.begin(115200);
  Serial.println("USB Host Testing");
  // digitalWrite(LEDPIN, HIGH);

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
  delay(400); // is this necessary anymore? 

  String productname = (char*)midicontroller.product();
  Serial.print("Reported name of connected USB device: ");
  Serial.println(productname);
  // todo check for null? 

  if(midicontroller.idVendor() == 0x2982 || midicontroller.idVendor() == 0x09E8){
    Serial.println("USB device made by Ableton");
    digitalWrite(3, HIGH);
  }
  if(midicontroller.idProduct() == 0x15){
    Serial.println("USB Device is Push 1");
    // https://github.com/Carlborg/hardpush/blob/master/hardpush.ino#L436
    uint8_t sysexUserModeMessage[] = {240, 71, 127, 21, 98, 0, 1, 0, 247};
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
    String welcome = "Welcome to Gridquencer";
    std::vector<uint8_t> message = {240,71,127,21,24,0};
    message.push_back(welcome.length()+1);
    message.push_back(0);
    for(uint8_t i = 0; i < welcome.length(); i++){
      message.push_back(welcome.charAt(i));
    }
    message.push_back(247);
    midicontroller.sendSysEx(message.size(), message.data(), true);

    midicontroller.sendControlChange(87, 127, 1); // new button
    midicontroller.sendControlChange(50, 127, 1); // note button
    midicontroller.sendControlChange(60, 1, 1); // mute button
    midicontroller.sendControlChange(49, 1, 1); // shift button 

    changeSubdivisionMode(41);
    //changeEntryMode();

    digitalWrite(3, HIGH);
  }
  if(midicontroller.idProduct() == 0x1967){
  // push2midispec.md
  //#define ABLETON_VENDOR_ID 0x2982
  //#define PUSH2_PRODUCT_ID  0x1967
    Serial.println("USB Device is Push 2");
    digitalWrite(3, HIGH);
  }
  if(midicontroller.idProduct() == 0x123){ 
    Serial.println("USB Device is LaunchPad");
    digitalWrite(3, HIGH);
  }
  Serial.print( "Product ID of MIDI controller: ");
  Serial.println( (uint16_t) midicontroller.idProduct() );
  Serial.print( "Vendor ID of MIDI controller: ");
  Serial.println( (uint16_t) midicontroller.idVendor() );
}

void seqfun(){
  sequencer.tick();
    // service notes for tick index
  if(sequencer._tickTime % 480 == 0){ // 480 should be taken from the sequencer
    Serial.println("beat");
    midi_module_output.sendClock(); // configure for more standard rates
  }
  for(Sequence * seq : sequencer._sequences ){
    for(uint8_t i = 0; i < seq->_notes.size(); i++){
      Note * n = &seq->_notes.at(i);
      if(seq->_tickTime == n->startIndex){
        n->playing = true;
        // Note resultingNote = resolveNoteProbabilities(n); // also velocity variance? 
        if(n->mute == true){
          Serial.println("muted note");
        }
        midi_module_output.sendNoteOn(n->pitch,n->velocity,10);
      }
      if(seq->_tickTime == n->endIndex){
        n->playing = false;
        midi_module_output.sendNoteOn(n->pitch, 0, 10); // config to send noteOff or noteOn velocity 0? 
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
  // Serial.print("Note On, ch=");
  // Serial.print(channel);
  // Serial.print(", note=");
  // Serial.print(note);
  // Serial.print(", velocity=");
  // Serial.print(velocity);
  // Serial.println();

  // Convert Notes to Cell notation zero indexed from bottom left of grid 

  if(note < 30){return;} // TODO don't listen to knob touches on push - need a better solution for this
  padsDown.push_back( pushNoteToCell(note) ); // todo make a set of classes for different controllers? 

  if(padsDown.size() == 2){ 
    addRegion(padsDown[0], padsDown[1]);
  }
  if(padsDown.size() == 1){
    grid._selectedCell = grid.getCell(padsDown.at(0));
    // if(grid._selectedCell->note != NULL){
      // Serial.println(grid._selectedCell->note->pitch);
      // Serial.println(grid._selectedCell->note->velocity);
    // }

    // switch(regionMode){
    //   case entryMode::MUTE:
    //     if(grid._selectedCell->note->mute == true){
    //       grid._selectedCell->note->mute = false;
    //     }else{
    //       grid._selectedCell->note->mute = true;
    //     }
    //     Serial.println(grid._selectedCell->note->mute);
    //   break;
    //   case entryMode::DELETE:
    //   break;
    // }
  }
}

void OnNoteOff(byte channel, byte note, byte velocity) {
  // Serial.print("Note Off, ch=");
  // Serial.print(channel);
  // Serial.print(", note=");
  // Serial.print(note);
  // Serial.println();

  if(padsDown.size() == 1){
    if(regionMode == entryMode::NEW){
      addRegion(padsDown[0], padsDown[0]);
    }
  }
  if(!padsDown.empty()){ // can I remove this check? 
    padsDown.clear();
  }
}

void OnControlChange(byte channel, byte control, byte value) {
  Serial.print("Control Change, ch=");
  Serial.print(channel);
  Serial.print(", control=");
  Serial.print(control);
  Serial.print(", value=");
  Serial.print(value);
  Serial.println();
  
  int8_t knobvaldiff = value;
  if(knobvaldiff > 64){
    knobvaldiff = -(128 - knobvaldiff);
  }

  switch(control){
    case 71: // knob 1
      // if(grid._selectedCell->note != NULL){
        // float thenote = grid._selectedCell->note->pitch;
        // thenote += float(knobvaldiff);
        // thenote = constrain(thenote, 0, 127);
        // grid._selectedCell->note->pitch = thenote;
        // Serial.println(grid._selectedCell->note->pitch);
      // }
    break;
    case 72:
    break;
    case 73:
    break;
    case 74:
    break;
    case 45:
      if(value == 127){
        // grid.requestMoveRegion(grid._selectedRegion, 1, 0);
      }
    break;
    case 46:

      // grid.requestMoveRegion(grid._selectedRegion,-1,0);
    break;
    case 47:
      // grid.requestMoveRegion(grid._selectedRegion,0,1);
    break;
    case 48:
      // grid.requestMoveRegion(grid._selectedRegion,0,1);
    break;
    case 60:
      if(value == 127){
        regionMode = entryMode::MUTE;
        midicontroller.sendControlChange(60, 10, 1);
        midicontroller.sendControlChange(87, 1, 1);
      }
    break;
    case 87:
      if(value == 127){
        regionMode = entryMode::NEW;
        midicontroller.sendControlChange(87, 12, 1);
        midicontroller.sendControlChange(60, 1, 1);
      }
    break;
    case 36:
      if(value == 127){ changeSubdivisionMode(control); }
    break;
    case 37:
      if(value == 127){ changeSubdivisionMode(control); }
    break;
    case 38:
      if(value == 127){ changeSubdivisionMode(control); }
    break;
    case 39:
      if(value == 127){ changeSubdivisionMode(control); }
    break;
    case 40:
      if(value == 127){ changeSubdivisionMode(control); }
    break;
    case 41:
      if(value == 127){ changeSubdivisionMode(control); }
    break;
    case 42:
      if(value == 127){ changeSubdivisionMode(control); }
    break;
    case 43:
      if(value == 127){ changeSubdivisionMode(control); }
    break;
  }
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

void updateGridDisplay() {
  /*
  for (GridCell & cell : grid.grid) {
    if (cell.note != NULL) {
      if (cell.note->playing == true) {
        sendGrid(cell.cell._x, cell.cell._y, PUSH2COLOURS::WHITE);
      } else {
        //Serial.println(cell._region->colour);
        uint8_t rc = uint8_t(cell._region->colour);
        sendGrid(cell.cell._x, cell.cell._y, rc);
      }
    } else {
      sendGrid(cell.cell._x, cell.cell._y, PUSH2COLOURS::OFF);
    }
  }*/
  for(uint8_t x = 0; x < 8; x++){// fix for grid size
    for(uint8_t y = 0; y < 8; y++){
      if(mediator.cellNotes.find((y*8)+x) == mediator.cellNotes.end()){
        sendGrid(x, y, PUSH2COLOURS::OFF);
        continue;
      }

      GridCell * gc = &mediator.cellNotes[(y*8)+x];
      if(gc->note->playing == true){
        sendGrid(x, y, PUSH2COLOURS::WHITE);
      }else{
        sendGrid(x, y, gc->_region->colour);
      }
    }
  }
}

void updateLCD(){
}

// PUSH SYSEX Spec
// 240,71,127,21,<24+line(0-3)>,0,<Nchars+1>,<Offset>,<Chars>,247
// 240,71,127,21,25,0,13,4,"Hello World",247
void lcdClearLine(uint8_t line)
{
  // todo check that line is between 0 and 3 (or 1 and 4?)
  line = 0;
  uint8_t message[] = {240,71,127,21,uint8_t(28+line),0,0,247};
  midicontroller.sendSysEx(10, message, true);
}

void addRegion(Cell start, Cell end){
    Region * newRegion  = new Region(start, end);
    newRegion->colour = 10 + (grid._regions.size() * 4); // TODO will be a problem if a region is deleted 

    if ( grid.addRegion( newRegion ) ){

      // std::vector<uint8_t> regionvec = newRegion->regionToVector(); 
      Sequence * newSequence = mediator.regionToSequence(newRegion, subdivisionMode);
      // Sequence * newSequence = new Sequence(regionvec, subdivisionMode); // TODO deallocate memory at appropriate time

      // for(uint8_t i = 0; i < newRegion->cells.size(); i++){ // todo move this into a function
        // GridCell * location = grid.getCell( newRegion->cells.at(i) );
        // location->note = &newSequence->_notes.at(i);
        // location->note->pitch = random(127);
        // location->_sequence = newSequence;
      // }
      sequencer.queueSequence(newSequence);  // TODO it would be nice if this held off until the noteOff, maybe?
    }else{
      Region * overlappingRegion = grid.getOverlappingRegion(newRegion);
      if(overlappingRegion != NULL){
        Serial.println("Attempting to modify the region");
        Serial.println(overlappingRegion->cells.size());
      
        if(overlappingRegion->modify(newRegion)){
          Serial.println("success");
          mediator.modifySequence(overlappingRegion);
          // Sequence * overlappingSequence = grid.getCell(overlappingRegion->cells.at(0))->_sequence;
          // overlappingSequence->modify(overlappingRegion->regionToVector()); // todo implement properly
          // Serial.println(overlappingRegion->cells.size());
          // grid.updateGrid(overlappingRegion, overlappingSequence);// todo implement
        }

      }else{
        Serial.println("Region not added to grid");
      }
      delete newRegion;
    }
}

void changeSubdivisionMode(uint8_t button){

  switch(button){
    case 43:
      subdivisionMode = mode::THIRTYSECOND_TUPLET;
    break;
    case 42: 
      subdivisionMode = mode::THIRTYSECOND;
    break;
    case 41: 
      subdivisionMode = mode::SIXTEENTH_TUPLET;
    break;
    case 40:
      subdivisionMode = mode::SIXTEENTH;
    break;
    case 39:
      subdivisionMode = mode::EIGHT_TUPLET;
    break;
    case 38: 
      subdivisionMode = mode::EIGHTH;
    break;
    case 37:
      subdivisionMode = mode::QUARTER_TUPLET;
    break;
    case 36:
      subdivisionMode = mode::QUARTER;
      break;
  }
  for(uint8_t b = 36; b <= 43; b++){
    if(b == button){
      midicontroller.sendControlChange(b, 127, 1);
    }else{
      midicontroller.sendControlChange(b, 7, 1);
    }
  }
}