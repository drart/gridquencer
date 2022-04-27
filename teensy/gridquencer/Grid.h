#ifndef G_GRID
#define G_GRID

#include "Cell.h"
#include "Region.h" 
#include "GridCell.h"
#include <vector>

class Grid {
  public: 
    Grid();
    bool addRegion(Cell start, Cell end);
  private: 
    char _rows;
    char _columns;
    bool _allowOverlap;

    std::vector<GridCell> grid;
    std::vector<Region> _regions;
    
    Cell & _selectedCell;
    Region & _selectedRegion;
};


#endif
