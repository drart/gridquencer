#ifndef G_GRID
#define G_GRID

#include "Cell.h"
#include "Region.h" 
#include "Sequence.h"
#include <vector>
#include <stddef.h>

struct GridCell {
  Cell cell;
  Note * note = NULL;
  Region * _region = NULL;
  Sequence * _sequence = NULL;
  std::vector<Region> memberOf;
};

class Grid {
  public: 
    Grid();
    bool addRegion(Region newRegion);
    bool requestMoveRegion(Region * region, int dx, int dy);
    bool checkOverlap(Region newRegion);
    GridCell* getCell(Cell cell);
    std::vector<GridCell> grid;
    Cell * _selectedCell;
    Region * _selectedRegion;
  // private: 
    char _rows;
    char _columns;
    bool _allowOverlap;
    std::vector<Region> _regions;

};

#endif
