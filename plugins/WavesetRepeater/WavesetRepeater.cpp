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

#include "SC_PlugIn.h"
static InterfaceTable *ft;

struct WavesetRepeater : public Unit {
	float *m_buf;
	int m_buflen;
	int m_bufindex;
	int m_bufindexmax;
	double m_repeatpos;
	int m_repeatcounter;
	int m_zerocrosscounter;
	float m_prev;
	short m_dir;
	short m_state;
};

void WavesetRepeater_Ctor(WavesetRepeater *unit);
void WavesetRepeater_Dtor(WavesetRepeater *unit);
void WavesetRepeater_next(WavesetRepeater *unit, int inNumSamples);

PluginLoad(InterfaceTable *inTable) {
	ft = inTable;
	DefineDtorUnit(WavesetRepeater);
}

void WavesetRepeater_Ctor(WavesetRepeater *unit) {
	SETCALC(WavesetRepeater_next);

	unit->m_buflen = sc_max((int)(ZIN0(4) * SAMPLERATE), 1);
	unit->m_buf = (float *)RTAlloc(unit->mWorld, unit->m_buflen * sizeof(float));
	ClearUnitIfMemFailed(unit->m_buf);
	memset(unit->m_buf, 0, unit->m_buflen * sizeof(float));

	unit->m_bufindex = 0;
	unit->m_repeatpos = 0.0;
	unit->m_repeatcounter = 0;
	unit->m_zerocrosscounter = 0;
	unit->m_prev = ZIN0(0);
	unit->m_dir = 0;
	unit->m_state = 0;

	// Print("buflen: %i\n", unit->m_buflen);
	WavesetRepeater_next(unit, 1);
}

void WavesetRepeater_Dtor(WavesetRepeater *unit) {
	RTFree(unit->mWorld, unit->m_buf);
}

void WavesetRepeater_next(WavesetRepeater *unit, int inNumSamples) {
	float *buf = unit->m_buf;
	int buflen = unit->m_buflen;
	int bufindex = unit->m_bufindex;
	int bufindexmax = unit->m_bufindexmax;
	double repeatpos = unit->m_repeatpos;
	int repeatcounter = unit->m_repeatcounter;
	int zerocrosscounter = unit->m_zerocrosscounter;
	float prev = unit->m_prev;
	short dir = unit->m_dir;
	short state = unit->m_state;

	float *in = ZIN(0);
	float *out = ZOUT(0);
	int repeats = sc_max((int)ZIN0(1), 0);
	float rate = ZIN0(2);
	int numzc = sc_max((int)ZIN0(3), 1);
	int interpol = ZIN0(5);

	float curr;
	LOOP1(
		inNumSamples,

		switch (state) {
			case 0: // waiting
				curr = ZXP(in);
				if ((prev <= 0.f) && (curr > 0.f)) {
					dir = 1;
				} else if ((prev > 0.f) && (curr <= 0.f)) {
					dir = -1;
				} else {
					dir = 0;
				}
				if (dir != 0) {
					buf[0] = curr;
					bufindex = 1;
					state = 1;
					ZXP(out) = curr;
				} else {
					ZXP(out) = 0.f;
				}
				prev = curr;
				break;

			case 1: // recording
				curr = ZXP(in);
				bool end;
				if (dir == 1) {
					end = (prev <= 0.f) && (curr > 0.f);
				} else {
					end = (prev > 0.f) && (curr <= 0.f);
				}
				if ((end && (++zerocrosscounter == numzc)) || (bufindex == buflen)) {
					zerocrosscounter = 0;
					if (repeats == 0) {
						state = 0;
					} else {
						bufindexmax = bufindex;
						buf[bufindex] = prev;
						repeatpos = 0.0;
						repeatcounter = repeats;
						if (interpol == 2) {
							state = 2;
						} else if (interpol == 4) {
							state = 4;
						} else {
							state = 3;
						}
					}
				} else {
					buf[bufindex] = curr;
					bufindex++;
				}
				ZXP(out) = curr;
				prev = curr;
				break;

			case 2: // repeating - linear interpolation
				PZ(in);
				repeatpos += abs(rate);
				if ((int)repeatpos >= bufindexmax) {
					repeatpos -= bufindexmax;
					repeatcounter--;
					if (repeatcounter == 0) {
						state = 0;
						prev = ZX(in);
						ZXP(out) = 0.f;
						break;
					}
				}
				if (rate == 0.f) {
					curr = 0.f;
				} else {
					int irp1 = (int)repeatpos;
					float frac = repeatpos - irp1;
					int irp2 = (irp1 + 1) % (bufindexmax + 1);
					if (rate < 0.f) {
						irp1 = bufindexmax - irp1;
						irp2 = bufindexmax - irp2;
					}
					curr = lininterp(frac, buf[irp1], buf[irp2]);
				}
				ZXP(out) = curr;
				break;

			case 3: // repeating - no interpolation
				PZ(in);
				repeatpos += abs(rate);
				if ((int)repeatpos >= bufindexmax) {
					repeatpos -= bufindexmax;
					repeatcounter--;
					if (repeatcounter == 0) {
						state = 0;
						prev = ZX(in);
						ZXP(out) = 0.f;
						break;
					}
				}
				if (rate > 0.f) {
					curr = buf[(int)repeatpos];
				} else if (rate < 0.f) {
					curr = buf[bufindexmax - (int)repeatpos];
				} else {
					curr = 0.f;
				}
				ZXP(out) = curr;
				break;

			case 4: // repeating - cubic interpolation
				PZ(in);
				repeatpos += abs(rate);
				if ((int)repeatpos >= bufindexmax) {
					repeatpos -= bufindexmax;
					repeatcounter--;
					if (repeatcounter == 0) {
						state = 0;
						prev = ZX(in);
						ZXP(out) = 0.f;
						break;
					}
				}
				if (rate == 0.f) {
					curr = 0.f;
				} else {
					int irp1 = (int)repeatpos;
					float frac = repeatpos - irp1;
					int mod = bufindexmax + 1;
					int irp2 = (irp1 + 1) % mod;
					int irp3 = (irp1 + 2) % mod;
					int irp0 = (irp1 - 1 + mod) % mod;
					if (rate < 0.f) {
						irp1 = bufindexmax - irp1;
						irp2 = bufindexmax - irp2;
						irp3 = bufindexmax - irp3;
						irp0 = bufindexmax - irp0;
					}
					curr = cubicinterp(frac, buf[irp0], buf[irp1], buf[irp2], buf[irp3]);
				}
				ZXP(out) = curr;
				break;
		}
	);

	unit->m_bufindex = bufindex;
	unit->m_bufindexmax = bufindexmax;
	unit->m_repeatpos = repeatpos;
	unit->m_repeatcounter = repeatcounter;
	unit->m_zerocrosscounter = zerocrosscounter;
	unit->m_prev = prev;
	unit->m_dir = dir;
	unit->m_state = state;
}
