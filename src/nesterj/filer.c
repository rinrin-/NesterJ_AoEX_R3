#include <pspiofilemgr.h>
#include <pspctrl.h>
#include <stdlib.h>
#include "main.h"
#include "fat.h"

extern u32 new_pad;

SceIoDirent files[MAX_ENTRY];
SceIoDirent *sortfiles[MAX_ENTRY];


SceIoDirent *fileTemp;
char szGbkFiles[MAX_ENTRY][256];
extern char cnRomPath[MAX_PATH];

extern void NES_DeleteFile(const char *pFile);
extern u32 MessageBox(char *pszMsg, u32 color, u32 ulTypes);
extern void mh_printGBK12(int x,int y,const char *str,int col);

////////////////////////////////////////////////////////////////////////
// 僋僀僢僋僜乕僩
void SJISCopyGBK(SceIoDirent *a, unsigned char *file)
{
	//unsigned char ca;
	int i;
	int len = _strlen(a->d_name);

	for(i=0;i<=len;i++)
	{
		file[i]=a->d_name[i];
	}

}

void SJISCopy(SceIoDirent *a, unsigned char *file)
{
	unsigned char ca;
	int i;
	int len = _strlen(a->d_name);

	for(i=0;i<=len;i++){
		ca = a->d_name[i];
		if (((0x81 <= ca)&&(ca <= 0x9f))
		|| ((0xe0 <= ca)&&(ca <= 0xef))){
			file[i++] = ca;
			file[i] = a->d_name[i];
		}
		else{
			if(ca>='a' && ca<='z') ca-=0x20;
			file[i] = ca;
		}
	}

}
int cmpFileGBK(SceIoDirent *a, SceIoDirent *b)
{
    unsigned char file1[0x108];
    unsigned char file2[0x108];
	unsigned char ca, cb;
	int i, n, ret;

	if(a->d_stat.st_attr==b->d_stat.st_attr){
		SJISCopyGBK(a, file1);
		SJISCopyGBK(b, file2);
		n=_strlen((char*)file1);
		for(i=0; i<=n; i++){
			ca=file1[i]; cb=file2[i];
			ret = ca-cb;
			if(ret!=0) return ret;
		}
		return 0;
	}

	if(a->d_stat.st_attr & FIO_SO_IFDIR) return -1;
	else					return 1;
}

int cmpFile(SceIoDirent *a, SceIoDirent *b)
{
    unsigned char file1[0x108];
    unsigned char file2[0x108];
	unsigned char ca, cb;
	int i, n, ret;

	if(a->d_stat.st_attr==b->d_stat.st_attr){
		SJISCopy(a, file1);
		SJISCopy(b, file2);
		n=_strlen((char*)file1);
		for(i=0; i<=n; i++){
			ca=file1[i]; cb=file2[i];
			ret = ca-cb;
			if(ret!=0) return ret;
		}
		return 0;
	}

	if(a->d_stat.st_attr & FIO_SO_IFDIR) return -1;
	else					return 1;
}

void sort(SceIoDirent **a, int left, int right) {
	SceIoDirent *tmp, *pivot;
	int i, p;

	if (left < right) {
		pivot = a[left];
		p = left;
		for (i=left+1; i<=right; i++) {
			if (cmpFile(a[i],pivot)<0){
				p=p+1;
				tmp=a[p];
				a[p]=a[i];
				a[i]=tmp;
			}
		}
		a[left] = a[p];
		a[p] = pivot;
		sort(a, left, p-1);
		sort(a, p+1, right);
	}
}

void sortGBK(SceIoDirent **a, int left, int right) {
	SceIoDirent *tmp, *pivot;
	int i, p;

	if (left < right) {
		pivot = a[left];
		p = left;
		for (i=left+1; i<=right; i++) {
			if (cmpFileGBK(a[i],pivot)<0){
				p=p+1;
				tmp=a[p];
				a[p]=a[i];
				a[i]=tmp;
			}
		}
		a[left] = a[p];
		a[p] = pivot;
		sortGBK(a, left, p-1);
		sortGBK(a, p+1, right);
	}
}

int getExtId(const char *szFilePath, LPEXTENTIONS pExt) {
	char *pszExt;
	int i;
	if((pszExt = _strrchr(szFilePath, '.'))) {
		pszExt++;
		for (i = 0; pExt[i].szExt; i++) {
			if (!_stricmp(pExt[i].szExt,pszExt)) {
				break;
			}
		}
		return pExt[i].nExtId;
	}
	return EXT_UNKNOWN;
}

int getExtIdGBK(const char *szFilePath, LPEXTENTIONS pExt) {
	char *pszExt;
	int i;
	if((pszExt = _strrchr(szFilePath, '.'))) {
		pszExt++;
		for (i = 0; pExt[i].szExt; i++) {
			if (!_stricmp(pExt[i].szExt,pszExt)) {
				break;
			}
		}
		return pExt[i].nExtId;
	}
	return EXT_UNKNOWN;
}



