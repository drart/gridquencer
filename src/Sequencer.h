#ifndef G_SEQUENCER
#define G_SEQUENCER

#include "Sequence.h"

class Sequencer {
    public: 
        Sequencer();
        Sequencer(float bpm);
        Sequencer(float bpm, float resolution);
        ~Sequencer();
        void start();
        void pause();
        void stop();
        void bpm(float bpm);
        bool addSequence(Sequence * s); 
        bool queueSequence(Sequence * s);
        bool removeSequence(Sequence * s);
        bool muteSequence(Sequence * s);
        Sequence& selectSequence(Sequence * s);
        float getPeriod();

        float _microsPerSecond = 1000000.0f;
//    private: 
        void tick();

        float _bpm;
        float _period;
        float _beatPeriod;
        int _resolution;

        std::vector<Sequence*> _sequences;
        std::vector<Sequence*> _sequencesToStart;
        Sequence * _selectedSequence;

        int _beatlength;
        int _tickTime; // should be a long or some other 64bit type? 
};

#endif
