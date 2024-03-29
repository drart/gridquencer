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
        Sequence(std::vector<uint8_t>);
        Sequence(std::vector<uint8_t>, mode m);
        Note makeNote(uint8_t b, uint8_t n, uint8_t k);
        void modifyNote(Note *, uint8_t, uint8_t, uint8_t);
        // Sequence(sd::vector<uint8_t>, mode, std::vector<uint8_t> notecycle);
        void modify(std::vector<uint8_t>);
        void setMode(mode m); // set the subdivision mode
        void tick();
        bool mute;
        bool loop;
        bool playing;
        uint8_t midiChannel;

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
        std::vector<uint8_t> pattern;

        void printNote(Note);
};

#endif