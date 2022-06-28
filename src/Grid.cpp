#include "Grid.h"
#include <vector>

Grid::Grid(){
  _allowOverlap = false;
  _columns = 8;
  _rows = 8;

  // grid.reserve()? 
  for( char x = 0; x < _columns; x++){
    for( char y = 0; y < _rows; y++){
      GridCell gridcell;
      gridcell.cell._x = x;
      gridcell.cell._y = y;
      grid.push_back( gridcell );
    } 
  }
}
GridCell Grid::getCell(Cell cell){

   return this->grid[(cell._y*8) + cell._x];
  
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


bool Grid::addRegion(Region newRegion){
  // TODO check grid for overlap
  _regions.push_back(newRegion);
}

bool Grid::requestMoveRegion(Region * _region, int dx, int dy){

  
  std::vector<Region> overlappingRegions;
  
  // check all cells in region for overlapping
  for ( int i = 0; i < _region->cells.size(); i++ ){

    GridCell thecell = getCell( _region->cells[i]);   
    thecell.cell._x +=dx;
    thecell.cell._y +=dy; 
    std::vector<Region> linkedRegions;
    for( auto _region:_regions ){
      if(thecell._region != NULL){
           linkedRegions.push_back(_region);
      }
    }
  
   std::vector<Region> regions = linkedRegions;
   overlappingRegions = linkedRegions;  
   overlappingRegions.insert(overlappingRegions.end(), 
   linkedRegions.begin(), linkedRegions.end());
  }


  if ( overlappingRegions.size() <= 1 ){

    for(int i=0; i < _region->cells.size(); i++){
        _region->cells[i]._x = _region->cells[i]._x + dx;
        _region->cells[i]._y = _region->cells[i]._y + dy;
    }

  }
  else{
    //Serial.println("Region move request denied: Overlapping other region");
  }
  

  return true;
}
