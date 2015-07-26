/*
	load rom image by ruka
*/
#include <pspkernel.h>
#include "nes/types.h"
#include "filer.h"
#include "menu.h"
#include "menu_submenu.h"
#include "main.h"
#include "syscall.h"
#include "pg.h"
#include "lib/zlibInterface.h"

#define SHOW_ERRMSG(bgcolor,msg,color,vcnt) \
	{int nCount=vcnt; \
	while (nCount--) { \
		pgFillvram(bgcolor); \
		mh_print(0,0,msg,color); \
		pgScreenFlipV(); \
	} \
}

extern char cnRomPath[MAX_PATH];

// progress
void draw_load_rom_progress(unsigned long ulTotal, unsigned long ulCurrent)
{
	int nPer = 100 * ulTotal / ulCurrent;
//	static int nOldPer = 0;
//	if (nOldPer == (nPer & 0xFFFFFFFE)) {
//		return ;
//	}
//	nOldPer = nPer;
	if(g_bEnableBitmap)
		pgBitBlt(0,0,480,272,1,g_bgBitmap);
	else
		pgFillvram(setting.color[0]);
	// プログレス
	pgDrawFrame(89,121,391,141,setting.color[1]);
	pgFillBox(90,123, 90+nPer*3, 139,setting.color[1]);
	// ％
	char szPer[16];
	_itoa(nPer, szPer);
	_strcat(szPer, "%");
	pgPrint(28,16,setting.color[3],szPer);
	// pgScreenFlipV()を使うとpgWaitVが呼ばれてしまうのでこちらで。
	// プログレスだからちらついても良いよね～
	pgScreenFlipV();
}



// コールバック受け渡し用
typedef struct {
	byte *pLoadBuffer;			// pointer to rom image
	uint32 ulLoadBufferSize;	// rom image size
	uint32 ulLoadedRomSize;		// loaded rom size
	boolean bIsBufferError;		// buffer error flag
	boolean bIsLoadDone;		// is load done?
}ROM_INFO, *LPROM_INFO;


// Unzip コールバック
int funcUnzipCallback(int nCallbackId, unsigned long ulExtractSize, unsigned long ulCurrentPosition,
                      const void *pData, unsigned long ulDataSize, unsigned long ulUserData)
{
    const char *pszFileName;
    int nExtId;
    const unsigned char *pbData;
    LPROM_INFO pRomInfo = (LPROM_INFO)ulUserData;

    switch(nCallbackId) {
    case UZCB_FIND_FILE:
		pszFileName = (const char *)pData;
		// 拡張子がファイルがNESなら展開
		nExtId = getExtId(pszFileName, (LPEXTENTIONS)&stExtRom);
		if (nExtId == EXT_NES) {
			draw_load_rom_progress(0, 100);
			return UZCBR_OK;
		}
        break;
    case UZCB_EXTRACT_PROGRESS:
		pbData = (const unsigned char *)pData;
		if ((ulCurrentPosition + ulDataSize) > pRomInfo->ulLoadBufferSize) {
			pRomInfo->bIsBufferError = TRUE;
			return UZEXR_CANCEL;
		}
		// 展開されたデータを格納しよう
		_memcpy(pRomInfo->pLoadBuffer + ulCurrentPosition, pbData, ulDataSize);
		draw_load_rom_progress(ulCurrentPosition + ulDataSize, ulExtractSize);
		// 展開終わっていたら抜ける
		if (ulCurrentPosition + ulDataSize == ulExtractSize) {
			pRomInfo->ulLoadedRomSize = ulExtractSize;
			pRomInfo->bIsLoadDone = TRUE;
			return UZEXR_CANCEL;
		}
		return UZCBR_OK;
        break;
    default: // unknown...
        break;
    }
    return UZCBR_PASS;
}


// load rom image by ruka
uint32 load_rom_image(uint8 *buf, uint32 bufLen, const char *szRomPath)
{
	int fd;
	ROM_INFO stRomInfo;
	int nRet;
	const char *szNewRomPath;

	stRomInfo.pLoadBuffer = buf;
	stRomInfo.ulLoadBufferSize = bufLen;
	stRomInfo.ulLoadedRomSize = 0;
	stRomInfo.bIsBufferError = FALSE;
	stRomInfo.bIsLoadDone = FALSE;

	szNewRomPath = szRomPath;
	if(setting.fatread)
	{
		szNewRomPath = cnRomPath;
	}
	switch(getExtId(szNewRomPath, (LPEXTENTIONS)&stExtRom)) {
	case EXT_NES:	// "nes" or "fds" or "fam" or "unf" or "nsf"
		fd=sceIoOpen(szNewRomPath, PSP_O_RDONLY, 0777);
		if (fd >= 0) {
			stRomInfo.ulLoadedRomSize = sceIoRead(fd, stRomInfo.pLoadBuffer, stRomInfo.ulLoadBufferSize);
			sceIoClose(fd);
			break;
		}
		return 0;
		break;
	case EXT_ZIP:	// "zip"
		// Unzipコールバックセット
		Unzip_setCallback(funcUnzipCallback);
		// Unzip展開する
	    nRet = Unzip_execExtract(szNewRomPath, (unsigned long)&stRomInfo);
		if (stRomInfo.bIsLoadDone) {
			// 展開成功
			break;
		}
		if (nRet == UZEXR_CANCEL && stRomInfo.bIsBufferError) {
			// out of buffer.
			SHOW_ERRMSG(RGB(255,0,0), "Unzip out of buffer.", 0xFFFF, 60);
			return 0;
		}
		// 該当のROMイメージが見つからなかった
		// fatal error.
		MessageBox("NES ROM Image was not found.", RGB(255, 0, 0), MB_OK);
		return 0;
		break;
	default:
		return 0;
		break;
	}
	return stRomInfo.ulLoadedRomSize;
}
