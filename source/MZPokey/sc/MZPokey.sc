//redFrik
MZPokey : UGen {
	*ar {|audf1= 0, audc1= 0, audf2= 0, audc2= 0, audf3= 0, audc3= 0, audf4= 0, audc4= 0, audctl= 0|
		^this.multiNew('audio', audf1, audc1, audf2, audc2, audf3, audc3, audf4, audc4, audctl)
	}
	*categories {^#["UGens>Emulators"]}
}
