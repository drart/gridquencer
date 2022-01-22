const midi = require('midi');
const input = new midi.Input();

// var term = require('terminal-kit').terminal; // maybe add this later to prevent all the scrawling text? 


var divisions = [ 1, 2, 5];
var divisionVolages = [13, 26, 66];

/// MIDI Devices
let pushController;
let virtualMidi;
let sequencerOut;
let gridDisplay;

let pulseCount = 0;


/// VCV Gate Send
// figure this out
// 40 clock
// 44 muxlicer channel 1
// 44 muxlicer channel 2
// 44 muxlicer channel 3
// 44 muxlicer channel 4
// 44 muxlicer channel 5
// 44 muxlicer channel 6
// 44 muxlicer channel 7
// 44 muxlicer channel 8


for( let i = 0; i < input.getPortCount(); i++){
	console.log( input.getPortName(i) );

	if ( input.getPortName(i) === "Ableton Push 2 Live Port" ){
		pushController = new midi.Input();
		pushController.on('message', function(deltaTime, message){
			//console.log(message);
		});
		pushController.openPort(i);

		gridDisplay = new midi.Output();
		gridDisplay.openPort(i);
		for(let i = 0; i < 8; i++){
			sendGrid(i, 2);
		}
	}

	if ( input.getPortName(i) === "IAC Driver Bus 1" ){
		virtualMidi = new midi.Input();
		sequencerOut = new midi.Output();	

		virtualMidi.on('message', function(deltaTime, message){
			if(message[0] === 144 && message[1] !== 40){
				if ( pulseCount > 7 ){
					pulseCount = 0;

				}

				sendGrid( pulseCount, 8 );
				let p = pulseCount;
				setTimeout( function(p){
					sendGrid( p , 2 );
				}, 40, p);
				sequencerOut.send([176, 0, 26 + pulseCount * 13]);

				if( pulseCount < 3 ) {
					sequencerOut.send([176, 1, 26]); // 2V === 1 step
				}else{
					sequencerOut.send([176, 1, 79]); // 6v === 5 steps
				}

				pulseCount++;

			}
			if(message[0] === 144 && message[1] === 40){
				console.log('vcv gate clock');
				sequencerOut.send([176, 2, 127]);
				setTimeout( function(){
					sequencerOut.send([176, 2, 0]);
				}, 20 );
			}
		});

		sequencerOut.openPort(i);
		virtualMidi.openPort(i);
	}
}


function sendGrid(gridpos, colour){

	console.log(gridpos);
	console.log(36 + gridpos *8);

	if( pushController ){
		if(gridpos < 3){
			gridDisplay.send([144, 36 + gridpos * 8, colour]);	
		}else{
			gridDisplay.send([144, 62 + gridpos - 5, colour]);	
		}
	}
}
