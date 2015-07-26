/*
** nester - NES emulator
** Copyright (C) 2000  Darren Ranalli
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Library General Public License for more details.  To obtain a
** copy of the GNU Library General Public License, write to the Free
** Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
** Any permitted reproduction of these routines, in whole or in part,
** must bear this legend.
**
** 埑弅僼傽僀儖懳墳偺偨傔丄傒偐傒偐側 偵傛偭偰丄Win32 API偺 File IO 偵
** 彂偒偐偊傜傟偰偄傑偡丅
**
** 晲揷偵傛偭偰FAM,g_ROM.fds宍幃偺撉傒崬傒偵娭偡傞僐乕僪偑捛壛偝傟偰偄傑偡丅
**
** PSP堏怉偺偨傔CPP偐傜C偵曄峏偟偰偄傑偡丅
*/

//#include <windows.h>
//#include <shlwapi.h>
//#include "arc.h"
#include "nes.h"
#include "nes_rom.h"
#include "../debug/debug.h"
//#include "crc32.h"
#include "nes_string.h"
//#include "resource.h"
#include "nes_crc32.h"
#include "../loadromimage.h"
#include "nes_config.h"
#include "nes_string.h"
#include "../fileio.h"
#include "unif.h"


NES_ROM g_ROM;	// ROM
#define MAX_IMAGE_SIZE 5*1024*1024+40960	// 嵟戝5M+40K偺撉傒崬傒僶僢僼傽
uint8 g_ReadRomImage[MAX_IMAGE_SIZE];

#define MKID(a) ((unsigned long) \
		(((a) >> 24) & 0x000000FF) | \
		(((a) >>  8) & 0x0000FF00) | \
		(((a) <<  8) & 0x00FF0000) | \
		(((a) << 24) & 0xFF000000))
		
#define CopyMemIncPtr(o,i,s) \
{\
	_memcpy(o,i,s);\
	i+=s;\
}

