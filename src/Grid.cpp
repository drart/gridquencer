#include "Grid.h"

Grid::Grid(){
  this->_allowOverlap = false;
  this->_columns = 8;
  this->_rows = 8;
}

Cell * Grid::getCell(Cell cell){
  for(auto & c : this->_cells){
    if( *c == cell ){
      return c;
    }
  }
  return NULL;
}

bool Grid::checkOverlap(Region * newRegion){ 
  for(auto & cell : newRegion->cells){
    if(this->getCell(cell) != NULL){
      return false;
    }
  }
  return true;
}

Region* Grid::getOverlappingRegion(Region * region){
  std::vector<Region*> overlappingRegions;
  
  // TODO FIX
  for(auto r : this->_regions){
    for(auto  c : region->cells){
      for(auto cell : region->cells){
        if(c == cell){
          overlappingRegions.push_back(r);
          continue; // this doesn't escape to the level I'd like
        }
      }
    }
  }

  if(overlappingRegions.size() == 1){
   return overlappingRegions.at(0);
  }
  return NULL;
}

bool Grid::addRegion(Region * newRegion){
  if( !this->checkOverlap(newRegion) ){
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
