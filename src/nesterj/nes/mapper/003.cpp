
/////////////////////////////////////////////////////////////////////
// Mapper 3
void NES_mapper3_Init()
{
	g_NESmapper.Reset = NES_mapper3_Reset;
	g_NESmapper.MemoryWrite = NES_mapper3_MemoryWrite;
}

void NES_mapper3_Reset()
{
	// set CPU bank pointers
	if(g_NESmapper.num_8k_ROM_banks > 2)
	{
		g_NESmapper.set_CPU_banks4(0,1,2,3);
	}
	else
	{
		g_NESmapper.set_CPU_banks4(0,1,0,1);
	}

	// set VROM banks
	g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);
}

void NES_mapper3_MemoryWrite(uint32 addr, uint8 data)
{
	uint32 base;
	data &= (g_NESmapper.num_1k_VROM_banks>>1)-1;

	base = ((uint32)data) << 3;
//LOG("W4 " << HEX(base,4)<< endl);
	g_NESmapper.set_PPU_banks8(base+0,base+1,base+2,base+3,base+4,base+5,base+6,base+7);
}
/////////////////////////////////////////////////////////////////////

