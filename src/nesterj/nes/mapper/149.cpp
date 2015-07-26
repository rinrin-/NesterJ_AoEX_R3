void NES_mapper149_Init()
{
	g_NESmapper.Reset = NES_mapper0_Reset;
	g_NESmapper.MemoryWrite = NES_mapper149_MemoryWrite;
}

void NES_mapper149_MemoryWrite(uint32 addr, uint8 data)
{
	if(  (addr>=0x8000)&&(addr <= 0xffff) ){
		g_NESmapper.SetVROM_8K_Bank( data>>7 );
	}
}
/////////////////////////////////////////////////////////////////////

