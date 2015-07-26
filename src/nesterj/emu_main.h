#ifndef EMU_MAIN_H
#define EMU_MAIN_H
//#include "nes/types.h"

enum {
	EMU_SPEED_NORMAL,
	EMU_SPEED_MODE1,
	EMU_SPEED_MODE2
};

// NES init
void PSPEMU_NES_Init(void);
// 指定した番号のステート呼び出し
u8 PSPEMU_LoadState(u32 ulNum);
// 指定した番号にステート保存
u8 PSPEMU_SaveState(u32 ulNum);
// 指定した番号にステート削除
u8 PSPEMU_DeleteState(u32 ulNum);
// Screen shot保存
u8 PSPEMU_SaveScreenShot(void);

// メニュー入るとき呼び出す
void PSPEMU_Freeze(void);

// Freeseして処理終えたら呼び出す
void PSPEMU_Thaw(void);

// get disk side number
u8 PSPEMU_GetDiskSideNum(void);

// set disk side
void PSPEMU_SetDiskSide(u8 side);

// get disk side
u8 PSPEMU_GetDiskSide(void);

// apply game genie code
int PSPEMU_ApplyGameGenie(const char *pszFile);
int PSPEMU_ApplyGameCheat(const char *pszFile);

// Apply graphics config
void PSPEMU_ApplyGraphicsConfig(void);

// Apply sound config
void PSPEMU_ApplySoundConfig(void);

// Save ram
void PSPEMU_SaveRAM(void);

// 現在の設定でのフレームレート設定を行う(internal)
void PSPEMU_SetFrameSpeed(void);

// エミュメイン
void PSPEMU_DoFrame(void);

#endif
