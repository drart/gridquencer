#include "RegionSequenceMediator.h"

#include "Arduino.h"  

RegionSequenceMediator::RegionSequenceMediator(){
  for(uint8_t x = 0; x < 8; x++){
    for(uint8_t y = 0; y < 8; y++){
      GridCell gc;
      this->cellNotes.push_back(gc);
    }
  }
}

Sequence * RegionSequenceMediator::regionToSequence(Region * region, mode subdivisionMode){

  std::vector<uint8_t> pattern = region->regionToVector();
  Sequence * newSequence =  new Sequence(pattern, subdivisionMode);

  for(uint8_t i = 0; i < region->cells.size(); i++){
    GridCell * gc = &this->cellNotes.at((region->cells.at(i)._y*8) + region->cells.at(i)._x);
    // todo move to this version
    // GridCell * gc = this->getCell( region->cells.at(i)._x, region->cells.at(i)._y );
    gc->cell = &region->cells.at(i);
    gc->note = &newSequence->_notes.at(i);
    gc->_region = region;
    gc->_sequence = newSequence;
  }

  return newSequence;
}

void RegionSequenceMediator::modifySequence(Region * region, Sequence * sequence){ // TODO subdivision mode? 
  std::vector<uint8_t> oldPattern = sequence->pattern;
  sequence->modify(region->regionToVector());
  std::vector<uint8_t> newPattern = sequence->pattern;

  for(uint8_t i = 0; i < region->cells.size(); i++){
    GridCell * gc = this->getCell(region->cells.at(i)._x, region->cells.at(i)._y);
    gc->cell = &region->cells.at(i); // is this necessary? 
    gc->note = &sequence->_notes.at(i);
    gc->_region = region;
    gc->_sequence = sequence;

    // todo move to this style
    // uint8_t x = region->cells.at(i)._x;
    // uint8_t y = region->cells.at(i)._y;
    // this->setCell(x,y,region,sequence,&sequence->_notes.at(i), &region->cells.at(i));
  }

  // pad pattern vectors with zeroes if they are a different size so next block doesn't crash
  while(oldPattern.size() > newPattern.size()){
    Serial.println("adding a row to newpattern");
    newPattern.push_back(0);
  }
  while(newPattern.size() > oldPattern.size()){
    Serial.println("adding a row to oldpattern");
    oldPattern.push_back(0);
  }

  // remove newly unused cells 
  for(uint8_t i = 0; i < newPattern.size(); i++){
    if(newPattern.at(i) != oldPattern.at(i)){
      if(newPattern.at(i) < oldPattern.at(i)){
        for(uint8_t n = newPattern.at(i); n < oldPattern.at(i); n++){
          Serial.print(region->_bottomLeft._y + i);
          Serial.print(",");
          Serial.println(region->_bottomLeft._x + n);
          this->resetCell(region->_bottomLeft._x + n, region->_bottomLeft._y + i);
        }
      }
    }
  }
}

Sequence * RegionSequenceMediator::getAssociatedSequence(Region * region){
  Sequence * seq = NULL;
  for(auto & cell : region->cells){
      GridCell * gc = &this->cellNotes[(cell._y*8) + cell._x]; // todo fix for grid size
      if(gc->_region == region){
        seq = gc->_sequence;
        break;
      }
  }
  return seq;
}

bool RegionSequenceMediator::cellHasNotes(uint8_t x, uint8_t y){ 
  GridCell * gc = this->getCell(x,y);
  if(gc->note != NULL){
    return true;
  }
  return false;
}

bool RegionSequenceMediator::cellNoteIsPlaying(uint8_t x, uint8_t y){ // doesn't take into accout if cell doesn't exist
  if(this->cellNotes[(y*8)+x].note->playing == true){ // todo fix for grid size
    return true; 
  }
  return false;
}

GridCell * RegionSequenceMediator::getCell(uint8_t x, uint8_t y){ // todo fix for grid size
  return &this->cellNotes[(y*8) + x]; 
}

void RegionSequenceMediator::setCell(uint8_t x, uint8_t y, Region * r, Sequence * s, Note * n, Cell * c){
  GridCell * gc = this->getCell(x,y);
  gc->_region = r;
  gc->_sequence = s;
  gc->note = n;
  gc->cell = c;
}

void RegionSequenceMediator::resetCell(uint8_t x, uint8_t y){
  GridCell * gc = this->getCell(x,y);
  gc->_region = NULL;
  gc->_sequence = NULL;
  gc->note = NULL;
  gc->cell = NULL;
}

void RegionSequenceMediator::erase(Region * r){
  Sequence * s = this->getAssociatedSequence(r);
  this->cellNotes.erase(std::remove_if(this->cellNotes.begin(), this->cellNotes.end(), [&](GridCell gc){return gc._region == r;}), this->cellNotes.end());
}