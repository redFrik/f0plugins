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
	SID6581f - SID Chip Sound Simulator for SC
	Author Peter Trauner, Michael Schwend
	
	Ported from the MAME 0.116 package by Fredrik Olofsson 070714
	but faulty!  not working as it should compared to a real sid.
	though still useful and i'll keep this as is.
	if i fix it it'll be released as a different ugen.
*/

#include "SC_PlugIn.h"
static InterfaceTable *ft;



/*
DIRECT_FIXPOINT
SID_REFTIMES
SID_FPUENVE
*/			//f0 -??? which one of these

#define SID_FPUENVE

#define FALSE 0
#define TRUE 1


/***************************************************************************

    sid6581.c

    MAME/MESS interface for SID6581 and SID8580 chips

***************************************************************************/

//#include "sndintrf.h"										//f0 -commented out
#include "sid6581.h"
#include "sid.h"
#include "sidenvel.h"										//f0 -added this



static SIDTYPE sidtype= MOS6581;							//f0 -change here for different chips/ugens!!!
//static SIDTYPE sidtype= MOS8580;

extern "C" {												//f0
	void SID6581f_Ctor(SID6581f *unit);
	void SID6581_next_k(SID6581f *unit, int inNumSamples);
};

PluginLoad(InterfaceTable *inTable) {						//f0
	ft= inTable;
	DefineSimpleUnit(SID6581f);								//'faulty' version
	sidInitMixerEngine();
	filterTableInit();
	sidInitWaveformTables(sidtype);
	enveEmuInit(44100, TRUE);
	MixerInit(0);
}

void SID6581f_Ctor(SID6581f *unit) {						//f0
	SETCALC(SID6581_next_k);
	
	unit->PCMfreq = SAMPLERATE;
	unit->clock = 985250*IN0(25);							//f0 -??? was clock  1022730 ntsc, 985250 pal
	unit->type = sidtype;
	sid6581_init(unit);
	
	unit->last00= -1;
	unit->last01= -1;
	unit->last02= -1;
	unit->last03= -1;
	unit->last04= -1;
	unit->last05= -1;
	unit->last06= -1;
	unit->last07= -1;
	unit->last08= -1;
	unit->last09= -1;
	unit->last0A= -1;
	unit->last0B= -1;
	unit->last0C= -1;
	unit->last0D= -1;
	unit->last0E= -1;
	unit->last0F= -1;
	unit->last10= -1;
	unit->last11= -1;
	unit->last12= -1;
	unit->last13= -1;
	unit->last14= -1;
	unit->last15= -1;
	unit->last16= -1;
	unit->last17= -1;
	unit->last18= -1;
	unit->reg[0x15]= unit->last15-1;
	unit->reg[0x16]= unit->last16-1;
	unit->reg[0x17]= unit->last17-1;
	unit->reg[0x18]= unit->last18-1;
		
	SID6581_next_k(unit, 1);
}

