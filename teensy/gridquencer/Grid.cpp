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

bool Grid::addRegion(Cell start, Cell end){

  if ( start.x == end.x && start.y == end.y ){
    // region with one cell
    // if grid cell has no regions then add this to the regions and add reference to the gridCell
    // _regions.push_back( Region(start) );
    // grid.thecell._regions.push_back( newRegion );
    
  }
  
  // find bottom left and top right
  

  return true;
}
