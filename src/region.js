//const { TouchBarButton } = require("electron");

fluid.defaults("adam.grid.region", {
    gradeNames: "fluid.modelComponent",

    model: {
        beats: undefined,
        startpoint: undefined,
        endpoint: undefined,
        steps: []
    },

    invokers: {
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
                thecell.createFromPos({row: i,column: j});
                that.model.steps.push(thecell);
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