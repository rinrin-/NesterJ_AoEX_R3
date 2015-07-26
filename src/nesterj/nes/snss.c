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

#include "types.h"
#include "../debug/debug.h"
#include "snss.h"
#include "cpu/nes6502.h"
#include "nes.h"
#include "nes_string.h"

#include "libsnss/libsnss.h"

// these functions apply a SNSS block to the current emulated NES

static void adopt_BASR(SnssBaseBlock* block, NES* nes)
{
	// BASR - Base Registers
	nes6502_context context;

	NES6502_GetContext(&context);

	context.a_reg = block->regA;
	context.x_reg = block->regX;
	context.y_reg = block->regY;
	context.p_reg = block->regFlags;
	context.s_reg = block->regStack;
	context.pc_reg = block->regPc;

	context.int_pending = 0;
	context.jammed = 0;
	context.burn_cycles = 0;
	//context.dma_cycles = 0;
	NES6502_SetContext(&context);

	// registers $2000 and $2001
	NES_MemoryWrite(0x2000, block->reg2000);
	NES_MemoryWrite(0x2001, block->reg2001);

	// RAM
	_memcpy(g_NES.RAM, block->cpuRam, 0x800);

	// SPR-RAM
	_memcpy(g_PPU.spr_ram, block->spriteRam, 0x100);

	// PPU $2000-$2FFF (Name Tables/Attrib Tables)
	_memcpy(g_PPU.PPU_nametables, block->ppuRam, 4*0x400);

	// palettes
	_memcpy(g_PPU.bg_pal,  &block->palette[0x00], 0x10);
	_memcpy(g_PPU.spr_pal, &block->palette[0x10], 0x10);

	// mirroring
	NES_PPU_set_mirroring((uint32)block->mirrorState[0]&0x03,
	                        (uint32)block->mirrorState[1]&0x03,
	                        (uint32)block->mirrorState[2]&0x03,
	                        (uint32)block->mirrorState[3]&0x03);

	// VRAM address
	g_PPU.loopy_t = block->vramAddress;

	// OAM (spr) address
	g_PPU.spr_ram_rw_ptr = block->spriteRamAddress;

	// tile X offset
	g_PPU.loopy_x = block->tileXOffset;
}

static void adopt_VRAM(SnssVramBlock* block, NES* nes)
{
	// VRAM

	// read MAX 32KB
	_memcpy(NES_PPU_get_patt(), &block->vram, block->vramSize);

#ifdef NESTER_DEBUG
	if(block->vramSize > 0x2000)
	{
		LOG("SNSS VRAM size greater than 8K; unsupported" << endl);
	}
#endif
}

static void adopt_SRAM(SnssSramBlock* block, NES* nes)
{
	// Save-RAM
	nes6502_context context;

	// read SRAM
	NES6502_GetContext(&context);
	_memcpy(context.mem_page[3], block->sram, (block->sramSize <= 0x2000) ? block->sramSize : 0x2000);
#ifdef NESTER_DEBUG
	if(block->sramSize > 0x2000)
	{
		LOG("SNSS SRAM size greater than 8K; unsupported" << endl);
	}
#endif
}

