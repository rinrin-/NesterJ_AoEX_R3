
/////////////////////////////////////////////////////////////////////
// Mapper 77
void NES_mapper77_Init()
{
	g_NESmapper.Reset = NES_mapper77_Reset;
	g_NESmapper.MemoryWrite = NES_mapper77_MemoryWrite;
}

void NES_mapper77_Reset()
{
	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,2,3);

	// for Napoleon Senki
	g_PPU.vram_write_protect = 0;
}

void NES_mapper77_MemoryWrite(uint32 addr, uint8 data)
{
	uint8 prg_bank = data & 0x07;
	uint8 chr_bank = (data & 0xF0) >> 4;

	g_NESmapper.set_CPU_bank4(prg_bank*4+0);
	g_NESmapper.set_CPU_bank5(prg_bank*4+1);
	g_NESmapper.set_CPU_bank6(prg_bank*4+2);
	g_NESmapper.set_CPU_bank7(prg_bank*4+3);

	g_NESmapper.set_PPU_bank0(chr_bank*2+0);
	g_NESmapper.set_PPU_bank1(chr_bank*2+1);
}
/////////////////////////////////////////////////////////////////////

