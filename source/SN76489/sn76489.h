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


//slightly modified for sc 070706 /f0


#ifndef SN76489_H
#define SN76489_H

typedef struct SN76489 : public Unit {		//f0 -renamed and made public unit
    unsigned char volume[4];
    unsigned short tone[3];
    unsigned char noise;

    unsigned short noise_shift;
    unsigned short noise_initial_shift;
    unsigned short noise_tapped;

    signed char tone_state[4];

    //unsigned char latched_reg;			//f0 -commented out

    float counter[4];

    //unsigned char enabled_channels;		//f0 -commented out

    //unsigned char output_channels;		//f0 -commented out

    float clocks_per_sample;
} sn76489_t;

#define LATCH_TONE0 0x00
#define LATCH_TONE1 0x20
#define LATCH_TONE2 0x40
#define LATCH_NOISE 0x60

#define LATCH_VOL0 0x10
#define LATCH_VOL1 0x30
#define LATCH_VOL2 0x50
#define LATCH_VOL3 0x70

#define ENABLE_TONE0 0x01
#define ENABLE_TONE1 0x02
#define ENABLE_TONE2 0x04
#define ENABLE_NOISE 0x08

/* Channel outputs */
#define TONE0_RIGHT 0x01
#define TONE1_RIGHT 0x02
#define TONE2_RIGHT 0x04
#define NOISE_RIGHT 0x08
#define TONE0_LEFT  0x10
#define TONE1_LEFT  0x20
#define TONE2_LEFT  0x40
#define NOISE_LEFT  0x80

int sn76489_init(sn76489_t *psg, float clock, float sample_rate,
                 unsigned short noise_shift, unsigned short tapped);
void sn76489_write(sn76489_t *psg, unsigned char byte);
void sn76489_execute_samples(sn76489_t *psg, float *buf,		//f0 -changed to float
                             int samples);						//f0 -changed to int

#endif
