#ifndef G_SEQUENCE
#define G_SEQUENCE

#include "Region.h"
#include <vector>

class Sequence {
    public: 
        Sequence();
        std::vector<int> regionToSequence(Region * r);
        void tick();
        bool mute;
        bool loop;
        bool playing;

    private: 
        struct Note {
            float note;
            float probability;
            float velocity;
        };
        
        int _tickTime; // should be a long or some other 64bit type? 
        int _ticksPerBeat;
        int _currentStep;
        int beats;
        float _length;
        std::vector<Note> _notes;

};

#endif
