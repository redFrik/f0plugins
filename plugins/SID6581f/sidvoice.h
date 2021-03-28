#ifndef __SIDVOICE_H_
#define __SIDVOICE_H_

//#include "sndintrf.h"				//f0 -commented out

/*
  approximation of the sid6581 chip
  this part is for 1 (of the 3) voices of a chip
*/
#include "sid6581.h"				//f0 -changed from "sound/sid6581.h"

struct sw_storage
{
	uint16 len;
#if defined(DIRECT_FIXPOINT)
	uint32 stp;
#else
	uint32 pnt;
	int16 stp;
#endif
};

struct _SID6581;

typedef struct _sidOperator
{
	struct _SID6581 *sid;
	uint8 reg[7];
	uint32 SIDfreq;
	uint16 SIDpulseWidth;
	uint8 SIDctrl;
	uint8 SIDAD, SIDSR;

	struct _sidOperator* carrier;
	struct _sidOperator* modulator;
	int sync;

	uint16 pulseIndex, newPulseIndex;
	uint16 curSIDfreq;
	uint16 curNoiseFreq;

    uint8 output;//, outputMask;

	char filtVoiceMask;
	int filtEnabled;
	float filtLow, filtRef;
	int8 filtIO;

	int32 cycleLenCount;
#if defined(DIRECT_FIXPOINT)
	cpuLword cycleLen, cycleAddLen;
#else
	uint32 cycleAddLenPnt;
	uint16 cycleLen, cycleLenPnt;
#endif

	int8(*outProc)(struct _sidOperator *);
	void(*waveProc)(struct _sidOperator *);

#if defined(DIRECT_FIXPOINT)
	cpuLword waveStep, waveStepAdd;
#else
	uint16 waveStep, waveStepAdd;
	uint32 waveStepPnt, waveStepAddPnt;
#endif
	uint16 waveStepOld;
	struct sw_storage wavePre[2];

#if defined(DIRECT_FIXPOINT) && defined(LARGE_NOISE_TABLE)
	cpuLword noiseReg;
#elif defined(DIRECT_FIXPOINT)
	cpuLBword noiseReg;
#else
	uint32 noiseReg;
#endif
	uint32 noiseStep, noiseStepAdd;
	uint8 noiseOutput;
	int noiseIsLocked;

	uint8 ADSRctrl;
//  int gateOnCtrl, gateOffCtrl;
	uint16 (*ADSRproc)(struct _sidOperator *);

#ifdef SID_FPUENVE
	float fenveStep, fenveStepAdd;
	uint32 enveStep;
#elif defined(DIRECT_FIXPOINT)
	cpuLword enveStep, enveStepAdd;
#else
	uint16 enveStep, enveStepAdd;
	uint32 enveStepPnt, enveStepAddPnt;
#endif
	uint8 enveVol, enveSusVol;
	uint16 enveShortAttackCount;
} sidOperator;

typedef int8 (*ptr2sidFunc)(sidOperator *);
typedef uint16 (*ptr2sidUwordFunc)(sidOperator *);
typedef void (*ptr2sidVoidFunc)(sidOperator *);

void sidClearOperator( sidOperator* pVoice );

void sidEmuSet(sidOperator* pVoice);
void sidEmuSet2(sidOperator* pVoice);
int8 sidWaveCalcNormal(sidOperator* pVoice);

void sidInitWaveformTables(SIDTYPE type);
void sidInitMixerEngine(void);

#if 0
extern ptr2sidVoidFunc sid6581ModeNormalTable[16];
extern ptr2sidVoidFunc sid6581ModeRingTable[16];
extern ptr2sidVoidFunc sid8580ModeNormalTable[16];
extern ptr2sidVoidFunc sid8580ModeRingTable[16];
#endif

#endif
