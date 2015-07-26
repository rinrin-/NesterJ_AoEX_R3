void BmcWs_Init()
{
	g_NESmapper.Reset = BmcWs_Reset;
	g_NESmapper.MemoryWriteSaveRAM = Poke_BmcWs_600X;
}

void BmcWs_Reset()
{
	g_NESmapper.BmcWs.reg = 0x00;
	Poke_BmcWs_6000(0x6000,0);
}

void Poke_BmcWs_600X(uint32 A,uint8 V)
{
	if(A<0x7000)
	{
		if(A&1)
			Poke_BmcWs_6001(A,V);
		else
			Poke_BmcWs_6000(A,V);
	}
}

void Poke_BmcWs_6000(uint32 A,uint8 V)
{
	if (!g_NESmapper.BmcWs.reg)
	{
		g_NESmapper.BmcWs.reg = V & 0x20;
		g_NESmapper.SetPROM_16K_Bank(4,V & ~(~V >> 3 & 0x1));
		g_NESmapper.SetPROM_16K_Bank(6,V | (~V >> 3 & 0x1) );
		
		if(V & 0x10)
			g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
		else
			g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
	}
}

void Poke_BmcWs_6001(uint32 A,uint8 V)
{
	if (!g_NESmapper.BmcWs.reg)
	{
		g_NESmapper.SetVROM_8K_Bank( V );
	}
}