NES_ROM *NES_ROM_LoadRom(const char* fn)
{
	uint8 *p = NULL;
	uint8 *buf = g_ReadRomImage;
	uint32 filesize;
	uint8 image_type;

	_memset(g_ROM.trainer, 0x00, sizeof(g_ROM.trainer));
	_memset(g_ROM.rom_name, 0x00, sizeof(g_ROM.rom_name));
	_memset(g_ROM.rom_path, 0x00, sizeof(g_ROM.rom_path));
	g_ROM.crc = 0;
	g_ROM.crc_all = 0;
	g_ROM.fds = 0;
	g_ROM.unif_mapper=0;
	g_ROM.GameTitle[0] =0;
	g_ROM.dbcorrect[0]=g_ROM.dbcorrect[1]=g_ROM.dbcorrect[2]=0;

	image_type = 0;

	// store filename and path
	NES_ROM_GetPathInfo( fn );

	filesize = load_rom_image(buf, sizeof(g_ReadRomImage), fn);
	if (!filesize) {
		DEBUG("load_rom_image failed");
		return NULL;
	}
	DEBUGVALUE("filesize", filesize);

	// Header check...
	g_ROM.screen_mode = 1;
	unsigned char dbflag=0;
	//自定义校验 文件
	//添加,增加兼容性
	p = buf;		
	//rom修复功能
	uint32 crcNesFile = 0;
	crcNesFile = CrcCalca(p, filesize, crcNesFile);
	DEBUGVALUE("CRC:",crcNesFile);	

	/*
	p[0]=0;
	p[1]=0;
	p[2]=0;
	p[3]=0;
	p[4]=0;
	p[5]=0;
	p[6]=0;
	p[7]=0;
	p[8]=0;
	p[9]=0;
	p[10]=0;
	p[11]=0;
	p[12]=0;
	p[13]=0;
	p[14]=0;
	p[15]=0;*/

	if(g_NESConfig.preferences.UseRomDataBase)
	{
		int hFile;
		uint32 c, pt, i;
		char fn2[256], buf[512];
		unsigned char buf2[100];
		unsigned short head;

		GetModulePath(fn2, sizeof(fn2));
		_strcat(fn2, "byemudb.dat");

		hFile = NES_fopen(fn2, FILE_MODE_READ);
		if(hFile > 0)
		{
			DEBUGVALUE("byemudb已经打开", hFile);
			while(NES_fgets(buf, sizeof(buf), hFile))
			{
				pt = 0;
				// file CRC
				c = 0;
				{
					for(i = 0; i < 8 && buf[pt] != ';' && buf[pt]; i++, pt++)
					{
						buf2[i] = buf[pt];
					}
					if(buf[pt]=='\0')
						continue;
					++pt;
					buf2[i] = 0;
					c = _atoh(buf2);
				}

				if(crcNesFile == c && crcNesFile != 0 && c != 0)
				{
					//head
					i = 0;
					while(buf[pt] != ';')
						buf2[i++] = buf[pt++];
					pt++;
					buf2[i] = '\0';
					head = _atoh(buf2);
					DEBUGVALUE("MAP头", head);
					//写头
					p[6] = head>>8;
					p[7] = head&0xFF;

					// PROM/CROM Size
					i = 0;
					while(buf[pt] != ';')buf2[i++] = buf[pt++];
					pt++;
					buf2[i] = '\0';
					head = _atoh(buf2);
					DEBUGVALUE("PROM", head);
					p[4] = head>>8;
					p[5] = head&0xFF;;

					// Country
					if( buf[pt] == 'P' )
					{
						// PAL MODE
						g_ROM.screen_mode = 2;
					}
					dbflag=1;
					break;
				}
			}
			NES_fclose(hFile);
		}
	}
	
	p = buf;
	CopyMemIncPtr( &g_ROM.header, p, sizeof(NES_header) );
	if ( ( !_strncmp( (char*)g_ROM.header.id, "NES", 3) &&  g_ROM.header.ctrl_z == 0x1A ) ||
		    ( !_strncmp( (char*)g_ROM.header.id, "NEZ", 3) &&  g_ROM.header.ctrl_z == 0x1A ) ||
		    ( !_strncmp( (char*)g_ROM.header.id, "FDS", 3) &&  g_ROM.header.ctrl_z == 0x1A ) ||
		    ( g_ROM.header.id[0] <= 0x1A && g_ROM.header.id[1] == 0x00 && (g_ROM.header.id[2] || g_ROM.header.id[3] || g_ROM.header.ctrl_z) && g_ROM.header.num_8k_vrom_banks == 0x00 ) ||
		    ( !_strncmp( (char*)g_ROM.header.id, "NES", 3) && g_ROM.header.ctrl_z == 'M' ) ||
		    ( !_strncmp( (char*)g_ROM.header.id, "UNIF", 4))
	  ) {
		p=buf+sizeof(NES_header);
	}
	else
	{
		DEBUG("UNKNOWN FORMAT!");
		return NULL;
	}

	// internal check...

	if ( (!_strncmp( (char*)g_ROM.header.id, "NES", 3) && g_ROM.header.ctrl_z == 0x1A) ||
		 (!_strncmp( (char*)g_ROM.header.id, "NEZ", 3) && g_ROM.header.ctrl_z == 0x1A) ) {
//		if(!g_NESConfig.preferences.DisableIPSPatch)
//			LoadIPSPatch((unsigned char *)p, (char *)fn);		//IPS PATCH

		// load g_ROM.trainer if present
		if( NES_ROM_has_trainer() )
		{
			if( p+TRAINER_LEN-buf > filesize ){
				DEBUG("Error reading Trainer");
				return NULL;
			}
			CopyMemIncPtr( g_ROM.trainer, p, TRAINER_LEN );
			g_ROM.crc = CrcCalc(g_ROM.trainer, TRAINER_LEN);
		}
		
		if( p + (16*1024) * g_ROM.header.num_16k_rom_banks - buf > filesize )
		{
			g_ROM.header.num_16k_rom_banks = (filesize-16)/(16*1024);
		}
		if(g_ROM.header.num_8k_vrom_banks)
		{
			if(      (16*1024 * g_ROM.header.num_16k_rom_banks+g_ROM.header.num_8k_vrom_banks*8*1024+16)>filesize         )
			{
				g_ROM.header.num_8k_vrom_banks = (filesize-16-16*1024 * g_ROM.header.num_16k_rom_banks)/(8*1024);
			}
		}

		if( p + (16*1024) * g_ROM.header.num_16k_rom_banks - buf > filesize )
		{
			DEBUG("Error reading ROM banks");
			return NULL;
		}
		CopyMemIncPtr( g_ROM.ROM_banks, p, (16*1024) * g_ROM.header.num_16k_rom_banks );

		if( p + (8*1024) * g_ROM.header.num_8k_vrom_banks - buf > filesize ){
			DEBUG("Error reading VROM banks");
			return NULL;
		}
		CopyMemIncPtr( g_ROM.VROM_banks, p, (8*1024) * g_ROM.header.num_8k_vrom_banks );
		if(((g_ROM.header.flags_1 >> 4) | (g_ROM.header.flags_2 & 0xF0)) == 20)
		{
			uint32 i;
			uint8 disk_num;
			uint8 disk[0x10000];
			uint8 disk_header[15] =
				{
				    0x01,0x2A,0x4E,0x49,0x4E,0x54,0x45,0x4E,0x44,0x4F,0x2D,0x48,0x56,0x43,0x2A
				};
			image_type = 1;
			// convert NES disk image
			disk_num = g_ROM.header.num_16k_rom_banks >> 2;
			if(disk_num > 4)
			{
				disk_num = 4;
			}

			for (i = 0; i < disk_num; i++)
			{
				int rpos, wpos;
				wpos = i*65500+16;
				rpos = i*0x10000;

				_memcpy(disk, &g_ROM.ROM_banks[rpos], 65500);
				_memcpy(&g_ROM.ROM_banks[wpos], disk_header, 15);
				wpos+=15;
				_memcpy(&g_ROM.ROM_banks[wpos], disk, 65500-15);
			}

			//g_ROM.header.id[0] = 'N';
			//g_ROM.header.id[1] = 'E';
			//g_ROM.header.id[2] = 'S';
			//g_ROM.header.num_16k_rom_banks = disk_num*4;
			//g_ROM.header.num_8k_vrom_banks = 0;
			//g_ROM.header.flags_1 = 0x40;
			//g_ROM.header.flags_2 = 0x10;
			//g_ROM.header.reserved[0] = g_ROM.header.reserved[1] = g_ROM.header.reserved[2] = g_ROM.header.reserved[3] = 0;
			//g_ROM.header.reserved[4] = g_ROM.header.reserved[5] = g_ROM.header.reserved[6] = g_ROM.header.reserved[7] = 0;
			g_ROM.ROM_banks[0] = 'F';
			g_ROM.ROM_banks[1] = 'D';
			g_ROM.ROM_banks[2] = 'S';
			g_ROM.ROM_banks[3] = 0x1A;
			g_ROM.ROM_banks[4] = disk_num;
		}
	}
	else if( !_strncmp( (char*)g_ROM.header.id, "FDS", 3) && ( g_ROM.header.ctrl_z == 0x1A) )
	{
//		if(!g_NESConfig.preferences.DisableIPSPatch)
//			LoadIPSPatch((unsigned char *)p, (char *)fn);		//IPS PATCH
		uint8 disk_num;

		image_type = 1;
		disk_num = g_ROM.header.num_16k_rom_banks;

		g_ROM.header.id[0] = 'N';
		g_ROM.header.id[1] = 'E';
		g_ROM.header.id[2] = 'S';
		g_ROM.header.num_16k_rom_banks *= 4;
		g_ROM.header.num_8k_vrom_banks = 0;
		g_ROM.header.flags_1 = 0x40;
		g_ROM.header.flags_2 = 0x10;
		g_ROM.header.reserved[0] = g_ROM.header.reserved[1] = g_ROM.header.reserved[2] = g_ROM.header.reserved[3] = 0;
		g_ROM.header.reserved[4] = g_ROM.header.reserved[5] = g_ROM.header.reserved[6] = g_ROM.header.reserved[7] = 0;

		if( p + 65500 * disk_num - buf > filesize ){
			DEBUG("Error reading FDS Image");
			return NULL;
		}
		CopyMemIncPtr( g_ROM.ROM_banks + 16, p, 65500 * disk_num );

		g_ROM.ROM_banks[0] = 'F';
		g_ROM.ROM_banks[1] = 'D';
		g_ROM.ROM_banks[2] = 'S';
		g_ROM.ROM_banks[3] = 0x1A;
		g_ROM.ROM_banks[4] = disk_num;
	}
	else if(g_ROM.header.id[0] <= 0x1A && g_ROM.header.id[1] == 0x00 && g_ROM.header.num_8k_vrom_banks == 0x00)
	{
		uint8 fam[6];
		uint8 disk_num;
		image_type = 1;
		fam[0] = g_ROM.header.id[0];
		fam[1] = g_ROM.header.id[1];
		fam[2] = g_ROM.header.id[2];
		fam[3] = g_ROM.header.ctrl_z;
		fam[4] = g_ROM.header.num_16k_rom_banks;
		fam[5] = g_ROM.header.num_8k_vrom_banks;

		p = 6 + buf;

		while(!((fam[0] == 0x13 || fam[0] == 0x1A) && fam[1] == 0x00))
		{
			if(p + (uint32)fam[2]+((uint32)fam[3]<<8)+((uint32)fam[4]<<16)-6 - buf > filesize) {
				DEBUG("Error reading FAM image");
				return NULL;
			}
			p += (uint32)fam[2]+((uint32)fam[3]<<8)+((uint32)fam[4]<<16)-6;
			if(p + 6 - buf > filesize) {
				DEBUG("Error reading FAM image");
				return NULL;
			}
			CopyMemIncPtr( fam, p, 6 );
		}

		disk_num = (uint8)(((uint32)fam[2]+((uint32)fam[3]<<8)+((uint32)fam[4]<<16))/65500);

		g_ROM.header.id[0] = 'N';
		g_ROM.header.id[1] = 'E';
		g_ROM.header.id[2] = 'S';
		g_ROM.header.num_16k_rom_banks = disk_num*4;
		g_ROM.header.num_8k_vrom_banks = 0;
		g_ROM.header.flags_1 = 0x40;
		g_ROM.header.flags_2 = 0x10;
		g_ROM.header.reserved[0] = g_ROM.header.reserved[1] = g_ROM.header.reserved[2] = g_ROM.header.reserved[3] = 0;
		g_ROM.header.reserved[4] = g_ROM.header.reserved[5] = g_ROM.header.reserved[6] = g_ROM.header.reserved[7] = 0;

		if(fam[0] == 0x1A){
			if( p + 16 - buf > filesize ) {
				DEBUG("Error reading FAM image");
				return NULL;
			}
			p += 16;
		}

		if( p + 65500 * disk_num - buf > filesize ) {
			DEBUG("Error reading FAM image");
			return NULL;
		}
		CopyMemIncPtr( g_ROM.ROM_banks + 16, p, 65500 * disk_num );

		g_ROM.ROM_banks[0] = 'F';
		g_ROM.ROM_banks[1] = 'D';
		g_ROM.ROM_banks[2] = 'S';
		g_ROM.ROM_banks[3] = 0x1A;
		g_ROM.ROM_banks[4] = disk_num;
	}
	else if(!_strncmp((const char*)g_ROM.header.id, "NES", 3) && (g_ROM.header.ctrl_z == 'M'))
	{
		image_type = 2;

		if( filesize > 0x40000 ){
			DEBUG("NSF file is over 256 KB");
			return NULL;
		}
		CopyMemIncPtr( g_ROM.ROM_banks + 0x10, p, filesize - ( p - buf ) );

		g_ROM.ROM_banks[0x0] = filesize & 0xFF;
		g_ROM.ROM_banks[0x1] = (filesize & 0xFF00) >> 8;
		g_ROM.ROM_banks[0x2] = (filesize & 0xFF0000) >> 16;
		g_ROM.ROM_banks[0x3] = filesize >> 24;
		g_ROM.ROM_banks[0x4] = g_ROM.header.num_16k_rom_banks;
		g_ROM.ROM_banks[0x5] = g_ROM.header.num_8k_vrom_banks;
		g_ROM.ROM_banks[0x6] = g_ROM.header.flags_1;
		g_ROM.ROM_banks[0x7] = g_ROM.header.flags_2;
		g_ROM.ROM_banks[0x8] = g_ROM.header.reserved[0];
		g_ROM.ROM_banks[0x9] = g_ROM.header.reserved[1];
		g_ROM.ROM_banks[0xA] = g_ROM.header.reserved[2];
		g_ROM.ROM_banks[0xB] = g_ROM.header.reserved[3];
		g_ROM.ROM_banks[0xC] = g_ROM.header.reserved[4];
		g_ROM.ROM_banks[0xD] = g_ROM.header.reserved[5];
		g_ROM.ROM_banks[0xE] = g_ROM.header.reserved[6];
		g_ROM.ROM_banks[0xF] = g_ROM.header.reserved[7];

		g_ROM.header.id[0] = 'N';
		g_ROM.header.id[1] = 'E';
		g_ROM.header.id[2] = 'S';
		g_ROM.header.ctrl_z = 0x1A;
		g_ROM.header.num_16k_rom_banks = 1;
		g_ROM.header.num_8k_vrom_banks = 0;
		g_ROM.header.flags_1 = 0xC0;
		g_ROM.header.flags_2 = 0x00;
		g_ROM.header.reserved[0] = g_ROM.header.reserved[1] = g_ROM.header.reserved[2] = g_ROM.header.reserved[3] = 0;
		g_ROM.header.reserved[4] = g_ROM.header.reserved[5] = g_ROM.header.reserved[6] = g_ROM.header.reserved[7] = 0;
	}
	else if(!_strncmp((char*)g_ROM.header.id, "UNIF", 4))
	{// UNIF
	
		//处理UNIF格式ROM过程
		//UNIF标志置位
		//需要读取board名字
		//VROM等信息保存到相应的位置
		//unif_mapper至少为1;
		image_type = 3;
		uint8 *pUnif = buf;
		uint32 Signature, BlockLen;
		uint32 ipos =0x20;//跳过UNIF头
		g_ROM.unif_mapper = 0xFF;
		g_ROM.header.id[0] = 'N';
		g_ROM.header.id[1] = 'E';
		g_ROM.header.id[2] = 'S';
		g_ROM.header.ctrl_z = 0x1A;
		//g_ROM.header.flags_1 = 0x40;
		//g_ROM.header.flags_2 = 0x90;
		//g_ROM.unif_board;//
		char info[100];
		uint8 id,i;
		uint8 *tPRG[0x10], *tCHR[0x10];
		uint32 sizePRG[0x10],sizeCHR[0x10];

		for (i = 0; i < 0x10; i++)
		{
			tPRG[i] = tCHR[i] = 0;
		}

		//filesize
		while(ipos<filesize)
		{
			id = 0;
			_memcpy(&Signature,&pUnif[ipos],4);ipos+=4;
			_memcpy(&BlockLen,&pUnif[ipos],4);ipos+=4;
			
			switch(Signature)
			{
				case MKID('MAPR')://board名字
					DEBUGVALUE("board ",0);
					_memcpy( g_ROM.unif_board, &pUnif[ipos], BlockLen);
					ipos+=BlockLen;
					DEBUGVALUE(g_ROM.unif_board,0);
					break;
					
				//case MKID('NAME'):
				//	break;
					
				case MKID('TVCI')://TV制式
					g_ROM.screen_mode = (pUnif[ipos]+1)%3;
					ipos+=BlockLen;
					break;
					
				//case MKID('BATR')://是否电池记忆
				//	DEBUGVALUE("BATR ",0);
				//	break;
					
				case MKID('MIRR'):
				//	DEBUGVALUE("MIRR ",0);
					g_ROM.header.flags_1 = 0;
					//if (pUnif[ipos]==0)
						//g_ROM.header.flags_1 &=14;
					if (pUnif[ipos]==1)
						g_ROM.header.flags_1 |=1;
					ipos+=BlockLen;
					break;

				case MKID('PRGF'):	id++;
				case MKID('PRGE'):	id++;
				case MKID('PRGD'):	id++;
				case MKID('PRGC'):	id++;
				case MKID('PRGB'):	id++;
				case MKID('PRGA'):	id++;
				case MKID('PRG9'):	id++;
				case MKID('PRG8'):	id++;
				case MKID('PRG7'):	id++;
				case MKID('PRG6'):	id++;
				case MKID('PRG5'):	id++;
				case MKID('PRG4'):	id++;
				case MKID('PRG3'):	id++;
				case MKID('PRG2'):	id++;
				case MKID('PRG1'):	id++;
				case MKID('PRG0'):
				//case 0x30475250:
					//g_ROM.header.num_16k_rom_banks = BlockLen>>14;
					//g_ROM.unif_psize_16k = BlockLen>>14;
					sizePRG[id] = BlockLen;
					tPRG[id] = (uint8*)malloc(BlockLen);
					_memcpy( tPRG[id], &pUnif[ipos], BlockLen);
					ipos+=BlockLen;
					DEBUGVALUE("PRGX ",BlockLen);
					break;

				case MKID('CHRF'):	id++;
				case MKID('CHRE'):	id++;
				case MKID('CHRD'):	id++;
				case MKID('CHRC'):	id++;
				case MKID('CHRB'):	id++;
				case MKID('CHRA'):	id++;
				case MKID('CHR9'):	id++;
				case MKID('CHR8'):	id++;
				case MKID('CHR7'):	id++;
				case MKID('CHR6'):	id++;
				case MKID('CHR5'):	id++;
				case MKID('CHR4'):	id++;
				case MKID('CHR3'):	id++;
				case MKID('CHR2'):	id++;
				case MKID('CHR1'):	id++;
				case MKID('CHR0'):
				//case 0x30524843:
					//g_ROM.header.num_8k_vrom_banks = BlockLen>>13;
					//g_ROM.unif_csize_8k = BlockLen>>13;
					sizeCHR[id] = BlockLen;
					tCHR[id] = (uint8*)malloc(BlockLen);
					_memcpy( tCHR[id], &pUnif[ipos], BlockLen);
					ipos+=BlockLen;
					DEBUGVALUE("CHRX ",BlockLen);
					break;
					
				default:
					ipos+=BlockLen;	break;
			}
		}

		g_ROM.unif_psize_16k = 0;
		g_ROM.unif_csize_8k = 0;
		uint32 LenPRG=0,LenCHR=0;
		for (i = 0; i < 16/*0x10*/; i++)
		{
			if (tPRG[i])
			{
				_memcpy(&g_ROM.ROM_banks[LenPRG], tPRG[i], sizePRG[i]);
				LenPRG += sizePRG[i];
				g_ROM.unif_psize_16k  = (g_ROM.unif_psize_16k)+(sizePRG[i]>>14);
				free(tPRG[i]);
			}
			if (tCHR[i])
			{
				_memcpy(&g_ROM.VROM_banks[LenCHR], tCHR[i], sizeCHR[i]);
				LenCHR += sizeCHR[i];
				g_ROM.unif_csize_8k = (g_ROM.unif_csize_8k)+(sizeCHR[i]>>13);
				free(tCHR[i]);
			}
		}
	}
	else
	{
		DEBUG("Unsupported File");
		return NULL;
	}

	//	uint32 j;
	//g_ROM.screen_mode = 1;

	// figure out g_ROM.mapper number
	g_ROM.mapper = ( g_ROM.header.flags_1 >> 4);

	// if there is anything in the reserved bytes,
	// don't trust the high nybble of the mapper number
	//	for( i = 0; i < sizeof(g_ROM.header.reserved); i++ )
	//	{
	//		if(g_ROM.header.reserved[i] != 0x00) throw "Invalid NES g_ROM.header ($8-$F)";
	//	}
	g_ROM.mapper |= ( g_ROM.header.flags_2 & 0xF0 );

	g_ROM.dbcorrect[0]=0;
//	if(g_ROM.unif_mapper)
//		return;

	if(image_type == 1)
	{
		int i;
		//	g_ROM.screen_mode = 1;
		g_ROM.mapper = 20;

		g_ROM.fds = (g_ROM.ROM_banks[0x1f] << 24) | (g_ROM.ROM_banks[0x20] << 16) |
		      (g_ROM.ROM_banks[0x21] <<  8) | (g_ROM.ROM_banks[0x22] <<  0);
		for(i = 0; i < g_ROM.ROM_banks[4]; i++)
		{
			uint8 file_num = 0;
			uint32 pt = 16+65500*i+0x3a;
			while(g_ROM.ROM_banks[pt] == 0x03)
			{
				pt += 0x0d;
				pt += g_ROM.ROM_banks[pt] + g_ROM.ROM_banks[pt+1] * 256 + 4;
				file_num++;
			}
			g_ROM.ROM_banks[16+65500*i+0x39] = file_num;
		}
	}
	else if(image_type == 2)
	{
		//    g_ROM.screen_mode = 1;
		g_ROM.mapper = 12; // 12 is private g_ROM.mapper number
	}
	else if(image_type == 0)
	{
//		g_ROM.crc = CrcCalc(g_ROM.ROM_banks, g_ROM.header.num_16k_rom_banks * 0x4000);
		g_ROM.crc = CrcCalca(g_ROM.ROM_banks, g_ROM.header.num_16k_rom_banks * 0x4000, g_ROM.crc);
		g_ROM.crc_all = CrcCalca(g_ROM.VROM_banks, g_ROM.header.num_8k_vrom_banks * 0x2000, g_ROM.crc);
#if 1
		// get rom info from DataBase, and apply.
		if(g_NESConfig.preferences.UseRomDataBase&&(dbflag==0))
		{
			int hFile;
			uint32 c, pt, i, db=2;
			char fn2[256], buf[512];
			unsigned char theader[2] = {0,0};
			//unsigned char dbflag=0;
			while(db){
				GetModulePath(fn2, sizeof(fn2));
				if(db==2){
					// JAPANESE
					_strcat(fn2, "famicom.dat");
				}
				else{
					// ENGLISH
					_strcat(fn2, "nesdbase.dat");
				}
				hFile = NES_fopen(fn2, FILE_MODE_READ);
				if(hFile > 0){
					while(NES_fgets(buf, sizeof(buf), hFile)){
						pt = 0;
						// All CRC
						for(;buf[pt] != ';' && buf[pt]!='\0';pt++);
						if(buf[pt]=='\0')
							continue;
						pt++;
						// PROM CRC
						c = 0;
						{
							uint8 buf2[16];
							for(i = 0; i < 8 && buf[pt] != ';' && buf[pt]; i++, pt++){
								buf2[i] = buf[pt];
							}
							if(buf[pt]=='\0')
								continue;
							++pt;
							buf2[i] = 0;
							c = _atoh(buf2);
						}
						if(g_ROM.crc == c && g_ROM.crc != 0 && c != 0){
							// Title
							for(i=0; buf[pt] != ';' && buf[pt]!='\0'; ++i, ++pt){
								g_ROM.GameTitle[i] = buf[pt];
							}
							g_ROM.GameTitle[i]=0;
							//MystrFnIcut(GameTitle);
							pt++;
							uint8 buf2[16];
							// Header 1
							i = 0;
							while(buf[pt] != ';') buf2[i++] = buf[pt++];
							pt++;
							buf2[i] = '\0';
							//						g_ROM.header.flags_1 = atoi(buf2);
							theader[0] = _atoi(buf2);
							// Header 2
							i = 0;
							while(buf[pt] != ';') buf2[i++] = buf[pt++];
							pt++;
							buf2[i] = '\0';
							//						header.flags_2 = atoi(buf2);
							theader[1] = _atoi(buf2);
							// PROM Size
							while(buf[pt] != ';') pt++;
							pt++;
							// CROM Size
							while(buf[pt] != ';') pt++;
							pt++;
							// Country
							if(/*buf[pt] == 'A' ||*/ buf[pt] == 'E' || (buf[pt] == 'P' && buf[pt+1] == 'D') || buf[pt] == 'S')
							{
								// Asia, Europe, PD, Swedish
								g_ROM.screen_mode = 2;
							}
							/*						else if(buf[pt] == 'J' || buf[pt] == 'U' || buf[pt] == 'V'){
														screen_mode = 1;
													}*/
							//						mapper = (header.flags_1 >> 4) | (header.flags_2 & 0xF0);
							for(i = 0; i < 8; i++) g_ROM.header.reserved[i] = 0;
							//						fseek(fp2, 0, SEEK_END);
							dbflag=1;
							break;
						}
					}
					NES_fclose(hFile);
				}
				if(dbflag)
					break;
				--db;
			}

			if(dbflag){
				if(theader[0]!=g_ROM.header.flags_1 || ((theader[1]&0xf0)!=(g_ROM.header.flags_2&0xf0))){
//					SHOW_ERRMSG(0, "DB HIT! Applying,,,", 0xFFFF, 30);
/*					PSP偱偼妋擣偟傑偣傫
					if(NESTER_settings.nes.preferences.AutoRomCorrect){
						char str2[64], str3[64];
						LoadString(g_main_instance, IDS_STRING_LMSG_05 , str2, 64);
						LoadString(g_main_instance, IDS_STRING_MSGB_10 , str3, 64);
						if(IDYES== MessageBox(main_window_handle,(LPCSTR)str3,(LPCSTR)str2, MB_YESNO))
						{
							dbcorrect[0]=1, dbcorrect[1]= header.flags_1, dbcorrect[2]= header.flags_2;
							header.flags_1 = theader[0];
							header.flags_2 = theader[1];
							mapper = (header.flags_1 >> 4) | (header.flags_2 & 0xF0);
						}
					}
					else{
*/
					g_ROM.dbcorrect[0]=1, g_ROM.dbcorrect[1]= g_ROM.header.flags_1, g_ROM.dbcorrect[2]= g_ROM.header.flags_2;
					g_ROM.header.flags_1 = theader[0];
					g_ROM.header.flags_2 = theader[1];
					g_ROM.mapper = (g_ROM.header.flags_1 >> 4) | (g_ROM.header.flags_2 & 0xF0);
				}
			}
		}
#endif
	}
	#include "nes_rom_correct.cpp"
	return &g_ROM;
}

