
var Cell = require('./cell');

exports.Grid = function(){

	this.rows = 8;
	this.columns = 8;
	this.regions = [];
	this.thegrid = [];
	this.selectedCell = null;
	this.selectedRegion = null;
	this.allowOverlap = false;

	for ( var x = 0; x < this.columns; x++){
		for( var y = 0; y < this.rows; y++){
			var i = x + ( this.columns * y );
			this.thegrid[i] = new Cell.Cell(x, y);;
			this.thegrid[i].linkedRegions = []; // TODO use Object.assign with all mixins
			this.thegrid[i].getRegions = function(){ return this.linkedRegions };
		}
	}

	console.log( this.thegrid );
}

exports.Grid.prototype.printUnemptyCells = function(){

	var unemptycells = 0;
	this.thegrid.forEach(function(cell){
		if( cell ){unemptycells++}	
	});
	//console.log("unempty cells in grid: " + unemptycells );

};

exports.Grid.prototype.addRegion = function(region){
	// todo make sure it is within bounds?  //this.printUnemptyCells();
	var overlappingRegions = [];

	// TODO this.allowOverlap check

	// check all cells in region for overlapping
	for ( var i = 0; i < region.steps.length; i++ ){

		var thecell = this.getCell( region.steps[i] );			
		var regions = thecell.getRegions();		
		overlappingRegions = overlappingRegions.concat( regions );
	}

	if ( overlappingRegions.length === 0 ){
		this.regions.push( region );
		for( var i = 0; i < region.steps.length; i++ ){
			thecell = this.getCell( region.steps[i] );
			thecell.linkedRegions.push( region );		
			console.log( thecell );
		}
	}else{
		console.log( overlappingRegions );
	}

	console.log( this.thegrid );
	return overlappingRegions;
};

exports.Grid.prototype.getCell = function( cell ){
	return this.thegrid[ (cell.y*8) + cell.x ];
};

exports.Grid.prototype.selectRegion = function(region){
	this.selectedRegion = region;
};


exports.Grid.prototype.selectCell = function( cell ){
	this.selectedCell = cell;
};

exports.Grid.prototype.modifyRegion = function(region){

	// determine the mod
	// modify just those bits of the grid? 

	
};

exports.Grid.prototype.removeRegion = function(region){

	region.steps.forEach(function(cell){
		this.thegrid[cell.y*8 + cell.x] = undefined;	
	}, this);
	

	var regionIndex = this.regions.findIndex( function( region ){
		return JSON.stringify( this.regions[i] ) == JSON.stringify( region );
	});

	console.log("find region index via array.findIndex: " +  regionIndex );

	var regionLocation; 
	for( var i = 0; i < this.regions.length; i++){
		if( JSON.stringify( this.regions[i] ) == JSON.stringify( region ) ){
			regionLocation = i;
			break;
		}	
	}

	if( regionLocation !== -1){
		this.regions.splice( regionLocation, 1 );
	}
};

exports.Grid.prototype.testTwoObjects = function( object1, object2 ){

	// this would be better
	//return( Object.keys(object1).every(function(key){ return object1[key] === object2[key]; });
	//return Object.keys(object1).every((key) =>  object1[key] === object2[key]);

	// works
	return JSON.stringify(object1) === JSON.stringify(object2);
}

//exports.Grid.prototype.toString = function(){ return "grid"; );
