#include <pspctrl.h>
#include <psppower.h>
#include "main.h"
#include "emu_main.h"
#include "nes/nes.h"
#include "menu_submenu.h"
#include "nes/nes_config.h"
#include "fileio.h"
#include "inputmanager.h"
#include "screenmanager.h"
#include "image.h"
#include "colbl.c"
#include "rewind.h"


extern u32 new_pad;
extern char cnRomPath[MAX_PATH];

int StateSlot=0;

//>>> davex
extern int rewind_memory_ready;
extern int rom_reload;
extern int f60count;
//<<<davex


// by kwn
void save_config(void)
{
	char CfgPath[MAX_PATH];

	GetModulePath(CfgPath, sizeof(CfgPath));
	_strcat(CfgPath, "NESTERJ.CFG");

	int fd;
	fd = sceIoOpen(CfgPath,PSP_O_CREAT | PSP_O_WRONLY | PSP_O_TRUNC, 0777);
	if(fd>=0){
		sceIoWrite(fd, &setting, sizeof(setting));
		sceIoWrite(fd, &g_NESConfig, sizeof(g_NESConfig));
		sceIoClose(fd);
	}
}

// by kwn
void load_config(void)
{
	char CfgPath[MAX_PATH];

	_memset(&setting, 0, sizeof(setting));

	GetModulePath(CfgPath, sizeof(CfgPath));
	_strcat(CfgPath, "NESTERJ.CFG");

	int fd;
	fd = sceIoOpen(CfgPath,PSP_O_RDONLY, 0777);
	if(fd>=0){
		uint32 unSizeOfSetting, unSizeOfNESConfig;
		unSizeOfSetting = sceIoRead(fd, &setting, sizeof(setting));
		unSizeOfNESConfig = sceIoRead(fd, &g_NESConfig, sizeof(g_NESConfig));
		sceIoClose(fd);
		// 簡易エラーチェック(setting)
		if (unSizeOfSetting != sizeof(setting)) goto SetDefault;
		if(_strcmp(setting.vercnf, NESTERJ_CFG_TAG)) goto SetDefault;
		if(setting.version != NESTERJ_CFG_FORMATVERSION) goto SetDefault;
		if(setting.bgbright<0 || setting.bgbright>100) goto SetDefault;
		if(setting.speedmode1 < SPEEDMODE_MIN || setting.speedmode1 > SPEEDMODE_MAX) goto SetDefault;
		if(setting.speedmode2 < SPEEDMODE_MIN || setting.speedmode2 > SPEEDMODE_MAX) goto SetDefault;
		if(setting.screenmode >= SCREEN_COUNT) goto SetDefault;
		if(setting.samplingrate >= SAMPLINGRATE_COUNT) goto SetDefault;
		if(setting.rapidmode >= RAPID_COUNT) goto SetDefault;
		// 問題なし
		if(setting.key_config[10]==0) setting.key_config[10] = PSP_CTRL_A_LEFT;
		//>>>davex
		if( setting.Reserved[0] > 360 || (setting.Reserved[0] % 5) != 0 ||
			setting.Reserved[0] <= 0  ) setting.Reserved[0] = 15;

		if( setting.Reserved[1] > 5000 || (setting.Reserved[1] % 5) != 0 ||
			setting.Reserved[1] <= 0   ) setting.Reserved[1] = 50;
		//<<<davex
		// 簡易エラーチェック(g_NESConfig)
		if (unSizeOfNESConfig != sizeof(g_NESConfig)) goto SetDefault;
		return;
	}
SetDefault:
	MessageBox("Load config FAILED. Setting to the default values...", RGB(255,0,0), MB_OK);

	_memset(&setting, 0, sizeof(setting));
	_strcpy(setting.vercnf, NESTERJ_CFG_TAG);
	setting.version = NESTERJ_CFG_FORMATVERSION;
	setting.screenmode = 3; //WIDE x1.5
	setting.vsync = VSYNC_ALWAYS;
	setting.showfps = TRUE;
	setting.savethumbnail = TRUE;
	setkeydefault();
	setting.color[0] = DEF_COLOR0;
	setting.color[1] = DEF_COLOR1;
	setting.color[2] = DEF_COLOR2;
	setting.color[3] = DEF_COLOR3;
	setting.bgbright=BGBRIGHT_DEFAULT;
	setting.bToggle    = FALSE;
	setting.speedmode1 = SPEEDMODE1_DEFAULT;
	setting.speedmode2 = SPEEDMODE2_DEFAULT;
	setting.batterypowersave = TRUE;
	setting.rapidmode = RAPID_15COUNT;
	setting.samplingrate = SAMPLINGRATE_22050;
	setting.fatread = READFILE_SCEIO;
	//>>>davex
	setting.Reserved[0] = 10;//davex: rewind save rate
	setting.Reserved[1] = 50;// davex: rewind replay delay
	//<<<
	NES_Config_SetDefaults_All();
}

