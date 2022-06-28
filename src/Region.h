#ifndef G_REGION
#define G_REGION

#include <vector>
#include "Cell.h"
#include <algorithm>

class Region{
  public: 
    Region();
    Region(Cell startPoint, Cell endPoint);
    std::vector<int> regionToVector(); // return [3,3,3] for a 3x3 grid
    int numberOfSteps();
    std::vector<Cell> cells;
    // std::vector<std::vector<Cell>> rows;
    // std::vector<Cell> steps;
  // private:
    int _beats;
    Cell _bottomLeft;
    Cell _bottomRight;
    Cell _topLeft;
    Cell _topRight;
};

#endif
