//redFrik
RedNoise : UGen {
	*ar {|clock= 0, mul= 1, add= 0|
		^this.multiNew('audio', clock).madd(mul, add)
	}
	*kr {|clock= 0, mul= 1, add= 0|
		^this.multiNew('control', clock).madd(mul, add)
	}
	*categories {^#["UGens>Generators>Stochastic", "UGens>Noise"]}
}
