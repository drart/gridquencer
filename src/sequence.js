fluid.defaults("adam.sequence", {
    gradeNames: "fluid.modelComponent",

    model: {
        steps: {},
        beats: 1, // do I really need this? sequencelength?
        beatlength: 480, // todo rename to tickresolution? beatticks? ticksPerBeat?
        target: null, 
        mute: false,
        loop: false,
        sync: "tempo", // should a sequence start immediately or have a way of getting into sync?
        direction: "forward", // reverse, random, random-ish, nan
        playing: false,
        addingsequencetoselect: true, // todo move this to sequencer
        sequenceticks: 0,
        ticktime: 0, 
        //offset: 0,
        currentstep: undefined,
        previousstep: undefined,
        // steps are either change appliers for synth.set
        // or json {"func":"name", "args",[]} invoking the target
    },

    invokers: {
        settarget: {
            func: function(that, target){
                that.model.target = target;
            },
            args: ["{that}", "{arguments}.0"]
        },

        tick: {
            func: function (that){
                that.model.ticktime++;
                // switch (direction){}
            },
            args: ["{that}"]
        },

        /*
        setstep: { // todo: this isn't used yet?
            func: function(that, step, payload){ // number of step, json object
                that.model.steps[step] = payload;
            },
            args: ["{that}", "{arguments}.0", "{arguments}.1"]
        },
        */

        setlocationpayload:{
            func: function(that, loc, payload){
                if (loc === undefined || payload === undefined){
                    console.log("error: incomplete call to set payload");
                    return;
                }
                for( s in that.model.steps ){
                    console.log( that.model.steps[s] );
                    if ( that.model.steps[s].location.row === loc.row && that.model.steps[s].location.column === loc.column){
                        that.model.steps[s].args= payload;
                        console.log("bingo");
                        console.log(that.model.steps[s].args);
                    }
                }
            },
            args: ["{that}", "{arguments}.0", "{arguments}.1"]
        },

        getlocationpayload: {
            func: function( that, loc ){
                //console.log( that.getStepFromLocation( loc ) );
                let step = that.getStepFromLocation( loc );
                return step.args;
            },
            args: [ "{that}", "{arguments}.0"]
        },
        
        arraytosequence: {
            func: function(that, arr){
                let beatlength = that.model.beatlength;
                if (!Array.isArray(arr)){
                    console.log('warning: arrays must be used for sequences');
                    return -1;
                }

                // 480 is divisible by many divisions up to 20 without being too unwieldly for the clock
                if( Array.isArray(arr[0])){ // multibeat sequence
                    that.model.beats = arr.length;
                    for (var b = 0; b < arr.length; b++){
                        const steplength = Math.floor(beatlength / arr[b].length);
                        for (var i = 0; i < arr[b].length; i++){
                            //console.log( (steplength *i) + (beatlength * b));
                            that.model.steps[ (steplength * i) + (beatlength * b) ] = arr[b][i];
                        }
                    }
                }else{ // single beat sequence
                    const steplength = Math.floor(beatlength / arr.length);
                    for(var i = 0; i < arr.length; i++){
                        that.model.steps[steplength * i] = arr[i];
                    }
                }

                that.model.sequenceticks = that.model.beats * that.model.beatlength;
            },
            args: ["{that}", "{arguments}.0", "{arguments}.1"]
        },
        /*
           retrigger: function(){}, // placeholder
           applyoffset: function(){}, // placeholder
           reversesequence: { // todo :  finish
           func: function(that, seq){
           if (!seq){
           console.log('warning: no sequence available to reverse');
           return;
           };
           var keys = Object.keys(that.model.steps);
           for (var i = 0; i < Math.floor(keys.length/2); i++){
           var temp = that.model.steps[i];
           console.log(temp);
           }
           console.log(that.model.steps);
           },
           args: ["{that}"]
           },
           */

        clearsequence: {
            func: function(that){
                that.model.steps = {};
            },
            args: "{that}"
        },

        clearBeat: {
            func: function(that,  beat = 0){
                if (beat > that.model.beats - 1 ) beat = that.model.beats - 1;
                let removedbeats = [];
                for ( let key in that.model.steps ){
                    if ( key >= beat * 480 && key < (beat * 480) + 480){
                        removedbeats.push( that.model.steps[key] );
                        delete that.model.steps[key];
                    }
                }
                return removedbeats;
            },
            args: ["{that}", "{arguments}.0"]
        },

        getStepFromLocation: {
            func: function(that, loc ){
                let thestep
                for ( key in that.model.steps ){
                    if (testTwoObjects( that.model.steps[key].location, loc) ){
                        thestep =  that.model.steps[key];
                        break;
                    }
                }
                return thestep;
            },
            args: ["{that}", "{arguments}.0"]
        },

        isStepOnBeat: {
            func: function( that, step ){
                for (key in that.model.steps ){
                    if ( testTwoObjects( that.model.steps[key].location, step.location )){
                        if ( key % 480 === 0 ){
                            return true;
                        }else {
                            return false; 
                        }
                    }
                }
            },
            args: ["{that}", "{arguments}.0"]
        },

        getStepBeat: { /// steps need location, func, args
            func: function(that, step){
                for ( key in that.model.steps ) {
                    if ( testTwoObjects( that.model.steps[key], step )){
                        return parseInt(key) / 480 ;
                    } 
                }
            },
            args: ["{that}", "{arguments}.0"]
        },

        reviseBeat: {
            func: function(that, seq, beat = 0){ // multibeats?
                if (beat > that.model.beats - 1 ) beat = that.model.beats - 1;
                //// todo if seq.model.beatlength > 1 then clear multiple beats
                let removedsteps = that.clearBeat(beat);

                for ( let key in seq.model.steps ){
                    that.model.steps[ parseInt(key) + beat * 480 ] = seq.model.steps[key];
                }
                return removedsteps;
            },
            args: ["{that}", "{arguments}.0", "{arguments}.1"]
        },

        reviseNumberBeats: {
            func: function (that, seq, numberOfBeats=1){
                // todo  // add some more steps or remove steps
                return;
            },
            args: []
        }

    }
});
