#ifndef NES_FILEIO_H_
#define NES_FILEIO_H_

//#include "nes/types.h"

#ifdef __cplusplus
extern "C" {
#endif

enum {
	FILE_SEEK_CUR,
	FILE_SEEK_END,
	FILE_SEEK_SET,
};

enum {
 FILE_MODE_READ,
 FILE_MODE_WRITE,
 FILE_MODE_APPEND,
};

//typedef int HANDLE;
//typedef unsigned int size_t;

int NES_fopen(const char *pPath, int mode);

void NES_DeleteFile(const char *pFile);

int NES_fgetc(int fh);
int NES_fputc(int ch, int fh);

int NES_fgets(char *Buf, int nLen, int fh);

int NES_fread(void *, int size, int count, int fh);
int NES_fwrite(void *, int size, int count, int fh);

int NES_fseek(int fh, long offset, int origin);

int NES_fclose(int fh);


void GetModulePath(char *fn, int nSize);
void GetStatePath(char *fn, int nSize);
void GetSavePath(char *fn, int nSize);
char *PathFindFileName(const char *fn);
void NES_PathAddBackslash(char *fn);
void NES_PathRemoveFileSpec(char *fn);
void NES_PathRemoveExtension(char *fn);




#ifdef __cplusplus
}
#endif

#endif
