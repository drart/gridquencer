#ifndef G_REGION
#define G_REGION

#include <list>
#include "Cell.h"

class Region{
  public: 
    Region();
    Region(Cell start, Cell end);
    GridCell( char x, char y);
    std::list<Cell> cells;
};


#endif