static void adopt_MPRD(SnssMapperBlock* block, NES* nes)
{
	uint8 i;
	// Mapper Data

	// set PRG pages
	{
		nes6502_context context;
		NES6502_GetContext(&context);
		context.mem_page[4] = NES_ROM_get_ROM_banks() + ((uint32)block->prgPages[0] << 13);
		context.mem_page[5] = NES_ROM_get_ROM_banks() + ((uint32)block->prgPages[1] << 13);
		context.mem_page[6] = NES_ROM_get_ROM_banks() + ((uint32)block->prgPages[2] << 13);
		context.mem_page[7] = NES_ROM_get_ROM_banks() + ((uint32)block->prgPages[3] << 13);
		NES6502_SetContext(&context);
	}

	// set CHR pages
	for(i = 0; i < 8; i++)
	{
		if(block->chrPages[i] & 0x8000)
		{
			// VRAM
			g_PPU.PPU_VRAM_banks[i] = NES_PPU_get_patt() + ((uint32)(block->chrPages[i] & 0x1f) << 10);
			NES_PPU_set_pattype(i, 0);
		}
		else
		{
			// VROM
			g_PPU.PPU_VRAM_banks[i] = NES_ROM_get_VROM_banks() + ((uint32)block->chrPages[i] << 10);
			NES_PPU_set_pattype(i, 1);
		}
	}

	// handle mapper-specific data
	switch(NES_ROM_get_mapper_num())
	{
	case 1:
		{
			struct mapper1Data* mapper_data = (struct mapper1Data*)&block->extraData;

			// last values written to the 4 registers
			_memcpy(g_NESmapper.Mapper1.regs, mapper_data->registers, 4);

			// latch register
			g_NESmapper.Mapper1.bits = mapper_data->latch;

			// number of bits written to unfinished reg
			g_NESmapper.Mapper1.write_count = mapper_data->numberOfBits;
		}
		break;

	case 4:
		{
			struct mapper4Data* mapper_data = (struct mapper4Data*)&block->extraData;

			int i;
			for( i = 0; i < 8; i++ ) {
				g_NESmapper.Mapper4.reg[i] = mapper_data->p[i];
			}
			g_NESmapper.Mapper4.prg0  = mapper_data->p[ 8];
			g_NESmapper.Mapper4.prg1  = mapper_data->p[ 9];
			g_NESmapper.Mapper4.chr01 = mapper_data->p[10];
			g_NESmapper.Mapper4.chr23 = mapper_data->p[11];
			g_NESmapper.Mapper4.chr4  = mapper_data->p[12];
			g_NESmapper.Mapper4.chr5  = mapper_data->p[13];
			g_NESmapper.Mapper4.chr6  = mapper_data->p[14];
			g_NESmapper.Mapper4.chr7  = mapper_data->p[15];
			g_NESmapper.Mapper4.irq_enable  = mapper_data->p[16];
			g_NESmapper.Mapper4.irq_counter = (int)mapper_data->p[17];
			g_NESmapper.Mapper4.irq_latch   = mapper_data->p[18];
			g_NESmapper.Mapper4.irq_request = mapper_data->p[19];
			g_NESmapper.Mapper4.irq_preset  = mapper_data->p[20];
			g_NESmapper.Mapper4.irq_preset_vbl = mapper_data->p[21];
		}
		break;

	case 5:
		{
			struct mapper5Data* mapper_data = (struct mapper5Data*)&block->extraData;

			g_NESmapper.Mapper5.irq_line = mapper_data->irqLineLowByte;
			g_NESmapper.Mapper5.irq_line |= mapper_data->irqLineHighByte << 8;
			g_NESmapper.Mapper5.irq_enabled = mapper_data->irqEnabled;
			g_NESmapper.Mapper5.irq_status = mapper_data->irqStatus;
			g_NESmapper.Mapper5.wram_protect0 = mapper_data->wramWriteProtect0;
			g_NESmapper.Mapper5.wram_protect1 = mapper_data->wramWriteProtect1;
			g_NESmapper.Mapper5.prg_size = mapper_data->romBankSize;
			g_NESmapper.Mapper5.chr_size = mapper_data->vromBankSize;
			g_NESmapper.Mapper5.gfx_mode = mapper_data->gfxMode;
			g_NESmapper.Mapper5.split_control = mapper_data->splitControl;
			g_NESmapper.Mapper5.split_bank = mapper_data->splitBank;
			g_NESmapper.Mapper5.value0 = mapper_data->last5205Write;
			g_NESmapper.Mapper5.value1 = mapper_data->last5206Write;
			g_NESmapper.Mapper5.wb[3] = mapper_data->wramBank3;
			g_NESmapper.Mapper5.wb[4] = mapper_data->wramBank4;
			g_NESmapper.Mapper5.wb[5] = mapper_data->wramBank5;
			g_NESmapper.Mapper5.wb[6] = mapper_data->wramBank6;
			g_NESmapper.Mapper5.chr_reg[0][0] = mapper_data->vromBank[0][0];
			g_NESmapper.Mapper5.chr_reg[1][0] = mapper_data->vromBank[1][0];
			g_NESmapper.Mapper5.chr_reg[2][0] = mapper_data->vromBank[2][0];
			g_NESmapper.Mapper5.chr_reg[3][0] = mapper_data->vromBank[3][0];
			g_NESmapper.Mapper5.chr_reg[4][0] = mapper_data->vromBank[4][0];
			g_NESmapper.Mapper5.chr_reg[5][0] = mapper_data->vromBank[5][0];
			g_NESmapper.Mapper5.chr_reg[6][0] = mapper_data->vromBank[6][0];
			g_NESmapper.Mapper5.chr_reg[7][0] = mapper_data->vromBank[7][0];
			g_NESmapper.Mapper5.chr_reg[0][1] = mapper_data->vromBank[0][1];
			g_NESmapper.Mapper5.chr_reg[1][1] = mapper_data->vromBank[1][1];
			g_NESmapper.Mapper5.chr_reg[2][1] = mapper_data->vromBank[2][1];
			g_NESmapper.Mapper5.chr_reg[3][1] = mapper_data->vromBank[3][1];
			g_NESmapper.Mapper5.chr_reg[4][1] = mapper_data->vromBank[4][1];
			g_NESmapper.Mapper5.chr_reg[5][1] = mapper_data->vromBank[5][1];
			g_NESmapper.Mapper5.chr_reg[6][1] = mapper_data->vromBank[6][1];
			g_NESmapper.Mapper5.chr_reg[7][1] = mapper_data->vromBank[7][1];
		}
		break;

	case 6:
		{
			struct mapper6Data* mapper_data = (struct mapper6Data*)&block->extraData;

			g_NESmapper.Mapper6.irq_counter = mapper_data->irqCounterLowByte;
			g_NESmapper.Mapper6.irq_counter |= mapper_data->irqCounterHighByte << 8;
			g_NESmapper.Mapper6.irq_enabled = mapper_data->irqCounterEnabled;
		}
		break;

	case 9:
		{
			struct mapper9Data* mapper_data = (struct mapper9Data*)&block->extraData;

			// 2 latch registers
			g_NESmapper.Mapper9.latch_0000 = mapper_data->latch[0];
			g_NESmapper.Mapper9.latch_1000 = mapper_data->latch[1];

			// regs (B/C/D/E000)
			g_NESmapper.Mapper9.regs[1] = mapper_data->lastB000Write;
			g_NESmapper.Mapper9.regs[2] = mapper_data->lastC000Write;
			g_NESmapper.Mapper9.regs[3] = mapper_data->lastD000Write;
			g_NESmapper.Mapper9.regs[4] = mapper_data->lastE000Write;

			NES_mapper9_SNSS_fixup();
		}
		break;

	case 10:
		{
			struct mapper10Data* mapper_data = (struct mapper10Data*)&block->extraData;

			// 2 latch registers
			g_NESmapper.Mapper10.latch_0000 = mapper_data->latch[0];
			g_NESmapper.Mapper10.latch_1000 = mapper_data->latch[1];

			// regs (B/C/D/E000)
			g_NESmapper.Mapper10.regs[1] = mapper_data->lastB000Write;
			g_NESmapper.Mapper10.regs[2] = mapper_data->lastC000Write;
			g_NESmapper.Mapper10.regs[3] = mapper_data->lastD000Write;
			g_NESmapper.Mapper10.regs[4] = mapper_data->lastE000Write;

			NES_mapper10_SNSS_fixup();
		}
		break;

	case 13:
		{
			struct mapper13Data* mapper_data = (struct mapper13Data*)&block->extraData;

			g_NESmapper.Mapper13.chr_bank = mapper_data->wramBank;

			NES_mapper13_SNSS_fixup();
		}
		break;

	case 16:
		{
			struct mapper16Data* mapper_data = (struct mapper16Data*)&block->extraData;

			g_NESmapper.Mapper16.irq_counter = mapper_data->irqCounterLowByte;
			g_NESmapper.Mapper16.irq_counter |= mapper_data->irqCounterHighByte << 8;
			g_NESmapper.Mapper16.irq_enabled = mapper_data->irqCounterEnabled;
			g_NESmapper.Mapper16.irq_latch = mapper_data->irqLatchCounterLowByte;
			g_NESmapper.Mapper16.irq_latch |= mapper_data->irqLatchCounterHighByte << 8;
		}
		break;

	case 17:
		{
			struct mapper17Data* mapper_data = (struct mapper17Data*)&block->extraData;

			g_NESmapper.Mapper17.irq_counter = mapper_data->irqCounterLowByte;
			g_NESmapper.Mapper17.irq_counter |= mapper_data->irqCounterHighByte << 8;
			g_NESmapper.Mapper17.irq_enabled = mapper_data->irqCounterEnabled;
		}
		break;

	case 18:
		{
			struct mapper18Data* mapper_data = (struct mapper18Data*)&block->extraData;

			g_NESmapper.Mapper18.irq_counter = mapper_data->irqCounterLowByte;
			g_NESmapper.Mapper18.irq_counter |= mapper_data->irqCounterHighByte << 8;
			g_NESmapper.Mapper18.irq_enabled = mapper_data->irqCounterEnabled;

			NES_mapper18_SNSS_fixup();
		}
		break;

	case 19:
		{
			struct mapper19Data* mapper_data = (struct mapper19Data*)&block->extraData;

			g_NESmapper.Mapper19.irq_counter = mapper_data->irqCounterLowByte;
			g_NESmapper.Mapper19.irq_counter |= mapper_data->irqCounterHighByte << 8;
			g_NESmapper.Mapper19.irq_enabled = mapper_data->irqCounterEnabled;
			g_NESmapper.Mapper19.regs[0] = (mapper_data->lastE800Write & 0x40) >> 6;
			g_NESmapper.Mapper19.regs[1] = (mapper_data->lastE800Write & 0x80) >> 7;
			g_NESmapper.Mapper19.regs[2] = mapper_data->lastF800Write;
		}
		break;

	case 20:
		{
			struct mapper20Data* mapper_data = (struct mapper20Data*)&block->extraData;

			g_NESmapper.Mapper20.irq_counter = mapper_data->irqCounterLowByte;
			g_NESmapper.Mapper20.irq_counter |= mapper_data->irqCounterHighByte << 8;
			g_NESmapper.Mapper20.irq_enabled = mapper_data->irqCounterEnabled;
			g_NESmapper.Mapper20.irq_latch = mapper_data->irqLatchCounterLowByte;
			g_NESmapper.Mapper20.irq_latch |= mapper_data->irqLatchCounterHighByte << 8;
			g_NESmapper.Mapper20.irq_wait = mapper_data->irqWaitCounter;
			g_NESmapper.Mapper20.disk_enabled = mapper_data->last4023Write;
			g_NESmapper.Mapper20.write_reg = mapper_data->last4025Write;
			g_NESmapper.Mapper20.head_position = mapper_data->HeadPositionLowByte;
			g_NESmapper.Mapper20.head_position |= mapper_data->HeadPositionHighByte << 8;
			g_NESmapper.Mapper20.disk_status = mapper_data->DiskStatus;
			g_NESmapper.Mapper20.write_skip = mapper_data->WriteSkip;
			g_NESmapper.Mapper20.current_side = mapper_data->CurrentDiskSide;
			g_NESmapper.Mapper20.last_side = mapper_data->LastDiskSide;
			g_NESmapper.Mapper20.insert_wait = mapper_data->DiskInsertWait;

			NES_mapper20_SNSS_fixup();
		}
		break;

	case 21:
		{
			struct mapper21Data* mapper_data = (struct mapper21Data*)&block->extraData;

			g_NESmapper.Mapper21.irq_counter = mapper_data->irqCounter;
			g_NESmapper.Mapper21.irq_enabled = mapper_data->irqCounterEnabled;
			g_NESmapper.Mapper21.irq_latch = mapper_data->irqLatchCounter;
			g_NESmapper.Mapper21.regs[8] = mapper_data->last9002Write;

			NES_mapper21_SNSS_fixup();
		}
		break;

	case 23:
		{
			struct mapper23Data* mapper_data = (struct mapper23Data*)&block->extraData;

			g_NESmapper.Mapper23.irq_counter = mapper_data->irqCounter;
			g_NESmapper.Mapper23.irq_enabled = mapper_data->irqCounterEnabled;
			g_NESmapper.Mapper23.irq_latch = mapper_data->irqLatchCounter;
			g_NESmapper.Mapper23.regs[8] = mapper_data->last9008Write;

			NES_mapper23_SNSS_fixup();
		}
		break;

	case 24:
		{
			struct mapper24Data* mapper_data = (struct mapper24Data*)&block->extraData;

			g_NESmapper.Mapper24.irq_counter = mapper_data->irqCounter;
			g_NESmapper.Mapper24.irq_enabled = mapper_data->irqCounterEnabled;
			g_NESmapper.Mapper24.irq_latch = mapper_data->irqLatchCounter;
		}
		break;

	case 25:
		{
			struct mapper25Data* mapper_data = (struct mapper25Data*)&block->extraData;

			g_NESmapper.Mapper25.irq_counter = mapper_data->irqCounter;
			g_NESmapper.Mapper25.irq_enabled = mapper_data->irqCounterEnabled;
			g_NESmapper.Mapper25.irq_latch = mapper_data->irqLatchCounter;
			g_NESmapper.Mapper25.regs[10] = mapper_data->last9001Write;

			NES_mapper25_SNSS_fixup();
		}
		break;

	case 26:
		{
			struct mapper26Data* mapper_data = (struct mapper26Data*)&block->extraData;

			g_NESmapper.Mapper26.irq_counter = mapper_data->irqCounter;
			g_NESmapper.Mapper26.irq_enabled = mapper_data->irqCounterEnabled;
			g_NESmapper.Mapper26.irq_latch = mapper_data->irqLatchCounter;
		}
		break;

	case 32:
		{
			struct mapper32Data* mapper_data = (struct mapper32Data*)&block->extraData;

			g_NESmapper.Mapper32.regs[0] = mapper_data->last9000Write;
		}
		break;

	case 33:
		{
			struct mapper33Data* mapper_data = (struct mapper33Data*)&block->extraData;

			g_NESmapper.Mapper33.irq_counter = mapper_data->irqCounter;
			g_NESmapper.Mapper33.irq_enabled = mapper_data->irqCounterEnabled;
		}
		break;

	case 40:
		{
			struct mapper40Data* mapper_data = (struct mapper40Data*)&block->extraData;

			// IRQ counter
			g_NESmapper.Mapper40.lines_to_irq = mapper_data->irqCounter;
			// IRQ enabled
			g_NESmapper.Mapper40.irq_enabled = mapper_data->irqCounterEnabled;
		}
		break;

	case 41:
		{
			struct mapper41Data* mapper_data = (struct mapper41Data*)&block->extraData;

			g_NESmapper.Mapper41.regs[0] = mapper_data->last6000Write;
		}
		break;

	case 42:
		{
			struct mapper42Data* mapper_data = (struct mapper42Data*)&block->extraData;

			g_NESmapper.Mapper42.irq_counter = mapper_data->irqCounter;
			g_NESmapper.Mapper42.irq_enabled = mapper_data->irqCounterEnabled;
		}
		break;

	case 43:
		{
			struct mapper43Data* mapper_data = (struct mapper43Data*)&block->extraData;

			g_NESmapper.Mapper43.irq_counter = mapper_data->irqCounterLowByte;
			g_NESmapper.Mapper43.irq_counter |= mapper_data->irqCounterHighByte << 8;
			g_NESmapper.Mapper43.irq_enabled = mapper_data->irqCounterEnabled;
		}
		break;

	case 46:
		{
			struct mapper46Data* mapper_data = (struct mapper46Data*)&block->extraData;

			g_NESmapper.Mapper46.regs[0] = mapper_data->last6000Write & 0x0F;
			g_NESmapper.Mapper46.regs[1] = (mapper_data->last6000Write & 0xF0) >> 4;
			g_NESmapper.Mapper46.regs[2] = mapper_data->last8000Write & 0x01;
			g_NESmapper.Mapper46.regs[3] = (mapper_data->last8000Write & 0x70) >> 4;
		}
		break;

	case 48:
		{
			struct mapper48Data* mapper_data = (struct mapper48Data*)&block->extraData;

			g_NESmapper.Mapper48.regs[0] = mapper_data->lastE000Write;
		}
		break;

	case 49:
		{
			struct mapper49Data* mapper_data = (struct mapper49Data*)&block->extraData;

			g_NESmapper.Mapper49.irq_counter = mapper_data->irqCounter;
			g_NESmapper.Mapper49.irq_latch = mapper_data->irqLatchCounter;
			g_NESmapper.Mapper49.irq_enabled = mapper_data->irqCounterEnabled;
			g_NESmapper.Mapper49.regs[0] = mapper_data->last8000Write;
			g_NESmapper.Mapper49.regs[1] = mapper_data->last6000Write;
			g_NESmapper.Mapper49.regs[2] = mapper_data->lastA001Write;

			NES_mapper49_SNSS_fixup();
		}
		break;

	case 50:
		{
			struct mapper50Data* mapper_data = (struct mapper50Data*)&block->extraData;

			g_NESmapper.Mapper50.irq_enabled = mapper_data->irqCounterEnabled;
		}
		break;

	case 51:
		{
			struct mapper51Data* mapper_data = (struct mapper51Data*)&block->extraData;

			g_NESmapper.Mapper51.bank = mapper_data->BankSelect;
			g_NESmapper.Mapper51.mode = mapper_data->MapperMode;
		}
		break;

	case 57:
		{
			struct mapper57Data* mapper_data = (struct mapper57Data*)&block->extraData;

			g_NESmapper.Mapper57.regs[0] = mapper_data->last8800Write;
		}
		break;

	case 64:
		{
			struct mapper64Data* mapper_data = (struct mapper64Data*)&block->extraData;

			g_NESmapper.Mapper64.irq_counter = mapper_data->irqCounter;
			g_NESmapper.Mapper64.irq_enabled = mapper_data->irqCounterEnabled;
			g_NESmapper.Mapper64.irq_latch = mapper_data->irqLatchCounter;
			g_NESmapper.Mapper64.regs[0] = mapper_data->last8000Write & 0x0F;
			g_NESmapper.Mapper64.regs[1] = mapper_data->last8000Write & 0x40;
			g_NESmapper.Mapper64.regs[2] = mapper_data->last8000Write & 0x80;
		}
		break;

	case 65:
		{
			struct mapper65Data* mapper_data = (struct mapper65Data*)&block->extraData;

			g_NESmapper.Mapper65.irq_counter = mapper_data->irqCounterLowByte;
			g_NESmapper.Mapper65.irq_counter |= mapper_data->irqCounterHighByte << 8;
			g_NESmapper.Mapper65.irq_enabled = mapper_data->irqCounterEnabled;
			g_NESmapper.Mapper65.irq_latch = mapper_data->irqLatchCounterLowByte;
			g_NESmapper.Mapper65.irq_latch |= mapper_data->irqLatchCounterHighByte << 8;
		}
		break;

	case 67:
		{
			struct mapper67Data* mapper_data = (struct mapper67Data*)&block->extraData;

			g_NESmapper.Mapper67.irq_counter = mapper_data->irqCounter;
			g_NESmapper.Mapper67.irq_enabled = mapper_data->irqCounterEnabled;
			g_NESmapper.Mapper67.irq_latch = mapper_data->irqLatchCounter;
		}
		break;

	case 68:
		{
			struct mapper68Data* mapper_data = (struct mapper68Data*)&block->extraData;

			g_NESmapper.Mapper68.regs[0] = (mapper_data->lastE000Write & 0x10) >> 4;
			g_NESmapper.Mapper68.regs[1] = mapper_data->lastE000Write & 0x03;
			g_NESmapper.Mapper68.regs[2] = mapper_data->lastC000Write;
			g_NESmapper.Mapper68.regs[3] = mapper_data->lastD000Write;
		}
		break;

	case 69:
		{
			struct mapper69Data* mapper_data = (struct mapper69Data*)&block->extraData;

			g_NESmapper.Mapper69.irq_counter = mapper_data->irqCounterLowByte;
			g_NESmapper.Mapper69.irq_counter |= mapper_data->irqCounterHighByte << 8;
			g_NESmapper.Mapper69.irq_enabled = mapper_data->irqCounterEnabled;
			g_NESmapper.Mapper69.regs[0] = mapper_data->last8000Write;
		}
		break;

	case 73:
		{
			struct mapper73Data* mapper_data = (struct mapper73Data*)&block->extraData;

			g_NESmapper.Mapper73.irq_counter = mapper_data->irqCounterLowByte;
			g_NESmapper.Mapper73.irq_counter |= mapper_data->irqCounterHighByte << 8;
			g_NESmapper.Mapper73.irq_enabled = mapper_data->irqCounterEnabled;
		}
		break;

	case 74:
	case 191:
	case 192:
	case 194:
	case 195:
		{
			struct mapper74Data* mapper_data = (struct mapper74Data*)&block->extraData;

				for(i = 0; i < 8; i++ ) {g_NESmapper.Mapper074.regs[i] = mapper_data->p[i];}
				g_NESmapper.Mapper074.prg0  = mapper_data->p[ 8];
				g_NESmapper.Mapper074.prg1  = mapper_data->p[ 9];
				g_NESmapper.Mapper074.chr[0]  = mapper_data->p[10];
				g_NESmapper.Mapper074.chr[2]  = mapper_data->p[11];
				g_NESmapper.Mapper074.chr[4]  = mapper_data->p[12];
				g_NESmapper.Mapper074.chr[5]  = mapper_data->p[13];
				g_NESmapper.Mapper074.chr[6]  = mapper_data->p[14];
				g_NESmapper.Mapper074.chr[7]  = mapper_data->p[15];
				g_NESmapper.Mapper074.irq_enable  = mapper_data->p[16];
				g_NESmapper.Mapper074.irq_counter = mapper_data->p[17];
				g_NESmapper.Mapper074.irq_latch   = mapper_data->p[18];
				g_NESmapper.Mapper074.irq_request = mapper_data->p[19];
		}
		break;

	case 75:
		{
			struct mapper75Data* mapper_data = (struct mapper75Data*)&block->extraData;

			g_NESmapper.Mapper75.regs[0] = mapper_data->lastE000Write;
			g_NESmapper.Mapper75.regs[1] = mapper_data->lastF000Write;
		}
		break;

	case 76:
		{
			struct mapper76Data* mapper_data = (struct mapper76Data*)&block->extraData;

			g_NESmapper.Mapper76.regs[0] = mapper_data->last8000Write;
		}
		break;

	case 82:
		{
			struct mapper82Data* mapper_data = (struct mapper82Data*)&block->extraData;

			g_NESmapper.Mapper82.regs[0] = mapper_data->last7EF6Write & 0x02;
		}
		break;

	case 83:
		{
			struct mapper83Data* mapper_data = (struct mapper83Data*)&block->extraData;

			g_NESmapper.Mapper83.irq_counter = mapper_data->irqCounterLowByte;
			g_NESmapper.Mapper83.irq_counter |= mapper_data->irqCounterHighByte << 8;
			g_NESmapper.Mapper83.irq_enabled = mapper_data->irqCounterEnabled;
			g_NESmapper.Mapper83.regs[0] = mapper_data->last8000Write;
			g_NESmapper.Mapper83.regs[1] = mapper_data->last8100Write;
			g_NESmapper.Mapper83.regs[2] = mapper_data->last5101Write;
		}
		break;

	case 85:
		{
			struct mapper85Data* mapper_data = (struct mapper85Data*)&block->extraData;

			g_NESmapper.Mapper85.irq_counter = mapper_data->irqCounter;
			g_NESmapper.Mapper85.irq_enabled = mapper_data->irqCounterEnabled;
			g_NESmapper.Mapper85.irq_latch = mapper_data->irqLatchCounter;
		}
		break;

	case 88:
		{
			struct mapper88Data* mapper_data = (struct mapper88Data*)&block->extraData;

			g_NESmapper.Mapper88.regs[0] = mapper_data->last8000Write;
		}
		break;

	case 91:
		{
			struct mapper91Data* mapper_data = (struct mapper91Data*)&block->extraData;

			g_NESmapper.Mapper91.irq_counter = mapper_data->irqCounter;
			g_NESmapper.Mapper91.irq_enabled = mapper_data->irqCounterEnabled;
		}
		break;

	case 95:
		{
			struct mapper95Data* mapper_data = (struct mapper95Data*)&block->extraData;

			g_NESmapper.Mapper95.regs[0] = mapper_data->last8000Write;
			NES_mapper95_SNSS_fixup();
		}
		break;

	case 96:
		{
			struct mapper96Data* mapper_data = (struct mapper96Data*)&block->extraData;

			g_NESmapper.Mapper96.vbank0 = mapper_data->wramBank;
		}
		break;

	case 105:
		{
			struct mapper105Data* mapper_data = (struct mapper105Data*)&block->extraData;

			g_NESmapper.Mapper105.irq_counter = mapper_data->irqCounter[0];
			g_NESmapper.Mapper105.irq_counter |= mapper_data->irqCounter[1] << 8;
			g_NESmapper.Mapper105.irq_counter |= mapper_data->irqCounter[2] << 16;
			g_NESmapper.Mapper105.irq_counter |= mapper_data->irqCounter[3] << 24;
			g_NESmapper.Mapper105.irq_enabled = mapper_data->irqCounterEnabled;
			g_NESmapper.Mapper105.init_state = mapper_data->InitialCounter;
			g_NESmapper.Mapper105.write_count = mapper_data->WriteCounter;
			g_NESmapper.Mapper105.bits = mapper_data->Bits;
			g_NESmapper.Mapper105.regs[0] = mapper_data->registers[0];
			g_NESmapper.Mapper105.regs[1] = mapper_data->registers[1];
			g_NESmapper.Mapper105.regs[2] = mapper_data->registers[2];
			g_NESmapper.Mapper105.regs[3] = mapper_data->registers[3];
		}
		break;

	case 117:
		{
			struct mapper117Data* mapper_data = (struct mapper117Data*)&block->extraData;

			g_NESmapper.Mapper117.irq_line = mapper_data->irqLine;
			g_NESmapper.Mapper117.irq_enabled1 = mapper_data->irqEnabled1;
			g_NESmapper.Mapper117.irq_enabled2 = mapper_data->irqEnabled2;
		}
		break;

	case 160:
		{
			struct mapper160Data* mapper_data = (struct mapper160Data*)&block->extraData;

			g_NESmapper.Mapper160.irq_counter = mapper_data->irqCounter;
			g_NESmapper.Mapper160.irq_enabled = mapper_data->irqCounterEnabled;
			g_NESmapper.Mapper160.irq_latch = mapper_data->irqLatchCounter;
			g_NESmapper.Mapper160.refresh_type = mapper_data->RefreshType;
		}
		break;

	case 165:
		{/*
			struct mapper165Data* mapper_data = (struct mapper165Data*)&block->extraData;

			g_NESmapper.Mapper165.latch = mapper_data->latch;

			// regs (B/C/D/E000)
			g_NESmapper.Mapper165.regs[1] = mapper_data->lastB000Write;
			g_NESmapper.Mapper165.regs[2] = mapper_data->lastC000Write;
			g_NESmapper.Mapper165.regs[3] = mapper_data->lastD000Write;
			g_NESmapper.Mapper165.regs[4] = mapper_data->lastE000Write;

			NES_mapper165_SNSS_fixup();*/
		}
		break;

	case 182:
		{
			struct mapper182Data* mapper_data = (struct mapper182Data*)&block->extraData;

			g_NESmapper.Mapper182.irq_counter = mapper_data->irqCounter;
			g_NESmapper.Mapper182.irq_enabled = mapper_data->irqCounterEnabled;
			g_NESmapper.Mapper182.regs[0] = mapper_data->lastA000Write;
		}
		break;

	case 189:
		{
			struct mapper189Data* mapper_data = (struct mapper189Data*)&block->extraData;

			g_NESmapper.Mapper189.irq_counter = mapper_data->irqCounter;
			g_NESmapper.Mapper189.irq_enabled = mapper_data->irqCounterEnabled;
			g_NESmapper.Mapper189.irq_latch = mapper_data->irqLatchCounter;
			g_NESmapper.Mapper189.regs[0] = mapper_data->last8000Write;
		}
		break;

	case 226:
		{
			struct mapper226Data* mapper_data = (struct mapper226Data*)&block->extraData;

			g_NESmapper.Mapper226.regs[0] = mapper_data->registers[0];
			g_NESmapper.Mapper226.regs[1] = mapper_data->registers[1];
		}
		break;

	case 230:
		{
			struct mapper230Data* mapper_data = (struct mapper230Data*)&block->extraData;

			g_NESmapper.Mapper230.rom_switch = mapper_data->numberOfResets;
		}
		break;

	case 232:
		{
			struct mapper232Data* mapper_data = (struct mapper232Data*)&block->extraData;

			g_NESmapper.Mapper232.regs[0] = mapper_data->last9000Write;
			g_NESmapper.Mapper232.regs[1] = mapper_data->lastA000Write;
		}
		break;

	case 234:
		{
			struct mapper234Data* mapper_data = (struct mapper234Data*)&block->extraData;

			g_NESmapper.Mapper234.regs[0] = mapper_data->lastFF80Write;
			g_NESmapper.Mapper234.regs[1] = mapper_data->lastFFE8Write;
			g_NESmapper.Mapper234.regs[2] = mapper_data->lastFFC0Write;
		}
		break;

	case 236:
		{
			struct mapper236Data* mapper_data = (struct mapper236Data*)&block->extraData;

			g_NESmapper.Mapper236.bank = mapper_data->BankSelect;
			g_NESmapper.Mapper236.mode = mapper_data->MapperMode;
		}
		break;

	case 243:
		{
			struct mapper243Data* mapper_data = (struct mapper243Data*)&block->extraData;

			g_NESmapper.Mapper243.regs[0] = mapper_data->registers[0];
			g_NESmapper.Mapper243.regs[1] = mapper_data->registers[1];
			g_NESmapper.Mapper243.regs[2] = mapper_data->registers[2];
			g_NESmapper.Mapper243.regs[3] = mapper_data->registers[3];
		}
		break;

	case 255:
		{
			struct mapper255Data* mapper_data = (struct mapper255Data*)&block->extraData;

			g_NESmapper.Mapper255.regs[0] = mapper_data->registers[0];
			g_NESmapper.Mapper255.regs[1] = mapper_data->registers[1];
			g_NESmapper.Mapper255.regs[2] = mapper_data->registers[2];
			g_NESmapper.Mapper255.regs[3] = mapper_data->registers[3];
		}
		break;
	}
}

