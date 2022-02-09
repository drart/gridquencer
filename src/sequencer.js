//var Sequence = require("./sequence");


exports.Sequencer = function(){
	this.bpm = 60;
	this.beatlength = 480;
	this.ticktime = 0;
	this.sequences = [];
	this.selectedsequence = null;
	this.sequencestostart = [];
	
	this.grid = null;
	
	let engine = setInterval( function(seq){

		if( seq.ticktime % seq.beatlength === 0 ){
			console.log('beat');
		}

		for ( let s of seq.sequences ){
			let evnt = s.tick();
			if (evnt){
				console.log(evnt);
			}
		}
	
		seq.ticktime++;
	}, 1, this);
};

exports.Sequencer.prototype.addSequence = function( sequence ){
	this.sequences.push( sequence );
	this.selectedsequence = sequence;
};

exports.Sequencer.prototype.setTempo = function(){}; 
exports.Sequencer.prototype.getSequence = function(){};
exports.Sequencer.prototype.removeSequence = function(){};
exports.Sequencer.prototype.muteSequence = function(){};
exports.Sequencer.prototype.selectSequence = function(){};



/*

adam.sequencer.selectSequence = function (that, seq) {
    if (that.getsequence(seq) !== -1) {
        that.model.selectedsequence = seq;
        return true;
    } else {
        return false;
    }
};

adam.sequencer.muteSequence = function (that, seq) {
    that.getsequence(seq).mute = true;
    return true;
};

adam.sequencer.getSequence = function (that, seq) {
    let result = that.model.sequences.indexOf(seq);

    if (result !== -1) {
        result = that.model.sequences[result];
        return result;
    } else {
        return undefined;
    }
};

*/
