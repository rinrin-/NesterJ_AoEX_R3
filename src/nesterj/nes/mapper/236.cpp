
/////////////////////////////////////////////////////////////////////
// Mapper 236
void NES_mapper236_Init()
{
	g_NESmapper.Reset = NES_mapper236_Reset;
	g_NESmapper.MemoryWrite = NES_mapper236_MemoryWrite;
}

void NES_mapper236_Reset()
{
	g_NESmapper.set_CPU_banks4(0,1,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);

	g_NESmapper.Mapper236.bank = g_NESmapper.Mapper236.mode = 0;
}

void NES_mapper236_MemoryWrite(uint32 addr, uint8 data)
{
	if(0x8000 <= addr && addr <= 0xbfff)
	{
		g_NESmapper.Mapper236.bank = ((addr & 0x03) << 4) | (g_NESmapper.Mapper236.bank & 0x07);
	}
	else
	{
		g_NESmapper.Mapper236.bank = (addr & 0x07) | (g_NESmapper.Mapper236.bank & 0x30);
		g_NESmapper.Mapper236.mode = addr & 0x30;
	}
	if(addr & 0x20)
	{
		g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
	}
	else
	{
		g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
	}
	switch(g_NESmapper.Mapper236.mode)
	{
	case 0x00:
		{
			g_NESmapper.Mapper236.bank |= 0x08;
			g_NESmapper.set_CPU_bank4(g_NESmapper.Mapper236.bank*2+0);
			g_NESmapper.set_CPU_bank5(g_NESmapper.Mapper236.bank*2+1);
			g_NESmapper.set_CPU_bank6((g_NESmapper.Mapper236.bank|0x07)*2+0);
			g_NESmapper.set_CPU_bank7((g_NESmapper.Mapper236.bank|0x07)*2+1);
		}
		break;

	case 0x10:
		{
			g_NESmapper.Mapper236.bank &= 0x37;
			g_NESmapper.set_CPU_bank4(g_NESmapper.Mapper236.bank*2+0);
			g_NESmapper.set_CPU_bank5(g_NESmapper.Mapper236.bank*2+1);
			g_NESmapper.set_CPU_bank6((g_NESmapper.Mapper236.bank|0x07)*2+0);
			g_NESmapper.set_CPU_bank7((g_NESmapper.Mapper236.bank|0x07)*2+1);
		}
		break;

	case 0x20:
		{
			g_NESmapper.Mapper236.bank |= 0x08;
			g_NESmapper.set_CPU_bank4((g_NESmapper.Mapper236.bank&0xfe)*2+0);
			g_NESmapper.set_CPU_bank5((g_NESmapper.Mapper236.bank&0xfe)*2+1);
			g_NESmapper.set_CPU_bank6((g_NESmapper.Mapper236.bank&0xfe)*2+2);
			g_NESmapper.set_CPU_bank7((g_NESmapper.Mapper236.bank&0xfe)*2+3);
		}
		break;

	case 0x30:
		{
			g_NESmapper.Mapper236.bank |= 0x08;
			g_NESmapper.set_CPU_bank4(g_NESmapper.Mapper236.bank*2+0);
			g_NESmapper.set_CPU_bank5(g_NESmapper.Mapper236.bank*2+1);
			g_NESmapper.set_CPU_bank6(g_NESmapper.Mapper236.bank*2+0);
			g_NESmapper.set_CPU_bank7(g_NESmapper.Mapper236.bank*2+1);
		}
		break;
	}
}
/////////////////////////////////////////////////////////////////////

