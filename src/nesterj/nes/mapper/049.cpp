
/////////////////////////////////////////////////////////////////////
// Mapper 49
void NES_mapper49_Init()
{
	g_NESmapper.Reset = NES_mapper49_Reset;
	g_NESmapper.MemoryWriteSaveRAM = NES_mapper49_MemoryWriteSaveRAM;
	g_NESmapper.MemoryWrite = NES_mapper49_MemoryWrite;
	g_NESmapper.HSync = NES_mapper49_HSync;
}

void NES_mapper49_Reset()
{
	// set CPU bank pointers
	g_NESmapper.Mapper49.prg0 = 0;
	g_NESmapper.Mapper49.prg1 = 1;
	g_NESmapper.set_CPU_banks4(0,1,2,3);

	// set VROM banks
	g_NESmapper.Mapper49.chr01 = 0;
	g_NESmapper.Mapper49.chr23 = 2;
	g_NESmapper.Mapper49.chr4  = 4;
	g_NESmapper.Mapper49.chr5  = 5;
	g_NESmapper.Mapper49.chr6  = 6;
	g_NESmapper.Mapper49.chr7  = 7;
	g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);

	g_NESmapper.Mapper49.regs[0] = g_NESmapper.Mapper49.regs[1] = g_NESmapper.Mapper49.regs[2] = 0;
	g_NESmapper.Mapper49.irq_enabled = 0;
	g_NESmapper.Mapper49.irq_counter = 0;
	g_NESmapper.Mapper49.irq_latch = 0;
}

void NES_mapper49_MemoryWriteSaveRAM(uint32 addr, uint8 data)
{
	if(g_NESmapper.Mapper49.regs[2] & 0x80)
	{
		g_NESmapper.Mapper49.regs[1] = data;
		NES_mapper49_MMC3_set_CPU_banks();
		NES_mapper49_MMC3_set_PPU_banks();
	}
}

