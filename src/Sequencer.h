#ifndef G_SEQUENCER
#define G_SEQUENCER

#include "Sequence.h"
#include "IntervalTimer.h"

class Sequencer {
    public: 
        Sequencer();
        Sequencer(float bpm);
        void start();
        void pause();
        void stop();
        float bpm(float bpm);
        bool addSequence(Sequence * s); 
        bool queueSequence(Sequence * s);
        bool removeSequence(Sequence * s);
        bool muteSequence(Sequence * s);
        Sequence& selectSequence(Sequence * s);


    private: 
        IntervalTimer _seqTimer;
        void tick();

        float _bpm;
        std::vector<Sequence> _sequences;
        std::vector<Sequence> _sequencesToStart;
        Sequence _selectedSequence;

        int _beatlength;
        int _tickTime; // should be a long or some other 64bit type? 
};

#endif
