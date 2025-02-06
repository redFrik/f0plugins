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
	Atari2600 - TIA Chip Sound Simulator for SC
	Author Ron Fries, Ported by Fredrik Olofsson 070211
	(updated 070226 - added playback rate argument /f0)
*/

#include "SC_PlugIn.h"
static InterfaceTable *ft;


/*****************************************************************************/
/*                                                                           */
/* Module:  TIA Chip Sound Simulator                                         */
/* Purpose: To emulate the sound generation hardware of the Atari TIA chip.  */
/* Author:  Ron Fries                                                        */
/*                                                                           */
/* Revision History:                                                         */
/*    10-Sep-96 - V1.0 - Initial Release                                     */
/*    14-Jan-97 - V1.1 - Cleaned up sound output by eliminating counter      */
/*                       reset.                                              */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*                 License Information and Copyright Notice                  */
/*                 ========================================                  */
/*                                                                           */
/* TiaSound is Copyright(c) 1996 by Ron Fries                                */
/*                                                                           */
/* This library is free software; you can redistribute it and/or modify it   */
/* under the terms of version 2 of the GNU Library General Public License    */
/* as published by the Free Software Foundation.                             */
/*                                                                           */
/* This library is distributed in the hope that it will be useful, but       */
/* WITHOUT ANY WARRANTY; without even the implied warranty of                */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library */
/* General Public License for more details.                                  */
/* To obtain a copy of the GNU Library General Public License, write to the  */
/* Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.   */
/*                                                                           */
/* Any permitted reproduction of these routines, in whole or in part, must   */
/* bear this legend.                                                         */
/*                                                                           */
/*****************************************************************************/


/* definitions for AUDCx (15, 16) */
#define SET_TO_1     0x00      /* 0000 */
#define POLY4        0x01      /* 0001 */
#define DIV31_POLY4  0x02      /* 0010 */
#define POLY5_POLY4  0x03      /* 0011 */
#define PURE         0x04      /* 0100 */
#define PURE2        0x05      /* 0101 */
#define DIV31_PURE   0x06      /* 0110 */
#define POLY5_2      0x07      /* 0111 */
#define POLY9        0x08      /* 1000 */
#define POLY5        0x09      /* 1001 */
#define DIV31_POLY5  0x0a      /* 1010 */
#define POLY5_POLY5  0x0b      /* 1011 */
#define DIV3_PURE    0x0c      /* 1100 */
#define DIV3_PURE2   0x0d      /* 1101 */
#define DIV93_PURE   0x0e      /* 1110 */
#define DIV3_POLY5   0x0f      /* 1111 */
                 
#define DIV3_MASK    0x0c                 
                 
#define AUDC0        0x15
#define AUDC1        0x16
#define AUDF0        0x17
#define AUDF1        0x18
#define AUDV0        0x19
#define AUDV1        0x1a

/* the size (in entries) of the 4 polynomial tables */
#define POLY4_SIZE  0x000f
#define POLY5_SIZE  0x001f
#define POLY9_SIZE  0x01ff

/* channel definitions */
#define CHAN1       0
#define CHAN2       1


/* Initialze the bit patterns for the polynomials. */

/* The 4bit and 5bit patterns are the identical ones used in the tia chip. */
/* Though the patterns could be packed with 8 bits per byte, using only a */
/* single bit per byte keeps the math simple, which is important for */
/* efficient processing. */

static uint8 Bit4[POLY4_SIZE] =
      { 1,1,0,1,1,1,0,0,0,0,1,0,1,0,0 };

static uint8 Bit5[POLY5_SIZE] =
      { 0,0,1,0,1,1,0,0,1,1,1,1,1,0,0,0,1,1,0,1,1,1,0,1,0,1,0,0,0,0,1 };

/* I've treated the 'Div by 31' counter as another polynomial because of */
/* the way it operates.  It does not have a 50% duty cycle, but instead */
/* has a 13:18 ratio (of course, 13+18 = 31).  This could also be */
/* implemented by using counters. */

static uint8 Div31[POLY5_SIZE] =
      { 0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0 };



struct Atari2600 : public Unit {
	
	/* to hold the 6 tia sound control bytes */
	uint8 AUDC[2];			/* AUDCx (15, 16) */
	uint8 AUDF[2];			/* AUDFx (17, 18) */
	uint8 AUDV[2];			/* AUDVx (19, 1A) */
	
	/* last output volume for each channel */
	uint8 Outvol[2];
	
