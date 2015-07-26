#ifdef _NES_MAPPER_CPP_

/////////////////////////////////////////////////////////////////////
// Mapper 46
void NES_mapper46_Init()
{
	g_NESmapper.Reset = NES_mapper46_Reset;
	g_NESmapper.MemoryWriteSaveRAM = NES_mapper46_MemoryWriteSaveRAM;
	g_NESmapper.MemoryWrite = NES_mapper46_MemoryWrite;
}

void NES_mapper46_Reset()
{
	g_NESmapper.Mapper46.regs[0] = g_NESmapper.Mapper46.regs[1] = g_NESmapper.Mapper46.regs[2] = g_NESmapper.Mapper46.regs[3] = 0;

	NES_mapper46_set_rom_banks();
	g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
}

void NES_mapper46_MemoryWriteSaveRAM(uint32 addr, uint8 data)
{
	g_NESmapper.Mapper46.regs[0] = data & 0x0F;
	g_NESmapper.Mapper46.regs[1] = (data & 0xF0) >> 4;
	NES_mapper46_set_rom_banks();
}

void NES_mapper46_MemoryWrite(uint32 addr, uint8 data)
{
	g_NESmapper.Mapper46.regs[2] = data & 0x01;
	g_NESmapper.Mapper46.regs[3] = (data & 0x70) >> 4;
	NES_mapper46_set_rom_banks();
}

void NES_mapper46_set_rom_banks()
{
	g_NESmapper.set_CPU_bank4(g_NESmapper.Mapper46.regs[0]*8+g_NESmapper.Mapper46.regs[2]*4+0);
	g_NESmapper.set_CPU_bank5(g_NESmapper.Mapper46.regs[0]*8+g_NESmapper.Mapper46.regs[2]*4+1);
	g_NESmapper.set_CPU_bank6(g_NESmapper.Mapper46.regs[0]*8+g_NESmapper.Mapper46.regs[2]*4+2);
	g_NESmapper.set_CPU_bank7(g_NESmapper.Mapper46.regs[0]*8+g_NESmapper.Mapper46.regs[2]*4+3);
	g_NESmapper.set_PPU_bank0(g_NESmapper.Mapper46.regs[1]*64+g_NESmapper.Mapper46.regs[3]*8+0);
	g_NESmapper.set_PPU_bank1(g_NESmapper.Mapper46.regs[1]*64+g_NESmapper.Mapper46.regs[3]*8+1);
	g_NESmapper.set_PPU_bank2(g_NESmapper.Mapper46.regs[1]*64+g_NESmapper.Mapper46.regs[3]*8+2);
	g_NESmapper.set_PPU_bank3(g_NESmapper.Mapper46.regs[1]*64+g_NESmapper.Mapper46.regs[3]*8+3);
	g_NESmapper.set_PPU_bank4(g_NESmapper.Mapper46.regs[1]*64+g_NESmapper.Mapper46.regs[3]*8+4);
	g_NESmapper.set_PPU_bank5(g_NESmapper.Mapper46.regs[1]*64+g_NESmapper.Mapper46.regs[3]*8+5);
	g_NESmapper.set_PPU_bank6(g_NESmapper.Mapper46.regs[1]*64+g_NESmapper.Mapper46.regs[3]*8+6);
	g_NESmapper.set_PPU_bank7(g_NESmapper.Mapper46.regs[1]*64+g_NESmapper.Mapper46.regs[3]*8+7);
}
/////////////////////////////////////////////////////////////////////

#endif
