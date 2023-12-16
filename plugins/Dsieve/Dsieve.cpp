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

struct Dsieve : public Unit {
	uint32 m_period;
	int32 m_shift;
	double m_repeats;
	uint32 m_repeatCount;
};

void Dsieve_Ctor(Dsieve *unit);
void Dsieve_next(Dsieve *unit, int inNumSamples);

PluginLoad(InterfaceTable *inTable) {
	ft = inTable;
	DefineSimpleUnit(Dsieve);
}

void Dsieve_Ctor(Dsieve *unit) {
	SETCALC(Dsieve_next);
	Dsieve_next(unit, 0);
	OUT0(0) = 0.f;
}

void Dsieve_next(Dsieve *unit, int inNumSamples) {
	if (inNumSamples) {

		float period = DEMANDINPUT_A(0, inNumSamples);
		if (!sc_isnan(period)) {
			unit->m_period = (uint32)period;
		}

		float shift = DEMANDINPUT_A(1, inNumSamples);
		if (!sc_isnan(shift)) {
			unit->m_shift = (int32)shift;
		}

		if (unit->m_repeats < 0.0) {
			float x = DEMANDINPUT_A(2, inNumSamples);
			unit->m_repeats = sc_isnan(x) ? 0.f : floor(x + 0.5f);
		}

		if (unit->m_repeatCount >= unit->m_repeats) {
			OUT0(0) = NAN;
			return;
		}

		uint32 test = (unit->m_repeatCount - unit->m_shift) % unit->m_period;
		int result = 0;
		for (int i = 3; i < unit->mNumInputs; ++i) {
			float x = DEMANDINPUT_A(i, inNumSamples);
			if (test == x) {
				result = 1;
				break;
			}
		}
		OUT0(0) = result;
		unit->m_repeatCount++;

	} else {
		unit->m_repeats = -1.f;
		unit->m_repeatCount = 0;
	}
}
