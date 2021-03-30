fluid.defaults("adam.grid.region", {
    gradeNames: "fluid.modelComponent",

    model: {
        beats: undefined,
        startpoint: undefined,
        endpoint: undefined,
        steps: [] // should this be cells instead?
        // direction: lefttoright, righttoleft
    },

    invokers: {
        createFromCells:{
            funcName: "adam.grid.region.createFromCells",
            args: ["{that}", "{arguments}.0"]
        },
        checkOverlap: {
            funcName: "adam.grid.region.checkOverlap",
            args: ["{that}", "{arguments}.0"]
        },
        equals: {
            funcName: "adam.grid.region.equals",
            args: ["{that}", "{arguments}.0"]
        }, 
        getRow: {
            funcName: "adam.grid.region.getRow",
            args: ["{that}", "{arguments}.0"]
        },
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

        if (that.model.startpoint.model.x <= that.model.endpoint.model.x){
            if(that.model.startpoint.model.y <= that.model.endpoint.model.y){
                bottomleft = that.model.startpoint;
                topright = that.model.endpoint;
                topleft = adam.grid.cell();
                bottomright = adam.grid.cell();
                bottomright.createFromPos({row: that.model.startpoint.model.x, column: that.model.endpoint.model.y});
                topleft.createFromPos({row: that.model.endpoint.model.x, column: that.model.startpoint.model.y});
            }else{
                bottomright = that.model.startpoint;
                topleft = that.model.endpoint;
                bottomleft = adam.grid.cell();
                topright = adam.grid.cell();
                bottomleft.createFromPos({row: that.model.startpoint.model.x, column: that.model.endpoint.model.y });
                topright.createFromPos({row: that.model.endpoint.model.x, column: that.model.startpoint.model.y});
            }
        }else{
            if (that.model.startpoint.model.y <= that.model.endpoint.model.y){
                bottomright = that.model.endpoint;
                topleft = that.model.startpoint;
                bottomleft = adam.grid.cell();
                bottomleft.createFromPos({row: that.model.endpoint.model.x, column: that.model.startpoint.model.y});
                topright = adam.grid.cell();
                topright.createFromPos({row: that.model.startpoint.model.x, column: that.model.endpoint.model.y});
            }else{
                topright = that.model.startpoint;
                bottomleft = that.model.endpoint;
                topleft = adam.grid.cell();
                bottomright = adam.grid.cell();
                bottomright.createFromPos({row: that.model.endpoint.model.x, column: that.model.startpoint.model.y});
                topleft.createFromPos({row: that.model.startpoint.model.x, column: that.model.endpoint.model.y});
            }
        }

        //console.log(bottomleft.model, bottomright.model, topleft.model, topright.model);
        that.model.beats = 0;
        for(let i = bottomleft.model.x; i <= topleft.model.x; i++){
            for(let j = bottomleft.model.y; j <= bottomright.model.y; j++){
                let thecell = adam.grid.cell();
                thecell.createFromPos({row: j,column: i});
                that.model.steps.push(thecell);
                console.log(thecell.model);
            }
            that.model.beats++;
        }
        //console.log(that.model.steps);

    }

    if (cells.length === 3){
        // do the same as above but amend one row based on the third touch
        // adam.grid.region.createFromCells(that, [cells[0], cells[1]]);
    }
};


adam.grid.region.checkOverlap = function(that, region){
    console.log(region);
    console.log('warning: checkoverlap not properly implemented yet');
    return true;
};

adam.grid.region.equals = function(that, region){
    if(region.model.steps.length !== that.model.steps.length){
        return false;
    }
    for (var i = 0; i < region.model.steps.length; i++){
        if ( !region.model.steps[i].equals(that.model.steps[i]) ){
            return false;
        }
    }
    return true;
};


adam.grid.region.getRow = function(that, rowNumber){ // zero indexed for first row of region
    if(rowNumber > that.model.beats){
        console.log('error: rowNumber greater than rows available');
        return;
    }

    let therow = [];
    let startRow = that.model.steps[0].model.y;

    for ( cell of that.model.steps ){
        if (cell.model.y === (rowNumber + startRow) ){
            therow.push( cell );
        }
    }

   return therow; 

};