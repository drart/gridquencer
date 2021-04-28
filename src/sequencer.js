// TODO remove all of the grid work from this and move it to gridsequencer
fluid.defaults("adam.sequencer",{
    gradeNames: ["flock.synth", "fluid.modelComponent"],

    model: {
        bpm: 60,
        beatlength: 480,
        ticktime: 0,
        sequences: [],
        selectedsequence: null,
        sequencestostart: [],
    },

    components: { 
        thegrid: {
            type: "adam.grid",
        }
    },

    synthDef: {
        ugen: "flock.ugen.triggerCallback",
        trigger: {
            id: "pulse",
            ugen: "flock.ugen.impulse",
            freq: 480 // 240 
        },
        options: {
            callback: {
                funcName: "adam.sequencer.engine",
                args: ["{that}"]
            }
        }
    },

    events: {
        barline: null,
        resync: null,
        beat: null,
    },

    listeners: {
        "beat.turnSequencesOn": {
            func: function(that){
                for ( s in that.model.sequencestostart){
                    that.model.sequencestostart[s].model.playing = true; 
                }
                that.model.sequencestostart = [];
            },
            args: "{that}"
        }
    },

    invokers: {
        setTempo: {
            funcName: "adam.sequencer.setTempo",
            args: ["{that}", "{arguments}.0"]
        },
        getsequence: { 
            funcName: "adam.sequence.getSequence",
            args: ["{that}", "{arguments}.0"]
        },

        addsequence: {
            func: function(that, seq){
                that.model.sequencestostart.push( seq );

                if(seq.model.addingsequencetoselect === true){
                    that.model.selectedsequence = seq ; 
                    console.log('squence was selected by adding');
                }

                that.model.sequences.push(seq);

                return true;
            },
            args: ["{that}", "{arguments}.0"]
        },


        removesequence: {
            func: function(that, seq){
                var deletedsequence;
                if (seq === undefined){ 
                    console.log('trying to delete sequence that does not exist');
                }else{
                    deletedsequence = that.getsequence(seq);
                    /*
                    for (key in deletedsequence.model.steps){
                        let step = deletedsequence.model.steps[key];
                        that.thegrid.removecell (step.location );
                    }
                    that.model.sequences.splice( that.model.sequences.indexOf(deletedsequence), 1);
                    that.thegrid.events.gridChanged.fire(); 
                    */
                }
            },
            args: ["{that}", "{arguments}.0"]
        },

        popsequence: {
            funcName: "{that}.model.sequences.pop"
        },
        mutesequence: {
            funcName: "adam.sequencer.muteSequence",
            args: ["{that}", "{arguments}.0"]
        },
        selectsequence: {
            funcName: "adam.sequencer.selectSequence",
            args: ["{that}", "{arguments}.0"]
        }
    }
});

adam.sequencer.engine = function (that) {

    if (that.model.ticktime % that.model.beatlength === 0) {
        that.events.beat.fire(that.model.ticktime / that.model.beatlength);
    }

    for (let s of that.model.sequences) {

        let thetick;
        if (s.model.playing === true) {

            thetick = (s.model.loop === true) ? that.model.ticktime % s.model.sequenceticks : that.model.ticktime;

            if (s.model.mute === true) {
                s.model.ticktime++;
                continue;
            }

        } else {
            continue;
        }

        /*
        // TODO current step considering sequence direction
        */

        if (s.model.steps[thetick] !== undefined) {
            const payload = s.model.steps[thetick].payload;
            const target = s.model.target;
            s.model.currentstep = s.model.steps[thetick];
            target[payload.func](payload.args);
        }
        s.model.ticktime++; /// change this to tick 
    }
    that.model.ticktime++;

};

adam.sequencer.setTempo = function (that, bpm) {
    that.model.bpm = bpm;
    that.set("pulse.freq", that.model.bpm / 60 * that.model.beatlength);
    console.log(bpm);
};

adam.sequencer.selectSequence = function (that, seq) {
    if (that.getsequence(seq) !== -1) {
        that.model.selectedsequence = seq;
        return true;
    } else {
        return false;
    }
};

adam.sequencer.muteSequence = function (that, seq) {
    that.getsequence(seq).mute = true;
    return true;
};

adam.sequencer.getSequence = function (that, seq) {
    let result = that.model.sequences.indexOf(seq);

    if (result !== -1) {
        result = that.model.sequences[result];
        return result;
    } else {
        return undefined;
    }
};