
/////////////////////////////////////////////////////////////////////
// Mapper 212


void NES_mapper212_Init()
{
	g_NESmapper.Reset = NES_mapper212_Reset;
	g_NESmapper.MemoryWrite = NES_mapper212_MemoryWrite;
}



void NES_mapper212_Reset()
{
	NES_mapper212_MemoryWrite(0xffff,0);
}

void NES_mapper212_SwapGfx(unsigned int A)
{
	if(A & 0x8)
		g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
	else
		g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);	
	g_NESmapper.SetVROM_8K_Bank(A);
}

void NES_mapper212_MemoryWrite(uint32 A, uint8 V)
{
	if(A<0xc000)
	{
		g_NESmapper.SetPROM_16K_Bank(4,A);
		g_NESmapper.SetPROM_16K_Bank(6,A);
		NES_mapper212_SwapGfx( A );
	}
	else
	{
		g_NESmapper.SetPROM_32K_Bank(A >> 1);
		NES_mapper212_SwapGfx( A );
	}	
}
