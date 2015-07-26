#ifdef _NES_MAPPER_CPP_

/////////////////////////////////////////////////////////////////////
// Mapper 95
void NES_mapper95_Init()
{
	g_NESmapper.Reset = NES_mapper95_Reset;
	g_NESmapper.MemoryWrite = NES_mapper95_MemoryWrite;
}

void NES_mapper95_Reset()
{
	g_NESmapper.Mapper95.regs[0] = 0x00;

	// set CPU bank pointers
	g_NESmapper.Mapper95.prg0 = 0;
	g_NESmapper.Mapper95.prg1 = 1;
	NES_mapper95_MMC3_set_CPU_banks();

	// set VROM banks
	if(g_NESmapper.num_1k_VROM_banks)
	{
		g_NESmapper.Mapper95.chr01 = 0;
		g_NESmapper.Mapper95.chr23 = 2;
		g_NESmapper.Mapper95.chr4  = 4;
		g_NESmapper.Mapper95.chr5  = 5;
		g_NESmapper.Mapper95.chr6  = 6;
		g_NESmapper.Mapper95.chr7  = 7;
		NES_mapper95_MMC3_set_PPU_banks();
	}
	else
	{
		g_NESmapper.Mapper95.chr01 = g_NESmapper.Mapper95.chr23 = g_NESmapper.Mapper95.chr4 = g_NESmapper.Mapper95.chr5 = g_NESmapper.Mapper95.chr6 = g_NESmapper.Mapper95.chr7 = 0;
	}
}

