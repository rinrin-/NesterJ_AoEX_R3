
/////////////////////////////////////////////////////////////////////
// Mapper 23
void NES_mapper23_Init()
{
	g_NESmapper.Reset = NES_mapper23_Reset;
	g_NESmapper.MemoryWrite = NES_mapper23_MemoryWrite;
	g_NESmapper.HSync = NES_mapper23_HSync;
}

void NES_mapper23_Reset()
{
	g_NESmapper.Mapper23.patch = 0xFFFF;

	if(NES_crc32() == 0x93794634) // Akumajou Special - Boku Dracula Kun
	{
		g_NESmapper.Mapper23.patch = 0xF00C;
	}

	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);

	// set PPU bank pointers
	g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);

	g_NESmapper.Mapper23.regs[0] = 0;
	g_NESmapper.Mapper23.regs[1] = 1;
	g_NESmapper.Mapper23.regs[2] = 2;
	g_NESmapper.Mapper23.regs[3] = 3;
	g_NESmapper.Mapper23.regs[4] = 4;
	g_NESmapper.Mapper23.regs[5] = 5;
	g_NESmapper.Mapper23.regs[6] = 6;
	g_NESmapper.Mapper23.regs[7] = 7;
	g_NESmapper.Mapper23.regs[8] = 0;

	g_NESmapper.Mapper23.irq_enabled = 0;
	g_NESmapper.Mapper23.irq_counter = 0;
	g_NESmapper.Mapper23.irq_latch = 0;
}

void NES_mapper23_MemoryWrite(uint32 addr, uint8 data)
{
	// g_NESmapper.Mapper23.regs[0] ... 1K VROM bank at PPU $0000
	// g_NESmapper.Mapper23.regs[1] ... 1K VROM bank at PPU $0400
	// g_NESmapper.Mapper23.regs[2] ... 1K VROM bank at PPU $0800
	// g_NESmapper.Mapper23.regs[3] ... 1K VROM bank at PPU $0C00
	// g_NESmapper.Mapper23.regs[4] ... 1K VROM bank at PPU $1000
	// g_NESmapper.Mapper23.regs[5] ... 1K VROM bank at PPU $1400
	// g_NESmapper.Mapper23.regs[6] ... 1K VROM bank at PPU $1800
	// g_NESmapper.Mapper23.regs[7] ... 1K VROM bank at PPU $1C00
	// g_NESmapper.Mapper23.regs[8] ... $9008 swap

	switch(addr & g_NESmapper.Mapper23.patch)
	{
	case 0x8000:
	case 0x8004:
	case 0x8008:
	case 0x800C:
		{
			if(g_NESmapper.Mapper23.regs[8])
			{
				g_NESmapper.set_CPU_bank6(data);
			}
			else
			{
				g_NESmapper.set_CPU_bank4(data);
			}
		}
		break;

	case 0x9000:
		{
			if(data != 0xFF)
			{
				data &= 0x03;
				if(data == 0)
				{
					g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
				}
				else if(data == 1)
				{
					g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
				}
				else if(data == 2)
				{
					g_NESmapper.set_mirroring(0,0,0,0);
				}
				else
				{
					g_NESmapper.set_mirroring(1,1,1,1);
				}
			}
		}
		break;

	case 0x9008:
		{
			g_NESmapper.Mapper23.regs[8] = data & 0x02;
		}
		break;

	case 0xA000:
	case 0xA004:
	case 0xA008:
	case 0xA00C:
		{
			g_NESmapper.set_CPU_bank5(data);
		}
		break;

	case 0xB000:
		{
			g_NESmapper.Mapper23.regs[0] = (g_NESmapper.Mapper23.regs[0] & 0xF0) | (data & 0x0F);
			g_NESmapper.set_PPU_bank0(g_NESmapper.Mapper23.regs[0]);
		}
		break;

	case 0xB001:
	case 0xB004:
		{
			g_NESmapper.Mapper23.regs[0] = (g_NESmapper.Mapper23.regs[0] & 0x0F) | ((data & 0x0F) << 4);
			g_NESmapper.set_PPU_bank0(g_NESmapper.Mapper23.regs[0]);
		}
		break;

	case 0xB002:
	case 0xB008:
		{
			g_NESmapper.Mapper23.regs[1] = (g_NESmapper.Mapper23.regs[1] & 0xF0) | (data & 0x0F);
			g_NESmapper.set_PPU_bank1(g_NESmapper.Mapper23.regs[1]);
		}
		break;

	case 0xB003:
	case 0xB00C:
		{
			g_NESmapper.Mapper23.regs[1] = (g_NESmapper.Mapper23.regs[1] & 0x0F) | ((data & 0x0F) << 4);
			g_NESmapper.set_PPU_bank1(g_NESmapper.Mapper23.regs[1]);
		}
		break;

	case 0xC000:
		{
			g_NESmapper.Mapper23.regs[2] = (g_NESmapper.Mapper23.regs[2] & 0xF0) | (data & 0x0F);
			g_NESmapper.set_PPU_bank2(g_NESmapper.Mapper23.regs[2]);
		}
		break;

	case 0xC001:
	case 0xC004:
		{
			g_NESmapper.Mapper23.regs[2] = (g_NESmapper.Mapper23.regs[2] & 0x0F) | ((data & 0x0F) << 4);
			g_NESmapper.set_PPU_bank2(g_NESmapper.Mapper23.regs[2]);
		}
		break;

	case 0xC002:
	case 0xC008:
		{
			g_NESmapper.Mapper23.regs[3] = (g_NESmapper.Mapper23.regs[3] & 0xF0) | (data & 0x0F);
			g_NESmapper.set_PPU_bank3(g_NESmapper.Mapper23.regs[3]);
		}
		break;

	case 0xC003:
	case 0xC00C:
		{
			g_NESmapper.Mapper23.regs[3] = (g_NESmapper.Mapper23.regs[3] & 0x0F) | ((data & 0x0F) << 4);
			g_NESmapper.set_PPU_bank3(g_NESmapper.Mapper23.regs[3]);
		}
		break;

	case 0xD000:
		{
			g_NESmapper.Mapper23.regs[4] = (g_NESmapper.Mapper23.regs[4] & 0xF0) | (data & 0x0F);
			g_NESmapper.set_PPU_bank4(g_NESmapper.Mapper23.regs[4]);
		}
		break;

	case 0xD001:
	case 0xD004:
		{
			g_NESmapper.Mapper23.regs[4] = (g_NESmapper.Mapper23.regs[4] & 0x0F) | ((data & 0x0F) << 4);
			g_NESmapper.set_PPU_bank4(g_NESmapper.Mapper23.regs[4]);
		}
		break;

	case 0xD002:
	case 0xD008:
		{
			g_NESmapper.Mapper23.regs[5] = (g_NESmapper.Mapper23.regs[5] & 0xF0) | (data & 0x0F);
			g_NESmapper.set_PPU_bank5(g_NESmapper.Mapper23.regs[5]);
		}
		break;

	case 0xD003:
	case 0xD00C:
		{
			g_NESmapper.Mapper23.regs[5] = (g_NESmapper.Mapper23.regs[5] & 0x0F) | ((data & 0x0F) << 4);
			g_NESmapper.set_PPU_bank5(g_NESmapper.Mapper23.regs[5]);
		}
		break;

	case 0xE000:
		{
			g_NESmapper.Mapper23.regs[6] = (g_NESmapper.Mapper23.regs[6] & 0xF0) | (data & 0x0F);
			g_NESmapper.set_PPU_bank6(g_NESmapper.Mapper23.regs[6]);
		}
		break;

	case 0xE001:
	case 0xE004:
		{
			g_NESmapper.Mapper23.regs[6] = (g_NESmapper.Mapper23.regs[6] & 0x0F) | ((data & 0x0F) << 4);
			g_NESmapper.set_PPU_bank6(g_NESmapper.Mapper23.regs[6]);
		}
		break;

	case 0xE002:
	case 0xE008:
		{
			g_NESmapper.Mapper23.regs[7] = (g_NESmapper.Mapper23.regs[7] & 0xF0) | (data & 0x0F);
			g_NESmapper.set_PPU_bank7(g_NESmapper.Mapper23.regs[7]);
		}
		break;

	case 0xE003:
	case 0xE00C:
		{
			g_NESmapper.Mapper23.regs[7] = (g_NESmapper.Mapper23.regs[7] & 0x0F) | ((data & 0x0F) << 4);
			g_NESmapper.set_PPU_bank7(g_NESmapper.Mapper23.regs[7]);
		}
		break;

	case 0xF000:
		{
			g_NESmapper.Mapper23.irq_latch = (g_NESmapper.Mapper23.irq_latch & 0xF0) | (data & 0x0F);
		}
		break;

	case 0xF004:
		{
			g_NESmapper.Mapper23.irq_latch = (g_NESmapper.Mapper23.irq_latch & 0x0F) | ((data & 0x0F) << 4);
		}
		break;

	case 0xF008:
		{
			g_NESmapper.Mapper23.irq_enabled = data & 0x03;
			if(g_NESmapper.Mapper23.irq_enabled & 0x02)
			{
				g_NESmapper.Mapper23.irq_counter = g_NESmapper.Mapper23.irq_latch;
			}
		}
		break;

	case 0xF00C:
		{
			g_NESmapper.Mapper23.irq_enabled = (g_NESmapper.Mapper23.irq_enabled & 0x01) * 3;
		}
		break;
	}
}

