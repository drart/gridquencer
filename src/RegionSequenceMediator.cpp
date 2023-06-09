#include "RegionSequenceMediator.h"

RegionSequenceMediator::RegionSequenceMediator(){
}

std::vector<u_int8_t> RegionSequenceMediator::regionToVector(Region * region){
  std::vector<uint8_t> thearray;
  uint8_t s = 1;
  if (region->cells.size() != 1) {
    for (uint8_t i = 1; i < (uint8_t)region->cells.size(); i++) {
      if((uint8_t)region->cells.at(i-1)._y != (uint8_t)region->cells.at(i)._y){
        thearray.push_back(s);
        s = 1;
      }else{
        s++;
      }
    }
  }
  thearray.push_back(s);

  return thearray;
}

Sequence * RegionSequenceMediator::regionToSequence(Region * region, mode subdivisionMode){

  std::vector<uint8_t> thearray = this->regionToVector(region);

  Sequence * newSequence =  new Sequence(thearray, subdivisionMode);

  for(uint8_t i = 0; i < region->cells.size(); i++){
    GridCell gc;
    gc.cell = &region->cells.at(i);
    gc.note = &newSequence->_notes.at(i);
    gc._region = region;
    gc._sequence = newSequence;

    this->cellNotes.at(gc.cell->_y*8 + gc.cell->_x) = gc;
  }

  return newSequence;
}

void RegionSequenceMediator::modifySequence(Region * region){
  // get the sequence associated with this region
  // figure out what changed
  // modify
}