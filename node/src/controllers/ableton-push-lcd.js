fluid.defaults("adam.midi.push.lcd", {
    gradeNames: "fluid.modelComponent",

    model: {
        lcdline1: ' '.padEnd(68, ' '),
        lcdline2: ' '.padEnd(68, ' '),
        lcdline3: 'Made by Ableton'.padStart(41, ' ').padEnd(68, ' '),
        lcdline4: 'Powered by Flocking.js'.padStart(45, ' ').padEnd(68, ' '),
    },

    modelListeners: {
        // wait until midi initializes?
        lcdline1: { 
            excludeSource: "init",
            funcName: "adam.midi.push.lcdWrite",
            args : ["{that}", "{change}.value" , 0]
        },
        lcdline2: {
            excludeSource: "init",
            funcName: "adam.midi.push.lcdWrite",
            args: ["{that}", "{change}.value" , 1]
        },
        lcdline3: {
            excludeSource: "init",
            funcName: "adam.midi.push.lcdWrite",
            args: ["{that}", "{change}.value" , 2]
        },
        lcdline4: {
            excludeSource: "init",
            funcName: "adam.midi.push.lcdWrite",
            args: ["{that}", "{change}.value" , 3]
        },
    },

    invokers: {
        lcdClearLine: {
            funcName: "adam.midi.push.lcdClearLine",
            args: ["{that}", "{arguments}.0"]
        },
        lcdClear: {
            funcName: "adam.midi.push.lcdClear",
            args: ["{that}"]
        },
        lcdWrite: { // lcdwriteline
            funcName: "adam.midi.push.lcdWrite",
            args: ["{that}", "{arguments}.0", "{arguments}.1", "{arguments}.2"]
        },
        lcdRefresh: {
            funcName: "adam.midi.push.lcdRefresh",
            args: ["{that}"]
        },
    }
});

// PUSH SYSEX Spec
// 240,71,127,21,<24+line(0-3)>,0,<Nchars+1>,<Offset>,<Chars>,247
// 240,71,127,21,25,0,13,4,"Hello World",247
adam.midi.push.lcdWrite = function(that, thestring="test", line = 0, offset = 0 ){
    var thestringinascii = []; 
    if(typeof thestring != "string"){
        thestring = thestring.toString();
    }
    for(var i = 0; i < thestring.length; i++){
        thestringinascii[i] = thestring.charCodeAt(i);
    }
    mysysexmessage = [240, 71, 127, 21];
    mysysexmessage.push(24 + line, 0);
    mysysexmessage.push(thestring.length + 1, offset);
    mysysexmessage.push(...thestringinascii);
    mysysexmessage.push(247);
    that.sendRaw( mysysexmessage );
    return(mysysexmessage);
};


adam.midi.push.lcdRefresh = function (that){
    that.lcdWrite( that.model.lcdline1, 0);
    that.lcdWrite( that.model.lcdline2, 1);
    that.lcdWrite( that.model.lcdline3, 2);
    that.lcdWrite( that.model.lcdline4, 3);
};

adam.midi.push.lcdClearLine = function (that, l = 0){
    if (typeof l === "number" && l < 4 && l >= 0) 
        that.sendRaw([240,71,127,21,28+l,0,0,247]); 
};

adam.midi.push.lcdClear = function(that){
    that.lcdClearLine(0);
    that.lcdClearLine(1);
    that.lcdClearLine(2);
    that.lcdClearLine(3);
};

adam.midi.push.knobsToString = function (that ){
    let knobstring = '';

    // todo test for empty? 
    for ( let i = 0; i < 8 ; i++ ){
        currentknobstring = that.model['knob' + (i + 1)].value.toString();
        currentknobstring = currentknobstring.padStart(8 + i%2  , ' ');
        //console.log(currentknobstring);
        knobstring += currentknobstring;
    }

    that.applier.change('lcdline1', knobstring);

    knobstring = '';

    for ( let i = 0; i < 8 ; i++ ){
        currentknobstring = that.model['knob' + (i + 1)].name.toString();
        currentknobstring = currentknobstring.substring(0, 7+i%2);
        currentknobstring = currentknobstring.padStart( 8 + i%2  , ' ');
        //console.log(currentknobstring);
        knobstring += currentknobstring;
    }

    that.applier.change('lcdline2', knobstring);
};