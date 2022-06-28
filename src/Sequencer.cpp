#include "Sequencer.h"

Sequencer::Sequencer(){
}

void Sequencer::bpm(float bpm){
    _bpm = bpm;
    _beatPeriod = (60 / _bpm ) * microsPerSecond;  
    _period = _beatPeriod / _resolution;
}


void Sequencer::tick(){
    // if ticktime/tickinterval == 0
    // if(_sequencesToStart.size() != 0 ){
    //     // 
    // }

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
    return _beatPeriod;
}