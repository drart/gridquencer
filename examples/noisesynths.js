fluid.defaults("adam.synth.whiteNoiseDrum", {
    gradeNames: ["flock.synth"],

    synthDef: {
        ugen: "flock.ugen.whiteNoise",
        mul: {
            id: "envelope",
            ugen: "flock.ugen.envGen",
            envelope:{
                levels: [0, 1, 0],
                times: [0.03, .5],
                curve: ["exponential, exponential"],
            },
            gate: {
                ugen: "flock.ugen.timedGate",
                duration: 0.6,
                trigger: {
                    id: "trig",
                    ugen: "flock.ugen.valueChangeTrigger"
                }
            }
        },
    },
    invokers: {
        trig: {
            func: "{that}.set",
            args: ["trig.source", Math.random()]
        }
    },
    listeners: {
        "noteOn.trigger": "{that}.trig"
    } 
});



// lfNoise .. freq,  whiteNoise, pinkNoise, dust .. density,