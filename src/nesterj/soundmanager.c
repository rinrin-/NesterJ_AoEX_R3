//
// NES emu と PSPの架け橋(サウンド編)
//
#include <pspkernel.h>
#include "soundmanager.h"
#include "debug/debug.h"
#include "nes/apu/nes_apu_wrapper.h"
#include "main.h"


// サンプリングレート
int Snd_GetSampleRate(void)
{
	switch (setting.samplingrate) {
		case SAMPLINGRATE_11025:          return 11025;
		case SAMPLINGRATE_22050:          return 22050;
		case SAMPLINGRATE_44100: default: return 44100;
	}
}

/*
// 量子化ビット
int Snd_GetSampleBits(void)
{
	return 16;
}*/

// 現在再生中の場所を教える
int Snd_GetCurrentPlayingBank()
{
	return g_BeforePlayBank;
}


// Lock buffers
boolean Snd_Lock(int which, int16** buf, uint32* buf_len)
{
	static int g_SndRenderBank = 0;
	// RenderしたBankと再生中のBankが同じならFALSE
	if (g_SndRenderBank == g_BeforePlayBank) return FALSE;
	if (g_SndRenderBank < g_BeforePlayBank) {
		*buf = &g_SoundBuf[PGA_SAMPLES * g_SndRenderBank];
		*buf_len = (g_BeforePlayBank - g_SndRenderBank) * PGA_SAMPLES * sizeof(short);
		g_SndRenderBank = g_BeforePlayBank;
	}
	else {
		*buf = &g_SoundBuf[PGA_SAMPLES * g_SndRenderBank];
		*buf_len = (SOUND_BANK_COUNT - g_SndRenderBank) * PGA_SAMPLES * sizeof(short);
		g_SndRenderBank = 0;
	}
	return TRUE;
}

// Unlock buffers
void Snd_UnLock(void)
{
   // 何もしない
}

// clear buffer
void Snd_ClearBuffer(void)
{
 	DEBUG("Snd_ClearBuffer");
    _memset(g_SoundBuf, 0x00, sizeof(g_SoundBuf));
}

