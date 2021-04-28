fluid.defaults("adam.grid.cell", {
    gradeNames: "fluid.modelComponent",
    
    model: {
        x: undefined,
        y: undefined,
        //row: undefined, // legacy names, unsure whether to use or not
        //column: undefined
    },
    
    invokers: {
        midiNoteToPos:{
            funcName: "adam.grid.cell.midiNoteToPos",
            args: ["{that}", "{arguments}.0"]
        },
        posToMidiNote: {
            funcName: "adam.grid.cell.posToMidiNote",
            args: ["{that}", "{arguments}.0"]
        },
        createFromPos: {
            funcName: "adam.grid.cell.createFromPos",
            args: ["{that}", "{arguments}.0"]
        },
        equals: {
            funcName: "adam.grid.cell.equals",
            args: ["{that}", "{arguments}.0"]
        }
    }
});


adam.grid.cell.createFromPos = function(that, pos){
    console.log('calling createFromPos, old code');
    that.model.x = pos.row;
    that.model.y = pos.column;
};

/// wired to ableton push mapping
adam.grid.cell.midiNoteToPos = function (that, msg) {
    that.model.x = Math.floor((msg.note - 36) / 8);
    that.model.y = (msg.note - 36) % 8;
};

/// wired to ableton push mapping
adam.grid.cell.posToMidiNote = function(that){
    return (Math.floor((that.model.x + 36) / 8)) + ((that.model.y + 36) % 8);
}

adam.grid.cell.equals = function (that, cell) {
    if (cell.model.x === that.model.x && cell.model.y && that.model.y) {
        return true;
    }

    return false;
};