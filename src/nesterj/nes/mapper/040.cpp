
/////////////////////////////////////////////////////////////////////
// Mapper 40 (smb2j)
void NES_mapper40_Init()
{
	g_NESmapper.Reset = NES_mapper40_Reset;
	g_NESmapper.MemoryWrite = NES_mapper40_MemoryWrite;
}

void NES_mapper40_Reset()
{
	g_NESmapper.Mapper40.irq_enabled = 0;
	g_NESmapper.Mapper40.lines_to_irq = 0;

	// set CPU bank pointers
	g_NESmapper.set_CPU_banks5(6,4,5,0,7);

	// set VROM banks
	if(g_NESmapper.num_1k_VROM_banks)
	{
		g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);
	}
}

void NES_mapper40_MemoryWrite(uint32 addr, uint8 data)
{
	switch(addr & 0xE000)
	{
	case 0x8000:
		g_NESmapper.Mapper40.irq_enabled = 0;
		//      LOG("MAP40: [$8000] = " << HEX(data,2) << endl);
		break;

	case 0xA000:
		g_NESmapper.Mapper40.irq_enabled = 1;
		g_NESmapper.Mapper40.lines_to_irq = 37;

		//      LOG("MAP40: [$A000] = " << HEX(data,2) << endl);
		break;

	case 0xC000:
		//      LOG("MAP40: [$C000] = " << HEX(data,2) << endl);
		//      LOG("MAP40: INVALID WRITE TO $C000" << endl);
		break;

	case 0xE000:
		//      LOG("MAP40: [$E000] = " << HEX(data,2) << endl);

		g_NESmapper.set_CPU_bank6(data & 0x07);
		break;

	}
}

void NES_mapper40_HSync(uint32 scanline)
{
	if(g_NESmapper.Mapper40.irq_enabled)
	{
		if((--g_NESmapper.Mapper40.lines_to_irq) <= 0)
		{
			NES6502_DoIRQ();
		}
	}
}
/////////////////////////////////////////////////////////////////////

