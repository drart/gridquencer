/// requires utils.js
//// todo 
// writes to hardware should instead call to state and let changeappliers work
fluid.defaults("adam.pushgridmapper", {
    gradeNames: ["fluid.modelComponent"], 
    model: {
        mode: "sequence", // notes, envelope
        action: "add", // delete, mute, select, solo, edit, ammend
        lastaction: undefined,
        selectedcell: undefined,
        lastselectedcell: undefined,
    },

    components: { 
        push: {
            type: "adam.midi.push",
        },
        sequencergrid: {
            type: "adam.grid",
        }
    },

    events: {
        regionCreated: null,
        selectcell: null,
        gridaction: null,
        overlapFound: null
    },

    notedown: undefined,

    modelListeners: {
        "{sequencergrid}.model.grid": {
            func: console.log,
            excludeSource: "init",
            priority: "last",
            args: 'grid change applier fired'
        },
    },

    listeners: {
        /*
        "{push}.events.onReady": {
            nameSpace: "setupKnobs",
            funcName: "adam.midi.push.knobsToString",
            args: '{push}'
        },
        */
        "{that}.events.overlapfound.print": {
            func: console.log,
        },
        "{push}.events.padPushed": {
            funcName: "adam.midi.push.gridNoteOn",
            args: ["{that}", "{arguments}.0", "{arguments}.1", "{arguments}.2", ]
        },
        "{push}.events.padReleased": {
            funcName: "adam.midi.push.gridNoteOff",
            args: ["{that}", "{arguments}.0", "{arguments}.1", "{arguments}.2", ]
        },
        "{sequencergrid}.events.gridChanged": {
            priority: "last",
            func: function(that){
                for(let r = 0; r < 8; r++){
                    for (let c = 0; c < 8 ; c++){
                        if ( that.sequencergrid.model.grid[r * 8 + c] !== undefined ){
                            that.push.padWrite(r, c);
                       }else{
                            that.push.padWrite(r, c, 0);
                        }
                    }
                }
            },
            args: "{that}"
        },

        /*
        // rethink this
        setdeletemode: {
            func: function(that, val){
                if( that.model.action === "delete"){
                    that.model.action = that.model.lastaction; 
                }else{
                    that.model.lastaction = that.model.action;
                    that.model.action  = "delete";
                }

                //console.log( that.model);
                
                if ( that.model.action === "delete" ) {
                    that.push.events.deletemodedisplay.fire(true);
                }else{
                    that.push.events.deletemodedisplay.fire(false);
                }
            },
            args: ["{that}", "{arguments}.0"]
        } ,
        */

        /*
       gridaction: { 
           func: function(that, cell){
               if(that.model.mode = "grid") that.gridmapping(cell);
               if(that.model.mode = "notes") that.notemapping(cell);
           },
           args: ["{that}"],
       },
       */

    },
});

///////  abstrsact to only define grid regions
adam.midi.push.gridNoteOn = function(that, pos, velocity){

    // todo check for overlapping
    // idea let orientation from origin determine reverse?
    // fire region {cells, origin, end}

    if (that.options.notedown !== undefined ){

        var startpoint, endpoint; 
        if (pos.column < that.options.notedown.column){
            startpoint = pos;
            endpoint = that.options.notedown;
        }else{
            endpoint = pos
            startpoint = that.options.notedown;
        };

        var stepz = [];
        var beats = endpoint.row - startpoint.row + 1;

        for (var r = startpoint.row; r <= endpoint.row; r++){
            if(endpoint.row !== startpoint.row){ 
                stepz.push([]);// mutli beat row
            }
            for (let c = startpoint.column; c <= endpoint.column; c++){
                let thecell = {};
                console.log(thecell); //// todo I do not understand how func shows up in thecell object after the next line
                thecell.location = {row: r, column: c}; 
                console.log(thecell);

                if(endpoint.row === startpoint.row){
                    stepz.push(thecell); // single beat sequence 
                    console.log(thecell);
                }else{

                    stepz[r-startpoint.row].push(thecell); //multi beat sequence
                    console.log(thecell);
                }
            }
        }

        ///// test overlap    if no overlap then create region, otherwise look for grid mofiication or highlight
        //console.log(stepz);
        if ( that.sequencergrid.checkzoneoverlap( stepz ) ){
            console.log('zone overlap, do seomthing rational here'); 
            // todo first overlapping cell should be the selectedcell
            // todo if the first cell of the new region is the first beat of an existing region then amend beat
            that.events.overlapFound.fire( stepz );
        }else{
            that.events.regionCreated.fire( stepz );
        }

        that.options.notedown = undefined;

    }else{

        that.options.notedown = pos;

    };
};

adam.midi.push.gridNoteOff = function(that, pos, velocity){

    if(that.options.notedown === undefined){ return; };

    if ( testTwoObjects(pos, that.options.notedown) ){ 
        // if single button is pressed, no region defined
        if ( that.sequencergrid.checkcelloverlap( pos )) {
            that.events.selectcell.fire( pos );
        }

    }else{
        console.log('appending a sequence');
        console.log(pos);
        //that.events.regionCreated.fire( stepz );
    }

    that.options.notedown = undefined;
};