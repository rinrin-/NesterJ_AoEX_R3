//////////////////////////////////////////////////////////////////////////
// Mapper142  SMB2j Pirate (KS 202)                                     //
//////////////////////////////////////////////////////////////////////////

void NES_mapper142_Init()
{
	g_NESmapper.Reset = NES_mapper142_Reset;
	g_NESmapper.MemoryWrite = NES_mapper142_MemoryWrite;
	g_NESmapper.HSync = NES_mapper142_HSync;
}

void NES_mapper142_Reset()
{
	int i;
	for( i = 0; i < 4; i++ ) g_NESmapper.Mapper142.reg[i] = i;
	g_NESmapper.Mapper142.reg[4] = 0;
	g_NESmapper.Mapper142.irq_enable = 0;
	g_NESmapper.Mapper142.irq_counter = 0;
	NES_mapper142_SetBank();
}

void NES_mapper142_MemoryWrite( uint32 addr, uint8 data )
{
	switch(addr & 0xF000)
	{
		case 0x8000:
			g_NESmapper.Mapper142.irq_counter = (g_NESmapper.Mapper142.irq_counter & 0xFFF0) | (data & 0x0F);
			break;
		case 0x9000:
			g_NESmapper.Mapper142.irq_counter = (g_NESmapper.Mapper142.irq_counter & 0xFF0F) | ((data & 0x0F)<<4);
			break;
		case 0xA000:
			g_NESmapper.Mapper142.irq_counter = (g_NESmapper.Mapper142.irq_counter & 0xF0FF) | ((data & 0x0F)<<8);
			break;
		case 0xB000:
			g_NESmapper.Mapper142.irq_counter = (g_NESmapper.Mapper142.irq_counter & 0x0FFF) | ((data & 0x0F)<<12);
			break;
		case 0xC000:
			g_NESmapper.Mapper142.irq_enable = data & 0x0F;
			break;
		case 0xE000:
			g_NESmapper.Mapper142.reg[4] = data & 0x0F;
			break;
		case 0xF000:
			switch(g_NESmapper.Mapper142.reg[4])
			{
				case 1:	g_NESmapper.Mapper142.reg[0] = data & 0x0F;break;
				case 2:	g_NESmapper.Mapper142.reg[1] = data & 0x0F;break;
				case 3:	g_NESmapper.Mapper142.reg[2] = data & 0x0F;break;
				case 4: g_NESmapper.Mapper142.reg[3] = data & 0x0F;break;
			}
			NES_mapper142_SetBank();
			break;
	}
}

void NES_mapper142_SetBank()
{
	g_NESmapper.set_CPU_banks4(g_NESmapper.Mapper142.reg[0],g_NESmapper.Mapper142.reg[1],g_NESmapper.Mapper142.reg[2],g_NESmapper.Mapper142.reg[3]);
	g_NESmapper.set_CPU_bank3(g_NESmapper.Mapper142.reg[3]);
	g_NESmapper.set_CPU_bank7(0xFF);
}

void NES_mapper142_HSync(uint32 scanline)
{
	if( g_NESmapper.Mapper142.irq_enable ) {
		g_NESmapper.Mapper142.irq_counter += 248;
		if( g_NESmapper.Mapper142.irq_counter >= 0x3000 ) {
			g_NESmapper.Mapper142.irq_counter = 0;
			NES6502_DoIRQ();
		}
	}
}