mirroring_type NES_ROM_get_mirroring()
{
	if(g_ROM.header.flags_1 & MASK_4SCREEN_MIRRORING)
	{
		return NES_PPU_MIRROR_FOUR_SCREEN;
	}
	else if(g_ROM.header.flags_1 & MASK_VERTICAL_MIRRORING)
	{
		return NES_PPU_MIRROR_VERT;
	}
	else
	{
		return NES_PPU_MIRROR_HORIZ;
	}
}


uint8 NES_ROM_get_mapper_num() { return g_ROM.mapper; }
uint8 NES_ROM_get_unifmapper_num() { return g_ROM.unif_mapper; }
int NES_ROM_get_unifBoardID()
{//可以优化算法
	int iCount=0;
	while(1)
	{
		if( iCount>=BOARD_MAX ) break;
		if(_stricmp(BoardInfo[iCount].boardName,g_ROM.unif_board)==0)
		{
			return BoardInfo[iCount].boardID;
		}
		iCount++;
	}

	return -1;
}

boolean  NES_ROM_has_save_RAM()   { return g_ROM.header.flags_1 & MASK_HAS_SAVE_RAM; }
boolean  NES_ROM_has_trainer()    { return g_ROM.header.flags_1 & MASK_HAS_TRAINER;  }
boolean  NES_ROM_is_VSUnisystem() { return g_ROM.header.flags_2 & 0x01;              }

