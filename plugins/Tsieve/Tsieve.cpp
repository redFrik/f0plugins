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
		Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
*/

// f.olofsson 2023

#include "SC_PlugIn.h"
static InterfaceTable *ft;

struct Tsieve : public Unit {
	double m_index;
};

void Tsieve_Ctor(Tsieve *unit);
void Tsieve_next_kk(Tsieve *unit, int inNumSamples);
void Tsieve_next_ak(Tsieve *unit, int inNumSamples);
void Tsieve_next_ka(Tsieve *unit, int inNumSamples);
void Tsieve_next_aa(Tsieve *unit, int inNumSamples);
float Tsieve_update(Tsieve *unit, double index, double shift, double period);

PluginLoad(InterfaceTable *inTable) {
	ft = inTable;
	DefineSimpleUnit(Tsieve);
}

void Tsieve_Ctor(Tsieve *unit) {
	unit->m_index = 0.0;

	if (unit->mCalcRate == calc_FullRate) {
		if ((INRATE(0) == calc_FullRate) && (INRATE(1) == calc_FullRate)) {
			SETCALC(Tsieve_next_aa);
		} else if (INRATE(0) == calc_FullRate) {
			SETCALC(Tsieve_next_ak);
		} else if (INRATE(1) == calc_FullRate) {
			SETCALC(Tsieve_next_ka);
		} else {
			SETCALC(Tsieve_next_kk);
		}
	} else {
		SETCALC(Tsieve_next_kk);
	}
}

void Tsieve_next_kk(Tsieve *unit, int inNumSamples) {
	float rate = ZIN0(0) * SAMPLEDUR;
	float width = ZIN0(1);
	double period = ZIN0(2);
	double shift = ZIN0(3);
	float *out = ZOUT(0);

	double index = unit->m_index;
	LOOP1(
		inNumSamples,
		float result = Tsieve_update(unit, index, shift, period);
		ZXP(out) = (sc_frac(index) <= width) ? result : 0.f;
		index += rate;
	);

	unit->m_index = index;
}

void Tsieve_next_ak(Tsieve *unit, int inNumSamples) {
	float *rate = ZIN(0);
	float width = ZIN0(1);
	double period = ZIN0(2);
	double shift = ZIN0(3);
	float *out = ZOUT(0);

	double index = unit->m_index;
	LOOP1(
		inNumSamples,
		float zrate = ZXP(rate) * SAMPLEDUR;
		float result = Tsieve_update(unit, index, shift, period);
		ZXP(out) = (sc_frac(index) <= width) ? result : 0.f;
		index += zrate;
	);

	unit->m_index = index;
}

void Tsieve_next_ka(Tsieve *unit, int inNumSamples) {
	float rate = ZIN0(0) * SAMPLEDUR;
	float *width = ZIN(1);
	double period = ZIN0(2);
	double shift = ZIN0(3);
	float *out = ZOUT(0);

	double index = unit->m_index;
	LOOP1(
		inNumSamples,
		float result = Tsieve_update(unit, index, shift, period);
		ZXP(out) = (sc_frac(index) <= ZXP(width)) ? result : 0.f;
		index += rate;
	);

	unit->m_index = index;
}

void Tsieve_next_aa(Tsieve *unit, int inNumSamples) {
	float *rate = ZIN(0);
	float *width = ZIN(1);
	double period = ZIN0(2);
	double shift = ZIN0(3);
	float *out = ZOUT(0);

	double index = unit->m_index;
	LOOP1(
		inNumSamples,
		float zrate = ZXP(rate) * SAMPLEDUR;
		float result = Tsieve_update(unit, index, shift, period);
		ZXP(out) = (sc_frac(index) <= ZXP(width)) ? result : 0.f;
		index += zrate;
	);

	unit->m_index = index;
}

float Tsieve_update(Tsieve *unit, double index, double shift, double period) {
	float test = sc_mod(sc_floor(index) - shift, period);
	for (int i = 4; i < unit->mNumInputs; ++i) {
		float x = ZIN0(i);
		if (test == x) {
			return 1.f;
		}
	}
	return 0.f;
}
