
/////////////////////////////////////////////////////////////////////
// Mapper 58
void NES_mapper58_Init()
{
	g_NESmapper.Reset = NES_mapper58_Reset;
	g_NESmapper.MemoryWrite = NES_mapper58_MemoryWrite;
}

void NES_mapper58_Reset()
{
	g_NESmapper.set_CPU_banks4(0,1,0,1);
	g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);
}

void NES_mapper58_MemoryWrite(uint32 addr, uint8 data)
{
	LOG("W " << HEX(addr,4) << "  " << HEX(data,2)  << endl);
	if(addr & 0x40)
	{
		g_NESmapper.set_CPU_bank4(2 * (addr & 0x07) + 0);
		g_NESmapper.set_CPU_bank5(2 * (addr & 0x07) + 1);
		g_NESmapper.set_CPU_bank6(2 * (addr & 0x07) + 0);
		g_NESmapper.set_CPU_bank7(2 * (addr & 0x07) + 1);
	}
	else
	{
		g_NESmapper.set_CPU_bank4(4 * ((addr & 0x06) >> 1) + 0);
		g_NESmapper.set_CPU_bank5(4 * ((addr & 0x06) >> 1) + 1);
		g_NESmapper.set_CPU_bank6(4 * ((addr & 0x06) >> 1) + 2);
		g_NESmapper.set_CPU_bank7(4 * ((addr & 0x06) >> 1) + 3);
	}

	g_NESmapper.set_PPU_bank0(8 * ((addr & 0x38) >> 3) + 0);
	g_NESmapper.set_PPU_bank1(8 * ((addr & 0x38) >> 3) + 1);
	g_NESmapper.set_PPU_bank2(8 * ((addr & 0x38) >> 3) + 2);
	g_NESmapper.set_PPU_bank3(8 * ((addr & 0x38) >> 3) + 3);
	g_NESmapper.set_PPU_bank4(8 * ((addr & 0x38) >> 3) + 4);
	g_NESmapper.set_PPU_bank5(8 * ((addr & 0x38) >> 3) + 5);
	g_NESmapper.set_PPU_bank6(8 * ((addr & 0x38) >> 3) + 6);
	g_NESmapper.set_PPU_bank7(8 * ((addr & 0x38) >> 3) + 7);

	if(data & 0x02)
	{
		g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
	}
	else
	{
		g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
	}
}
/////////////////////////////////////////////////////////////////////

