
/////////////////////////////////////////////////////////////////////
// Mapper 232
void NES_mapper232_Init()
{
	g_NESmapper.Reset = NES_mapper232_Reset;
	g_NESmapper.MemoryWrite = NES_mapper232_MemoryWrite;
}

void NES_mapper232_Reset()
{
	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);

	g_NESmapper.Mapper232.regs[0] = 0x0C;
	g_NESmapper.Mapper232.regs[1] = 0x00;
}

void NES_mapper232_MemoryWrite(uint32 addr, uint8 data)
{
	if(addr == 0x9000)
	{
		g_NESmapper.Mapper232.regs[0] = (data & 0x18) >> 1;
	}
	else if(0xA000 <= addr && addr <= 0xFFFF)
	{
		g_NESmapper.Mapper232.regs[1] = data & 0x03;
	}

	g_NESmapper.set_CPU_bank4((g_NESmapper.Mapper232.regs[0] | g_NESmapper.Mapper232.regs[1]) * 2 + 0);
	g_NESmapper.set_CPU_bank5((g_NESmapper.Mapper232.regs[0] | g_NESmapper.Mapper232.regs[1]) * 2 + 1);
	g_NESmapper.set_CPU_bank6((g_NESmapper.Mapper232.regs[0] | 0x03) * 2 + 0);
	g_NESmapper.set_CPU_bank7((g_NESmapper.Mapper232.regs[0] | 0x03) * 2 + 1);
}
/////////////////////////////////////////////////////////////////////

