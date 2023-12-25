//With inspiration from FoscSieve by N. Armstrong.
//Written under the supervision of Nit the cat.

Dsieve : DUGen {
	*new {|indices= #[0], period= 1, shift= 0, repeats= inf|
		^this.multiNewList(['demand', period, shift, repeats] ++ indices)
	}
	*categories {^#["UGens>Demand"]}

	union {|other| ^this.bitOr(other)}
	sect {|other| ^this.bitAnd(other)}
	difference {|other| ^this*(1-other)}
	symmetricDifference {|other| ^this.bitXor(other)}
}