static void adopt_ExMPRD(const char* fn, NES* nes)
{
	uint32 i;
	uint32 block_num;
	int hFile = NES_fopen(fn, FILE_MODE_READ);

	// pass other block
	NES_fseek(hFile, 4, FILE_SEEK_SET);

	block_num = 0;
	block_num |= NES_fgetc(hFile) << 24;
	block_num |= NES_fgetc(hFile) << 16;
	block_num |= NES_fgetc(hFile) << 8;
	block_num |= NES_fgetc(hFile) << 0;

	for(i = 0; i < block_num; i++)
	{
		uint32 block_length = 0;
		NES_fseek(hFile, 8, FILE_SEEK_CUR);
		block_length |= NES_fgetc(hFile) << 24;
		block_length |= NES_fgetc(hFile) << 16;
		block_length |= NES_fgetc(hFile) << 8;
		block_length |= NES_fgetc(hFile) << 0;
		NES_fseek(hFile, block_length, FILE_SEEK_CUR);
	}

	if(NES_ROM_get_mapper_num() == 1 && NES_crc32() == 0xb8747abf)
	{
		// Best Play - Pro Yakyuu Special (J)

		// save WRAM
		NES_fseek(hFile, 12, FILE_SEEK_CUR);
		NES_fread(&g_NES.SaveRAM[0x2000], 0x2000, 1, hFile);
	}
	else if(NES_ROM_get_mapper_num() == 5)
	{
		// read WRAM
		NES_fseek(hFile, 12, FILE_SEEK_CUR);
		NES_fread(g_NESmapper.Mapper5.wram, 0x10000, 1, hFile);
	}
	else if(NES_ROM_get_mapper_num() == 20)
	{
		// read WRAM
		NES_fseek(hFile, 12, FILE_SEEK_CUR);
		NES_fread(g_NESmapper.Mapper20.wram, 0x8000, 1, hFile);

		// read Disk #1 - #4
		NES_fseek(hFile, 12, FILE_SEEK_CUR);
		NES_fread(g_NESmapper.Mapper20.disk, 65500*4, 1, hFile);
	}

	NES_fclose(hFile);
}

