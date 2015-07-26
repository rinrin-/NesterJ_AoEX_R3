
/////////////////////////////////////////////////////////////////////
// Mapper 6
void NES_mapper6_Init()
{
	g_NESmapper.Reset = NES_mapper6_Reset;
	g_NESmapper.MemoryWriteLow = NES_mapper6_MemoryWriteLow;
	g_NESmapper.MemoryWrite = NES_mapper6_MemoryWrite;
	g_NESmapper.HSync = NES_mapper6_HSync;
}


void NES_mapper6_Reset()
{
	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,14,15);

	// set PPU bank pointers
	if(g_NESmapper.num_1k_VROM_banks)
	{
		g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);
	}
	else
	{
		g_NESmapper.set_VRAM_bank(0, 0);
		g_NESmapper.set_VRAM_bank(0, 1);
		g_NESmapper.set_VRAM_bank(0, 2);
		g_NESmapper.set_VRAM_bank(0, 3);
		g_NESmapper.set_VRAM_bank(0, 4);
		g_NESmapper.set_VRAM_bank(0, 5);
		g_NESmapper.set_VRAM_bank(0, 6);
		g_NESmapper.set_VRAM_bank(0, 7);
	}
	g_PPU.vram_size = 0x8000;
}

void NES_mapper6_MemoryWriteLow(uint32 addr, uint8 data)
{
	switch(addr)
	{
	case 0x42FE:
		{
			if(data & 0x10)
			{
				g_NESmapper.set_mirroring(1,1,1,1);
			}
			else
			{
				g_NESmapper.set_mirroring(0,0,0,0);
			}
		}
		break;

	case 0x42FF:
		{
			if(data & 0x10)
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
			}
			else
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
			}
		}
		break;

	case 0x4501:
		{
			g_NESmapper.Mapper6.irq_enabled = 0;
		}
		break;

	case 0x4502:
		{
			g_NESmapper.Mapper6.irq_counter = (g_NESmapper.Mapper6.irq_counter & 0xFF00) | (uint32)data;
		}
		break;

	case 0x4503:
		{
			g_NESmapper.Mapper6.irq_counter = (g_NESmapper.Mapper6.irq_counter & 0x00FF) | ((uint32)data << 8);
			g_NESmapper.Mapper6.irq_enabled = 1;
		}
		break;
	}
}

void NES_mapper6_MemoryWrite(uint32 addr, uint8 data)
{
	uint8 prg_bank = (data & 0x3C) >> 2;
	uint8 chr_bank = data & 0x03;

	g_NESmapper.set_CPU_bank4(prg_bank*2+0);
	g_NESmapper.set_CPU_bank5(prg_bank*2+1);

	g_NESmapper.set_VRAM_bank(0, chr_bank * 8 + 0);
	g_NESmapper.set_VRAM_bank(1, chr_bank * 8 + 1);
	g_NESmapper.set_VRAM_bank(2, chr_bank * 8 + 2);
	g_NESmapper.set_VRAM_bank(3, chr_bank * 8 + 3);
	g_NESmapper.set_VRAM_bank(4, chr_bank * 8 + 4);
	g_NESmapper.set_VRAM_bank(5, chr_bank * 8 + 5);
	g_NESmapper.set_VRAM_bank(6, chr_bank * 8 + 6);
	g_NESmapper.set_VRAM_bank(7, chr_bank * 8 + 7);
}

void NES_mapper6_HSync(uint32 scanline)
{
	if(g_NESmapper.Mapper6.irq_enabled)
	{
		g_NESmapper.Mapper6.irq_counter += 133;
		if(g_NESmapper.Mapper6.irq_counter >= 0xFFFF)
		{
			NES6502_DoIRQ();
			g_NESmapper.Mapper6.irq_counter = 0;
		}
	}
}
/////////////////////////////////////////////////////////////////////

