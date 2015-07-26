
/////////////////////////////////////////////////////////////////////
// Mapper 52
void NES_mapper52_Init()
{
	g_NESmapper.Reset = NES_mapper52_Reset;
	g_NESmapper.MemoryWriteSaveRAM = NES_mapper52_MemoryWriteSaveRAM;
	g_NESmapper.MemoryWrite = NES_mapper52_MemoryWrite;
	g_NESmapper.HSync = NES_mapper52_HSync;
}

void NES_mapper52_Reset()
{
	// clear registers FIRST!!!
	int i;
	for(i = 0; i < 8; i++) g_NESmapper.Mapper52.regs[i] = 0x00;

	// set CPU bank pointers
	g_NESmapper.Mapper52.prg0 = 0;
	g_NESmapper.Mapper52.prg1 = 1;
	NES_mapper52_MMC3_set_CPU_banks();

	// set VROM banks
	if(g_NESmapper.num_1k_VROM_banks)
	{
		g_NESmapper.Mapper52.chr01 = 0;
		g_NESmapper.Mapper52.chr23 = 2;
		g_NESmapper.Mapper52.chr4  = 4;
		g_NESmapper.Mapper52.chr5  = 5;
		g_NESmapper.Mapper52.chr6  = 6;
		g_NESmapper.Mapper52.chr7  = 7;
		NES_mapper52_MMC3_set_PPU_banks();
	}

	g_NESmapper.Mapper52.irq_enabled = 0;
	g_NESmapper.Mapper52.irq_counter = 0;
	g_NESmapper.Mapper52.irq_latch = 0;
}

void NES_mapper52_MemoryWriteSaveRAM(uint32 addr, uint8 data)
{
	if(!g_NESmapper.Mapper52.regs[1])
	{
		g_NESmapper.Mapper52.regs[1] = 1;
		g_NESmapper.Mapper52.regs[2] = data;
		NES_mapper52_MMC3_set_PPU_banks();
		NES_mapper52_MMC3_set_CPU_banks();
	}
}

