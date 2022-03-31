const events = require("events");

exports.Sequence = function(){

	//this.steps = {};
	this.tickTime = 0;
	this.beats = 1;
	this.ticksPerBeat= 480;
	this.mute = false;
	this.loop = false;
	//this.sync = "tempo";
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

exports.Sequence.prototype.regionToSequence = function(region){

	if (!Array.isArray(region)){
		console.log('warning: arrays must be used for sequences');
		return -1;
	}

	if( Array.isArray(region[0]) ){ // multibeat sequence
		this.beats = region.length;	
		for ( let b = 0; b < region.length; b++){
			for ( let i = 0; i < region[b].length; i++){
				var startTick = b * this.ticksPerBeat;
				startTick += (i/region[b].length) * this.ticksPerBeat;
				startTick = Math.floor( startTick );

				var endTick =  this.ticksPerBeat / region[b].length ;
				endTick += startTick;	
				endTick = Math.floor( endTick );
				endTick = endTick % (this.beats * this.ticksPerBeat); // handle case where notes are longer than the length of the sequence

				this.notes.push( Object.assign( {start_tick:startTick, end_tick:endTick, gridcell: region[b][i]}, this._note ));
			}
		}
	}else{ // single beat sequence
		this.beats = 1;
		for( let i = 0; i < region.length; i++ ){
			var startTick = i / region.length;
			startTick *= this.ticksPerBeat;
			startTick = Math.floor( startTick );

			var endTick = this.ticksPerBeat / region.length;
			endTick *= this.ticksPerBeat;
			endTick += startTick;
			endTick = Math.floor( endTick );
			endTick = endTick % this.ticksPerBeat;
			
			this.notes.push( Object.assign( {start_tick: startTick, end_tick: endTick, gridcell: region[i]}, this._note ) );
		}
	}

	this.length = this.beats * this.ticksPerBeat;

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
		this.tickTime = (this.loop === true )? this.tickTime % this.length : this.tickTime;

	} else {
		return;
	}

	if( this.ticks[ this.tickTime ] !== undefined ){
		for (var i = 0; i < this.ticks[ this.tickTime ].length; i++){
			this.events.emit(this.ticks[this.tickTime][i].type, this.ticks[this.tickTime][i]);
		}
	}
};
