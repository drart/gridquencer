#include "Sequence.h"
#include <numeric>

Sequence::Sequence(){
    // this->loop = false;
    // this->_tickTime = 0;
};

Sequence::Sequence(std::vector<uint8_t> inputvec){
    this->loop = true;
    this->mute = false;
    this->_tickTime = 0;
    this->playing = false;
    this->_beats = inputvec.size();
    this->_ticksPerBeat = 480; // todo clean up
    this->_sequenceLengthInTicks = (this->_ticksPerBeat * this->_beats);
    this->subDivisionMode = mode::SIXTEENTH_TUPLET;
    this->pattern = inputvec;

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


Sequence::Sequence(std::vector<uint8_t> inputvec, mode m){

    this->loop = true;
    this->mute = false;
    this->_tickTime = 0;
    this->playing = false;
    this->_beats = inputvec.size();
    this->_ticksPerBeat = 480; // todo clean up
    this->subDivisionMode = m;
    this->pattern = inputvec;

    uint8_t numberOfNotes = std::accumulate(inputvec.begin(), inputvec.end(), 0);
    switch(m){
        case mode::QUARTER:
        this->_noteDurationInTicks = 480;
        this->_sequenceLengthInTicks = (numberOfNotes* this->_noteDurationInTicks);
        break;
        case mode::QUARTER_TUPLET:
        this->_ticksPerBeat *= 4;
        this->_sequenceLengthInTicks = (this->_ticksPerBeat * this->_beats);
        break;
        case mode::EIGHTH:
        this->_noteDurationInTicks = 240;
        this->_sequenceLengthInTicks = (numberOfNotes* this->_noteDurationInTicks);
        break;
        case mode::EIGHT_TUPLET: 
        this->_ticksPerBeat *= 2;
        this->_sequenceLengthInTicks = (this->_ticksPerBeat * this->_beats);
        break;
        case mode::SIXTEENTH:
        this->_noteDurationInTicks = 120;
        this->_sequenceLengthInTicks = (numberOfNotes* this->_noteDurationInTicks);
        break;
        case mode::SIXTEENTH_TUPLET:
        this->_sequenceLengthInTicks = (this->_ticksPerBeat * this->_beats);
        break;
        case mode::THIRTYSECOND:
        this->_noteDurationInTicks = 60;
        this->_sequenceLengthInTicks = (numberOfNotes* this->_noteDurationInTicks);
        break;
        case mode::THIRTYSECOND_TUPLET:
        this->_ticksPerBeat /= 2;
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
    if(this->_tickTime == this->_sequenceLengthInTicks){
        this->_tickTime = 0;
    }
}

void Sequence::modify(std::vector<uint8_t> newbeat){
    for(uint8_t i = 0; i < newbeat.size(); i++){
        if(this->pattern.at(i) == newbeat.at(i)){

        }
    }
    this->pattern = newbeat;
}

void Sequence::changeMode(mode m){
    /// take mode and then redistibute the durations of the notes
}


// create some methods that populate the notes in different ways