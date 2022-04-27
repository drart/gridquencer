#ifndef G_GRIDCELL
#define G_GRIDCELL

#include <vector>
#include "Cell.h"
#include "Region.h"
// #include "Sequence.h"

class GridCell: public Cell{
  public: 
    GridCell( char g_x, char g_y);
    std::vector<Region> regions;
    std::vector<int> sequences; // todo
};


#endif
