#include "Sequencer.h"

Sequencer::Sequencer(){
}

void Sequencer::bpm(float bpm){
    _bpm = bpm;
    _beatPeriod = (60.0f / _bpm );  
    _period = _beatPeriod * _microsPerSecond /  _resolution;
}

void Sequencer::tick(){
    // if ticktime/tickinterval == 0
    if( !_sequencesToStart.empty() ){
        // add queued sequences 
    }

    for( auto &seq : _sequences ){
        seq.tick();
    }
}

bool Sequencer::addSequence(Sequence s){
    _sequences.push_back(s);
    return true;
}

bool Sequencer::queueSequence(Sequence s){
    _sequencesToStart.push_back(s);
    return true;
}

float Sequencer::getPeriod(){
    return _period;
}