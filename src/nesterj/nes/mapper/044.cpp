
/////////////////////////////////////////////////////////////////////
// Mapper 44
void NES_mapper44_Init()
{
	g_NESmapper.Reset = NES_mapper44_Reset;
	g_NESmapper.MemoryWrite = NES_mapper44_MemoryWrite;
	g_NESmapper.HSync = NES_mapper44_HSync;
}

void NES_mapper44_Reset()
{
	// clear registers FIRST!!!
	int i;
	for(i = 0; i < 8; i++) g_NESmapper.Mapper44.regs[i] = 0x00;

	// set game select register
	g_NESmapper.Mapper44.rom_bank = 0;

	// set CPU bank pointers
	g_NESmapper.Mapper44.prg0 = 0;
	g_NESmapper.Mapper44.prg1 = 1;
	NES_mapper44_MMC3_set_CPU_banks();

	// set VROM banks
	if(g_NESmapper.num_1k_VROM_banks)
	{
		g_NESmapper.Mapper44.chr01 = 0;
		g_NESmapper.Mapper44.chr23 = 2;
		g_NESmapper.Mapper44.chr4  = 4;
		g_NESmapper.Mapper44.chr5  = 5;
		g_NESmapper.Mapper44.chr6  = 6;
		g_NESmapper.Mapper44.chr7  = 7;
		NES_mapper44_MMC3_set_PPU_banks();
	}
	else
	{
		g_NESmapper.Mapper44.chr01 = g_NESmapper.Mapper44.chr23 = g_NESmapper.Mapper44.chr4 = g_NESmapper.Mapper44.chr5 = g_NESmapper.Mapper44.chr6 = g_NESmapper.Mapper44.chr7 = 0;
	}

	g_NESmapper.Mapper44.irq_enabled = 0;
	g_NESmapper.Mapper44.irq_counter = 0;
	g_NESmapper.Mapper44.irq_latch = 0;
}

