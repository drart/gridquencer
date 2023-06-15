#include "RegionSequenceMediator.h"

#include "Arduino.h"  

RegionSequenceMediator::RegionSequenceMediator(){
}

Sequence * RegionSequenceMediator::regionToSequence(Region * region, mode subdivisionMode){

  std::vector<uint8_t> thearray = region->regionToVector();

  Sequence * newSequence =  new Sequence(thearray, subdivisionMode);

  for(uint8_t i = 0; i < region->cells.size(); i++){
    GridCell gc;
    gc.cell = &region->cells.at(i);
    gc.note = &newSequence->_notes.at(i);
    gc._region = region;
    gc._sequence = newSequence;

    this->cellNotes[(gc.cell->_y*8) + gc.cell->_x] = gc; // todo fix for grid size
  }

  return newSequence;
}

void RegionSequenceMediator::modifySequence(Region * region, Sequence * sequence){ // TODO subdivision mode? 
  std::vector<uint8_t> oldPattern = sequence->pattern;
  sequence->modify(region->regionToVector());
  std::vector<uint8_t> newPattern = sequence->pattern;

  uint8_t i = 0;
  for(uint8_t beat = 0; beat < newPattern.size(); beat++){
    if(oldPattern.at(beat) != newPattern.at(beat)){
      Serial.println("change this row?");
      if(oldPattern.at(beat) < newPattern.at(beat)){
        i += oldPattern.at(beat); 
        for(uint8_t n = oldPattern.at(beat); n < newPattern.at(beat); n++){ 
          Serial.println(i);
          GridCell gc;
          gc.cell = &region->cells.at(i);
          gc.note = &sequence->_notes.at(i);
          gc._region = region;
          gc._sequence = sequence;
          this->cellNotes[(gc.cell->_y*8) + gc.cell->_x] = gc; // TODO Fix for grid size
        
          Serial.print(gc.cell->_x);
          Serial.print(",");
          Serial.println(gc.cell->_y);
          Serial.println(gc.note->start_time);
          i++;
        }
      }else{
        /// TODO FIX? 
        i += newPattern.at(beat); 
      }
    }else{
      i += newPattern.at(beat);
    }
  }
  // if this.cellnotes has a cell that points to this region and isn't in region then delete it
  // this->cellNotes.erase(std::remove_if(this->cellNotes.begin(), this->cellNotes.end(), [&](GridCell gc){return region->containsCell(gc.cell);}));
}

Sequence * RegionSequenceMediator::getAssociatedSequence(Region * region){
  Sequence * seq = NULL;
  for(auto & cell : region->cells){
    if(this->cellNotes.find((cell._y*8) + cell._x) != this->cellNotes.end()){ // todo fix for grid size
      GridCell * gc = &this->cellNotes[(cell._y*8) + cell._x]; // todo fix for grid size
      if(gc->_region == region){
        seq = gc->_sequence;
        break;
      }
    }
  }
  return seq;
}

// TODO BROKEN? 
bool RegionSequenceMediator::cellHasNotes(uint8_t x, uint8_t y){ // todo fix for size of grid
  if(this->cellNotes.find((y*8)+x) != this->cellNotes.end()){
    return true;
  }
  return false;
}

bool RegionSequenceMediator::cellNoteIsPlaying(uint8_t x, uint8_t y){ // doesn't take into accout if cell doesn't exist
  if(this->cellNotes[(y*8)+x].note->playing){ // todo fix for grid size
    return true; 
  }
  return false;
}