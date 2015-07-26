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

#include "nes.h"
#include "nes_rom.h"
#include "ppu/nes_ppu.h"
#include "pixmap.h"
#include "snss.h"
#include "../screenmanager.h"
#include "../ulunzip.h"
#include "nes_config.h"
#include "nes_string.h"
#include "../fileio.h"

#include "../debug/debug.h"

#include <stdio.h>
#include "cheat.h"

uint8 NES_preset_palette[11][64][3] =
{{
    // include the NES palette
	#include "nes_pal.h"
},
{
    // include the Nintendulator palette
	#include "nintendulator_pal.h"
},
{
    // include AspiringSquire's Real palette(>>>nibbles27)
	#include "asqrealc_pal.h"
},
{
     // include Loopy's palette
	#include "loopy_pal.h"
},
{
    // include Quor's palette
	#include "quor_pal.h"
},
{
    // include Chris Covell's palette
	#include "chriscovell_pal.h"
},
{
    // include Matt Conte's palette
	#include "mattconte_pal.h"
},
{
    // include PasoFami/99 palette
	#include "pasofami_pal.h"
},
{
    // include CrashMan's palette
	#include "crashman_pal.h"
},
{
    // include MESS palette(<<<)
	#include "mess_pal.h"
},
{
    // include the Nintendulator palette in sepia tones(davex)
	#include "nintendulator_pal_sepia.h"
}};

NES g_NES; // NES 幚懱

NES *NES_Init(const char* ROM_name)
{
	DEBUG("nester - NES emulator by Darren Ranalli, (c) 2000");
	// Param init...
	g_NES.mapper_extramsize = 0;
	g_NES.mapper = NULL;

	DEBUG("NES_APU_Init...");

	NES_APU_Init();
	DEBUG("NES_loadROM...");
	if (!NES_loadROM(ROM_name)) {
		DEBUG("NES_ROM_LoadRom ERROR");
		return NULL;
	}

	// set up palette and assert it
	g_NES.use_vs_palette = 0;
	NES_calculate_palette();
	DEBUG("Scr_AssertPalette...");
	Scr_AssertPalette(g_NES.NES_RGB_pal);

	g_NES.pad3_count=0, g_NES.pad4_count=0;
	g_NES.is_frozen = FALSE;
	return &g_NES;
}

void NES_set_pad(unsigned char* c)
{
	g_NES.pad1 = c; g_NES.pad2 = c+1; g_NES.pad3 = c+2; g_NES.pad4 = c+3;
	g_NES.mic_bits = c+4; g_NES.coin_bits = c+5;
}

boolean NES_loadROM(const char* fn)
{
	long crc;

	DEBUG("NES_APU_Init...");
	NES_APU_Init();

	// if working?
	if (g_NES.mapper) {
		NES_freeROM();
	}

	// init genie
	g_NES.genie_num = 0;
	g_NES.cheat_num = 0;

	if(g_NES.pCheatInfo)
	{
		CHEATINFO *pNext = g_NES.pCheatInfo;
		CHEATINFO* pCurrent;
		while(1)
		{
			if(pNext==NULL)
				break;
			
			pCurrent = pNext->pNext;
			free(pNext);
			pNext = pCurrent;
		}
		g_NES.pCheatInfo = NULL;
	}

	DEBUG("NES_ROM_LoadRom");
	if (!NES_ROM_LoadRom(fn)) {
		DEBUG("NES_ROM_LoadRom ERROR");
		return FALSE;
	}

	// set up the mapper
	crc = NES_crc32();
	#include "nes_set_cycles.cpp"
	g_NES.mapper = GetMapper();
	if(!g_NES.mapper) {
		DEBUGVALUE("unsupported mapper", NES_ROM_get_mapper_num());
		return FALSE;
	}
#ifdef NESTER_DEBUG
	DEBUGVALUE("mapper", NES_ROM_get_mapper_num());
	switch(NES_ROM_get_mirroring())
	{
	case NES_PPU_MIRROR_HORIZ:
		DEBUG("H ");
		break;
	case NES_PPU_MIRROR_VERT:
		DEBUG("V ");
		break;
	case NES_PPU_MIRROR_FOUR_SCREEN:
		DEBUG("F ");
		break;
	}

	if(NES_ROM_has_save_RAM())
	{
		DEBUG("S ");
	}
	if(NES_ROM_has_trainer())
	{
		DEBUG("T ");
	}

	LOG(16*NES_ROM_get_num_16k_ROM_banks() << "K/" << 8*NES_ROM_get_num_8k_VROM_banks() << "K " << endl);
#endif

	// load datas to save it at the top of reset()
	NES_Load_SaveRAM();
	NES_Load_Disk();

	Scr_SetScreenMode(NES_ROM_get_screen_mode());
	if(g_NESConfig.preferences.TV_Mode){
		g_NES.nes_type = g_NESConfig.preferences.TV_Mode;
		NES_ROM_set_screen_mode(g_NES.nes_type);
	}
	else
	{
		g_NES.nes_type = NES_ROM_get_screen_mode();
		NES_ROM_set_screen_mode(g_NES.nes_type);
	}
	NES_reset(0);
	DEBUG("Starting emulation...");
	return TRUE;
}

void NES_freeROM()
{
	NES_Save_SaveRAM();
	NES_Save_Disk();

	if(g_NES.pCheatInfo)
	{
		CHEATINFO *pNext = g_NES.pCheatInfo;
		CHEATINFO* pCurrent;
		while(1)
		{
			if(pNext==NULL)
				break;

			pCurrent = pNext->pNext;
			free(pNext);
			pNext = pCurrent;
		}
		g_NES.pCheatInfo = NULL;
	}

	g_NES.cheat_num = 0;

	DEBUG("Freeing ROM...");

	g_NES.mapper = NULL;
	Scr_ClearScreen(0x00);
	Scr_BltScreen();
	DEBUG("Done");
}

