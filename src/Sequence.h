#ifndef G_SEQUENCE
#define G_SEQUENCE

#include "Region.h"

struct Note
{
    // ableton compatible fields
    float pitch;              // 0-127
    float velocity;           // 0-127
    float velocity_deviation; // 0-127
    float start_time;         // beats ex 1.5 starts on the & of beat 1
    float duration;           // beats
    float probability;        // 0-1
    bool mute;                //
    // gridquencer fields
    int startIndex;
    int endIndex;
    bool playing;
};

class Sequence {
    public: 
        Sequence();
        Sequence(std::vector<int>);
        void tick();
        bool mute;
        bool loop;
        bool playing;

    // private: 
        int _tickTime; // should be a long or some other 64bit type? 
        int _ticksPerBeat;
        int _beats;
        // float _length;
        // int _currentStep;
        std::vector<Note> _notes;
};

#endif