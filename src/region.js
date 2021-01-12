fluid.defaults("adam.grid.region", {
    gradeNames: "fluid.modelComponent",

    model: {
        beats: undefined,
        startpoint: undefined,
        endpoint: undefined,
        steps: []
    },

    invokers: {
        normalize: {
            func: function(){},
            args: []
        },
        createFromCells:{
            funcName: "adam.grid.region.createFromCells",
            args: ["{that}", "{arguments}.0"]
        }
    }
});

adam.grid.region.createFromCells = function(that, cells){

    if (cells.length === 0 ){return;}

    if (cells.length === 1){
        that.model.startpoint = cells[0];
        that.model.endpoint = cells[0];
        that.model.steps = cells;
        that.model.beats = 1;
    }

    if (cells.length === 2 ){
        that.model.startpoint = cells[0];
        that.model.endpoint = cells[1];
       
        var bottomleft, bottomright, topleft, topright;
        // for range of points
        // thecell = adam.grid.cell();
        // thecell.createFromPos( {row: r, column: c} );
        
    
    }

    if (cells.length === 3){

    }
};