void NES_reset(unsigned char softreset)
{
	long crc;
	DEBUG("NES_reset in");
	// save SaveRAM
	NES_Save_SaveRAM();
	NES_Save_Disk();

	if(!softreset || NES_ROM_get_mapper_num()==20){
		DEBUG("clear RAM");
		// clear RAM
		_memset(g_NES.RAM, 0x00, sizeof(g_NES.RAM));
	}

	// set up CPU
	{
		nes6502_context context;
		DEBUG("set up CPU");

		_memset((void*)&context, 0x00, sizeof(context));
		NES6502_GetContext(&context);

		context.mem_page[0] = g_NES.RAM;
		context.mem_page[3] = g_NES.SaveRAM;

		NES6502_SetContext(&context);
	}

	DEBUG("NES_PPU_reset");
	// reset the PPU
	NES_PPU_reset();

	// VROM write protect
	g_PPU.vram_write_protect = NES_ROM_get_num_8k_VROM_banks() ? 1 : 0;
	g_PPU.vram_size = 0x2000;

	DEBUG("NES_APU_reset");
	// reset the APU
	NES_APU_reset();

	g_NES.frame_irq_enabled = 0xFF;
	g_NES.frame_irq_disenabled = 0;

	if(g_NES.mapper)
	{
		DEBUG("reset the mapper");
		// reset the mapper
		g_NES.mapper->Reset();
	}

	DEBUG("NES6502_Reset");
	// reset the CPU
	NES6502_Reset();



	DEBUG("NES_Load_SaveRAM");
	// load SaveRAM
	NES_Load_SaveRAM();
	DEBUG("NES_Load_Disk");
	NES_Load_Disk();

	// set up the trainer if present
	if(NES_ROM_has_trainer())
	{
		DEBUG("trainer is located at 0x7000; SaveRAM is 0x2000 bytes at 0x6000");
		// trainer is located at 0x7000; SaveRAM is 0x2000 bytes at 0x6000
		_memcpy(&g_NES.SaveRAM[0x1000], NES_ROM_get_trainer(), TRAINER_LEN);
	}
	crc = NES_crc32();
  #include "nes_set_vspalette.h"

	g_NES.ideal_cycle_count  = 0;
	g_NES.emulated_cycle_count = 0;

	g_NES.disk_side_flag = 0;

	// pad1 & pad2 reset
	g_NES.net_pad1_bits = 0;
	g_NES.net_pad2_bits = 0;
	g_NES.net_past_pad1_bits = 0;
	g_NES.net_past_pad2_bits = 0;
	g_NES.net_past_disk_side = 0;
	g_NES.net_syncframe = 0;

	g_NES.pad_strobe = FALSE;
	g_NES.pad1_bits = 0;
	g_NES.pad2_bits = 0;

	g_NES.pad3_bits=0;
	g_NES.pad4_bits=0;
	g_NES.pad3_bitsnes=0;
	g_NES.pad4_bitsnes=0;
	DEBUG("NES_reset out");
}

boolean NES_emulate_frame(boolean draw)
{
	//作弊代码
	static int i = 0;
	if( (++i)==20)//每隔约1/3秒写一次内存
	{
		i = 0;	
		g_NESmapper.set_cheat();
	}
	
	if(1==NES_ROM_get_screen_mode()) //g_NESConfig.graphics.show_all_scanlines
		return NES_emulate_NTSC_frame(draw);
	else
		return NES_emulate_PAL_frame(draw);
}



boolean NES_emulate_NTSC_frame(boolean draw)
{
	uint32 i;
	pixmap p;
	uint8* cur_line = (uint8*)(uint32)draw; // ptr to screen buffer
//	boolean retval = draw;

	//测试
	g_NES.NES_scanline = 0;

	NES_trim_cycle_counts();

	// do frame
	NES_PPU_start_frame();

	if(cur_line) {
		if(!Scr_Lock(&p))
			cur_line = NULL;
		else
			cur_line = p.data;
	}

	// LINES 0-7(Don't draw)
	for(i = 0; i < 8; i++) {
		// do one line's worth of CPU cycles
		//NES_emulate_CPU_cycles(g_NES.CYCLES_PER_LINE);
		//mapper->HSync(i);

		if(cur_line) {
			// Bankswitch per line Support for Mother
			if(g_NES.BANKSWITCH_PER_TILE) {
				// render line
				NES_PPU_do_scanline_and_draw(cur_line, g_NES.CYCLES_PER_LINE * 32 / 42);
				// do half line's worth of CPU cycles (hblank)
				NES_emulate_CPU_cycles(13 << CYCLES_SHIFT);
				g_NESmapper.HSync(i);
				NES_emulate_CPU_cycles(g_NES.CYCLES_PER_LINE * 10 / 42 - (13 << CYCLES_SHIFT));
				if(i == 0)
				{
					NES_emulate_CPU_cycles(g_NES.CYCLES_PER_LINE * 32 / 42 + (13 << CYCLES_SHIFT));
					g_NESmapper.HSync(i);
					NES_emulate_CPU_cycles(g_NES.CYCLES_PER_LINE * 10 / 42 - (13 << CYCLES_SHIFT));
				}
			}
			else {
				// do one line's worth of CPU cycles
				NES_emulate_CPU_cycles(g_NES.CYCLES_PER_LINE);
				g_NESmapper.HSync(i);
				// render line
				NES_PPU_do_scanline_and_dont_draw();
			}
			// point to next line
			cur_line += NES_BUFFER_WIDTH_PITCH;
		}
		else {
			// do one line's worth of CPU cycles
			NES_emulate_CPU_cycles(g_NES.CYCLES_PER_LINE);
			g_NESmapper.HSync(i);
			NES_PPU_do_scanline_and_dont_draw();
		}

		g_NES.NES_scanline = i;
	}

	// LINES 8-231
	for(; i < NES_NUM_FRAME_LINES-8; i++) {
		// do one line's worth of CPU cycles
		//NES_emulate_CPU_cycles(g_NES.CYCLES_PER_LINE);
		//mapper->HSync(i);

		if(cur_line) {
			// Bankswitch per line Support for Mother
			if(g_NES.BANKSWITCH_PER_TILE) {
				// render line
				NES_PPU_do_scanline_and_draw(cur_line, g_NES.CYCLES_PER_LINE * 32 / 42);
				// do half line's worth of CPU cycles (hblank)
				NES_emulate_CPU_cycles(13 << CYCLES_SHIFT);
				g_NESmapper.HSync(i);
				NES_emulate_CPU_cycles(g_NES.CYCLES_PER_LINE * 10 / 42 - (13 << CYCLES_SHIFT));
			}
			else {
				// do one line's worth of CPU cycles
				NES_emulate_CPU_cycles(g_NES.CYCLES_PER_LINE);
				g_NESmapper.HSync(i);
				// render line
				NES_PPU_do_scanline_and_draw(cur_line, 0);
			}
			// point to next line
			cur_line += NES_BUFFER_WIDTH_PITCH;
		}
		else {
			// do one line's worth of CPU cycles
			NES_emulate_CPU_cycles(g_NES.CYCLES_PER_LINE);
			g_NESmapper.HSync(i);
			NES_PPU_do_scanline_and_dont_draw();
		}

		g_NES.NES_scanline = i;
	}

	// LINES 232-239
	for(; i < NES_NUM_FRAME_LINES; i++) {
		// do one line's worth of CPU cycles
		//NES_emulate_CPU_cycles(g_NES.CYCLES_PER_LINE);
		//mapper->HSync(i);

		if(cur_line) {
			// Bankswitch per line Support for Mother
			if(g_NES.BANKSWITCH_PER_TILE) {
				// render line
				NES_PPU_do_scanline_and_draw(cur_line, g_NES.CYCLES_PER_LINE * 32 / 42);
				// do half line's worth of CPU cycles (hblank)
				NES_emulate_CPU_cycles(13 << CYCLES_SHIFT);
				g_NESmapper.HSync(i);
				NES_emulate_CPU_cycles(g_NES.CYCLES_PER_LINE * 10 / 42 - (13 << CYCLES_SHIFT));
			}
			else {
				// do one line's worth of CPU cycles
				NES_emulate_CPU_cycles(g_NES.CYCLES_PER_LINE);
				g_NESmapper.HSync(i);
				// render line
				NES_PPU_do_scanline_and_dont_draw();
			}
			// point to next line
			cur_line += NES_BUFFER_WIDTH_PITCH;
		}
		else {
			// do one line's worth of CPU cycles
			NES_emulate_CPU_cycles(g_NES.CYCLES_PER_LINE);
			g_NESmapper.HSync(i);
			NES_PPU_do_scanline_and_dont_draw();
		}

		g_NES.NES_scanline = i;
	}

	if(cur_line) {
		Scr_Unlock();
	}

	NES_PPU_end_frame();

	// fram_IRQ
	if(!(g_NES.frame_irq_enabled & 0xC0)) {
		NES6502_DoPendingIRQ();
	}

	for(i = 240; i <= 261; i++) {
	    if(i == 261) {
	      NES_PPU_end_vblank();
	    }
		else if(i == 241) {
			// do v-blank
			NES_PPU_start_vblank();
			g_NESmapper.VSync();

			// 1 instruction between vblank flag and NMI
			NES_emulate_CPU_cycles(g_NES.CYCLES_BEFORE_NMI);
			if(NES_PPU_NMI_enabled()) NES6502_DoNMI();
			NES_emulate_CPU_cycles(g_NES.CYCLES_PER_LINE - g_NES.CYCLES_BEFORE_NMI);
			g_NESmapper.HSync(i);
			continue;
		}

		NES_emulate_CPU_cycles(g_NES.CYCLES_PER_LINE);
		g_NESmapper.HSync(i);

		g_NES.NES_scanline = i;
	}
	// HALF-LINE 262.5
	//NES_emulate_CPU_cycles(g_NES.CYCLES_PER_LINE/2);

	NES_APU_DoFrame();
	NES_APU_SyncAPURegister();

	return cur_line ? 1: 0;
}

