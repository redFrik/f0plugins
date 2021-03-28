//redFrik
SN76489 : UGen {
	*ar {|tone0= 512, tone1= 0, tone2= 0, noise= 0, vol0= 15, vol1= 0, vol2= 0, vol3= 0, rate= 1|
		^this.multiNew('audio', tone0, tone1, tone2, noise, vol0, vol1, vol2, vol3, rate)
	}
	*categories {^#["UGens>Emulators"]}
}
