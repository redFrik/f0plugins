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

//RedLbyl by redFrik 091025
//an implementation of Matt Wright's lbyl MaxMSP object.

#include "SC_PlugIn.h"
static InterfaceTable *ft;

struct RedLbyl : public Unit {
	float m_prevout;
	unsigned long m_counter;
};

extern "C" {
	void load(InterfaceTable *inTable);
	void RedLbyl_Ctor(RedLbyl *unit);
	void RedLbyl_next_a(RedLbyl *unit, int inNumSamples);
	void RedLbyl_next_k(RedLbyl *unit, int inNumSamples);
}

PluginLoad(InterfaceTable *inTable) {
	ft= inTable;
	DefineSimpleUnit(RedLbyl);
}

void RedLbyl_Ctor(RedLbyl *unit) {
	if(unit->mCalcRate==calc_FullRate) {
		SETCALC(RedLbyl_next_a);
	} else {
		SETCALC(RedLbyl_next_k);
	}
	unit->m_prevout= ZIN0(0);
	unit->m_counter= 0;
	RedLbyl_next_k(unit, 1);
}

void RedLbyl_next_a(RedLbyl *unit, int inNumSamples) {
	float *out= ZOUT(0);
	float *in= ZIN(0);
	float thresh= ZIN0(1);
	float samples= ZIN0(2);
	float prevout= unit->m_prevout;
	unsigned long counter= unit->m_counter;
	
	LOOP(inNumSamples,
		 float zout= ZXP(in);
		 if(sc_abs(zout-prevout)>thresh) {
			counter++;
			if(counter<samples) {
				zout= prevout;
			} else {
				counter= 0;
			}
		 } else {
			counter= 0;
		 }
		 prevout= zout;
		 ZXP(out)= zout;
	);
	unit->m_counter= counter;
	unit->m_prevout= prevout;
}

void RedLbyl_next_k(RedLbyl *unit, int inNumSamples) {
	float *out= ZOUT(0);
	float in= ZIN0(0);
	float thresh= ZIN0(1);
	float samples= ZIN0(2);
	float prevout= unit->m_prevout;
	unsigned long counter= unit->m_counter;
	
	LOOP(inNumSamples,
		 float zout= in;
		 if(sc_abs(zout-prevout)>thresh) {
			counter++;
			if(counter<samples) {
				zout= prevout;
			} else {
				counter= 0;
			}
		 } else {
			counter= 0;
		 }
		 prevout= zout;
		 ZXP(out)= zout;
	);
	unit->m_counter= counter;
	unit->m_prevout= prevout;
}
