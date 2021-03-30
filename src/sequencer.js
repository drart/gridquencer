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
                func: function(that){
                    if (that.model.ticktime % that.model.beatlength === 0){
                        //console.log(that.model.ticktime);
                        that.events.beat.fire( that.model.ticktime / that.model.beatlength );                        
                    }
                    /*
                    if (that.model.ticktime % that.model.beatlength === that.model.beatlength / 2 ) {
                        for ( let s of that.model.sequences){
                            if ( Object.keys(s.model.steps).length === 1 && s.model.playing === true){
                                // hack 
                                //that.push.padWrite( s.model.steps[0].location.row, s.model.steps[0].location.column );

                                // hack todo make a better solution
                                if (s.model.steps[0].args.type === "noteOn"){
                                    let noteOff = fluid.copy(s.model.steps[0].args);
                                    noteOff.type = "noteOff";
                                    s.model.target[s.model.steps[0].func](noteOff);
                                }
                                // end noteoff hack
                            }
                        }
                    }
                    */

                    for (let s of that.model.sequences){
                        
                        let thetick;
                        if (s.model.playing === true){

                            thetick = (s.model.loop === true) ? that.model.ticktime % s.model.sequenceticks: that.model.ticktime;
                        
                            if(s.model.mute === true){
                                s.model.ticktime++;
                                continue;
                            }
                            
                        }else{
                            continue;
                        }

                        /*
                        // TODO current step considering sequence direction
                        */

                        if ( s.model.steps[thetick] !== undefined ){
                            const payload = s.model.steps[thetick].payload;
                            const target = s.model.target;
                            s.model.currentstep = s.model.steps[thetick];
                            target[payload.func](payload.args);
                            // todo if grid exisits then upadte it with payload location to be highlighted? 
                            // todo move the pad updates to a separate function
                            /*
                            if(that.sequencergrid !== undefined){
                                that.push.padWrite( payload.location.row, payload.location.column, 30);
                                if (s.model.previousstep){
                                    that.push.padWrite( s.model.previousstep.location.row, s.model.previousstep.location.column );
                                }
                            }

                            if(payload.func){
                                target[payload.func](payload.args);
                            }else{
                                target.set(payload);
                            }
                            

                            if ( Object.keys(s.model.steps).length > 1){
                                s.model.previousstep = s.model.steps[thetick];
                            }
                            */
                        }
                        s.model.ticktime++; /// change this to tick 
                    }
                    that.model.ticktime++;
                },
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
            func: function(that, bpm){
                that.model.bpm = bpm;
                that.set("pulse.freq", that.model.bpm/60 * that.model.beatlength);
                console.log( bpm );
            },
            args: ["{that}", "{arguments}.0"]
        },
        getsequence: { 
            func: function(that, seq){
                let result = that.model.sequences.indexOf(seq);

                if( result !== -1 ){
                    result = that.model.sequences[ result ]; 
                    return result;
                }else{
                    return undefined;
                }
            },
            args: ["{that}", "{arguments}.0"]
        },
        addsequence: {
            func: function(that, seq){
                if(that.thegrid === null){
                    console.log('null grid');
                    return false;
                }
                /* this checking for overlap has been put into the grid mapping instead
                }else{
                    if ( that.thegrid.checkzoneoverlap( seq.model.steps ) ){

                        // get the overlapping sequence  // no logic for overlapping sequences
                        // set the selected cell to be the first overlapping cell
                        let keys = Object.keys(seq.model.steps);
                        let foundseq = null; 
                        for ( let key of keys ){
                            foundseq = that.thegrid.getcell( seq.model.steps[key].location );
                            if (foundseq){
                                /// todo , make this selection an option
                                that.thegrid.model.selectedcell = seq.model.steps[key].location;
                                console.log("selected cell is " ); 
                                console.log(that.thegrid.model.selectedcell);

                                if (key === keys[0]){ /// if the first cell is first beat of found sequence then revise it
                                    let step = foundseq.getStepFromLocation( seq.model.steps[0].location ); 
                                    let beat = foundseq.getStepBeat( step );
                                    let removedbeats = foundseq.reviseBeat(seq, beat);

                                    //if (that.push){ /// todo bad solution
                                    //    for(step of removedbeats){
                                    //        that.push.padWrite( step.location.row, step.location.column, 0 );
                                    //        console.log( step.location );
                                    //    }
                                    //}
                                    //return true;

                                }
                                break; 
                            } 
                        }
                        return false;
                    }
                };
                */

                /*
                // if no overlap put into grid with reference to sequence 
                for( let key of Object.keys(seq.model.steps)){
                    let loc = seq.model.steps[key].location;
                    that.thegrid.addcell( loc, seq );
                }
                */

                //seq.model.currentstep = 0;
                //seq.model.playing = true; // todo: defer this to actual playing?

                that.model.sequencestostart.push( seq );

                if(seq.model.addingsequencetoselect === true){that.model.selectedsequence = seq ; console.log('squence was selected by adding');};

                that.model.sequences.push(seq);

                return true;
            },
            args: ["{that}", "{arguments}.0"]
        },
        popsequence: {
            func: function(that){
                let thesequence = that.model.sequences.pop();
                if (thesequence === undefined) {
                    return thesequence;
                }

                /*
                for ( key in thesequence.model.steps ){ // or just pass on to remove? 
                    let step = thesequence.model.steps[key];
                    that.thegrid.removecell( step.location );
                }
                that.thegrid.events.gridChanged.fire(); 
                */

                return thesequence;
            },
            args: "{that}"
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
        mutesequence: {
            func: function(that, seq){
                that.getsequence(seq).mute = true;
                return true;
            },
            args: ["{that}", "{arguments}.0"]
        },
        selectsequence: {
            func: function(that, seq){
                if ( that.getsequence(seq) !== -1 ){ 
                    that.model.selectedsequence = seq;
                    return true;
                }else{
                    return false;
                }
            },
            args: ["{that}", "{arguments}.0"]
        }
    }
});

