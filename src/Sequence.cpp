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
    this->_beats = inputvec.size();
    this->_ticksPerBeat = 480; // todo clean up
    this->_durationInTicks = (this->_ticksPerBeat * this->_beats);

    uint8_t beatz = 0;
    for(int value : inputvec){
        // this->_beats++;
        beatz++;
        int beatchop = 480 / value; // TODO clean up
        for(int i = 0; i < value; i++){
            Note n;
            n.pitch = 60;
            n.velocity = 127;
            n.velocity_deviation = 0;
            n.start_time = (float)beatz + (float)value*(1.0f/(float)value);
            n.duration = (1.0f/(float)value);
            n.probability = 127;
            n.mute = false;
            n.startIndex = (beatz-1)*this->_ticksPerBeat+ beatchop * i ;
            n.endIndex = n.startIndex + (int)(n.duration * this->_ticksPerBeat);
            n.endIndex = n.endIndex % this->_durationInTicks;
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