void setkeydefault(void)
{
	int i;
	setting.key_config[0] = 0;				// UP
	setting.key_config[1] = 0;				// DOWN
	setting.key_config[2] = 0;				// LEFT
	setting.key_config[3] = 0;				// RIGHT
	setting.key_config[4] = PSP_CTRL_CIRCLE;	// A
	setting.key_config[5] = PSP_CTRL_CROSS;		// B
	setting.key_config[6] = PSP_CTRL_TRIANGLE;	// A(RAPID)
	setting.key_config[7] = PSP_CTRL_SQUARE;	// B(RAPID)
	setting.key_config[8] = PSP_CTRL_SELECT;	// SELECT
	setting.key_config[9] = PSP_CTRL_START;		// START
	setting.key_config[10] = PSP_CTRL_A_LEFT;	// MENU
	setting.key_config[11] = 0;				// PLAYER2MIC
	setting.key_config[12] = PSP_CTRL_LTRIGGER;	// SPEEDMODE1
	setting.key_config[13] = 0; //PSP_CTRL_RTRIGGER;	// SPEEDMODE2
	setting.key_config[14] = 0;				// SOUND
	setting.key_config[15] = PSP_CTRL_A_RIGHT;	// SCREENSIZE
	setting.key_config[16] = PSP_CTRL_RTRIGGER|PSP_CTRL_SELECT;	// QUICKSAVE
	setting.key_config[17] = PSP_CTRL_RTRIGGER|PSP_CTRL_START;	// QUICKLOAD
	setting.key_config[18] = 0;				// SAVE_SLOT
	setting.key_config[19] = PSP_CTRL_A_DOWN;	// BATTERY_METER
	setting.key_config[20] = 0;					// SCREENSHOT
	setting.key_config[21] = PSP_CTRL_RTRIGGER;	// REWIND KEY (davex)
	for(i=22; i<25; i++)
		setting.key_config[i] = 0;
}

boolean g_bEnableBitmap;
unsigned short g_bgBitmap[480*272];

// by kwn
void load_menu_bg()
{
	byte *menu_bg;
	unsigned char *vptr;
	static byte menu_bg_buf[480*272*3+0x36];
	char BgPath[MAX_PATH];
 	unsigned short x,y,yy,r,g,b,data;

	// PNG 対応
	{
		GetModulePath(BgPath, sizeof(BgPath));
		_strcat(BgPath, "MENU.png");
		if(read_png(BgPath,g_bgBitmap,sizeof(g_bgBitmap))) {
			g_bEnableBitmap = TRUE;
			return;
		}
	}

	GetModulePath(BgPath, sizeof(BgPath));
	_strcat(BgPath, "MENU.BMP");

	int fd;
	fd = sceIoOpen(BgPath,PSP_O_RDONLY,0777);
	if(fd>=0){
		sceIoRead(fd, menu_bg_buf, 480*272*3+0x36);
		sceIoClose(fd);

		menu_bg = menu_bg_buf + 0x36;
		vptr=(unsigned char*)g_bgBitmap;
		for(y=0; y<272; y++){
			for(x=0; x<480; x++){
				yy = 271 - y;
				r = *(menu_bg + (yy*480 + x)*3 + 2);
				g = *(menu_bg + (yy*480 + x)*3 + 1);
				b = *(menu_bg + (yy*480 + x)*3);
				data = (((b & 0xf8) << 7) | ((g & 0xf8) << 2) | (r >> 3));
				*(unsigned short *)vptr=data;
				vptr+=2;
			}
		}
		g_bEnableBitmap = TRUE;
	}else{
		g_bEnableBitmap = FALSE;
	}
}

