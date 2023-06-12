#ifndef G_GRID
#define G_GRID

#include <vector>
#include <map>
#include <stddef.h>

#include "Cell.h"
#include "Region.h" 

class Grid {
  public: 
    Grid();
    Grid(uint8_t, uint8_t);
    bool addRegion(Region * newRegion);
    bool requestMoveRegion(Region * region, int dx, int dy);
    bool checkOverlap(Region * newRegion);
    Region * getOverlappingRegion(Region * region);
    Cell * getCell(Cell cell);
    std::vector<Region*> _regions;
    std::vector<Cell*> _cells;
    std::map<uint8_t, Cell*> _cellMap;
    Cell * _selectedCell;
    Region * _selectedRegion;
  // private: 
    uint8_t _rows;
    uint8_t _columns;
    bool _allowOverlap;
};

#endif