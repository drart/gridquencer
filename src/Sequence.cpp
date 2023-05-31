#include "Sequence.h"
#include <numeric>

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
    this->_sequenceLengthInTicks = (this->_ticksPerBeat * this->_beats);

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
            n.endIndex = n.endIndex % this->_sequenceLengthInTicks;
            n.playing = false;
            this->_notes.push_back(n);
        }
    }
}


Sequence::Sequence(std::vector<int> inputvec, mode m){

    this->loop = true;
    this->mute = false;
    this->_tickTime = 0;
    this->playing = false;
    this->_beats = inputvec.size();
    this->_ticksPerBeat = 480; // todo clean up

    int totalbeats = std::accumulate(inputvec.begin(), inputvec.end(), 0);
    switch(m){
        case mode::QUARTER:
        this->_sequenceLengthInTicks = (totalbeats * this->_ticksPerBeat);
        this->_noteDurationInTicks = 480;
        // noteLengthInRatios?
        break;
        case mode::QUARTER_TUPLET:
        this->_sequenceLengthInTicks = (this->_ticksPerBeat * this->_beats);
        break;
        case mode::EIGHTH:
        this->_sequenceLengthInTicks = (totalbeats * this->_ticksPerBeat / 2);
        this->_noteDurationInTicks = 240;
        break;
        case mode::EIGHT_TUPLET: 
        this->_sequenceLengthInTicks = (this->_ticksPerBeat * this->_beats);
        break;
        case mode::SIXTEENTH:
        this->_sequenceLengthInTicks = (totalbeats * this->_ticksPerBeat / 4);
        this->_noteDurationInTicks = 120;
        break;
        case mode::SIXTEENTH_TUPLET:
        this->_sequenceLengthInTicks = (this->_ticksPerBeat * this->_beats);
        break;
        case mode::THIRTYSECOND:
        this->_sequenceLengthInTicks = (totalbeats * this->_ticksPerBeat / 8);
        this->_noteDurationInTicks = 60;
        break;
        case mode::THIRTYSECOND_TUPLET:
        this->_sequenceLengthInTicks = (this->_ticksPerBeat * this->_beats);
        break;
    }

    uint8_t beatz = 0;
    uint8_t notecount = 0;
    for(int value : inputvec){
        beatz++;
        for(int i = 0; i < value; i++){
            Note n;
            n.pitch = 60;
            n.velocity = 127;
            n.velocity_deviation = 0;
            if(m == mode::QUARTER_TUPLET || m == mode::EIGHT_TUPLET || m == mode::SIXTEENTH_TUPLET || m == mode::THIRTYSECOND_TUPLET){
                int beatchop = this->_ticksPerBeat / value;  /// overwrite ticksperbeat? 
                n.duration = (1.0f/(float)value);
                n.startIndex = (beatz-1)*this->_ticksPerBeat+ beatchop * i ;
                n.endIndex = n.startIndex + (int)(n.duration * this->_ticksPerBeat);
                n.endIndex = n.endIndex % this->_sequenceLengthInTicks;
            }else{
                n.start_time = (float)beatz + (float)(notecount*this->_noteDurationInTicks/480);
                n.duration = (float)this->_noteDurationInTicks/480.0f;
                n.startIndex = notecount*this->_noteDurationInTicks;
                n.endIndex = n.startIndex + this->_noteDurationInTicks;
                n.endIndex = n.endIndex % this->_sequenceLengthInTicks;
                notecount++;
            }
            n.probability = 127;
            n.mute = false;
            n.playing = false;
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