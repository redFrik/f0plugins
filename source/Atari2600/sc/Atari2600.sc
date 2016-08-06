//redFrik
Atari2600 : UGen {
	*ar {|audc0= 1, audc1= 2, audf0= 3, audf1= 4, audv0= 5, audv1= 5, rate= 1|
		^this.multiNew('audio', audc0, audc1, audf0, audf1, audv0, audv1, rate)
	}
	*categories {^#["UGens>Emulators"]}
}
