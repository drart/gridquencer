#ifndef G_CELL
#define G_CELL

class Cell {
  public: 
    Cell( char c_x, char c_y );
    Cell();
    char _x;
    char _y;  
};

bool operator== (const Cell& c1, const Cell& c2);
bool operator!= (const Cell& c1, const Cell& c2);

#endif
