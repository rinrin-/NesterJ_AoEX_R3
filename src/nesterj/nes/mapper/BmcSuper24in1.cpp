/////////////////////////////////////////////////////////////////////
// BmcSuper24in1
void BmcSuper24in1_Init()
{
	Mmc3_Init();
	
	UpdateChr = BmcSuper24in1_UpdateChr;
	UpdatePrg = BmcSuper24in1_UpdatePrg;
	g_NESmapper.Reset = BmcSuper24in1_Reset;
	g_NESmapper.MemoryWriteLow = BmcSuper24in1_MemoryWriteLow;
}

void BmcSuper24in1_Reset()
{	
	g_NESmapper.BmcSuper24in1.exRegs[0] = 0x24;
	g_NESmapper.BmcSuper24in1.exRegs[1] = 0x9F;
	g_NESmapper.BmcSuper24in1.exRegs[2] = 0x00;						
	
	Mmc3_Reset();	
}

void BmcSuper24in1_MemoryWriteLow(uint32 A, uint8 V)
{
	if(A==0x5ff0)
	{
		if (g_NESmapper.BmcSuper24in1.exRegs[0] != V)
		{
			g_NESmapper.BmcSuper24in1.exRegs[0] = V;
			Mmc3_UpdateChr();
			Mmc3_UpdatePrg();
		}
	} else if(A==0x5ff1)
	{
		if (g_NESmapper.BmcSuper24in1.exRegs[1] != V)
		{
			g_NESmapper.BmcSuper24in1.exRegs[1] = V;
			Mmc3_UpdatePrg();
		}
	}
	else if(A==0x5ff2)
	{
		if (g_NESmapper.BmcSuper24in1.exRegs[2] != V)
		{
			g_NESmapper.BmcSuper24in1.exRegs[2] = V;
			Mmc3_UpdateChr();
		}
	}	
}


void BmcSuper24in1_UpdatePrg(unsigned int A,unsigned int V)
{
	static const uint8 masks[8] = {0x3F,0x1F,0xF,0x1,0x3,0x0,0x0,0x0};
	//prg.SwapBank<SIZE_8K>( address, (exRegs[1] << 1) | (bank & masks[exRegs[0] & 0x7]) );
	g_NESmapper.SetPROM_8K_Bank( (A>>13)+4,
	(g_NESmapper.BmcSuper24in1.exRegs[1] << 1) | (V & masks[g_NESmapper.BmcSuper24in1.exRegs[0] & 0x7]));
}
void BmcSuper24in1_UpdateChr(unsigned int A,unsigned int V)
{
	//chr.Source( exRegs[0] >> 5 & 0x1 ).SwapBank<SIZE_1K>( address, (exRegs[2] << 3 & 0xF00) | bank );
	
	if( g_NESmapper.BmcSuper24in1.exRegs[0] >> 5 & 0x1 )
	{
		g_NESmapper.SetCRAM_1K_Bank( A>>10, (g_NESmapper.BmcSuper24in1.exRegs[2] << 3 & 0xF00) | V );
	}else{
		g_NESmapper.SetVROM_1K_Bank( A>>10, (g_NESmapper.BmcSuper24in1.exRegs[2] << 3 & 0xF00) | V );
	}

}
/////////////////////////////////////////////////////////////////////













