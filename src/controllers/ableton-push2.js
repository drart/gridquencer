//const pushDisplay = require("ableton-push-canvas-display");

fluid.defaults("adam.midi.push2", {
    //gradeNames: ["adam.midi.push", "fluid.rendererComponent"],
    gradeNames: ["adam.midi.push"],
    ports: {
        input: {
            name : "Ableton Push 2 User Port"
        },
        output: {
            name : "Ableton Push 2 User Port"
        }
    },

    // add canvas thing
});


// adam.midi.push2.drawawesome = function(){};