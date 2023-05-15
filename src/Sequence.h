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
    uint16_t startIndex;
    uint16_t endIndex;
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
        uint16_t _tickTime; 
        uint16_t _ticksPerBeat;
        uint16_t _beats;
        uint16_t _durationInTicks;
        // float _length;
        // int _currentStep;
        std::vector<Note> _notes;
};

#endif