boolean NES_emulate_PAL_frame(boolean draw)
{
	uint32 i;
	pixmap p;
	uint8* cur_line = (uint8*)(uint32)draw; // ptr to screen buffer
//	boolean retval = draw;

	g_NES.NES_scanline = 0;

	NES_trim_cycle_counts();

	// do frame
	NES_PPU_start_frame();

	if(cur_line) {
		if(!Scr_Lock(&p))
			cur_line = NULL;
		else
			cur_line = p.data;
	}

	// LINES 0-7
	for(i = 0; i < 8; i++) {
		if(cur_line) {
			// do one line's worth of CPU cycles
			NES_emulate_CPU_cycles(g_NES.CYCLES_PER_LINE);
			g_NESmapper.HSync(i);
			// render line
			NES_PPU_do_scanline_and_dont_draw();
			cur_line += NES_BUFFER_WIDTH_PITCH;
		}
		else {
			// do one line's worth of CPU cycles
			NES_emulate_CPU_cycles(g_NES.CYCLES_PER_LINE);
			g_NESmapper.HSync(i);
			NES_PPU_do_scanline_and_dont_draw();
		}

		g_NES.NES_scanline = i;
	}

	// LINES 8-231
	for(; i < 240-8; i++) {
		if(cur_line) {
			// do one line's worth of CPU cycles
			NES_emulate_CPU_cycles(g_NES.CYCLES_PER_LINE);
			g_NESmapper.HSync(i);
			// render line
			NES_PPU_do_scanline_and_draw(cur_line, 0);
			cur_line += NES_BUFFER_WIDTH_PITCH;
		}
		else {
			// do one line's worth of CPU cycles
			NES_emulate_CPU_cycles(g_NES.CYCLES_PER_LINE);
			g_NESmapper.HSync(i);
			NES_PPU_do_scanline_and_dont_draw();
		}

		g_NES.NES_scanline = i;
	}

	// LINES 232-239
	for(; i < 240; i++) {
		if(cur_line) {
			// do one line's worth of CPU cycles
			NES_emulate_CPU_cycles(g_NES.CYCLES_PER_LINE);
			g_NESmapper.HSync(i);
			// render line
			NES_PPU_do_scanline_and_dont_draw();
			cur_line += NES_BUFFER_WIDTH_PITCH;
		}
		else {
			// do one line's worth of CPU cycles
			NES_emulate_CPU_cycles(g_NES.CYCLES_PER_LINE);
			g_NESmapper.HSync(i);
			NES_PPU_do_scanline_and_dont_draw();
		}

		g_NES.NES_scanline = i;
	}

	if(cur_line)
	{
		Scr_Unlock();
	}

	NES_PPU_end_frame();

	// fram_IRQ
	if(!(g_NES.frame_irq_enabled & 0xC0))
	{
		NES6502_DoPendingIRQ();
	}

	for(i = 240; i <= 311; i++)
	{
		if(i == 311)
		{
			NES_PPU_end_vblank();
		}
		else if(i == 241)
		{
			// do v-blank
			NES_PPU_start_vblank();
			g_NESmapper.VSync();
			// 1 instruction between vblank flag and NMI
			NES_emulate_CPU_cycles(g_NES.CYCLES_BEFORE_NMI);
			if(NES_PPU_NMI_enabled()) NES6502_DoNMI();
			NES_emulate_CPU_cycles(g_NES.CYCLES_PER_LINE - g_NES.CYCLES_BEFORE_NMI);
			g_NESmapper.HSync(i);
			continue;
		}

		NES_emulate_CPU_cycles(g_NES.CYCLES_PER_LINE);
		g_NESmapper.HSync(i);

		g_NES.NES_scanline = i;
	}

	NES_APU_DoFrame();
	NES_APU_SyncAPURegister();

	return cur_line ? 1: 0;
}



void NES_freeze()
{
	NES_APU_freeze();
	g_NES.is_frozen = TRUE;
}

void NES_thaw()
{
	NES_APU_thaw();
	g_NES.is_frozen = FALSE;
}

boolean NES_frozen()
{
	return g_NES.is_frozen;
}

uint8 NES_MemoryRead(uint32 addr)
{
	//  LOG("Read " << HEX(addr,4) << endl);

	if(addr < 0x2000) // RAM
	{
		return NES_ReadRAM(addr);
	}
	else if(addr < 0x4000) // low registers
	{
		return NES_ReadLowRegs(addr);
	}
	else if(addr < 0x4018) // high registers
	{
		return NES_ReadHighRegs(addr);
	}
	else if(addr < 0x6000) // mapper low
	{
		//    LOG("MAPPER LOW READ: " << HEX(addr,4) << endl);
		//    return((uint8)(addr >> 8)); // what's good for conte is good for me
		return g_NESmapper.MemoryReadLow(addr);
	}
	else // save RAM, or ROM (mapper 40)
	{
		g_NESmapper.MemoryReadSaveRAM(addr);
		return NES6502_GetByte(addr);
	}
}

void NES_MemoryWrite(uint32 addr, uint8 data)
{
	//  LOG("Write " << HEX(addr,4) << " " << HEX(data,2) << endl);

	if(addr < 0x2000) // RAM
	{
		NES_WriteRAM(addr, data);
	}
	else if(addr < 0x4000) // low registers
	{
		NES_WriteLowRegs(addr, data);
	}
	else if(addr < 0x4018) // high registers
	{
		NES_WriteHighRegs(addr, data);
		g_NESmapper.WriteHighRegs(addr, data);
	}
	else if(addr < 0x6000) // mapper low
	{
		g_NESmapper.MemoryWriteLow(addr, data);
	}
	else if(addr < 0x8000) // save RAM
	{
		g_NES.SaveRAM[addr - 0x6000] = data;
		g_NESmapper.MemoryWriteSaveRAM(addr, data);
	}
	else // mapper
	{
		g_NESmapper.MemoryWrite(addr, data);
	}
}

