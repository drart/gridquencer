flock.tablegenerators.badnoise = function (size) {
	return flock.generateBuffer( size, function(i){
		return (Math.random() * 2.0) - 1.0;
	});
};
