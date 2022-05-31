#include "Sequence.h"

Sequence::Sequence(){
    loop = false;
    _tickTime = 0;

};



Sequence::Sequence(std::vector<int>){
    loop = true;
}