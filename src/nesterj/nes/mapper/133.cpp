//////////////////////////////////////////////////////////////////////////
// Mapper133  SACHEN CHEN                                             //
//////////////////////////////////////////////////////////////////////////
// Mapper 133
void NES_mapper133_Init()
{
	g_NESmapper.Reset = NES_mapper133_Reset;
	g_NESmapper.MemoryWriteLow = NES_mapper133_MemoryWriteLow;
}

void NES_mapper133_Reset()
{
	g_NESmapper.SetPROM_32K_Bank( 0 );	
	g_NESmapper.SetVROM_8K_Bank( 0 );
}

void NES_mapper133_MemoryWriteLow(uint32 addr, uint8 data)
{
	if(  (addr>=0x4100)&&(addr <= 0x6000) ){
		g_NESmapper.SetPROM_32K_Bank( (data)>>2 );
		g_NESmapper.SetVROM_8K_Bank( data );
	}
}
/////////////////////////////////////////////////////////////////////

