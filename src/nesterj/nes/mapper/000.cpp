
/////////////////////////////////////////////////////////////////////
// Mapper 0
void NES_mapper0_Reset();

void NES_mapper0_Init()
{
	g_NESmapper.Reset = NES_mapper0_Reset;
}

void NES_mapper0_Reset()
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

	if(g_NESmapper.num_1k_VROM_banks)
	{
		g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);
	}
}
/////////////////////////////////////////////////////////////////////

