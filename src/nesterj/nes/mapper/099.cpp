
/////////////////////////////////////////////////////////////////////
// Mapper 99
void NES_mapper99_Init()
{
	g_NESmapper.Reset = NES_mapper97_Reset;
}

void NES_mapper99_Reset()
{
	// set CPU bank pointers
	if(g_NESmapper.num_8k_ROM_banks > 2)
	{
		g_NESmapper.set_CPU_banks4(0,1,2,3);
	}
	else if(g_NESmapper.num_8k_ROM_banks > 1)
	{
		g_NESmapper.set_CPU_banks4(0,1,0,1);
	}
	else
	{
		g_NESmapper.set_CPU_banks4(0,0,0,0);
	}

	// set VROM bank
	if(g_NESmapper.num_1k_VROM_banks)
	{
		g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);
	}
}

void NES_mapper99_WriteHighRegs(uint32 addr, uint8 data)
{
	if(addr == 0x4016)
	{
		if(data & 0x04)
		{
			g_NESmapper.set_PPU_banks8(8,9,10,11,12,13,14,15);
		}
		else
		{
			g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);
		}
	}
}
/////////////////////////////////////////////////////////////////////

