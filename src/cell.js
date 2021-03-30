
fluid.defaults("adam.grid.cell", {
    gradeNames: "fluid.modelComponent",
    
    model: {
        x: undefined,
        y: undefined,
        row: undefined,
        column: undefined
    },
    
    invokers: {
        midiNoteToPos:{
            func: function(that, msg  ){
                that.model.x = Math.floor((msg.note - 36) / 8);
                that.model.y = (msg.note -36) % 8;
            }, 
            args: ["{that}", "{arguments}.0"]
        },
        posToMidiNote: {
            func: function(that, msg){
                return (Math.floor((that.model.x + 36)/8)) + ((that.model.y + 36) % 8);
            },
            args: ["{that}", "{arguments}.0"]
        },
        createFromPos: {
            func: function(that, cell){
                //console.log(cell);
                that.model.x = cell.row;
                that.model.y = cell.column;
            },
            args: ["{that}", "{arguments}.0"]
        },
        equals: {
            funcName: "adam.grid.cell.equals",
            args: ["{that}", "{arguments}.0"]
        }
    }
    // modelListeners // x->row row->x
});

adam.grid.cell.equals = function(that, cell){
    if (cell.model.x === that.model.x && cell.model.y && that.model.y ){
        return true;
    }

    return false;
};