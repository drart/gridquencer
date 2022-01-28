exports.Grid = function(){

	this.rows = 8;
	this.columns = 8;
	this.regions = [];
	this.thegrid = [];
	this.selectedCell = null;
	this.selectedRegion = null;
	this.allowOverlap = false;

	//for (var i = 0; i < this.rows * this.columns; i++){
	//		this.thegrid[i] = undefined;
	//}
}

exports.Grid.prototype.printUnemptyCells = function(){

	var unemptycells = 0;
	this.thegrid.forEach(function(cell){
		if( cell ){unemptycells++}	
	});
	console.log("unempty cells in grid: " + unemptycells );

};

exports.Grid.prototype.addRegion = function(region){
	// todo make sure it is within bounds?  //this.printUnemptyCells();

	if(this.allowOverlap){
		this.regions.push(region);
	}else{
		var doesOverlap = this.doesOverlap( region );
		console.log( "region overlapping: " + doesOverlap );

		// todo check to see that it doesn't overlap with more than one region? 

		if( doesOverlap ){
			var overlappingRegion = this.thegrid[ region.steps[0].y*8 + region.steps[0].x ].region; 
	
			if( overlappingRegion !== undefined ){
				if( overlappingRegion.onBeat( region.steps[0] ) ){ // adjust a beat
					
					overlappingRegion.steps.forEach( function(cell){
						this.thegrid[cell.y*8 + cell.x]  = undefined;	
					},this);
					
					overlappingRegion.mergeRegion( region );
					region = overlappingRegion;

				}else{
					return undefined;
				}
			}		
		}else{
			this.regions.push(region);
		}
	}

	// todo: what if region exists? this will create weird behaviours?
       region.steps.forEach( function(cell){
               this.thegrid[ (cell.y*8) + cell.x] = { 
                       region: region,
                       cell: cell
               };
       }, this);

	this.printUnemptyCells();

	return region;
};

exports.Grid.prototype.selectRegion = function(region){
	this.selectedRegion = region;
};


exports.Grid.prototype.moveRegion = function(region,newOrigin){

};

exports.Grid.prototype.selectCell = function( cell ){
	this.selectedCell = cell;
};

exports.Grid.prototype.doesOverlap = function(region){

	var grid = this.thegrid;

	for ( var i = 0; i < region.steps.length; i++){
		if( grid[ region.steps[i].y*8 + region.steps[i].x] !== undefined){
			return true;
		}
	}
	return false;
};

exports.Grid.prototype.overlappingRegions = function( newRegion ){
	var overlappingRegions = [];
	
	// if found a region check to see it doesn't already exist in the array
	this.regions.forEach(function(region){
		region.rows.forEach(function(row){
			row.forEach(function(cell){
				// if overlap overlappingRegions.push(region)					
			});
		});
	});	

	return overlappingRegions;
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
