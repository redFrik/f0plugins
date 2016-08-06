//redFrik
AY8910 : MultiOutUGen {
	*ar {|r0= 0, r1= 0, r2= 0, r3= 0, r4= 0, r5= 0, r6= 0, r7= 0, r8= 0, r9= 0, rA= 0, rB= 0, rC= 0, rD= 0, rate= 1|
		^this.multiNew('audio', r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, rA, rB, rC, rD, rate)
	}
	init {|...theInputs|
		inputs= theInputs;
		^this.initOutputs(3, rate);
	}
	*categories {^#["UGens>Emulators"]}
}
