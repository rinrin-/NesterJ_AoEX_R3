
/////////////////////////////////////////////////////////////////////
// Mapper 11
void NES_mapper11_Init()
{
	g_NESmapper.Reset = NES_mapper11_Reset;
	g_NESmapper.MemoryWrite = NES_mapper11_MemoryWrite;
}

void NES_mapper11_Reset()
{
	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,2,3);

	// set PPU bank pointers
	g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);

	g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
}

void NES_mapper11_MemoryWrite(uint32 addr, uint8 data)
{
	uint8 prg_bank = data & 0x01;
	uint8 chr_bank = (data & 0x70) >> 4;

	g_NESmapper.set_CPU_bank4(prg_bank*4+0);
	g_NESmapper.set_CPU_bank5(prg_bank*4+1);
	g_NESmapper.set_CPU_bank6(prg_bank*4+2);
	g_NESmapper.set_CPU_bank7(prg_bank*4+3);

	g_NESmapper.set_PPU_bank0(chr_bank*8+0);
	g_NESmapper.set_PPU_bank1(chr_bank*8+1);
	g_NESmapper.set_PPU_bank2(chr_bank*8+2);
	g_NESmapper.set_PPU_bank3(chr_bank*8+3);
	g_NESmapper.set_PPU_bank4(chr_bank*8+4);
	g_NESmapper.set_PPU_bank5(chr_bank*8+5);
	g_NESmapper.set_PPU_bank6(chr_bank*8+6);
	g_NESmapper.set_PPU_bank7(chr_bank*8+7);
}
/////////////////////////////////////////////////////////////////////

