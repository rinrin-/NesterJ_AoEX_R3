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

#ifndef _NES_H_
#define _NES_H_

#include "cpu/nes_6502.h"
#include "nes_mapper.h"
#include "nes_rom.h"
#include "apu/nes_apu_wrapper.h"

#include "libsnss/libsnss.h"

#include "cheat.h"
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

enum {
	NES_NUM_VBLANK_LINES = 20,
	NES_NUM_FRAME_LINES = 240,

	// these are 0-based, and actions occur at start of line
	NES_NMI_LINE = 241,
	NES_VBLANK_FLAG_SET_LINE = 241,
	NES_VBLANK_FLAG_RESET_LINE = 261,
	NES_SPRITE0_FLAG_RESET_LINE = 261,

	// Draw pixel buffer width pitch
	NES_BUFFER_WIDTH_PITCH = 512,

// Palette indexÇÃÉxÅ[ÉXÇ0Ç…ÇµÇΩÇŸÇ§Ç™â¡éZèàóùÇ™ñ≥Ç≠Ç»ÇÈÇÃÇ≈çÇë¨âªÇ≥ÇÍÇÈ ruka
	NES_COLOR_BASE = 0x00, // NES palette is set starting at color 0x00
//	NES_COLOR_BASE = 0x40, // NES palette is set starting at color 0x40 (64)
	NES_NUM_COLORS = 64    // 64 colors in the NES palette
};

// â∫ãLíl2Ç¬ÇïœçXÇ∑ÇÈÇ∆é~Ç‹ÇÈÉ\ÉtÉgÇ™èoÇƒÇ≠ÇÈÇÃÇ≈íçà” ruka
#define CYCLES_SHIFT  15	// bit shift count
// ((float)((double)1364.0/(double)12.0)) << CYCLES_SHIFT;
// Ç¬Ç‹ÇË 113.6666666... * (2^CYCLES_SHIFT) ÇÃílÇÉZÉbÉgÇ∑ÇÈ
#define CYCLES_DEFAULT_CYCLES_PER_LINE 3724629

typedef struct _NES {

	uint32 CYCLES_PER_LINE;				// CYCLES_SHIFT
	uint32 CYCLES_BEFORE_NMI;			// CYCLES_SHIFT
	boolean BANKSWITCH_PER_TILE;
	boolean DPCM_IRQ;
	// frame-IRQ
	uint8 frame_irq_enabled;
	uint8 frame_irq_disenabled;
	uint8 NES_RGB_pal[NES_NUM_COLORS][3];

	struct _NES_mapper* mapper;

//	NES_settings settings;

	boolean is_frozen;

	uint8 nes_type;		// 1 = NTSC, 2 = PAL

	uint32 ideal_cycle_count;  // CYCLES_SHIFT // number of cycles that should have executed so far
	uint32 emulated_cycle_count; //CYCLES_SHIFT // number of cycles that have executed so far

	// internal memory
	uint8 RAM[0x800];
	uint8 SaveRAM[0x10000];

	uint8 mapper_extram[0x10000];
	uint32 mapper_extramsize;

	// joypad stuff
	unsigned char *pad1,*pad2,*pad3,*pad4;

	boolean  pad_strobe;
	uint8 pad1_bits;
	uint8 pad2_bits;

	//  uint8 mic_bits;
	uint8 *mic_bits;
	uint8 *coin_bits;


	uint8 pad3_count;
	uint8 pad4_count;
	uint8 pad3_bits;
	uint8 pad4_bits;
	uint8 pad3_bitsnes;
	uint8 pad4_bitsnes;


	// network joypad stuff
	uint8 net_pad1_bits;
	uint8 net_pad2_bits;
	uint8 net_past_pad1_bits;
	uint8 net_past_pad2_bits;
	uint8 net_past_disk_side;
	uint8 net_syncframe;

	uint8 net_pad3_bits;
	uint8 net_pad4_bits;
	uint8 net_past_pad3_bits;
	uint8 net_past_pad4_bits;

	// Disk System
	uint8 disk_side_flag;

	// Game Genie
	uint8 genie_num;
	uint32 genie_code[256];
	
	//Game Cheat
	//ÃÌº”
	CHEATINFO *pCheatInfo;
	uint8 cheat_num;

	// VS-Unisystem
	uint8 vs_palette[192];
	uint8 use_vs_palette;
	uint8 pad_swap;
	uint8 vstopgun_ppu;
	uint8 vstopgun_value;

	int	NES_scanline;
}NES;

extern NES g_NES; // é¿ëÃÇÕ NES.cpp

