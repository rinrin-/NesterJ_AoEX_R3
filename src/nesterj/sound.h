#ifndef SOUND_H_
#define SOUND_H_
#include "pg.h"

extern int wavout_enable;
extern unsigned long g_CurPlayBank;		// ç°
extern unsigned long g_BeforePlayBank;	// ëO

int wavoutInit(void);


#define SOUND_BANK_COUNT 10

#define SOUND_BANKLEN (PGA_SAMPLES*SOUND_BANK_COUNT)

extern short g_SoundBuf[SOUND_BANKLEN];

#endif
