flock.synth({
    synthDef: {
        ugen: "flock.ugen.granulator",
        delayDur: {
            ugen: "flock.ugen.lfNoise",
            freq: 1, 
            add: 0.5
        },
        numGrains: {
            ugen: "flock.ugen.lfNoise",
            freq: 2,
            mul: 75,
            add: 150,
            options: {
                interpolation: "linear"
            }
        },
        source: {
        ugen: "flock.ugen.sum",
        sources: [
            {
                ugen: "flock.ugen.sinOsc",
                freq: 400
            },
            {
                ugen: "flock.ugen.sinOsc",
                freq: {
                    ugen: "flock.ugen.sin",
                    rate: "control",
                    freq: .25,
                    add: 405,
                    mul: 5
                }
            }
        ],        }
    }
});