	/* Rather than have a table with 511 entries, I use a random number generator. */
	uint8 Bit9[POLY9_SIZE];
	
	uint8 P4[2];			/* Position pointer for the 4-bit POLY array */
	uint8 P5[2];			/* Position pointer for the 5-bit POLY array */
	uint16 P9[2];			/* Position pointer for the 9-bit POLY array */
	uint8 Div_n_cnt[2];		/* Divide by n counter. one for each channel */
	uint8 Div_n_max[2];		/* Divide by n maximum, one for each channel */
	
	/* In my routines, I treat the sample output as another divide by N counter. */
	/* For better accuracy, the Samp_n_cnt has a fixed binary decimal point */
	/* which has 8 binary digits to the right of the decimal point. */
	uint16 Samp_n_max;		/* Sample max, multiplied by 256 */
	int16 Samp_n_cnt;		/* Sample cnt. */
};

extern "C" {
	void Atari2600_Ctor(Atari2600 *unit);
	void Atari2600_next_k(Atari2600 *unit, int inNumSamples);
	void Tia_sound_init(Atari2600 *unit, uint16 sample_freq, uint16 playback_freq);
	void Update_tia_sound(Atari2600 *unit, uint16 addr, uint8 val);
	void Tia_process(Atari2600 *unit, float *buffer, uint16 n);
};

PluginLoad(InterfaceTable *inTable) {
	ft= inTable;
	DefineSimpleUnit(Atari2600);
}

void Atari2600_Ctor(Atari2600 *unit) {
	SETCALC(Atari2600_next_k);
	Tia_sound_init(unit, SAMPLERATE, SAMPLERATE);	//sample_freq, playback_freq
	Atari2600_next_k(unit, 1);
}

void Atari2600_next_k(Atari2600 *unit, int inNumSamples) {
	float *out= OUT(0);
	Update_tia_sound(unit, 0x15, IN0(0));			//unit, addr, val
	Update_tia_sound(unit, 0x16, IN0(1));
	Update_tia_sound(unit, 0x17, IN0(2));
	Update_tia_sound(unit, 0x18, IN0(3));
	Update_tia_sound(unit, 0x19, IN0(4));
	Update_tia_sound(unit, 0x1a, IN0(5));
	unit->Samp_n_max= (uint16)(((uint32)SAMPLERATE<<8)/SAMPLERATE)*IN0(6);
	Tia_process(unit, out, inNumSamples);			//unit, buffer, n
}


/*****************************************************************************/
/* Module:  Tia_sound_init()                                                 */
/* Purpose: to handle the power-up initialization functions                  */
/*          these functions should only be executed on a cold-restart        */
/*                                                                           */
/* Author:  Ron Fries                                                        */
/* Date:    September 10, 1996                                               */
/*                                                                           */
/* Inputs:  sample_freq - the value for the '30 Khz' Tia audio clock         */
/*          playback_freq - the playback frequency in samples per second     */
/*                                                                           */
/* Outputs: Adjusts local globals - no return value                          */
/*                                                                           */
/*****************************************************************************/

void Tia_sound_init (Atari2600 *unit, uint16 sample_freq, uint16 playback_freq)
{
   uint8 chan;
   int16 n;

   /* fill the 9bit polynomial with random bits */
   for (n=0; n<POLY9_SIZE; n++)
   {
      unit->Bit9[n] = rand() & 0x01;       /* fill poly9 with random bits */
   }

   /* calculate the sample 'divide by N' value based on the playback freq. */
   unit->Samp_n_max = (uint16)(((uint32)sample_freq<<8)/playback_freq);
   unit->Samp_n_cnt = 0;  /* initialize all bits of the sample counter */

   /* initialize the local globals */
   for (chan = CHAN1; chan <= CHAN2; chan++)
   {
      unit->Outvol[chan] = 0;
      unit->Div_n_cnt[chan] = 0;
      unit->Div_n_max[chan] = 0;
      unit->AUDC[chan] = 0;
      unit->AUDF[chan] = 0;
      unit->AUDV[chan] = 0;
      unit->P4[chan] = 0;
      unit->P5[chan] = 0;
      unit->P9[chan] = 0;
   }
}


