#ifdef _NES_MAPPER_CPP_

/////////////////////////////////////////////////////////////////////
// Mapper 82
void NES_mapper82_Init()
{
	g_NESmapper.Reset = NES_mapper82_Reset;
	g_NESmapper.MemoryWriteSaveRAM = NES_mapper82_MemoryWriteSaveRAM;
}

void NES_mapper82_Reset()
{
	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);

	// set PPU bank pointers
	if(g_NESmapper.num_1k_VROM_banks)
	{
		g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);
	}

	// set Mirroring
	g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);

	g_NESmapper.Mapper82.regs[0] = 0;
}

void NES_mapper82_MemoryWriteSaveRAM(uint32 addr, uint8 data)
{
	switch (addr)
	{
	case 0x7EF0:
		/* Switch 2k VROM at $0000 or $1000 */
		{
			if(g_NESmapper.Mapper82.regs[0])
			{
				g_NESmapper.set_PPU_bank4((data & 0xFE)+0);
				g_NESmapper.set_PPU_bank5((data & 0xFE)+1);
			}
			else
			{
				g_NESmapper.set_PPU_bank0((data & 0xFE)+0);
				g_NESmapper.set_PPU_bank1((data & 0xFE)+1);
			}
		}
		break;

	case 0x7EF1:
		{
			if(g_NESmapper.Mapper82.regs[0])
			{
				g_NESmapper.set_PPU_bank6((data & 0xFE)+0);
				g_NESmapper.set_PPU_bank7((data & 0xFE)+1);
			}
			else
			{
				g_NESmapper.set_PPU_bank2((data & 0xFE)+0);
				g_NESmapper.set_PPU_bank3((data & 0xFE)+1);
			}
		}
		break;

	case 0x7EF2:
		{
			if(!g_NESmapper.Mapper82.regs[0])
			{
				g_NESmapper.set_PPU_bank4(data);
			}
			else
			{
				g_NESmapper.set_PPU_bank0(data);
			}
		}
		break;

	case 0x7EF3:
		{
			if(!g_NESmapper.Mapper82.regs[0])
			{
				g_NESmapper.set_PPU_bank5(data);
			}
			else
			{
				g_NESmapper.set_PPU_bank1(data);
			}
		}
		break;

	case 0x7EF4:
		{
			if(!g_NESmapper.Mapper82.regs[0])
			{
				g_NESmapper.set_PPU_bank6(data);
			}
			else
			{
				g_NESmapper.set_PPU_bank2(data);
			}
		}
		break;

	case 0x7EF5:
		{
			if(!g_NESmapper.Mapper82.regs[0])
			{
				g_NESmapper.set_PPU_bank7(data);
			}
			else
			{
				g_NESmapper.set_PPU_bank3(data);
			}
		}
		break;

	case 0x7EF6:
		{
			g_NESmapper.Mapper82.regs[0] = data & 0x02;
			if(data & 0x01)
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
			}
			else
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
			}
		}
		break;

	case 0x7EFA:
		{
			g_NESmapper.set_CPU_bank4(data >> 2);
		}
		break;

	case 0x7EFB:
		{
			g_NESmapper.set_CPU_bank5(data >> 2);
		}
		break;

	case 0x7EFC:
		{
			g_NESmapper.set_CPU_bank6(data >> 2);
		}
		break;
	}
}
/////////////////////////////////////////////////////////////////////

#endif
