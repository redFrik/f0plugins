//redFrik
BeepU : UGen {}					//do not instantiate
Beep : UGen {
	*ar {|freq= 3250, vol= 1|
		^BeepU.multiNew('audio', freq, vol)
	}
	*categories {^#["UGens>Emulators"]}
}  