fluid.defaults("adam.grid.sequence", {
    gradeNames: ["adam.sequence"],

    model: {
        region: null
    },

    invokers: {
        regionToSequence: {
            func: function(that, region){

               let thing = {   func: console.log,
                    args: 'aldkjfldjladflksdflkdfj'
                };

                console.timeLog(that.model.region.model.steps);
                for ( let b = 0; b < that.model.region.model.beats; b++){
                    console.log(b);
                    let row = that.model.region.getRow(b);
                    console.log(row);
                }

                that.model.sequenceticks = that.model.region.beats * that.model.beatlength;

            },
            args: ["{that}", "{arguments}.0"]
        },
        
        /*
        setCellPayload: {
            func: function(that, cell, payload){
                console.log('hi');
            },
            args: ["{that}", "{arguments}.0", "{arguments}.1"]
        }
        */
    }
});