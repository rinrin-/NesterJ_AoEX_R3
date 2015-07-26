#ifdef _NES_MAPPER_CPP_

/////////////////////////////////////////////////////////////////////
// Mapper 17
void NES_mapper17_Init()
{
	g_NESmapper.Reset = NES_mapper17_Reset;
	g_NESmapper.MemoryWriteLow = NES_mapper17_MemoryWriteLow;
	g_NESmapper.HSync = NES_mapper17_HSync;
}

void NES_mapper17_Reset()
{
	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);

	// set PPU bank pointers
	if(g_NESmapper.num_1k_VROM_banks)
	{
		g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);
	}

	g_NESmapper.Mapper17.irq_enabled = 0;
	g_NESmapper.Mapper17.irq_counter = 0;
	g_NESmapper.Mapper17.irq_latch = 0;
}

void NES_mapper17_MemoryWriteLow(uint32 addr, uint8 data)
{
	switch(addr)
	{
	case 0x42FE:
		{
			if((data & 0x10) == 0)
			{
				g_NESmapper.set_mirroring(0,0,0,0);
			}
			else
			{
				g_NESmapper.set_mirroring(1,1,1,1);
			}
		}
		break;

	case 0x42FF:
		{
			if((data & 0x10) == 0)
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
			}
			else
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
			}
		}
		break;

	case 0x4501:
		{
			g_NESmapper.Mapper17.irq_enabled = 0;
		}
		break;

	case 0x4502:
		{
			g_NESmapper.Mapper17.irq_latch = (g_NESmapper.Mapper17.irq_latch & 0xFF00) | data;
		}
		break;

	case 0x4503:
		{
			g_NESmapper.Mapper17.irq_latch = (g_NESmapper.Mapper17.irq_latch & 0x00FF) | ((uint32)data << 8);
			g_NESmapper.Mapper17.irq_counter = g_NESmapper.Mapper17.irq_latch;
			g_NESmapper.Mapper17.irq_enabled = 1;
		}
		break;

	case 0x4504:
		{
			g_NESmapper.set_CPU_bank4(data);
		}
		break;

	case 0x4505:
		{
			g_NESmapper.set_CPU_bank5(data);
		}
		break;

	case 0x4506:
		{
			g_NESmapper.set_CPU_bank6(data);
		}
		break;

	case 0x4507:
		{
			g_NESmapper.set_CPU_bank7(data);
		}
		break;

	case 0x4510:
		{
			g_NESmapper.set_PPU_bank0(data);
		}
		break;

	case 0x4511:
		{
			g_NESmapper.set_PPU_bank1(data);
		}
		break;

	case 0x4512:
		{
			g_NESmapper.set_PPU_bank2(data);
		}
		break;

	case 0x4513:
		{
			g_NESmapper.set_PPU_bank3(data);
		}
		break;

	case 0x4514:
		{
			g_NESmapper.set_PPU_bank4(data);
		}
		break;

	case 0x4515:
		{
			g_NESmapper.set_PPU_bank5(data);
		}
		break;

	case 0x4516:
		{
			g_NESmapper.set_PPU_bank6(data);
		}
		break;

	case 0x4517:
		{
			g_NESmapper.set_PPU_bank7(data);
		}
		break;
	}
}

void NES_mapper17_HSync(uint32 scanline)
{
	if(g_NESmapper.Mapper17.irq_enabled)
	{
		if(g_NESmapper.Mapper17.irq_counter >= 0xFFFF - 113)
		{
			NES6502_DoIRQ();
			g_NESmapper.Mapper17.irq_counter = 0;
			g_NESmapper.Mapper17.irq_enabled = 0;
		}
		else
		{
			g_NESmapper.Mapper17.irq_counter += 113;
		}
	}
}
/////////////////////////////////////////////////////////////////////

#endif