static void adopt_CNTR(SnssControllersBlock* block, NES* nes)
{
}

static void adopt_SOUN(SnssSoundBlock* block, NES* nes)
{
	// Sound Data

	// give them to the apu
	NES_APU_load_regs(block->soundRegisters);
}

boolean LoadSNSS(const char* fn, NES* nes)
{
	int i;
	SNSS_BLOCK_TYPE blockType;
	SNSS_FILE* snssFile = NULL;
	DEBUG("LoadSNSS in");

	if(SNSS_OK != SNSS_OpenFile(&snssFile, fn, SNSS_OPEN_READ)) {
		goto OpenError;
	}

	// at this point, it's too late to go back, and the NES must be reset on failure
	for(i = 0; i < (int)snssFile->headerBlock.numberOfBlocks; i++)
	{
		DEBUG("SNSS_GetNextBlockType");
		DEBUG("snssFile");
		if(SNSS_OK != SNSS_GetNextBlockType(&blockType, snssFile))
			goto ReadError;

		DEBUGVALUE("SNSS_ReadBlock ", blockType);
		if(SNSS_OK != SNSS_ReadBlock(snssFile, blockType))
			goto ReadError;

		DEBUGVALUE("adopt_Hoge in", blockType);
		switch(blockType)
		{
		case SNSS_BASR:
			adopt_BASR(&snssFile->baseBlock, nes);
			break;

		case SNSS_VRAM:
			adopt_VRAM(&snssFile->vramBlock, nes);
			break;

		case SNSS_SRAM:
			adopt_SRAM(&snssFile->sramBlock, nes);
			break;

		case SNSS_MPRD:
			adopt_MPRD(&snssFile->mapperBlock, nes);
			break;

		case SNSS_CNTR:
			adopt_CNTR(&snssFile->contBlock, nes);
			break;

		case SNSS_SOUN:
			adopt_SOUN(&snssFile->soundBlock, nes);
			break;

		case SNSS_UNKNOWN_BLOCK:
			break;

		default:
			goto ReadError;
			break;

		}
	}

	SNSS_CloseFile(&snssFile);

	// read Extra Mapper Data
	adopt_ExMPRD(fn, nes);

	DEBUG("Loaded done");

	return TRUE;
ReadError:
	NES_reset(0);
OpenError:
	DEBUG("Error reading ");
	if(snssFile) SNSS_CloseFile(&snssFile);
	return FALSE;
}


// these functions create a SNSS block from the current emulated NES
// return 0 if block is valid

static int extract_BASR(SnssBaseBlock* block, NES* nes)
{
	nes6502_context context;

	// get the CPU context
	NES6502_GetContext(&context);

	// CPU data
	block->regA = context.a_reg;
	block->regX = context.x_reg;
	block->regY = context.y_reg;
	block->regFlags = context.p_reg;
	block->regStack = context.s_reg;
	block->regPc = context.pc_reg;

	// $2000 and $2001
	block->reg2000 = g_PPU.LowRegs[0];
	block->reg2001 = g_PPU.LowRegs[1];

	// RAM
	_memcpy(block->cpuRam, g_NES.RAM, 0x800);

	// SPR-RAM
	_memcpy(block->spriteRam, g_PPU.spr_ram, 0x100);

	// PPU $2000-$2FFF (Name Tables/Attrib Tables)
	_memcpy(block->ppuRam, g_PPU.PPU_nametables, 4*0x400);

	// palettes
	_memcpy(&block->palette[0x00], g_PPU.bg_pal,  0x10);
	_memcpy(&block->palette[0x10], g_PPU.spr_pal, 0x10);

	// mirroring
	block->mirrorState[0] = (g_PPU.PPU_VRAM_banks[0x08] - g_PPU.PPU_nametables) >> 10;
	block->mirrorState[1] = (g_PPU.PPU_VRAM_banks[0x09] - g_PPU.PPU_nametables) >> 10;
	block->mirrorState[2] = (g_PPU.PPU_VRAM_banks[0x0A] - g_PPU.PPU_nametables) >> 10;
	block->mirrorState[3] = (g_PPU.PPU_VRAM_banks[0x0B] - g_PPU.PPU_nametables) >> 10;
	ASSERT(block->mirrorState[0] < 4); ASSERT(block->mirrorState[1] < 4);
	ASSERT(block->mirrorState[2] < 4); ASSERT(block->mirrorState[3] < 4);

	// VRAM address
	block->vramAddress = g_PPU.loopy_t;

	// OAM (sprite) address
	block->spriteRamAddress = g_PPU.spr_ram_rw_ptr;

	// tile X offset
	block->tileXOffset = g_PPU.loopy_x;

	return 0;
}

static int extract_VRAM(SnssVramBlock* block, NES* nes)
{
	// if cart has VROM, don't write any VRAM
	//if(NES_ROM_get_num_8k_VROM_banks()) return -1;
	uint32 i;
	uint8* patterntables = NES_PPU_get_patt();

	for(i = 0; i < 0x8000; i++)
	{
		if(patterntables[i] != 0x00) break;
	}
	if(i == 0x8000) return -1;

	// 8K of VRAM data
	block->vramSize = g_PPU.vram_size;
	_memcpy(&block->vram, NES_PPU_get_patt(), block->vramSize);

	return 0;
}

static int extract_SRAM(SnssSramBlock* block, NES* nes)
{
	nes6502_context context;
	uint32 i;
	uint32 *dp;

	// if nothing has been written to SRAM, don't write it out
	// has anything been written to Save RAM?
#if 0
	for(i = 0; i < sizeof(g_NES.SaveRAM); i++)
	{
		if(g_NES.SaveRAM[i] != 0x00) break;
	}
	if(i == sizeof(g_NES.SaveRAM)) return -1;
#endif

	dp = (uint32 *)g_NES.SaveRAM;
	for(i = 0; i < sizeof(g_NES.SaveRAM)/4; i++)
	{
		if(dp[i] != 0x00) break;
	}
	if(i == sizeof(g_NES.SaveRAM)/4) return -1;

	// SRAM writeable flag
	block->sramEnabled = 1;

	// SRAM size (8k)
	block->sramSize = 0x2000;

	// SRAM data
	NES6502_GetContext(&context);
	_memcpy(block->sram, context.mem_page[3], 0x2000);

	return 0;
}

