
/////////////////////////////////////////////////////////////////////
// Mapper 203


void NES_mapper203_Init()
{
	g_NESmapper.Reset = NES_mapper203_Reset;
	g_NESmapper.MemoryWrite = NES_mapper203_MemoryWrite;
}



void NES_mapper203_Reset()
{
	//ROM_BANK16(0x8000,0);
	//ROM_BANK16(0xc000,0);
	//VROM_BANK8(0);
	g_NESmapper.SetPROM_16K_Bank(4,0);
	g_NESmapper.SetPROM_16K_Bank(6,0);
	g_NESmapper.SetVROM_8K_Bank(0);
}

void NES_mapper203_MemoryWrite(uint32 addr, uint8 data)
{
	//ROM_BANK16(0x8000,(V>>2)&3);
	//ROM_BANK16(0xc000,(V>>2)&3);
	//VROM_BANK8(V&3);
	g_NESmapper.SetPROM_16K_Bank(4,(data>>2)&3);
	g_NESmapper.SetPROM_16K_Bank(6,(data>>2)&3);
	g_NESmapper.SetVROM_8K_Bank(data&3);
}
