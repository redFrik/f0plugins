//redFrik

RedDPCMencode : Filter {
	var prev= 0;
	
	//--lang
	dpcm {|in= 0.0, round= 0|
		var src= (in-prev).round(round);
		prev= prev+src;
		^src;
	}
	reset {
		prev= 0;
	}
	
	//--ugen
	*ar {|in= 0.0, round= 0|
		^this.multiNew('audio', in, round)
	}
	*kr {|in= 0.0, round= 0|
		^this.multiNew('control', in, round)
	}
	*categories {^#["UGens>Filters>Linear", "UGens>Maths"]}
}
