#ifndef G_SEQUENCER
#define G_SEQUENCER

#include <vector>
#include "Sequence.h"
#include "IntervalTimer.h"

class Sequencer {
    public: 
        Sequencer();
        Sequencer(float bpm)
        start();
        pause();
        stop();
        bpm(float bpm);
        addSequence(Sequence s); 
        removeSequence(Sequence s);
        queueSequence(Sequence s);
        muteSequence(Sequence s);
        selectSequence(Sequence s);


    private: 
        IntervalTimer _seqTimer;
        tick();

        float _bpm;
        std::vector<Sequence> _sequences;
        std::vector<Sequence> _sequencesToStart;
        Sequence _selectedSequence;

        int _beatlength;
        int _tickTime; // should be a long or some other 64bit type? 
}

#endif
