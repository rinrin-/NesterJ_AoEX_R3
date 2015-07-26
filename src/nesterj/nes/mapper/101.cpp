#ifdef _NES_MAPPER_CPP_

/////////////////////////////////////////////////////////////////////
// Mapper 101
void NES_mapper101_Init()
{
	g_NESmapper.Reset = NES_mapper101_Reset;
	g_NESmapper.MemoryWriteSaveRAM = NES_mapper101_MemoryWriteSaveRAM;
	g_NESmapper.MemoryWrite = NES_mapper101_MemoryWrite;
}

void NES_mapper101_Reset()
{
	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,2,3);
	g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);
}

void NES_mapper101_MemoryWriteSaveRAM(uint32 addr, uint8 data)
{
	data &= 0x03;
	g_NESmapper.set_PPU_bank0(data*8+0);
	g_NESmapper.set_PPU_bank1(data*8+1);
	g_NESmapper.set_PPU_bank2(data*8+2);
	g_NESmapper.set_PPU_bank3(data*8+3);
	g_NESmapper.set_PPU_bank4(data*8+4);
	g_NESmapper.set_PPU_bank5(data*8+5);
	g_NESmapper.set_PPU_bank6(data*8+6);
	g_NESmapper.set_PPU_bank7(data*8+7);
}

void NES_mapper101_MemoryWrite(uint32 addr, uint8 data)
{
	data &= 0x03;
	g_NESmapper.set_PPU_bank0(data*8+0);
	g_NESmapper.set_PPU_bank1(data*8+1);
	g_NESmapper.set_PPU_bank2(data*8+2);
	g_NESmapper.set_PPU_bank3(data*8+3);
	g_NESmapper.set_PPU_bank4(data*8+4);
	g_NESmapper.set_PPU_bank5(data*8+5);
	g_NESmapper.set_PPU_bank6(data*8+6);
	g_NESmapper.set_PPU_bank7(data*8+7);
}
/////////////////////////////////////////////////////////////////////

#endif
