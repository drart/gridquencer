#ifndef G_GRID
#define G_GRID

#include "Cell.h"
#include "Region.h" 
#include "GridCell.h"
#include <vector>

class Grid {
  public: 
    Grid();
//  private: 
    char _rows;
    char _columns;
    std::vector<GridCell> grid;
    Cell _selectedCell;
    Region _selectedRegion;
    bool _allowOverlap;
};


#endif
