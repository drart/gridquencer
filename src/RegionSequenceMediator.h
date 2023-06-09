#ifndef G_REGIONSEQUENCE
#define G_REGIONSEQUENCE

#include <vector>
#include <map>

#include "Region.h"
#include "Sequence.h"

struct GridCell {
  Cell * cell;
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
    void modifySequence(Region * region);
    std::map<uint8_t, GridCell> cellNotes;
};

#endif