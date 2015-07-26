#ifndef FILER_H
#define FILER_H

#include <pspiofilemgr.h>

#define MAX_ENTRY 2048
#define MAXPATH 512
#define MAXNAME 256

extern SceIoDirent files[MAX_ENTRY];
extern SceIoDirent *sortfiles[MAX_ENTRY];

extern char FilerMsg[];

// 拡張子管理用
typedef struct {
	char *szExt;
	int nExtId;
} EXTENTIONS, *LPEXTENTIONS;

int getExtId(const char *szFilePath, LPEXTENTIONS pExt);

int getFilePath(char *out, char *pszStartPath, LPEXTENTIONS pExt, char *pszSelFile, int nOldSel);

// 有効な拡張子
enum {
	EXT_NES,
	EXT_ZIP,
	EXT_ALL,
	EXT_CHJ,
	EXT_UNKNOWN
};

int getExtIdGBK(const char *szFilePath, LPEXTENTIONS pExt);
int getFilePathGBK(char *out, char *pszStartPath, LPEXTENTIONS pExt, char *pszSelFile, int nOldSel);

#endif
