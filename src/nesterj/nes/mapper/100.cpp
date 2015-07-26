
/////////////////////////////////////////////////////////////////////
// Mapper 100
void NES_mapper100_Init()
{
	g_NESmapper.Reset = NES_mapper100_Reset;
	g_NESmapper.MemoryWrite = NES_mapper100_MemoryWrite;
	g_NESmapper.HSync = NES_mapper100_HSync;
}

void NES_mapper100_Reset()
{
	int i;
	// clear registers FIRST!!!
	for(i = 0; i < 8; i++) g_NESmapper.Mapper100.regs[i] = 0x00;

	// set CPU bank pointers
	g_NESmapper.Mapper100.prg0 = 0;
	g_NESmapper.Mapper100.prg1 = 1;
	g_NESmapper.Mapper100.prg2 = g_NESmapper.num_8k_ROM_banks - 2;
	g_NESmapper.Mapper100.prg3 = g_NESmapper.num_8k_ROM_banks - 1;
	NES_mapper100_MMC3_set_CPU_banks();

	// set VROM banks
	if(g_NESmapper.num_1k_VROM_banks)
	{
		g_NESmapper.Mapper100.chr0 = 0;
		g_NESmapper.Mapper100.chr1 = 1;
		g_NESmapper.Mapper100.chr2 = 2;
		g_NESmapper.Mapper100.chr3 = 3;
		g_NESmapper.Mapper100.chr4 = 4;
		g_NESmapper.Mapper100.chr5 = 5;
		g_NESmapper.Mapper100.chr6 = 6;
		g_NESmapper.Mapper100.chr7 = 7;
		NES_mapper100_MMC3_set_PPU_banks();
	}
	else
	{
		g_NESmapper.Mapper100.chr0 = g_NESmapper.Mapper100.chr2 = g_NESmapper.Mapper100.chr4 = g_NESmapper.Mapper100.chr5 = g_NESmapper.Mapper100.chr6 = g_NESmapper.Mapper100.chr7 = 0;
		g_NESmapper.Mapper100.chr1 = g_NESmapper.Mapper100.chr3 = 1;
	}

	g_NESmapper.Mapper100.irq_enabled = 0;
	g_NESmapper.Mapper100.irq_counter = 0;
	g_NESmapper.Mapper100.irq_latch = 0;
}

