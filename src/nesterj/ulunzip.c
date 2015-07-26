
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "debug/debug.h"

#define UNZ_BUFSIZE (65536)

#include "unzip.h"
#include "zip.h"
#include "ulunzip.h"

int UseUnzipLibExeLoad(char *fn, unsigned char **buff, unsigned long *ucsize, const char **szExtensions){
	uLong i;
	int j;
	unz_global_info gi;
	int err;
	unzFile uf=NULL;
	unsigned char *tmem;
	char filename_inzip[256], *extp;
	unz_file_info file_info;

	uf = unzOpen(fn);
	if(uf==NULL)
		return 0;
	err = unzGetGlobalInfo (uf,&gi);
	if(err!=UNZ_OK)
		return 0;
	for (i=0;i<gi.number_entry;i++){
		err = unzGetCurrentFileInfo(uf,&file_info,filename_inzip,sizeof(filename_inzip),NULL,0,NULL,0);
		if(err!=UNZ_OK)
			break;
		extp=_strrchr(filename_inzip, '.');
		++extp;
		int flag=0;
		for(j=0; szExtensions[j]!=NULL; ++j){
			if(!_stricmp(extp, (char *)szExtensions[j])){
				flag=1;
				break;
			}
		}
		if(flag){
			if(ucsize)
				*ucsize = file_info.uncompressed_size;
			tmem = (unsigned char *)malloc(file_info.uncompressed_size);
			if(tmem==NULL)
				break;
			err = unzOpenCurrentFile(uf);
			if(err!=UNZ_OK)
				break;
			err = unzReadCurrentFile(uf, tmem, file_info.uncompressed_size);
			unzCloseCurrentFile(uf);
			unzClose(uf);
			if((unsigned int)err != file_info.uncompressed_size){
				free(tmem);
				return 0;
			}
			*buff = tmem;
			return 1;
		}
		if ((i+1)<gi.number_entry){
			err = unzGoToNextFile(uf);
			if (err!=UNZ_OK){
				break;
			}
		}
	}
	unzCloseCurrentFile(uf);
	unzClose(uf);
	return 0;
}



int UseUnzipLibExeLoad_P(char *fn, unsigned char *buff, unsigned long msize, unsigned long *ucsize, const char **szExtensions){
	uLong i;
	int j;
	unz_global_info gi;
	int err;
	unzFile uf=NULL;
	char filename_inzip[256], *extp;
	unz_file_info file_info;

	if(buff==NULL)
		return 0;
	uf = unzOpen(fn);
	if(uf==NULL)
		return 0;
	err = unzGetGlobalInfo (uf,&gi);
	if(err!=UNZ_OK)
		return 0;
	for (i=0;i<gi.number_entry;i++){
		err = unzGetCurrentFileInfo(uf,&file_info,filename_inzip,sizeof(filename_inzip),NULL,0,NULL,0);
		if(err!=UNZ_OK)
			break;
		extp=_strrchr(filename_inzip, '.');
		++extp;
		int flag=0;
		for(j=0; szExtensions[j]!=NULL; ++j){
			if(!_stricmp(extp, (char *)szExtensions[j])){
				flag=1;
				break;
			}
		}
		if(flag){
			if(ucsize)
				*ucsize = file_info.uncompressed_size;
			err = unzOpenCurrentFile(uf);
			if(err!=UNZ_OK)
				break;
			if(!msize)
				msize = file_info.uncompressed_size;
			err = unzReadCurrentFile(uf, buff, msize/*file_info.uncompressed_size*/);
			unzCloseCurrentFile(uf);
			unzClose(uf);
			if((unsigned int)err != msize/*file_info.uncompressed_size*/){
				DEBUG("err != msize");
				return 0;
			}
			return 1;
		}
		if ((i+1)<gi.number_entry){
			err = unzGoToNextFile(uf);
			if (err!=UNZ_OK){
				break;
			}
		}
	}
	unzCloseCurrentFile(uf);
	unzClose(uf);
	DEBUG("UseUnzipLibExeLoad_P Failed");
	return 0;
}


int UsezipLibMemToZip(unsigned char *memp, unsigned int csize, char *outfn, char *infn){
	zipFile zf;
	zip_fileinfo zi;
    int opt_compress_level=Z_DEFAULT_COMPRESSION;
    int err=0;

	zf = zipOpen(outfn,0);
	if(zf == NULL){
		return 0;
	}

	zi.tmz_date.tm_sec = zi.tmz_date.tm_min = zi.tmz_date.tm_hour = zi.tmz_date.tm_mday = zi.tmz_date.tm_min = zi.tmz_date.tm_year = 0;
	zi.dosDate = 0;
	zi.internal_fa = 0;
	zi.external_fa = 0;
//	filetime(filenameinzip,&zi.tmz_date,&zi.dosDate);

	err = zipOpenNewFileInZip(zf, infn/*outfn*/,&zi,	NULL,0,NULL,0,NULL /* comment*/,
		(opt_compress_level != 0) ? Z_DEFLATED : 0,	opt_compress_level);
	if(err != ZIP_OK)
		return 0;
	err = ZIP_OK;
	err = zipWriteInFileInZip (zf, memp, csize);
	err = zipCloseFileInZip(zf);
	zipClose(zf,NULL);
	return 1;
}



int UsezipLibFindFileFromExtention(char *fn, const char **szExtensions){
	uLong i;
	int j;
	unz_global_info gi;
	int err;
	unzFile uf=NULL;
	char filename_inzip[256], *extp;
	unz_file_info file_info;

	uf = unzOpen(fn);
	if(uf==NULL)
		return 0;
	err = unzGetGlobalInfo (uf,&gi);
	if(err!=UNZ_OK)
		return 0;
	for (i=0;i<gi.number_entry;i++){
		err = unzGetCurrentFileInfo(uf,&file_info,filename_inzip,sizeof(filename_inzip),NULL,0,NULL,0);
		if(err!=UNZ_OK)
			break;
		extp=_strrchr(filename_inzip, '.');
		++extp;
		int flag=0;
		for(j=0; szExtensions[j]!=NULL; ++j){
			if(!_stricmp(extp, (char *)szExtensions[j])){
				flag=1;
				break;
			}
		}
		if(flag){
			unzCloseCurrentFile(uf);
			unzClose(uf);
			return 1;
		}
		if ((i+1)<gi.number_entry){
			err = unzGoToNextFile(uf);
			if (err!=UNZ_OK){
				break;
			}
		}
	}
	unzCloseCurrentFile(uf);
	unzClose(uf);
	return 0;
}
