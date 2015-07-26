
/////////////////////////////////////////////////////////////////////
// Mapper 70
void NES_mapper70_Init()
{
	g_NESmapper.Reset = NES_mapper70_Reset;
	g_NESmapper.MemoryWrite = NES_mapper70_MemoryWrite;
}

void NES_mapper70_Reset()
{
	g_NESmapper.Mapper70.patch = 0;

	if(NES_crc32() == 0xa59ca2ef || // Kamen Rider Club
	        NES_crc32() == 0x10bb8f9a)   // Family Trainer - Manhattan Police
	{
		g_NESmapper.Mapper70.patch = 1;
	}

	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);
}

void NES_mapper70_MemoryWrite(uint32 addr, uint8 data)
{
	uint8 chr_bank = data & 0x0F;
	uint8 prg_bank = (data & 0x70) >> 4;

	g_NESmapper.set_CPU_bank4(prg_bank*2+0);
	g_NESmapper.set_CPU_bank5(prg_bank*2+1);

	g_NESmapper.set_PPU_bank0(chr_bank*8+0);
	g_NESmapper.set_PPU_bank1(chr_bank*8+1);
	g_NESmapper.set_PPU_bank2(chr_bank*8+2);
	g_NESmapper.set_PPU_bank3(chr_bank*8+3);
	g_NESmapper.set_PPU_bank4(chr_bank*8+4);
	g_NESmapper.set_PPU_bank5(chr_bank*8+5);
	g_NESmapper.set_PPU_bank6(chr_bank*8+6);
	g_NESmapper.set_PPU_bank7(chr_bank*8+7);

	if(g_NESmapper.Mapper70.patch)
	{
		if(data & 0x80)
		{
			g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
		}
		else
		{
			g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
		}
	}
	else
	{
		if(data & 0x80)
		{
			g_NESmapper.set_mirroring(1,1,1,1);
		}
		else
		{
			g_NESmapper.set_mirroring(0,0,0,0);
		}
	}
}
/////////////////////////////////////////////////////////////////////

