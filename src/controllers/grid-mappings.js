/// requires utils.js
// todo rename this more appropriately
fluid.defaults("adam.pushgridmapper", {
    gradeNames: ["fluid.modelComponent"], 
    model: {
        padsdown: []
    },

    events: {
        regionCreated: null,
    },

    listeners: {
        regionCreated: {
            func: console.log
        }
    },

    invokers: {
        padPressed: {
            funcName: "adam.midi.push.gridNoteOn",
            args: ["{that}", "{arguments}.0", "{arguments}.1", "{arguments}.2", ]
        },
        padReleased: {
            funcName: "adam.midi.push.gridNoteOff",
            args: ["{that}", "{arguments}.0", "{arguments}.1", "{arguments}.2", ] 
        }
    }
});

adam.midi.push.gridNoteOn = function(that, pos, velocity){

    var thecell = adam.grid.cell();
    thecell.createFromPos(pos);
    
    that.model.padsdown.push( thecell );
};

adam.midi.push.gridNoteOff = function(that, pos, velocity){

    if ( that.model.padsdown.length === 0 ){
        return;
    }
    
    var region = adam.grid.region();
    var cells = fluid.copy( that.model.padsdown );
    region.createFromCells( cells );
    
    that.events.regionCreated.fire( region );
    
    that.model.padsdown = []; // should this blank the array or just remove the current cell?

};