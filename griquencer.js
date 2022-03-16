const midi = require('midi');
const input = new midi.Input();
// var term = require('terminal-kit').terminal; // maybe add this later to prevent all the scrawling text? 

var Cell = require("./src/cell");
var Region = require("./src/region");
var Grid = require("./src/grid");

var Sequence = require("./src/sequence");
var Sequencer = require("./src/sequencer");


var divisions = [ 1, 2, 5];
var divisionVolages = [13, 26, 66];
let pulseCount = 0;

/// MIDI Devices
let pushController;
let sequencerOut;
let gridDisplay;

var grid = new Grid.Grid();
var padsDown = [];

var Seq = new Sequencer.Sequencer();

function setup(){

	for( let i = 0; i < input.getPortCount(); i++){
		if ( input.getPortName(i) === "Ableton Push 2 Live Port" ){
			pushController = new midi.Input();
			pushController.on('message', pushControllerMidiEvent );
			pushController.openPort(i);

			gridDisplay = new midi.Output();
			gridDisplay.openPort(i);
			blankGridDisplay();

			/// sequencerOut - new midi.Output();
			/// sequencerOut.openVirtualPort('griquencer');
		}
		if ( input.getPortName(i) === "IAC Driver Bus 1" ){
			sequencerOut = new midi.Output();	
			sequencerOut.openPort(i);
		}
	}


	Seq.events.on('beat', function(){
		//console.log('beat');
	});	
}

function createRegion( padsdown ){
	
	var region = new Region.Region();
	region.addCells( padsdown );
	
	return region;
}

function createSequence( region ){

	var sequence = new Sequence.Sequence();

	sequence.arrayToSequence( region.rows );
	sequence.playing = true;
	sequence.loop = true;
	sequence.colour = Seq.sequences.length + 1;
	Seq.queueSequence( sequence );
	Seq.selectSequence( sequence );

	// TODO This nees to be more sensible
	sequence.events.on('trigger', function(e){ 
		sendGrid(e, 10 );
		sequencerOut.send([144, 48 + sequence.colour, 127]);
		setTimeout( function(){
			sendGrid( e, sequence.colour );
			sequencerOut.send([144, 48 + sequence.colour, 0]);
		}, 100 );
	});
}

function pushControllerMidiEvent( deltaTime, midimsg ){

	if (midimsg[0] === 144){ // NoteOn

		var newCell = new Cell.Cell();
		newCell.fromMIDINote( midimsg );

		padsDown.push( newCell );
		return;
	}
	
	if (midimsg[0] === 128){// NoteOff

		if ( padsDown.length === 0 ){
			return;
		}
		
		var newRegion = createRegion( padsDown );
		var overlappingRegions = grid.addRegion( newRegion ); 

		if ( overlappingRegions.length ==  0 ){
			createSequence( newRegion );
			console.log( ' added region ' );
		}else{
			console.log('region overlapped with exisiting region');
		}
		
		updatePushController();
		padsDown = [];
	}	
}

function updatePushController(){
	blankGridDisplay();

	grid.regions.forEach(function(region, regionIndex){
		region.rows.forEach(function(row){
			row.forEach(function(cell){
				sendGrid( cell, regionIndex + 1 );
			});
		});
	});
};

/*
// receives virtual MIDI messages from VCV rack
function syncSequencer( deltaTime, message ){


	/// TODO respond to region, rather than hardcoded a sequence
	if(message[0] === 144 && message[1] !== 40){
		if ( pulseCount > 7 ){
			pulseCount = 0;
		}

		//sendGrid( pulseCount, 8 );
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
}
*/

function sendGrid(gridpos, colour){

	if( pushController ){
		//console.log(gridpos);
		gridDisplay.send([144, 36 + gridpos.x + (gridpos.y*8),  colour]);
	}
}

function blankGridDisplay(){
	for ( let x = 0; x < 8; x++){
		for ( let y = 0; y < 8; y++ ){
			sendGrid( {x:x, y:y}, 0 );
		}
	}
}

setup();
