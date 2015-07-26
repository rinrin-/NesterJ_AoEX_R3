void A65as_Init()
{
	g_NESmapper.Reset = A65as_Reset;
	g_NESmapper.MemoryWrite = A65as_MemoryWrite;
}

void A65as_Reset()
{
	A65as_MemoryWrite(0x8000,0);	
}

void A65as_MemoryWrite(uint32 A, uint8 V)
{
	if (V & 0x40)
		g_NESmapper.SetPROM_32K_Bank(V >> 1);
	else
	{
		g_NESmapper.SetPROM_16K_Bank(4,(V >> 1 & 0x18) | (V & 0x7));
		g_NESmapper.SetPROM_16K_Bank(6,(V >> 1 & 0x18) | 0x7 );
	}

	//ppu.SetMirroring
	(V & 0x80) ? 
	(V & 0x20) ? 
	g_NESmapper.set_mirroring(1,1,1,1): 
	g_NESmapper.set_mirroring(0,0,0,0):
	(V & 0x08) ? 
	g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ) : 
	g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
}
/////////////////////////////////////////////////////////////////////