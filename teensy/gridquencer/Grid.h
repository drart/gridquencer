#ifndef G_GRID
#define G_GRID

#include "Cell.h"
#include "Region.h" 
#include "GridCell.h"
#include <list>

class Grid {
  public: 
    Grid();
    char _rows;
    char _columns;
    std::list<GridCell> grid;
    Cell _selectedCell;
    Region _selectedRegion;
    bool _allowOverlap;
};


#endif
