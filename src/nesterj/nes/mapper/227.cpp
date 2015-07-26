
/////////////////////////////////////////////////////////////////////
// Mapper 227
void NES_mapper227_Init()
{
	g_NESmapper.Reset = NES_mapper227_Reset;
	g_NESmapper.MemoryWrite = NES_mapper227_MemoryWrite;
}

void NES_mapper227_Reset()
{
	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,0,1);
}

void NES_mapper227_MemoryWrite(uint32 addr, uint8 data)
{
	uint8 prg_bank = ((addr & 0x0100) >> 4) | ((addr & 0x0078) >> 3);

	if(addr & 0x0001)
	{
		g_NESmapper.set_CPU_bank4(prg_bank*4+0);
		g_NESmapper.set_CPU_bank5(prg_bank*4+1);
		g_NESmapper.set_CPU_bank6(prg_bank*4+2);
		g_NESmapper.set_CPU_bank7(prg_bank*4+3);
	}
	else
	{
		if(addr & 0x0004)
		{
			g_NESmapper.set_CPU_bank4(prg_bank*4+2);
			g_NESmapper.set_CPU_bank5(prg_bank*4+3);
			g_NESmapper.set_CPU_bank6(prg_bank*4+2);
			g_NESmapper.set_CPU_bank7(prg_bank*4+3);
		}
		else
		{
			g_NESmapper.set_CPU_bank4(prg_bank*4+0);
			g_NESmapper.set_CPU_bank5(prg_bank*4+1);
			g_NESmapper.set_CPU_bank6(prg_bank*4+0);
			g_NESmapper.set_CPU_bank7(prg_bank*4+1);
		}
	}
	if(!(addr & 0x0080))
	{
		if(addr & 0x0200)
		{
			g_NESmapper.set_CPU_bank6((prg_bank & 0x1C)*4+14);
			g_NESmapper.set_CPU_bank7((prg_bank & 0x1C)*4+15);
		}
		else
		{
			g_NESmapper.set_CPU_bank6((prg_bank & 0x1C)*4+0);
			g_NESmapper.set_CPU_bank7((prg_bank & 0x1C)*4+1);
		}
	}

	if(addr & 0x0002)
	{
		g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
	}
	else
	{
		g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
	}
}
/////////////////////////////////////////////////////////////////////

