 var Cell = require("./cell");



exports.Region = function(){
	this.beats = undefined;
	this.startPoint = undefined;
	this.endPoint = undefined;
	this.rows = [];
	this.steps = [];	

	this.bottomLeft = undefined;
	this.bottomRight = undefined;
	this.topLeft = undefined;
	this.topRight = undefined; 
};

// createFromCells
// checkOverlap
// equals
// getRow

// adjust row

exports.Region.prototype.addCells = function(cells){
	if (cells.length === 0 ){return;}

	if( cells.length === 1){
		this.startpoint = cells[0];
		this.endpoint = cells[0];
		this.beats = 1;
		this.steps = cells;
		this.rows[0] = cells;
		return;
	}

	if (cells.length === 2){
		this.startPoint = cells[0];
		this.endPoint = cells[1];

		//log("startpoint: " + this.startPoint.x + "," + this.startPoint.y );
		//log("endpoint: " + this.endPoint.x + " " + this.endPoint.y);

		if( this.startPoint.x <= this.endPoint.x){
			if( this.startPoint.y <= this.endPoint.y ){
				this.bottomLeft = this.startPoint;
				this.topRight = this.endPoint;	
				this.topLeft = new Cell.Cell(this.startPoint.x, this.endPoint.y);
				this.bottomRight = new Cell.Cell(this.endPoint.x, this.startPoint.y);
			}else{
				this.topLeft = this.startPoint;
			}
		}else{
			console.log('yet to be implemented');
		}

		this.beats = 0;

		for(var i = this.bottomLeft.y; i <= this.topLeft.y; i++){
			this.rows[this.beats] = [];
			for(var j = this.bottomLeft.x; j <= this.bottomRight.x; j++){
				var newCell = new Cell.Cell(j,i);

				this.steps.push( newCell );
				this.rows[this.beats].push(newCell);

				console.log( newCell );
			}
			this.beats++;
		}

	}

	if( cells.legnth > 2 ){
		console.log(' bigger shapes not yet implemented' );
	}
};

exports.Region.prototype.toNotes = function(){
	var newnotes = {};
	newnotes.notes = [];
	
	for (var i = 0; i < this.rows.length; i++){
		for (var j = 0; j < this.rows[i].length; j++){
			newnotes.notes.push({
				pitch: 60 + i + j, 
				start_time: (j / this.rows[i].length ) + i,
				duration: 0.5 / this.rows[i].length		
			});
		}
	}
	return newnotes;
}

exports.Region.prototype.getRow = function(rowNumber){
	if( rowNumber > this.beats){return undefined}

	var theRow = [];
	var startRow = this.steps[0].y;

	this.steps.forEach(function(cell){
		if( cell.y === (rowNumber + startRow ) ){
			therow.push( cell );
		}
	});

	return theRow;
};

exports.Region.prototype.replaceRow = function(beat, row){

	//log("replace row with: ");
	if( Array.isArray(row) ){
		row.forEach(function(cell){console.log(cell)});
		this.rows[beat] = row;
	}else{
		console.log('error adding notes to region');
	}
}

exports.Region.prototype.clearBeat = function(beat){
	// check beat is a number and within the range of beats
	this.rows[beat] = [];	
}

exports.Region.prototype.onBeat = function( cell ){
	if( this.steps[0].x === cell.x ){
		return true; 
	}else{
		return false;
	}
};

exports.Region.prototype.mergeRegion = function( region ){
	
	console.log("adding region with this many rows: " +  region.rows.length );

	var startingPoint; 
	for( var i = 0; i < this.rows.length; i++){
		if(   this.rows[i][0].equals( region.steps[0] )){
			startingPoint = i;
			break;
		}
	}

	for( var i = 0; i < region.rows.length; i++){
		this.replaceRow( startingPoint + i, region.rows[i] );	
	}

	this.rebuildRegion();
};

exports.Region.prototype.rebuildRegion = function(){

	/*
	this.bottomLeft = this.rows[0][0];	
	this.bottomRight = this.rows[0][this.rows[0].length-1];
	this.topLeft = this.rows[this.rows.length-1][0];
	this.topRight = this.rows[this.rows.length-1][this.rows[this.rows.length-1].length-1];
	*/

	this.beats = this.rows.length;
	
	//log( this.steps );
	this.steps =  [].concat.apply([], this.rows) ;

	/*
	this.steps = [];
	for( var i = 0; i < this.rows.length; i++){
		for ( var j = 0; j < this.rows[i].length; j++){
			this.steps.push( this.rows[i,j] );
		}
	}
	*/

	console.log('rebuilt steps. length: ' + this.steps.length );
};
