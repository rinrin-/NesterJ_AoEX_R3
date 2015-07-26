
/////////////////////////////////////////////////////////////////////
// Mapper 112
void NES_mapper112_Init()
{
	g_NESmapper.Reset = NES_mapper112_Reset;
	g_NESmapper.MemoryWrite = NES_mapper112_MemoryWrite;
	g_NESmapper.HSync = NES_mapper112_HSync;
}

void NES_mapper112_Reset()
{
	// clear registers FIRST!!!
	int i;
	for(i = 0; i < 8; i++) g_NESmapper.Mapper112.regs[i] = 0x00;

	// set CPU bank pointers
	g_NESmapper.Mapper112.prg0 = 0;
	g_NESmapper.Mapper112.prg1 = 1;
	NES_mapper112_MMC3_set_CPU_banks();

	// set VROM banks
	if(g_NESmapper.num_1k_VROM_banks)
	{
		g_NESmapper.Mapper112.chr01 = 0;
		g_NESmapper.Mapper112.chr23 = 2;
		g_NESmapper.Mapper112.chr4  = 4;
		g_NESmapper.Mapper112.chr5  = 5;
		g_NESmapper.Mapper112.chr6  = 6;
		g_NESmapper.Mapper112.chr7  = 7;
		NES_mapper112_MMC3_set_PPU_banks();
	}
	else
	{
		g_NESmapper.Mapper112.chr01 = g_NESmapper.Mapper112.chr23 = g_NESmapper.Mapper112.chr4 = g_NESmapper.Mapper112.chr5 = g_NESmapper.Mapper112.chr6 = g_NESmapper.Mapper112.chr7 = 0;
	}

	g_NESmapper.Mapper112.irq_enabled = 0;
	g_NESmapper.Mapper112.irq_counter = 0;
	g_NESmapper.Mapper112.irq_latch = 0;
}

