#include "Sequence.h"

Sequence::Sequence(){
    loop = false;
    _tickTime = 0;

};

Sequence::Sequence(std::vector<int> inputvec){
    loop = true;
    mute = false;
    _tickTime = 0;
    playing = false;
    _beats = 1;

    for(const auto& value: inputvec){
        Note n = {60,127,0,(float)_beats + (float)value*(1/(float)value), 1.0/(float)value, 1.0, true};
        _notes.push_back(n);
        _beats++;
    }

}


void Sequence::tick(){
    _tickTime++;
}