
/////////////////////////////////////////////////////////////////////
// Mapper 226
void NES_mapper226_Init()
{
	g_NESmapper.Reset = NES_mapper226_Reset;
	g_NESmapper.MemoryWrite = NES_mapper226_MemoryWrite;
}

void NES_mapper226_Reset()
{
	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,2,3);

	g_NESmapper.Mapper226.regs[0] = 0;
	g_NESmapper.Mapper226.regs[1] = 0;
}

void NES_mapper226_MemoryWrite(uint32 addr, uint8 data)
{
	uint8 prg_bank;
	if(addr & 0x001)
	{
		g_NESmapper.Mapper226.regs[1] = data;
	}
	else
	{
		g_NESmapper.Mapper226.regs[0] = data;
	}

	if(g_NESmapper.Mapper226.regs[0] & 0x40)
	{
		g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
	}
	else
	{
		g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
	}

	prg_bank = ((g_NESmapper.Mapper226.regs[0] & 0x1E) >> 1) | ((g_NESmapper.Mapper226.regs[0] & 0x80) >> 3) | ((g_NESmapper.Mapper226.regs[1] & 0x01) << 5);

	if(g_NESmapper.Mapper226.regs[0] & 0x20)
	{
		if(g_NESmapper.Mapper226.regs[0] & 0x01)
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
	else
	{
		g_NESmapper.set_CPU_bank4(prg_bank*4+0);
		g_NESmapper.set_CPU_bank5(prg_bank*4+1);
		g_NESmapper.set_CPU_bank6(prg_bank*4+2);
		g_NESmapper.set_CPU_bank7(prg_bank*4+3);
	}
}
/////////////////////////////////////////////////////////////////////

