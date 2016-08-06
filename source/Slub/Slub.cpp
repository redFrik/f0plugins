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

// /fO 050202
// updated 070224.  got "warning: statement has no effect".  hm, compilers are getting smarter by each upgrade.  had to work around this.

#include "SC_PlugIn.h"

static InterfaceTable *ft;

struct Slub : public Unit
{
	float m_prevtrig;
	float dummy;
};

extern "C"
{
	void load(InterfaceTable *inTable);
	void theFunction(Slub *unit, float spikeValue);
	void Slub_Ctor(Slub *unit);
	void Slub_next(Slub *unit, int inNumSamples);
};

//////////////////////////////////////////////////////////////////
void Slub_Ctor(Slub *unit)
{
	SETCALC(Slub_next);
	unit->m_prevtrig = 0.f;
	ZOUT0(0) = 0.f;
}

//////////////////////////////////////////////////////////////////
void Slub_next(Slub *unit, int inNumSamples)
{
	float *trig = ZIN(0);
	float *spike= ZIN(1);
	float prevtrig = unit->m_prevtrig;
	
	LOOP(inNumSamples,
		float curtrig = ZXP(trig);
		if (prevtrig <= 0.f && curtrig > 0.f) theFunction(unit, ZXP(spike));
		prevtrig = curtrig;
	);
	unit->m_prevtrig = prevtrig;
}

//////////////////////////////////////////////////////////////////
PluginLoad(InterfaceTable *inTable)
{
	ft = inTable;
	DefineSimpleUnit(Slub);
}

//////////////////////////////////////////////////////////////////
void theFunction(Slub *unit, float spikeValue)
{
	//Print("spike %g\n", spikeValue);

	long
			i
					,
			j
	,
			k
					,
			l
	,
			x
					;
			x
	=
			(long)round
					(
			pow
	(
	2
	,
			spikeValue
					)
			)
	;
			for
					(
			i
					=
					0
						;
			i
	<
					x
			;
	i++
			)
					{
			for
	(j=
			0
			;
					j
			<
	x
	;
			j++
					)
			{		for
			(
					k
			=0
			;
						k<
	x
			;
					k++
			)
	{
			for
			
					(
			l
			=
					0;
			l
					<
			x
	;
			l++
					)
			{
	fmod(
			l				,
			unit->dummy= 3.1415
	)
			;
					}
			}}
	}
}
