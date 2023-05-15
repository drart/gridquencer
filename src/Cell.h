#ifndef G_CELL
#define G_CELL

#include <stdint.h>

class Cell {
  public: 
    Cell( char c_x, char c_y );
    Cell();
    uint8_t _x;
    uint8_t _y;
      
};

bool operator== (const Cell& c1, const Cell& c2);
bool operator!= (const Cell& c1, const Cell& c2);

#endif
