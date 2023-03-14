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

struct Dbjorklund : public Unit {
	double m_length;
	int32 m_count;
	unsigned short *m_currarr;
	unsigned short *m_copyarr;
	int32 m_k;
	int32 m_kprev;
	int32 m_n;
	int32 m_offset;
};

void Dbjorklund_Ctor(Dbjorklund *unit);
void Dbjorklund_Dtor(Dbjorklund *unit);
void Dbjorklund_next(Dbjorklund *unit, int inNumSamples);
void Dbjorklund_update(Dbjorklund *unit);
void Dbjorklund_recursive(unsigned short *curr, unsigned short *copy, int n, int k, int alen, int blen);

PluginLoad(InterfaceTable *inTable) {
	ft = inTable;
	DefineDtorUnit(Dbjorklund);
}

void Dbjorklund_Ctor(Dbjorklund *unit) {
	unit->m_n = (int32)sc_max(ZIN0(2), 1.f);

	unit->m_currarr = (unsigned short *)RTAlloc(unit->mWorld, unit->m_n * sizeof(unsigned short));
	ClearUnitIfMemFailed(unit->m_currarr);
	unit->m_copyarr = (unsigned short *)RTAlloc(unit->mWorld, unit->m_n * sizeof(unsigned short));
	ClearUnitIfMemFailed(unit->m_copyarr);

	unit->m_kprev = -1;

	SETCALC(Dbjorklund_next);
	Dbjorklund_next(unit, 0);
	OUT0(0) = 0.f;
}

void Dbjorklund_Dtor(Dbjorklund *unit) {
	RTFree(unit->mWorld, unit->m_currarr);
	RTFree(unit->mWorld, unit->m_copyarr);
}

void Dbjorklund_next(Dbjorklund *unit, int inNumSamples) {
	if (inNumSamples) {
		if (unit->m_length < 0.0) {
			float x = DEMANDINPUT_A(0, inNumSamples);
			unit->m_length = sc_isnan(x) ? 0.f : floor(x + 0.5f);
		}
		if (unit->m_count >= unit->m_length) {
			OUT0(0) = NAN;
			return;
		}

		int32 n = unit->m_n;
		float k = DEMANDINPUT_A(1, inNumSamples);
		if (!sc_isnan(k)) {
			unit->m_k = sc_min((int32)sc_max(k, 0.f), n);
		} else {
			OUT0(0) = NAN;
			return;
		}
		float offset = DEMANDINPUT_A(3, inNumSamples);
		if (!sc_isnan(offset)) {
			unit->m_offset = (int32)offset;
		} else {
			OUT0(0) = NAN;
			return;
		}

		unsigned short *arr = unit->m_currarr;
		int index = sc_mod(unit->m_count, n);
		if ((index == 0) && (unit->m_k != unit->m_kprev)) {
			Dbjorklund_update(unit);
			unit->m_kprev = unit->m_k;
		}
		index = sc_mod(index + unit->m_offset, n);
		OUT0(0) = arr[index];
		unit->m_count++;

	} else {
		unit->m_length = -1.f;
		unit->m_count = 0;
	}
}

void Dbjorklund_update(Dbjorklund *unit) {
	unsigned short *curr = unit->m_currarr;
	for (int i = 0; i < unit->m_n; i++) {
		curr[i] = (i < unit->m_k) ? 1 : 0;
	}
	if ((unit->m_k == 0) || ((unit->m_n - unit->m_k) <= 1)) {
		return;
	}

	Dbjorklund_recursive(curr, unit->m_copyarr, unit->m_n, unit->m_k, 1, 1);
}

void Dbjorklund_recursive(unsigned short *curr, unsigned short *copy, int n, int k, int alen, int blen) {
	int a = 0;
	int b = k;
	int i = 0;
	while ((a < k) && (b < n)) {
		for (int x = 0; x < alen; x++) {
			copy[i++] = curr[a++];
		}
		for (int y = 0; y < blen; y++) {
			copy[i++] = curr[b++];
		}
	}

	int remain;
	if (b == n) {
		remain = (k - a) / alen;
		int tmp = blen;
		blen = alen;
		alen += tmp;
	} else {
		remain = (n - i) / blen;
		alen += blen;
	}

	k = i;

	while (i < n) {
		copy[i++] = curr[a++];
	}
	for (i = 0; i < n; i++) {
		curr[i] = copy[i];
	}
	if (remain > 1) {
		Dbjorklund_recursive(curr, copy, n, k, alen, blen);
	}
}
