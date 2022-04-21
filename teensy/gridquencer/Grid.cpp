#include "Grid.h"


Grid::Grid(){
  for( char x = 0; x < _columns; x++){
    for( char y = 0; y < _rows; y++){
      GridCell cell = GridCell(x,y);
      //char i = x + ( y * _columns );
      grid.push_back( cell );
    } 
  }
}
