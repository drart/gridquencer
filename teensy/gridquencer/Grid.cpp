#include "Grid.h"


Grid::Grid(){
  for( char x = 0; x < _columns; x++){
    for( char y = 0; y < _rows; y++){
      GridCell gridcell;
      gridcell.cell._x = x;
      gridcell.cell._y = y;
      grid.push_back( gridcell );
    } 
  }
}

bool Grid::addRegion(Cell start, Cell end){

/*
  if ( start.x == end.x && start.y == end.y ){
    // region with one cell
    // if grid cell has no regions then add this to the regions and add reference to the gridCell
    // _regions.push_back( Region(start) );
    // grid[cell]->_region = &newRegion;
    
  }
  
  // find bottom left and top right

    for all cells in region
     grid.getCell(x,y) == cell 
     if not then return false 
     if true then add region to grid and add region reference to each gridcell
  */

  return true;
}
