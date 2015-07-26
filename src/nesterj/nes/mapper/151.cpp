#ifdef _NES_MAPPER_CPP_

/////////////////////////////////////////////////////////////////////
// Mapper 151
void NES_mapper151_Init()
{
	g_NESmapper.Reset = NES_mapper151_Reset;
	g_NESmapper.MemoryWrite = NES_mapper151_MemoryWrite;
}
void NES_mapper151_Reset()
{
	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);

	// set VROM banks
	g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);
}

void NES_mapper151_MemoryWrite(uint32 addr, uint8 data)
{
	switch(addr & 0xF000)
	{
	case 0x8000:
		{
			g_NESmapper.set_CPU_bank4(data);
		}
		break;

	case 0xA000:
		{
			g_NESmapper.set_CPU_bank5(data);
		}
		break;

	case 0xC000:
		{
			g_NESmapper.set_CPU_bank6(data);
		}
		break;

	case 0xE000:
		{
			g_NESmapper.set_PPU_bank0(data*4+0);
			g_NESmapper.set_PPU_bank1(data*4+1);
			g_NESmapper.set_PPU_bank2(data*4+2);
			g_NESmapper.set_PPU_bank3(data*4+3);
		}
		break;

	case 0xF000:
		{
			g_NESmapper.set_PPU_bank4(data*4+0);
			g_NESmapper.set_PPU_bank5(data*4+1);
			g_NESmapper.set_PPU_bank6(data*4+2);
			g_NESmapper.set_PPU_bank7(data*4+3);
		}
		break;
	}
}
/////////////////////////////////////////////////////////////////////

#endif