uint8 NES_ReadLowRegs(uint32 addr)
{
	//return NES_PPU_ReadLowRegs(addr & 0xE007);
	if(g_NES.vstopgun_ppu)
	{
		if(addr == 0x2002)
		{
			return NES_PPU_ReadLowRegs(addr & 0x2002) | 0x1b;
		}
	}
	return NES_PPU_ReadLowRegs(addr & 0xE007);
}

void NES_WriteLowRegs(uint32 addr, uint8 data)
{
	if(g_NES.vstopgun_ppu)
	{
		if(addr == 0x2000)
		{
			g_NES.vstopgun_value = data & 0x7F;
			NES_PPU_WriteLowRegs(0x2000, (uint8)(data & 0x7F));
			return;
		}
		if(addr == 0x2001)
		{
			NES_PPU_WriteLowRegs(0x2000, (uint8)(g_NES.vstopgun_value | (data & 0x80)));
			NES_PPU_WriteLowRegs(0x2001, data);
			return;
		}
	}
	if (addr & 0xFFFF0000) {
		addr = 123;
	}
	NES_PPU_WriteLowRegs(addr & 0xE007, data);
}


uint8 NES_ReadHighRegs(uint32 addr)
{
	if(addr == 0x4014) // SPR-RAM DMA
	{
#ifdef NESTER_DEBUG
		LOG("Read from SPR-RAM DMA reg??" << endl);
#endif
		return NES_PPU_Read0x4014();
	}
	else if(addr == 0x4015 && !(g_NES.frame_irq_enabled & 0xC0)) // frame_IRQ
	{
		return NES_APU_Read(0x4015) | 0x40;
	}
	else if(addr < 0x4016) // APU
	{
		//    LOG("APU READ:" << HEX(addr,4) << endl);
		return NES_APU_Read(addr);
	}
	else // joypad regs
	{
		uint8 retval=0;

		if(addr == 0x4016)
		{
			{
				if(g_NES.pad3_count<8){
					// joypad 1
					retval |=  g_NES.pad1_bits & 0x01;
					g_NES.pad1_bits >>= 1;

					// joypad 3 (Famicom)
					retval |=  (g_NES.pad3_bits & 0x01)<<1;
					g_NES.pad3_bits >>= 1;
				}
				else if(g_NES.pad3_count<16){		//NES 4play (joypad 3)
					retval |=  (g_NES.pad3_bitsnes & 0x01);
					g_NES.pad3_bitsnes >>= 1;
				}
//				else if(pad3_count==19)		//NES 4play adapter info?
//					retval |= 1;

				// mic on joypad 2
				retval |= *g_NES.mic_bits;
				retval |= *g_NES.coin_bits;

				retval |= 0x40;		// ?
			}
			//	  LOG("4016READ: " << HEX(addr,4) << "  "<< HEX(pad3_count,2)<<"  "<< HEX(retval,2)<< endl);
			++g_NES.pad3_count;
		}
		else if(addr == 0x4017)
		{
			{
				if(g_NES.pad4_count<8){		// joypad 2
					retval |=  (g_NES.pad2_bits & 0x01);
					g_NES.pad2_bits >>= 1;
					//joypad 4 (Famicom)
					retval |= ((g_NES.pad4_bits & 0x01) <<1);
					g_NES.pad4_bits >>= 1;
				}
				else if(g_NES.pad4_count<16){		//NES 4play (joypad 4)
					retval |=  (g_NES.pad4_bitsnes & 0x01);
					g_NES.pad4_bitsnes >>= 1;
				}
//				else if(pad4_count==18)		//NES 4play adapter info?
//					retval |= 1;
			}
			//	  LOG("4017READ: " << HEX(addr,4) << "  "<< HEX(pad4_count,2)<<"  "<< HEX(retval,2)<< endl);
			++g_NES.pad4_count;
		}
		return retval;
	}
}

void NES_WriteHighRegs(uint32 addr, uint8 data)
{
	if(addr == 0x4014) // SPR-RAM DMA
	{
		NES_PPU_Write0x4014(data);
		NES6502_SetDMA(514);		//CYCLES_PER_DMA
	}
	else if(addr < 0x4016) // APU
	{
		NES_APU_Write(addr, data);
	}
	else if(addr == 0x4016) // joy pad
	{
		// bit 0 == joypad strobe
		if(data & 0x01)
		{
			g_NES.pad_strobe = TRUE;
		}
		else
		{
			if(g_NES.pad_strobe)
			{
				g_NES.pad_strobe = FALSE;
				g_NES.pad3_count=0, g_NES.pad4_count=0;		//Pad read counter clear

				// get input states
				g_NES.pad1_bits = *g_NES.pad1;
				g_NES.pad2_bits = *g_NES.pad2;
				g_NES.pad3_bitsnes = g_NES.pad3_bits = *g_NES.pad3;
				g_NES.pad4_bitsnes = g_NES.pad4_bits = *g_NES.pad4;

				// MIC, Insert Coin
				if(*g_NES.mic_bits){
					g_NES.pad2_bits = 0x00;
				}
				if(*g_NES.coin_bits){
					*g_NES.coin_bits = 0x20;
				}

				// swap pad_bits for VS Super Sky Kid, VS Dr.Mario
				if(g_NES.pad_swap == 1)
				{
					g_NES.pad_swap = g_NES.pad1_bits;
					g_NES.pad1_bits = (g_NES.pad1_bits & 0x0C) | (g_NES.pad2_bits & 0xF3);
					g_NES.pad2_bits = (g_NES.pad2_bits & 0x0C) | (g_NES.pad_swap & 0xF3);
					g_NES.pad_swap = 1;
				}
				// swap pad_bits for VS Pinball (Alt)
				if(g_NES.pad_swap == 2)
				{
					g_NES.pad_swap = g_NES.pad1_bits;
					g_NES.pad1_bits = (g_NES.pad1_bits & 0xFD) | ((g_NES.pad2_bits & 0x01) << 1);
					g_NES.pad2_bits = (g_NES.pad2_bits & 0xFE) | ((g_NES.pad_swap & 0x02) >> 1);
					g_NES.pad_swap = 2;
				}
				// swap pad_bits for Nintendo World Championship (#105)
				if(NES_crc32() == 0x0b0e128f)
				{
					g_NES.pad2_bits |= g_NES.pad1_bits & 0x08;
				}
			}
		}
	}
	else if(addr == 0x4017) // frame-IRQ
	{
		if(!g_NES.frame_irq_disenabled)
		{
			g_NES.frame_irq_enabled = data;
		}
		NES_APU_Write(addr, data);
	}
}

//#include "NES_external_device.cpp"

