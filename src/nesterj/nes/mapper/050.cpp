
/////////////////////////////////////////////////////////////////////
// Mapper 50
void NES_mapper50_Init()
{
	g_NESmapper.Reset = NES_mapper50_Reset;
	g_NESmapper.MemoryWriteLow = NES_mapper50_MemoryWriteLow;
	g_NESmapper.HSync = NES_mapper50_HSync;
}

void NES_mapper50_Reset()
{
	g_NESmapper.set_CPU_banks5(15,8,9,0,11);
	g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);

	g_NESmapper.Mapper50.irq_enabled = 0;
}

void NES_mapper50_MemoryWriteLow(uint32 addr, uint8 data)
{
	if((addr & 0xE060) == 0x4020)
	{
		if(addr & 0x0100)
		{
			g_NESmapper.Mapper50.irq_enabled = data & 0x01;
		}
		else
		{
			g_NESmapper.set_CPU_bank6((data&0x08)|((data&0x01)<<2)|((data&0x06)>>1));
		}
	}
}

void NES_mapper50_HSync(uint32 scanline)
{
	if(g_NESmapper.Mapper50.irq_enabled)
	{
		if(scanline == 21)
		{
			NES6502_DoIRQ();
		}
	}
}
/////////////////////////////////////////////////////////////////////