/*****************************************************************************/
/* Module:  Update_tia_sound()                                               */
/* Purpose: To process the latest control values stored in the AUDF, AUDC,   */
/*          and AUDV registers.  It pre-calculates as much information as    */
/*          possible for better performance.  This routine has not been      */
/*          optimized.                                                       */
/*                                                                           */
/* Author:  Ron Fries                                                        */
/* Date:    January 14, 1997                                                 */
/*                                                                           */
/* Inputs:  addr - the address of the parameter to be changed                */
/*          val - the new value to be placed in the specified address        */
/*                                                                           */
/* Outputs: Adjusts local globals - no return value                          */
/*                                                                           */
/*****************************************************************************/

void Update_tia_sound (Atari2600 *unit, uint16 addr, uint8 val)
{
    uint16 new_val = 0;
    uint8 chan;

    /* determine which address was changed */
    switch (addr)
    {
       case AUDC0:
          unit->AUDC[0] = val & 0x0f;
          chan = 0;
          break;

       case AUDC1:
          unit->AUDC[1] = val & 0x0f;
          chan = 1;
          break;

       case AUDF0:
          unit->AUDF[0] = val & 0x1f;
          chan = 0;
          break;

       case AUDF1:
          unit->AUDF[1] = val & 0x1f;
          chan = 1;
          break;

       case AUDV0:
          unit->AUDV[0] = (val & 0x0f) << 3;
          chan = 0;
          break;

       case AUDV1:
          unit->AUDV[1] = (val & 0x0f) << 3;
          chan = 1;
          break;

       default:
          chan = 255;
          break;
    }

    /* if the output value changed */
    if (chan != 255)
    {
       /* an AUDC value of 0 is a special case */
       if (unit->AUDC[chan] == SET_TO_1)
       {
          /* indicate the clock is zero so no processing will occur */
          new_val = 0;

          /* and set the output to the selected volume */
          unit->Outvol[chan] = unit->AUDV[chan];
       }
       else
       {
          /* otherwise calculate the 'divide by N' value */
          new_val = unit->AUDF[chan] + 1;

          /* if bits 2 & 3 are set, then multiply the 'div by n' count by 3 */
          if ((unit->AUDC[chan] & DIV3_MASK) == DIV3_MASK)
          {
             new_val *= 3;
          }
       }

       /* only reset those channels that have changed */
       if (new_val != unit->Div_n_max[chan])
       {
          /* reset the divide by n counters */
          unit->Div_n_max[chan] = new_val;

          /* if the channel is now volume only or was volume only */
          if ((unit->Div_n_cnt[chan] == 0) || (new_val == 0))
          {
             /* reset the counter (otherwise let it complete the previous) */
             unit->Div_n_cnt[chan] = new_val;
          }
       }
    }
}


/*****************************************************************************/
/* Module:  Tia_process()                                                    */
/* Purpose: To fill the output buffer with the sound output based on the     */
/*          tia chip parameters.  This routine has been optimized.           */
/*                                                                           */
/* Author:  Ron Fries                                                        */
/* Date:    September 10, 1996                                               */
/*                                                                           */
/* Inputs:  *buffer - pointer to the buffer where the audio output will      */
/*                    be placed                                              */
/*          n - size of the playback buffer                                  */
/*                                                                           */
/* Outputs: the buffer will be filled with n bytes of audio - no return val  */
/*                                                                           */
/*****************************************************************************/