void NES_emulate_CPU_cycles(uint32 num_cycles)
{
	int32 cycle_deficit;

	g_NES.ideal_cycle_count += num_cycles;

	if(g_NES.ideal_cycle_count > g_NES.emulated_cycle_count) {
		cycle_deficit = (uint32)(g_NES.ideal_cycle_count - g_NES.emulated_cycle_count) >> CYCLES_SHIFT;
		g_NES.emulated_cycle_count += (NES6502_Execute(cycle_deficit) << CYCLES_SHIFT);
		if(NES_APU_SyncDMCRegister(cycle_deficit) && g_NES.DPCM_IRQ)
		{
			NES6502_DoPendingIRQ();
		}
	}
}


// call every once in a while to avoid cycle count overflow
void NES_trim_cycle_counts()
{
	uint32 trim_amount;

	trim_amount = g_NES.ideal_cycle_count;
	if(trim_amount > g_NES.emulated_cycle_count) trim_amount = g_NES.emulated_cycle_count;

	g_NES.ideal_cycle_count  -= trim_amount;
	g_NES.emulated_cycle_count -= trim_amount;
}

void NES_Save_SaveRAM()
{
	uint32 i;
	// does the ROM use save ram?
	if(!NES_ROM_has_save_RAM()) return;

	// has anything been written to Save RAM?
	for(i = 0; i < sizeof(g_NES.SaveRAM); i++)
	{
		if(g_NES.SaveRAM[i] != 0x00) break;
	}
	if(i < sizeof(g_NES.SaveRAM))
	{
		int hFile = -1;
		char fn[MAX_PATH], *extp;
		uint8 flag=0;

		DEBUG("Saving Save RAM...");
		GetModulePath(fn, sizeof(fn));
		_strcat(fn, "SAVE/");
		_strcat(fn, NES_ROM_GetRomName());
		_strcat(fn, ".sav");
		extp = _strrchr(fn, '.');
		// zsv
		{
			char inpfn[MAX_PATH];
			_strcpy(extp, ".zsv");
			_strcpy(inpfn, NES_ROM_GetRomName());
			_strcat(inpfn, ".sav");
			if(UsezipLibMemToZip(g_NES.SaveRAM, NES_ROM_get_size_SaveRAM(), fn, inpfn)) {
				flag = 1;
			}
			else {
				DEBUG("UsezipLibMemToZip Failed");
			}
		}

		// sav
		if (!flag) {
			_strcpy(extp, ".sav");
			DEBUG("Save RAM");
			DEBUG(fn);
			hFile = NES_fopen(fn, FILE_MODE_WRITE);
			if(hFile < 0) {
				DEBUG("can't open save RAM file");
			}
			else {
				if(NES_fwrite(g_NES.SaveRAM, 1, NES_ROM_get_size_SaveRAM(), hFile) != NES_ROM_get_size_SaveRAM()) {
					DEBUG("can't open save RAM file");
					NES_fclose(hFile);
					NES_DeleteFile(fn);
					return;
				}
				NES_fclose(hFile);
				DEBUG("Done.");
			}
		}
		else {
			// zsv曐懚偟偨偺偱sav僼傽僀儖偼嶍彍偡傞
			_strcpy(extp, ".sav");
			NES_DeleteFile(fn);
		}
	}
}

void NES_Load_SaveRAM()
{
	_memset(g_NES.SaveRAM, 0x00, sizeof(g_NES.SaveRAM));

	// does the ROM use save ram?
	if(!NES_ROM_has_save_RAM()) return;

	{
		int hFile = -1;
		char fn[MAX_PATH], *extp;
		uint8 flag = 0;

		GetModulePath(fn, sizeof(fn));
		_strcat(fn, "SAVE/");
		_strcat(fn, NES_ROM_GetRomName());
		_strcat(fn, ".sav");
		extp = _strrchr(fn, '.');
		// zsv
		{
			const char *savextentions[] = { "sav", NULL};
			unsigned long rdw;
			_strcpy(extp, ".zsv");
			if(UseUnzipLibExeLoad_P(fn, g_NES.SaveRAM, 0, &rdw, savextentions)){
				flag = 1;
			}
			else {
				DEBUG("UseUnzipLibExeLoad_P Failed");
			}
		}

		// sav
		if (!flag) {
			_strcpy(extp, ".sav");
			DEBUG("Load RAM");
			DEBUG(fn);
			hFile = NES_fopen(fn, FILE_MODE_READ);
			if(hFile < 0) {
				DEBUG("none found.");
			}
			else {
				LOG("Loading Save RAM...");
				if(NES_fread(g_NES.SaveRAM, 1, NES_ROM_get_size_SaveRAM(), hFile) != NES_ROM_get_size_SaveRAM())
				{
					DEBUG("error reading Save RAM file");
					_memset(g_NES.SaveRAM, 0x00, sizeof(g_NES.SaveRAM));
				}
				NES_fclose(hFile);
				DEBUG("Done.");
			}
		}
	}
}

void NES_Save_Disk()
{
	// must not save before load disk image to disk[] in mapper reset
	if (!g_NES.mapper) {
		return;
	}
	uint8 *diskp = g_NES.mapper->GetDiskDatap();
	if(!diskp)
		return;
	if(diskp[0] == 0x01)
	{
		unsigned char headbuff[0x10] = {'F','D','S',0x1a};
		int hFile = -1;
		char fn[MAX_PATH], *extp;
		uint8 flag = 0;

		headbuff[4] = g_NES.mapper->GetDiskSideNum();
		GetModulePath(fn, sizeof(fn));
		_strcat(fn, "SAVE/");
		_strcat(fn, NES_ROM_GetRomName());
		_strcat(fn, ".sdk");
		extp = _strrchr(fn, '.');
		// zsv
		{
			// rom load 帪偺僶僢僼傽傪棳梡
			extern uint8 g_ReadRomImage[];
			char inpfn[MAX_PATH];
			unsigned char *p = g_ReadRomImage;
			_strcpy(extp, ".zsv");
			_strcpy(inpfn, NES_ROM_GetRomName());
			_strcat(inpfn, ".fds");
			uint32 sn = g_NES.mapper->GetDiskSideNum();
			sn=sn*65500+0x10;
//			_memset(p, 0x00, sn);
			_memcpy(p, headbuff, 0x10);
			_memcpy(p+0x10, diskp, sn-0x10);
			if(UsezipLibMemToZip(p, sn, fn, inpfn)) {
				flag=1;
			}
			else {
				DEBUG("UsezipLibMemToZip Failed");
			}
		}

		// sdk
		if (!flag) {
			_strcpy(extp, ".sdk");
			hFile = NES_fopen(fn, FILE_MODE_WRITE);
			if(hFile < 0){
				DEBUG("can't open save disk file")
			}
			else
			{
				uint8 sn = g_NES.mapper->GetDiskSideNum();
				NES_fwrite(headbuff, 0x10, 1, hFile);
				if (NES_fwrite(diskp, 65500*sn, 1, hFile) != 65500*sn) {
					// 僙乕僽幐攕側偺偱嶍彍
					NES_fclose(hFile);
					NES_DeleteFile(fn);
					return;
				}
				NES_fclose(hFile);
			}
		}
		else {
			// zsv曐懚偟偨偺偱sdk僼傽僀儖偼嶍彍偡傞
			_strcpy(extp, ".sdk");
			NES_DeleteFile(fn);
		}
	}
}


