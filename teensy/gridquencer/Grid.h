#ifndef G_GRID
#define G_GRID

#include "Cell.h"
#include "Region.h" 
#include "Sequence.h"
#include <vector>
#include <stddef.h>

struct GridCell {
  Cell cell;
  Region * _region = NULL;
  Sequence * _sequence = NULL;
};


class Grid {
  public: 
    Grid();
    bool addRegion(Cell start, Cell end);
    bool requestMoveRegion(Region* region, int dx, int dy);
    std::vector<GridCell> grid;
    Cell * _selectedCell;
    Region * _selectedRegion;
  private: 
    char _rows;
    char _columns;
    bool _allowOverlap;
    std::vector<Region> _regions;

};

#endif