//void Tia_process (Atari2600 *unit, register unsigned char *buffer, register uint16 n)
void Tia_process (Atari2600 *unit, float *buffer, uint16 n)
{
    uint16 Samp_n_max = unit->Samp_n_max;
	int16 Samp_n_cnt = unit->Samp_n_cnt;
	
    uint8 audc0,audv0,audc1,audv1;
    uint8 div_n_cnt0,div_n_cnt1;
    uint8 p5_0, p5_1,outvol_0,outvol_1;

    audc0 = unit->AUDC[0];
    audv0 = unit->AUDV[0];
    audc1 = unit->AUDC[1];
    audv1 = unit->AUDV[1];

    /* make temporary local copy */
    p5_0 = unit->P5[0];
    p5_1 = unit->P5[1];
    outvol_0 = unit->Outvol[0];
    outvol_1 = unit->Outvol[1];
    div_n_cnt0 = unit->Div_n_cnt[0];
    div_n_cnt1 = unit->Div_n_cnt[1];

    /* loop until the buffer is filled */
    while (n)
    {
       /* Process channel 0 */
       if (div_n_cnt0 > 1)
       {
          div_n_cnt0--;
       }
       else if (div_n_cnt0 == 1)
       {
          div_n_cnt0 = unit->Div_n_max[0];

          /* the P5 counter has multiple uses, so we inc it here */
          p5_0++;
          if (p5_0 == POLY5_SIZE)
             p5_0 = 0;

          /* check clock modifier for clock tick */
          if  (((audc0 & 0x02) == 0) ||
              (((audc0 & 0x01) == 0) && Div31[p5_0]) ||
              (((audc0 & 0x01) == 1) &&  Bit5[p5_0]))
          {
             if (audc0 & 0x04)       /* pure modified clock selected */
             {
                if (outvol_0)        /* if the output was set */
                   outvol_0 = 0;     /* turn it off */
                else
                   outvol_0 = audv0; /* else turn it on */
             }
             else if (audc0 & 0x08)    /* check for p5/p9 */
             {
                if (audc0 == POLY9)    /* check for poly9 */
                {
                   /* inc the poly9 counter */
                   unit->P9[0]++;
                   if (unit->P9[0] == POLY9_SIZE)
                      unit->P9[0] = 0;

                   if (unit->Bit9[unit->P9[0]])
                      outvol_0 = audv0;
                   else
                      outvol_0 = 0;
                }
                else                        /* must be poly5 */
                {
                   if (Bit5[p5_0])
                      outvol_0 = audv0;
                   else
                      outvol_0 = 0;
                }
             }
             else  /* poly4 is the only remaining option */
             {
                /* inc the poly4 counter */
                unit->P4[0]++;
                if (unit->P4[0] == POLY4_SIZE)
                   unit->P4[0] = 0;

                if (Bit4[unit->P4[0]])
                   outvol_0 = audv0;
                else
                   outvol_0 = 0;
             }
          }
       }


       /* Process channel 1 */
       if (div_n_cnt1 > 1)
       {
          div_n_cnt1--;
       }
       else if (div_n_cnt1 == 1)
       {
          div_n_cnt1 = unit->Div_n_max[1];

          /* the P5 counter has multiple uses, so we inc it here */
          p5_1++;
          if (p5_1 == POLY5_SIZE)
             p5_1 = 0;

          /* check clock modifier for clock tick */
          if  (((audc1 & 0x02) == 0) ||
              (((audc1 & 0x01) == 0) && Div31[p5_1]) ||
              (((audc1 & 0x01) == 1) &&  Bit5[p5_1]))
          {
             if (audc1 & 0x04)       /* pure modified clock selected */
             {
                if (outvol_1)        /* if the output was set */
                   outvol_1 = 0;     /* turn it off */
                else
                   outvol_1 = audv1; /* else turn it on */
             }
             else if (audc1 & 0x08)    /* check for p5/p9 */
             {
                if (audc1 == POLY9)    /* check for poly9 */
                {
                   /* inc the poly9 counter */
                   unit->P9[1]++;
                   if (unit->P9[1] == POLY9_SIZE)
                      unit->P9[1] = 0;

                   if (unit->Bit9[unit->P9[1]])
                      outvol_1 = audv1;
                   else
                      outvol_1 = 0;
                }
                else                        /* must be poly5 */
                {
                   if (Bit5[p5_1])
                      outvol_1 = audv1;
                   else
                      outvol_1 = 0;
                }
             }
             else  /* poly4 is the only remaining option */
             {
                /* inc the poly4 counter */
                unit->P4[1]++;
                if (unit->P4[1] == POLY4_SIZE)
                   unit->P4[1] = 0;

                if (Bit4[unit->P4[1]])
                   outvol_1 = audv1;
                else
                   outvol_1 = 0;
             }
          }
       }

       /* decrement the sample counter - value is 256 since the lower
          byte contains the fractional part */
       Samp_n_cnt -= 256;

       /* if the count down has reached zero */
       if (Samp_n_cnt < 256)
       {
          /* adjust the sample counter */
          Samp_n_cnt += Samp_n_max;

          /* calculate the latest output value and place in buffer */
		  *(buffer++) = ((outvol_0 + outvol_1)*0.00390625);
		  
          /* and indicate one less byte to process */
          n--;
       }
    }

    /* save for next round */
    unit->P5[0] = p5_0;
    unit->P5[1] = p5_1;
    unit->Outvol[0] = outvol_0;
    unit->Outvol[1] = outvol_1;
    unit->Div_n_cnt[0] = div_n_cnt0;
    unit->Div_n_cnt[1] = div_n_cnt1;
	unit->Samp_n_max = Samp_n_max;
	unit->Samp_n_cnt = Samp_n_cnt;
}
