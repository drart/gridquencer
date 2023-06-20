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

  // TODO remove unused cells after modification
  // todo fix for the case where more rows are added

  for(uint8_t i = 0; i < region->cells.size(); i++){
    GridCell * gc = this->getCell(region->cells.at(i)._x, region->cells.at(i)._y);
    gc->cell = &region->cells.at(i);
    gc->note = &sequence->_notes.at(i);
    gc->_region = region;
    gc->_sequence = sequence;
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

// TODO BROKEN? 
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

GridCell * RegionSequenceMediator::getCell(uint8_t x, uint8_t y){
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