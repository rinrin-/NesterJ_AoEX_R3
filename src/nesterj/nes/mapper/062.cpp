#ifdef _NES_MAPPER_CPP_

/////////////////////////////////////////////////////////////////////
// Mapper 62
void NES_mapper62_Init()
{
	g_NESmapper.Reset = NES_mapper62_Reset;
	g_NESmapper.MemoryWrite = NES_mapper62_MemoryWrite;
	g_NESmapper.MemoryWriteSaveRAM = NES_mapper62_MemoryWriteSaveRAM;
	g_NESmapper.MemoryWriteLow = NES_mapper62_MemoryWriteLow;
}

void NES_mapper62_Reset()
{
	g_NESmapper.set_CPU_banks4(0,1,2,3);
	g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);
}

void NES_mapper62_MemoryWriteSaveRAM(uint32 addr, uint8 data)
{
	LOG("W " << HEX(addr,4) << "  " << HEX(data,2)  << endl);
}

void NES_mapper62_MemoryWriteLow(uint32 addr, uint8 data)
{
	LOG("W " << HEX(addr,4) << "  " << HEX(data,2)  << endl);
}

void NES_mapper62_MemoryWrite(uint32 addr, uint8 data)
{
	LOG("W " << HEX(addr,4) << "  " << HEX(data,2)  << endl);
	switch(addr & 0xFF00){
		case 0x8100:
			g_NESmapper.set_CPU_bank4(data);
			g_NESmapper.set_CPU_bank5(data+1);
			break;
		case 0x8500:
			g_NESmapper.set_CPU_bank4(data);
			break;
		case 0x8700:
			g_NESmapper.set_CPU_bank5(data);
			break;
		g_NESmapper.set_PPU_bank0(data);
		g_NESmapper.set_PPU_bank1(data+1);
		g_NESmapper.set_PPU_bank2(data + 2);
		g_NESmapper.set_PPU_bank3(data + 3);
		g_NESmapper.set_PPU_bank4(data + 4);
		g_NESmapper.set_PPU_bank5(data + 5);
		g_NESmapper.set_PPU_bank6(data + 6);
		g_NESmapper.set_PPU_bank7(data + 7);
	}

}
/////////////////////////////////////////////////////////////////////

#endif
