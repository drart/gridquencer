flock.synth({
    synthDef: {
        ugen: "flock.ugen.filter.moog",
        cutoff: 2000,
        resonance: {
            ugen: "flock.ugen.sin",
            mul: 2,
            add: 5,
            freq: 0.13
        },
        source: {
            ugen: "flock.ugen.dust",
            density: {
                ugen: "flock.ugen.sin",
                mul: 300,
                add: 500,
                freq: .1
            },
            mul: 2
        },
        mul: 2
    }
});
