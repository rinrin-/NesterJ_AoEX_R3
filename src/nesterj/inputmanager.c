/*
	Input manager by ruka
*/
#include <pspkernel.h>
#include <pspctrl.h>
#include "main.h"
#include "nes/nes_pad.h"
#include "emu_main.h"
#include "nes/nes_config.h"
#include "inputmanager.h"
#include "screenmanager.h"
#include "menu_submenu.h"


//int now_sensor_x,now_sensor_y;

unsigned char g_PadBit[6];

extern int g_mirror; //davex
extern int screen_rotated_on; //davex


#define UPPER_THRESHOLD  0xcf
#define LOWER_THRESHOLD  0x2f

KeyState stKeyState = {FALSE, FALSE, EMU_SPEED_NORMAL};

void InputUpdateRapid(void)
{
	static uint32 ulCounter = 0;
	boolean bPush = FALSE;
	ulCounter++;
	switch(setting.rapidmode) {
	case RAPID_30COUNT:  // 30count/sec
		if (ulCounter%2) bPush = TRUE;
	break;
	case RAPID_20COUNT:  // 20count/sec
		if (ulCounter%3>=1) bPush = TRUE;
	break;
	case RAPID_15COUNT:  // 15count/sec
		if (ulCounter%4>=2) bPush = TRUE;
	break;
	case RAPID_10COUNT:  // 10count/sec
		if (ulCounter%6>=3) bPush = TRUE;
	break;
	}
	if (bPush) {
		if (stKeyState.bRapidA) g_PadBit[0] |= NES_A;
		if (stKeyState.bRapidB) g_PadBit[0] |= NES_B;
	}
	else {
		if (stKeyState.bRapidA) g_PadBit[0] &= ~NES_A;
		if (stKeyState.bRapidB) g_PadBit[0] &= ~NES_B;
	}
}


