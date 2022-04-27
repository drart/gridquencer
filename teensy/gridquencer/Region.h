#ifndef G_REGION
#define G_REGION

#include <vector>
#include "Cell.h"
#include "GridCell.h"

class Region{
  public: 
    Region();
    Region(Cell start, Cell end);
  private:
    Cell bottomLeft;
    Cell bottomRight;
    Cell topRight;
    Cell topLeft;  
    std::vector<GridCell> _cells;
};


#endif
