#ifndef G_REGIONSEQUENCE
#define G_REGIONSEQUENCE

#include <vector>
#include <map>
#include <algorithm>

#include "Grid.h"
#include "Region.h"
#include "Sequence.h"

struct GridCell {
  Cell * cell = NULL;
  Note * note = NULL;
  Region * _region = NULL;
  Sequence * _sequence = NULL;
};

class RegionSequenceMediator{
  public: 
    RegionSequenceMediator();
    std::vector<uint8_t> regionToVector(Region * region);
    std::vector<uint8_t> sequenceToVector(Sequence * sequence);
    Sequence * regionToSequence(Region * region, mode subdivisionMode);
    Sequence * getAssociatedSequence(Region *);
    void modifySequence(Region * region, Sequence * sequence);
    // std::map<uint8_t, GridCell> cellNotes;
    std::vector<GridCell> cellNotes;

    Grid grid; 
    
    bool cellHasNotes(uint8_t x, uint8_t y);
    bool cellNoteIsPlaying(uint8_t x, uint8_t y);

    GridCell * getCell(uint8_t x, uint8_t y);
    void setCell(uint8_t x, uint8_t y, Region * r, Sequence * s, Note * n, Cell * c);
    void resetCell(uint8_t x, uint8_t y);
};

#endif