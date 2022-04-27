#ifndef G_REGION
#define G_REGION

#include <vector>
#include "Cell.h"
#include "GridCell.h"

class Region{
  public: 
    Region();
    Region(Cell startPoint, Cell endPoint);
  private:
    std::vector<Cell> cells;
    int beats;
    std::vector<std::vector<Cell>> rows;
    std::vector<Cell> steps;
    Cell bottomLeft;
    Cell bottomRight;
    Cell topLeft;
    Cell topRight;
};


#endif