static int extract_MPRD(SnssMapperBlock* block, NES* nes)
{
	nes6502_context context;
	uint8 i;

	if(0 == NES_ROM_get_mapper_num()) return -1;

	// 8K PRG page numbers
	NES6502_GetContext(&context);
	block->prgPages[0] = (context.mem_page[4] - NES_ROM_get_ROM_banks()) >> 13;
	block->prgPages[1] = (context.mem_page[5] - NES_ROM_get_ROM_banks()) >> 13;
	block->prgPages[2] = (context.mem_page[6] - NES_ROM_get_ROM_banks()) >> 13;
	block->prgPages[3] = (context.mem_page[7] - NES_ROM_get_ROM_banks()) >> 13;

	// 1K CHR page numbers
	for(i = 0; i < 8; i++)
	{
		if(NES_PPU_get_pattype(i))
		{
			// VROM
			block->chrPages[i] = (g_PPU.PPU_VRAM_banks[i] - NES_ROM_get_VROM_banks()) >> 10;
		}
		else
		{
			// VRAM
			block->chrPages[i] = ((g_PPU.PPU_VRAM_banks[i] - NES_PPU_get_patt()) >> 10) | 0x8000;
		}
	}

	switch(NES_ROM_get_mapper_num())
	{
	case 1:
		{
			struct mapper1Data* mapper_data = (struct mapper1Data*)&block->extraData;

			// last values written to the 4 registers
			_memcpy(mapper_data->registers, g_NESmapper.Mapper1.regs, 4);

			// latch register
			mapper_data->latch = g_NESmapper.Mapper1.bits;

			// number of bits written to unfinished reg
			mapper_data->numberOfBits = g_NESmapper.Mapper1.write_count;
		}
		break;

	case 4:
		{
			struct mapper4Data* mapper_data = (struct mapper4Data*)&block->extraData;

			int i;
			for( i = 0; i < 8; i++ ) {
				mapper_data->p[i] = g_NESmapper.Mapper4.reg[i];
			}
			mapper_data->p[ 8] = g_NESmapper.Mapper4.prg0;
			mapper_data->p[ 9] = g_NESmapper.Mapper4.prg1;
			mapper_data->p[10] = g_NESmapper.Mapper4.chr01;
			mapper_data->p[11] = g_NESmapper.Mapper4.chr23;
			mapper_data->p[12] = g_NESmapper.Mapper4.chr4;
			mapper_data->p[13] = g_NESmapper.Mapper4.chr5;
			mapper_data->p[14] = g_NESmapper.Mapper4.chr6;
			mapper_data->p[15] = g_NESmapper.Mapper4.chr7;
			mapper_data->p[16] = g_NESmapper.Mapper4.irq_enable;
			mapper_data->p[17] = (unsigned char)g_NESmapper.Mapper4.irq_counter;
			mapper_data->p[18] = g_NESmapper.Mapper4.irq_latch;
			mapper_data->p[19] = g_NESmapper.Mapper4.irq_request;
			mapper_data->p[20] = g_NESmapper.Mapper4.irq_preset;
			mapper_data->p[21] = g_NESmapper.Mapper4.irq_preset_vbl;
		}
		break;

	case 5:
		{
			struct mapper5Data* mapper_data = (struct mapper5Data*)&block->extraData;

			mapper_data->irqLineLowByte = g_NESmapper.Mapper5.irq_line & 0x00ff;
			mapper_data->irqLineHighByte = (g_NESmapper.Mapper5.irq_line & 0xff00) >> 8;
			mapper_data->irqEnabled = g_NESmapper.Mapper5.irq_enabled;
			mapper_data->irqStatus = g_NESmapper.Mapper5.irq_status;
			mapper_data->wramWriteProtect0 = g_NESmapper.Mapper5.wram_protect0;
			mapper_data->wramWriteProtect1 = g_NESmapper.Mapper5.wram_protect1;
			mapper_data->romBankSize = g_NESmapper.Mapper5.prg_size;
			mapper_data->vromBankSize = g_NESmapper.Mapper5.chr_size;
			mapper_data->gfxMode = g_NESmapper.Mapper5.gfx_mode;
			mapper_data->splitControl = g_NESmapper.Mapper5.split_control;
			mapper_data->splitBank = g_NESmapper.Mapper5.split_bank;
			mapper_data->last5205Write = g_NESmapper.Mapper5.value0;
			mapper_data->last5206Write = g_NESmapper.Mapper5.value1;
			mapper_data->wramBank3 = g_NESmapper.Mapper5.wb[3];
			mapper_data->wramBank4 = g_NESmapper.Mapper5.wb[4];
			mapper_data->wramBank5 = g_NESmapper.Mapper5.wb[5];
			mapper_data->wramBank6 = g_NESmapper.Mapper5.wb[6];
			mapper_data->vromBank[0][0] = g_NESmapper.Mapper5.chr_reg[0][0];
			mapper_data->vromBank[1][0] = g_NESmapper.Mapper5.chr_reg[1][0];
			mapper_data->vromBank[2][0] = g_NESmapper.Mapper5.chr_reg[2][0];
			mapper_data->vromBank[3][0] = g_NESmapper.Mapper5.chr_reg[3][0];
			mapper_data->vromBank[4][0] = g_NESmapper.Mapper5.chr_reg[4][0];
			mapper_data->vromBank[5][0] = g_NESmapper.Mapper5.chr_reg[5][0];
			mapper_data->vromBank[6][0] = g_NESmapper.Mapper5.chr_reg[6][0];
			mapper_data->vromBank[7][0] = g_NESmapper.Mapper5.chr_reg[7][0];
			mapper_data->vromBank[0][1] = g_NESmapper.Mapper5.chr_reg[0][1];
			mapper_data->vromBank[1][1] = g_NESmapper.Mapper5.chr_reg[1][1];
			mapper_data->vromBank[2][1] = g_NESmapper.Mapper5.chr_reg[2][1];
			mapper_data->vromBank[3][1] = g_NESmapper.Mapper5.chr_reg[3][1];
			mapper_data->vromBank[4][1] = g_NESmapper.Mapper5.chr_reg[4][1];
			mapper_data->vromBank[5][1] = g_NESmapper.Mapper5.chr_reg[5][1];
			mapper_data->vromBank[6][1] = g_NESmapper.Mapper5.chr_reg[6][1];
			mapper_data->vromBank[7][1] = g_NESmapper.Mapper5.chr_reg[7][1];
		}
		break;

	case 6:
		{
			struct mapper6Data* mapper_data = (struct mapper6Data*)&block->extraData;

			mapper_data->irqCounterLowByte = g_NESmapper.Mapper6.irq_counter & 0x00FF;
			mapper_data->irqCounterHighByte = (g_NESmapper.Mapper6.irq_counter & 0xFF00) >> 8;
			mapper_data->irqCounterEnabled = g_NESmapper.Mapper6.irq_enabled;
		}
		break;

	case 9:
		{
			struct mapper9Data* mapper_data = (struct mapper9Data*)&block->extraData;

			// 2 latch registers
			mapper_data->latch[0] = g_NESmapper.Mapper9.latch_0000;
			mapper_data->latch[1] = g_NESmapper.Mapper9.latch_1000;

			// regs (B/C/D/E000)
			mapper_data->lastB000Write = g_NESmapper.Mapper9.regs[1];
			mapper_data->lastC000Write = g_NESmapper.Mapper9.regs[2];
			mapper_data->lastD000Write = g_NESmapper.Mapper9.regs[3];
			mapper_data->lastE000Write = g_NESmapper.Mapper9.regs[4];
		}
		break;

	case 10:
		{
			struct mapper10Data* mapper_data = (struct mapper10Data*)&block->extraData;

			// 2 latch registers
			mapper_data->latch[0] = g_NESmapper.Mapper10.latch_0000;
			mapper_data->latch[1] = g_NESmapper.Mapper10.latch_1000;

			// regs (B/C/D/E000)
			mapper_data->lastB000Write = g_NESmapper.Mapper10.regs[1];
			mapper_data->lastC000Write = g_NESmapper.Mapper10.regs[2];
			mapper_data->lastD000Write = g_NESmapper.Mapper10.regs[3];
			mapper_data->lastE000Write = g_NESmapper.Mapper10.regs[4];
		}
		break;

	case 13:
		{
			struct mapper13Data* mapper_data = (struct mapper13Data*)&block->extraData;

			mapper_data->wramBank = g_NESmapper.Mapper13.chr_bank;
		}
		break;

	case 16:
		{
			struct mapper16Data* mapper_data = (struct mapper16Data*)&block->extraData;

			mapper_data->irqCounterLowByte = g_NESmapper.Mapper16.irq_counter & 0x00FF;
			mapper_data->irqCounterHighByte = (g_NESmapper.Mapper16.irq_counter & 0xFF00) >> 8;
			mapper_data->irqCounterEnabled = g_NESmapper.Mapper16.irq_enabled;
			mapper_data->irqLatchCounterLowByte = g_NESmapper.Mapper16.irq_latch & 0x00FF;
			mapper_data->irqLatchCounterHighByte = (g_NESmapper.Mapper16.irq_latch & 0xFF00) >> 8;
		}
		break;

	case 17:
		{
			struct mapper17Data* mapper_data = (struct mapper17Data*)&block->extraData;

			mapper_data->irqCounterLowByte = g_NESmapper.Mapper17.irq_counter & 0x00FF;
			mapper_data->irqCounterHighByte = (g_NESmapper.Mapper17.irq_counter & 0xFF00) >> 8;
			mapper_data->irqCounterEnabled = g_NESmapper.Mapper17.irq_enabled;
		}
		break;

	case 18:
		{
			struct mapper18Data* mapper_data = (struct mapper18Data*)&block->extraData;

			mapper_data->irqCounterLowByte = g_NESmapper.Mapper18.irq_counter & 0x00FF;
			mapper_data->irqCounterHighByte = (g_NESmapper.Mapper18.irq_counter & 0xFF00) >> 8;
			mapper_data->irqCounterEnabled = g_NESmapper.Mapper18.irq_enabled;
		}
		break;

	case 19:
		{
			struct mapper19Data* mapper_data = (struct mapper19Data*)&block->extraData;

			mapper_data->irqCounterLowByte = g_NESmapper.Mapper19.irq_counter & 0x00FF;
			mapper_data->irqCounterHighByte = (g_NESmapper.Mapper19.irq_counter & 0xFF00) >> 8;
			mapper_data->irqCounterEnabled = g_NESmapper.Mapper19.irq_enabled;
			mapper_data->lastE800Write = (g_NESmapper.Mapper19.regs[0] & 0x01) << 6;
			mapper_data->lastE800Write |= (g_NESmapper.Mapper19.regs[1] & 0x01) << 7;
			mapper_data->lastF800Write = g_NESmapper.Mapper19.regs[2];
		}
		break;

	case 20:
		{
			struct mapper20Data* mapper_data = (struct mapper20Data*)&block->extraData;

			mapper_data->irqCounterLowByte = g_NESmapper.Mapper20.irq_counter & 0x00FF;
			mapper_data->irqCounterHighByte = (g_NESmapper.Mapper20.irq_counter & 0xFF00) >> 8;
			mapper_data->irqCounterEnabled = g_NESmapper.Mapper20.irq_enabled;
			mapper_data->irqLatchCounterLowByte = g_NESmapper.Mapper20.irq_latch & 0x00FF;
			mapper_data->irqLatchCounterHighByte = (g_NESmapper.Mapper20.irq_latch & 0xFF00) >> 8;
			mapper_data->irqWaitCounter = g_NESmapper.Mapper20.irq_wait;
			mapper_data->last4023Write = g_NESmapper.Mapper20.disk_enabled;
			mapper_data->last4025Write = g_NESmapper.Mapper20.write_reg;
			mapper_data->HeadPositionLowByte = g_NESmapper.Mapper20.head_position & 0x00FF;
			mapper_data->HeadPositionHighByte = (g_NESmapper.Mapper20.head_position & 0xFF00) >> 8;
			mapper_data->DiskStatus = g_NESmapper.Mapper20.disk_status;
			mapper_data->WriteSkip = g_NESmapper.Mapper20.write_skip;
			mapper_data->CurrentDiskSide = g_NESmapper.Mapper20.current_side;
			mapper_data->LastDiskSide = g_NESmapper.Mapper20.last_side;
			mapper_data->DiskInsertWait = g_NESmapper.Mapper20.insert_wait;
		}
		break;

	case 21:
		{
			struct mapper21Data* mapper_data = (struct mapper21Data*)&block->extraData;

			mapper_data->irqCounter = g_NESmapper.Mapper21.irq_counter;
			mapper_data->irqCounterEnabled = g_NESmapper.Mapper21.irq_enabled;
			mapper_data->irqLatchCounter = g_NESmapper.Mapper21.irq_latch;
			mapper_data->last9002Write = g_NESmapper.Mapper21.regs[8];
		}
		break;

	case 23:
		{
			struct mapper23Data* mapper_data = (struct mapper23Data*)&block->extraData;

			mapper_data->irqCounter = g_NESmapper.Mapper23.irq_counter;
			mapper_data->irqCounterEnabled = g_NESmapper.Mapper23.irq_enabled;
			mapper_data->irqLatchCounter = g_NESmapper.Mapper23.irq_latch;
			mapper_data->last9008Write = g_NESmapper.Mapper23.regs[8];
		}
		break;

	case 24:
		{
			struct mapper24Data* mapper_data = (struct mapper24Data*)&block->extraData;

			mapper_data->irqCounter = g_NESmapper.Mapper24.irq_counter;
			mapper_data->irqCounterEnabled = g_NESmapper.Mapper24.irq_enabled;
			mapper_data->irqLatchCounter = g_NESmapper.Mapper24.irq_latch;
		}
		break;

	case 25:
		{
			struct mapper25Data* mapper_data = (struct mapper25Data*)&block->extraData;

			mapper_data->irqCounter = g_NESmapper.Mapper25.irq_counter;
			mapper_data->irqCounterEnabled = g_NESmapper.Mapper25.irq_enabled;
			mapper_data->irqLatchCounter = g_NESmapper.Mapper25.irq_latch;
			mapper_data->last9001Write = g_NESmapper.Mapper25.regs[10];
		}
		break;

	case 26:
		{
			struct mapper26Data* mapper_data = (struct mapper26Data*)&block->extraData;

			mapper_data->irqCounter = g_NESmapper.Mapper26.irq_counter;
			mapper_data->irqCounterEnabled = g_NESmapper.Mapper26.irq_enabled;
			mapper_data->irqLatchCounter = g_NESmapper.Mapper26.irq_latch;
		}
		break;

	case 32:
		{
			struct mapper32Data* mapper_data = (struct mapper32Data*)&block->extraData;

			mapper_data->last9000Write = g_NESmapper.Mapper32.regs[0];
		}
		break;

	case 33:
		{
			struct mapper33Data* mapper_data = (struct mapper33Data*)&block->extraData;

			mapper_data->irqCounter = g_NESmapper.Mapper33.irq_counter;
			mapper_data->irqCounterEnabled = g_NESmapper.Mapper33.irq_enabled;
		}
		break;

	case 40:
		{
			struct mapper40Data* mapper_data = (struct mapper40Data*)&block->extraData;

			// IRQ counter
			mapper_data->irqCounter = g_NESmapper.Mapper40.lines_to_irq;

			// IRQ enabled
			mapper_data->irqCounterEnabled = g_NESmapper.Mapper40.irq_enabled;
		}
		break;

	case 41:
		{
			struct mapper41Data* mapper_data = (struct mapper41Data*)&block->extraData;

			mapper_data->last6000Write = g_NESmapper.Mapper41.regs[0];
		}
		break;

	case 42:
		{
			struct mapper42Data* mapper_data = (struct mapper42Data*)&block->extraData;

			mapper_data->irqCounter = g_NESmapper.Mapper42.irq_counter;
			mapper_data->irqCounterEnabled = g_NESmapper.Mapper42.irq_enabled;
		}
		break;

	case 43:
		{
			struct mapper43Data* mapper_data = (struct mapper43Data*)&block->extraData;

			mapper_data->irqCounterLowByte = g_NESmapper.Mapper43.irq_counter & 0x00FF;
			mapper_data->irqCounterHighByte = (g_NESmapper.Mapper43.irq_counter & 0xFF00) >> 8;
			mapper_data->irqCounterEnabled = g_NESmapper.Mapper43.irq_enabled;
		}
		break;

	case 46:
		{
			struct mapper46Data* mapper_data = (struct mapper46Data*)&block->extraData;

			mapper_data->last6000Write = g_NESmapper.Mapper46.regs[0] | (g_NESmapper.Mapper46.regs[1] << 4);
			mapper_data->last8000Write = g_NESmapper.Mapper46.regs[2] | (g_NESmapper.Mapper46.regs[1] << 4);
		}
		break;

	case 48:
		{
			struct mapper48Data* mapper_data = (struct mapper48Data*)&block->extraData;

			mapper_data->lastE000Write = g_NESmapper.Mapper48.regs[0];
		}
		break;

	case 49:
		{
			struct mapper49Data* mapper_data = (struct mapper49Data*)&block->extraData;

			mapper_data->irqCounter = g_NESmapper.Mapper49.irq_counter;
			mapper_data->irqLatchCounter = g_NESmapper.Mapper49.irq_latch;
			mapper_data->irqCounterEnabled = g_NESmapper.Mapper49.irq_enabled;
			mapper_data->last8000Write = g_NESmapper.Mapper49.regs[0];
			mapper_data->last6000Write = g_NESmapper.Mapper49.regs[1];
			mapper_data->lastA001Write = g_NESmapper.Mapper49.regs[2];
		}
		break;

	case 50:
		{
			struct mapper50Data* mapper_data = (struct mapper50Data*)&block->extraData;

			mapper_data->irqCounterEnabled = g_NESmapper.Mapper50.irq_enabled;
		}
		break;

	case 51:
		{
			struct mapper51Data* mapper_data = (struct mapper51Data*)&block->extraData;

			mapper_data->BankSelect = g_NESmapper.Mapper51.bank;
			mapper_data->MapperMode = g_NESmapper.Mapper51.mode;
		}
		break;

	case 57:
		{
			struct mapper57Data* mapper_data = (struct mapper57Data*)&block->extraData;

			mapper_data->last8800Write = g_NESmapper.Mapper57.regs[0];
		}
		break;

	case 64:
		{
			struct mapper64Data* mapper_data = (struct mapper64Data*)&block->extraData;

			mapper_data->irqCounter = g_NESmapper.Mapper64.irq_counter;
			mapper_data->irqCounterEnabled = g_NESmapper.Mapper64.irq_enabled;
			mapper_data->irqLatchCounter = g_NESmapper.Mapper64.irq_latch;
			mapper_data->last8000Write = g_NESmapper.Mapper64.regs[0] | g_NESmapper.Mapper64.regs[1] | g_NESmapper.Mapper64.regs[2];
		}
		break;

	case 65:
		{
			struct mapper65Data* mapper_data = (struct mapper65Data*)&block->extraData;

			mapper_data->irqCounterLowByte = g_NESmapper.Mapper65.irq_counter & 0x00FF;
			mapper_data->irqCounterHighByte = (g_NESmapper.Mapper65.irq_counter & 0xFF00) >> 8;
			mapper_data->irqCounterEnabled = g_NESmapper.Mapper65.irq_enabled;
			mapper_data->irqLatchCounterLowByte = g_NESmapper.Mapper65.irq_latch & 0x00FF;
			mapper_data->irqLatchCounterHighByte = (g_NESmapper.Mapper65.irq_latch & 0xFF00) >> 8;
		}
		break;

	case 67:
		{
			struct mapper67Data* mapper_data = (struct mapper67Data*)&block->extraData;

			mapper_data->irqCounter = g_NESmapper.Mapper67.irq_counter;
			mapper_data->irqCounterEnabled = g_NESmapper.Mapper67.irq_enabled;
			mapper_data->irqLatchCounter = g_NESmapper.Mapper67.irq_latch;
		}
		break;

	case 68:
		{
			struct mapper68Data* mapper_data = (struct mapper68Data*)&block->extraData;

			mapper_data->lastC000Write = g_NESmapper.Mapper68.regs[2];
			mapper_data->lastD000Write = g_NESmapper.Mapper68.regs[3];
			mapper_data->lastE000Write = (g_NESmapper.Mapper68.regs[0] << 4) | g_NESmapper.Mapper68.regs[1];
		}
		break;

	case 69:
		{
			struct mapper69Data* mapper_data = (struct mapper69Data*)&block->extraData;

			mapper_data->irqCounterLowByte = g_NESmapper.Mapper69.irq_counter & 0x00FF;
			mapper_data->irqCounterHighByte = (g_NESmapper.Mapper69.irq_counter & 0xFF00) >> 8;
			mapper_data->irqCounterEnabled = g_NESmapper.Mapper69.irq_enabled;
			mapper_data->last8000Write = g_NESmapper.Mapper69.regs[0];
		}
		break;

	case 73:
		{
			struct mapper73Data* mapper_data = (struct mapper73Data*)&block->extraData;

			mapper_data->irqCounterLowByte = g_NESmapper.Mapper73.irq_counter & 0x00FF;
			mapper_data->irqCounterHighByte = (g_NESmapper.Mapper73.irq_counter & 0xFF00) >> 8;
			mapper_data->irqCounterEnabled = g_NESmapper.Mapper73.irq_enabled;
		}
		break;

	case 74:
	case 191:
	case 192:
	case 194:
	case 195:
		{
			struct mapper74Data* mapper_data = (struct mapper74Data*)&block->extraData;

			for( i = 0; i < 8; i++ ) {	mapper_data->p[i] = g_NESmapper.Mapper074.regs[i];}
			mapper_data->p[ 8] = g_NESmapper.Mapper074.prg0;
			mapper_data->p[ 9] = g_NESmapper.Mapper074.prg1;
			mapper_data->p[10] = g_NESmapper.Mapper074.chr[0];
			mapper_data->p[11] = g_NESmapper.Mapper074.chr[2];
			mapper_data->p[12] = g_NESmapper.Mapper074.chr[4];
			mapper_data->p[13] = g_NESmapper.Mapper074.chr[5];
			mapper_data->p[14] = g_NESmapper.Mapper074.chr[6];
			mapper_data->p[15] = g_NESmapper.Mapper074.chr[7];
			mapper_data->p[16] = g_NESmapper.Mapper074.irq_enable;
			mapper_data->p[17] = g_NESmapper.Mapper074.irq_counter;
			mapper_data->p[18] = g_NESmapper.Mapper074.irq_latch;
			mapper_data->p[19] = g_NESmapper.Mapper074.irq_request;
		}
		break;

	case 75:
		{
			struct mapper75Data* mapper_data = (struct mapper75Data*)&block->extraData;

			mapper_data->lastE000Write = g_NESmapper.Mapper75.regs[0];
			mapper_data->lastF000Write = g_NESmapper.Mapper75.regs[1];
		}
		break;

	case 76:
		{
			struct mapper76Data* mapper_data = (struct mapper76Data*)&block->extraData;

			mapper_data->last8000Write = g_NESmapper.Mapper76.regs[0];
		}
		break;

	case 82:
		{
			struct mapper82Data* mapper_data = (struct mapper82Data*)&block->extraData;

			mapper_data->last7EF6Write = g_NESmapper.Mapper82.regs[0];
		}
		break;

	case 83:
		{
			struct mapper83Data* mapper_data = (struct mapper83Data*)&block->extraData;

			mapper_data->irqCounterLowByte = g_NESmapper.Mapper83.irq_counter & 0x00FF;
			mapper_data->irqCounterHighByte = (g_NESmapper.Mapper83.irq_counter & 0xFF00) >> 8;
			mapper_data->irqCounterEnabled = g_NESmapper.Mapper83.irq_enabled;
			mapper_data->last8000Write = g_NESmapper.Mapper83.regs[0];
			mapper_data->last8100Write = g_NESmapper.Mapper83.regs[1];
			mapper_data->last5101Write = g_NESmapper.Mapper83.regs[2];
		}
		break;

	case 85:
		{
			struct mapper85Data* mapper_data = (struct mapper85Data*)&block->extraData;

			mapper_data->irqCounter = g_NESmapper.Mapper85.irq_counter;
			mapper_data->irqCounterEnabled = g_NESmapper.Mapper85.irq_enabled;
			mapper_data->irqLatchCounter = g_NESmapper.Mapper85.irq_latch;
		}
		break;

	case 88:
		{
			struct mapper88Data* mapper_data = (struct mapper88Data*)&block->extraData;

			mapper_data->last8000Write = g_NESmapper.Mapper88.regs[0];
		}
		break;

	case 91:
		{
			struct mapper91Data* mapper_data = (struct mapper91Data*)&block->extraData;

			mapper_data->irqCounter = g_NESmapper.Mapper91.irq_counter;
			mapper_data->irqCounterEnabled = g_NESmapper.Mapper91.irq_enabled;
		}
		break;

	case 95:
		{
			struct mapper95Data* mapper_data = (struct mapper95Data*)&block->extraData;

			mapper_data->last8000Write = g_NESmapper.Mapper95.regs[0];
		}
		break;

	case 96:
		{
			struct mapper96Data* mapper_data = (struct mapper96Data*)&block->extraData;

			mapper_data->wramBank = g_NESmapper.Mapper96.vbank0;
		}
		break;

	case 105:
		{
			struct mapper105Data* mapper_data = (struct mapper105Data*)&block->extraData;

			mapper_data->irqCounter[0] = (g_NESmapper.Mapper105.irq_counter &0x000000ff) >> 0;
			mapper_data->irqCounter[1] = (g_NESmapper.Mapper105.irq_counter &0x0000ff00) >> 8;
			mapper_data->irqCounter[2] = (g_NESmapper.Mapper105.irq_counter &0x00ff0000) >> 16;
			mapper_data->irqCounter[3] = (g_NESmapper.Mapper105.irq_counter &0xff000000) >> 24;
			mapper_data->irqCounterEnabled = g_NESmapper.Mapper105.irq_enabled;
			mapper_data->InitialCounter = g_NESmapper.Mapper105.init_state;
			mapper_data->WriteCounter = g_NESmapper.Mapper105.write_count;
			mapper_data->Bits = g_NESmapper.Mapper105.bits;
			mapper_data->registers[0] = g_NESmapper.Mapper105.regs[0];
			mapper_data->registers[1] = g_NESmapper.Mapper105.regs[1];
			mapper_data->registers[2] = g_NESmapper.Mapper105.regs[2];
			mapper_data->registers[3] = g_NESmapper.Mapper105.regs[3];
		}
		break;

	case 117:
		{
			struct mapper117Data* mapper_data = (struct mapper117Data*)&block->extraData;

			mapper_data->irqLine = g_NESmapper.Mapper117.irq_line;
			mapper_data->irqEnabled1 = g_NESmapper.Mapper117.irq_enabled1;
			mapper_data->irqEnabled2 = g_NESmapper.Mapper117.irq_enabled2;
		}
		break;

	case 160:
		{
			struct mapper160Data* mapper_data = (struct mapper160Data*)&block->extraData;

			mapper_data->irqCounter = g_NESmapper.Mapper160.irq_counter;
			mapper_data->irqCounterEnabled = g_NESmapper.Mapper160.irq_enabled;
			mapper_data->irqLatchCounter = g_NESmapper.Mapper160.irq_latch;
			mapper_data->RefreshType = g_NESmapper.Mapper160.refresh_type;
		}
		break;

	case 165:
		{/*
			struct mapper165Data* mapper_data = (struct mapper165Data*)&block->extraData;

			mapper_data->latch = g_NESmapper.Mapper165.latch;

			// regs (B/C/D/E000)
			mapper_data->lastB000Write = g_NESmapper.Mapper165.regs[1];
			mapper_data->lastC000Write = g_NESmapper.Mapper165.regs[2];
			mapper_data->lastD000Write = g_NESmapper.Mapper165.regs[3];
			mapper_data->lastE000Write = g_NESmapper.Mapper165.regs[4];*/
		}
		break;

	case 182:
		{
			struct mapper182Data* mapper_data = (struct mapper182Data*)&block->extraData;

			mapper_data->irqCounter = g_NESmapper.Mapper182.irq_counter;
			mapper_data->irqCounterEnabled = g_NESmapper.Mapper182.irq_enabled;
			mapper_data->lastA000Write = g_NESmapper.Mapper182.regs[0];
		}
		break;

	case 189:
		{
			struct mapper189Data* mapper_data = (struct mapper189Data*)&block->extraData;

			mapper_data->irqCounter = g_NESmapper.Mapper189.irq_counter;
			mapper_data->irqCounterEnabled = g_NESmapper.Mapper189.irq_enabled;
			mapper_data->irqLatchCounter = g_NESmapper.Mapper189.irq_latch;
			mapper_data->last8000Write = g_NESmapper.Mapper189.regs[0];
		}
		break;

	case 226:
		{
			struct mapper226Data* mapper_data = (struct mapper226Data*)&block->extraData;

			mapper_data->registers[0] = g_NESmapper.Mapper226.regs[0];
			mapper_data->registers[1] = g_NESmapper.Mapper226.regs[1];
		}
		break;

	case 230:
		{
			struct mapper230Data* mapper_data = (struct mapper230Data*)&block->extraData;

			mapper_data->numberOfResets = g_NESmapper.Mapper230.rom_switch;
		}
		break;

	case 232:
		{
			struct mapper232Data* mapper_data = (struct mapper232Data*)&block->extraData;

			mapper_data->last9000Write = g_NESmapper.Mapper232.regs[0];
			mapper_data->lastA000Write = g_NESmapper.Mapper232.regs[1];
		}
		break;

	case 234:
		{
			struct mapper234Data* mapper_data = (struct mapper234Data*)&block->extraData;

			mapper_data->lastFF80Write = g_NESmapper.Mapper234.regs[0];
			mapper_data->lastFFE8Write = g_NESmapper.Mapper234.regs[1];
			mapper_data->lastFFC0Write = g_NESmapper.Mapper234.regs[2];
		}
		break;

	case 236:
		{
			struct mapper236Data* mapper_data = (struct mapper236Data*)&block->extraData;

			mapper_data->BankSelect = g_NESmapper.Mapper236.bank;
			mapper_data->MapperMode = g_NESmapper.Mapper236.mode;
		}
		break;

	case 243:
		{
			struct mapper243Data* mapper_data = (struct mapper243Data*)&block->extraData;

			mapper_data->registers[0] = g_NESmapper.Mapper243.regs[0];
			mapper_data->registers[1] = g_NESmapper.Mapper243.regs[1];
			mapper_data->registers[2] = g_NESmapper.Mapper243.regs[2];
			mapper_data->registers[3] = g_NESmapper.Mapper243.regs[3];
		}
		break;

	case 255:
		{
			struct mapper255Data* mapper_data = (struct mapper255Data*)&block->extraData;

			mapper_data->registers[0] = g_NESmapper.Mapper255.regs[0];
			mapper_data->registers[1] = g_NESmapper.Mapper255.regs[1];
			mapper_data->registers[2] = g_NESmapper.Mapper255.regs[2];
			mapper_data->registers[3] = g_NESmapper.Mapper255.regs[3];
		}
		break;
	}

	return 0;
}