// 半透明処理
unsigned short rgbTransp(unsigned short fgRGB, unsigned short bgRGB, int alpha) {

    unsigned short fgR, fgG, fgB;
    unsigned short bgR, bgG, bgB;
	unsigned short R, G, B;
 	unsigned short rgb;

    fgB = (fgRGB >> 10) & 0x1F;
    fgG = (fgRGB >> 5) & 0x1F;
    fgR = fgRGB & 0x1F;

    bgB = (bgRGB >> 10) & 0x1F;
    bgG = (bgRGB >> 5) & 0x1F;
    bgR = bgRGB & 0x1F;

	R = coltbl[fgR][bgR][alpha/10];
	G = coltbl[fgG][bgG][alpha/10];
	B = coltbl[fgB][bgB][alpha/10];

	rgb = (((B & 0x1F)<<10)+((G & 0x1F)<<5)+((R & 0x1F)<<0)+0x8000);
    return rgb;
}

void bgbright_change()
{
	unsigned short *vptr,rgb;
 	int i;

//	load_menu_bg();
	vptr=g_bgBitmap;
	for(i=0; i<272*480; i++){
			rgb = *vptr;
			*vptr = rgbTransp(rgb, 0x0000, setting.bgbright);
			vptr++;
	}
}

void GetBatteryInfoString(char *msg, int *color)
{
	char tmp[32];
	int ret;
	if ((ret = scePowerGetBatteryLifePercent()) >= 0) {
		_strcpy(msg,"Battery[");
		_itoa(ret,tmp);
		_strcat(msg,tmp);
		_strcat(msg,"%");
		if (ret <= 15)
			*color = RGB(245,40,40);
		else if (ret <= 30)
			*color = RGB(245,245,40);
		else
			*color = RGB(105,105,135);
		if(!scePowerIsPowerOnline()) {
			if((ret=scePowerGetBatteryLifeTime()) >= 0) {
				_strcat(msg,"(");
				_itoa(ret/60,tmp);
				_strcat(msg,tmp);
				_strcat(msg,":");
				_itoa(ret%60+100,tmp);
				_strcat(msg,tmp+1);
				_strcat(msg,")");
			}
		} else {
			_strcat(msg,"(AC)");
			*color = RGB(85,85,95);
		}
		_strcat(msg,"]");
	}
	else {
		*msg = '\0';
	}
}


void menu_frame(const char *msg0, const char *msg1)
{
	char msg[32];
	int color;
	if(g_bEnableBitmap)
		pgBitBlt(0,0,480,272,1,g_bgBitmap);
	else
		pgFillvram(setting.color[0]);
	//mh_print(314, 1, " ■ NesterJ for PSP Ver1.12 ■", setting.color[1]);
	//mh_print(310, 1, " ■ NesterJ for PSP Plus 0.5 ■", setting.color[1]);
	mh_print(250, 1, " ■ (unnoficial) NesterJ for PSP AoEX R3 SP ■", setting.color[1]);
	GetBatteryInfoString(msg, &color);
	mh_print(464-(_strlen(msg)*5), 254, msg, color);
	// メッセージなど
	if(msg0!=0) mh_print(17, 12, msg0, setting.color[2]);
	pgDrawFrame(17,23,463,250,setting.color[1]);
	pgDrawFrame(18,24,462,249,setting.color[1]);
	// 操作説明
	if(msg1!=0) mh_print(17, 254, msg1, setting.color[2]);
}

typedef enum {
	UNCHECKED,	// Unchecked
	AVAILABLE,	// Available
	NOT_EXIST	// not exist Thumbnail
}enumThumbnail;

struct SlotData {
	char szData[30];
	uint8 AvailableThumbnail;
	boolean bSlotExist;
	uint16 thumbnail[112*128];
} g_Slot[STATE_SLOT_MAX];

