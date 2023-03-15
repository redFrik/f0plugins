Dbjorklund : UGen {
	*new {|k= 4, n= 8, offset= 0, length= inf|
		^this.multiNew('demand', length, k, n, offset)
	}
	*categories {^#["UGens>Demand"]}
}

Dbjorklund2 : UGen {
	*new {|k= 4, n= 8, offset= 0, length= inf|
		if(k.isNumber and:{k<=0}, {Error("k must be >= 1").throw});
		^this.multiNew('demand', length, k, n, offset)
	}
	*categories {^#["UGens>Demand"]}
}