#include "GBK_USC2.h"

unsigned short Ucs2toGbk(unsigned short Uni)
{
	unsigned short iUni = 0;
	for(;iUni<22047; iUni++)
	{
		if( GBK_UCS2[iUni][1]==Uni )
			return GBK_UCS2[iUni][0];
	}
	return 0;
}

int Ucs2StringtoGbk(unsigned short *StrUniCode,char *strGbk)
{
	if( (StrUniCode==NULL)||(strGbk==NULL) )
		return 0;

	int iUni=0,jGbk=0;
	unsigned short Gbk;
	while(1)
	{
		Gbk = Ucs2toGbk(StrUniCode[iUni++]);
		if(Gbk==0)
			break;
		if(Gbk>0xFF)
		{
			strGbk[jGbk++]= Gbk>>8;
			strGbk[jGbk++]= Gbk&0xff;
		}
		else{
			strGbk[jGbk++]= Gbk;
		}

	}
	strGbk[jGbk]= 0;
	return jGbk;
}

int getDirExGBK(const char *comppath, LPEXTENTIONS pExt)
{
	char pszGbk[256];

	if( fat_init(sceKernelDevkitVersion())==0)
	{
		return 0;
	}
	p_fat_info info;
	int nfiles = 0;
	int b=0;
	char path[256];

	_strcpy(path,comppath);
	unsigned long i, count = fat_readdir( comppath ,  path , &info);
	if(count == INVALID)
	{
		return 0;
	}

	for(i=0; i<MAX_ENTRY;i++)
		_memset(szGbkFiles[i],0x00,256);

	if(_strcmp(comppath,"ms0:/"))//判断是否根目录
	{
		_strcpy(files[nfiles].d_name,"..");
		_strcpy(szGbkFiles[nfiles],"..");
		files[nfiles].d_stat.st_attr = FIO_SO_IFDIR;
		sortfiles[nfiles] = files + nfiles;
		nfiles++;
		b=1;
	}

	for(i = 0; i < count; i ++)
	{
			_memset(&files[nfiles], 0x00, sizeof(SceIoDirent));//清空files[nfiles]

			files[nfiles].d_stat.st_attr = info[i].attr;
			//_strcpy(files[nfiles].d_name,info[i].filename);
			//Utf8StringtoGbk((unsigned char*)info[i].longname,pszGbk);
			if(_strcmp(info[i].filename,info[i].longname)==0)
			{
				_strcpy(szGbkFiles[nfiles],info[i].filename);
			}
			else
			{
				_memset(pszGbk,0x00,256);
				Ucs2StringtoGbk((unsigned short*)info[i].longname,pszGbk);
				_strcpy(szGbkFiles[nfiles],pszGbk);//后面排序文件，所以这样做不对
			}
			_strcpy(files[nfiles].d_name,info[i].filename);
			//filename[256];longname[256];

			if( nfiles<MAX_ENTRY)
			{
				if(files[nfiles].d_name[0] == '.')//继续
					continue;

				if(files[nfiles].d_stat.st_attr == FAT_FILEATTR_DIRECTORY)//保存目录
				{
					_strcat(files[nfiles].d_name, "/");
					_strcat(szGbkFiles[nfiles],"/");
					sortfiles[nfiles] = files + nfiles;
					nfiles++;
					continue;
				}
				sortfiles[nfiles] = files + nfiles;//保存文件
				if (pExt) //是否判断扩展名
				{
					if(getExtId(files[nfiles].d_name, pExt) != EXT_UNKNOWN) nfiles++;
				}
				else
				{
					nfiles++;
				}
			}
	}
	free((void *)info);
	fat_free();

	if(b)
		sortGBK(sortfiles+1, 0, nfiles-2);
	else
		sortGBK(sortfiles, 0, nfiles-1);
	return nfiles;
}

