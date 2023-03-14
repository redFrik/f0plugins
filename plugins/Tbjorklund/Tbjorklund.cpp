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

//TODO tappar sync!

#include "SC_PlugIn.h"
static InterfaceTable *ft;

struct Tbjorklund : public Unit {
	unsigned short *m_currarr;
	unsigned short *m_copyarr;
	int m_k;
	int m_n;
	float m_readpos;
};

void Tbjorklund_Ctor(Tbjorklund *unit);
void Tbjorklund_Dtor(Tbjorklund *unit);
void Tbjorklund_next_kk(Tbjorklund *unit, int inNumSamples);
void Tbjorklund_next_ak(Tbjorklund *unit, int inNumSamples);
void Tbjorklund_next_aa(Tbjorklund *unit, int inNumSamples);
void Tbjorklund_update(Tbjorklund *unit);
void Tbjorklund_recursive(unsigned short *curr, unsigned short *copy, int n, int k, int alen, int blen);

PluginLoad(InterfaceTable *inTable) {
	ft = inTable;
	DefineDtorUnit(Tbjorklund);
}

void Tbjorklund_Ctor(Tbjorklund *unit) {
	unit->m_k = -1;
	unit->m_n = sc_max(ZIN0(3), 1);
	unit->m_readpos = unit->m_n;

	unit->m_currarr = (unsigned short *)RTAlloc(unit->mWorld, unit->m_n * sizeof(unsigned short));
	ClearUnitIfMemFailed(unit->m_currarr);
	unit->m_copyarr = (unsigned short *)RTAlloc(unit->mWorld, unit->m_n * sizeof(unsigned short));
	ClearUnitIfMemFailed(unit->m_copyarr);

	if (unit->mCalcRate == calc_FullRate) {
		if ((INRATE(0) == calc_FullRate) && (INRATE(1) == calc_FullRate)) {
			SETCALC(Tbjorklund_next_aa);
		} else if ((INRATE(0) == calc_FullRate) && (INRATE(1) == calc_BufRate)) {
			SETCALC(Tbjorklund_next_ak);
		} else {
			SETCALC(Tbjorklund_next_kk);
		}
	} else {
		SETCALC(Tbjorklund_next_kk);
	}
}

void Tbjorklund_Dtor(Tbjorklund *unit) {
	RTFree(unit->mWorld, unit->m_currarr);
	RTFree(unit->mWorld, unit->m_copyarr);
}

void Tbjorklund_next_kk(Tbjorklund *unit, int inNumSamples) {
	int n = unit->m_n;
	float readpos = unit->m_readpos;
	unsigned short *arr = unit->m_currarr;

	float rate = ZIN0(0) * SAMPLEDUR;
	float width = ZIN0(1);
	int offset = ZIN0(4);
	float *out = ZOUT(0);

	int index;
	LOOP1(
		inNumSamples,
		index = (int)readpos;
		if (index >= n) {
			Tbjorklund_update(unit);
			readpos -= n;
			index = 0;
		}
		float fraction = readpos - index;
		index = sc_mod(index + offset, n);

		readpos += abs(rate);
		if (fraction > width) {
			ZXP(out) = 0.f;
		} else {
			if (rate < 0.f) {
				ZXP(out) = arr[n - 1 - index];
			} else {
				ZXP(out) = arr[index];
			}
		}
	);

	unit->m_readpos = readpos;
}

void Tbjorklund_next_ak(Tbjorklund *unit, int inNumSamples) {
	int n = unit->m_n;
	float readpos = unit->m_readpos;
	unsigned short *arr = unit->m_currarr;

	float *rate = ZIN(0);
	float width = ZIN0(1);
	int offset = ZIN0(4);
	float *out = ZOUT(0);

	int index;
	LOOP1(
		inNumSamples,
		index = (int)readpos;
		if (index >= n) {
			Tbjorklund_update(unit);
			readpos -= n;
			index = 0;
		}
		float fraction = readpos - index;
		index = sc_mod(index + offset, n);

		float zrate = ZXP(rate) * SAMPLEDUR;
		readpos += abs(zrate);
		if (fraction > width) {
			ZXP(out) = 0.f;
		} else {
			if (zrate < 0.f) {
				ZXP(out) = arr[n - 1 - index];
			} else {
				ZXP(out) = arr[index];
			}
		}
	);

	unit->m_readpos = readpos;
}

void Tbjorklund_next_aa(Tbjorklund *unit, int inNumSamples) {
	int n = unit->m_n;
	float readpos = unit->m_readpos;
	unsigned short *arr = unit->m_currarr;

	float *rate = ZIN(0);
	float *width = ZIN(1);
	int offset = ZIN0(4);
	float *out = ZOUT(0);

	int index;
	LOOP1(
		inNumSamples,
		index = (int)readpos;
		if (index >= n) {
			Tbjorklund_update(unit);
			readpos -= n;
			index = 0;
		}
		float fraction = readpos - index;
		index = sc_mod(index + offset, n);

		float zrate = ZXP(rate) * SAMPLEDUR;
		readpos += abs(zrate);
		if (fraction > ZXP(width)) {
			ZXP(out) = 0.f;
		} else {
			if (zrate < 0.f) {
				ZXP(out) = arr[n - 1 - index];
			} else {
				ZXP(out) = arr[index];
			}
		}
	);

	unit->m_readpos = readpos;
}

void Tbjorklund_update(Tbjorklund *unit) {
	int n = unit->m_n;

	int k = sc_min(sc_max(ZIN0(2), 0), n);
	if (unit->m_k == k) {
		return;
	}
	unit->m_k = k;

	unsigned short *curr = unit->m_currarr;
	for (int i = 0; i < n; i++) {
		curr[i] = (i < k) ? 1 : 0;
	}
	if ((k == 0) || ((n - k) <= 1)) {
		return;
	}

	Tbjorklund_recursive(curr, unit->m_copyarr, n, k, 1, 1);
}

void Tbjorklund_recursive(unsigned short *curr, unsigned short *copy, int n, int k, int alen, int blen) {
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
		Tbjorklund_recursive(curr, copy, n, k, alen, blen);
	}
}
