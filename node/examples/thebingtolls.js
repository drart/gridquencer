flock.synth({
    synthDef: {
        ugen: "flock.ugen.sum",
        sources: [
            {
                ugen: "flock.ugen.sinOsc",
                freq: 400
            },
            {
                ugen: "flock.ugen.sinOsc",
                freq: 405
            }
        ],
        mul: {
            ugen: "flock.ugen.asr",
            start: 0.0,
            attack: 0.0001,
            sustain: 0.25,
            release: 2.0,
            gate: {
                ugen: "flock.ugen.impulse",
                rate: "control",
                freq: 0.3,
                phase: 1.0
            }
        }
    }
});
