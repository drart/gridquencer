//var Sequence = require("./sequence");

const events = require("events");

exports.Sequencer = function(){
	this.bpm = 60;
	this.beatlength = 480;
	this.ticktime = 0;
	this.sequences = [];
	this.selectedsequence = null;
	this.sequencestostart = [];
	
	this.grid = null;
	this.events = new events.EventEmitter();

	let engine = setInterval( function(seq){

		if( seq.ticktime % seq.beatlength === 0 ){

			seq.sequencestostart.forEach(function(sequence){
				seq.addSequence(sequence);
			});
			seq.sequencestostart = [];

			seq.events.emit('beat');				
		}

		for ( let s of seq.sequences ){
			let evnt = s.tick();
		}
	
		seq.ticktime++;
	}, 1, this);
};


exports.Sequencer.prototype.queueSequence = function( sequence ){
	this.sequencestostart.push( sequence );
};


exports.Sequencer.prototype.addSequence = function( sequence ){
	this.sequences.push( sequence );
	//this.selectedsequence = sequence; // todo rethink this
};

exports.Sequencer.prototype.setTempo = function(){}; 
exports.Sequencer.prototype.removeSequence = function(){};
exports.Sequencer.prototype.muteSequence = function(){};

exports.Sequencer.prototype.selectSequence = function(seq){
	if ( this.getSequence( seq ) !== null ){
		this.selectedSequence  = seq;
		return true;
	}
	return false;
};

exports.Sequencer.prototype.getSequence = function(seq){
	var indx = this.sequences.indexOf( seq );
	return ( indx !== -1 ) ? this.sequences[indx] :  null;
};

/*
adam.sequencer.muteSequence = function (that, seq) {
    that.getsequence(seq).mute = true;
    return true;
};
*/
