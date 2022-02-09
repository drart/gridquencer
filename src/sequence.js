const events = require("events");

exports.Sequence = function(){

	this.steps = {};
	this.tickTime = 0;
	this.beats = 1;
	this.ticksPerBeat= 480;
	this.mute = false;
	this.loop = false;
	this.sync = "tempo";
	this.direction = "forward";
	this.playing = false;
	this.currentstep = undefined;
	this.previousstep = undefined;

	this.events = new events.EventEmitter();

};


/// converts an array in region.rows format to a sequence
exports.Sequence.prototype.arrayToSequence= function(arr){

	if (!Array.isArray(arr)){
		console.log('warning: arrays must be used for sequences');
		return -1;
	}

	if( Array.isArray(arr[0]) ){ // multibeat sequence
		this.beats = arr.length;	
		for ( let b = 0; b < arr.length; b++){
			for ( let i = 0; i < arr[b].length; i++){
				this.steps[ (this.ticksPerBeat * b) + Math.floor( (this.ticksPerBeat/arr[b].length) * i ) ] = arr[b][i];
			}
		}
	}else{ // single beat sequence
		for( let i = 0; i < arr.length; i++ ){
			this.steps[ this.ticksPerBeat * i] = arr[i];
		}
	}

};


exports.Sequence.prototype.tick = function(){

	this.tickTime++;

	if ( this.playing === true ){
		this.tickTime = (this.loop === true )? this.tickTime % (this.beats * this.ticksPerBeat) : this.tickTime;

	} else {
		return;
	}

	if( this.steps[ this.tickTime ] !== undefined ){
		//console.log( this.steps[ this.tickTime ] );
		this.events.emit('trigger');
	}

};
