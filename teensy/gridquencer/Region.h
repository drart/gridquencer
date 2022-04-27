#ifndef G_REGION
#define G_REGION

#include <vector>
#include "Cell.h"

class Region{
  public: 
    Region();
    Region(Cell start, Cell end);
    std::vector<int> regionToVector(); // return [3,3,3] for a 3x3 grid
  private:
    Cell bottomLeft;
    Cell bottomRight;
    Cell topRight;
    Cell topLeft;  
    std::vector<Cell> _cells;
};


#endif
