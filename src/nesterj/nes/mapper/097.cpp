#ifdef _NES_MAPPER_CPP_

/////////////////////////////////////////////////////////////////////
// Mapper 97
void NES_mapper97_Init()
{
	g_NESmapper.Reset = NES_mapper97_Reset;
	g_NESmapper.MemoryWrite = NES_mapper97_MemoryWrite;
}

void NES_mapper97_Reset()
{
	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1,0,1);

	// set PPU bank pointers ?
	if(g_NESmapper.num_1k_VROM_banks)
	{
		g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);
	}
}

void NES_mapper97_MemoryWrite(uint32 addr, uint8 data)
{
	if(addr < 0xC000)
	{
		uint8 prg_bank = data & 0x0F;

		g_NESmapper.set_CPU_bank6(prg_bank*2+0);
		g_NESmapper.set_CPU_bank7(prg_bank*2+1);

		if((data & 0x80) == 0)
		{
			g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
		}
		else
		{
			g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
		}
	}
}
/////////////////////////////////////////////////////////////////////

#endif
