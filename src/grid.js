// TODO fix all of the 8 magic numbers
fluid.defaults("adam.grid", {
    gradeNames: "fluid.modelComponent",
    model: {
        rows: 8,
        columns: 8,
        grid: [], 
        regions: [],
        selectedcell: null, 
        selectedregion: null,
        allowoverlap: false, 
    },

    events: {
        gridchanged: null
    },

    invokers: {

        // old
        checkoverlap: {
            func: function( that, thing){
                // does it overlap? 
                // is it a particular kind of overlap? 
            },
            args: ["{that}", "{arguments}.0"]
        },

        addcell: { // todo fix to grid size?
            func: function(that, cell, ref = true){
                that.model.grid[cell.model.x*8 + cell.model.y] = ref;
            },
            args: ["{that}", "{arguments}.0", "{arguments}.1"]
        },

        // old
        getcell: {
            func: function(that, cell){
                if(cell === undefined){ // todo: maybe not?
                    return that.model.grid[ that.model.selectedcell.row * that.options.columns + that.model.selectedcell.column];
                }else{
                    return that.model.grid[cell.row * that.options.columns  + cell.column];
                }
            },
            args: ["{that}", "{arguments}.0"]
        },

        addregion: {
            func: function(that, region){
                if (that.model.allowoverlap){
                    // todo make a way to have multiple regions in a grid location
                    that.model.regions.push(region);
                }else{
                    // todo checkoverlap
                    that.model.regions.push(region);
                }
                for (step of region.model.steps){
                    // todo cell step to grid size? 
                    that.model.grid[step.model.x*8+step.model.y] = region; 
                }
                that.events.gridchanged.fire();

            },
            args: ["{that}", "{arguments}.0"]
        },

        /*
        removeregion: {},
        */

        selectregion: {
            func: function(that, region){
                that.model.selectedregion = region;
            },
            args: ["{that}", "{arguments}.0"]
        },

        selectcell: {
            func: function(that, cell){
                that.model.selectedcell = cell;
            },
            args: ["{that}", "{arguments}.0"]
        },


        // old
       checkzoneoverlap:{ 
           func: function(that, stepz){
               for ( key of Object.keys( stepz )){
                   if( Array.isArray( stepz[key] )){
                       for (subkey of Object.keys(stepz[key])){
                           if ( that.checkcelloverlap( stepz[key][subkey].location )){
                               console.log('zone overlap');
                               return true;
                           }
                       }
                   }else{
                       if ( that.checkcelloverlap( stepz[key].location )){
                           console.log('zone overlap');
                           return true;
                       }
                   }
               }
               console.log ('no zone overlap');
               return false;
           },
           args: ["{that}", "{arguments}.0"]
       },
       checkcelloverlap: {
            func: function(that, cell){
                if( that.model.grid[cell.row*that.options.columns + cell.column] === undefined ){ 
                    return false;
                }else{
                    return true;
                }
            },
            args: ["{that}", "{arguments}.0"]
        },
        /*
        checkexactoverlap: {
            func: function( that, cellz ){
              console.log(' not implemented yet' ); 
            },
            args: ["{that}", "{arguments}.0"]
        },
        */
        getoverlap: {
            func: function(that, cellz){
                let foundcells = [];
                for ( key of Object.keys( cellz ) ){
                    if (Array.isArray( cellz[key] ) ){
                        for ( subkey of Object.keys( cells[key] ) ){
                            
                        }
                    }else{

                    }
                }
                return foundcells;
            },
            args: ["{that}", "{arguments}.0"]
        },

        // old
        // TODO also remove from region?
        removecell: {
            func: function(that, cell){
                that.model.grid[cell.row*that.options.columns + cell.column] = undefined;
            },
            args: ["{that}", "{arguments}.0"]
        }
    },

    listeners: {
        onCreate: {
            func: function(that){
                for (var i = 0; i <  that.model.rows * that.model.columns; i++){
                    that.model.grid[i] = undefined;
                }
                // todo do I need this? 
                //that.model.oldgrid = that.model.grid;
            },
            args: ["{that}"]
        },
    }
});



/// old idea, still ok?
//------------------------------------------
// grid model
//------------------------------------------
/*
fluid.defaults("adam.testgrid", {
   gradeNames:  "fluid.modelComponent",
   numPads: 64,
   model: {
   pads: "@expand:adam.grid.initEmptyGrid({that}.options.numPads)"
//                 pads: {
//                     "0": true,
//                     "1": false,
//                     "2": true
//                }
},
modelListeners: {
"pads.*": {
    namespace: "sendMIDIPadState",
    funcName: "adam.padStateChange", // connect this to adam.push?
    args: ["{change}.path.0", "{change}.value"]
    }
    }
});

adam.grid.initEmptyGrid = function (numPads) {
    var pads = [];
    for (var i = 0; i < numPads; i++) {
    pads[i] = false;
    }
    // return pads;  // ??
};

adam.grid.padStateChange = function (padIdx, value) {
    adam.push.sendMidiForStateChange(padIdx, value);
};
*/