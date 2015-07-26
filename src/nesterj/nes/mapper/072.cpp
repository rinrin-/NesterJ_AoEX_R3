
/////////////////////////////////////////////////////////////////////
// Mapper 72 (Created by rinao)
void NES_mapper72_Init()
{
	g_NESmapper.Reset = NES_mapper72_Reset;
	g_NESmapper.MemoryWrite = NES_mapper72_MemoryWrite;
}

void NES_mapper72_Reset()
{
	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);

	// set PPU bank pointers
	if(g_NESmapper.num_1k_VROM_banks)
	{
		g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);
	}
}

void NES_mapper72_MemoryWrite(uint32 addr, uint8 data)
{
	uint8 bank = data & 0x0f;
	if (data & 0x80)
	{
		g_NESmapper.set_CPU_banks4(bank*2, bank*2+1,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);
	}
	if (data & 0x40)
	{
		g_NESmapper.set_PPU_banks8(bank*8, bank*8+1,bank*8+2,bank*8+3,bank*8+4,bank*8+5,bank*8+6,bank*8+7);
	}
}
/////////////////////////////////////////////////////////////////////

