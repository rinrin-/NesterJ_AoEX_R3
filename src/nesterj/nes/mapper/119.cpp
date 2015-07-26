
/////////////////////////////////////////////////////////////////////
// Mapper 119
void NES_mapper119_Init()
{
	g_NESmapper.Reset = NES_mapper119_Reset;
	g_NESmapper.MemoryWrite = NES_mapper119_MemoryWrite;
	g_NESmapper.HSync = NES_mapper119_HSync;
}

void NES_mapper119_Reset()
{
	// clear registers FIRST!!!
	int i;
	for(i = 0; i < 8; i++) g_NESmapper.Mapper119.regs[i] = 0x00;

	// set CPU bank pointers
	g_NESmapper.Mapper119.prg0 = 0;
	g_NESmapper.Mapper119.prg1 = 1;
	NES_mapper119_MMC3_set_CPU_banks();

	// set VROM banks
	g_NESmapper.Mapper119.chr01 = 0;
	g_NESmapper.Mapper119.chr23 = 2;
	g_NESmapper.Mapper119.chr4  = 4;
	g_NESmapper.Mapper119.chr5  = 5;
	g_NESmapper.Mapper119.chr6  = 6;
	g_NESmapper.Mapper119.chr7  = 7;
	NES_mapper119_MMC3_set_PPU_banks();

	g_NESmapper.Mapper119.irq_enabled = 0;
	g_NESmapper.Mapper119.irq_counter = 0;
	g_NESmapper.Mapper119.irq_latch = 0;

	g_PPU.vram_write_protect = 0;
}