extern uint8 NES_preset_palette[11][64][3];


NES *NES_Init(const char* ROM_name);

void NES_set_pad(unsigned char* c);

boolean NES_emulate_frame(boolean draw);
boolean NES_emulate_NTSC_frame(boolean draw);
boolean NES_emulate_PAL_frame(boolean draw);

void NES_reset(unsigned char);

//const char* NES_getROMname() {return NES_ROM_GetRomName();}
//const char* NES_getROMpath() {return NES_ROM_GetRomPath();}
#define NES_getROMname() NES_ROM_GetRomName()
#define NES_getROMpath() NES_ROM_GetRomPath()

boolean NES_loadState(const char* fn);
boolean NES_saveState(const char* fn);

//void NES_GetCPUContext(unsigned char *context){ NES6502_GetContext((nes6502_context*)context);}
//void NES_SetCPUContext(unsigned char *context){ NES6502_SetContext((nes6502_context*)context);}
#define NES_GetCPUContext(context) NES6502_GetContext((nes6502_context*)context)
#define NES_SetCPUContext(context) NES6502_SetContext((nes6502_context*)context)

static inline uint8 NES_GetScreenMode() {return g_NES.nes_type;}

static inline void NES_SetScreenMode(uint8 mode) { g_NES.nes_type = mode;}

void NES_GetROMInfoStr(char *wt);
//void NES_GetGameTitleName(char *s){ strcpy(s, NES_ROM_get_GameTitleName()); };
#define NES_GetGameTitleName(s) strcpy(s, NES_ROM_get_GameTitleName())

void NES_freeze();
void NES_thaw();
boolean NES_frozen();

void NES_calculate_palette();

//uint8 NES_getBGColor() { return NES_PPU_getBGColor(); }
#define NES_getBGColor() NES_PPU_getBGColor()
void  NES_ppu_rgb();


//uint32 NES_crc32() { return NES_ROM_crc32(); }
//uint32 NES_crc32_all() { return NES_ROM_crc32_all(); }
//uint32 NES_fds_id() { return NES_ROM_fds_id(); }
#define NES_crc32() NES_ROM_crc32()
#define NES_crc32_all() NES_ROM_crc32_all()
#define NES_fds_id() NES_ROM_fds_id()

// Disk System
uint8 NES_GetDiskSideNum();
uint8 NES_GetDiskSide();
void NES_SetDiskSide(uint8 side);
uint8 NES_DiskAccessed();

// Game Genie
static inline uint8 NES_GetGenieCodeNum() { return g_NES.genie_num; }
static inline uint32 NES_GetGenieCode(uint8 num) { return g_NES.genie_code[num]; }

// Game cheat
// ÃÌº”
static inline uint8 NES_GetCheatCodeNum() { return g_NES.cheat_num; }
static inline int	NES_GetScanline()	{ return g_NES.NES_scanline ;}

// SaveRAM control
static inline void  NES_WriteSaveRAM(uint32 addr, uint8 data) { g_NES.SaveRAM[addr] = data;}
static inline uint8 NES_ReadSaveRAM(uint32 addr) { return g_NES.SaveRAM[addr]; }

void NES_emulate_CPU_cycles(uint32 num_cycles);

boolean NES_loadROM(const char* fn);
void NES_freeROM();

// these are called by the CPU
uint8 NES_MemoryRead(uint32 addr);
void  NES_MemoryWrite(uint32 addr, uint8 data);

// internal read/write functions
static inline uint8 NES_ReadRAM(uint32 addr) {return g_NES.RAM[addr & 0x7FF];};
static inline void  NES_WriteRAM(uint32 addr, uint8 data) {g_NES.RAM[addr & 0x7FF] = data;};

uint8 NES_ReadLowRegs(uint32 addr);
void  NES_WriteLowRegs(uint32 addr, uint8 data);

uint8 NES_ReadHighRegs(uint32 addr);
void  NES_WriteHighRegs(uint32 addr, uint8 data);

void  NES_trim_cycle_counts();

// file stuff
void NES_Save_SaveRAM();
void NES_Load_SaveRAM();
void NES_Save_Disk();
void NES_Load_Disk();

int NES_Load_Genie(const char *szLastGeniePath);
int NES_Load_Cheat(const char *szLastGeniePath);

//Davex: Functions for rewind
int get_mem_save_snss_size(void);
int load_state_from_mem(unsigned char *stor);
int save_state_to_mem(unsigned char *stor, int save_size);

#ifdef __cplusplus
}
#endif

#endif
