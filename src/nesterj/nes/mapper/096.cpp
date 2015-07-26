
/////////////////////////////////////////////////////////////////////
// Mapper 96
void NES_mapper96_Init()
{
	g_NESmapper.Reset = NES_mapper96_Reset;
	g_NESmapper.PPU_Latch_Address = NES_mapper96_PPU_Latch_Address;
	g_NESmapper.MemoryWrite = NES_mapper96_MemoryWrite;
}

void NES_mapper96_Reset()
{
	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,2,3);

	// set PPU bank pointers
	g_NESmapper.Mapper96.vbank0 = g_NESmapper.Mapper96.vbank1 = 0;
	NES_mapper96_sync_PPU_banks();
	g_PPU.vram_size = 0x8000;

	g_NESmapper.set_mirroring(0,0,0,0);
}

void NES_mapper96_PPU_Latch_Address(uint32 addr)
{
	if((addr & 0xF000) == 0x2000)
	{
		g_NESmapper.Mapper96.vbank1 = (addr & 0x0300) >> 8;
		NES_mapper96_sync_PPU_banks();
	}
}

void NES_mapper96_MemoryWrite(uint32 addr, uint8 data)
{
	g_NESmapper.set_CPU_bank4((data & 0x03) * 4 + 0);
	g_NESmapper.set_CPU_bank5((data & 0x03) * 4 + 1);
	g_NESmapper.set_CPU_bank6((data & 0x03) * 4 + 2);
	g_NESmapper.set_CPU_bank7((data & 0x03) * 4 + 3);

	g_NESmapper.Mapper96.vbank0 = (data & 0x04) >> 2;
	NES_mapper96_sync_PPU_banks();
}

void NES_mapper96_sync_PPU_banks()
{
	g_NESmapper.set_VRAM_bank(0, g_NESmapper.Mapper96.vbank0 * 16 + g_NESmapper.Mapper96.vbank1 * 4 + 0);
	g_NESmapper.set_VRAM_bank(1, g_NESmapper.Mapper96.vbank0 * 16 + g_NESmapper.Mapper96.vbank1 * 4 + 1);
	g_NESmapper.set_VRAM_bank(2, g_NESmapper.Mapper96.vbank0 * 16 + g_NESmapper.Mapper96.vbank1 * 4 + 2);
	g_NESmapper.set_VRAM_bank(3, g_NESmapper.Mapper96.vbank0 * 16 + g_NESmapper.Mapper96.vbank1 * 4 + 3);
	g_NESmapper.set_VRAM_bank(4, g_NESmapper.Mapper96.vbank0 * 16 + 12);
	g_NESmapper.set_VRAM_bank(5, g_NESmapper.Mapper96.vbank0 * 16 + 13);
	g_NESmapper.set_VRAM_bank(6, g_NESmapper.Mapper96.vbank0 * 16 + 14);
	g_NESmapper.set_VRAM_bank(7, g_NESmapper.Mapper96.vbank0 * 16 + 15);
}
/////////////////////////////////////////////////////////////////////

