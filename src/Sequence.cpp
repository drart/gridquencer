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

    for(int value : inputvec){
        _beats++;

        for(int i = 0; i < value; i++){
            Note n;
            n.pitch = 60;
            n.velocity = 127;
            n.velocity_deviation = 0;
            n.start_time = (float)_beats + (float)value*(1.0f/(float)value);
            n.duration = (float)value*(1.0f/(float)value)*0.5f;
            n.probability = 127;
            n.mute = false;
            _notes.push_back(n);
        }
    }
}


void Sequence::tick(){
    _tickTime++;
}