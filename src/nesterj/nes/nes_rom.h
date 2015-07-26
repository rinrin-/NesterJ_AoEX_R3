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
*/

#ifndef _NES_ROM_H_
#define _NES_ROM_H_

//#include <stdio.h>

#include "types.h"
//#include "nes_types.h"
#include "ppu/nes_ppu.h"
//#include "NES_settings.h"

#define MAX_ROM_PATH 512			// rom name len
#define MAX_ROM_BANK (64*(16*1024)*3)	// ROM BANK 3M
#define MAX_VROM_BANK (64*(8*1024)*3*2)	// VROM BANK 3M

enum {
	TRAINER_ADDRESS = 0x7000,
	TRAINER_LEN     = 512
};

enum {
	MASK_VERTICAL_MIRRORING = 0x01,
	MASK_HAS_SAVE_RAM       = 0x02,
	MASK_HAS_TRAINER        = 0x04,
	MASK_4SCREEN_MIRRORING  = 0x08
};

#ifdef __cplusplus
extern "C" {
#endif

// ROM header
typedef struct _NES_ROM_header
{
	unsigned char id[3]; // 'NES'
	unsigned char ctrl_z; // control-z
	unsigned char num_16k_rom_banks;
	unsigned char num_8k_vrom_banks;
	unsigned char flags_1;
	unsigned char flags_2;
	unsigned char reserved[8];
} NES_header;

typedef struct _NES_ROM
{
	unsigned char dbcorrect[3];
	NES_header header;
	uint8 mapper;
	uint32 size_SaveRAM;
	uint32 crc, crc_all, fds;
	//  uint32 crc, fds0, fds1;
	uint8 screen_mode;
	uint8 unif_mapper;
	char  unif_board[MAX_PATH];

	uint8 trainer[TRAINER_LEN];
	uint8 ROM_banks[MAX_ROM_BANK];
	uint8 VROM_banks[MAX_VROM_BANK];

	uint32 unif_psize_16k;
	uint32 unif_csize_8k;

	char rom_name[MAX_ROM_PATH]; // filename w/out ".NES"
	char rom_path[MAX_PATH]; // rom file path
	char GameTitle[MAX_PATH];

}NES_ROM;

extern NES_ROM g_ROM; // ŽÀ‘Ì‚Ínes_rom.c

NES_ROM *NES_ROM_LoadRom(const char* fn);

mirroring_type NES_ROM_get_mirroring();

uint8 NES_ROM_get_mapper_num();
uint8 NES_ROM_get_unifmapper_num();
int NES_ROM_get_unifBoardID();

boolean  NES_ROM_has_save_RAM();
boolean  NES_ROM_has_trainer();
boolean  NES_ROM_is_VSUnisystem();

uint8 NES_ROM_get_num_16k_ROM_banks();
uint8 NES_ROM_get_num_8k_VROM_banks();

uint8* NES_ROM_get_trainer();
uint8* NES_ROM_get_ROM_banks();
uint8* NES_ROM_get_VROM_banks();

const char* NES_ROM_GetRomName();
const char* NES_ROM_GetRomPath();

uint8 NES_ROM_get_screen_mode();
void NES_ROM_set_screen_mode(int Mode);
void NES_ROM_GetROMInfoStr(char *);
char *NES_ROM_get_GameTitleName();

uint32 NES_ROM_crc32();
uint32 NES_ROM_crc32_all();
uint32 NES_ROM_fds_id();

// for Best Play - Pro Yakyuu Special
uint32 NES_ROM_get_size_SaveRAM();

uint32 NES_ROM_get_UNIF_psize_16k();
uint32 NES_ROM_get_UNIF_csize_8k();

void NES_ROM_GetPathInfo(const char* fn);

#ifdef __cplusplus
}
#endif

/*
 .NES file format
---------------------------------------------------------------------------
0-3      String "NES^Z" used to recognize .NES files.
4        Number of 16kB ROM banks.
5        Number of 8kB VROM banks.
6        bit 0     1 for vertical mirroring, 0 for horizontal mirroring
         bit 1     1 for battery-backed RAM at $6000-$7FFF
         bit 2     1 for a 512-byte trainer at $7000-$71FF
         bit 3     1 for a four-screen VRAM layout
         bit 4-7   Four lower bits of ROM Mapper Type.
7        bit 0-3   Reserved, must be zeroes!
         bit 4-7   Four higher bits of ROM Mapper Type.
8-15     Reserved, must be zeroes!
16-...   ROM banks, in ascending order. If a trainer is present, its
         512 bytes precede the ROM bank contents.
...-EOF  VROM banks, in ascending order.
---------------------------------------------------------------------------
*/

#endif