// state更新
void update_stateslot(int nUpdateSlotNo)
{
	char path[MAXPATH], name[MAXNAME], tmp[8];
	char thumbnailPath[MAXPATH];
	char zsPath[MAXPATH];
	int i,j,fd;
	int nStart = 0, nEnd = STATE_SLOT_MAX;

	GetStatePath(path, sizeof(path));
	_strcpy(name, NES_ROM_GetRomName());
	_strcpy(thumbnailPath, path);
	_strcat(thumbnailPath, name);
	_strcat(thumbnailPath, ".tn0");
	_strcpy(zsPath, name);
	_strcat(name,".ss0");
	_strcat(zsPath,".zs0");

	int nfiles = 0;
	fd = sceIoDopen(path);
	while(nfiles<MAX_ENTRY){
		if(sceIoDread(fd, &files[nfiles])<=0) break;
		nfiles++;
	}
	sceIoDclose(fd);

	if (nUpdateSlotNo != -1) {
		nStart = nUpdateSlotNo;
		nEnd = nUpdateSlotNo + 1;
	}
	for(i=nStart; i<nEnd; i++){
		_strcpy(g_Slot[i].szData,"0 - ");
		g_Slot[i].szData[0] = zsPath[_strlen(zsPath)-1] = name[_strlen(name)-1] = i + '0';
		for(j=0; j<nfiles; j++){
			if(!_stricmp(name,files[j].d_name) || !_stricmp(zsPath,files[j].d_name)){
				_itoa(files[j].d_stat.st_mtime.year,tmp);
				_strcat(g_Slot[i].szData,tmp);
				_strcat(g_Slot[i].szData,"/");

				if(files[j].d_stat.st_mtime.month < 10) _strcat(g_Slot[i].szData,"0");
				_itoa(files[j].d_stat.st_mtime.month,tmp);
				_strcat(g_Slot[i].szData,tmp);
				_strcat(g_Slot[i].szData,"/");

				if(files[j].d_stat.st_mtime.day < 10) _strcat(g_Slot[i].szData,"0");
				_itoa(files[j].d_stat.st_mtime.day,tmp);
				_strcat(g_Slot[i].szData,tmp);
				_strcat(g_Slot[i].szData," ");

				if(files[j].d_stat.st_mtime.hour < 10) _strcat(g_Slot[i].szData,"0");
				_itoa(files[j].d_stat.st_mtime.hour,tmp);
				_strcat(g_Slot[i].szData,tmp);
				_strcat(g_Slot[i].szData,":");

				if(files[j].d_stat.st_mtime.minute < 10) _strcat(g_Slot[i].szData,"0");
				_itoa(files[j].d_stat.st_mtime.minute,tmp);
				_strcat(g_Slot[i].szData,tmp);
				_strcat(g_Slot[i].szData,":");

				if(files[j].d_stat.st_mtime.second < 10) _strcat(g_Slot[i].szData,"0");
				_itoa(files[j].d_stat.st_mtime.second,tmp);
				_strcat(g_Slot[i].szData,tmp);
				g_Slot[i].bSlotExist = TRUE;
				g_Slot[i].AvailableThumbnail = UNCHECKED;
				break;
			}
		}
		if(j>=nfiles){
			g_Slot[i].bSlotExist = FALSE;
			g_Slot[i].AvailableThumbnail = NOT_EXIST;
			_strcat(g_Slot[i].szData, "Not exist");
		}
	}
}




// 最後に選択したファイルのsel位置
int nSelRomFiler = 0, nSelGenieFiler = 0;

