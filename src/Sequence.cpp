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
    this->midiChannel = 10;

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
    this->midiChannel = 10;

    uint8_t numberOfNotes = std::accumulate(inputvec.begin(), inputvec.end(), 0);
    switch(this->subDivisionMode){
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
                n.start_time = (float)beatz + (i * n.duration);
                n.startIndex = (beatz-1)*this->_ticksPerBeat+ beatchop * i ;
                n.endIndex = n.startIndex + (uint16_t)(n.duration * this->_ticksPerBeat);
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

void Sequence::modify(std::vector<uint8_t> newPattern){ 
    for(uint8_t i = 0; i < newPattern.size(); i++){
        if(this->pattern.at(i) != newPattern.at(i)){
            uint8_t b = i + 1;
            Serial.println("beat modification needed");
            std::vector<Note>::iterator insertPoint = std::find_if(this->_notes.begin(), this->_notes.end(), [&](Note n){return (uint8_t)floor(n.start_time) == b;});
            
            if(this->pattern.at(i) < newPattern.at(i)){
                std::vector<Note> newNotes;
                for(uint8_t j = 0; j < this->pattern.at(i); j++){
                    this->modifyNote(&(*insertPoint), b, j, newPattern.at(i)); 
                    std::advance(insertPoint, 1);
                }
                for(uint8_t k = this->pattern.at(i); k < newPattern.at(i); k++ ){
                    Serial.println("making new notes");
                    Note n = this->makeNote(b, k, newPattern.at(i));
                    newNotes.push_back(n);
                }
                this->_notes.insert(insertPoint, newNotes.begin(), newNotes.end());
            }else{ // TODO UNTESTED
                for(uint8_t j = 0; j < newPattern.at(i); j++){
                    this->modifyNote(&(*insertPoint), b, j, newPattern.at(i)); // TODO FIX
                    std::advance(insertPoint, 1); 
                }
                this->_notes.erase(std::remove_if(insertPoint,this->_notes.end(), [&](Note n){return (uint8_t)floor(n.start_time) == b;}));
            }
        }
    }
    this->pattern = newPattern;
}

void Sequence::setMode(mode m){
}

Note Sequence::makeNote(uint8_t b, uint8_t n, uint8_t k) {
    Note note;
    note.pitch = 60;
    note.velocity = 127;
    note.velocity_deviation = 0;
    if (this->subDivisionMode == mode::QUARTER_TUPLET || this->subDivisionMode == mode::EIGHT_TUPLET || this->subDivisionMode == mode::SIXTEENTH_TUPLET || this->subDivisionMode == mode::THIRTYSECOND_TUPLET){
        uint16_t beatchop = this->_ticksPerBeat / k; /// overwrite ticksperbeat?
        note.duration = (1.0f / (float)k);
        note.start_time = (float)b + (note.duration * n);
        note.startIndex =  this->_ticksPerBeat * ((beatchop * n) + b);
        note.endIndex = note.startIndex + (uint16_t)(note.duration * this->_ticksPerBeat);
        note.endIndex = note.endIndex % this->_sequenceLengthInTicks;
    }else{
        note.start_time = (float)b + (float)(n * this->_noteDurationInTicks / 480); // TODO Fix
        note.duration = (float)this->_noteDurationInTicks / 480.0f; // TODO FIX
        note.startIndex = n * this->_noteDurationInTicks;
        note.endIndex = note.startIndex + this->_noteDurationInTicks;
        note.endIndex = note.endIndex % this->_sequenceLengthInTicks;
    }
    note.probability = 127;
    note.mute = false;
    note.playing = false;

    return note;
}

void Sequence::modifyNote(Note * note, uint8_t b, uint8_t n, uint8_t k){
     if (this->subDivisionMode == mode::QUARTER_TUPLET || this->subDivisionMode == mode::EIGHT_TUPLET || this->subDivisionMode == mode::SIXTEENTH_TUPLET || this->subDivisionMode == mode::THIRTYSECOND_TUPLET){
        uint16_t beatchop = this->_ticksPerBeat / k; /// overwrite ticksperbeat?
        note->duration = (1.0f / (float)k);
        note->start_time = (float)b + (note->duration * n);
        note->startIndex =  this->_ticksPerBeat * ((beatchop * n) + b);
        note->endIndex = note->startIndex + (uint16_t)(note->duration * this->_ticksPerBeat);
        note->endIndex = note->endIndex % this->_sequenceLengthInTicks;
    }else{
        note->start_time = (float)b + (float)(n * this->_noteDurationInTicks / 480); // TODO Fix
        note->duration = (float)this->_noteDurationInTicks / 480.0f; // TODO FIX
        note->startIndex = n * this->_noteDurationInTicks;
        note->endIndex = note->startIndex + this->_noteDurationInTicks;
        note->endIndex = note->endIndex % this->_sequenceLengthInTicks;
    } 
}