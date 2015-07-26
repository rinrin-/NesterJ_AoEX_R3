#include <pspkernel.h>
#include "pg.h"
#include "string.h"
#include "sound.h"


unsigned long g_CurPlayBank = 0;
unsigned long g_BeforePlayBank = 0;
int wavout_enable=0;
short g_SoundBuf[SOUND_BANKLEN];
short g_SoundBufSilent[PGA_SAMPLES];

//44100,chan:1å≈íË
static void *wavout_snd0_callback(void)
{
	if (!wavout_enable) {
		return g_SoundBufSilent;
	}

	g_BeforePlayBank = g_CurPlayBank;
	// éüÇÃÉoÉìÉNçƒê∂
	g_CurPlayBank = (g_CurPlayBank + 1) % SOUND_BANK_COUNT;
	return &g_SoundBuf[g_CurPlayBank*(SOUND_BANKLEN / SOUND_BANK_COUNT)];
}


//return 0 if success
int wavoutInit()
{
	_memset(g_SoundBuf, 0x00, sizeof(g_SoundBuf));
	_memset(g_SoundBufSilent, 0x00, sizeof(g_SoundBufSilent));
	wavout_enable=0;
	pgaSetChannelCallback(wavout_snd0_callback);
	pgaInit();
	return 0;
}

