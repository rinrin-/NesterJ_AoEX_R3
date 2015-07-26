#ifdef _NES_MAPPER_CPP_

/////////////////////////////////////////////////////////////////////
// Mapper 180
void NES_mapper180_Init()
{
	g_NESmapper.Reset = NES_mapper180_Reset;
	g_NESmapper.MemoryWrite = NES_mapper180_MemoryWrite;
}

void NES_mapper180_Reset()
{
	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,2,3);

	// set PPU bank pointers
	if(g_NESmapper.num_1k_VROM_banks)
	{
		g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);
	}

	if(NES_crc32() == 0xc68363f6) // Crazy Climber
	{
		g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
	}
}

void NES_mapper180_MemoryWrite(uint32 addr, uint8 data)
{
	g_NESmapper.set_CPU_bank6((data & 0x07)*2+0);
	g_NESmapper.set_CPU_bank7((data & 0x07)*2+1);
}
/////////////////////////////////////////////////////////////////////

#endif
