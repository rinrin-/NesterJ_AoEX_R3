/////////////////////////////////////////////////////////////////////
// Mapper 241
void NES_mapper241_Init()
{
	g_NESmapper.Reset = NES_mapper241_Reset;
	g_NESmapper.MemoryWrite = NES_mapper241_MemoryWrite;
	g_NESmapper.MemoryReadLow = NES_mapper241_MemoryReadLow;
}

void NES_mapper241_Reset()
{
		g_NESmapper.SetPROM_32K_Bank(0);
}


uint8 NES_mapper241_MemoryReadLow(uint32 addr)
{
	if( (addr>=0x4020)&&(addr<=0x5fff) )
		return 0x50;
	return (uint8)(addr>>8);
}

void NES_mapper241_MemoryWrite(uint32 addr, uint8 data)
{
	if(addr>=0x8000)
		g_NESmapper.SetPROM_32K_Bank(data);
}
/////////////////////////////////////////////////////////////////////