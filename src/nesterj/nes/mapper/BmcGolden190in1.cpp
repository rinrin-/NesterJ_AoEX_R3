
void Golden190in1_Reset()
{
	Golden190in1_MemoryWrite(0x8000,0);
}
void Golden190in1_MemoryWrite(uint32 A, uint8 V)
{
	if(A &0x1)
		g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
	else
		g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);	
		
	V >>= 2;
	g_NESmapper.SetPROM_16K_Bank(4,V);
	g_NESmapper.SetPROM_16K_Bank(6,V);
	g_NESmapper.SetVROM_8K_Bank(V);
}
void Golden190in1_Init()
{
	g_NESmapper.Reset = Golden190in1_Reset;
	g_NESmapper.MemoryWrite = Golden190in1_MemoryWrite;
}

