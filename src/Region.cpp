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
        cells.push_back( newCell );
        //rows.at(beats).push_back(newCell); // causes a crash!
      }
      beats++;
    }

}

std::vector<int> Region::regionToVector(){
  std::vector<int> thearray;
  int s = 1;
  thearray.push_back(s);
  if (cells.size() != 1) {
    for (int i = 1; i < (int)cells.size(); i++) {
      if((int)cells.at(i-1)._y != (int)cells.at(i)._y){
        thearray.push_back(s++);
      }
    }
  }
  return thearray;
}

int Region::numberOfSteps(){
  return (int)cells.size();
}