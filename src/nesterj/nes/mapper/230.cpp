
/////////////////////////////////////////////////////////////////////
// Mapper 230
void NES_mapper230_Init()
{
	g_NESmapper.Reset = NES_mapper230_Reset;
	g_NESmapper.MemoryWrite = NES_mapper230_MemoryWrite;
}

void NES_mapper230_Reset()
{
	// Contra - 22 Games switch
	if(g_NESmapper.Mapper230.rom_switch)
	{
		g_NESmapper.Mapper230.rom_switch = 0;
	}
	else
	{
		g_NESmapper.Mapper230.rom_switch = 1;
	}

	// set CPU bank pointers
	if(g_NESmapper.Mapper230.rom_switch)
	{
		g_NESmapper.set_CPU_banks4(0,1,14,15);
	}
	else
	{
		g_NESmapper.set_CPU_banks4(16,17,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);
	}
}

void NES_mapper230_MemoryWrite(uint32 addr, uint8 data)
{
	if(g_NESmapper.Mapper230.rom_switch)
	{
		g_NESmapper.set_CPU_bank4((data & 0x07)*2+0);
		g_NESmapper.set_CPU_bank5((data & 0x07)*2+1);
	}
	else
	{
		if(data & 0x40)
		{
			g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
		}
		else
		{
			g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
		}
		if(data & 0x20)
		{
			g_NESmapper.set_CPU_bank4((data & 0x1F)*2+16);
			g_NESmapper.set_CPU_bank5((data & 0x1F)*2+17);
			g_NESmapper.set_CPU_bank6((data & 0x1F)*2+16);
			g_NESmapper.set_CPU_bank7((data & 0x1F)*2+17);
		}
		else
		{
			g_NESmapper.set_CPU_bank4((data & 0x1E)*2+16);
			g_NESmapper.set_CPU_bank5((data & 0x1E)*2+17);
			g_NESmapper.set_CPU_bank6((data & 0x1E)*2+18);
			g_NESmapper.set_CPU_bank7((data & 0x1E)*2+19);
		}
	}
}
/////////////////////////////////////////////////////////////////////

