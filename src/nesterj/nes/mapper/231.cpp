
/////////////////////////////////////////////////////////////////////
// Mapper 231
void NES_mapper231_Init()
{
	g_NESmapper.Reset = NES_mapper231_Reset;
	g_NESmapper.MemoryWrite = NES_mapper231_MemoryWrite;
}

void NES_mapper231_Reset()
{
	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,2,3);

	// set PPU bank pointers
	if(g_NESmapper.num_1k_VROM_banks)
	{
		g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);
	}
}

void NES_mapper231_MemoryWrite(uint32 addr, uint8 data)
{
	if(addr & 0x0020)
	{
		uint8 prg_bank = (uint8)(addr >> 1) & 0x0F;
		g_NESmapper.set_CPU_bank4(prg_bank*4+0);
		g_NESmapper.set_CPU_bank5(prg_bank*4+1);
		g_NESmapper.set_CPU_bank6(prg_bank*4+2);
		g_NESmapper.set_CPU_bank7(prg_bank*4+3);
	}
	else
	{
		uint8 prg_bank = addr & 0x001E;
		g_NESmapper.set_CPU_bank4(prg_bank*2+0);
		g_NESmapper.set_CPU_bank5(prg_bank*2+1);
		g_NESmapper.set_CPU_bank6(prg_bank*2+0);
		g_NESmapper.set_CPU_bank7(prg_bank*2+1);
	}

	if(addr & 0x0080)
	{
		g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
	}
	else
	{
		g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
	}
}
/////////////////////////////////////////////////////////////////////