void NES_Load_Disk()
{
	// must not load before load disk image to disk[] in mapper reset
	uint8 *diskp = g_NES.mapper->GetDiskDatap();

	if(!diskp)
		return;
	if(diskp[0] == 0x01)
	{
		int hFile = -1;
		char fn[256], *extp;
		uint8 flag =0;

		GetModulePath(fn, sizeof(fn));
		_strcat(fn, "SAVE/");
		_strcat(fn, NES_ROM_GetRomName());
		_strcat(fn, ".sdk");
		extp = _strrchr(fn, '.');
		// zsv
		{
			const char *savextentions[] = { "fds", NULL};
			extern uint8 g_ReadRomImage[];
			unsigned long rdw;
			unsigned char *sdp = g_ReadRomImage;
			_strcpy(extp, ".zsv");
			if(UseUnzipLibExeLoad_P(fn, sdp, 0, &rdw, savextentions)){
				_memcpy(diskp, sdp+0x10, rdw-0x10);
				flag = 1;
			}
			else {
				DEBUG("UseUnzipLibExeLoad Failed");
			}
		}

		// sdk
		if (!flag) {
			hFile = NES_fopen(fn, FILE_MODE_READ);
			if(hFile < 0){
				DEBUG("none found.");
			}
			else {
				int d0 = NES_fgetc(hFile);
				int d1 = NES_fgetc(hFile);
				int d2 = NES_fgetc(hFile);
				int d3 = NES_fgetc(hFile);

				if(d0 == 'F' && d1 == 'D' && d2 == 'S' && d3 == 0x1a)
				{
					uint32 sn= g_NES.mapper->GetDiskSideNum();
					// new disk save format
					NES_fseek(hFile, 16, FILE_SEEK_SET);
					NES_fread(diskp, sn*65500, 1, hFile);
				}
				else {
					DEBUG("error.");
				}
				NES_fclose(hFile);
			}
		}
	}
}


//>>>davex: wrappers around memory state save
int get_mem_save_snss_size(void){
	unsigned char *s1=NULL;
	int size_save = MemSaveSNSS(&s1, &g_NES);
	return size_save;
}

int load_state_from_mem(unsigned char *stor){
	return MemLoadSNSS(stor, &g_NES);
}
int save_state_to_mem(unsigned char *stor, int save_size){
	return save_snss(stor, &g_NES, save_size);
}
//<<<


boolean NES_loadState(const char* fn)
{
	uint8 flag = 1;
	// zs? file
	if (flag) {
		// rom load 帪偺僶僢僼傽傪棳梡
		extern uint8 g_ReadRomImage[];
		unsigned char *stated=g_ReadRomImage;
		unsigned long rdw;
		const char *savextentions[] = { "std", NULL};
		char ssfn[MAX_PATH], *p;
		_strcpy(ssfn, fn);
		p = _strrchr(ssfn, '.');
		*++p = 'z';
		DEBUG(ssfn);
		if(UseUnzipLibExeLoad_P((char *)ssfn, stated, 0, &rdw, savextentions)){
			DEBUG("UseUnzipLibExeLoad_P succeeded");
			MemLoadSNSS(stated, &g_NES);
		}
		else{
			flag = 0;
		}
	}
	if (!flag) {
		// ss? file
		return LoadSNSS(fn, &g_NES);
	}
	return flag;
}

boolean NES_saveState(const char* fn)
{
	uint8 flag = 0;
	// zs? file
	unsigned char *s1=NULL;
	if(MemSaveSNSS(&s1, &g_NES)){
		char inpfn[MAX_PATH], ssfn[MAX_PATH], *p;
		uint32 sn;
		_strcpy(ssfn, fn);
		p = _strrchr(ssfn, '.');
		*++p = 'z';
		_strcpy(inpfn, NES_ROM_GetRomName());
		_strcat(inpfn, ".std");
		sn = (*(uint32 *)&s1[4])+0x20;
		if(UsezipLibMemToZip(s1, sn, ssfn, inpfn))
			flag=1;
	}
	if(!flag) {
		// ss? file
		return SaveSNSS(fn, &g_NES);
	}
	else {
		// zs?曐懚偟偨偺偱ss?僼傽僀儖偼嶍彍偡傞
		NES_DeleteFile(fn);
	}
	return flag;
}

void NES_calculate_palette()
{
	if( g_NES.use_vs_palette ){
		_memcpy(g_NES.NES_RGB_pal, g_NES.vs_palette, sizeof(g_NES.NES_RGB_pal));
	}
	else{
		_memcpy(g_NES.NES_RGB_pal, NES_preset_palette[g_NESConfig.graphics.DefaultPaletteNo], sizeof(g_NES.NES_RGB_pal));
	}
#if 0
	else
	{
		if( use_vs_palette )
		{
			_memcpy(g_NES.NES_RGB_pal, g_NES.vs_palette, sizeof(g_NES.NES_RGB_pal));
		}
		else
		{
			_memcpy(g_NES.NES_RGB_pal, NES_preset_palette[g_NESConfig.graphics.DefaultPaletteNo], sizeof(g_NES.NES_RGB_pal));
		}
	}
#endif
	if(NES_PPU_rgb_pal())
	{
		int i;
		for(i = 0; i < NES_NUM_COLORS; i++)
		{
			switch(NES_PPU_rgb_pal())
			{
			case 0x20:
				g_NES.NES_RGB_pal[i][1] = ((int)(g_NES.NES_RGB_pal[i][1] * 80) /100);
				g_NES.NES_RGB_pal[i][2] = ((int)(g_NES.NES_RGB_pal[i][2] * 73) /100);
				break;
			case 0x40:
				g_NES.NES_RGB_pal[i][0] = ((int)(g_NES.NES_RGB_pal[i][0] * 73) /100);
				g_NES.NES_RGB_pal[i][2] = ((int)(g_NES.NES_RGB_pal[i][2] * 70) /100);
				break;
			case 0x60:
				g_NES.NES_RGB_pal[i][0] = ((int)(g_NES.NES_RGB_pal[i][0] * 76) /100);
				g_NES.NES_RGB_pal[i][1] = ((int)(g_NES.NES_RGB_pal[i][1] * 78) /100);
				g_NES.NES_RGB_pal[i][2] = ((int)(g_NES.NES_RGB_pal[i][2] * 58) /100);
				break;
			case 0x80:
				g_NES.NES_RGB_pal[i][0] = ((int)(g_NES.NES_RGB_pal[i][0] * 86) /100);
				g_NES.NES_RGB_pal[i][1] = ((int)(g_NES.NES_RGB_pal[i][1] * 80) /100);
				break;
			case 0xA0:
				g_NES.NES_RGB_pal[i][0] = ((int)(g_NES.NES_RGB_pal[i][0] * 83) /100);
				g_NES.NES_RGB_pal[i][1] = ((int)(g_NES.NES_RGB_pal[i][1] * 68) /100);
				g_NES.NES_RGB_pal[i][2] = ((int)(g_NES.NES_RGB_pal[i][2] * 85) /100);
				break;
			case 0xC0:
				g_NES.NES_RGB_pal[i][0] = ((int)(g_NES.NES_RGB_pal[i][0] * 67) /100);
				g_NES.NES_RGB_pal[i][1] = ((int)(g_NES.NES_RGB_pal[i][1] * 77) /100);
				g_NES.NES_RGB_pal[i][2] = ((int)(g_NES.NES_RGB_pal[i][2] * 83) /100);
				break;
			case 0xE0:
				g_NES.NES_RGB_pal[i][0] = ((int)(g_NES.NES_RGB_pal[i][0] * 68) /100);
				g_NES.NES_RGB_pal[i][1] = ((int)(g_NES.NES_RGB_pal[i][1] * 68) /100);
				g_NES.NES_RGB_pal[i][2] = ((int)(g_NES.NES_RGB_pal[i][2] * 68) /100);
				break;
			}
		}
	}

	if(g_NESConfig.graphics.black_and_white)
	{
		int i;

		for(i = 0; i < NES_NUM_COLORS; i++)
		{
			uint8 Y;

			Y = (uint8)(((g_NES.NES_RGB_pal[i][0] * 299) + (g_NES.NES_RGB_pal[i][1] * 587) + (g_NES.NES_RGB_pal[i][2] * 114))  / 1000);
//			Y = (uint8)(((float)g_NES.NES_RGB_pal[i][0] * 0.299) +
//			            ((float)g_NES.NES_RGB_pal[i][1] * 0.587) +
//			            ((float)g_NES.NES_RGB_pal[i][2] * 0.114));
			g_NES.NES_RGB_pal[i][0] = Y;
			g_NES.NES_RGB_pal[i][1] = Y;
			g_NES.NES_RGB_pal[i][2] = Y;
		}
	}
}

