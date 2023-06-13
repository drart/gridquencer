#include "Region.h"

Region::Region(Cell firstPoint, Cell secondPoint){

  this->_bottomLeft = Cell(std::min(firstPoint._x,secondPoint._x), std::min(firstPoint._y,secondPoint._y));
  this->_topRight = Cell(std::max(firstPoint._x,secondPoint._x), std::max(firstPoint._y,secondPoint._y));
  this->_topLeft = Cell(std::min(firstPoint._x,secondPoint._x), std::max(firstPoint._y,secondPoint._y));
  this->_bottomRight = Cell(std::max(firstPoint._x,secondPoint._x), std::min(firstPoint._y,secondPoint._y));

  this->_beats = 0;
  
    for(uint8_t i = _bottomLeft._y; i <= _topLeft._y; i++){
      //std::vector<Cell> therow; // work on this. it should solve the issue. 
      for(uint8_t j = _bottomLeft._x; j <= _bottomRight._x; j++){
        Cell newCell = Cell(j,i);
        this->cells.push_back( newCell );
        //rows.at(beats).push_back(newCell); // causes a crash!
      }
      this->_beats++;
      // rows.push_back(therow);
    }

}

std::vector<uint8_t> Region::regionToVector(){
  std::vector<uint8_t> thearray;
  uint8_t s = 1;
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
  return (uint8_t)cells.size();
}

bool Region::modify(Region * region){
  if( !this->leftSideAligned(region) ){ //TODO TEST
    return false;
  }

  uint8_t cellrow = region->cells.at(0)._y;
  std::vector<Cell>::iterator insertPoint = std::find_if(this->cells.begin(), this->cells.end(), [&](Cell c){return c._y == cellrow;});
  this->cells.erase( std::remove_if(this->cells.begin(), this->cells.end(), [&](Cell c){return c._y == cellrow;}), this->cells.end());
  for( auto & cell : region->cells ){
    if(cell._y != cellrow){
      cellrow = cell._y;
      this->cells.erase( std::remove_if(this->cells.begin(), this->cells.end(), [&](Cell c){return c._y == cellrow;}), this->cells.end());
    }
  }
  this->cells.insert(insertPoint, region->cells.begin(), region->cells.end()); 
  return true;
}

bool Region::containsCell(Cell * c){
  for(auto cell : this->cells){
    if(c->_x == cell._x && c->_y == cell._y){
      return true;
    }
  }
  return false;
}

bool Region::doesOverlap(Region * r){
  for(auto c : r->cells){
    if(this->containsCell(&c)){
      return true;
    }
  }
  return false;
}

bool Region::leftSideAligned(Region * r){
  if(r->_bottomLeft._x == this->_bottomLeft._x){
    return true;
  }
  return false;
}