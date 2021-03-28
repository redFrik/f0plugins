//redFrik
Astrocade : UGen {
	*ar {|reg0= 0, reg1= 127, reg2= 0, reg3= 0, reg4= 0, reg5= 0, reg6= 15, reg7= 0|
		^this.multiNew('audio', reg0, reg1, reg2, reg3, reg4, reg5, reg6, reg7)
	}
	*categories {^#["UGens>Emulators"]}
}