void SID6581_next_k(SID6581f *unit, int inNumSamples) {		//f0
	float *out= OUT(0);
	
	if((int)IN0(0)!=unit->last00) {
		unit->last00= (int)IN0(0);
		sid6581_port_w(unit, 0x00, unit->last00);
	}
	if((int)IN0(1)!=unit->last01) {
		unit->last01= (int)IN0(1);
		sid6581_port_w(unit, 0x01, unit->last01);
	}
	if((int)IN0(2)!=unit->last02) {
		unit->last02= (int)IN0(2);
		sid6581_port_w(unit, 0x02, unit->last02);
	}
	if((((int)IN0(3))&15)!=unit->last03) {
		unit->last03= ((int)IN0(3))&15;
		sid6581_port_w(unit, 0x03, unit->last03);
	}
	if((int)IN0(4)!=unit->last04) {
		unit->last04= (int)IN0(4);
		sid6581_port_w(unit, 0x04, unit->last04);
	}
	if((int)IN0(5)!=unit->last05) {
		unit->last05= (int)IN0(5);
		sid6581_port_w(unit, 0x05, unit->last05);
	}
	if((int)IN0(6)!=unit->last06) {
		unit->last06= (int)IN0(6);
		sid6581_port_w(unit, 0x06, unit->last06);
	}
	if((int)IN0(7)!=unit->last07) {
		unit->last07= (int)IN0(7);
		sid6581_port_w(unit, 0x07, unit->last07);
	}
	if((int)IN0(8)!=unit->last08) {
		unit->last08= (int)IN0(8);
		sid6581_port_w(unit, 0x08, unit->last08);
	}
	if((int)IN0(9)!=unit->last09) {
		unit->last09= (int)IN0(9);
		sid6581_port_w(unit, 0x09, unit->last09);
	}
	if((((int)IN0(10))&15)!=unit->last0A) {
		unit->last0A= ((int)IN0(10))&15;
		sid6581_port_w(unit, 0x0A, unit->last0A);
	}
	if((int)IN0(11)!=unit->last0B) {
		unit->last0B= (int)IN0(11);
		sid6581_port_w(unit, 0x0B, unit->last0B);
	}
	if((int)IN0(12)!=unit->last0C) {
		unit->last0C= (int)IN0(12);
		sid6581_port_w(unit, 0x0C, unit->last0C);
	}
	if((int)IN0(13)!=unit->last0D) {
		unit->last0D= (int)IN0(13);
		sid6581_port_w(unit, 0x0D, unit->last0D);
	}
	if((int)IN0(14)!=unit->last0E) {
		unit->last0E= (int)IN0(14);
		sid6581_port_w(unit, 0x0E, unit->last0E);
	}
	if((int)IN0(15)!=unit->last0F) {
		unit->last0F= (int)IN0(15);
		sid6581_port_w(unit, 0x0F, unit->last0F);
	}
	if((int)IN0(16)!=unit->last10) {
		unit->last10= (int)IN0(16);
		sid6581_port_w(unit, 0x10, unit->last10);
	}
	if((((int)IN0(17))&15)!=unit->last11) {
		unit->last11= ((int)IN0(17))&15;
		sid6581_port_w(unit, 0x11, unit->last11);
	}
	if((int)IN0(18)!=unit->last12) {
		unit->last12= (int)IN0(18);
		sid6581_port_w(unit, 0x12, unit->last12);
	}
	if((int)IN0(19)!=unit->last13) {
		unit->last13= (int)IN0(19);
		sid6581_port_w(unit, 0x13, unit->last13);
	}
	if((int)IN0(20)!=unit->last14) {
		unit->last14= (int)IN0(20);
		sid6581_port_w(unit, 0x14, unit->last14);
	}
	if((((int)IN0(21))&7)!=unit->last15) {
		unit->last15= ((int)IN0(21))&7;
		sid6581_port_w(unit, 0x15, unit->last15);
	}
	if((int)IN0(22)!=unit->last16) {
		unit->last16= (int)IN0(22);
		sid6581_port_w(unit, 0x16, unit->last16);
	}
	if((int)IN0(23)!=unit->last17) {
		unit->last17= (int)IN0(23);
		sid6581_port_w(unit, 0x17, unit->last17);
	}
	if((int)IN0(24)!=unit->last18) {
		unit->last18= (int)IN0(24);
		sid6581_port_w(unit, 0x18, unit->last18);
	}
	
	//f0 -add dynamic rate later???
	
	sidEmuFillBuffer(unit, out, inNumSamples);
}





