#include "Region.h"

Region::Region(Cell firstPoint, Cell secondPoint){

  bottomLeft = Cell(std::min(firstPoint._x,secondPoint._x), std::min(firstPoint._y,secondPoint._y));
  topRight = Cell(std::max(firstPoint._x,secondPoint._x), std::max(firstPoint._y,secondPoint._y));
  topLeft = Cell(std::min(firstPoint._x,secondPoint._x), std::max(firstPoint._y,secondPoint._y));
  bottomRight = Cell(std::max(firstPoint._x,secondPoint._x), std::min(firstPoint._y,secondPoint._y));

  beats = 0;
  
    for(int i = bottomLeft._y; i <= topLeft._y; i++){
      for(int j = bottomLeft._x; j <= bottomRight._x; j++){
        Cell newCell = Cell(j,i);
        steps.push_back( newCell );
        rows[beats].push_back(newCell);
      }
      beats++;
    }
 
}
