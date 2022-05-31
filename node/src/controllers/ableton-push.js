// todo have a more abstract class that handles just the midi and then put the drawing in each controller setting
/// todo components pads, knobs, buttons, lcd
// todo add colours for each type

fluid.defaults("adam.midi.push", {
    gradeNames: ["flock.midi.connection", "fluid.modelComponent"],
    sysex: true,
    openImmediately: true,

    ports: {
        input: {
            name : "Ableton Push User Port"
        },
        output: {
            name : "Ableton Push User Port"
        }
    },

    model : {
        knob1: { 
            name: "something",
            min: 0,
            max: 100,
            value: 50,
            increment: 1
        },
        knob2: { 
            name: "something",
            min: 0,
            max: 100,
            value: 50,
            increment: 1
        },
        knob3: { 
            name: "something",
            min: 0,
            max: 100,
            value: 50,
            increment: 1
        },
        knob4: { 
            name: "something",
            min: 0,
            max: 100,
            value: 50,
            increment: 1
        },
        knob5: { 
            name: "something",
            min: 0,
            max: 100,
            value: 50,
            increment: 1
        },
        knob6: { 
            name: "something",
            min: 0,
            max: 100,
            value: 50,
            increment: 1
        },
        knob7: { 
            name: "something",
            min: 0,
            max: 100,
            value: 50,
            increment: 1
        },
        knob8: { 
            name: "something",
            min: 0,
            max: 100,
            value: 50,
            increment: 1
        },
        swingKnob: { 
            name: "swing",
            min: 0,
            max: 100,
            value: 50,
            increment: 1
        },
        tempoKnob: { 
            name: "tempo",
            min: 0,
            max: 300,
            value: 50,
            increment: 1
        },
        volumeKnob: { 
            name: "volume",
            min: 0,
            max: 100,
            value: 50,
            increment: 1
        },
        pedal1inverse : false,
        pedal2inverse : false,
        padColours: {
            off: 0,
            selected: 1,
            highlighted: 30
        },
        buttons: { // dynamic component set to 0
            quarter: 1,
            quartertuple: 1,
            eighthtuple: 1,
            eighth: 1,
            sixteenth: 1,
            sixteenthtuple: 1,
            thirtysecond: 1,
            thitysecondtuple: 1,
            left: 1, 
            right: 1, 
            down: 1, 
            up: 1,
            newbutton: 1,
            deletebutton: 1, 
            automation: 1,
            note: 1, 
            session: 1,
            select: 1,
            shift: 1,
            addTrack: 1,
            repeat: 1, 
            accent: 1,
            mute: 1,
            solo: 1, 
            stop: 1,
            track: 1,
            clip: 1,
            undo: 1,
            mute: 1,
            device: 1,
        }
    },

    modelListeners: {
        buttons: {
            funcName: "adam.midi.push.buttonWrite",
            args: ["{that}", "{change}.value", "{change}.oldvalue" ]
        },
    },

    events: {
        knobTouched: null,
        knobReleased: null,
        padPushed: null,
        padReleased: null,
        pedal1: null,
        pedal2: null,
        knob1: null,
        knob2: null,
        knob3: null,
        knob4: null,
        knob5: null,
        knob6: null, 
        knob7: null,
        knob8: null,
        tempoKnob: null,
        swingKnob: null,
        volumeKnob: null,
        buttonPlayPressed: null,
        buttonPlayReleased: null,
        buttonPressed: null, 
        buttonReleased: null
    },

    invokers: {
        // Pad and button handlers
        padWrite: {
            funcName: "adam.midi.push.padWrite",
            args: ["{that}", "{arguments}.0", "{arguments}.1", "{arguments}.2"]
        },
        padClearAll: {
            funcName: "adam.midi.push.padClearAll",
            args: ["{that}"]
        },
        buttonClearAll: {
            funcName: "adam.midi.push.buttonClearAll",
            args: ["{that}"]
        },
        /*
        buttonWrite: {
            funcName: "adam.midi.push.buttonWrite",
            args: ["{that}", "{arguments}.0", "{arguments}.1", "{arguments}.2"]
        },
        */
    },

    listeners : {
        noteOn : {
            funcName: "adam.midi.push.noteToEvents",
            args: ["{that}", "{arguments}.0"]
        },
        noteOff: {
            funcName: "adam.midi.push.noteToEvents",
            args: ["{that}", "{arguments}.0"]
        },
        control: {
            funcName:  "adam.midi.push.controlToEvents",
            args: ["{that}", "{arguments}.0"]
        },
        aftertouch: function(msg){ console.log(msg)},
        //pitchbend: function(msg){}
    },
    //dynamicComponents: {},
});



adam.midi.push.padWrite = function(that, x = 0, y = 0, colour = 1){
    var midimessage = {type: "noteOn", channel: 0, note: 36, velocity: colour}
    midimessage.note = ( x * 8 ) + y + 36;
    that.send(midimessage); 
};

adam.midi.push.padClearAll = function(that){ // todo adjust for grid size
    for( let x = 0; x < 8; x++){
        for( let y = 0; y < 8; y++){
            that.padWrite(x, y, 0);
        }
    }
};

