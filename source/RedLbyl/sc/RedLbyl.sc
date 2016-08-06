//redFrik
RedLbyl : Filter {
	var counter= 0, prevout;
	
	//--ugen
	*ar {|in= 0.0, thresh= 0.5, samples= 2|
		^this.multiNew('audio', in, thresh, samples)
	}
	*kr {|in= 0.0, thresh= 0.5, samples= 2|
		^this.multiNew('control', in, thresh, samples)
	}
	*categories {^#["UGens>Filters>Linear", "UGens>Maths"]}
	
	//--lang
	lbyl {|in= 0.0, thresh= 0.5, samples= 2|
		var out= in;
		if(prevout.notNil and:{(out-prevout).abs>thresh}, {
			counter= counter+1;
			if(counter<samples, {
				out= prevout;
			}, {
				counter= 0;
			});
		}, {
			counter= 0;
		});
		prevout= out;
		^out;
	}
}
