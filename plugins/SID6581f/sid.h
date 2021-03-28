#ifndef __SID_H_
#define __SID_H_

/*
  approximation of the sid6581 chip
  this part is for one chip,
*/

#include "sid6581.h"												//f0 -changed from "sound/sid6581.h"
#include "sidvoice.h"
//#include "streams.h"												//f0 -commented out

/* private area */
typedef struct _SID6581 : public Unit								//f0 -made public unit
{
    //sound_stream *mixer_channel; // mame stream/ mixer channel	//f0 -commented out
	
	int last00, last01, last02, last03, last04, last05, last06;		//f0 -added these
	int last07, last08, last09, last0A, last0B, last0C, last0D;
	int last0E, last0F, last10, last11, last12, last13, last14;
	int last15, last16, last17, last18;
	
    //int (*ad_read) (int which);									//f0 -commented out
    SIDTYPE type;
    uint32 clock;

    uint16 PCMfreq; // samplerate of the current systems soundcard/DAC
    uint32 PCMsid, PCMsidNoise;

#if 0
	/* following depends on type */
	ptr2sidVoidFunc ModeNormalTable[16];
	ptr2sidVoidFunc ModeRingTable[16];
	// for speed reason it could be better to make them global!
	uint8* waveform30;
	uint8* waveform50;
	uint8* waveform60;
	uint8* waveform70;
#endif
	int reg[0x20];

//  bool sidKeysOn[0x20], sidKeysOff[0x20];

	uint8 masterVolume;
	uint16 masterVolumeAmplIndex;


	struct
	{
		int Enabled;
		uint8 Type, CurType;
		float Dy, ResDy;
		uint16 Value;
	} filter;

	sidOperator optr1, optr2, optr3;
    int optr3_outputmask;
} SID6581f;																	//f0 -added f

void sid6581_init (SID6581f *This);

int sidEmuReset(SID6581f *This);

//int sid6581_port_r (SID6581 *This, int offset);							//f0 -commented out
void sid6581_port_w (SID6581f *This, int offset, int data);

//void sid_set_type(SID6581 *This, SIDTYPE type);							//f0 -commented out

void initMixerEngine(void);
void filterTableInit(void);
extern void MixerInit(int threeVoiceAmplify);

void sidEmuFillBuffer(SID6581f *This, float *buffer, uint32 bufferLen );		//f0 -changed to float

#if 0
void sidFilterTableInit(void);
#endif

#endif
