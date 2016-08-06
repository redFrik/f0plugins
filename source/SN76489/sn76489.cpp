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
	SN76489 - CrabEmu's SegaMasterSystem Chip Sound Simulator for SC
	Author Lawrence Sebald, Ported by Fredrik Olofsson 070706
*/

#include "SC_PlugIn.h"
static InterfaceTable *ft;



/*
    This file is part of CrabEmu.

    Copyright (C) 2005, 2006, 2007 Lawrence Sebald

    CrabEmu is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 
    as published by the Free Software Foundation.

    CrabEmu is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with CrabEmu; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <stdio.h>
#include "sn76489.h"

/* These constants came from Maxim's core. */
static const int volume_values[16] = { 
    892, 774, 669, 575, 492, 417, 351, 292,
    239, 192, 150, 113,  80,  50,  24,   0
};






extern "C" {											//f0
	void load(InterfaceTable *inTable);
	void SN76489_Ctor(SN76489 *unit);
	void SN76489_next_k(SN76489 *unit, int inNumSamples);
};

PluginLoad(InterfaceTable *inTable) {					//f0
	ft= inTable;
	DefineSimpleUnit(SN76489);
}

void SN76489_Ctor(SN76489 *unit) {						//f0
	SETCALC(SN76489_next_k);
	unit->counter[0] = 0x00;
    unit->counter[1] = 0x00;
    unit->counter[2] = 0x00;
    unit->counter[3] = 0x00;
	unit->tone_state[0] = 1;
    unit->tone_state[1] = 1;
    unit->tone_state[2] = 1;
    unit->tone_state[3] = 1;
	unit->noise_shift = unit->noise_initial_shift = 0x8000;
    unit->noise_tapped = 0x0009;
	SN76489_next_k(unit, 1);
}

void SN76489_next_k(SN76489 *unit, int numSamples) {	//f0
	float *out= OUT(0);
	unit->tone[0] = (((unsigned short)IN0(0)) & 0x3FF);
    unit->tone[1] = (((unsigned short)IN0(1)) & 0x3FF);
    unit->tone[2] = (((unsigned short)IN0(2)) & 0x3FF);
	unsigned char newNoise= (((unsigned char)IN0(3)) & 0x07);
	if(newNoise!=unit->noise) {
		unit->noise= newNoise;
		unit->noise_shift = unit->noise_initial_shift;
	}
	unit->volume[0] = 0x0F - (((unsigned char)IN0(4)) & 0x0F);
	unit->volume[1] = 0x0F - (((unsigned char)IN0(5)) & 0x0F);
	unit->volume[2] = 0x0F - (((unsigned char)IN0(6)) & 0x0F);
	unit->volume[3] = 0x0F - (((unsigned char)IN0(7)) & 0x0F);
	unit->clocks_per_sample = 3579545.0f / 16.0f / SAMPLERATE * IN0(8);	//NTSC
	//unit->clocks_per_sample = 3546893.0f / 16.0f / SAMPLERATE * IN0(8);	//PAL
	sn76489_execute_samples(unit, out, numSamples);
}



//f0 -this one is not in use
/*
int sn76489_init(sn76489_t *psg, float clock, float sample_rate,
                 unsigned short noise_shift, unsigned short tapped) {
    psg->volume[0] = 0xF;
    psg->volume[1] = 0xF;
    psg->volume[2] = 0xF;
    psg->volume[3] = 0xF;

    psg->tone[0] = 0x00;
    psg->tone[1] = 0x00;
    psg->tone[2] = 0x00;
    psg->noise = 0x00;

    psg->latched_reg = LATCH_TONE0;

    psg->counter[0] = 0x00;
    psg->counter[1] = 0x00;
    psg->counter[2] = 0x00;
    psg->counter[3] = 0x00;

    psg->tone_state[0] = 1;
    psg->tone_state[1] = 1;
    psg->tone_state[2] = 1;
    psg->tone_state[3] = 1;

    psg->enabled_channels = 0x0F;

    psg->output_channels = 0xFF; // All Channels, both sides

    psg->clocks_per_sample = clock / 16.0f / sample_rate;

    psg->noise_shift = psg->noise_initial_shift = noise_shift;
    psg->noise_tapped = tapped;

    return 0;
}
*/

