
/////////////////////////////////////////////////////////////////////
// Mapper 13
void NES_mapper13_Init()
{
	g_NESmapper.Reset = NES_mapper13_Reset;
	g_NESmapper.MemoryWrite = NES_mapper13_MemoryWrite;
}

void NES_mapper13_Reset()
{
	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,2,3);

	// set PPU bank pointers
	g_NESmapper.set_VRAM_bank(0, 0);
	g_NESmapper.set_VRAM_bank(1, 1);
	g_NESmapper.set_VRAM_bank(2, 2);
	g_NESmapper.set_VRAM_bank(3, 3);
	g_NESmapper.set_VRAM_bank(4, 0);
	g_NESmapper.set_VRAM_bank(5, 1);
	g_NESmapper.set_VRAM_bank(6, 2);
	g_NESmapper.set_VRAM_bank(7, 3);

	g_PPU.vram_size = 0x4000;

	g_NESmapper.Mapper13.prg_bank = g_NESmapper.Mapper13.chr_bank = 0;
}

void NES_mapper13_MemoryWrite(uint32 addr, uint8 data)
{
	g_NESmapper.Mapper13.prg_bank = (data & 0x30) >> 4;
	g_NESmapper.Mapper13.chr_bank = data & 0x03;

	g_NESmapper.set_CPU_bank4(g_NESmapper.Mapper13.prg_bank*4+0);
	g_NESmapper.set_CPU_bank5(g_NESmapper.Mapper13.prg_bank*4+1);
	g_NESmapper.set_CPU_bank6(g_NESmapper.Mapper13.prg_bank*4+2);
	g_NESmapper.set_CPU_bank7(g_NESmapper.Mapper13.prg_bank*4+3);

	g_NESmapper.set_VRAM_bank(4, g_NESmapper.Mapper13.chr_bank * 4 + 0);
	g_NESmapper.set_VRAM_bank(5, g_NESmapper.Mapper13.chr_bank * 4 + 1);
	g_NESmapper.set_VRAM_bank(6, g_NESmapper.Mapper13.chr_bank * 4 + 2);
	g_NESmapper.set_VRAM_bank(7, g_NESmapper.Mapper13.chr_bank * 4 + 3);
}

void NES_mapper13_SNSS_fixup()
{
	g_NESmapper.set_VRAM_bank(4, g_NESmapper.Mapper13.chr_bank * 4 + 0);
	g_NESmapper.set_VRAM_bank(5, g_NESmapper.Mapper13.chr_bank * 4 + 1);
	g_NESmapper.set_VRAM_bank(6, g_NESmapper.Mapper13.chr_bank * 4 + 2);
	g_NESmapper.set_VRAM_bank(7, g_NESmapper.Mapper13.chr_bank * 4 + 3);
}
/////////////////////////////////////////////////////////////////////

