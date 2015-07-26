
/////////////////////////////////////////////////////////////////////
// Mapper 93
void NES_mapper93_Init()
{
	g_NESmapper.Reset = NES_mapper93_Reset;
	g_NESmapper.MemoryWriteSaveRAM = NES_mapper93_MemoryWriteSaveRAM;
}

void NES_mapper93_Reset()
{
	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);
	// set PPU bank pointers
	g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);
}

void NES_mapper93_MemoryWriteSaveRAM(uint32 addr, uint8 data)
{
	if(addr == 0x6000)
	{
		g_NESmapper.set_CPU_bank4(data*2+0);
		g_NESmapper.set_CPU_bank5(data*2+1);
	}
}

/////////////////////////////////////////////////////////////////////

