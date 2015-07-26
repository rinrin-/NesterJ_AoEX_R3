/////////////////////////////////////////////////////////////////////
// Mapper 200
void NES_mapper200_Init()
{
	g_NESmapper.Reset = NES_mapper200_Reset;
	g_NESmapper.MemoryWrite = NES_mapper200_MemoryWrite;
}

void NES_mapper200_Reset()
{
	NES_mapper200_MemoryWrite(0x8000,0);
}


void NES_mapper200_MemoryWrite(uint32 A, uint8 V)
{
	if(A & 0x8)
		g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
	else
		g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
	g_NESmapper.SetPROM_16K_Bank(4,A);
	g_NESmapper.SetPROM_16K_Bank(6,A);	
	g_NESmapper.SetVROM_8K_Bank(A);
}
/////////////////////////////////////////////////////////////////////

