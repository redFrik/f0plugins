
Tbjorklund : UGen {
	*ar {|rate= 8, width= 0.5, k= 4, n= 8, offset= 0|
		^this.multiNew('audio', rate, width, k, n, offset)
	}
	*kr {|rate= 8, width= 0.5, k= 4, n= 8, offset= 0|
		^this.multiNew('control', rate, width, k, n, offset)
	}
	signalRange { ^\unipolar }
	*categories { ^#["UGens>Triggers"] }
}