static void extract_ExMPRD(const char* fn, NES* nes)
{
	int hFile = NES_fopen(fn, FILE_MODE_APPEND);

	if(NES_ROM_get_mapper_num() == 1 && NES_crc32() == 0xb8747abf)
	{
		// Best Play - Pro Yakyuu Special (J)

		// save WRAM
		NES_fputc('W',  hFile); NES_fputc('R',  hFile); NES_fputc('A',  hFile); NES_fputc('M',  hFile);
		NES_fputc(0x00, hFile); NES_fputc(0x00, hFile); NES_fputc(0x00, hFile); NES_fputc(0x01, hFile);
		NES_fputc(0x00, hFile); NES_fputc(0x01, hFile); NES_fputc(0x00, hFile); NES_fputc(0x00, hFile);
		NES_fwrite(&g_NES.SaveRAM[0x2000], 1, 0x2000, hFile);
	}
	else if(NES_ROM_get_mapper_num() == 5)
	{
		// save WRAM
		NES_fputc('W',  hFile); NES_fputc('R',  hFile); NES_fputc('A',  hFile); NES_fputc('M',  hFile);
		NES_fputc(0x00, hFile); NES_fputc(0x00, hFile); NES_fputc(0x00, hFile); NES_fputc(0x01, hFile);
		NES_fputc(0x00, hFile); NES_fputc(0x01, hFile); NES_fputc(0x00, hFile); NES_fputc(0x00, hFile);
		NES_fwrite(g_NESmapper.Mapper5.wram, 1, 0x10000, hFile);
	}
	else if(NES_ROM_get_mapper_num() == 20)
	{
		// save WRAM
		NES_fputc('W',  hFile); NES_fputc('R',  hFile); NES_fputc('A',  hFile); NES_fputc('M',  hFile);
		NES_fputc(0x00, hFile); NES_fputc(0x00, hFile); NES_fputc(0x00, hFile); NES_fputc(0x01, hFile);
		NES_fputc(0x00, hFile); NES_fputc(0x00, hFile); NES_fputc(0x80, hFile); NES_fputc(0x00, hFile);
		NES_fwrite(g_NESmapper.Mapper20.wram, 1, 0x8000, hFile);
		// write Disk #1 - #4
		NES_fputc('F',  hFile); NES_fputc('D',  hFile); NES_fputc('S',  hFile); NES_fputc('1',  hFile);
		NES_fputc(0x00, hFile); NES_fputc(0x00, hFile); NES_fputc(0x00, hFile); NES_fputc(0x01, hFile);
		NES_fputc(0x00, hFile); NES_fputc(0x00, hFile); NES_fputc(0xFF, hFile); NES_fputc(0xDC, hFile);
		NES_fwrite(g_NESmapper.Mapper20.disk, 65500*4, 1, hFile);

		NES_fputc('F',  hFile); NES_fputc('D',  hFile); NES_fputc('S',  hFile); NES_fputc('2',  hFile);
		NES_fputc(0x00, hFile); NES_fputc(0x00, hFile); NES_fputc(0x00, hFile); NES_fputc(0x01, hFile);
		NES_fputc(0x00, hFile); NES_fputc(0x00, hFile); NES_fputc(0xFF, hFile); NES_fputc(0xDC, hFile);

		NES_fputc('F',  hFile); NES_fputc('D',  hFile); NES_fputc('S',  hFile); NES_fputc('3',  hFile);
		NES_fputc(0x00, hFile); NES_fputc(0x00, hFile); NES_fputc(0x00, hFile); NES_fputc(0x01, hFile);
		NES_fputc(0x00, hFile); NES_fputc(0x00, hFile); NES_fputc(0xFF, hFile); NES_fputc(0xDC, hFile);

		NES_fputc('F',  hFile); NES_fputc('D',  hFile); NES_fputc('S',  hFile); NES_fputc('4',  hFile);
		NES_fputc(0x00, hFile); NES_fputc(0x00, hFile); NES_fputc(0x00, hFile); NES_fputc(0x01, hFile);
		NES_fputc(0x00, hFile); NES_fputc(0x00, hFile); NES_fputc(0xFF, hFile); NES_fputc(0xDC, hFile);
	}
	NES_fclose(hFile);
}

