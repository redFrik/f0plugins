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
	
	--
	Beep - OldSkool Beep Sound Simulator for SC
	Author KT
	Ported from the MAME 0.116 package by Fredrik Olofsson 070707
	080607 - had to rename Beep->BeepU because silly windows beep already defined
*/

#include "SC_PlugIn.h"
static InterfaceTable *ft;



/***************************************************************************

    beep.c

    This is used for computers/systems which can only output a constant tone.
    This tone can be turned on and off.
    e.g. PCW and PCW16 computer systems
    KT - 25-Jun-2000

    Sound handler

****************************************************************************/

/*													//f0 -commented out
#include "sndintrf.h"
#include "sound/beep.h"
#include "streams.h"
*/

//#define BEEP_RATE			48000					//f0 -commented out - use SAMPLERATE instead


struct BeepU : public Unit							//f0 -renamed and made public unit
{
	//sound_stream *stream; 	/* stream number */	//f0 -commented out
	int enable; 			/* enable beep */
	int frequency;			/* set frequency - this can be changed using the appropiate function */
	int incr;				/* initial wave state */
	int16 signal;			/* current signal */
	float volume;									//f0 -added this
};




extern "C" {										//f0
	void BeepU_Ctor(BeepU *unit);
	void BeepU_next_k(BeepU *unit, int inNumSamples);
	void BeepU_sound_update(BeepU *unit, float *buffer, int length);
};

PluginLoad(InterfaceTable *inTable) {				//f0
	ft= inTable;
	DefineSimpleUnit(BeepU);
}

void BeepU_Ctor(BeepU *unit) {						//f0
	SETCALC(BeepU_next_k);
	unit->enable = 0;
	unit->frequency = 3250;
	unit->incr = 0;
	unit->signal = 0x07fff;
	unit->volume = 1.f;
	BeepU_next_k(unit, 1);
}

void BeepU_next_k(BeepU *unit, int inNumSamples) {	//f0
	float *out= OUT(0);
	if(IN0(0)!=unit->frequency) {
		unit->frequency = IN0(0);
		unit->signal = 0x07fff;
		unit->incr = 0;
	}
	if(IN0(1)!=0.f) {
		if(unit->enable==0) {
			unit->incr = 0;
			unit->signal = 0x07fff;
		}
		unit->enable = 1;
		unit->volume = IN0(1);
	} else {
		unit->enable = 0;
	}
	BeepU_sound_update(unit, out, inNumSamples);
}





/*************************************
 *
 *  Stream updater
 *
 *************************************/

void BeepU_sound_update(BeepU *unit, float *buffer, int length)	//f0 -changed
{
	//struct beep_sound *bs = (struct Beep *) param;					//f0 -commented out
	//stream_sample_t *buffer = _buffer[0];								//f0 -commented out
	int16 signal = unit->signal;
	int clock = 0, rate = SAMPLERATE / 2;								//f0 -changed from BEEP_RATE

    /* get progress through wave */
	int incr = unit->incr;

	if (unit->frequency > 0)
		clock = unit->frequency;

	/* if we're not enabled, just fill with 0 */
	if ( !unit->enable || clock == 0 )
	{
		memset( buffer, 0, length * sizeof(*buffer) );
		return;
	}

	/* fill in the sample */
	while( length-- > 0 )
	{
		*buffer++ = signal/32767.f*unit->volume;						//f0 -convert into float and added vol
		incr -= clock;
		while( incr < 0 )
		{
			incr += rate;
			signal = -signal;
		}
	}

	/* store progress through wave */
	unit->incr = incr;
	unit->signal = signal;
}


/*************************************
 *
 *  Sound handler start
 *
 *************************************/
//f0 -commented out
//static void *beep_start(int sndindex, int clock, const void *config)
//{
//	struct beep_sound *pBeep;
//
//	pBeep = auto_malloc(sizeof(*pBeep));
//	memset(pBeep, 0, sizeof(*pBeep));
//
//	pBeep->stream = stream_create(0, 1, BEEP_RATE, pBeep, beep_sound_update );
//	pBeep->enable = 0;
//	pBeep->frequency = 3250;
//	pBeep->incr = 0;
//	pBeep->signal = 0x07fff;
//	return pBeep;
//}


/*************************************
 *
 *  changing state to on from off will restart tone
 *
 *************************************/
//f0 -commented out
//void beep_set_state( int num, int on )
//{
//	struct beep_sound *info = sndti_token(SOUND_BEEP, num);
//
//	/* only update if new state is not the same as old state */
//	if (info->enable == on)
//		return;
//
//	stream_update(info->stream);
//
//	info->enable = on;
//	/* restart wave from beginning */
//	info->incr = 0;
//	info->signal = 0x07fff;
//}




/*************************************
 *
 *  setting new frequency starts from beginning
 *
 *************************************/
//f0 -commented out
//void beep_set_frequency(int num,int frequency)
//{
//	struct beep_sound *info = sndti_token(SOUND_BEEP, num);
//
//	if (info->frequency == frequency)
//		return;
//
//	stream_update(info->stream);
//	info->frequency = frequency;
//	info->signal = 0x07fff;
//	info->incr = 0;
//}



/*************************************
 *
 *  change a channel volume
 *
 *************************************/
//f0 -commented out
//void beep_set_volume(int num, int volume)
//{
//	struct beep_sound *info = sndti_token(SOUND_BEEP, num);
//
//	stream_update(info->stream);
//
//	volume = 100 * volume / 7;
//
//	sndti_set_output_gain(SOUND_BEEP, num, 0, volume );
//}



/**************************************************************************
 * Generic get_info
 **************************************************************************/
//f0 -commented out
//static void beep_set_info(void *token, UINT32 state, sndinfo *info)
//{
//	switch (state)
//	{
//		/* no parameters to set */
//	}
//}

//f0 -commented out
//void beep_get_info(void *token, UINT32 state, sndinfo *info)
//{
//	switch (state)
//	{
//		/* --- the following bits of info are returned as 64-bit signed integers --- */
//
//		/* --- the following bits of info are returned as pointers to data or functions --- */
//		case SNDINFO_PTR_SET_INFO:						info->set_info = beep_set_info;			break;
//		case SNDINFO_PTR_START:							info->start = beep_start;				break;
//		case SNDINFO_PTR_STOP:							/* nothing */							break;
//		case SNDINFO_PTR_RESET:							/* nothing */							break;
//
//		/* --- the following bits of info are returned as NULL-terminated strings --- */
//		case SNDINFO_STR_NAME:							info->s = "Beep";						break;
//		case SNDINFO_STR_CORE_FAMILY:					info->s = "Beep";						break;
//		case SNDINFO_STR_CORE_VERSION:					info->s = "1.0";						break;
//		case SNDINFO_STR_CORE_FILE:						info->s = __FILE__;						break;
//		case SNDINFO_STR_CORE_CREDITS:					info->s = "Copyright (c) 2005, The MESS Team"; break;
//	}
//}
