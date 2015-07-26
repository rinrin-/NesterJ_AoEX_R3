/////////////////////////////////////////////////////////////////////
// Mapper 205
void NES_mapper205_Init()
{
	Mmc3_Init();
	UpdateChr = NES_mapper205_UpdateChr;
	UpdatePrg = NES_mapper205_UpdatePrg;
	g_NESmapper.Reset = NES_mapper205_Reset;
	g_NESmapper.MemoryWriteLow = NES_mapper205_MemoryWriteLow;//0x4020,0x7FFF
	g_NESmapper.MemoryWriteSaveRAM = NES_mapper205_MemoryWriteLow;//0x4020,0x7FFF
}

void NES_mapper205_Reset()
{
	g_NESmapper.Mapper205.exReg=0;
	Mmc3_Reset();	
}

void NES_mapper205_MemoryWriteLow(uint32 A, uint8 V)
{
	if( ( (A>=0x6800)&&(A<=0x6fff) )||( (A>=0x7800)&&(A<=0x7fff) ) )
	{
		V = V << 4 & 0x30;
		
		if (g_NESmapper.Mapper205.exReg != V)
		{
			g_NESmapper.Mapper205.exReg = V;
			Mmc3_UpdatePrg();
			Mmc3_UpdateChr();
		}
	}
}
/////////////////////////////////////////////////////////////////////


void NES_mapper205_UpdatePrg(unsigned int A,unsigned int V)
{
	g_NESmapper.SetPROM_8K_Bank((A>>13)+4,g_NESmapper.Mapper205.exReg | (V & ((g_NESmapper.Mapper205.exReg & 0x20) ? 0x0F : 0x1F)) );
}
void NES_mapper205_UpdateChr(unsigned int A,unsigned int V)
{
	g_NESmapper.SetVROM_1K_Bank( A>>10, g_NESmapper.Mapper205.exReg << 3 | V);
}