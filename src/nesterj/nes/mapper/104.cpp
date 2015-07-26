
/////////////////////////////////////////////////////////////////////
// Mapper 104
void NES_mapper104_Init()
{
	g_NESmapper.Reset = NES_mapper104_Reset;
	g_NESmapper.MemoryWrite = NES_mapper104_MemoryWrite;
}

void NES_mapper104_Reset()
{
	g_NESmapper.SetPROM_16K_Bank(6,0xf);
	g_NESmapper.Mapper104.prg_bank0 = 0;
}

void NES_mapper104_MemoryWrite(uint32 A, uint8 V)
{
	if(A<0xA000)//8000
	{
	
		if (V & 0x8)
		{
			g_NESmapper.Mapper104.prg_bank0 = ((V & 0x07) << 4) | (g_NESmapper.Mapper104.prg_bank0 & 0x0f);
			//prg.SwapBank<SIZE_16K,0x0000>( (V << 4 & 0x70) | (prg.GetBank<SIZE_16K,0x0000>() & 0x0F) );
			g_NESmapper.SetPROM_16K_Bank(4,g_NESmapper.Mapper104.prg_bank0);
			//prg.SwapBank<SIZE_16K,0x4000>( (V << 4 & 0x70) | 0x0F );
			g_NESmapper.SetPROM_16K_Bank(6,(V << 4 & 0x70) | 0x0F);
		}
	
	}
	else if(A>=0xc000)//c000
	{
		g_NESmapper.Mapper104.prg_bank0  = (g_NESmapper.Mapper104.prg_bank0 & 0x70) | (V & 0x0f);
		g_NESmapper.SetPROM_16K_Bank(4,g_NESmapper.Mapper104.prg_bank0);
		//prg.SwapBank<SIZE_16K,0x0000>( (prg.GetBank<SIZE_16K,0x0000>() & 0x70) | (data & 0x0F) );
	}	
}
