#include "Region.h"

Region::Region(Cell firstPoint, Cell secondPoint){

  _bottomLeft = Cell(std::min(firstPoint._x,secondPoint._x), std::min(firstPoint._y,secondPoint._y));
  _topRight = Cell(std::max(firstPoint._x,secondPoint._x), std::max(firstPoint._y,secondPoint._y));
  _topLeft = Cell(std::min(firstPoint._x,secondPoint._x), std::max(firstPoint._y,secondPoint._y));
  _bottomRight = Cell(std::max(firstPoint._x,secondPoint._x), std::min(firstPoint._y,secondPoint._y));

  _beats = 0;
  
    for(int i = _bottomLeft._y; i <= _topLeft._y; i++){
      //std::vector<Cell> therow; // work on this. it should solve the issue. 
      for(int j = _bottomLeft._x; j <= _bottomRight._x; j++){
        Cell newCell = Cell(j,i);
        cells.push_back( newCell );
        //rows.at(beats).push_back(newCell); // causes a crash!
      }
      _beats++;
      // rows.push_back(therow);
    }

}

std::vector<int> Region::regionToVector(){
  std::vector<int> thearray;
  int s = 1;
  if (cells.size() != 1) {
    for (int i = 1; i < (int)cells.size(); i++) {
      if((int)cells.at(i-1)._y != (int)cells.at(i)._y){
        thearray.push_back(s);
        s = 1;
      }else{
        s++;
      }
    }
  }
  thearray.push_back(s);
  return thearray;
}

uint8_t Region::numberOfSteps(){
  return (int)cells.size();
}


bool Region::modify(Region * region){
  Serial.println(region->cells.at(0)._x);
  Serial.println(this->_bottomLeft._x);
  if(region->cells.at(0)._x != this->_bottomLeft._x){
    return false;
  }

  uint8_t cellcolumn = region->cells.at(0)._x;
  Serial.println(cellcolumn);
  this->cells.erase( std::remove_if(this->cells.begin(), this->cells.end(), [&](Cell c){return c._x == cellcolumn;}), this->cells.end());
  for( auto & cell : region->cells ){
    if(cell._y != cellcolumn){
      cellcolumn = cell._y;
      Serial.println(cellcolumn);
      this->cells.erase( std::remove_if(this->cells.begin(), this->cells.end(), [&](Cell c){return c._y == cellcolumn;}), this->cells.end());
    }
  }
  this->cells.insert(this->cells.end(), region->cells.begin(), region->cells.end());
  return true;
}