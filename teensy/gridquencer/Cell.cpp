#include "Cell.h"

Cell::Cell(char c_x, char c_y){
  _x = c_x;
  _y = c_y;
}

Cell::Cell(){
  _x = 0;
  _y = 0;
}

bool operator== (const Cell& c1, const Cell& c2){
  return (c1._x == c2._x && c1._y == c2._y);
}

bool operator!= (const Cell& c1, const Cell& c2){
  return (c1._x != c2._x || c1._y != c2._y);
}
