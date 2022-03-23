const events = require("events");

exports.Sequence = function(){

	this.steps = {};
	this.tickTime = 0;
	this.beats = 1;
	this.ticksPerBeat= 480;
	this.mute = false;
	this.loop = false;
	this.sync = "tempo";
	//this.direction = "forward";
	this.playing = false;
	this.currentstep = undefined;
	this.previousstep = undefined;

	this.events = new events.EventEmitter();


	this.ticks = [];
	this.length;
	this.notes = [];
	this._note = { 
		pitch: 60,
		start_time: 1,
		duration: 1,
		velocity: 127,
		release_velocity: 127,
		velocity_deviation: 0,
		mute: false,
		probability: 1
	};
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


	this.cellArrayToSequence( arr );

};


exports.Sequence.prototype.cellArrayToSequence = function(cellArray){

	if (!Array.isArray(cellArray)){
		console.log('warning: arrays must be used for sequences');
		return -1;
	}

	if( Array.isArray(cellArray[0]) ){ // multibeat sequence
		this.beats = cellArray.length;	
		this.length = cellArray.length;
		for ( let b = 0; b < cellArray.length; b++){
			for ( let i = 0; i < cellArray[b].length; i++){
				var startTick = b * this.ticksPerBeat;
				startTick += (i/cellArray[b].length) * this.ticksPerBeat;
				startTick = Math.floor( startTick );

				var endTick =  this.ticksPerBeat / cellArray[b].length ;
				endTick += startTick;	
				endTick = Math.floor( endTick );
				endTick = endTick % (this.beats * this.ticksPerBeat); // handle case where notes are longer than the length of the sequence

				this.notes.push( Object.assign( {start_tick:startTick, end_tick:endTick, gridcell: cellArray[b][i]}, this._note, {pitch: Math.random()} ) );
			}
		}
	}else{ // single beat sequence
		this.beats = 1;
		this.length = 1;
		for( let i = 0; i < cellArray.length; i++ ){
			var startTick = i / cellArray.length;
			startTick *= this.ticksPerBeat;
			startTick = Math.floor( startTick );

			var endTick = this.ticksPerBeat / cellArray.length;
			endTick *= this.ticksPerBeat;
			endTick += startTick;
			endTick = Math.floor( endTick );
			endTick = endTick % this.ticksPerBeat;
			
			this.notes.push( Object.assign( {start_tick: startTick, end_tick: endTick, gridcell: cellArray[i]}, this._note ) );
		}
	}

	for( var i = 0; i < this.notes.length; i++){
		if( this.ticks[this.notes[i].start_tick] === undefined ){
			this.ticks[this.notes[i].start_tick] = [];
		}
		if( this.ticks[this.notes[i].end_tick] === undefined ){
			this.ticks[this.notes[i].end_tick] = [];
		}
		this.ticks[ this.notes[i].start_tick ].push(  Object.assign({type: "noteOn"}, this.notes[i]));
		this.ticks[ this.notes[i].end_tick].push( Object.assign({type: "noteOff"}, this.notes[i]));
	}

	console.log( this.notes );
	console.log( this.ticks );

};

exports.Sequence.prototype.tick = function(){

	this.tickTime++;

	if ( this.playing === true ){
		this.tickTime = (this.loop === true )? this.tickTime % (this.beats * this.ticksPerBeat) : this.tickTime;

	} else {
		return;
	}

	if( this.steps[ this.tickTime ] !== undefined ){
		this.events.emit('trigger', this.steps[this.tickTime]);

		for (var i = 0; i < this.ticks[ this.tickTime ].length; i++){
			this.events.emit(this.ticks[this.tickTime][i].type, this.ticks[this.tickTime][i]);
		}
		return( this.steps[this.tickTime] );
	}

};