void NES_mapper49_MemoryWrite(uint32 addr, uint8 data)
{
	switch(addr & 0xE001)
	{
	case 0x8000:
		{
			if((data & 0x40) != (g_NESmapper.Mapper49.regs[0] & 0x40))
			{
				NES_mapper49_MMC3_set_CPU_banks();
			}
			if((data & 0x80) != (g_NESmapper.Mapper49.regs[0] & 0x80))
			{
				g_NESmapper.Mapper49.regs[0] = data;
				NES_mapper49_MMC3_set_PPU_banks();
			}
			g_NESmapper.Mapper49.regs[0] = data;
		}
		break;

	case 0x8001:
		{
			switch(g_NESmapper.Mapper49.regs[0] & 0x07)
			{
			case 0x00:
				{
					g_NESmapper.Mapper49.chr01 = data & 0xFE;
					NES_mapper49_MMC3_set_PPU_banks();
				}
				break;

			case 0x01:
				{
					g_NESmapper.Mapper49.chr23 = data & 0xFE;
					NES_mapper49_MMC3_set_PPU_banks();
				}
				break;

			case 0x02:
				{
					g_NESmapper.Mapper49.chr4 = data;
					NES_mapper49_MMC3_set_PPU_banks();
				}
				break;

			case 0x03:
				{
					g_NESmapper.Mapper49.chr5 = data;
					NES_mapper49_MMC3_set_PPU_banks();
				}
				break;

			case 0x04:
				{
					g_NESmapper.Mapper49.chr6 = data;
					NES_mapper49_MMC3_set_PPU_banks();
				}
				break;

			case 0x05:
				{
					g_NESmapper.Mapper49.chr7 = data;
					NES_mapper49_MMC3_set_PPU_banks();
				}
				break;

			case 0x06:
				{
					g_NESmapper.Mapper49.prg0 = data;
					NES_mapper49_MMC3_set_CPU_banks();
				}
				break;

			case 0x07:
				{
					g_NESmapper.Mapper49.prg1 = data;
					NES_mapper49_MMC3_set_CPU_banks();
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
			g_NESmapper.Mapper49.regs[2] = data;
		}
		break;

	case 0xC000:
		{
			g_NESmapper.Mapper49.irq_counter = data;
		}
		break;

	case 0xC001:
		{
			g_NESmapper.Mapper49.irq_latch = data;
		}
		break;

	case 0xE000:
		{
			g_NESmapper.Mapper49.irq_enabled = 0;
		}
		break;

	case 0xE001:
		{
			g_NESmapper.Mapper49.irq_enabled = 1;
		}
		break;
	}
}

void NES_mapper49_HSync(uint32 scanline)
{
	if(g_NESmapper.Mapper49.irq_enabled)
	{
		if((scanline >= 0) && (scanline <= 239))
		{
			if(NES_PPU_spr_enabled() || NES_PPU_bg_enabled())
			{
				if(!(g_NESmapper.Mapper49.irq_counter--))
				{
					g_NESmapper.Mapper49.irq_counter = g_NESmapper.Mapper49.irq_latch;
					NES6502_DoIRQ();
				}
			}
		}
	}
}

void NES_mapper49_MMC3_set_CPU_banks()
{
	if(g_NESmapper.Mapper49.regs[1] & 0x01)
	{
		if(g_NESmapper.Mapper49.regs[0] & 0x40)
		{
			g_NESmapper.set_CPU_bank4(((g_NESmapper.num_8k_ROM_banks - 1) & 0xE) | ((g_NESmapper.Mapper49.regs[1] & 0xC0) >> 2));
			g_NESmapper.set_CPU_bank5((g_NESmapper.Mapper49.prg1 & 0x0F) | ((g_NESmapper.Mapper49.regs[1] & 0xC0) >> 2));
			g_NESmapper.set_CPU_bank6((g_NESmapper.Mapper49.prg0 & 0x0F) | ((g_NESmapper.Mapper49.regs[1] & 0xC0) >> 2));
			g_NESmapper.set_CPU_bank7(((g_NESmapper.num_8k_ROM_banks - 1) & 0xF) | ((g_NESmapper.Mapper49.regs[1] & 0xC0) >> 2));
		}
		else
		{
			g_NESmapper.set_CPU_bank4((g_NESmapper.Mapper49.prg0 & 0x0F) | ((g_NESmapper.Mapper49.regs[1] & 0xC0) >> 2));
			g_NESmapper.set_CPU_bank5((g_NESmapper.Mapper49.prg1 & 0x0F) | ((g_NESmapper.Mapper49.regs[1] & 0xC0) >> 2));
			g_NESmapper.set_CPU_bank6(((g_NESmapper.num_8k_ROM_banks - 1) & 0xE) | ((g_NESmapper.Mapper49.regs[1] & 0xC0) >> 2));
			g_NESmapper.set_CPU_bank7(((g_NESmapper.num_8k_ROM_banks - 1) & 0xF) | ((g_NESmapper.Mapper49.regs[1] & 0xC0) >> 2));
		}
	}
	else
	{
		g_NESmapper.set_CPU_bank4(((g_NESmapper.Mapper49.regs[1] & 0x70) >> 2) | 0);
		g_NESmapper.set_CPU_bank5(((g_NESmapper.Mapper49.regs[1] & 0x70) >> 2) | 1);
		g_NESmapper.set_CPU_bank6(((g_NESmapper.Mapper49.regs[1] & 0x70) >> 2) | 2);
		g_NESmapper.set_CPU_bank7(((g_NESmapper.Mapper49.regs[1] & 0x70) >> 2) | 3);
	}
}

void NES_mapper49_MMC3_set_PPU_banks()
{
	g_NESmapper.Mapper49.chr01 = (g_NESmapper.Mapper49.chr01 & 0x7F) | ((g_NESmapper.Mapper49.regs[1] & 0xC0) << 1);
	g_NESmapper.Mapper49.chr23 = (g_NESmapper.Mapper49.chr23 & 0x7F) | ((g_NESmapper.Mapper49.regs[1] & 0xC0) << 1);
	g_NESmapper.Mapper49.chr4 = (g_NESmapper.Mapper49.chr4 & 0x7F) | ((g_NESmapper.Mapper49.regs[1] & 0xC0) << 1);
	g_NESmapper.Mapper49.chr5 = (g_NESmapper.Mapper49.chr5 & 0x7F) | ((g_NESmapper.Mapper49.regs[1] & 0xC0) << 1);
	g_NESmapper.Mapper49.chr6 = (g_NESmapper.Mapper49.chr6 & 0x7F) | ((g_NESmapper.Mapper49.regs[1] & 0xC0) << 1);
	g_NESmapper.Mapper49.chr7 = (g_NESmapper.Mapper49.chr7 & 0x7F) | ((g_NESmapper.Mapper49.regs[1] & 0xC0) << 1);

	if(g_NESmapper.Mapper49.regs[0] & 0x80)
	{
		g_NESmapper.set_PPU_banks8(g_NESmapper.Mapper49.chr4,g_NESmapper.Mapper49.chr5,g_NESmapper.Mapper49.chr6,g_NESmapper.Mapper49.chr7,g_NESmapper.Mapper49.chr01,g_NESmapper.Mapper49.chr01+1,g_NESmapper.Mapper49.chr23,g_NESmapper.Mapper49.chr23+1);
	}
	else
	{
		g_NESmapper.set_PPU_banks8(g_NESmapper.Mapper49.chr01,g_NESmapper.Mapper49.chr01+1,g_NESmapper.Mapper49.chr23,g_NESmapper.Mapper49.chr23+1,g_NESmapper.Mapper49.chr4,g_NESmapper.Mapper49.chr5,g_NESmapper.Mapper49.chr6,g_NESmapper.Mapper49.chr7);
	}
}

#define MAP49_ROM(ptr)  (((ptr)-NES_ROM_get_ROM_banks())  >> 13)
#define MAP49_VROM(ptr) (((ptr)-NES_ROM_get_VROM_banks()) >> 10)

void NES_mapper49_SNSS_fixup()
{
	nes6502_context context;
	NES6502_GetContext(&context);

	if(g_NESmapper.Mapper49.regs[1])
	{
		g_NESmapper.Mapper49.prg0 = MAP49_ROM(context.mem_page[(g_NESmapper.Mapper49.regs[0] & 0x40) ? 6 : 4]);
		g_NESmapper.Mapper49.prg1 = MAP49_ROM(context.mem_page[5]);
	}

	if(g_NESmapper.Mapper49.regs[0] & 0x80)
	{
		g_NESmapper.Mapper49.chr01 = MAP49_VROM(g_PPU.PPU_VRAM_banks[4]);
		g_NESmapper.Mapper49.chr23 = MAP49_VROM(g_PPU.PPU_VRAM_banks[6]);
		g_NESmapper.Mapper49.chr4  = MAP49_VROM(g_PPU.PPU_VRAM_banks[0]);
		g_NESmapper.Mapper49.chr5  = MAP49_VROM(g_PPU.PPU_VRAM_banks[1]);
		g_NESmapper.Mapper49.chr6  = MAP49_VROM(g_PPU.PPU_VRAM_banks[2]);
		g_NESmapper.Mapper49.chr7  = MAP49_VROM(g_PPU.PPU_VRAM_banks[3]);
	}
	else
	{
		g_NESmapper.Mapper49.chr01 = MAP49_VROM(g_PPU.PPU_VRAM_banks[0]);
		g_NESmapper.Mapper49.chr23 = MAP49_VROM(g_PPU.PPU_VRAM_banks[2]);
		g_NESmapper.Mapper49.chr4  = MAP49_VROM(g_PPU.PPU_VRAM_banks[4]);
		g_NESmapper.Mapper49.chr5  = MAP49_VROM(g_PPU.PPU_VRAM_banks[5]);
		g_NESmapper.Mapper49.chr6  = MAP49_VROM(g_PPU.PPU_VRAM_banks[6]);
		g_NESmapper.Mapper49.chr7  = MAP49_VROM(g_PPU.PPU_VRAM_banks[7]);
	}
}

/////////////////////////////////////////////////////////////////////

