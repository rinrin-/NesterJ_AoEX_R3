void BmcY2k64in1_Init()
{
	g_NESmapper.Reset = BmcY2k64in1_Reset;
	g_NESmapper.MemoryWrite = BmcY2k64in1_MemoryWrite;
	g_NESmapper.MemoryWriteLow = BmcY2k64in1_MemoryWriteLow;
}

void BmcY2k64in1_Reset()
{
	g_NESmapper.y2k.regs[0] = 0x80;
	g_NESmapper.y2k.regs[1] = 0x43;
	g_NESmapper.y2k.regs[2] = 0x00;
	g_NESmapper.y2k.regs[3] = 0x00;

	Y2k64in1_Update();
}

void Y2k64in1_Update()
{
	unsigned int bank = g_NESmapper.y2k.regs[1] & 0x1FU;

	if (g_NESmapper.y2k.regs[0] & 0x80U & g_NESmapper.y2k.regs[1])
	{
		g_NESmapper.SetPROM_32K_Bank( bank );
	}
	else
	{
		bank = (bank << 1) | (g_NESmapper.y2k.regs[1] >> 6 & 0x1U);
		g_NESmapper.SetPROM_16K_Bank(6, bank );

		if (g_NESmapper.y2k.regs[0] & 0x80U)
			g_NESmapper.SetPROM_16K_Bank(4, bank );
	}

		
	if(g_NESmapper.y2k.regs[0] & 0x20U)
		g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
	else
		g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
		
	g_NESmapper.SetVROM_8K_Bank( (g_NESmapper.y2k.regs[2] << 2) | (g_NESmapper.y2k.regs[0] >> 1 & 0x3U) );
}

void BmcY2k64in1_MemoryWriteLow(uint32 A, uint8 V)
{
	if( (A>=0x5000)&&(A<=0x5003) )
		{
		g_NESmapper.y2k.regs[A & 0x3] = V;
		Y2k64in1_Update();
	}
}


void BmcY2k64in1_MemoryWrite(uint32 A, uint8 V)
{
	g_NESmapper.y2k.regs[3] = V;
	Y2k64in1_Update();
}