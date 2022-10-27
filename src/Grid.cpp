#include "Grid.h"
#include <vector>

Grid::Grid(){
  _allowOverlap = false;
  _columns = 8;
  _rows = 8;

  // todo grid.reserve()? 
  for( char y = 0; y < _rows; y++){
    for( char x = 0; x < _columns; x++){
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

bool Grid::checkOverlap(Region newRegion){
  for ( auto & cell : newRegion.cells ){
    if( !this->grid.at(cell._y*8 + cell._x).memberOf.empty() ){
      return false;
    }
  }
  return true;
}

bool Grid::addRegion(Region newRegion){
  if( !this->checkOverlap(newRegion) ){
    return false;
  }

  for(auto &cell: newRegion.cells ){
    GridCell newCell;
    newCell.cell = cell;
    newCell._region = &newRegion;
    newCell.memberOf.push_back(newRegion);
    this->grid.at(cell._y*8 + cell._x) = newCell;
  }
  _regions.push_back(newRegion);
  _selectedRegion = &newRegion;
  return true;
}

bool Grid::requestMoveRegion(Region * _region, int dx, int dy){
  
  std::vector<Region> overlappingRegions;
  
  // check all cells in region for overlapping
  for ( size_t i = 0; i < _region->cells.size(); i++ ){

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
  // todo check that new places are within grid size
  if ( overlappingRegions.size() <= 1 ){
    for(size_t i=0; i < _region->cells.size(); i++){
        _region->cells[i]._x = _region->cells[i]._x + dx;
        _region->cells[i]._y = _region->cells[i]._y + dy;
    }
  }else{
    //Serial.println("Region move request denied: Overlapping other region");
    return false;
  }

  return true;
}