void NES_mapper23_HSync(uint32 scanline)
{
	if(g_NESmapper.Mapper23.irq_enabled & 0x02)
	{
		if(g_NESmapper.Mapper23.irq_counter == 0xFF)
		{
			NES6502_DoIRQ();
			g_NESmapper.Mapper23.irq_counter = g_NESmapper.Mapper23.irq_latch;
			g_NESmapper.Mapper23.irq_enabled = (g_NESmapper.Mapper23.irq_enabled & 0x01) * 3;
		}
		else
		{
			g_NESmapper.Mapper23.irq_counter++;
		}
	}
}

#define MAP23_VROM(ptr) (((ptr)-NES_ROM_get_VROM_banks()) >> 10)

void NES_mapper23_SNSS_fixup()
{
	g_NESmapper.Mapper23.regs[0] = MAP23_VROM(g_PPU.PPU_VRAM_banks[0]);
	g_NESmapper.Mapper23.regs[1] = MAP23_VROM(g_PPU.PPU_VRAM_banks[1]);
	g_NESmapper.Mapper23.regs[2] = MAP23_VROM(g_PPU.PPU_VRAM_banks[2]);
	g_NESmapper.Mapper23.regs[3] = MAP23_VROM(g_PPU.PPU_VRAM_banks[3]);
	g_NESmapper.Mapper23.regs[4] = MAP23_VROM(g_PPU.PPU_VRAM_banks[4]);
	g_NESmapper.Mapper23.regs[5] = MAP23_VROM(g_PPU.PPU_VRAM_banks[5]);
	g_NESmapper.Mapper23.regs[6] = MAP23_VROM(g_PPU.PPU_VRAM_banks[6]);
	g_NESmapper.Mapper23.regs[7] = MAP23_VROM(g_PPU.PPU_VRAM_banks[7]);
}
/////////////////////////////////////////////////////////////////////

