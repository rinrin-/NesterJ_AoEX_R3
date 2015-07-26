
/////////////////////////////////////////////////////////////////////
// Mapper 32
void NES_mapper32_Init()
{
	g_NESmapper.Reset = NES_mapper32_Reset;
	g_NESmapper.MemoryWrite = NES_mapper32_MemoryWrite;
}

void NES_mapper32_Reset()
{
	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);

	// set PPU bank pointers
	if(g_NESmapper.num_1k_VROM_banks)
	{
		g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);
	}

	g_NESmapper.Mapper32.patch = 0;

	if(NES_crc32() == 0xc0fed437) // Major League
	{
		g_NESmapper.set_mirroring(0,0,0,0);
		g_NESmapper.Mapper32.patch = 1;
	}

	if(NES_crc32() == 0xfd3fc292) // Ai Sensei no Oshiete - Watashi no Hoshi
	{
		g_NESmapper.set_CPU_banks4(30,31,30,31);
	}

	g_NESmapper.Mapper32.regs[0] = 0;
}

void NES_mapper32_MemoryWrite(uint32 addr, uint8 data)
{
	switch(addr & 0xF000)
	{
	case 0x8000:
		{
			if(g_NESmapper.Mapper32.regs[0] & 0x02)
			{
				g_NESmapper.set_CPU_bank6(data);
			}
			else
			{
				g_NESmapper.set_CPU_bank4(data);
			}
		}
		break;

	case 0x9000:
		{
			if(data & 0x01)
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
			}
			else
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
			}
			g_NESmapper.Mapper32.regs[0] = data;
		}
		break;

	case 0xA000:
		{
			g_NESmapper.set_CPU_bank5(data);
		}
		break;
	}

	switch(addr & 0xF007)
	{
	case 0xB000:
		{
			g_NESmapper.set_PPU_bank0(data);
		}
		break;

	case 0xB001:
		{
			g_NESmapper.set_PPU_bank1(data);
		}
		break;

	case 0xB002:
		{
			g_NESmapper.set_PPU_bank2(data);
		}
		break;

	case 0xB003:
		{
			g_NESmapper.set_PPU_bank3(data);
		}
		break;

	case 0xB004:
		{
			g_NESmapper.set_PPU_bank4(data);
		}
		break;

	case 0xB005:
		{
			g_NESmapper.set_PPU_bank5(data);
		}
		break;

	case 0xB006:
		{
			if ((g_NESmapper.Mapper32.patch == 1) && (data & 0x40))
			{
				g_NESmapper.set_mirroring(0,0,0,1);
			}
			g_NESmapper.set_PPU_bank6(data);
		}
		break;

	case 0xB007:
		{
			if ((g_NESmapper.Mapper32.patch == 1) && (data & 0x40))
			{
				g_NESmapper.set_mirroring(0,0,0,0);
			}
			g_NESmapper.set_PPU_bank7(data);
		}
		break;
	}
}
/////////////////////////////////////////////////////////////////////

