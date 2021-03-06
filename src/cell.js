fluid.defaults("adam.grid.cell", {
    gradeNames: "fluid.modelComponent",
    
    model: {
        x: undefined,
        y: undefined
    },
    
    invokers: {
        midiNoteToPos:{
            func: function(that, msg  ){
                that.model.x = Math.floor((msg.note - 36) / 8);
                that.model.y = (msg.note -36) % 8;
            }, 
            args: ["{that}", "{arguments}.0"]
        },
        createFromPos: {
            func: function(that, cell){
                console.log(cell);
                that.model.x = cell.row;
                that.model.y = cell.column;
            },
            args: ["{that}", "{arguments}.0"]
        }

    }
});