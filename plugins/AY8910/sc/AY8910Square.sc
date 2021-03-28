//redFrik
AY8910Square {
	*ar {|freq= 100, noise= 15, ctrl= 0, vol= 15, envRate= 200, envShape= 14, rate= 1|
		var r0, r1, r6, r7, r8, rB, rC, rD;
		var control, enable0, enable1, enable2;
		r0= freq%256;
		r1= Clip.kr(freq/256, 0, 15);
		r6= Clip.kr(noise, 0, 31);
		control= Clip.kr(ctrl, 0, 7);
		enable0= control%2>0;
		enable1= (control/2).floor%2>0;
		enable2= (control/4).floor%4>0;
		r7= enable0+(enable1*8);
		r8= Clip.kr(vol, 0, 15)+(enable2*16);
		rB= envRate%256;
		rC= Clip.kr(envRate/256, 0, 255);
		rD= Clip.kr(envShape, 0, 15);
		^AY8910.ar(r0, r1, 0, 0, 0, 0, r6, r7, r8, 0, 0, rB, rC, rD, rate)
	}
	*categories {^#["UGens>Emulators"]}
}
