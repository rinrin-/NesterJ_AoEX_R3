
/////////////////////////////////////////////////////////////////////
// Mapper 85
void NES_mapper85_Init()
{
	g_NESmapper.Reset = NES_mapper85_Reset;
	g_NESmapper.MemoryWrite = NES_mapper85_MemoryWrite;
	g_NESmapper.HSync = NES_mapper85_HSync;
}

void NES_mapper85_Reset()
{
	// Init ExSound
	NES_APU_SelectExSound(2);

	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);

	// set PPU bank pointers
	if(g_NESmapper.num_1k_VROM_banks)
	{
		g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);
	}
	else
	{
		g_NESmapper.set_VRAM_bank(0, 0);
		g_NESmapper.set_VRAM_bank(1, 0);
		g_NESmapper.set_VRAM_bank(2, 0);
		g_NESmapper.set_VRAM_bank(3, 0);
		g_NESmapper.set_VRAM_bank(4, 0);
		g_NESmapper.set_VRAM_bank(5, 0);
		g_NESmapper.set_VRAM_bank(6, 0);
		g_NESmapper.set_VRAM_bank(7, 0);
	}

	g_NESmapper.Mapper85.patch = 0;

	if(NES_crc32() == 0x33CE3FF0)	// lagurange
		g_NESmapper.Mapper85.patch = 1;

	g_NESmapper.Mapper85.irq_enabled = 0;
	g_NESmapper.Mapper85.irq_counter = 0;
	g_NESmapper.Mapper85.irq_latch = 0;
}

void NES_mapper85_MemoryWrite(uint32 addr, uint8 data)
{
	switch(addr & 0xF038)
	{
	case 0x8000:
		{
			g_NESmapper.set_CPU_bank4(data);
		}
		break;

	case 0x8008:
	case 0x8010:
		{
			g_NESmapper.set_CPU_bank5(data);
		}
		break;

	case 0x9000:
		{
			g_NESmapper.set_CPU_bank6(data);
		}
		break;

	case 0x9010:
	case 0x9030:
		{
			NES_APU_ExWrite(addr, data);
		}
		break;

	case 0xA000:
		{
			if(g_NESmapper.num_1k_VROM_banks)
			{
				g_NESmapper.set_PPU_bank0(data);
			}
			else
			{
				g_NESmapper.set_VRAM_bank(0, data);
			}
		}
		break;

	case 0xA008:
	case 0xA010:
		{
			if(g_NESmapper.num_1k_VROM_banks)
			{
				g_NESmapper.set_PPU_bank1(data);
			}
			else
			{
				g_NESmapper.set_VRAM_bank(1, data);
			}
		}
		break;

	case 0xB000:
		{
			if(g_NESmapper.num_1k_VROM_banks)
			{
				g_NESmapper.set_PPU_bank2(data);
			}
			else
			{
				g_NESmapper.set_VRAM_bank(2, data);
			}
		}
		break;

	case 0xB008:
	case 0xB010:
		{
			if(g_NESmapper.num_1k_VROM_banks)
			{
				g_NESmapper.set_PPU_bank3(data);
			}
			else
			{
				g_NESmapper.set_VRAM_bank(3, data);
			}
		}
		break;

	case 0xC000:
		{
			if(g_NESmapper.num_1k_VROM_banks)
			{
				g_NESmapper.set_PPU_bank4(data);
			}
			else
			{
				g_NESmapper.set_VRAM_bank(4, data);
			}
		}
		break;

	case 0xC008:
	case 0xC010:
		{
			if(g_NESmapper.num_1k_VROM_banks)
			{
				g_NESmapper.set_PPU_bank5(data);
			}
			else
			{
				g_NESmapper.set_VRAM_bank(5, data);
			}
		}
		break;

	case 0xD000:
		{
			if(g_NESmapper.num_1k_VROM_banks)
			{
				g_NESmapper.set_PPU_bank6(data);
			}
			else
			{
				g_NESmapper.set_VRAM_bank(6, data);
			}
		}
		break;

	case 0xD008:
	case 0xD010:
		{
			if(g_NESmapper.num_1k_VROM_banks)
			{
				g_NESmapper.set_PPU_bank7(data);
			}
			else
			{
				g_NESmapper.set_VRAM_bank(7, data);
			}
		}
		break;

	case 0xE000:
		{
			data &= 0x03;
			if(data == 0x00)
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
			}
			else if(data == 0x01)
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
			}
			else if(data == 0x02)
			{
				g_NESmapper.set_mirroring(0,0,0,0);
			}
			else
			{
				g_NESmapper.set_mirroring(1,1,1,1);
			}
		}
		break;

	case 0xE008:
	case 0xE010:
		{
			g_NESmapper.Mapper85.irq_latch = data;
		}
		break;

	case 0xF000:
		{
			g_NESmapper.Mapper85.irq_enabled = data & 0x03;
			if(g_NESmapper.Mapper85.irq_enabled & 0x02)
			{
				g_NESmapper.Mapper85.irq_counter = g_NESmapper.Mapper85.irq_latch;
			}
		}
		break;

	case 0xF008:
	case 0xF010:
		{
			g_NESmapper.Mapper85.irq_enabled = (g_NESmapper.Mapper85.irq_enabled & 0x01) * 3;
		}
		break;
	}
}

void NES_mapper85_HSync(uint32 scanline)
{
	if(g_NESmapper.Mapper85.irq_enabled & 0x02)
	{
		if(g_NESmapper.Mapper85.irq_counter == (g_NESmapper.Mapper85.patch?0:0xFF))
		{
			NES6502_DoIRQ();
			g_NESmapper.Mapper85.irq_counter = g_NESmapper.Mapper85.irq_latch;
		}
		else
		{
			g_NESmapper.Mapper85.irq_counter++;
		}
	}
}
/////////////////////////////////////////////////////////////////////