void NES_mapper52_MemoryWrite(uint32 addr, uint8 data)
{
	switch(addr & 0xE001)
	{
	case 0x8000:
		{
			g_NESmapper.Mapper52.regs[0] = data;
			NES_mapper52_MMC3_set_PPU_banks();
			NES_mapper52_MMC3_set_CPU_banks();
		}
		break;

	case 0x8001:
		{
			uint32 bank_num = data;

			switch(g_NESmapper.Mapper52.regs[0] & 0x07)
			{
			case 0x00:
				{
					if(g_NESmapper.num_1k_VROM_banks)
					{
						bank_num &= 0xfe;
						g_NESmapper.Mapper52.chr01 = bank_num;
						NES_mapper52_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x01:
				{
					if(g_NESmapper.num_1k_VROM_banks)
					{
						bank_num &= 0xfe;
						g_NESmapper.Mapper52.chr23 = bank_num;
						NES_mapper52_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x02:
				{
					if(g_NESmapper.num_1k_VROM_banks)
					{
						g_NESmapper.Mapper52.chr4 = bank_num;
						NES_mapper52_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x03:
				{
					if(g_NESmapper.num_1k_VROM_banks)
					{
						g_NESmapper.Mapper52.chr5 = bank_num;
						NES_mapper52_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x04:
				{
					if(g_NESmapper.num_1k_VROM_banks)
					{
						g_NESmapper.Mapper52.chr6 = bank_num;
						NES_mapper52_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x05:
				{
					if(g_NESmapper.num_1k_VROM_banks)
					{
						g_NESmapper.Mapper52.chr7 = bank_num;
						NES_mapper52_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x06:
				{
					g_NESmapper.Mapper52.prg0 = bank_num;
					NES_mapper52_MMC3_set_CPU_banks();
				}
				break;

			case 0x07:
				{
					g_NESmapper.Mapper52.prg1 = bank_num;
					NES_mapper52_MMC3_set_CPU_banks();
				}
				break;
			}
		}
		break;

	case 0xA000:
		{
			if(NES_ROM_get_mirroring() != NES_PPU_MIRROR_FOUR_SCREEN)
			{
				if(data & 0x01)
				{
					g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
				}
				else
				{
					g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
				}
			}
		}
		break;

	case 0xA001:
		{
			if(data & 0x80)
			{
				// enable save RAM $6000-$7FFF
			}
			else
			{
				// disable save RAM $6000-$7FFF
			}
		}
		break;

	case 0xC000:
		g_NESmapper.Mapper52.irq_counter = data;
		break;

	case 0xC001:
		g_NESmapper.Mapper52.irq_latch = data;
		break;

	case 0xE000:
		g_NESmapper.Mapper52.irq_enabled = 0;
		break;

	case 0xE001:
		g_NESmapper.Mapper52.irq_enabled = 1;
		break;
	}
}

void NES_mapper52_HSync(uint32 scanline)
{
	if(g_NESmapper.Mapper52.irq_enabled)
	{
		if((scanline >= 0) && (scanline <= 239))
		{
			if(NES_PPU_spr_enabled() || NES_PPU_bg_enabled())
			{
				if(!(g_NESmapper.Mapper52.irq_counter--))
				{
					g_NESmapper.Mapper52.irq_counter = g_NESmapper.Mapper52.irq_latch;
					NES6502_DoIRQ();
				}
			}
		}
	}
}

void NES_mapper52_MMC3_set_CPU_banks()
{
	uint32 mask, bank;

	if(g_NESmapper.Mapper52.regs[2] & 0x08)
	{
		mask = 0x0f;
		bank = (g_NESmapper.Mapper52.regs[2] & 0x07) << 4;
	}
	else
	{
		mask = 0x1f;
		bank = (g_NESmapper.Mapper52.regs[2] & 0x06) << 4;
	}

	if(g_NESmapper.Mapper52.regs[0] & 0x40)
	{
		g_NESmapper.set_CPU_banks4(bank | (mask & (g_NESmapper.num_8k_ROM_banks-2)),
		              bank | (mask & g_NESmapper.Mapper52.prg1),
		              bank | (mask & g_NESmapper.Mapper52.prg0),
		              bank | (mask & (g_NESmapper.num_8k_ROM_banks-1)));
	}
	else
	{
		g_NESmapper.set_CPU_banks4(bank | (mask & g_NESmapper.Mapper52.prg0),
		              bank | (mask & g_NESmapper.Mapper52.prg1),
		              bank | (mask & (g_NESmapper.num_8k_ROM_banks-2)),
		              bank | (mask & (g_NESmapper.num_8k_ROM_banks-1)));
	}
}

void NES_mapper52_MMC3_set_PPU_banks()
{
	uint32 mask, bank;

	if(g_NESmapper.Mapper52.regs[2] & 0x40)
	{
		mask = 0x7f;
		bank = (((g_NESmapper.Mapper52.regs[2] & 0x20) >> 3) | ((g_NESmapper.Mapper52.regs[2] & 0x10) >> 4) | ((g_NESmapper.Mapper52.regs[2] & 0x04) >> 1)) << 7;
	}
	else
	{
		mask = 0xff;
		bank = (((g_NESmapper.Mapper52.regs[2] & 0x20) >> 3) | ((g_NESmapper.Mapper52.regs[2] & 0x10) >> 4)) << 7;
	}

	if(g_NESmapper.Mapper52.regs[0] & 0x80)
	{
		g_NESmapper.set_PPU_banks8(bank | (mask & g_NESmapper.Mapper52.chr4),
		              bank | (mask & g_NESmapper.Mapper52.chr5),
		              bank | (mask & g_NESmapper.Mapper52.chr6),
		              bank | (mask & g_NESmapper.Mapper52.chr7),
		              bank | (mask & g_NESmapper.Mapper52.chr01),
		              bank | (mask & (g_NESmapper.Mapper52.chr01+1)),
		              bank | (mask & g_NESmapper.Mapper52.chr23),
		              bank | (mask & (g_NESmapper.Mapper52.chr23+1)));
	}
	else
	{
		g_NESmapper.set_PPU_banks8(bank | (mask & g_NESmapper.Mapper52.chr01),
		              bank | (mask & (g_NESmapper.Mapper52.chr01+1)),
		              bank | (mask & g_NESmapper.Mapper52.chr23),
		              bank | (mask & (g_NESmapper.Mapper52.chr23+1)),
		              bank | (mask & g_NESmapper.Mapper52.chr4),
		              bank | (mask & g_NESmapper.Mapper52.chr5),
		              bank | (mask & g_NESmapper.Mapper52.chr6),
		              bank | (mask & g_NESmapper.Mapper52.chr7));
	}
}

#define MAP52_ROM(ptr)  (((ptr)-NES_ROM_get_ROM_banks())  >> 13)
#define MAP52_VROM(ptr) (((ptr)-NES_ROM_get_VROM_banks()) >> 10)

void NES_mapper52_SNSS_fixup() // HACK HACK HACK HACK
{
	nes6502_context context;
	NES6502_GetContext(&context);

	g_NESmapper.Mapper52.prg0 = MAP52_ROM(context.mem_page[(g_NESmapper.Mapper52.regs[0] & 0x40) ? 6 : 4]);
	g_NESmapper.Mapper52.prg1 = MAP52_ROM(context.mem_page[5]);
	if(g_NESmapper.num_1k_VROM_banks)
	{
		if(g_NESmapper.Mapper52.regs[0] & 0x80)
		{
			g_NESmapper.Mapper52.chr01 = MAP52_VROM(g_PPU.PPU_VRAM_banks[4]);
			g_NESmapper.Mapper52.chr23 = MAP52_VROM(g_PPU.PPU_VRAM_banks[6]);
			g_NESmapper.Mapper52.chr4  = MAP52_VROM(g_PPU.PPU_VRAM_banks[0]);
			g_NESmapper.Mapper52.chr5  = MAP52_VROM(g_PPU.PPU_VRAM_banks[1]);
			g_NESmapper.Mapper52.chr6  = MAP52_VROM(g_PPU.PPU_VRAM_banks[2]);
			g_NESmapper.Mapper52.chr7  = MAP52_VROM(g_PPU.PPU_VRAM_banks[3]);
		}
		else
		{
			g_NESmapper.Mapper52.chr01 = MAP52_VROM(g_PPU.PPU_VRAM_banks[0]);
			g_NESmapper.Mapper52.chr23 = MAP52_VROM(g_PPU.PPU_VRAM_banks[2]);
			g_NESmapper.Mapper52.chr4  = MAP52_VROM(g_PPU.PPU_VRAM_banks[4]);
			g_NESmapper.Mapper52.chr5  = MAP52_VROM(g_PPU.PPU_VRAM_banks[5]);
			g_NESmapper.Mapper52.chr6  = MAP52_VROM(g_PPU.PPU_VRAM_banks[6]);
			g_NESmapper.Mapper52.chr7  = MAP52_VROM(g_PPU.PPU_VRAM_banks[7]);
		}
	}
}
/////////////////////////////////////////////////////////////////////