void NES_mapper112_MemoryWrite(uint32 addr, uint8 data)
{
	switch(addr & 0xE001)
	{
	case 0x8000:
		{
			g_NESmapper.Mapper112.regs[0] = data;
			NES_mapper112_MMC3_set_PPU_banks();
			NES_mapper112_MMC3_set_CPU_banks();
		}
		break;

	case 0xA000:
		{
			uint32 bank_num;
			g_NESmapper.Mapper112.regs[1] = data;
			bank_num = g_NESmapper.Mapper112.regs[1];
			switch(g_NESmapper.Mapper112.regs[0] & 0x07)
			{
			case 0x02:
				{
					if(g_NESmapper.num_1k_VROM_banks)
					{
						bank_num &= 0xfe;
						g_NESmapper.Mapper112.chr01 = bank_num;
						NES_mapper112_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x03:
				{
					if(g_NESmapper.num_1k_VROM_banks)
					{
						bank_num &= 0xfe;
						g_NESmapper.Mapper112.chr23 = bank_num;
						NES_mapper112_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x04:
				{
					if(g_NESmapper.num_1k_VROM_banks)
					{
						g_NESmapper.Mapper112.chr4 = bank_num;
						NES_mapper112_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x05:
				{
					if(g_NESmapper.num_1k_VROM_banks)
					{
						g_NESmapper.Mapper112.chr5 = bank_num;
						NES_mapper112_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x06:
				{
					if(g_NESmapper.num_1k_VROM_banks)
					{
						g_NESmapper.Mapper112.chr6 = bank_num;
						NES_mapper112_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x07:
				{
					if(g_NESmapper.num_1k_VROM_banks)
					{
						g_NESmapper.Mapper112.chr7 = bank_num;
						NES_mapper112_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x00:
				{
					g_NESmapper.Mapper112.prg0 = bank_num;
					NES_mapper112_MMC3_set_CPU_banks();
				}
				break;

			case 0x01:
				{
					g_NESmapper.Mapper112.prg1 = bank_num;
					NES_mapper112_MMC3_set_CPU_banks();
				}
				break;
			}
		}
		break;

	case 0x8001:
		{
			g_NESmapper.Mapper112.regs[2] = data;
			if(NES_ROM_get_mirroring() != NES_PPU_MIRROR_FOUR_SCREEN)
			{
				if(data & 0x01)
				{
					g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
				}
				else
				{
					g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
				}
			}
		}
		break;

	case 0xA001:
		{
			g_NESmapper.Mapper112.regs[3] = data;
		}
		break;

	case 0xC000:
		{
			g_NESmapper.Mapper112.regs[4] = data;
			g_NESmapper.Mapper112.irq_counter = g_NESmapper.Mapper112.regs[4];
		}
		break;

	case 0xC001:
		{
			g_NESmapper.Mapper112.regs[5] = data;
			g_NESmapper.Mapper112.irq_latch = g_NESmapper.Mapper112.regs[5];
		}
		break;

	case 0xE000:
		{
			g_NESmapper.Mapper112.regs[6] = data;
			g_NESmapper.Mapper112.irq_enabled = 0;
			if(data)
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
			}
			else
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
			}
		}
		break;

	case 0xE001:
		{
			g_NESmapper.Mapper112.regs[7] = data;
			g_NESmapper.Mapper112.irq_enabled = 1;
		}
		break;
	}
}

void NES_mapper112_HSync(uint32 scanline)
{
	if(g_NESmapper.Mapper112.irq_enabled)
	{
		if((scanline >= 0) && (scanline <= 239))
		{
			if(NES_PPU_spr_enabled() || NES_PPU_bg_enabled())
			{
				if(!(g_NESmapper.Mapper112.irq_counter--))
				{
					g_NESmapper.Mapper112.irq_counter = g_NESmapper.Mapper112.irq_latch;
					NES6502_DoIRQ();
				}
			}
		}
	}
}

void NES_mapper112_MMC3_set_CPU_banks()
{
	if(g_NESmapper.Mapper112.regs[0] & 0x40)
	{
		g_NESmapper.set_CPU_banks4(g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.Mapper112.prg1,g_NESmapper.Mapper112.prg0,g_NESmapper.num_8k_ROM_banks-1);
	}
	else
	{
		g_NESmapper.set_CPU_banks4(g_NESmapper.Mapper112.prg0,g_NESmapper.Mapper112.prg1,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);
	}
}
void NES_mapper112_MMC3_set_PPU_banks()
{
	if(g_NESmapper.num_1k_VROM_banks)
	{
		if(g_NESmapper.Mapper112.regs[0] & 0x80)
		{
			g_NESmapper.set_PPU_banks8(g_NESmapper.Mapper112.chr4,g_NESmapper.Mapper112.chr5,g_NESmapper.Mapper112.chr6,g_NESmapper.Mapper112.chr7,g_NESmapper.Mapper112.chr01,g_NESmapper.Mapper112.chr01+1,g_NESmapper.Mapper112.chr23,g_NESmapper.Mapper112.chr23+1);
		}
		else
		{
			g_NESmapper.set_PPU_banks8(g_NESmapper.Mapper112.chr01,g_NESmapper.Mapper112.chr01+1,g_NESmapper.Mapper112.chr23,g_NESmapper.Mapper112.chr23+1,g_NESmapper.Mapper112.chr4,g_NESmapper.Mapper112.chr5,g_NESmapper.Mapper112.chr6,g_NESmapper.Mapper112.chr7);
		}
	}
}


#define MAP112_ROM(ptr)  (((ptr)-NES_ROM_get_ROM_banks())  >> 13)
#define MAP112_VROM(ptr) (((ptr)-NES_ROM_get_VROM_banks()) >> 10)

void NES_mapper112_SNSS_fixup()
{
	nes6502_context context;
	NES6502_GetContext(&context);

	g_NESmapper.Mapper112.prg0 = MAP112_ROM(context.mem_page[(g_NESmapper.Mapper112.regs[0] & 0x40) ? 6 : 4]);
	g_NESmapper.Mapper112.prg1 = MAP112_ROM(context.mem_page[5]);
	if(g_NESmapper.num_1k_VROM_banks)
	{
		if(g_NESmapper.Mapper112.regs[0] & 0x80)
		{
			g_NESmapper.Mapper112.chr01 = MAP112_VROM(g_PPU.PPU_VRAM_banks[4]);
			g_NESmapper.Mapper112.chr23 = MAP112_VROM(g_PPU.PPU_VRAM_banks[6]);
			g_NESmapper.Mapper112.chr4  = MAP112_VROM(g_PPU.PPU_VRAM_banks[0]);
			g_NESmapper.Mapper112.chr5  = MAP112_VROM(g_PPU.PPU_VRAM_banks[1]);
			g_NESmapper.Mapper112.chr6  = MAP112_VROM(g_PPU.PPU_VRAM_banks[2]);
			g_NESmapper.Mapper112.chr7  = MAP112_VROM(g_PPU.PPU_VRAM_banks[3]);
		}
		else
		{
			g_NESmapper.Mapper112.chr01 = MAP112_VROM(g_PPU.PPU_VRAM_banks[0]);
			g_NESmapper.Mapper112.chr23 = MAP112_VROM(g_PPU.PPU_VRAM_banks[2]);
			g_NESmapper.Mapper112.chr4  = MAP112_VROM(g_PPU.PPU_VRAM_banks[4]);
			g_NESmapper.Mapper112.chr5  = MAP112_VROM(g_PPU.PPU_VRAM_banks[5]);
			g_NESmapper.Mapper112.chr6  = MAP112_VROM(g_PPU.PPU_VRAM_banks[6]);
			g_NESmapper.Mapper112.chr7  = MAP112_VROM(g_PPU.PPU_VRAM_banks[7]);
		}
	}
}

/////////////////////////////////////////////////////////////////////

