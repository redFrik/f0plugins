//With inspiration from FoscSieve by N. Armstrong.
//Written under the supervision of Nit the cat.

Tsieve : UGen {
	*ar {|rate= 8, width= 0.5, indices= #[0], period= 1, shift= 0|
		^this.multiNewList(['audio', rate, width, period, shift] ++ indices)
	}
	*kr {|rate= 8, width= 0.5, indices= #[0], period= 1, shift= 0|
		^this.multiNewList(['control', rate, width, period, shift] ++ indices)
	}
	signalRange { ^\unipolar }
	*categories { ^#["UGens>Triggers"] }

	union {|other| ^this.bitOr(other)}
	sect {|other| ^this.bitAnd(other)}
	difference {|other| ^this*(1-other)}
	symmetricDifference {|other| ^this.bitXor(other)}
}
