#ifndef _MZPOKEYSND_H_
#define _MZPOKEYSND_H_




//--taken from atari.h		/f0

#ifndef FALSE
#define FALSE  0
#endif
#ifndef TRUE
#define TRUE   1
#endif

/* SBYTE and UBYTE must be exactly 1 byte long. */
/* SWORD and UWORD must be exactly 2 bytes long. */
/* SLONG and ULONG must be exactly 4 bytes long. */
#define SBYTE signed char
#define SWORD signed short
#define SLONG signed int
#define UBYTE unsigned char
#define UWORD unsigned short
#define ULONG unsigned int
/* Note: in various parts of the emulator we assume that char is 1 byte
   and int is 4 bytes. */

//--until here				/f0





//#include "config.h"		//f0
#include "pokeysnd.h"
/*extern "C" {				//f0
int Pokey_sound_init_mz(PokeyState *ps, uint32 freq17,
                        uint16 playback_freq,
                        uint8 num_pokeys,
                        int flags,
                        int quality
#ifdef __PLUS
                        , int clear_regs
#endif
                       );
}*/
#endif /* _MZPOKEYSND_H_ */