extern void _Str2Hex(/*IN*/const char* Str,/*OUT*/ char* pHex);
int getFilePathGBK(char *out, char *pszStartPath, LPEXTENTIONS pExt, char *pszSelFile, int nOldSel)
{
	unsigned long color;
	int top, rows=17, x, y, h, i, bMsg=0, up=0;
	char path[MAXPATH], oldDir[MAXNAME], *p;
	int sel = nOldSel;
	int nfiles;
	unsigned long iShowFile;
	char delFile[MAXPATH];

	top = sel-3;

	_strcpy(path, pszStartPath);
	if(FilerMsg[0])
		bMsg=1;

	nfiles = getDirExGBK(path, pExt);

	if (pszSelFile) {
		// 堦抳偡傞僼傽僀儖柤傪扵偡
		int nIndex;
		for (nIndex = 0; nIndex < nfiles; nIndex++) {
			if (!_strcmp(sortfiles[nIndex]->d_name, pszSelFile)) {
				sel = nIndex;
				break;
			}
		}
	}

	for(;;){
		readpad();
		if(new_pad)
			bMsg=0;
		if(new_pad & PSP_CTRL_CIRCLE){
			if(sortfiles[sel]->d_stat.st_attr == FIO_SO_IFDIR){
				if(!_strcmp(sortfiles[sel]->d_name,"..")){
					up=1;
				}else{
					_strcat(path,sortfiles[sel]->d_name);
					nfiles = getDirExGBK(path, pExt);
					sel=0;
				}
			}else{
				_Str2Hex(sortfiles[sel]->d_name,delFile);
				_strcpy(out, path);
				if(setting.fatread)
				{
					_strcat(out, delFile);
					_strcpy(cnRomPath,path);
					_strcat(cnRomPath,sortfiles[sel]->d_name);
				}
				else
					_strcat(out, sortfiles[sel]->d_name);

				_strcpy(pszStartPath,path);
				if (pszSelFile)
				{
					if(setting.fatread)
					_strcpy(pszSelFile,delFile);
					else
					_strcpy(pszSelFile,sortfiles[sel]->d_name);
				}
				return sel;
			}
		}else if(new_pad & PSP_CTRL_SQUARE){
			if(sortfiles[sel]->d_stat.st_attr == FIO_SO_IFDIR)
			{
				;//dummy
			}else{
				if (MessageBox("Are you sure to delete this file ?", RGB(255, 0, 0), 2) == 1) 
				{
					_strcpy(delFile, path);
					_strcat(delFile, sortfiles[sel]->d_name);
					NES_DeleteFile(delFile);
					nfiles = getDirExGBK(path, pExt);
					sel=0;//删除以后
				}
			}
		}else if(new_pad & PSP_CTRL_CROSS){
			return -1;
		}else if(new_pad & PSP_CTRL_TRIANGLE){
			up=1;
		}else if(new_pad & PSP_CTRL_AD_UP){
			sel--;
		}else if(new_pad & PSP_CTRL_AD_DOWN){
			sel++;
		}else if(new_pad & PSP_CTRL_AD_LEFT){
			sel-=10;
		}else if(new_pad & PSP_CTRL_AD_RIGHT){
			sel+=10;
		}

		if(up){
			if(_strcmp(path,"ms0:/")){
				p=_strrchr(path,'/');
				*p=0;
				p=_strrchr(path,'/');
				p++;
				_strcpy(oldDir,p);
				_strcat(oldDir,"/");
				*p=0;
				nfiles = getDirExGBK(path, pExt);
				sel=0;
				for(i=0; i<nfiles; i++) {
					if(!_strcmp(oldDir, sortfiles[i]->d_name)) {
						sel=i;
						top=sel-3;
						break;
					}
				}
			}
			up=0;
		}

		if(top > nfiles-rows)	top=nfiles-rows;
		if(top < 0)				top=0;
		if(sel >= nfiles)		sel=nfiles-1;
		if(sel < 0)				sel=0;
		if(sel >= top+rows)		top=sel-rows+1;
		if(sel < top)			top=sel;


		if(bMsg)
			menu_frame(FilerMsg,"仜丗OK丂亊丗CANCEL丂仮丗UP");
		else
			menu_frame(path,"仜丗OK丂亊丗CANCEL丂仮丗UP");


		// 僗僋儘乕儖僶乕
		if(nfiles > rows){
			h = 219;
			pgDrawFrame(445,25,446,248,setting.color[1]);
			pgFillBox(448, h*top/nfiles + 27,
				460, h*(top+rows)/nfiles + 27,setting.color[1]);
		}

		x=28; y=32;
		for(i=0; i<rows; i++){
			if(top+i >= nfiles) break;
			if(top+i == sel) color = setting.color[2];
			else			 color = setting.color[3];
			//mh_printGBK(x, y, sortfiles[top+i]->d_name, color);
			//mh_printGBK(x, y, szGbkFiles[top+i], color);

			fileTemp = sortfiles[top+i] -files;
			iShowFile = (unsigned long)fileTemp;
			mh_printGBK12(x, y, szGbkFiles[iShowFile], color);

			y+=12;
		}

		pgScreenFlipV();
	}
}