void NES_mapper44_MemoryWrite(uint32 addr, uint8 data)
{
	switch(addr & 0xE001)
	{
	case 0x8000:
		{
			g_NESmapper.Mapper44.regs[0] = data;
			NES_mapper44_MMC3_set_PPU_banks();
			NES_mapper44_MMC3_set_CPU_banks();
		}
		break;

	case 0x8001:
		{
			uint32 bank_num;
			g_NESmapper.Mapper44.regs[1] = data;
			bank_num = g_NESmapper.Mapper44.regs[1];

			switch(g_NESmapper.Mapper44.regs[0] & 0x07)
			{
			case 0x00:
				{
					if(g_NESmapper.num_1k_VROM_banks)
					{
						bank_num &= 0xfe;
						g_NESmapper.Mapper44.chr01 = bank_num;
						NES_mapper44_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x01:
				{
					if(g_NESmapper.num_1k_VROM_banks)
					{
						bank_num &= 0xfe;
						g_NESmapper.Mapper44.chr23 = bank_num;
						NES_mapper44_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x02:
				{
					if(g_NESmapper.num_1k_VROM_banks)
					{
						g_NESmapper.Mapper44.chr4 = bank_num;
						NES_mapper44_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x03:
				{
					if(g_NESmapper.num_1k_VROM_banks)
					{
						g_NESmapper.Mapper44.chr5 = bank_num;
						NES_mapper44_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x04:
				{
					if(g_NESmapper.num_1k_VROM_banks)
					{
						g_NESmapper.Mapper44.chr6 = bank_num;
						NES_mapper44_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x05:
				{
					if(g_NESmapper.num_1k_VROM_banks)
					{
						g_NESmapper.Mapper44.chr7 = bank_num;
						NES_mapper44_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x06:
				{
					g_NESmapper.Mapper44.prg0 = bank_num;
					NES_mapper44_MMC3_set_CPU_banks();
				}
				break;

			case 0x07:
				{
					g_NESmapper.Mapper44.prg1 = bank_num;
					NES_mapper44_MMC3_set_CPU_banks();
				}
				break;
			}
		}
		break;

	case 0xA000:
		{
			g_NESmapper.Mapper44.regs[2] = data;

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
			g_NESmapper.Mapper44.rom_bank = data & 0x07;
			if(g_NESmapper.Mapper44.rom_bank == 7)
			{
				g_NESmapper.Mapper44.rom_bank = 6;
			}
			NES_mapper44_MMC3_set_CPU_banks();
			NES_mapper44_MMC3_set_PPU_banks();
		}
		break;

	case 0xC000:
		g_NESmapper.Mapper44.regs[4] = data;
		g_NESmapper.Mapper44.irq_counter = g_NESmapper.Mapper44.regs[4];
		break;

	case 0xC001:
		g_NESmapper.Mapper44.regs[5] = data;
		g_NESmapper.Mapper44.irq_latch = g_NESmapper.Mapper44.regs[5];
		break;

	case 0xE000:
		g_NESmapper.Mapper44.regs[6] = data;
		g_NESmapper.Mapper44.irq_enabled = 0;
		break;

	case 0xE001:
		g_NESmapper.Mapper44.regs[7] = data;
		g_NESmapper.Mapper44.irq_enabled = 1;
		break;
	}
}

void NES_mapper44_HSync(uint32 scanline)
{
	if(g_NESmapper.Mapper44.irq_enabled)
	{
		if((scanline >= 0) && (scanline <= 239))
		{
			if(NES_PPU_spr_enabled() || NES_PPU_bg_enabled())
			{
				if(!(g_NESmapper.Mapper44.irq_counter--))
				{
					g_NESmapper.Mapper44.irq_counter = g_NESmapper.Mapper44.irq_latch;
					NES6502_DoIRQ();
				}
			}
		}
	}
}

void NES_mapper44_MMC3_set_CPU_banks()
{
	if(g_NESmapper.Mapper44.regs[0] & 0x40)
	{
		g_NESmapper.set_CPU_bank4(((g_NESmapper.Mapper44.rom_bank == 6) ? 0x1e : 0x0e) | (g_NESmapper.Mapper44.rom_bank << 4));
		g_NESmapper.set_CPU_bank5(((g_NESmapper.Mapper44.rom_bank == 6) ? 0x1f & g_NESmapper.Mapper44.prg1 : 0x0f & g_NESmapper.Mapper44.prg1) | (g_NESmapper.Mapper44.rom_bank << 4));
		g_NESmapper.set_CPU_bank6(((g_NESmapper.Mapper44.rom_bank == 6) ? 0x1f & g_NESmapper.Mapper44.prg0 : 0x0f & g_NESmapper.Mapper44.prg0) | (g_NESmapper.Mapper44.rom_bank << 4));
		g_NESmapper.set_CPU_bank7(((g_NESmapper.Mapper44.rom_bank == 6) ? 0x1f : 0x0f) | (g_NESmapper.Mapper44.rom_bank << 4));
	}
	else
	{
		g_NESmapper.set_CPU_bank4(((g_NESmapper.Mapper44.rom_bank == 6) ? 0x1f & g_NESmapper.Mapper44.prg0: 0x0f & g_NESmapper.Mapper44.prg0) | (g_NESmapper.Mapper44.rom_bank << 4));
		g_NESmapper.set_CPU_bank5(((g_NESmapper.Mapper44.rom_bank == 6) ? 0x1f & g_NESmapper.Mapper44.prg1: 0x0f & g_NESmapper.Mapper44.prg1) | (g_NESmapper.Mapper44.rom_bank << 4));
		g_NESmapper.set_CPU_bank6(((g_NESmapper.Mapper44.rom_bank == 6) ? 0x1e : 0x0e) | (g_NESmapper.Mapper44.rom_bank << 4));
		g_NESmapper.set_CPU_bank7(((g_NESmapper.Mapper44.rom_bank == 6) ? 0x1f : 0x0f) | (g_NESmapper.Mapper44.rom_bank << 4));
	}
}

void NES_mapper44_MMC3_set_PPU_banks()
{
	if(g_NESmapper.num_1k_VROM_banks)
	{
		if(g_NESmapper.Mapper44.regs[0] & 0x80)
		{
			g_NESmapper.set_PPU_bank0(((g_NESmapper.Mapper44.rom_bank == 6) ? 0xff & g_NESmapper.Mapper44.chr4 : 0x7f & g_NESmapper.Mapper44.chr4) | (g_NESmapper.Mapper44.rom_bank << 7));
			g_NESmapper.set_PPU_bank1(((g_NESmapper.Mapper44.rom_bank == 6) ? 0xff & g_NESmapper.Mapper44.chr5 : 0x7f & g_NESmapper.Mapper44.chr5) | (g_NESmapper.Mapper44.rom_bank << 7));
			g_NESmapper.set_PPU_bank2(((g_NESmapper.Mapper44.rom_bank == 6) ? 0xff & g_NESmapper.Mapper44.chr6 : 0x7f & g_NESmapper.Mapper44.chr6) | (g_NESmapper.Mapper44.rom_bank << 7));
			g_NESmapper.set_PPU_bank3(((g_NESmapper.Mapper44.rom_bank == 6) ? 0xff & g_NESmapper.Mapper44.chr7 : 0x7f & g_NESmapper.Mapper44.chr7) | (g_NESmapper.Mapper44.rom_bank << 7));
			g_NESmapper.set_PPU_bank4(((g_NESmapper.Mapper44.rom_bank == 6) ? 0xff & g_NESmapper.Mapper44.chr01 : 0x7f & g_NESmapper.Mapper44.chr01) | (g_NESmapper.Mapper44.rom_bank << 7));
			g_NESmapper.set_PPU_bank5(((g_NESmapper.Mapper44.rom_bank == 6) ? 0xff & (g_NESmapper.Mapper44.chr01+1) : 0x7f & (g_NESmapper.Mapper44.chr01+1)) | (g_NESmapper.Mapper44.rom_bank << 7));
			g_NESmapper.set_PPU_bank6(((g_NESmapper.Mapper44.rom_bank == 6) ? 0xff & g_NESmapper.Mapper44.chr23: 0x7f & g_NESmapper.Mapper44.chr23) | (g_NESmapper.Mapper44.rom_bank << 7));
			g_NESmapper.set_PPU_bank7(((g_NESmapper.Mapper44.rom_bank == 6) ? 0xff & (g_NESmapper.Mapper44.chr23+1) : 0x7f & (g_NESmapper.Mapper44.chr23+1)) | (g_NESmapper.Mapper44.rom_bank << 7));
		}
		else
		{
			g_NESmapper.set_PPU_bank0(((g_NESmapper.Mapper44.rom_bank == 6) ? 0xff & g_NESmapper.Mapper44.chr01 : 0x7f & g_NESmapper.Mapper44.chr01) | (g_NESmapper.Mapper44.rom_bank << 7));
			g_NESmapper.set_PPU_bank1(((g_NESmapper.Mapper44.rom_bank == 6) ? 0xff & (g_NESmapper.Mapper44.chr01+1) : 0x7f & (g_NESmapper.Mapper44.chr01+1)) | (g_NESmapper.Mapper44.rom_bank << 7));
			g_NESmapper.set_PPU_bank2(((g_NESmapper.Mapper44.rom_bank == 6) ? 0xff & g_NESmapper.Mapper44.chr23: 0x7f & g_NESmapper.Mapper44.chr23) | (g_NESmapper.Mapper44.rom_bank << 7));
			g_NESmapper.set_PPU_bank3(((g_NESmapper.Mapper44.rom_bank == 6) ? 0xff & (g_NESmapper.Mapper44.chr23+1) : 0x7f & (g_NESmapper.Mapper44.chr23+1)) | (g_NESmapper.Mapper44.rom_bank << 7));
			g_NESmapper.set_PPU_bank4(((g_NESmapper.Mapper44.rom_bank == 6) ? 0xff & g_NESmapper.Mapper44.chr4 : 0x7f & g_NESmapper.Mapper44.chr4) | (g_NESmapper.Mapper44.rom_bank << 7));
			g_NESmapper.set_PPU_bank5(((g_NESmapper.Mapper44.rom_bank == 6) ? 0xff & g_NESmapper.Mapper44.chr5 : 0x7f & g_NESmapper.Mapper44.chr5) | (g_NESmapper.Mapper44.rom_bank << 7));
			g_NESmapper.set_PPU_bank6(((g_NESmapper.Mapper44.rom_bank == 6) ? 0xff & g_NESmapper.Mapper44.chr6 : 0x7f & g_NESmapper.Mapper44.chr6) | (g_NESmapper.Mapper44.rom_bank << 7));
			g_NESmapper.set_PPU_bank7(((g_NESmapper.Mapper44.rom_bank == 6) ? 0xff & g_NESmapper.Mapper44.chr7 : 0x7f & g_NESmapper.Mapper44.chr7) | (g_NESmapper.Mapper44.rom_bank << 7));
		}
	}
}

/////////////////////////////////////////////////////////////////////

