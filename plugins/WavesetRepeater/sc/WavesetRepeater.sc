WavesetRepeater : UGen {
	*ar {|in, repeats= 8, rate= 1, numzc= 1, memlen= 0.1, interpolation= 2|
		^this.multiNew('audio', in, repeats, rate, numzc, memlen, interpolation)
	}
	*kr {|in, repeats= 8, rate= 1, numzc= 1, memlen= 0.1, interpolation= 2|
		^this.multiNew('control', in, repeats, rate, numzc, memlen, interpolation)
	}
	checkInputs { ^if(rate == \audio or:{rate == \control}) { this.checkSameRateAsFirstInput } }
	*categories {^#["UGens>f0plugins", "UGens>Filters>Nonlinear"]}
}
