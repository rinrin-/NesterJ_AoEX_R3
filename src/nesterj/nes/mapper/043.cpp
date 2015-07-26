
/////////////////////////////////////////////////////////////////////
// Mapper 43
void NES_mapper43_Init()
{
	g_NESmapper.Reset = NES_mapper43_Reset;
	g_NESmapper.MemoryReadLow = NES_mapper43_MemoryReadLow;
	g_NESmapper.MemoryWriteLow = NES_mapper43_MemoryWriteLow;
	g_NESmapper.MemoryWrite = NES_mapper43_MemoryWrite;
	g_NESmapper.HSync = NES_mapper43_HSync;
}

void NES_mapper43_Reset()
{
	g_NESmapper.set_CPU_banks5(2,1,0,4,9);
	g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);

	//set_mirroring(NES_PPU_MIRROR_VERT);

	g_NESmapper.Mapper43.irq_enabled = 1;
	g_NESmapper.Mapper43.irq_counter = 0;
}

uint8 NES_mapper43_MemoryReadLow(uint32 addr)
{
	if(0x5000 <= addr && addr < 0x6000)
	{
		return g_NESmapper.ROM_banks[0x2000*8 + 0x1000 + (addr - 0x5000)];
	}
	return (uint8)(addr >> 8);
}

void NES_mapper43_MemoryWriteLow(uint32 addr, uint8 data)
{
	if((addr & 0xF0FF) == 0x4022)
	{
		switch(data & 0x07)
		{
		case 0x0:
		case 0x2:
		case 0x3:
		case 0x4:
			{
				g_NESmapper.set_CPU_bank6(4);
			}
			break;

		case 0x1:
			{
				g_NESmapper.set_CPU_bank6(3);
			}
			break;

		case 0x5:
			{
				g_NESmapper.set_CPU_bank6(7);
			}
			break;

		case 0x6:
			{
				g_NESmapper.set_CPU_bank6(5);
			}
			break;

		case 0x7:
			{
				g_NESmapper.set_CPU_bank6(6);
			}
			break;
		}
	}
}

void NES_mapper43_MemoryWrite(uint32 addr, uint8 data)
{
	if(addr == 0x8122)
	{
		if(data & 3)
		{
			g_NESmapper.Mapper43.irq_enabled = 1;
		}
		else
		{
			g_NESmapper.Mapper43.irq_counter = 0;
			g_NESmapper.Mapper43.irq_enabled = 0;
		}
	}
}

void NES_mapper43_HSync(uint32 scanline)
{
	if(g_NESmapper.Mapper43.irq_enabled)
	{
		g_NESmapper.Mapper43.irq_counter += 114;
		if(g_NESmapper.Mapper43.irq_counter >= 4096)
		{
			g_NESmapper.Mapper43.irq_counter -= 4096;
			NES6502_DoIRQ();
		}
	}
}
/////////////////////////////////////////////////////////////////////

