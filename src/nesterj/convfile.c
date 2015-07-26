#include <pspkernel.h>
#include <pspctrl.h>
#include <psppower.h>
#include <pspdebug.h>
#include <stdio.h>
#include "main.h"
#include "emu_main.h"
#include "filer.h"
#include "menu_submenu.h"
#include "screenmanager.h"
#include "inputmanager.h"
#include "nes/nes.h"
#include "nes/nes_config.h"
#include "image.h"
#include "ulunzip.h"

uint32 LoadFile(const char *pszFile, void *pBuf, uint32 uLoadSize)
{
	uint32 uReadSize = 0;
	int fd;
	fd=sceIoOpen(pszFile, PSP_O_RDONLY, 0777);
	if (fd >= 0) {
		uReadSize = sceIoRead(fd, pBuf, uLoadSize);
		sceIoClose(fd);
	}
	return uReadSize;
}

// 1.10までのファイル形式を1.11以降に変換します。
// thumbnail Raw -> PNG
// STATE, SAVE データはフォーマットがかなり違うので断念
void ConvertThumbnailFile(void)
{
	char path[MAXPATH], szSrcPath[MAXPATH], szDestFile[MAXPATH], szMsg[128];
	GetStatePath(path, sizeof(path));
	int cbI, nFiles = 0, fd;
	extern uint8 g_ReadRomImage[];// 間借り
	uint8 *pBuf = g_ReadRomImage;
	char *pszFile, *pszExt;
	uint32 nStateNum, FailedCount = 0;

	// tn?ファイルのみ検索する dummyに?部格納
	fd = sceIoDopen(path);
	while(nFiles<MAX_ENTRY){
		if(sceIoDread(fd, &files[nFiles])<=0) break;
		pszExt = _strrchr(files[nFiles].d_name, '.');
		if (!pszExt) continue;
		files[nFiles].dummy = pszExt[3] - '0';
		if ((pszExt[1] == 't' || pszExt[1] == 'T') &&
			(pszExt[2] == 'n' || pszExt[2] == 'N') &&
			files[nFiles].dummy <= 9) {
			// Thumbnailファイルなので覚えとく
			nFiles++;
		}
	}
	sceIoDclose(fd);

	if (!nFiles) {
		MessageBox("Old thumbnail file is not found.", 0xFFFF, MB_OK);
		return;
	}

	for (cbI = 0; cbI < nFiles; cbI++) {
		menu_frame(0, "");
		sprintf(szMsg, "Now Converting ...(%d/%d)", cbI+1, nFiles);
		pgPrint(17, 8,setting.color[3],szMsg);
		pgScreenFlipV();
		readpad();
		if (new_pad & (PSP_CTRL_CIRCLE | PSP_CTRL_CROSS | PSP_CTRL_TRIANGLE | PSP_CTRL_SQUARE)) {
			if (MessageBox("Abort ?", RGB(255, 0, 0), MB_YESNO) == IDYES) {
				return;
			}
		}
		pszFile = files[cbI].d_name;
		nStateNum = files[cbI].dummy;
		sprintf(szSrcPath, "%s%s", path, pszFile);
		if (LoadFile(szSrcPath, pBuf, 1024*1024) != sizeof(uint16)*128*112) {
			// Thumbnail was not exist
			continue;
		}
		menu_frame(0, "");
		pgPrint(17, 8,setting.color[3],szMsg);
#define POS_X 480/2-64
#define POS_Y 110
		pgDrawFrame(POS_X-1,POS_Y-1,POS_X+128,POS_Y+112,setting.color[1]);
		pgBitBlt(POS_X,POS_Y,128,112,1,(uint16*)pBuf);
		pgScreenFlipV();
		readpad();
		if (new_pad & (PSP_CTRL_CIRCLE | PSP_CTRL_CROSS | PSP_CTRL_TRIANGLE | PSP_CTRL_SQUARE)) {
			if (MessageBox("Abort ?", RGB(255, 0, 0), MB_YESNO) == IDYES) {
				return;
			}
		}
		// PNGパス
		_strcpy(szDestFile, szSrcPath);
		_strcat(szDestFile, ".png");
		{
			uint8 thumbnail[112][128][3]; // localで…
			int x,y;
			byte r, g, b;
			uint16 *pColor = (uint16 *)pBuf;
			for (y = 0; y < 112; y++) {
				for (x = 0; x < 128; x++) {
					r = (*pColor      ) & 0x1F;
					g = (*pColor >>  5) & 0x1F;
					b = (*pColor >> 10) & 0x1F;
					thumbnail[y][x][0] = r * 0xFF / 0x1F;
					thumbnail[y][x][1] = g * 0xFF / 0x1F;
					thumbnail[y][x][2] = b * 0xFF / 0x1F;
					pColor++;
				}
			}
			if (!write_png(szDestFile, (byte *)thumbnail, 128, 112)) {
				FailedCount++;
				continue;
			}
		}
		// Successfully!! delete old file
		sceIoRemove(szSrcPath);
		pgWaitVn(10);
	}
	MessageBox("Convert completed", 0xFFFF, MB_OK);
}

