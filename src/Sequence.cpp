#include "Sequence.h"

Sequence::Sequence(){
    // this->loop = false;
    // this->_tickTime = 0;
};

Sequence::Sequence(std::vector<int> inputvec){
    this->loop = true;
    this->mute = false;
    this->_tickTime = 0;
    this->playing = false;
    this->_beats = 0;
    this->_ticksPerBeat = 480; // todo clean up

    for(int value : inputvec){
        this->_beats++;
        int beatchop = 480 / value; // TODO clean up
        for(int i = 0; i < value; i++){
            Note n;
            n.pitch = 60;
            n.velocity = 127;
            n.velocity_deviation = 0;
            n.start_time = (float)_beats + (float)value*(1.0f/(float)value);
            n.duration = (float)value*(1.0f/(float)value)*0.5f;
            n.probability = 127;
            n.mute = false;
            n.startIndex = (_beats-1)*480 + beatchop * i ;
            n.endIndex = n.startIndex + (int)(n.duration * _ticksPerBeat);
            this->_notes.push_back(n);
        }
    }
}

void Sequence::tick(){
    this->_tickTime++;
    if(this->_tickTime == this->_beats * this->_ticksPerBeat ){
        this->_tickTime = 0;
    }
}