void NES_mapper100_MemoryWrite(uint32 addr, uint8 data)
{
	switch(addr & 0xE001)
	{
	case 0x8000:
		{
			g_NESmapper.Mapper100.regs[0] = data;
		}
		break;

	case 0x8001:
		{
			uint32 bank_num;
			g_NESmapper.Mapper100.regs[1] = data;
			bank_num = g_NESmapper.Mapper100.regs[1];

			switch(g_NESmapper.Mapper100.regs[0] & 0xC7)
			{
			case 0x00:
				{
					if(g_NESmapper.num_1k_VROM_banks)
					{
						bank_num &= 0xfe;
						g_NESmapper.Mapper100.chr0 = bank_num;
						g_NESmapper.Mapper100.chr1 = bank_num + 1;
						NES_mapper100_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x01:
				{
					if(g_NESmapper.num_1k_VROM_banks)
					{
						bank_num &= 0xfe;
						g_NESmapper.Mapper100.chr2 = bank_num;
						g_NESmapper.Mapper100.chr3 = bank_num + 1;
						NES_mapper100_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x02:
				{
					if(g_NESmapper.num_1k_VROM_banks)
					{
						g_NESmapper.Mapper100.chr4 = bank_num;
						NES_mapper100_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x03:
				{
					if(g_NESmapper.num_1k_VROM_banks)
					{
						g_NESmapper.Mapper100.chr5 = bank_num;
						NES_mapper100_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x04:
				{
					if(g_NESmapper.num_1k_VROM_banks)
					{
						g_NESmapper.Mapper100.chr6 = bank_num;
						NES_mapper100_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x05:
				{
					if(g_NESmapper.num_1k_VROM_banks)
					{
						g_NESmapper.Mapper100.chr7 = bank_num;
						NES_mapper100_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x06:
				{
					g_NESmapper.Mapper100.prg0 = bank_num;
					NES_mapper100_MMC3_set_CPU_banks();
				}
				break;

			case 0x07:
				{
					g_NESmapper.Mapper100.prg1 = bank_num;
					NES_mapper100_MMC3_set_CPU_banks();
				}
				break;

			case 0x46:
				{
					g_NESmapper.Mapper100.prg2 = bank_num;
					NES_mapper100_MMC3_set_CPU_banks();
				}
				break;

			case 0x47:
				{
					g_NESmapper.Mapper100.prg3 = bank_num;
					NES_mapper100_MMC3_set_CPU_banks();
				}
				break;

			case 0x80:
				{
					if(g_NESmapper.num_1k_VROM_banks)
					{
						bank_num &= 0xfe;
						g_NESmapper.Mapper100.chr4 = bank_num;
						g_NESmapper.Mapper100.chr5 = bank_num + 1;
						NES_mapper100_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x81:
				{
					if(g_NESmapper.num_1k_VROM_banks)
					{
						bank_num &= 0xfe;
						g_NESmapper.Mapper100.chr6 = bank_num;
						g_NESmapper.Mapper100.chr7 = bank_num + 1;
						NES_mapper100_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x82:
				{
					if(g_NESmapper.num_1k_VROM_banks)
					{
						g_NESmapper.Mapper100.chr0 = bank_num;
						NES_mapper100_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x83:
				{
					if(g_NESmapper.num_1k_VROM_banks)
					{
						g_NESmapper.Mapper100.chr1 = bank_num;
						NES_mapper100_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x84:
				{
					if(g_NESmapper.num_1k_VROM_banks)
					{
						g_NESmapper.Mapper100.chr2 = bank_num;
						NES_mapper100_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x85:
				{
					if(g_NESmapper.num_1k_VROM_banks)
					{
						g_NESmapper.Mapper100.chr3 = bank_num;
						NES_mapper100_MMC3_set_PPU_banks();
					}
				}
				break;
			}
		}
		break;

	case 0xA000:
		{
			g_NESmapper.Mapper100.regs[2] = data;

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
			g_NESmapper.Mapper100.regs[3] = data;
		}
		break;

	case 0xC000:
		g_NESmapper.Mapper100.regs[4] = data;
		g_NESmapper.Mapper100.irq_counter = g_NESmapper.Mapper100.regs[4];
		break;

	case 0xC001:
		g_NESmapper.Mapper100.regs[5] = data;
		g_NESmapper.Mapper100.irq_latch = g_NESmapper.Mapper100.regs[5];
		break;

	case 0xE000:
		g_NESmapper.Mapper100.regs[6] = data;
		g_NESmapper.Mapper100.irq_enabled = 0;
		break;

	case 0xE001:
		g_NESmapper.Mapper100.regs[7] = data;
		g_NESmapper.Mapper100.irq_enabled = 1;
		break;
	}
}

void NES_mapper100_MMC3_set_CPU_banks()
{
	g_NESmapper.set_CPU_banks4(g_NESmapper.Mapper100.prg0,g_NESmapper.Mapper100.prg1,g_NESmapper.Mapper100.prg2,g_NESmapper.Mapper100.prg3);
}

void NES_mapper100_MMC3_set_PPU_banks()
{
	if(g_NESmapper.num_1k_VROM_banks)
	{
		g_NESmapper.set_PPU_banks8(g_NESmapper.Mapper100.chr0,g_NESmapper.Mapper100.chr1,g_NESmapper.Mapper100.chr2,g_NESmapper.Mapper100.chr3,g_NESmapper.Mapper100.chr4,g_NESmapper.Mapper100.chr5,g_NESmapper.Mapper100.chr6,g_NESmapper.Mapper100.chr7);
	}
}

void NES_mapper100_HSync(uint32 scanline)
{
	if(g_NESmapper.Mapper100.irq_enabled)
	{
		if((scanline >= 0) && (scanline <= 239))
		{
			if(NES_PPU_spr_enabled() || NES_PPU_bg_enabled())
			{
				if(!(g_NESmapper.Mapper100.irq_counter--))
				{
					g_NESmapper.Mapper100.irq_counter = g_NESmapper.Mapper100.irq_latch;
					NES6502_DoIRQ();
				}
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////

