/////////////////////////////////////////////////////////////////////
// Mapper 134
void NES_mapper134_Init()
{
	Mmc3_Init();
	UpdateChr = NES_mapper134_UpdateChr;
	UpdatePrg = NES_mapper134_UpdatePrg;
	g_NESmapper.Reset = NES_mapper134_Reset;
	g_NESmapper.MemoryWriteSaveRAM = NES_mapper134_MemoryWriteSaveRAM;
}

void NES_mapper134_Reset()
{
	g_NESmapper.Mapper134.exReg = 0;
	Mmc3_Reset();	
}

void NES_mapper134_MemoryWriteSaveRAM(uint32 A, uint8 V)
{
	if(A==0x6001)
	{
		if (g_NESmapper.Mapper134.exReg != V)
		{
			g_NESmapper.Mapper134.exReg = V;
			Mmc3_UpdatePrg();
			Mmc3_UpdateChr();
		}
	}
}



void NES_mapper134_UpdatePrg(unsigned int A,unsigned int V)
{
	g_NESmapper.SetPROM_8K_Bank( (A>>13)+4, ((g_NESmapper.Mapper134.exReg &2 )<< 4) | (V & 0x1F));
}
void NES_mapper134_UpdateChr(unsigned int A,unsigned int V)
{
	g_NESmapper.SetVROM_1K_Bank( A>>10, ((g_NESmapper.Mapper134.exReg&0x20) << 3) | (V&0xff) );
}