//f0 -this one is not in use
/*
void sn76489_write(sn76489_t *psg, unsigned char byte)  {
    if(byte & 0x80) {
        // This is a LATCH/DATA byte 
        psg->latched_reg = (byte & 0x70);

        switch(psg->latched_reg)    {
            case LATCH_TONE0:
                psg->tone[0] = (psg->tone[0] & 0x3F0) | (byte & 0x0F);
                break;
            case LATCH_TONE1:
                psg->tone[1] = (psg->tone[1] & 0x3F0) | (byte & 0x0F);
                break;
            case LATCH_TONE2:
                psg->tone[2] = (psg->tone[2] & 0x3F0) | (byte & 0x0F);
                break;
            case LATCH_NOISE:
                psg->noise = (byte & 0x07);
                psg->noise_shift = psg->noise_initial_shift;
                break;
            case LATCH_VOL0:
                psg->volume[0] = (byte & 0x0F);
                break;
            case LATCH_VOL1:
                psg->volume[1] = (byte & 0x0F);
                break;
            case LATCH_VOL2:
                psg->volume[2] = (byte & 0x0F);
                break;
            case LATCH_VOL3:
                psg->volume[3] = (byte & 0x0F);
                break;
        }
    }
    else    {
        // This is a DATA byte 
        switch(psg->latched_reg)    {
            case LATCH_TONE0:
                psg->tone[0] = (psg->tone[0] & 0x000F) | ((byte & 0x3F) << 4);
                break;
            case LATCH_TONE1:
                psg->tone[1] = (psg->tone[1] & 0x000F) | ((byte & 0x3F) << 4);
                break;
            case LATCH_TONE2:
                psg->tone[2] = (psg->tone[2] & 0x000F) | ((byte & 0x3F) << 4);
                break;
            case LATCH_NOISE:
                psg->noise = (byte & 0x07);
                psg->noise_shift = psg->noise_initial_shift;
                break;
            case LATCH_VOL0:
                psg->volume[0] = (byte & 0x0F);
                break;
            case LATCH_VOL1:
                psg->volume[1] = (byte & 0x0F);
                break;
            case LATCH_VOL2:
                psg->volume[2] = (byte & 0x0F);
                break;
            case LATCH_VOL3:
                psg->volume[3] = (byte & 0x0F);
                break;
        }
    }
}
*/

/* This is pretty much taken directly from Maxim's SN76489 document. */
static inline int parity(unsigned short input)    {
    input ^= input >> 8;
    input ^= input >> 4;
    input ^= input >> 2;
    input ^= input >> 1;
    return input & 1;
}

void sn76489_execute_samples(sn76489_t *psg, float *buf,		//f0 -changed to float
                             int samples) {						//f0 -changed to int
    signed int channels[4];
    int i, j;													//f0 -changed to int

    for(i = 0; i < samples; ++i)    {
        for(j = 0; j < 3; ++j)  {
            psg->counter[j] -= psg->clocks_per_sample;
			channels[j] = //((psg->enabled_channels >> j) & 0x01) *		//f0 -commented out
                          psg->tone_state[j] * volume_values[psg->volume[j]];
            if(psg->counter[j] <= 0.0f) {
                if(psg->tone[j] < 7)    {
                    /* The PSG doesn't change states if the tone isn't at least
                       7, this fixes the "Sega" at the beginning of Sonic The
                       Hedgehog 2 for the Game Gear. */
                    psg->tone_state[j] = 1;
                }
                else    {
                    psg->tone_state[j] = -psg->tone_state[j];
                }

                psg->counter[j] += psg->tone[j];
            }
        }

        channels[3] = //((psg->enabled_channels >> 3) & 0x01) *			//f0 -commented out
                      (psg->noise_shift & 0x01) * volume_values[psg->volume[3]];

        psg->counter[3] -= psg->clocks_per_sample;
        
        if(psg->counter[3] < 0.0f)  {
            psg->tone_state[3] = -psg->tone_state[3];
            if((psg->noise & 0x03) == 0x03) {
                psg->counter[3] = psg->counter[2];
            }
            else    {
                psg->counter[3] += 0x10 << (psg->noise & 0x03);
            }

            if(psg->tone_state[3] == 1) {
                if(psg->noise & 0x04)   {
                    psg->noise_shift = (psg->noise_shift >> 1) |
                                        (parity(psg->noise_shift &
                                                psg->noise_tapped) << 15);
                }
                else    {
                    psg->noise_shift = (psg->noise_shift >> 1) |
                                       ((psg->noise_shift & 0x01) << 15);
                }
            }
        }
		
		//f0 -changed from stereo to mono and to float
		buf[i] = (channels[0] + channels[1] + channels[2] + channels[3]) /*& 0xFFFF*/ /3600.f;	//f0
		
        /*												//f0 -commented out
		buf[i << 1] = buf[(i << 1) + 1] = 0;
		
        if(psg->output_channels & TONE0_LEFT)
            buf[i << 1] += channels[0];

        if(psg->output_channels & TONE1_LEFT)
            buf[i << 1] += channels[1];

        if(psg->output_channels & TONE2_LEFT)
            buf[i << 1] += channels[2];

        if(psg->output_channels & NOISE_LEFT)
            buf[i << 1] += channels[3];
		
		if(psg->output_channels & TONE0_RIGHT)
            buf[(i << 1) + 1] += channels[0];

        if(psg->output_channels & TONE1_RIGHT)
            buf[(i << 1) + 1] += channels[1];

        if(psg->output_channels & TONE2_RIGHT)
            buf[(i << 1) + 1] += channels[2];

        if(psg->output_channels & NOISE_RIGHT)
            buf[(i << 1) + 1] += channels[3];
		*/

        //buf[i << 1] = (channels[0] + channels[1] + channels[2] + channels[3]) & 0xFFFF;
        //buf[(i << 1) + 1] = buf[i << 1];
    }
}
