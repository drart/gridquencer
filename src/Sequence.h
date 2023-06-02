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


enum class mode {
    QUARTER,
    QUARTER_TUPLET,
    EIGHTH,
    EIGHT_TUPLET,
    SIXTEENTH,
    SIXTEENTH_TUPLET,
    THIRTYSECOND,
    THIRTYSECOND_TUPLET
};

class Sequence {
    public: 
        Sequence();
        Sequence(std::vector<int>);
        Sequence(std::vector<int>, mode m);
        void modify(std::vector<int>);
        void changeMode(mode m); // set the subdivision mode
        void tick();
        bool mute;
        bool loop;
        bool playing;

    // private: 
        uint16_t _tickTime; 
        uint16_t _ticksPerBeat;
        uint16_t _beats;
        uint16_t _noteDurationInTicks;
        uint16_t _sequenceLengthInTicks;
        // float _length;
        // int _currentStep;
        std::vector<Note> _notes;
        mode subDivisionMode;
};

#endif