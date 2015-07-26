#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H
#include "nes/types.h"

// 下記enumとchar*arrayは同じ順序で並んでいる必要があります。
enum {
	RAPID_30COUNT = 0,
	RAPID_20COUNT,
	RAPID_15COUNT,
	RAPID_10COUNT,
	RAPID_COUNT	// Last
};

extern const char *aszRapidModeName[];

void InputUpdatePad(void);
void InputUpdateRapid(void);

typedef struct {
	boolean bRapidA;
	boolean bRapidB;
	int nSpeedMode;
}KeyState;

extern KeyState stKeyState;

#endif