int getDir(const char *path, LPEXTENTIONS pExt) {
	int fd, b=0;

	int nfiles = 0;

	if(_strcmp(path,"ms0:/")){
		_strcpy(files[nfiles].d_name,"..");
		files[nfiles].d_stat.st_attr = FIO_SO_IFDIR;
		sortfiles[nfiles] = files + nfiles;
		nfiles++;
		b=1;
	}

	fd = sceIoDopen(path);
	while(nfiles<MAX_ENTRY){
		_memset(&files[nfiles], 0x00, sizeof(SceIoDirent));
		if(sceIoDread(fd, &files[nfiles])<=0) break;
		if(files[nfiles].d_name[0] == '.') continue;
		if(files[nfiles].d_stat.st_attr == FIO_SO_IFDIR){
			_strcat(files[nfiles].d_name, "/");
			sortfiles[nfiles] = files + nfiles;
			nfiles++;
			continue;
		}
		sortfiles[nfiles] = files + nfiles;
		if (pExt) {
			if(getExtId(files[nfiles].d_name, pExt) != EXT_UNKNOWN) nfiles++;
		}
		else {
			nfiles++;
		}
	}
	sceIoDclose(fd);
	if(b)
		sort(sortfiles+1, 0, nfiles-2);
	else
		sort(sortfiles, 0, nfiles-1);
	return nfiles;
}

char FilerMsg[256];
int getFilePath(char *out, char *pszStartPath, LPEXTENTIONS pExt, char *pszSelFile, int nOldSel)
{
	unsigned long color;
	int top, rows=21, x, y, h, i, bMsg=0, up=0;
	char path[MAXPATH], oldDir[MAXNAME], *p;
	int sel = nOldSel;
	int nfiles;

	top = sel-3;

	_strcpy(path, pszStartPath);
	if(FilerMsg[0])
		bMsg=1;

	nfiles = getDir(path, pExt);

	if (pszSelFile) {
		// 堦抳偡傞僼傽僀儖柤傪扵偡
		int nIndex;
		for (nIndex = 0; nIndex < nfiles; nIndex++) {
			if (!_strcmp(sortfiles[nIndex]->d_name, pszSelFile)) {
				sel = nIndex;
				break;
			}
		}
	}

	for(;;){
		readpad();
		if(new_pad)
			bMsg=0;
		if(new_pad & PSP_CTRL_CIRCLE){
			if(sortfiles[sel]->d_stat.st_attr == FIO_SO_IFDIR){
				if(!_strcmp(sortfiles[sel]->d_name,"..")){
					up=1;
				}else{
					_strcat(path,sortfiles[sel]->d_name);
					nfiles = getDir(path, pExt);
					sel=0;
				}
			}else{
				_strcpy(out, path);
				_strcat(out, sortfiles[sel]->d_name);
				_strcpy(pszStartPath,path);
				if (pszSelFile) _strcpy(pszSelFile,sortfiles[sel]->d_name);
				return sel;
			}
		}else if(new_pad & PSP_CTRL_CROSS){
			return -1;
		}else if(new_pad & PSP_CTRL_TRIANGLE){
			up=1;
		}else if(new_pad & PSP_CTRL_AD_UP){
			sel--;
		}else if(new_pad & PSP_CTRL_AD_DOWN){
			sel++;
		}else if(new_pad & PSP_CTRL_AD_LEFT){
			sel-=10;
		}else if(new_pad & PSP_CTRL_AD_RIGHT){
			sel+=10;
		}

		if(up){
			if(_strcmp(path,"ms0:/")){
				p=_strrchr(path,'/');
				*p=0;
				p=_strrchr(path,'/');
				p++;
				_strcpy(oldDir,p);
				_strcat(oldDir,"/");
				*p=0;
				nfiles = getDir(path, pExt);
				sel=0;
				for(i=0; i<nfiles; i++) {
					if(!_strcmp(oldDir, sortfiles[i]->d_name)) {
						sel=i;
						top=sel-3;
						break;
					}
				}
			}
			up=0;
		}

		if(top > nfiles-rows)	top=nfiles-rows;
		if(top < 0)				top=0;
		if(sel >= nfiles)		sel=nfiles-1;
		if(sel < 0)				sel=0;
		if(sel >= top+rows)		top=sel-rows+1;
		if(sel < top)			top=sel;

		if(bMsg)
			menu_frame(FilerMsg,"仜丗OK丂亊丗CANCEL丂仮丗UP");
		else
			menu_frame(path,"仜丗OK丂亊丗CANCEL丂仮丗UP");

		// 僗僋儘乕儖僶乕
		if(nfiles > rows){
			h = 219;
			pgDrawFrame(445,25,446,248,setting.color[1]);
			pgFillBox(448, h*top/nfiles + 27,
				460, h*(top+rows)/nfiles + 27,setting.color[1]);
		}

		x=28; y=32;
		for(i=0; i<rows; i++){
			if(top+i >= nfiles) break;
			if(top+i == sel) color = setting.color[2];
			else			 color = setting.color[3];
			mh_print(x, y, sortfiles[top+i]->d_name, color);
			y+=10;
		}

		pgScreenFlipV();
	}
}