static int extract_CNTR(SnssControllersBlock* block, NES* nes)
{
	return -1;
}

static int extract_SOUN(SnssSoundBlock* block, NES* nes)
{
	// get sound registers
	NES_APU_get_regs(block->soundRegisters);

	return 0;
}

boolean SaveSNSS(const char* fn, NES* nes)
{
	SNSS_FILE* snssFile;
	DEBUG("Enter SaveSNSS");

	if(SNSS_OK != SNSS_OpenFile(&snssFile, fn, SNSS_OPEN_WRITE))
		goto OpenError;

	// write BASR
	if(!extract_BASR(&snssFile->baseBlock, nes))
	{
		if(SNSS_OK != SNSS_WriteBlock(snssFile, SNSS_BASR))
			goto WriteError;
	}

	// write VRAM
	if(!extract_VRAM(&snssFile->vramBlock, nes))
	{
		if(SNSS_OK != SNSS_WriteBlock(snssFile, SNSS_VRAM))
			goto WriteError;
	}

	// write SRAM
	if(!extract_SRAM(&snssFile->sramBlock, nes))
	{
		if(SNSS_OK != SNSS_WriteBlock(snssFile, SNSS_SRAM))
			goto WriteError;
	}

	// write MPRD
	if(!extract_MPRD(&snssFile->mapperBlock, nes))
	{
		if(SNSS_OK != SNSS_WriteBlock(snssFile, SNSS_MPRD))
			goto WriteError;
	}

	// write CNTR
	if(!extract_CNTR(&snssFile->contBlock, nes))
	{
		if(SNSS_OK != SNSS_WriteBlock(snssFile, SNSS_CNTR))
			goto WriteError;
	}

	// write SOUN
	if(!extract_SOUN(&snssFile->soundBlock, nes))
	{
		if(SNSS_OK != SNSS_WriteBlock(snssFile, SNSS_SOUN))
			goto WriteError;
	}

	if(SNSS_OK != SNSS_CloseFile(&snssFile))
		goto CloseError;

	// write Extra Mapper Data
	extract_ExMPRD(fn, nes);

	DEBUG("Saved done");

	return TRUE;
CloseError:
WriteError:
OpenError:
	DEBUG("Error writing");
	if(snssFile) SNSS_CloseFile(&snssFile);
	return FALSE;
}


