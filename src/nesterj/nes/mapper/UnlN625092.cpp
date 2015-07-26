void N625092_Init()
{
	g_NESmapper.Reset = N625092_Reset;
	g_NESmapper.MemoryWrite = N625092_MemoryWrite;
}

void N625092_Reset()
{
	g_NESmapper.n625092.regs[0] = 0;
	g_NESmapper.n625092.regs[1] = 0;
	N625092_UpdatePrg();
}

void N625092_UpdatePrg()
{
	g_NESmapper.SetPROM_16K_Bank(4,(g_NESmapper.n625092.regs[0] >> 1 & 0x38) | ((g_NESmapper.n625092.regs[0] & 0x1) ? (g_NESmapper.n625092.regs[0] & 0x80) ? g_NESmapper.n625092.regs[1] : (g_NESmapper.n625092.regs[1] & 0x6) | 0x0 : g_NESmapper.n625092.regs[1]));
	g_NESmapper.SetPROM_16K_Bank(6,(g_NESmapper.n625092.regs[0] >> 1 & 0x38) | ((g_NESmapper.n625092.regs[0] & 0x1) ? (g_NESmapper.n625092.regs[0] & 0x80) ?     0x7 : (g_NESmapper.n625092.regs[1] & 0x6) | 0x1 : g_NESmapper.n625092.regs[1]));
}


void N625092_MemoryWrite(uint32 A, uint8 V)
{
	if(A<0xC000)
	{
		if(A & 0x1)
			g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
		else
			g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
		
		A = A >> 1 & 0xFF;

		if (g_NESmapper.n625092.regs[0] != A)
		{
			g_NESmapper.n625092.regs[0] = A;
			N625092_UpdatePrg();
		}
	}
	else if(A>0xBFFF)
	{
		A &= 0x7;

		if (g_NESmapper.n625092.regs[1] != A)
		{
			g_NESmapper.n625092.regs[1] = A;
			N625092_UpdatePrg();
		}	
	}
}