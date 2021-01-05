// TODO fix all of the 8 magic numbers
fluid.defaults("adam.grid", {
    gradeNames: "fluid.modelComponent",
    model: {
        allowoverflow: false, // ie: zones must be unique
        grid: [], // 0-64
        selectedcell: null, // maybe? 
    },
    rows: 8,
    columns: 8,
    modelListeners: {
    }, 
    events: {
        gridChanged: null
    },
    invokers: {
        /*
           addzone: { 
        //TODO:  should this function take a zone or should it also define the zone?
        func: function(that, startpos, endpos ){
        if (that.model.allowoverflow){
        // allow all additions to the grid
        return true;
        }else{
        // if region doesn't overlap then add
        if(endpos !== undefined){
        return that.checkzoneoverlap(startpos, endpos);
        }else{
        return that.checkcelloverlap(startpos);
        }
        }
        },
        args: ["{that}", "{arguments}.0", "{arguments}.1"]
        },
        */
        addcell: { // todo fix to grid size?
            func: function(that, pos, ref = true){
                let oldgrid = that.model.grid;
                that.model.grid[pos.row*8 + pos.column] = ref;
            },
            args: ["{that}", "{arguments}.0", "{arguments}.1"]
        },
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
        selectcell: {
            func: function(that, cell){
                that.model.selectedcell = cell;
            },
            args: ["{that}", "{arguments}.0"]
        },
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
                for (var i = 0; i < (that.model.rows * that.model.columns); i++){
                    that.model.grid[i] = undefined;
                }
                that.model.oldgrid = that.model.grid;
            },
            args: ["{that}"]
        },
    }
});

// todo future model?
//// have regions marked? 
fluid.defaults("adam.grid.region", {
    gradeNames: "fluid.modelComponent",
    model: {
        steps: []
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

