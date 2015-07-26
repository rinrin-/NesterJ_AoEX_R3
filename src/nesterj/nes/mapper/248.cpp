
/////////////////////////////////////////////////////////////////////
// Mapper 248
void NES_mapper248_Init()
{
	g_NESmapper.Reset = NES_mapper248_Reset;
	g_NESmapper.MemoryWriteSaveRAM = NES_mapper248_MemoryWriteSaveRAM;
	g_NESmapper.MemoryWrite = NES_mapper248_MemoryWrite;
	g_NESmapper.HSync = NES_mapper248_HSync;
}

void NES_mapper248_Reset()
{
	// clear registers FIRST!!!
	int i;
	for(i = 0; i < 8; i++) g_NESmapper.Mapper248.regs[i] = 0x00;

	// set CPU bank pointers
	g_NESmapper.Mapper248.prg0 = 0;
	g_NESmapper.Mapper248.prg1 = 1;
	NES_mapper248_MMC3_set_CPU_banks();

	// set VROM banks
	g_NESmapper.Mapper248.chr01 = 0;
	g_NESmapper.Mapper248.chr23 = 2;
	g_NESmapper.Mapper248.chr4  = 4;
	g_NESmapper.Mapper248.chr5  = 5;
	g_NESmapper.Mapper248.chr6  = 6;
	g_NESmapper.Mapper248.chr7  = 7;
	NES_mapper248_MMC3_set_PPU_banks();

	g_NESmapper.Mapper248.irq_enabled = 0;
	g_NESmapper.Mapper248.irq_counter = 0;
	g_NESmapper.Mapper248.irq_latch = 0;
}

void NES_mapper248_MemoryWriteSaveRAM(uint32 addr, uint8 data)
{
	g_NESmapper.set_CPU_banks4(2*data,2*data+1,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);
}

