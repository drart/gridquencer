#include "Sequencer.h"

Sequencer::Sequencer(){
    // _resolution = 480;
    // this->bpm(60.0f);
}

Sequencer::Sequencer(float bpm){
    this->bpm(bpm);
}

Sequencer::Sequencer(float bpm, float resolution){
    this->_resolution = resolution;
    this->bpm(bpm);
}

void Sequencer::bpm(float bpm){
    this->_bpm = bpm;
    this->_beatPeriod = (60.0f / _bpm );  
    this->_period = _beatPeriod / _resolution * _microsPerSecond;
}

void Sequencer::tick(){
    for( Sequence *seq : this->_sequences ){
        seq->tick();
    }
    this->_tickTime++;
    if(this->_tickTime % this->_resolution == 0){
        for( Sequence *seq : this->_sequencesToStart ){
            this->_sequences.push_back(seq);
        }
        this->_sequencesToStart.clear();
    }
}

bool Sequencer::addSequence(Sequence * s){
    this->_sequences.push_back(s);
    return true;
}

bool Sequencer::queueSequence(Sequence * s){
    this->_sequencesToStart.push_back(s);
    return true;
}

float Sequencer::getPeriod(){
    return this->_period;
}

Sequencer::~Sequencer(){
    // TODO delete _sequences and queued sequences
}