adam.midi.push.buttonClearAll = function(that){
    let midiccs = [ 3, 9, 20, 21, 22, 23,24, 25, 26, 27, 28, 29, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 85, 86, 87, 88, 89, 90, 102, 103, 104, 105, 106, 107, 108, 109, 116, 117, 118, 119, 110, 111, 112, 113,114, 115];
    var midimessage = {type: "control", channel: 0, number: 0 , value: 0 }
    for ( b in midiccs ){
        midimessage.number = midiccs[ b ];
        that.send( midimessage );
    }
};

///// TODO FIX THIS 
// map button to the buttons in the midi 
// idea rename to buttonstate?
// gridquencer.push.applier.change("buttons.quarter", 2)
adam.midi.push.buttonWrite = function (that, buttons, oldstate, path){
    var buttonMapping = {
        quarter: 36,
        quartertuple: 37,
        eighthtuple: 39,
        eighth: 38,
        sixteenth: 40,
        sixteenthtuple: 41,
        thirtysecond: 42,
        thitysecondtuple: 43,
        left: 44, 
        right: 45, 
        down: 47, 
        up: 46,
        newbutton: 87,
        deletebutton: 118, 
        duplicate: 88,
        automation: 89,
        note: 50, 
        session: 51,
        undo: 119,
        shift: 49,
        addTrack: 53,
        select: 48,
        device: 110,
        mute: 60
    };

    var midimessage = {type: "control", channel: 0, number: 36, value: 1};

    for ( let b in buttonMapping ){
        midimessage.number = buttonMapping[ b ];
        midimessage.value = buttons[ b ];
        that.send( midimessage );
    }
};

adam.midi.push.noteToEvents = function(that, msg){
    if (msg.note < 20){
        that.events.knobTouched.fire(msg); // unroll this to knob and value?
    }else{
        var notenumber = msg.note;
        var r = Math.floor((notenumber - 36) / 8);
        var c = (notenumber-36) % 8;
        var pos = { column : c, row: r };
        if( msg.velocity > 0){
            that.events.padPushed.fire( pos, msg.velocity );
        }else{
            that.events.padReleased.fire( pos, msg.velocity );
        }
    } 
};



//// TODO Fix with temp value  => change applier?
adam.midi.push.controlToEvents = function(that, msg){
    if (msg.number > 70 && msg.number < 79){
        // todo respect min and max for knob
        /// modelize this
        that.model["knob" + (msg.number-70)].value += msg.value > 64 ? - (128-msg.value) : msg.value;
        that.model["knob" + (msg.number-70)].value = clamp( that.model["knob" + (msg.number-70)].value, 0, 100 ); 

        that.events["knob" + (msg.number-70)].fire( that.model["knob" + (msg.number-70)].value );
        return;
    }
    if (msg.number === 79){
        //that.model["volumeKnob"] += msg.value > 64 ? - (128-msg.value) : msg.value;
        //that.model["volumeKnob"] = adam.clamp( that.model["volumeKnob"], 0, 100 );
        let knobval = msg.value > 64 ? - (128-msg.value) : msg.value;
        that.events.volumeKnob.fire( knobval );
        return;
    }
    if (msg.number === 14){
        //that.model["swingKnob"] += msg.value > 64 ? - (128-msg.value) : msg.value;
        //that.model["swingKnob"] = adam.clamp( that.model["swingKnob"], 0, 100 );
        let knobval = msg.value > 64 ? - (128-msg.value) : msg.value;
        that.events.swingKnob.fire( knobval );
        return;
    }
    if (msg.number === 15){
        //that.model["tempoKnob"] += msg.value > 64 ? - (128-msg.value) : msg.value;
        //that.model["tempoKnob"] = adam.clamp( that.model["tempoKnob"], 20, 200 );
        let knobval = msg.value > 64 ? - (128-msg.value) : msg.value;
        that.events.tempoKnob.fire( knobval );
        return;
    }
    
    if (msg.number === 64){
        var down = (that.model.pedal1inverse) ? 0 : 127 ;
        var up = (that.model.pedal1inverse) ? 127 : 0;
        if(msg.value === down ){
            that.events.pedal1.fire("down");
        }else{
            that.events.pedal1.fire("up");
        }
        return;
    }
    if (msg.number === 69){
        var down = (that.model.pedal2inverse) ? 0 : 127 ;
        var up = (that.model.pedal2inverse) ? 127 : 0;
        if (msg.value === down){
            that.events.pedal2.fire("down");
        }else{
            that.events.pedal2.fire("up");
        }
        return;
    }
    if (msg.number === 85){
        if (msg.value === 127){
            that.events.buttonPlayPressed.fire();
        }else{
            that.events.buttonPlayReleased.fire();
        }
        return;
    }

    /// all of the other buttons go here
    // idea the track buttons on top of the grid get separated? 
    if ( msg.value === 127 ) {
        that.events.buttonPressed.fire( msg.number );
    }else{
        that.events.buttonReleased.fire( msg.number );
    }

}; 