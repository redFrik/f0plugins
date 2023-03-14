Dbjorklund : UGen {
	*new {|k= 4, n= 8, offset= 0, length= inf|
		^this.multiNew('demand', length, k, n, offset)
	}
	*categories {^#["UGens>Demand"]}
}