void NES_ppu_rgb()
{
	NES_calculate_palette();
	Scr_AssertPalette(g_NES.NES_RGB_pal);
}

uint8 NES_GetDiskSideNum()
{
	if (g_NES.mapper) {
		return g_NES.mapper->GetDiskSideNum();
	}
	return 0;
}

uint8 NES_GetDiskSide()
{
	if (g_NES.mapper) {
		return g_NES.mapper->GetDiskSide();
	}
	return 0;
}

void NES_SetDiskSide(uint8 side)
{
	if (g_NES.mapper) {
		g_NES.disk_side_flag = side;// | 0x10;
		g_NES.mapper->SetDiskSide((uint8)(g_NES.disk_side_flag & 0x0F));
	}
}

uint8 NES_DiskAccessed()
{
	if (g_NES.mapper) {
		return g_NES.mapper->DiskAccessed();
	}
	return FALSE;
}

int LoadCheatFile(const char *pszFilename)
{
	FILE* fp = fopen(pszFilename, ("rt"));//打开文件
	if (fp == NULL) {
		return 1;//出错
	}

	int nLen;
	int i, j, n = 0;
	char tmp[32];
	char szLine[1024];
	
	//内存计数器
	long iMem = 0;

	CHEATINFO* pCurrentCheat = NULL;

	while (1) 
	{
		if( iMem>=20000 )
			break;//内存要不足了
		
		if (fgets(szLine, 1024, fp) == NULL)//读一行
			break;

		nLen = _strlen(szLine);//得到一行字符的长度

		if (nLen < 3 || szLine[0] == '[')//继续读下一行
			continue;

		//检查作弊码，防止死机
		for(i=0,j=0; i<1024; i++)
		{
			if(szLine[i]==0)
			{
				fclose (fp);
				return 0;
			}
			
			if(szLine[i]=='=')			
			{
				j=1;break;
			}
		}
		if(j==0)
			continue;

		if (!_strncmp ("Name=", szLine, 5)) //取得名称
		{
			n = 0;

			CHEATINFO* pPreviousCheat = pCurrentCheat;
			pCurrentCheat = (CHEATINFO*)malloc(sizeof(CHEATINFO));
			iMem++;
			g_NES.cheat_num++;
			if (g_NES.pCheatInfo == NULL) 
			{
				g_NES.pCheatInfo = pCurrentCheat;
			}

			_memset(pCurrentCheat, 0, sizeof(CHEATINFO));
			pCurrentCheat->pPrevious = pPreviousCheat;
			if (pPreviousCheat) 
			{
				pPreviousCheat->pNext = pCurrentCheat;
			}

			// Fill in defaults
			pCurrentCheat->nStatus =  0;						// Disable cheat
			pCurrentCheat->nDefault = 0;						// Set default option

			_strncpy (pCurrentCheat->szCheatName, szLine + 5, QUOTE_MAX);//取得名字
			pCurrentCheat->szCheatName[nLen-6] = '\0';

			continue;
		}

		if (!_strncmp ("Default=", szLine, 8) && n >= 0) 
		{
			_strncpy (tmp, szLine + 8, nLen-9);
			tmp[nLen-9] = '\0';
			sscanf (tmp, "%d", &(pCurrentCheat->nDefault));
			continue;
		}

		i = 0, j = 0;
		while (i < nLen) 
		{
			if(n>=(CHEAT_MAX_OPTIONS-1))
				break;//限制
						
			if (szLine[i] == '=' && i < 4)
				j = i+1;

			if (szLine[i] == ',' || szLine[i] == '\n') 
			{
				if(pCurrentCheat==NULL)
				{
					fclose (fp);
					return 0;
				}
				if (pCurrentCheat->pOption[n] == NULL) {
					pCurrentCheat->pOption[n] = (CHEATOPTION*)malloc(sizeof(CHEATOPTION));
					iMem++;
				}
				_memset(pCurrentCheat->pOption[n], 0, sizeof(CHEATOPTION));

				_strncpy (pCurrentCheat->pOption[n]->szOptionName, szLine + j, QUOTE_MAX * sizeof(char));
				pCurrentCheat->pOption[n]->szOptionName[i-j] = '\0';

				i++;
				j = i;
				break;
			}
			i++;
		}

		int nAddress = -1;
		int nValue = 0;
		int nCurrentAddress = 0;

		while (nCurrentAddress < CHEAT_MAX_ADDRESS) 
		{
			if(n>=(CHEAT_MAX_OPTIONS-1))
				break;//限制
			if (i == nLen) break;

			if (szLine[i] == ',' || szLine[i] == '\n') {
				_strncpy (tmp, szLine + j, i-j);
				tmp[i-j] = '\0';

				if (nAddress == -1) {
					sscanf (tmp, "%x", &nAddress);
				} else {
					sscanf (tmp, "%x", &nValue);

					nAddress = (nAddress&0xFFFF) <<8;//显示FC内存地址范围
					pCurrentCheat->pOption[n]->AddressInfo[nCurrentAddress] = nAddress;
					pCurrentCheat->pOption[n]->AddressInfo[nCurrentAddress] += nValue&0xFF;
					nCurrentAddress++;

					nAddress = -1;
					nValue = 0;
				}
				j = i+1;
			}
			i++;
		}
		n++;
	}

	fclose (fp);
	//添加，组成循环双向链表
	//g_NES.pCheatInfo->pPrevious = pCurrentCheat;
	//pCurrentCheat->pNext = g_NES.pCheatInfo;

	return 0;
}

