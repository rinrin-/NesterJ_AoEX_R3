
/////////////////////////////////////////////////////////////////////
// Mapper 34
void NES_mapper34_Init()
{
	g_NESmapper.Reset = NES_mapper34_Reset;
	g_NESmapper.MemoryWriteSaveRAM = NES_mapper34_MemoryWriteSaveRAM;
	g_NESmapper.MemoryWrite = NES_mapper34_MemoryWrite;
}

void NES_mapper34_Reset()
{
	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);

	// set PPU bank pointers
	if(g_NESmapper.num_1k_VROM_banks)
	{
		g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);
	}
}

void NES_mapper34_MemoryWriteSaveRAM(uint32 addr, uint8 data)
{
	switch(addr)
	{
	case 0x7FFD:
		{
			g_NESmapper.set_CPU_banks4(data*4,data*4+1,data*4+2,data*4+3);
		}
		break;

	case 0x7FFE:
		{
			g_NESmapper.set_PPU_bank0(data*4+0);
			g_NESmapper.set_PPU_bank1(data*4+1);
			g_NESmapper.set_PPU_bank2(data*4+2);
			g_NESmapper.set_PPU_bank3(data*4+3);
		}
		break;

	case 0x7FFF:
		{
			g_NESmapper.set_PPU_bank4(data*4+0);
			g_NESmapper.set_PPU_bank5(data*4+1);
			g_NESmapper.set_PPU_bank6(data*4+2);
			g_NESmapper.set_PPU_bank7(data*4+3);
		}
		break;
	}
}

void NES_mapper34_MemoryWrite(uint32 addr, uint8 data)
{
	g_NESmapper.set_CPU_banks4(data*4,data*4+1,data*4+2,data*4+3);
}
/////////////////////////////////////////////////////////////////////

