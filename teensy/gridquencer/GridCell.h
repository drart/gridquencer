#ifndef G_GRIDCELL
#define G_GRIDCELL

#include <list>
#include "Cell.h"
#include "Region.h"

class GridCell: public Cell{
  public: 
    GridCell( char g_x, char g_y);
    std::list<Region> regions;
    std::list<int> sequences; // todo
};


#endif
