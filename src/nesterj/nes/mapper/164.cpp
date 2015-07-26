
/////////////////////////////////////////////////////////////////////
// Mapper 164

void NES_mapper164_Init()
{
	g_NESmapper.Reset = NES_mapper164_Reset;
	g_NESmapper.MemoryWriteLow = NES_mapper164_MemoryWriteLow;
	//g_NESmapper.PPU_Latch_FDFE  = NES_mapper164_PPU_ChrLatch;	
}

void NES_mapper164_Reset()
{
	g_NESmapper.Mapper164.regs[0] = 0;
	g_NESmapper.Mapper164.regs[1] = 0;
	
	//prg.SwapBank<SIZE_16K,0x4000>(0x1F);
	g_NESmapper.SetPROM_16K_Bank(6, 0x1f);
}

void NES_mapper164_MemoryWriteLow(uint32 addr, uint8 data)
{
	if( (addr<0x5000)||(addr>=0x6000) )
	return;
	
	//5000~ 51FF
	//5400~ 55FF
	//5800~ 59FF
	//5C00~ 5DFF
	if( ( (addr>=0x5000)&&(addr<=0x51FF) )||
		( (addr>=0x5400)&&(addr<=0x55FF) )||
		( (addr>=0x5800)&&(addr<=0x59FF) )||
		( (addr>=0x5C00)&&(addr<=0x5DFF) ) )
	{
		addr = addr >> 8 & 0x1;

		if (g_NESmapper.Mapper164.regs[addr] != data)
		{
			g_NESmapper.Mapper164.regs[addr] = data;
			data = g_NESmapper.Mapper164.regs[1] << 5 & 0x20;

			switch (g_NESmapper.Mapper164.regs[0] & 0x70)
			{
				case 0x00:
				case 0x20:
				case 0x40:
				case 0x60:

					g_NESmapper.SetPROM_16K_Bank(4, data | (g_NESmapper.Mapper164.regs[0] >> 1 & 0x10) | (g_NESmapper.Mapper164.regs[0] & 0xF));
					g_NESmapper.SetPROM_16K_Bank(6, data | 0x1F);
					break;

				case 0x50:
					g_NESmapper.SetPROM_32K_Bank( (data >> 1) | (g_NESmapper.Mapper164.regs[0] & 0xF));
					break;

				case 0x70:
					g_NESmapper.SetPROM_16K_Bank(4, data | (g_NESmapper.Mapper164.regs[0] << 1 & 0x10) | (g_NESmapper.Mapper164.regs[0] & 0xF));
					g_NESmapper.SetPROM_16K_Bank(6, data | 0x1F);
					break;
			}
		}
	}
	
}
/////////////////////////////////////////////////////////////////////

