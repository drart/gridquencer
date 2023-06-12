#include "Grid.h"

#include "Arduino.h"

Grid::Grid(){
  this->_allowOverlap = false;
  this->_columns = 8;
  this->_rows = 8;
}

Grid::Grid(uint8_t x, uint8_t y){
  this->_rows = x;
  this->_columns = y;
  this->_allowOverlap = false;

  // TODO is this a better implementation? 
  uint8_t totalcells = x * y;
  for(uint8_t i = 0; i < totalcells; i++){
    this->_cellMap[i] = NULL;
  }
}

Cell * Grid::getCell(Cell cell){

  for(auto region : this->_regions){
    for( auto rc : region->cells){
      if(rc._x == cell._x && rc._y == cell._y){
        return &rc;
      }
    }
  }
  return NULL;
}

bool Grid::checkOverlap(Region * newRegion){ // todo better name
  for(auto r : this->_regions){
    if(r->doesOverlap(newRegion)){
      Serial.println("checking a region");
      return true;
    }
  }
  return false;
}

Region * Grid::getOverlappingRegion(Region * region){
  std::vector<Region*> overlappingRegions;
  
  for(auto r : this->_regions){ // TODO FIX
    if(r->doesOverlap(region)){
      overlappingRegions.push_back(r);
    }
  }

  if(overlappingRegions.size() == 1){
   return overlappingRegions.at(0);
  }
  return NULL;
}

bool Grid::addRegion(Region * newRegion){
  if( this->checkOverlap(newRegion) ){
    return false;
  }

  for(auto cell: newRegion->cells ){
    this->_cells.push_back( &cell );
  }

  this->_regions.push_back(newRegion);
  this->_selectedRegion = newRegion;
  return true;
}

bool Grid::requestMoveRegion(Region * _region, int dx, int dy){
  
  std::vector<Region> overlappingRegions;
  
  // check all cells in region for overlapping
  /*
  for ( size_t i = 0; i < _region->cells.size(); i++ ){

    GridCell * thecell = getCell( _region->cells[i]);   
    thecell->cell._x +=dx;
    thecell->cell._y +=dy; 
    std::vector<Region> linkedRegions;
    for( auto _region:_regions ){
      if(thecell->_region != NULL){
           //linkedRegions.push_back(_region);
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
*/
  return false;
}