boolean nesterj_menu(void)
{
	enum
	{
		SAVE_SLOT,
		STATE_SAVE,
		STATE_LOAD,
		STATE_DELETE,
		SCREEN_SHOT,
		DISK_CHANGE,
		PREFERENCES_CONFIG,
		GRAPHIC_CONFIG,
		SOUND_CONFIG,
		MENU_COLOR_CONFIG,
		KEY_CONFIG,
		REWIND_CONFIG,
		CHEAT_GAME_GENIE,
		CHEAT_NESTERJ,
		CHEAT_CONFIG,
		LOAD_ROM,
		RESET,
		CONTINUE,
		EXIT_NESTERJ,
	};
	char msg[256], szSlotNum[128], szMsg[96], szTmp[32];
	boolean bRet = TRUE;
	static int sel=0;
	int x, y, cbI, nNoKeyCount = 20;
	int nCursor = 0;
	uint8 disksidenum = PSPEMU_GetDiskSideNum();

	scePowerSetClockFrequency(200, 200, 100);
	msg[0]='\0';
	update_stateslot(-1);

	for(;;){
		readpad();
		if (nNoKeyCount) {nNoKeyCount--; new_pad = 0;}
		if(new_pad & PSP_CTRL_CIRCLE){
			if(sel == SAVE_SLOT){
				StateSlot++;
				if(StateSlot >= STATE_SLOT_MAX) StateSlot=0;
			}else if(sel == STATE_SAVE){
				_strcpy(szMsg, "Are you sure to SAVE slot -");
				_itoa(StateSlot, szTmp);
				_strcat(szTmp, " ?");
				_strcat(szMsg, szTmp);
				if (MessageBox(szMsg, RGB(255, 64, 64), MB_OKCANCEL) == IDOK) {
					if(PSPEMU_SaveState(StateSlot)) {
						_strcpy(msg, "State Saved Successfully");
						update_stateslot(StateSlot);
					}
					else {
						_strcpy(msg, "State Save Failed");
					}
				}
			}else if(sel == STATE_LOAD && g_Slot[StateSlot].bSlotExist){
				_strcpy(szMsg, "Are you sure to LOAD slot -");
				_itoa(StateSlot, szTmp);
				_strcat(szTmp, " ?");
				_strcat(szMsg, szTmp);
				if (MessageBox(szMsg, RGB(64, 64, 255), MB_OKCANCEL) == IDOK) {
					if(PSPEMU_LoadState(StateSlot))
						break;
					else
						_strcpy(msg, "State Load Failed");
				}
			}else if(sel == STATE_DELETE && g_Slot[StateSlot].bSlotExist){
				_strcpy(szMsg, "Are you sure to DELETE slot -");
				_itoa(StateSlot, szTmp);
				_strcat(szTmp, " ?");
				_strcat(szMsg, szTmp);
				if (MessageBox(szMsg, RGB(255, 64, 64), MB_OKCANCEL) == IDOK) {
					PSPEMU_DeleteState(StateSlot);
					update_stateslot(StateSlot);
				}
			}else if(sel == SCREEN_SHOT){
				if (MessageBox("Are you sure to save the SCREEN SHOT ?", RGB(64, 64, 255), MB_OKCANCEL) == IDOK) {
					if (PSPEMU_SaveScreenShot()) {
						_strcpy(msg, "Screen Shot saved successfully");
					}
					else {
						_strcpy(msg, "Failed to save Screen Shot!");
					}
				}
			}else if(sel == DISK_CHANGE && disksidenum){
				submenu_diskchange();
				msg[0]=0;
			}else if(sel == PREFERENCES_CONFIG){
				submenu_preferencesconfig();
				msg[0]=0;
			}else if(sel == GRAPHIC_CONFIG){
				submenu_graphicsconfig();
				msg[0]=0;
			}else if(sel == SOUND_CONFIG){
				submenu_soundconfig();
				msg[0]=0;
			}else if(sel == MENU_COLOR_CONFIG){
				submenu_menucolorconfig();
				msg[0]=0;
			}else if(sel == KEY_CONFIG){
				submenu_keyconfig();
				msg[0]=0;
			}else if( sel == REWIND_CONFIG){
				submenu_rewindconfig();
				msg[0]=0;
			}else if(sel == CHEAT_GAME_GENIE){
				int nRet;
				_strcpy(FilerMsg, "Select Game Genie file");
				if(setting.fatread)
					nRet = getFilePathGBK(szLastGeniePath, setting.szLastGeniePath, NULL, NULL, nSelGenieFiler);
				else
					nRet = getFilePath(szLastGeniePath, setting.szLastGeniePath, NULL, NULL, nSelGenieFiler);
				if (nRet >= 0){
					nSelGenieFiler = nRet;
					// apply game genie code
					if(setting.fatread)//cnRomPath
					{
						if (PSPEMU_ApplyGameGenie(cnRomPath)) {
							_strcpy(msg, "Apply Game Genie code(s) Successfully");
						}
						else {
							_strcpy(msg, "Apply Game Genie code(s) Failed");
						}
					}
					else
					{
						if (PSPEMU_ApplyGameGenie(szLastGeniePath)) {
							_strcpy(msg, "Apply Game Genie code(s) Successfully");
						}
						else {
							_strcpy(msg, "Apply Game Genie code(s) Failed");
						}
					}
				}
			}else if(sel == CHEAT_NESTERJ){
				int nRet;
				_strcpy(FilerMsg, "Select Cheat file");
				if(setting.fatread)
					nRet = getFilePathGBK(szLastGeniePath, setting.szLastGeniePath, (LPEXTENTIONS)&stExtCheat, NULL, nSelGenieFiler);
				else
					nRet = getFilePath(szLastGeniePath, setting.szLastGeniePath, (LPEXTENTIONS)&stExtCheat, NULL, nSelGenieFiler);
				if (nRet >= 0){
					nSelGenieFiler = nRet;
				;	// apply cheat code
					if(setting.fatread)
					{
						if (PSPEMU_ApplyGameCheat(cnRomPath)){
							submenu_configcheat();
							_strcpy(msg, "Apply Cheat code(s) Successfully");
							//submenu_configcheat();
						}
						else {
							_strcpy(msg, "Apply Cheat code(s) Failed");
						}
					}else
					{
						if (PSPEMU_ApplyGameCheat(szLastGeniePath)){
							submenu_configcheat();
							_strcpy(msg, "Apply Cheat code(s) Successfully");
						}
						else {
							_strcpy(msg, "Apply Cheat code(s) Failed");
						}
					}
				}
			}else if(CHEAT_CONFIG==sel)
			{
				submenu_configcheat();
			}
			else if(sel == LOAD_ROM){
				int nRet;
				FilerMsg[0]=0;
				if(setting.fatread)
					nRet = getFilePathGBK(RomPath, setting.szLastPath, (LPEXTENTIONS)&stExtRom, setting.szLastFile, nSelRomFiler);
				else
					nRet = getFilePath(RomPath, setting.szLastPath, (LPEXTENTIONS)&stExtRom, setting.szLastFile, nSelRomFiler);
				if (nRet >= 0){
					nSelRomFiler = nRet;
					//>>>davex
					free_rewind_states();
					rewind_memory_ready = 0;
					rom_reload = 1;
					f60count = 0;
					//<<<
					// ロム読み込み
					if (NES_loadROM(RomPath)) {
						break;
					}else
					MessageBox("Unsupported board. Plz mail to: tensai_wang@msn.com",RGB(255,0,0), 1);
				}
			}else if(sel == RESET){
				if (MessageBox("Are you sure to RESET game ?", RGB(255, 0, 0), MB_OKCANCEL) == IDOK) {
					// リセット
					NES_reset(0);
					break;
				}
			}else if(sel == CONTINUE){
				break;
			}else if(sel == EXIT_NESTERJ){
				if (MessageBox("Are you sure to EXIT to PSP menu ?", RGB(255, 0, 0), MB_OKCANCEL) == IDOK) {
					bRet = FALSE;
					break;
				}
			}
		}else if(new_pad & PSP_CTRL_CROSS){
			break;
		}else if(new_pad & PSP_CTRL_AD_LEFT){
			if (sel >= SAVE_SLOT && sel <= STATE_DELETE) {
				StateSlot = (StateSlot) ?  StateSlot-1: STATE_SLOT_MAX-1;
			}
		}else if(new_pad & PSP_CTRL_AD_RIGHT){
			if (sel >= SAVE_SLOT && sel <= STATE_DELETE) {
				StateSlot++;
				if (StateSlot >= STATE_SLOT_MAX) StateSlot = 0;
			}

		}else if(setting.key_config[10] && (new_pad&setting.key_config[10])==setting.key_config[10]){
			break;
		}else if(new_pad & PSP_CTRL_AD_UP){
			if(sel!=0)
				sel--;
			else
				sel=EXIT_NESTERJ;
		}else if(new_pad & PSP_CTRL_AD_DOWN){
			if(sel!=EXIT_NESTERJ)	sel++;
			else			sel=0;
		}

		menu_frame(msg, "○：OK　×：CANCEL");

		if (sel == SCREEN_SHOT) {
#define POS_X 206
#define POS_Y 25
			pgDrawFrame(POS_X-2,POS_Y-1,POS_X-1,POS_Y+224,setting.color[1]);
			DrawCurrentScreen(POS_X,POS_Y);
#undef POS_X
#undef POS_Y
			pgPrint(43, 30,setting.color[2], "Current Screen");
		}
		else {
			pgPrint(30, 4,setting.color[2], "State Save List");
			for (cbI = 0; cbI < STATE_SLOT_MAX; cbI++) {
				int nColorIndex = (StateSlot == cbI) ? 3: 2;
				pgPrint(33, cbI + 5,setting.color[nColorIndex], g_Slot[cbI].szData);
			}

			if (g_Slot[StateSlot].AvailableThumbnail == UNCHECKED) {
				// thumbnail呼び出し
				char thumbnailPath[MAXPATH];
				GetStatePath(thumbnailPath, sizeof(thumbnailPath));
				_strcat(thumbnailPath, NES_ROM_GetRomName());
				_strcat(thumbnailPath, ".tn0");
				thumbnailPath[_strlen(thumbnailPath)-1] = StateSlot + '0';
				_strcat(thumbnailPath, ".png");
				if (LoadThumnailFile(thumbnailPath, g_Slot[StateSlot].thumbnail)) {
					g_Slot[StateSlot].AvailableThumbnail = AVAILABLE;
				}
				else {
					g_Slot[StateSlot].AvailableThumbnail = NOT_EXIST;
				}
			}

			if (g_Slot[StateSlot].AvailableThumbnail == AVAILABLE) {
#define POS_X 330
#define POS_Y 130
				pgDrawFrame(POS_X-1,POS_Y-1,POS_X+128,POS_Y+112,setting.color[1]);
				pgBitBltAlpha75(POS_X,POS_Y,128,112,(uint16*)g_Slot[StateSlot].thumbnail);
			}
		}

#define MENU_X 2
#define MENU_Y 4
		x = MENU_X;
		y = MENU_Y;

		_itoa(StateSlot, szTmp);
		_strcpy(szSlotNum, "  SAVE SLOT  : ");
		_strcat(szSlotNum, szTmp);
		pgPrint(x,y++,setting.color[3], szSlotNum);
		pgPrint(x,y++,setting.color[3],"  STATE SAVE");
		pgPrint(x,y++,setting.color[3],"  STATE LOAD");
		pgPrint(x,y++,setting.color[3],"  STATE DELETE");
		y++;
		pgPrint(x,y++,setting.color[3],"  SCREEN SHOT");
		y++;
		pgPrint(x,y++,disksidenum ? setting.color[3]: setting.color[2],"  DISK CHANGE");
		y++;
		pgPrint(x,y++,setting.color[3],"  PREFERENCES CONFIG");
		pgPrint(x,y++,setting.color[3],"  GRAPHICS CONFIG");
		pgPrint(x,y++,setting.color[3],"  SOUND CONFIG");
		pgPrint(x,y++,setting.color[3],"  MENU COLOR CONFIG");
		pgPrint(x,y++,setting.color[3],"  KEY CONFIG");
		pgPrint(x,y++,setting.color[3],"  REWIND CONFIG");
		y++;
		pgPrint(x,y++,setting.color[3],"  LOAD GAME GENIE FILE");
		pgPrint(x,y++,setting.color[3],"  LOAD CHEAT FILE");
		pgPrint(x,y++,NES_GetCheatCodeNum() ? setting.color[3]:setting.color[2],"  CONFIG CHEAT");

		y++;
		pgPrint(x,y++,setting.color[3],"  Back to ROM list");
//		only Kernel mode
//		pgPrint(x,y++,setting.color[3],"  Wifi Network Play(Adhoc)");
//		pgPrint(x,y++,setting.color[2],"  Wifi Network Play(Adhoc)");
        y++;
		pgPrint(x,y++,setting.color[3],"  Reset");
		pgPrint(x,y++,setting.color[3],"  Continue");
		y++;
		pgPrint(x,y++,setting.color[3],"  Exit to PSP Menu");

		y = sel + MENU_Y;

		if (sel >= SCREEN_SHOT) y++;
		if (sel >= DISK_CHANGE) y++;
		if (sel >= PREFERENCES_CONFIG) y++;
		if (sel >= CHEAT_GAME_GENIE) y++;
		if (sel >= LOAD_ROM) y++;
		if (sel >= RESET) y++;
		if (sel >= EXIT_NESTERJ) y++;

		if (nCursor/5) pgPutChar((x+1)*8,y*8,setting.color[3],0,127,1,0,1);
		nCursor = (nCursor + 1 ) %10;
        sceKernelDelayThread(1);
		pgScreenFlipV();
	}

	pgFillvram(0);
	pgScreenFlipV();
	pgFillvram(0);
	pgScreenFlipV();
	pgFillvram(0);
	pgScreenFlipV();
	pgWaitVn(10);
//	_memset(&paddata, 0x00, sizeof(paddata));
	scePowerSetClockFrequency(setting.cpufrequency, setting.cpufrequency, setting.cpufrequency/2);

	extern int bSleep;
	bSleep = 0;
	return bRet;
}