void NES_mapper95_MemoryWrite(uint32 addr, uint8 data)
{
	switch(addr & 0xE001)
	{
	case 0x8000:
		{
			g_NESmapper.Mapper95.regs[0] = data;
			NES_mapper95_MMC3_set_PPU_banks();
			NES_mapper95_MMC3_set_CPU_banks();
		}
		break;

	case 0x8001:
		{
			uint32 bank_num;

			if(g_NESmapper.Mapper95.regs[0] <= 0x05)
			{
				if(data & 0x20)
				{
					g_NESmapper.set_mirroring(1,1,1,1);
				}
				else
				{
					g_NESmapper.set_mirroring(0,0,0,0);
				}
				data &= 0x1F;
			}

			bank_num = data;

			switch(g_NESmapper.Mapper95.regs[0] & 0x07)
			{
			case 0x00:
				{
					if(g_NESmapper.num_1k_VROM_banks)
					{
						bank_num &= 0xfe;
						g_NESmapper.Mapper95.chr01 = bank_num;
						NES_mapper95_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x01:
				{
					if(g_NESmapper.num_1k_VROM_banks)
					{
						bank_num &= 0xfe;
						g_NESmapper.Mapper95.chr23 = bank_num;
						NES_mapper95_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x02:
				{
					if(g_NESmapper.num_1k_VROM_banks)
					{
						g_NESmapper.Mapper95.chr4 = bank_num;
						NES_mapper95_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x03:
				{
					if(g_NESmapper.num_1k_VROM_banks)
					{
						g_NESmapper.Mapper95.chr5 = bank_num;
						NES_mapper95_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x04:
				{
					if(g_NESmapper.num_1k_VROM_banks)
					{
						g_NESmapper.Mapper95.chr6 = bank_num;
						NES_mapper95_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x05:
				{
					if(g_NESmapper.num_1k_VROM_banks)
					{
						g_NESmapper.Mapper95.chr7 = bank_num;
						NES_mapper95_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x06:
				{
					g_NESmapper.Mapper95.prg0 = bank_num;
					NES_mapper95_MMC3_set_CPU_banks();
				}
				break;

			case 0x07:
				{
					g_NESmapper.Mapper95.prg1 = bank_num;
					NES_mapper95_MMC3_set_CPU_banks();
				}
				break;
			}
		}
		break;
	}
}

void NES_mapper95_MMC3_set_CPU_banks()
{
	if(g_NESmapper.Mapper95.regs[0] & 0x40)
	{
		g_NESmapper.set_CPU_banks4(g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.Mapper95.prg1,g_NESmapper.Mapper95.prg0,g_NESmapper.num_8k_ROM_banks-1);
	}
	else
	{
		g_NESmapper.set_CPU_banks4(g_NESmapper.Mapper95.prg0,g_NESmapper.Mapper95.prg1,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);
	}
}

void NES_mapper95_MMC3_set_PPU_banks()
{
	if(g_NESmapper.num_1k_VROM_banks)
	{
		if(g_NESmapper.Mapper95.regs[0] & 0x80)
		{
			g_NESmapper.set_PPU_banks8(g_NESmapper.Mapper95.chr4,g_NESmapper.Mapper95.chr5,g_NESmapper.Mapper95.chr6,g_NESmapper.Mapper95.chr7,g_NESmapper.Mapper95.chr01,g_NESmapper.Mapper95.chr01+1,g_NESmapper.Mapper95.chr23,g_NESmapper.Mapper95.chr23+1);
		}
		else
		{
			g_NESmapper.set_PPU_banks8(g_NESmapper.Mapper95.chr01,g_NESmapper.Mapper95.chr01+1,g_NESmapper.Mapper95.chr23,g_NESmapper.Mapper95.chr23+1,g_NESmapper.Mapper95.chr4,g_NESmapper.Mapper95.chr5,g_NESmapper.Mapper95.chr6,g_NESmapper.Mapper95.chr7);
		}
	}
}

#define MAP95_ROM(ptr)  (((ptr)-NES_ROM_get_ROM_banks())  >> 13)
#define MAP95_VROM(ptr) (((ptr)-NES_ROM_get_VROM_banks()) >> 10)

void NES_mapper95_SNSS_fixup()
{
	nes6502_context context;
	NES6502_GetContext(&context);

	g_NESmapper.Mapper95.prg0 = MAP95_ROM(context.mem_page[(g_NESmapper.Mapper95.regs[0] & 0x40) ? 6 : 4]);
	g_NESmapper.Mapper95.prg1 = MAP95_ROM(context.mem_page[5]);
	if(g_NESmapper.num_1k_VROM_banks)
	{
		if(g_NESmapper.Mapper95.regs[0] & 0x80)
		{
			g_NESmapper.Mapper95.chr01 = MAP95_VROM(g_PPU.PPU_VRAM_banks[4]);
			g_NESmapper.Mapper95.chr23 = MAP95_VROM(g_PPU.PPU_VRAM_banks[6]);
			g_NESmapper.Mapper95.chr4  = MAP95_VROM(g_PPU.PPU_VRAM_banks[0]);
			g_NESmapper.Mapper95.chr5  = MAP95_VROM(g_PPU.PPU_VRAM_banks[1]);
			g_NESmapper.Mapper95.chr6  = MAP95_VROM(g_PPU.PPU_VRAM_banks[2]);
			g_NESmapper.Mapper95.chr7  = MAP95_VROM(g_PPU.PPU_VRAM_banks[3]);
		}
		else
		{
			g_NESmapper.Mapper95.chr01 = MAP95_VROM(g_PPU.PPU_VRAM_banks[0]);
			g_NESmapper.Mapper95.chr23 = MAP95_VROM(g_PPU.PPU_VRAM_banks[2]);
			g_NESmapper.Mapper95.chr4  = MAP95_VROM(g_PPU.PPU_VRAM_banks[4]);
			g_NESmapper.Mapper95.chr5  = MAP95_VROM(g_PPU.PPU_VRAM_banks[5]);
			g_NESmapper.Mapper95.chr6  = MAP95_VROM(g_PPU.PPU_VRAM_banks[6]);
			g_NESmapper.Mapper95.chr7  = MAP95_VROM(g_PPU.PPU_VRAM_banks[7]);
		}
	}
}
/////////////////////////////////////////////////////////////////////

#endif
