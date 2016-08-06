//redFrik
Nes2 : UGen {
	*ar {|trig= 0,
		a0= 0, a1= 0, a2= 0, a3= 0,
		b0= 0, b1= 0, b2= 0, b3= 0,
		c0= 0,        c2= 0, c3= 0,
		d0= 0,        d2= 0, d3= 0,
		e0= 0, e1= 0, e2= 0, e3= 0,
		smask= 0|
		^this.multiNew('audio', trig, a0, a1, a2, a3, b0, b1, b2, b3, c0, c2, c3, d0, d2, d3, e0, e1, e2, e3, smask)
	}
	*categories {^#["UGens>Emulators"]}
}

//requiress my Bit plugins
Nes2Square {
	*ar {|trig= 0, dutycycle= 0, loopenv= 0, envdecay= 0, vol= 10, sweep= 0, sweeplen= 0, sweepdir= 0, sweepshi= 0, freq= 100, vbl= 0|
		var a0, a1, a2, a3;
		a0= Clip.kr(dutycycle.round, 0, 3)*64;
		a0= BitOr.kr(a0, (Clip.kr(loopenv.round, 0, 1)*32));
		a0= BitOr.kr(a0, (Clip.kr(envdecay.round, 0, 1)*16));
		a0= BitOr.kr(a0, Clip.kr(vol.round, 0, 15));
		a1= Clip.kr(sweep.round, 0, 1)*128;
		a1= BitOr.kr(a1, (Clip.kr(sweeplen.round, 0, 7)*16));
		a1= BitOr.kr(a1, (Clip.kr(sweepdir.round, 0, 1)*8));
		a1= BitOr.kr(a1, Clip.kr(sweepshi.round, 0, 7));
		a2= freq.max(0).round%256;
		a3= Clip.kr((freq/256).floor, 0, 7);
		a3= BitOr.kr(a3, (Clip.kr(vbl.round, 0, 31)*8));
		^Nes2.ar(trig, a0, a1, a2, a3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1)
	}
	*categories {^#["UGens>Emulators"]}
}

//requiress my Bit plugins
Nes2Triangle {
	*ar {|trig= 0, start= 0, counter= 10, freq= 100, vbl= 0|
		var c0, c2, c3;
		c0= Clip.kr(start.round, 0, 1)*128;
		c0= BitOr.kr(c0, Clip.kr(counter.round, 0, 127));
		c2= freq.max(0).round%256;
		c3= Clip.kr((freq/256).floor, 0, 7);
		c3= BitOr.kr(c3, (Clip.kr(vbl.round, 0, 31)*8));
		^Nes2.ar(trig, 0, 0, 0, 0, 0, 0, 0, 0, c0, c2, c3, 0, 0, 0, 0, 0, 0, 0, 4)
	}
	*categories {^#["UGens>Emulators"]}
}

//requiress my Bit plugins
Nes2Noise {
	*ar {|trig= 0, loopenv= 0, envdecay= 0, vol= 10, short= 0, freq= 10, vbl= 0|
		var d0, d2, d3;
		d0= Clip.kr(loopenv.round, 0, 1)*32;
		d0= BitOr.kr(d0, (Clip.kr(envdecay.round, 0, 1)*16));
		d0= BitOr.kr(d0, Clip.kr(vol.round, 0, 15));
		d2= Clip.kr(short.round, 0, 1)*128;
		d2= BitOr.kr(d2, Clip.kr(freq.round, 0, 15));
		d3= Clip.kr(vbl.round, 0, 31)*8;
		^Nes2.ar(trig, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, d0, d2, d3, 0, 0, 0, 0, 8)
	}
	*categories {^#["UGens>Emulators"]}
}

//requiress my Bit plugins
Nes2DMC {
	*ar {|trig= 0, loop= 0, freq= 1|
		var e0, e1, e2, e3;
		e0= Clip.kr(loop.round, 0, 1)*64;
		e0= BitOr.kr(e0, Clip.kr(freq.round, 0, 7));
		e1= e2= e3= 0;
		^Nes2.ar(trig, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, e0, e1, e2, e3, 16)
	}
	*categories {^#["UGens>Emulators"]}
}
	/* RECTANGLE WAVE
	** ==============
	** ab0: 7-6=duty cycle, 5=hold, 4=envelope, 3-0=volume
	** ab1: 7=sweep on, 6-4=sweep length, 3=sweep inc/dec, 2-0=sweep shifts
	** ab2: 7-0=freq
	** ab3: 7-3=vbl length counter, 2-0=high freq
	*/
	/* TRIANGLE WAVE
	** =============
	** c0: 7=holdnote, 6-0=linear length counter
	** c2: low 8 bits of frequency
	** c3: 7-3=length counter, 2-0=high 3 bits of frequency
	*/
	/* WHITE NOISE CHANNEL
	** ===================
	** d0: 0-3=volume, 4=envelope, 5=hold
	** d2: 7=small(93 byte) sample, 3-0=freq lookup
	** d3: 7-3=vbl length counter
	*/
	/* DELTA MODULATION CHANNEL
	** =========================
	** e0: 7=irq gen, 6=looping, 3-0=pointer to clock table
	** e1: output dc level, 6 bits unsigned
	** e2: 8 bits of 64-byte aligned address offset : $C000 + (value * 64)
	** e3: length, (value * 16) + 1
	*/