void NES_mapper119_MemoryWrite(uint32 addr, uint8 data)
{
	switch(addr & 0xE001)
	{
	case 0x8000:
		{
			g_NESmapper.Mapper119.regs[0] = data;
			NES_mapper119_MMC3_set_PPU_banks();
			NES_mapper119_MMC3_set_CPU_banks();
		}
		break;

	case 0x8001:
		{
			uint32 bank_num;

			g_NESmapper.Mapper119.regs[1] = data;
			bank_num = g_NESmapper.Mapper119.regs[1];

			switch(g_NESmapper.Mapper119.regs[0] & 0x07)
			{
			case 0x00:
				{
					if(g_NESmapper.num_1k_VROM_banks)
					{
						bank_num &= 0xfe;
						g_NESmapper.Mapper119.chr01 = bank_num;
						NES_mapper119_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x01:
				{
					if(g_NESmapper.num_1k_VROM_banks)
					{
						bank_num &= 0xfe;
						g_NESmapper.Mapper119.chr23 = bank_num;
						NES_mapper119_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x02:
				{
					if(g_NESmapper.num_1k_VROM_banks)
					{
						g_NESmapper.Mapper119.chr4 = bank_num;
						NES_mapper119_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x03:
				{
					if(g_NESmapper.num_1k_VROM_banks)
					{
						g_NESmapper.Mapper119.chr5 = bank_num;
						NES_mapper119_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x04:
				{
					if(g_NESmapper.num_1k_VROM_banks)
					{
						g_NESmapper.Mapper119.chr6 = bank_num;
						NES_mapper119_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x05:
				{
					if(g_NESmapper.num_1k_VROM_banks)
					{
						g_NESmapper.Mapper119.chr7 = bank_num;
						NES_mapper119_MMC3_set_PPU_banks();
					}
				}
				break;

			case 0x06:
				{
					g_NESmapper.Mapper119.prg0 = bank_num;
					NES_mapper119_MMC3_set_CPU_banks();
				}
				break;

			case 0x07:
				{
					g_NESmapper.Mapper119.prg1 = bank_num;
					NES_mapper119_MMC3_set_CPU_banks();
				}
				break;
			}
		}
		break;

	case 0xA000:
		{
			g_NESmapper.Mapper119.regs[2] = data;

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
			g_NESmapper.Mapper119.regs[3] = data;
		}
		break;

	case 0xC000:
		g_NESmapper.Mapper119.regs[4] = data;
		g_NESmapper.Mapper119.irq_counter = g_NESmapper.Mapper119.regs[4];
		break;

	case 0xC001:
		g_NESmapper.Mapper119.regs[5] = data;
		g_NESmapper.Mapper119.irq_latch = g_NESmapper.Mapper119.regs[5];
		break;

	case 0xE000:
		g_NESmapper.Mapper119.regs[6] = data;
		g_NESmapper.Mapper119.irq_enabled = 0;
		break;

	case 0xE001:
		g_NESmapper.Mapper119.regs[7] = data;
		g_NESmapper.Mapper119.irq_enabled = 1;
		break;
	}
}

void NES_mapper119_HSync(uint32 scanline)
{
	if(g_NESmapper.Mapper119.irq_enabled)
	{
		if((scanline >= 0) && (scanline <= 239))
		{
			if(NES_PPU_spr_enabled() || NES_PPU_bg_enabled())
			{
				if(!(g_NESmapper.Mapper119.irq_counter--))
				{
					g_NESmapper.Mapper119.irq_counter = g_NESmapper.Mapper119.irq_latch;
					NES6502_DoIRQ();
				}
			}
		}
	}
}

void NES_mapper119_MMC3_set_CPU_banks()
{
	if(g_NESmapper.Mapper119.regs[0] & 0x40)
	{
		g_NESmapper.set_CPU_banks4(g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.Mapper119.prg1,g_NESmapper.Mapper119.prg0,g_NESmapper.num_8k_ROM_banks-1);
	}
	else
	{
		g_NESmapper.set_CPU_banks4(g_NESmapper.Mapper119.prg0,g_NESmapper.Mapper119.prg1,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);
	}
}

void NES_mapper119_MMC3_set_PPU_banks()
{
	uint8 chr_bank[8];

	if(g_NESmapper.Mapper119.regs[0] & 0x80)
	{
		chr_bank[0] = g_NESmapper.Mapper119.chr4;
		chr_bank[1] = g_NESmapper.Mapper119.chr5;
		chr_bank[2] = g_NESmapper.Mapper119.chr6;
		chr_bank[3] = g_NESmapper.Mapper119.chr7;
		chr_bank[4] = g_NESmapper.Mapper119.chr01+0;
		chr_bank[5] = g_NESmapper.Mapper119.chr01+1;
		chr_bank[6] = g_NESmapper.Mapper119.chr23+0;
		chr_bank[7] = g_NESmapper.Mapper119.chr23+1;
	}
	else
	{
		chr_bank[0] = g_NESmapper.Mapper119.chr01+0;
		chr_bank[1] = g_NESmapper.Mapper119.chr01+1;
		chr_bank[2] = g_NESmapper.Mapper119.chr23+0;
		chr_bank[3] = g_NESmapper.Mapper119.chr23+1;
		chr_bank[4] = g_NESmapper.Mapper119.chr4;
		chr_bank[5] = g_NESmapper.Mapper119.chr5;
		chr_bank[6] = g_NESmapper.Mapper119.chr6;
		chr_bank[7] = g_NESmapper.Mapper119.chr7;
	}

	if(chr_bank[0] & 0x40)
		g_NESmapper.set_VRAM_bank(0, chr_bank[0] & 0x07);
	else
		g_NESmapper.set_PPU_bank0(chr_bank[0]);

	if(chr_bank[1] & 0x40)
		g_NESmapper.set_VRAM_bank(1, chr_bank[1] & 0x07);
	else
		g_NESmapper.set_PPU_bank1(chr_bank[1]);

	if(chr_bank[2] & 0x40)
		g_NESmapper.set_VRAM_bank(2, chr_bank[2] & 0x07);
	else
		g_NESmapper.set_PPU_bank2(chr_bank[2]);

	if(chr_bank[3] & 0x40)
		g_NESmapper.set_VRAM_bank(3, chr_bank[3] & 0x07);
	else
		g_NESmapper.set_PPU_bank3(chr_bank[3]);

	if(chr_bank[4] & 0x40)
		g_NESmapper.set_VRAM_bank(4, chr_bank[4] & 0x07);
	else
		g_NESmapper.set_PPU_bank4(chr_bank[4]);

	if(chr_bank[5] & 0x40)
		g_NESmapper.set_VRAM_bank(5, chr_bank[5] & 0x07);
	else
		g_NESmapper.set_PPU_bank5(chr_bank[5]);

	if(chr_bank[6] & 0x40)
		g_NESmapper.set_VRAM_bank(6, chr_bank[6] & 0x07);
	else
		g_NESmapper.set_PPU_bank6(chr_bank[6]);

	if(chr_bank[7] & 0x40)
		g_NESmapper.set_VRAM_bank(7, chr_bank[7] & 0x07);
	else
		g_NESmapper.set_PPU_bank7(chr_bank[7]);
}
/////////////////////////////////////////////////////////////////////