inline void SetStateChunkHeader(uint32 *Memp, int Code, int Size)
{
	uint32 Buf[4];
	Buf[0] = Code;
	Buf[1] = Size;
	Buf[2] = 0;
	Buf[3] = 0;
	_memcpy(Memp, Buf, sizeof(uint32)*4);
}


static int extractMem_ExMPRD(unsigned char **extm, uint32 nsize, NES* nes)
{
	int size=0;
	unsigned char *extp;

	if(NES_ROM_get_mapper_num() == 1 && NES_crc32() == 0xb8747abf)
	{
		// Best Play - Pro Yakyuu Special (J)
		// save WRAM
//		tmem = (unsigned char *)realloc(*extm, nsize + 0x2000+0x10);
//		if(tmem==NULL)
//			return 0;
		extp=*extm+nsize;
		SetStateChunkHeader((uint32 *)extp, 0x44504D45, 0x2000);
		_memcpy(extm+0x10, &g_NES.SaveRAM[0x2000], 0x2000);
		size = 0x2000+0x10;
	}
	else if(NES_ROM_get_mapper_num() == 5)
	{
		// save WRAM
//		tmem = (unsigned char *)realloc(*extm, nsize + 0x10000+0x10);
//		if(tmem==NULL)
//			return 0;
		extp=*extm+nsize;
		SetStateChunkHeader((uint32 *)extp, 0x44504D45, 0x10000);
		_memcpy(extp+0x10, g_NESmapper.Mapper5.wram, 0x10000);
		size = 0x10000+0x10;
	}
	else if(NES_ROM_get_mapper_num() == 20)
	{
		// save WRAM
//		tmem = (unsigned char *)realloc(*extm, nsize + 0x8000+(65500*4)+0x10);
//		if(tmem==NULL)
//			return 0;
		extp=*extm+nsize;
		SetStateChunkHeader((uint32 *)extp, 0x44504D45, 0x8000+(65500*4));
		_memcpy(extp+0x10, g_NESmapper.Mapper20.wram, 0x8000);
		extp+=0x10;

		// write Disk #1 - #4
		_memcpy(&extp[0x8000], g_NESmapper.Mapper20.disk, 65500);
		_memcpy(&extp[0x8000+65500], &g_NESmapper.Mapper20.disk[0x10000], 65500);
		_memcpy(&extp[0x8000+65500*2], &g_NESmapper.Mapper20.disk[0x20000], 65500);
		_memcpy(&extp[0x8000+65500*3], &g_NESmapper.Mapper20.disk[0x30000], 65500);
		size = 0x8000+65500*4+0x10;
	}
	else
		return 0;
//	*extm=tmem;
	return size;
}

static int adoptMem_ExMPRD(unsigned char *extm, uint32 nsize, NES* nes)
{
	//  uint32 i;
	int size=0;

	if(extm==NULL)
		return 0;
	if(NES_ROM_get_mapper_num() == 1 && NES_crc32() == 0xb8747abf)
	{
		// Best Play - Pro Yakyuu Special (J)
		// copy WRAM
		_memcpy(&g_NES.SaveRAM[0x2000],extm, 0x2000);
		size = 0x2000;
	}
	else if(NES_ROM_get_mapper_num() == 5)
	{
		// copy WRAM
		_memcpy(g_NESmapper.Mapper5.wram, extm, 0x10000);
		_memcpy(g_NES.SaveRAM, extm, 0x10000);
		size = 0x10000;
	}
	else if(NES_ROM_get_mapper_num() == 20)
	{
		// copy WRAM
		_memcpy(g_NESmapper.Mapper20.wram, extm, 0x8000);
		// read Disk #1 - #4
		_memcpy(g_NESmapper.Mapper20.disk, &extm[0x8000], 65500);
		_memcpy(&g_NESmapper.Mapper20.disk[0x10000], &extm[0x8000+65500], 65500);
		_memcpy(&g_NESmapper.Mapper20.disk[0x20000], &extm[0x8000+65500*2], 65500);
		_memcpy(&g_NESmapper.Mapper20.disk[0x30000], &extm[0x8000+65500*3], 65500);
		size = 0x8000+65500*4;
	}
	return size;
}

// rom load 時のバッファを流用
extern uint8 g_ReadRomImage[];

int MemSaveSNSS(unsigned char **ssp, NES* nes)
{
	unsigned char *snss=g_ReadRomImage, *p;
	int ssize=0;

//	if(*ssp){
//		free(*ssp);
//		*ssp=NULL;
//	}
//	snss = (unsigned char *)malloc(0x20+0x60+sizeof(SnssBaseBlock)+sizeof(SnssVramBlock)+sizeof(SnssSramBlock)+sizeof(SnssMapperBlock)+sizeof(SnssControllersBlock)+sizeof(SnssSoundBlock));
//	if(snss==NULL)
//		return 0;
	_memset(snss, 0, 0x10+sizeof(SnssBaseBlock)+sizeof(SnssVramBlock)+sizeof(SnssSramBlock)+sizeof(SnssMapperBlock)+sizeof(SnssControllersBlock)+sizeof(SnssSoundBlock));
	_memset(snss, 0, 0x20);
	p = snss+0x20;

	SetStateChunkHeader((uint32 *)p, 0x52534142, sizeof(SnssBaseBlock));
	if(!extract_BASR((SnssBaseBlock *)(p+0x10), nes)){
		ssize+=(sizeof(SnssBaseBlock)+0x10);
		p+=(sizeof(SnssBaseBlock)+0x10);
	}
	SetStateChunkHeader((uint32 *)p, 0x4D415256, sizeof(SnssVramBlock));
	if(!extract_VRAM((SnssVramBlock *)(p+0x10), nes)){
		ssize+=(sizeof(SnssVramBlock)+0x10);
		p+=(sizeof(SnssVramBlock)+0x10);
	}
	SetStateChunkHeader((uint32 *)p, 0x4D415253, sizeof(SnssSramBlock));
	if(!extract_SRAM((SnssSramBlock *)(p+0x10), nes)){
		ssize+=(sizeof(SnssSramBlock)+0x10);
		p+=(sizeof(SnssSramBlock)+0x10);
	}
	SetStateChunkHeader((uint32 *)p, 0x4452504D, sizeof(SnssMapperBlock));
	if(!extract_MPRD((SnssMapperBlock *)(p+0x10), nes)){
		ssize+=(sizeof(SnssMapperBlock)+0x10);
		p+=(sizeof(SnssMapperBlock)+0x10);
	}
	SetStateChunkHeader((uint32 *)p, 0x52544E43, sizeof(SnssControllersBlock));
	if(!extract_CNTR((SnssControllersBlock *)(p+0x10), nes)){
		ssize+=(sizeof(SnssControllersBlock)+0x10);
		p+=(sizeof(SnssControllersBlock)+0x10);
	}
	SetStateChunkHeader((uint32 *)p, 0x4E554F53, sizeof(SnssSoundBlock));
	if(!extract_SOUN((SnssSoundBlock *)(p+0x10), nes)){
		ssize+=(sizeof(SnssSoundBlock)+0x10);
		p+=(sizeof(SnssSoundBlock)+0x10);
	}
//	snss = (unsigned char *)realloc(snss, ssize+0x20);
	ssize += extractMem_ExMPRD(&snss, ssize+0x20,nes);
	*((uint32 *)(void *)&snss[0]) = 0x54445453;
	*((uint32 *)(void *)&snss[4]) = ssize;
	*ssp = snss;

	return ssize;
}


int MemLoadSNSS(unsigned char *ssp , NES* nes){
	unsigned char *snss = ssp;
	uint32 tsize, lsize=0;
	uint32 snssHeader[2];
	if(ssp==NULL) return 0;
	if(*(uint32 *)&ssp[0]!=0x54445453) return 0;
	tsize = *((uint32 *)&ssp[4])+0x20;
	snss+=0x20;
	lsize = 0x20;
	while(lsize < tsize){
		_memcpy(snssHeader, snss, sizeof(uint32)*2);
		switch(snssHeader[0]){
			case 0x52534142:
				adopt_BASR((SnssBaseBlock *)(snss+0x10), nes);
				break;
			case 0x4D415256:
				adopt_VRAM((SnssVramBlock *)(snss+0x10), nes);
				break;
			case 0x4D415253:
				adopt_SRAM((SnssSramBlock *)(snss+0x10), nes);
				break;
			case 0x4452504D:
				adopt_MPRD((SnssMapperBlock *)(snss+0x10), nes);
				break;
			case 0x52544E43:
				adopt_CNTR((SnssControllersBlock *)(snss+0x10), nes);
				break;
			case 0x4E554F53:
				adopt_SOUN((SnssSoundBlock *)(snss+0x10), nes);
				break;
			case 0x44504D45:
				adoptMem_ExMPRD(snss+0x10, *(uint32 *)&snss[4], nes);
				break;
		}
		uint32 tdw = snssHeader[1]+0x10;
		snss+=tdw;
		lsize+=tdw;

//		snss+=((*(uint32 *)&snss[4])+0x10);
//		lsize+=((*(uint32 *)&snss[4])+0x10);

	}
	return lsize;
}

//davex
int save_snss(unsigned char *stor, NES *nes, int save_size){

	unsigned char *beg_stor = stor;
	unsigned char *p;
	int ssize = 0;

	_memset(stor, 0, save_size);//se ponen a cero todas las estructuras
	p = stor+0x20;//se avanza el apuntador

	SetStateChunkHeader((uint32 *)p, 0x52534142, sizeof(SnssBaseBlock));
	if(!extract_BASR((SnssBaseBlock *)(p+0x10), nes)){
		ssize+=(sizeof(SnssBaseBlock)+0x10);
		p+=(sizeof(SnssBaseBlock)+0x10);
	}


	SetStateChunkHeader((uint32 *)p, 0x4D415256, sizeof(SnssVramBlock));
	if(!extract_VRAM((SnssVramBlock *)(p+0x10), nes)){
		ssize+=(sizeof(SnssVramBlock)+0x10);
		p+=(sizeof(SnssVramBlock)+0x10);
	}


	SetStateChunkHeader((uint32 *)p, 0x4D415253, sizeof(SnssSramBlock));
	if(!extract_SRAM((SnssSramBlock *)(p+0x10), nes)){
		ssize+=(sizeof(SnssSramBlock)+0x10);
		p+=(sizeof(SnssSramBlock)+0x10);
	}


	SetStateChunkHeader((uint32 *)p, 0x4452504D, sizeof(SnssMapperBlock));
	if(!extract_MPRD((SnssMapperBlock *)(p+0x10), nes)){
		ssize+=(sizeof(SnssMapperBlock)+0x10);
		p+=(sizeof(SnssMapperBlock)+0x10);
	}


	SetStateChunkHeader((uint32 *)p, 0x52544E43, sizeof(SnssControllersBlock));
	if(!extract_CNTR((SnssControllersBlock *)(p+0x10), nes)){
		ssize+=(sizeof(SnssControllersBlock)+0x10);
		p+=(sizeof(SnssControllersBlock)+0x10);
	}



	SetStateChunkHeader((uint32 *)p, 0x4E554F53, sizeof(SnssSoundBlock));
	if(!extract_SOUN((SnssSoundBlock *)(p+0x10), nes)){
		ssize+=(sizeof(SnssSoundBlock)+0x10);
		p+=(sizeof(SnssSoundBlock)+0x10);
	}


	ssize += extractMem_ExMPRD(&beg_stor, ssize+0x20,nes);
	*((uint32 *)(void *)&beg_stor[0]) = 0x54445453;
	*((uint32 *)(void *)&beg_stor[4]) = ssize;

	return ssize;
}
