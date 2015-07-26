#ifndef _SOUND_MANAGER
#define _SOUND_MANAGER

#include "nes/types.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

// 下記enumとchar*arrayは同じ順序で並んでいる必要があります。
enum {
	SAMPLINGRATE_44100 = 0,
	SAMPLINGRATE_22050,
	SAMPLINGRATE_11025,
	SAMPLINGRATE_COUNT	// Last
};

extern const char *aszSamplingRateName[];

// サンプリングレート
int Snd_GetSampleRate(void);

// 量子化ビット
int Snd_GetSampleBits(void);

// 現在再生中の場所を教える(全体を中心からみて前か後ろか)
// returns SOUND_BUF_LOW or SOUND_BUF_HIGH
int Snd_GetCurrentPlayingBank();

// Lock buffer
boolean Snd_Lock(int which, int16** buf, uint32* buf_len);

// Unlock buffer
void Snd_UnLock(void);

// clear buffer
void Snd_ClearBuffer(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