void NES_mapper248_MemoryWrite(uint32 addr, uint8 data)
{
	switch(addr & 0xE001)
	{
	case 0x8000:
		{
			g_NESmapper.Mapper248.regs[0] = data;
			NES_mapper248_MMC3_set_PPU_banks();
			NES_mapper248_MMC3_set_CPU_banks();
		}
		break;

	case 0x8001:
		{
			uint32 bank_num;

			g_NESmapper.Mapper248.regs[1] = data;
			bank_num = g_NESmapper.Mapper248.regs[1];

			switch(g_NESmapper.Mapper248.regs[0] & 0x07)
			{
			case 0x00:
				{
					//if(g_NESmapper.num_1k_VROM_banks)
					{
						bank_num &= 0xfe;
						g_NESmapper.Mapper248.chr01 = bank_num;
						NES_mapper248_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x01:
				{
					//if(g_NESmapper.num_1k_VROM_banks)
					{
						bank_num &= 0xfe;
						g_NESmapper.Mapper248.chr23 = bank_num;
						NES_mapper248_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x02:
				{
					//if(g_NESmapper.num_1k_VROM_banks)
					{
						g_NESmapper.Mapper248.chr4 = bank_num;
						NES_mapper248_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x03:
				{
					//if(g_NESmapper.num_1k_VROM_banks)
					{
						g_NESmapper.Mapper248.chr5 = bank_num;
						NES_mapper248_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x04:
				{
					//if(g_NESmapper.num_1k_VROM_banks)
					{
						g_NESmapper.Mapper248.chr6 = bank_num;
						NES_mapper248_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x05:
				{
					//if(g_NESmapper.num_1k_VROM_banks)
					{
						g_NESmapper.Mapper248.chr7 = bank_num;
						NES_mapper248_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x06:
				{
					g_NESmapper.Mapper248.prg0 = bank_num;
					NES_mapper248_MMC3_set_CPU_banks();
				}
				break;

			case 0x07:
				{
					g_NESmapper.Mapper248.prg1 = bank_num;
					NES_mapper248_MMC3_set_CPU_banks();
				}
				break;
			}
		}
		break;

	case 0xA000:
		{
			g_NESmapper.Mapper248.regs[2] = data;

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
		{
			g_NESmapper.Mapper248.irq_counter = g_NESmapper.Mapper248.irq_latch = 0xbe;
			g_NESmapper.Mapper248.irq_enabled = 0;
		}
		break;

	case 0xC001:
		{
			g_NESmapper.Mapper248.irq_counter = g_NESmapper.Mapper248.irq_latch = 0xbe;
			g_NESmapper.Mapper248.irq_enabled = 1;
		}
		break;

	case 0xE000:
		{
			g_NESmapper.Mapper248.irq_enabled = 0;
		}
		break;

	case 0xE001:
		{
			g_NESmapper.Mapper248.irq_enabled = 1;
		}
		break;
	}
}

void NES_mapper248_HSync(uint32 scanline)
{
	if(g_NESmapper.Mapper248.irq_enabled)
	{
		if((scanline >= 0) && (scanline <= 239))
		{
			if(NES_PPU_spr_enabled() || NES_PPU_bg_enabled())
			{
				if(!(g_NESmapper.Mapper248.irq_counter--))
				{
					g_NESmapper.Mapper248.irq_counter = g_NESmapper.Mapper248.irq_latch;
					NES6502_DoIRQ();
				}
			}
		}
	}
}

void NES_mapper248_MMC3_set_CPU_banks()
{
	if(g_NESmapper.Mapper248.regs[0] & 0x40)
	{
		g_NESmapper.set_CPU_banks4(g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.Mapper248.prg1,g_NESmapper.Mapper248.prg0,g_NESmapper.num_8k_ROM_banks-1);
	}
	else
	{
		g_NESmapper.set_CPU_banks4(g_NESmapper.Mapper248.prg0,g_NESmapper.Mapper248.prg1,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);
	}
}

void NES_mapper248_MMC3_set_PPU_banks()
{
	if(g_NESmapper.Mapper248.regs[0] & 0x80)
	{
		g_NESmapper.set_PPU_banks8(g_NESmapper.Mapper248.chr4,g_NESmapper.Mapper248.chr5,g_NESmapper.Mapper248.chr6,g_NESmapper.Mapper248.chr7,g_NESmapper.Mapper248.chr01,g_NESmapper.Mapper248.chr01+1,g_NESmapper.Mapper248.chr23,g_NESmapper.Mapper248.chr23+1);
	}
	else
	{
		g_NESmapper.set_PPU_banks8(g_NESmapper.Mapper248.chr01,g_NESmapper.Mapper248.chr01+1,g_NESmapper.Mapper248.chr23,g_NESmapper.Mapper248.chr23+1,g_NESmapper.Mapper248.chr4,g_NESmapper.Mapper248.chr5,g_NESmapper.Mapper248.chr6,g_NESmapper.Mapper248.chr7);
	}
}

#define MAP248_ROM(ptr)  (((ptr)-NES_ROM_get_ROM_banks())  >> 13)
#define MAP248_VROM(ptr) (((ptr)-NES_ROM_get_VROM_banks()) >> 10)

void NES_mapper248_SNSS_fixup() // HACK HACK HACK HACK
{
	nes6502_context context;
	NES6502_GetContext(&context);

	g_NESmapper.Mapper248.prg0 = MAP248_ROM(context.mem_page[(g_NESmapper.Mapper248.regs[0] & 0x40) ? 6 : 4]);
	g_NESmapper.Mapper248.prg1 = MAP248_ROM(context.mem_page[5]);

	if(g_NESmapper.Mapper248.regs[0] & 0x80)
	{
		g_NESmapper.Mapper248.chr01 = MAP248_VROM(g_PPU.PPU_VRAM_banks[4]);
		g_NESmapper.Mapper248.chr23 = MAP248_VROM(g_PPU.PPU_VRAM_banks[6]);
		g_NESmapper.Mapper248.chr4  = MAP248_VROM(g_PPU.PPU_VRAM_banks[0]);
		g_NESmapper.Mapper248.chr5  = MAP248_VROM(g_PPU.PPU_VRAM_banks[1]);
		g_NESmapper.Mapper248.chr6  = MAP248_VROM(g_PPU.PPU_VRAM_banks[2]);
		g_NESmapper.Mapper248.chr7  = MAP248_VROM(g_PPU.PPU_VRAM_banks[3]);
	}
	else
	{
		g_NESmapper.Mapper248.chr01 = MAP248_VROM(g_PPU.PPU_VRAM_banks[0]);
		g_NESmapper.Mapper248.chr23 = MAP248_VROM(g_PPU.PPU_VRAM_banks[2]);
		g_NESmapper.Mapper248.chr4  = MAP248_VROM(g_PPU.PPU_VRAM_banks[4]);
		g_NESmapper.Mapper248.chr5  = MAP248_VROM(g_PPU.PPU_VRAM_banks[5]);
		g_NESmapper.Mapper248.chr6  = MAP248_VROM(g_PPU.PPU_VRAM_banks[6]);
		g_NESmapper.Mapper248.chr7  = MAP248_VROM(g_PPU.PPU_VRAM_banks[7]);
	}
}
/////////////////////////////////////////////////////////////////////