void InputUpdatePad(void)
{
	SceCtrlData paddata;
	static boolean bPushFlag = FALSE;
	static boolean bTogglePushFlag = FALSE;

	g_PadBit[0]=0;
	stKeyState.bRapidA = FALSE;
	stKeyState.bRapidB = FALSE;
	sceCtrlPeekBufferPositive(&paddata, 1);

	// Analog pad state
	if (paddata.Ly > UPPER_THRESHOLD) {
		paddata.Buttons |= PSP_CTRL_A_DOWN;	// DOWN(analog)
	}
	else if (paddata.Ly < LOWER_THRESHOLD) {
		paddata.Buttons |= PSP_CTRL_A_UP;	// UP(analog)
	}
	if (paddata.Lx < LOWER_THRESHOLD) {
		paddata.Buttons |= PSP_CTRL_A_LEFT; // LEFT(analog)
	}
	else if (paddata.Lx > UPPER_THRESHOLD) {
		paddata.Buttons |= PSP_CTRL_A_RIGHT; // RIGHT(analog)
	}

    // Hold
    if (paddata.Buttons & PSP_CTRL_HOLD) {
		paddata.Buttons = 0;
	}

	// for Enter Menu
	now_pad = paddata.Buttons;

	// ↑↓←→
	if((paddata.Buttons&setting.key_config[0])==setting.key_config[0] && setting.key_config[0])
		g_PadBit[0] |= NES_UP;
	if((paddata.Buttons&setting.key_config[1])==setting.key_config[1] && setting.key_config[1])
		g_PadBit[0] |= NES_DOWN;
	if((paddata.Buttons&setting.key_config[2])==setting.key_config[2] && setting.key_config[2])
		g_PadBit[0] |= NES_LEFT;
	if((paddata.Buttons&setting.key_config[3])==setting.key_config[3] && setting.key_config[3])
		g_PadBit[0] |= NES_RIGHT;

	// A、B AB連射
	if((paddata.Buttons&setting.key_config[6])==setting.key_config[6] && setting.key_config[6])
		stKeyState.bRapidA = TRUE;
	else if((paddata.Buttons&setting.key_config[4])==setting.key_config[4] && setting.key_config[4])
		g_PadBit[0] |= NES_A;
	if((paddata.Buttons&setting.key_config[7])==setting.key_config[7] && setting.key_config[7])
		stKeyState.bRapidB = TRUE;
	else if((paddata.Buttons&setting.key_config[5])==setting.key_config[5] && setting.key_config[5])
		g_PadBit[0] |= NES_B;
	if((paddata.Buttons&setting.key_config[8])==setting.key_config[8] && setting.key_config[8])
		g_PadBit[0] |= NES_SELECT;
	if((paddata.Buttons&setting.key_config[9])==setting.key_config[9] && setting.key_config[9])
		g_PadBit[0] |= NES_START;

	g_PadBit[4]=0;
	// PLAYER2 MIC
	if((paddata.Buttons&setting.key_config[11])==setting.key_config[11] && setting.key_config[11])
		g_PadBit[4] |= 0x04;

	//>>>davex
	if( screen_rotated_on){
		if(paddata.Buttons & PSP_CTRL_DOWN)  g_PadBit[0] |= NES_UP;
		if(paddata.Buttons & PSP_CTRL_UP)    g_PadBit[0] |= NES_DOWN;
		if(paddata.Buttons & PSP_CTRL_LEFT)  g_PadBit[0] |= NES_RIGHT;
		if(paddata.Buttons & PSP_CTRL_RIGHT) g_PadBit[0] |= NES_LEFT;

	}else{

		// cross
		if(paddata.Buttons & PSP_CTRL_DOWN)  g_PadBit[0] |= NES_DOWN;
		if(paddata.Buttons & PSP_CTRL_UP)    g_PadBit[0] |= NES_UP;
		if( g_mirror){
			if(paddata.Buttons & PSP_CTRL_LEFT)  g_PadBit[0] |= NES_RIGHT;
			if(paddata.Buttons & PSP_CTRL_RIGHT) g_PadBit[0] |= NES_LEFT;
		}else{
			if(paddata.Buttons & PSP_CTRL_LEFT)  g_PadBit[0] |= NES_LEFT;
			if(paddata.Buttons & PSP_CTRL_RIGHT) g_PadBit[0] |= NES_RIGHT;
		}
	}//<<<

	// Toggle モードじゃないときの処理
	if(!setting.bToggle) {
		// Speed mode
		if(setting.key_config[12] && (paddata.Buttons&setting.key_config[12])==setting.key_config[12]){
			stKeyState.nSpeedMode = EMU_SPEED_MODE1;
		}else if(setting.key_config[13] && (paddata.Buttons&setting.key_config[13])==setting.key_config[13]){
			stKeyState.nSpeedMode = EMU_SPEED_MODE2;
		}
		else {
			stKeyState.nSpeedMode = EMU_SPEED_NORMAL;
		}
	}
	else {
		// Speed mode 1(Toggle)
		if(setting.bToggle && setting.key_config[12] && (paddata.Buttons&setting.key_config[12])==setting.key_config[12]){
			if (!bTogglePushFlag) {
				if( stKeyState.nSpeedMode == EMU_SPEED_MODE1 ){
					char szTemp[128];
					stKeyState.nSpeedMode = EMU_SPEED_NORMAL;
					_strcpy(szTemp, "SPEED MODE: NORMAL");
					Scr_SetMessage(szTemp, 120, 0xFFFF);
				}else{
					char msg[256], szTemp[128];
					stKeyState.nSpeedMode = EMU_SPEED_MODE1;
					_strcpy(szTemp, "SPEED MODE: MODE 1 (");
					_itoa(setting.speedmode1, msg);
					_strcat(szTemp, msg);
					_strcat(szTemp, "fps)");
					Scr_SetMessage(szTemp, 120, 0xFFFF);
				}
				bTogglePushFlag = TRUE;
			}
			// キーをクリアすると押したとき動きが止まるのでクリアしない
			//g_PadBit[0]=0;
		}
		// Speed mode 2(Toggle)
		else if(setting.bToggle && setting.key_config[13] && (paddata.Buttons&setting.key_config[13])==setting.key_config[13]){
			if (!bTogglePushFlag) {
				if( stKeyState.nSpeedMode == EMU_SPEED_MODE2 ){
					char szTemp[128];
					stKeyState.nSpeedMode = EMU_SPEED_NORMAL;
					_strcpy(szTemp, "SPEED MODE: NORMAL");
					Scr_SetMessage(szTemp, 120, 0xFFFF);
				}else{
					char msg[256], szTemp[128];
					stKeyState.nSpeedMode = EMU_SPEED_MODE2;
					_strcpy(szTemp, "SPEED MODE: MODE 2 (");
					_itoa(setting.speedmode2, msg);
					_strcat(szTemp, msg);
					_strcat(szTemp, "fps)");
					Scr_SetMessage(szTemp, 120, 0xFFFF);
				}
				bTogglePushFlag = TRUE;
			}
			// キーをクリアすると押したとき動きが止まるのでクリアしない
			//g_PadBit[0]=0;
		}
		else {
			bTogglePushFlag = FALSE;
		}
	}

	if (1) {
		// Sound on/off
		if(setting.key_config[14] && (paddata.Buttons&setting.key_config[14])==setting.key_config[14]){
			if (!bPushFlag) {
				g_NESConfig.sound.enabled = g_NESConfig.sound.enabled ? 0: 1;
				Scr_SetMessage(g_NESConfig.sound.enabled ? "SOUND ENABLE": "SOUND DISABLE", 120, 0xFFFF);
				PSPEMU_ApplySoundConfig();
				wavout_enable=g_NESConfig.sound.enabled;
				bPushFlag = TRUE;
			}
			g_PadBit[0]=0;
		}
		// Screen size change
		else if(setting.key_config[15] && (paddata.Buttons&setting.key_config[15])==setting.key_config[15]){
			if (!bPushFlag) {
				setting.screenmode = (setting.screenmode + 1) % SCREEN_COUNT;
				char szTemp[128];
				_strcpy(szTemp, "SCREEN SIZE:");
				_strcat(szTemp, aszScreenName[setting.screenmode]);
				// 画面の掃除
				pgFillvram(0); pgScreenFlip();
				pgFillvram(0); pgScreenFlip();
				pgFillvram(0); pgScreenFlip();
				Scr_SetMessage(szTemp, 120, 0xFFFF);
				bPushFlag = TRUE;
			}
			g_PadBit[0]=0;
		}
		// Quick save
		else if(setting.key_config[16] && (paddata.Buttons&setting.key_config[16])==setting.key_config[16]){
			if (!bPushFlag) {
				PSPEMU_Freeze();
				if(PSPEMU_SaveState(StateSlot)) {
					char szTemp[128];
					_strcpy(szTemp, "State save at x");
					szTemp[sizeof("State save at x") - 2] = StateSlot+'0';
					Scr_SetMessage(szTemp, 120, 0xFFFF);
				}
				else
					Scr_SetMessage("State save Failed", 120, RGB(255, 0, 0));
				bPushFlag = TRUE;
				PSPEMU_Thaw();
			}
			g_PadBit[0]=0;
		}
		// Quick load
		else if(setting.key_config[17] && (paddata.Buttons&setting.key_config[17])==setting.key_config[17]){
			if (!bPushFlag) {
				if(PSPEMU_LoadState(StateSlot)) {
					char szTemp[128];
					_strcpy(szTemp, "State load at x");
					szTemp[sizeof("State load at x") - 2] = StateSlot+'0';
					Scr_SetMessage(szTemp, 120, 0xFFFF);
				}
				else
					Scr_SetMessage("State load Failed", 120, RGB(255, 0, 0));
				bPushFlag = TRUE;
			}
			g_PadBit[0]=0;
		}
		// Quick Slot change
		else if(setting.key_config[18] && (paddata.Buttons&setting.key_config[18])==setting.key_config[18]){
			if (!bPushFlag) {
				char szTemp[128];
				StateSlot = (StateSlot + 1) % STATE_SLOT_MAX;
				_strcpy(szTemp,"QUICK SLOT : x");
				szTemp[sizeof("QUICK SLOT : x") -2] = StateSlot+'0';
				Scr_SetMessage(szTemp, 120, 0xFFFF);
				bPushFlag = TRUE;
			}
			g_PadBit[0]=0;
		}
		//Battery Display by Smiths
		else if(setting.key_config[19] && (paddata.Buttons&setting.key_config[19])==setting.key_config[19]){
			if (!bPushFlag) {
				char szTemp[128];
				int color;
				GetBatteryInfoString(szTemp, &color);
				Scr_SetMessage(szTemp, 120, 0xFFFF);
				bPushFlag = TRUE;
			}
			// キーをクリアすると押したとき動きが止まるのでクリアしない
			// メッセージしか表示しないしねぇ
			// g_PadBit[0]=0;
		}
		// Screen shot
		else if(setting.key_config[20] && (paddata.Buttons&setting.key_config[20])==setting.key_config[20]){
			if (!bPushFlag) {
				PSPEMU_Freeze();
				if(PSPEMU_SaveScreenShot()) {
					Scr_SetMessage("Save the Screen Shot Successfully", 120, 0xFFFF);
				}
				else
					Scr_SetMessage("Save the Screen Shot Failed", 120, RGB(255, 0, 0));
				bPushFlag = TRUE;
				PSPEMU_Thaw();
			}
			g_PadBit[0]=0;
		}
		else {
			bPushFlag = FALSE;
		}
	}

//	now_sensor_x=2047-paddata.analog[PSP_CTRL_ANALOG_X]+127;
//	now_sensor_y=2047-paddata.analog[PSP_CTRL_ANALOG_Y]+127;
}
