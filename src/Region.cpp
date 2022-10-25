#include "Region.h"

Region::Region(Cell firstPoint, Cell secondPoint){

  _bottomLeft = Cell(std::min(firstPoint._x,secondPoint._x), std::min(firstPoint._y,secondPoint._y));
  _topRight = Cell(std::max(firstPoint._x,secondPoint._x), std::max(firstPoint._y,secondPoint._y));
  _topLeft = Cell(std::min(firstPoint._x,secondPoint._x), std::max(firstPoint._y,secondPoint._y));
  _bottomRight = Cell(std::max(firstPoint._x,secondPoint._x), std::min(firstPoint._y,secondPoint._y));

  _beats = 0;
  
    for(int i = _bottomLeft._y; i <= _topLeft._y; i++){
      for(int j = _bottomLeft._x; j <= _bottomRight._x; j++){
        Cell newCell = Cell(j,i);
        cells.push_back( newCell );
        //rows.at(beats).push_back(newCell); // causes a crash!
      }
      _beats++;
    }

}

// TODO improve vector representation of regions
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