void NES_GetROMInfoStr(char *wh)
{
	NES_ROM_GetROMInfoStr(wh);
}

int NES_Load_Cheat(const char *szLastGeniePath)
{
	if(g_NES.pCheatInfo)
	{
		CHEATINFO *pNext = g_NES.pCheatInfo;
		CHEATINFO* pCurrent;
		while(1)
		{
			if(pNext==NULL)
				break;
			
			pCurrent = pNext->pNext;
			free(pNext);
			pNext = pCurrent;
		}
		g_NES.pCheatInfo = NULL;
	}
	g_NES.cheat_num = 0;
	
	LoadCheatFile(szLastGeniePath);
	return 1;
}


int NES_Load_Genie(const char *szLastGeniePath)
{
	int hFile;
	int c;

	g_NES.genie_num = 0;

#define EOF (-1)

	hFile = NES_fopen(szLastGeniePath, FILE_MODE_READ);
	if(hFile < 0) return 0;
	while((c = NES_fgetc(hFile)) != EOF)
	{
		uint8 code[9], i, p = 0;
		_memset(code, 0x00, sizeof(code));
		code[0] = c;
		if(!(c == 0x0D || c == 0x0A || c == EOF))
		{
			for(;;)
			{
				c = NES_fgetc(hFile);
				if(c == 0x0D || c == 0x0A || c == EOF) break;
				p++;
				if(p < 8) code[p] = c;
			}
		}		

		if(	g_NES.genie_num==256 )
			break;
		
		for(i = 0; i < 9; i++)
		{
			switch(code[i])
			{
			case 'A': code[i] = 0x00; break;
			case 'P': code[i] = 0x01; break;
			case 'Z': code[i] = 0x02; break;
			case 'L': code[i] = 0x03; break;
			case 'G': code[i] = 0x04; break;
			case 'I': code[i] = 0x05; break;
			case 'T': code[i] = 0x06; break;
			case 'Y': code[i] = 0x07; break;
			case 'E': code[i] = 0x08; break;
			case 'O': code[i] = 0x09; break;
			case 'X': code[i] = 0x0A; break;
			case 'U': code[i] = 0x0B; break;
			case 'K': code[i] = 0x0C; break;
			case 'S': code[i] = 0x0D; break;
			case 'V': code[i] = 0x0E; break;
			case 'N': code[i] = 0x0F; break;
			default:  p = i; i = 9; break;
			}
		}
		if(p == 6)
		{
			uint32 addr = 0x0000,data = 0x0000;
			// address
			addr |= (code[3] & 0x4) ? 0x4000 : 0x0000;
			addr |= (code[3] & 0x2) ? 0x2000 : 0x0000;
			addr |= (code[3] & 0x1) ? 0x1000 : 0x0000;
			addr |= (code[4] & 0x8) ? 0x0800 : 0x0000;
			addr |= (code[5] & 0x4) ? 0x0400 : 0x0000;
			addr |= (code[5] & 0x2) ? 0x0200 : 0x0000;
			addr |= (code[5] & 0x1) ? 0x0100 : 0x0000;
			addr |= (code[1] & 0x8) ? 0x0080 : 0x0000;
			addr |= (code[2] & 0x4) ? 0x0040 : 0x0000;
			addr |= (code[2] & 0x2) ? 0x0020 : 0x0000;
			addr |= (code[2] & 0x1) ? 0x0010 : 0x0000;
			addr |= (code[3] & 0x8) ? 0x0008 : 0x0000;
			addr |= (code[4] & 0x4) ? 0x0004 : 0x0000;
			addr |= (code[4] & 0x2) ? 0x0002 : 0x0000;
			addr |= (code[4] & 0x1) ? 0x0001 : 0x0000;
			// value
			data |= (code[0] & 0x8) ? 0x0080 : 0x0000;
			data |= (code[1] & 0x4) ? 0x0040 : 0x0000;
			data |= (code[1] & 0x2) ? 0x0020 : 0x0000;
			data |= (code[1] & 0x1) ? 0x0010 : 0x0000;
			data |= (code[5] & 0x8) ? 0x0008 : 0x0000;
			data |= (code[0] & 0x4) ? 0x0004 : 0x0000;
			data |= (code[0] & 0x2) ? 0x0002 : 0x0000;
			data |= (code[0] & 0x1) ? 0x0001 : 0x0000;
			g_NES.genie_code[g_NES.genie_num] = (addr << 16) | data;
			g_NES.genie_num++;
		}
		else if(p == 8)
		{
			uint32 addr = 0x0000,data = 0x0000;
			// address
			addr |= (code[3] & 0x4) ? 0x4000 : 0x0000;
			addr |= (code[3] & 0x2) ? 0x2000 : 0x0000;
			addr |= (code[3] & 0x1) ? 0x1000 : 0x0000;
			addr |= (code[4] & 0x8) ? 0x0800 : 0x0000;
			addr |= (code[5] & 0x4) ? 0x0400 : 0x0000;
			addr |= (code[5] & 0x2) ? 0x0200 : 0x0000;
			addr |= (code[5] & 0x1) ? 0x0100 : 0x0000;
			addr |= (code[1] & 0x8) ? 0x0080 : 0x0000;
			addr |= (code[2] & 0x4) ? 0x0040 : 0x0000;
			addr |= (code[2] & 0x2) ? 0x0020 : 0x0000;
			addr |= (code[2] & 0x1) ? 0x0010 : 0x0000;
			addr |= (code[3] & 0x8) ? 0x0008 : 0x0000;
			addr |= (code[4] & 0x4) ? 0x0004 : 0x0000;
			addr |= (code[4] & 0x2) ? 0x0002 : 0x0000;
			addr |= (code[4] & 0x1) ? 0x0001 : 0x0000;
			// value
			data |= (code[0] & 0x8) ? 0x0080 : 0x0000;
			data |= (code[1] & 0x4) ? 0x0040 : 0x0000;
			data |= (code[1] & 0x2) ? 0x0020 : 0x0000;
			data |= (code[1] & 0x1) ? 0x0010 : 0x0000;
			data |= (code[7] & 0x8) ? 0x0008 : 0x0000;
			data |= (code[0] & 0x4) ? 0x0004 : 0x0000;
			data |= (code[0] & 0x2) ? 0x0002 : 0x0000;
			data |= (code[0] & 0x1) ? 0x0001 : 0x0000;
			// compare value
			data |= (code[6] & 0x8) ? 0x8000 : 0x0000;
			data |= (code[7] & 0x4) ? 0x4000 : 0x0000;
			data |= (code[7] & 0x2) ? 0x2000 : 0x0000;
			data |= (code[7] & 0x1) ? 0x1000 : 0x0000;
			data |= (code[5] & 0x8) ? 0x0800 : 0x0000;
			data |= (code[6] & 0x4) ? 0x0400 : 0x0000;
			data |= (code[6] & 0x2) ? 0x0200 : 0x0000;
			data |= (code[6] & 0x1) ? 0x0100 : 0x0000;
			g_NES.genie_code[g_NES.genie_num] = (addr << 16) | data | 0x80000000;
			g_NES.genie_num++;
		}
	}
	NES_fclose(hFile);
	return g_NES.genie_num;
}

