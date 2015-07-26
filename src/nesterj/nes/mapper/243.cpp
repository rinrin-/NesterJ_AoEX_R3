#ifdef _NES_MAPPER_CPP_

/////////////////////////////////////////////////////////////////////
// Mapper 243
void NES_mapper243_Init()
{
	g_NESmapper.Reset = NES_mapper243_Reset;
	g_NESmapper.MemoryWriteLow = NES_mapper243_MemoryWriteLow;
	g_NESmapper.MemoryWriteSaveRAM = NES_mapper243_MemoryWriteSaveRAM;
}

void NES_mapper243_Reset()
{
	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,2,3);

	// set PPU bank pointers
	if(g_NESmapper.num_1k_VROM_banks > 32)
	{
		g_NESmapper.set_PPU_banks8(24,25,26,27,28,29,30,31);
	}
	g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);

	g_NESmapper.Mapper243.regs[0] = 0;
	g_NESmapper.Mapper243.regs[1] = 0;
	g_NESmapper.Mapper243.regs[2] = 3;
	g_NESmapper.Mapper243.regs[3] = 0;
}

void NES_mapper243_MemoryWriteLow(uint32 addr, uint8 data)
{
	if((addr & 0x4101) == 0x4100)
	{
		g_NESmapper.Mapper243.regs[0] = data;
	}
	else if((addr & 0x4101) == 0x4101)
	{
		switch(g_NESmapper.Mapper243.regs[0] & 0x07)
		{
		case 0:
			{
				g_NESmapper.Mapper243.regs[1] = 0;
				g_NESmapper.Mapper243.regs[2] = 3;
			}
			break;

		case 4:
			{
				g_NESmapper.Mapper243.regs[2] = (g_NESmapper.Mapper243.regs[2] & 0x06) |  (data & 0x01);
			}
			break;

		case 5:
			{
				g_NESmapper.Mapper243.regs[1] = data & 0x01;
			}
			break;

		case 6:
			{
				g_NESmapper.Mapper243.regs[2] = (g_NESmapper.Mapper243.regs[2] & 0x01) | ((data & 0x03) << 1);
			}
			break;

		case 7:
			{
				g_NESmapper.Mapper243.regs[3] = data & 1;
			}
			break;
		}

		g_NESmapper.set_CPU_banks4(g_NESmapper.Mapper243.regs[1]*4+0, g_NESmapper.Mapper243.regs[1]*4+1, g_NESmapper.Mapper243.regs[1]*4+2, g_NESmapper.Mapper243.regs[1]*4+3);
		g_NESmapper.set_PPU_banks8(g_NESmapper.Mapper243.regs[2]*8+0, g_NESmapper.Mapper243.regs[2]*8+1, g_NESmapper.Mapper243.regs[2]*8+2, g_NESmapper.Mapper243.regs[2]*8+3,
		              g_NESmapper.Mapper243.regs[2]*8+4, g_NESmapper.Mapper243.regs[2]*8+5, g_NESmapper.Mapper243.regs[2]*8+6, g_NESmapper.Mapper243.regs[2]*8+7);
		if(g_NESmapper.Mapper243.regs[3])
		{
			g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
		}
		else
		{
			g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
		}
	}
}

void NES_mapper243_MemoryWriteSaveRAM(uint32 addr, uint8 data)
{
	if((addr & 0x4101) == 0x4100)
	{
		g_NESmapper.Mapper243.regs[0] = data;
	}
	else if((addr & 0x4101) == 0x4101)
	{
		switch(g_NESmapper.Mapper243.regs[0] & 0x07)
		{
		case 0:
			{
				g_NESmapper.Mapper243.regs[1] = 0;
				g_NESmapper.Mapper243.regs[2] = 3;
			}
			break;

		case 4:
			{
				g_NESmapper.Mapper243.regs[2] = (g_NESmapper.Mapper243.regs[2] & 0x06) |  (data & 0x01);
			}
			break;

		case 5:
			{
				g_NESmapper.Mapper243.regs[1] = data & 0x01;
			}
			break;

		case 6:
			{
				g_NESmapper.Mapper243.regs[2] = (g_NESmapper.Mapper243.regs[2] & 0x01) | ((data & 0x03) << 1);
			}
			break;

		case 7:
			{
				g_NESmapper.Mapper243.regs[3] = data & 1;
			}
			break;
		}

		g_NESmapper.set_CPU_banks4(g_NESmapper.Mapper243.regs[1]*4+0, g_NESmapper.Mapper243.regs[1]*4+1, g_NESmapper.Mapper243.regs[1]*4+2, g_NESmapper.Mapper243.regs[1]*4+3);
		g_NESmapper.set_PPU_banks8(g_NESmapper.Mapper243.regs[2]*8+0, g_NESmapper.Mapper243.regs[2]*8+1, g_NESmapper.Mapper243.regs[2]*8+2, g_NESmapper.Mapper243.regs[2]*8+3,
		              g_NESmapper.Mapper243.regs[2]*8+4, g_NESmapper.Mapper243.regs[2]*8+5, g_NESmapper.Mapper243.regs[2]*8+6, g_NESmapper.Mapper243.regs[2]*8+7);
		if(g_NESmapper.Mapper243.regs[3])
		{
			g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
		}
		else
		{
			g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
		}
	}
}
/////////////////////////////////////////////////////////////////////

#endif
