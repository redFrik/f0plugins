//redFrik
SID6581f : UGen {
	*ar {|	freqLo0= 0, freqHi0= 0, pwLo0= 0, pwHi0= 0, ctrl0= 0, atkDcy0= 0, susRel0= 0,
			freqLo1= 0, freqHi1= 0, pwLo1= 0, pwHi1= 0, ctrl1= 0, atkDcy1= 0, susRel1= 0,
			freqLo2= 0, freqHi2= 0, pwLo2= 0, pwHi2= 0, ctrl2= 0, atkDcy2= 0, susRel2= 0,
			fcLo= 0, fcHi= 0, resFilt= 0, modeVol= 0, rate= 1|
		^this.multiNew('audio',
			freqLo0, freqHi0, pwLo0, pwHi0, ctrl0, atkDcy0, susRel0,
			freqLo1, freqHi1, pwLo1, pwHi1, ctrl1, atkDcy1, susRel1,
			freqLo2, freqHi2, pwLo2, pwHi2, ctrl2, atkDcy2, susRel2,
			fcLo, fcHi, resFilt, modeVol, rate)
	}
	*categories {^#["UGens>Emulators"]}
}
/*

//--todo
SID8580 : SID6581 {}
SID6581Square {}
SID8580Square : SID6581Square {}
*/