uint8 NES_ROM_get_num_16k_ROM_banks() { return g_ROM.header.num_16k_rom_banks; }
uint8 NES_ROM_get_num_8k_VROM_banks() { return g_ROM.header.num_8k_vrom_banks; }

uint8* NES_ROM_get_trainer()    { return g_ROM.trainer;     }
uint8* NES_ROM_get_ROM_banks()  { return g_ROM.ROM_banks;   }
uint8* NES_ROM_get_VROM_banks() { return g_ROM.VROM_banks;  }

const char* NES_ROM_GetRomName() { return g_ROM.rom_name; }
const char* NES_ROM_GetRomPath() { return g_ROM.rom_path; }

uint8 NES_ROM_get_screen_mode() { return g_ROM.screen_mode; }
void NES_ROM_set_screen_mode(int Mode) { g_ROM.screen_mode = Mode; }
char *NES_ROM_get_GameTitleName(){return g_ROM.GameTitle; }

uint32 NES_ROM_crc32()       { return g_ROM.crc;  }
uint32 NES_ROM_crc32_all()       { return g_ROM.crc_all;  }
uint32 NES_ROM_fds_id()	{ return g_ROM.fds; }

// for Best Play - Pro Yakyuu Special
uint32 NES_ROM_get_size_SaveRAM() { return g_ROM.size_SaveRAM; }

