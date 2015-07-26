/////////////////////////////////////////////////////////////////////
// Mapper 202
void NES_mapper202_Init()
{
	g_NESmapper.Reset = NES_mapper202_Reset;
	g_NESmapper.MemoryWrite = NES_mapper202_MemoryWrite;
	g_NESmapper.MemoryWriteSaveRAM = NES_mapper202_MemoryWrite;
	g_NESmapper.MemoryWriteLow = NES_mapper202_MemoryWrite;
}

void NES_mapper202_Reset()
{
	NES_mapper202_MemoryWrite(0x8000,0);
}


void NES_mapper202_MemoryWrite(uint32 A, uint8 V)
{
	if(A>=0x4020)
	{
		if(A & 0x1)
			g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
		else
			g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);			
			
		const unsigned int bank = A >> 1 & 0x7;			
		g_NESmapper.SetPROM_16K_Bank(4,bank);
		g_NESmapper.SetPROM_16K_Bank(6,bank + ((A & 0xC) == 0xC));	
		g_NESmapper.SetVROM_8K_Bank(bank);
	}
}
/////////////////////////////////////////////////////////////////////