//f0 -commented out???
//static SID6581 *get_sid(int indx)
//{
//	int type = sndnum_to_sndti(indx, NULL);
//	assert((type == SOUND_SID6581) || (type == SOUND_SID8580));
//	return (SID6581 *) sndti_token(type, indx);
//}
//
//
//static void sid_update(void *token,stream_sample_t **inputs, stream_sample_t **_buffer,int length)
//{
//	SID6581 *sid = (SID6581 *) token;
//	sidEmuFillBuffer(sid, _buffer[0], length);
//}
//
//
//f0 -commented out
//static void *sid_start(int sndindex, int clock, const void *config, SIDTYPE sidtype)
//{
//	SID6581 *sid;
//	const SID6581_interface *iface = (const SID6581_interface*) config;
//
//	sid = (SID6581 *) auto_malloc(sizeof(*sid));
//	memset(sid, 0, sizeof(*sid));
//
//	sid->mixer_channel = stream_create (0, 1,  Machine->sample_rate, (void *) sid, sid_update);
//	sid->PCMfreq = Machine->sample_rate;
//	sid->clock = clock;
//	sid->ad_read = iface ? iface->ad_read : NULL;
//	sid->type = sidtype;
//
//	sid6581_init(sid);
//	sidInitWaveformTables(sidtype);
//	return sid;
//}
//
//
//static void sid_reset(void *token)
//{
//	SID6581 *sid = (SID6581 *) token;
//	sidEmuReset(sid);
//}
//
//
//static void *sid6581_start(int sndindex, int clock, const void *config)
//{
//	return sid_start(sndindex, clock, config, MOS6581);
//}
//
//
//
//static void *sid8580_start(int sndindex, int clock, const void *config)
//{
//	return sid_start(sndindex, clock, config, MOS8580);
//}
//
//
/*
READ8_HANDLER ( sid6581_0_port_r )
{
	return sid6581_port_r(get_sid(0), offset);
}

READ8_HANDLER ( sid6581_1_port_r )
{
	return sid6581_port_r(get_sid(1), offset);
}

WRITE8_HANDLER ( sid6581_0_port_w )
{
	sid6581_port_w(get_sid(0), offset, data);
}

WRITE8_HANDLER ( sid6581_1_port_w )
{
	sid6581_port_w(get_sid(1), offset, data);
}
*/


/**************************************************************************
 * Generic get_info
 **************************************************************************/
//f0 -commented out
//static void sid6581_set_info(void *token, UINT32 state, sndinfo *info)
//{
//	switch (state)
//	{
//		/* no parameters to set */
//	}
//}
//
//
//void sid6581_get_info(void *token, UINT32 state, sndinfo *info)
//{
//	switch (state)
//	{
//		/* --- the following bits of info are returned as 64-bit signed integers --- */
//
//		/* --- the following bits of info are returned as pointers to data or functions --- */
//		case SNDINFO_PTR_SET_INFO:						info->set_info = sid6581_set_info;		break;
//		case SNDINFO_PTR_START:							info->start = sid6581_start;			break;
//		case SNDINFO_PTR_STOP:							info->stop = NULL;						break;
//		case SNDINFO_PTR_RESET:							info->reset = sid_reset;				break;
//
//		/* --- the following bits of info are returned as NULL-terminated strings --- */
//		case SNDINFO_STR_NAME:							info->s = "SID6581";					break;
//		case SNDINFO_STR_CORE_FAMILY:					info->s = "SID";						break;
//		case SNDINFO_STR_CORE_VERSION:					info->s = "1.0";						break;
//		case SNDINFO_STR_CORE_FILE:						info->s = __FILE__;						break;
//		case SNDINFO_STR_CORE_CREDITS:					info->s = "Copyright (c) 2005, The MESS Team"; break;
//	}
//}
//
//
//void sid8580_get_info(void *token, UINT32 state, sndinfo *info)
//{
//	switch (state)
//	{
//		/* --- the following bits of info are returned as pointers to data or functions --- */
//		case SNDINFO_PTR_START:							info->start = sid8580_start;			break;
//
//		/* --- the following bits of info are returned as NULL-terminated strings --- */
//		case SNDINFO_STR_NAME:							info->s = "SID8580";					break;
//		default:										sid6581_get_info(token, state, info);	break;
//	}
//}

