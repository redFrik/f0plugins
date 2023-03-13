
Tbjorklund : UGen {
	*ar {|rate= 1, k= 4, n= 8, offset= 0|
		^this.multiNew('audio', rate, k, n, offset)
	}
	*kr {|rate= 1, k= 4, n= 8, offset= 0|
		^this.multiNew('control', rate, k, n, offset)
	}
	signalRange { ^\unipolar }
	*categories { ^#["UGens>Triggers"] }
}

Tbjorklund2 : UGen {
	*ar {|rate= 1, k= 4, n= 8|
		^(Timer.ar(Tbjorklund.ar(rate, k, n))*rate).round
	}
	*kr {|rate= 1, k= 4, n= 8|
		^(Timer.kr(Tbjorklund.kr(rate, k, n))*rate).round
	}
}
