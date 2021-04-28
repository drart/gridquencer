fluid.defaults("adam.gridquencer.sequence", {
    gradeNames: ["adam.sequence"], // refactor this to be a region and a sequence?

    model: {
        region: null
    },

    invokers: {
        regionToSequence: {
            funcName: "adam.gridquencer.sequence.regionToSequence", 
            args: ["{that}", "{arguments}.0", "{arguments}.1"]
        },
    }
});

adam.gridquencer.sequence.regionToSequence = function(that, region, payload){
    that.model.region = region;

    that.model.beats = that.model.region.model.beats;

    for ( let b = 0; b < that.model.region.model.beats; b++){
        let row = that.model.region.getRow(b);
        let steplength = Math.floor(that.model.beatlength / row.length );

        for (let [i, cell] of row.entries() ){
            cell.payload = payload;
            //console.log(cell);
            that.model.steps[ (steplength * i) + (that.model.beatlength * b) ] = cell;
        }
    }

    that.model.sequenceticks = that.model.beats * that.model.beatlength;

};