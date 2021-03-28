/*
	SuperCollider real time audio synthesis system
    Copyright (c) 2002 James McCartney. All rights reserved.
	http://www.audiosynth.com

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

//RedNoise by redFrik

#include "SC_PlugIn.h"
static InterfaceTable *ft;

struct RedNoise : public Unit {
	uint16 noise;
	uint16 clock;
	uint16 count;
};

extern "C" {
	void RedNoise_Ctor(RedNoise *unit);
	void RedNoise_next_k(RedNoise *unit, int inNumSamples);
	void RedNoise_next_a(RedNoise *unit, int inNumSamples);
}

PluginLoad(InterfaceTable *inTable) {
	ft= inTable;
	DefineSimpleUnit(RedNoise);
}

void RedNoise_Ctor(RedNoise *unit) {
	unit->noise= 0;
	unit->clock= 0;
	unit->count= 0;
	if(unit->mCalcRate==calc_FullRate) {
		if(INRATE(0)==calc_FullRate) {
			SETCALC(RedNoise_next_a);
			RedNoise_next_a(unit, 1);
		} else {
			SETCALC(RedNoise_next_k);
			RedNoise_next_k(unit, 1);
		}
	} else {
		SETCALC(RedNoise_next_k);
		RedNoise_next_k(unit, 1);
	}
}

void RedNoise_next_k(RedNoise *unit, int inNumSamples) {
	float *out= ZOUT(0);
	uint16 noise= unit->noise;
	uint16 clock= sc_max(ZIN0(0), 0);
	uint16 count= unit->count;
	LOOP(inNumSamples,
		count++;
		if(count>=clock) {
			count= 0;
			noise= (noise<<1)|(~((noise>>14)^(noise>>13))&1);
		}
		ZXP(out)= ((float)noise/32767.5)-1.f;
	);
	unit->noise= noise;
	unit->count= count;
}

void RedNoise_next_a(RedNoise *unit, int inNumSamples) {
	float *in= ZIN(0);
	float *out= ZOUT(0);
	uint16 noise= unit->noise;
	uint16 clock;
	uint16 count= unit->count;
	LOOP(inNumSamples,
		count++;
		if(count>=sc_max(ZXP(in), 0)) {
			count= 0;
			noise= (noise<<1)|(~((noise>>14)^(noise>>13))&1);
		}
		ZXP(out)= ((float)noise/32767.5)-1.f;
	);
	unit->noise= noise;
	unit->count= count;
}
