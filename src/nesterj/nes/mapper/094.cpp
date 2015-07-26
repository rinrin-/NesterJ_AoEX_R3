#ifdef _NES_MAPPER_CPP_

/////////////////////////////////////////////////////////////////////
// Mapper 94
void NES_mapper94_Init()
{
	g_NESmapper.Reset = NES_mapper94_Reset;
	g_NESmapper.MemoryWrite = NES_mapper94_MemoryWrite;
}

void NES_mapper94_Reset()
{
	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);
}

void NES_mapper94_MemoryWrite(uint32 addr, uint8 data)
{
	if((addr & 0xFFF0) == 0xFF00)
	{
		uint8 bank = (data & 0x1C) >> 2;
		g_NESmapper.set_CPU_bank4(bank*2+0);
		g_NESmapper.set_CPU_bank5(bank*2+1);
	}
	//  LOG("W " << HEX(addr,4) << HEX(data,2)<< endl);
}

/////////////////////////////////////////////////////////////////////

#endif
