#ifndef G_REGION
#define G_REGION

#include <vector>
#include "Cell.h"

class Region{
  public: 
    Region();
    Region(Cell start, Cell end);
  private:
    Cell bottomLeft;
    Cell bottomRight;
    Cell topRight;
    Cell topLeft;  
    std::vector<Cell> _cells; // move this to GridCell
};


#endif
