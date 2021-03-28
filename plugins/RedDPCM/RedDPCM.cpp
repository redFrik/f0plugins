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

//RedDPCM by redFrik 091027

#include "SC_PlugIn.h"
static InterfaceTable *ft;

struct RedDPCMencode : public Unit {
	float m_prevout;
};

struct RedDPCMdecode : public Unit {
	float m_prevout;
};

extern "C" {
	void load(InterfaceTable *inTable);
	void RedDPCMencode_Ctor(RedDPCMencode *unit);
	void RedDPCMencode_next_a(RedDPCMencode *unit, int inNumSamples);
	void RedDPCMencode_next_k(RedDPCMencode *unit, int inNumSamples);
	void RedDPCMdecode_Ctor(RedDPCMdecode *unit);
	void RedDPCMdecode_next_a(RedDPCMdecode *unit, int inNumSamples);
	void RedDPCMdecode_next_k(RedDPCMdecode *unit, int inNumSamples);
}

PluginLoad(InterfaceTable *inTable) {
	ft= inTable;
	DefineSimpleUnit(RedDPCMencode);
	DefineSimpleUnit(RedDPCMdecode);
}

void RedDPCMencode_Ctor(RedDPCMencode *unit) {
	if(unit->mCalcRate==calc_FullRate) {
		SETCALC(RedDPCMencode_next_a);
	} else {
		SETCALC(RedDPCMencode_next_k);
	}
	unit->m_prevout= ZIN0(0);
	ZOUT0(0)= 0.f;
}

void RedDPCMencode_next_a(RedDPCMencode *unit, int inNumSamples) {
	float *out= ZOUT(0);
	float *in= ZIN(0);
	float round= ZIN0(1);
	float prev= unit->m_prevout;
	LOOP(inNumSamples,
		 float src= sc_round(ZXP(in)-prev, round);
		 prev= prev+src;
		 ZXP(out)= src;
	);
	unit->m_prevout= prev;
}

void RedDPCMencode_next_k(RedDPCMencode *unit, int inNumSamples) {
	float *out= ZOUT(0);
	float in= ZIN0(0);
	float round= ZIN0(1);
	float prev= unit->m_prevout;
	LOOP(inNumSamples,
		 float src= sc_round(in-prev, round);
		 prev= prev+src;
		 ZXP(out)= src;
	);
	unit->m_prevout= prev;
}

//-----

void RedDPCMdecode_Ctor(RedDPCMdecode *unit) {
	if(unit->mCalcRate==calc_FullRate) {
		SETCALC(RedDPCMdecode_next_a);
	} else {
		SETCALC(RedDPCMdecode_next_k);
	}
	unit->m_prevout= ZIN0(0);
	ZOUT0(0)= 0.f;
}

void RedDPCMdecode_next_a(RedDPCMdecode *unit, int inNumSamples) {
	float *out= ZOUT(0);
	float *in= ZIN(0);
	float prev= unit->m_prevout;
	LOOP(inNumSamples,
		 float src= ZXP(in)+prev;
		 prev= src;
		 ZXP(out)= src;
	);
	unit->m_prevout= prev;
}

void RedDPCMdecode_next_k(RedDPCMdecode *unit, int inNumSamples) {
	float *out= ZOUT(0);
	float in= ZIN0(0);
	float prev= unit->m_prevout;
	LOOP(inNumSamples,
		 float src= in+prev;
		 prev= src;
		 ZXP(out)= src;
	);
	unit->m_prevout= prev;
}
