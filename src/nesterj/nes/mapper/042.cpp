
/////////////////////////////////////////////////////////////////////
// Mapper 42
void NES_mapper42_Init()
{
	g_NESmapper.Reset = NES_mapper42_Reset;
	g_NESmapper.MemoryWrite = NES_mapper42_MemoryWrite;
	g_NESmapper.HSync = NES_mapper42_HSync;
}

void NES_mapper42_Reset()
{
	// set CPU bank pointers
	g_NESmapper.set_CPU_bank3(0);
	g_NESmapper.set_CPU_bank4(g_NESmapper.num_8k_ROM_banks-4);
	g_NESmapper.set_CPU_bank5(g_NESmapper.num_8k_ROM_banks-3);
	g_NESmapper.set_CPU_bank6(g_NESmapper.num_8k_ROM_banks-2);
	g_NESmapper.set_CPU_bank7(g_NESmapper.num_8k_ROM_banks-1);

	// set PPU bank pointers
	g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);
}

void NES_mapper42_MemoryWrite(uint32 addr, uint8 data)
{
	switch(addr & 0xE003)
	{
	case 0x8000:
		g_NESmapper.set_PPU_banks8(((data&0x1f)<<3)+0,((data&0x1f)<<3)+1,((data&0x1f)<<3)+2,((data&0x1f)<<3)+3,((data&0x1f)<<3)+4,((data&0x1f)<<3)+5,((data&0x1f)<<3)+6,((data&0x1f)<<3)+7);
		break;
	case 0xE000:
		{
			g_NESmapper.set_CPU_bank3(data & 0x0F);
		}
		break;

	case 0xE001:
		{
			if(data & 0x08)
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
			}
			else
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
			}
		}
		break;

	case 0xE002:
		{
			if(data & 0x02)
			{
				g_NESmapper.Mapper42.irq_enabled = 1;
			}
			else
			{
				g_NESmapper.Mapper42.irq_enabled = 0;
				g_NESmapper.Mapper42.irq_counter = 0;
			}
		}
		break;
	}
//	LOG("W " << HEX(addr,4) << "  " << HEX(data,2) << endl);
}

void NES_mapper42_HSync(uint32 scanline)
{
	if(g_NESmapper.Mapper42.irq_enabled)
	{
		if(g_NESmapper.Mapper42.irq_counter < 215)
		{
			g_NESmapper.Mapper42.irq_counter++;
		}
		if(g_NESmapper.Mapper42.irq_counter == 215)
		{
			NES6502_DoIRQ();
			g_NESmapper.Mapper42.irq_enabled = 0;
		}
	}
}
/////////////////////////////////////////////////////////////////////

