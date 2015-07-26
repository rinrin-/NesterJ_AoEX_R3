#ifdef _NES_MAPPER_CPP_

/////////////////////////////////////////////////////////////////////
// Mapper 185
void NES_mapper185_Init()
{
	g_NESmapper.Reset = NES_mapper185_Reset;
	g_NESmapper.MemoryWrite = NES_mapper185_MemoryWrite;
}

void NES_mapper185_Reset()
{
	uint32 i;
	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,2,3);

	g_NESmapper.Mapper185.patch = 0;

	if(NES_crc32() == 0xb36457c7) // Spy Vs Spy
	{
		g_NESmapper.Mapper185.patch = 1;
	}

	for(i = 0; i < 0x400; i++)
	{
		g_NESmapper.Mapper185.dummy_chr_rom[i] = 0xFF;
	}
}

void NES_mapper185_MemoryWrite(uint32 addr, uint8 data)
{
	if((!g_NESmapper.Mapper185.patch && (data & 0x03)) || (g_NESmapper.Mapper185.patch && data == 0x21))
	{
		g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);
	}
	else
	{
		g_PPU.PPU_VRAM_banks[0] = g_NESmapper.Mapper185.dummy_chr_rom;
		g_PPU.PPU_VRAM_banks[1] = g_NESmapper.Mapper185.dummy_chr_rom;
		g_PPU.PPU_VRAM_banks[2] = g_NESmapper.Mapper185.dummy_chr_rom;
		g_PPU.PPU_VRAM_banks[3] = g_NESmapper.Mapper185.dummy_chr_rom;
		g_PPU.PPU_VRAM_banks[4] = g_NESmapper.Mapper185.dummy_chr_rom;
		g_PPU.PPU_VRAM_banks[5] = g_NESmapper.Mapper185.dummy_chr_rom;
		g_PPU.PPU_VRAM_banks[6] = g_NESmapper.Mapper185.dummy_chr_rom;
		g_PPU.PPU_VRAM_banks[7] = g_NESmapper.Mapper185.dummy_chr_rom;
	}
}
/////////////////////////////////////////////////////////////////////

#endif
