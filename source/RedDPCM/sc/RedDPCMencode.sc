//redFrik

RedDPCMdecode : Filter {
	var prev= 0;
	
	//--lang
	dpcm {|in= 0.0|
		var src= in+prev;
		prev= src;
		^src;
	}
	reset {
		prev= 0;
	}
	
	//--ugen
	*ar {|in= 0.0|
		^this.multiNew('audio', in)
	}
	*kr {|in= 0.0|
		^this.multiNew('control', in)
	}
	*categories {^#["UGens>Filters>Linear", "UGens>Maths"]}
}
