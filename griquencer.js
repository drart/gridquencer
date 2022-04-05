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

	sequence.regionToSequence( region.rows );
	sequence.playing = true;
	sequence.loop = true;
	Seq.queueSequence( sequence ); 
	Seq.selectSequence( sequence );
	sequence.colour = 4 * (Seq.sequences.length+1) + 3;
	sequence.channel = Seq.sequences.length;


	// TODO better way of changing sequence channel
	sequence.events.on('noteOn', function(e){
		sendGrid(e.gridcell, sequence.colour-2);
		sequencerOut.send([144 + sequence.channel , e.pitch, e.velocity]); 
	});
	sequence.events.on('noteOff', function(e){
		sendGrid(e.gridcell, sequence.colour);
		sequencerOut.send([128 + sequence.channel, e.pitch , e.release_velocity]); 
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
			newSequence = createSequence( newRegion );
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
				sendGrid( cell,(regionIndex+1)*4 + 3 );
			});
		});
	});
};

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