uint32 NES_ROM_get_UNIF_psize_16k() { return g_ROM.unif_psize_16k;  }
uint32 NES_ROM_get_UNIF_csize_8k() { return g_ROM.unif_csize_8k;  }


void NES_ROM_GetPathInfo(const char* fn)
{
	_strcpy( g_ROM.rom_path, fn );
	NES_PathRemoveFileSpec( g_ROM.rom_path );
	NES_PathAddBackslash( g_ROM.rom_path );

	_strcpy( g_ROM.rom_name, PathFindFileName( fn ) );
	NES_PathRemoveExtension( g_ROM.rom_name );
}


void NES_ROM_GetROMInfoStr(char *h){
	char headerflag[5],headerflag2[5];
	int i, j;
	unsigned char *th = (unsigned char *)&g_ROM.header;
//	h[0x10]= g_ROM.dbcorrect[0], h[0x11]= g_ROM.dbcorrect[1], h[0x12]= g_ROM.dbcorrect[2];
	for(i=0,j=1; i<4; ++i, j<<=1){
		if(th[6] & j)
			headerflag[i]='1';
		else
			headerflag[i]='0';
		if(th[0x11] & j)
			headerflag2[i]='1';
		else
			headerflag2[i]='0';
	}
	headerflag[4]=0, headerflag2[4]=0;
/*	if(g_ROM.dbcorrect[0]){
		wsprintf(h, " Mapper [ %u -> %u ], PROM %uKB, CROM %uKB, FLAG[ %s -> %s ], PROM g_ROM.crc 0x%08X, ROM g_ROM.crc 0x%08X", (g_ROM.dbcorrect[1] >> 4)|(g_ROM.dbcorrect[2] & 0xF0),(th[6] >> 4)|(th[7] & 0xF0), th[4]*16, th[5]*8, headerflag2, headerflag, g_ROM.crc, g_ROM.crc_all);
	}
	else{
		wsprintf(h, " Mapper [ %u ], PROM %uKB, CROM %uKB, FLAG %s, PROM g_ROM.crc 0x%08X, ROM g_ROM.crc 0x%08X", (th[6] >> 4)|(th[7] & 0xF0), th[4]*16, th[5]*8, headerflag, g_ROM.crc, g_ROM.crc_all);
	}*/
}
