
/////////////////////////////////////////////////////////////////////
// Mapper 51
void NES_mapper51_Init()
{
	g_NESmapper.Reset = NES_mapper51_Reset;
	g_NESmapper.MemoryWriteSaveRAM = NES_mapper51_MemoryWriteSaveRAM;
	g_NESmapper.MemoryWrite = NES_mapper51_MemoryWrite;
}

void NES_mapper51_Reset()
{
	g_NESmapper.Mapper51.bank = 0;
	g_NESmapper.Mapper51.mode = 1;
	NES_mapper51_Sync_Prg_Banks();
	g_PPU.vram_write_protect = 0;
}

void NES_mapper51_MemoryWriteSaveRAM(uint32 addr, uint8 data)
{
	g_NESmapper.Mapper51.mode = ((data & 0x10) >> 3) | ((data & 0x02) >> 1);
	NES_mapper51_Sync_Prg_Banks();
}

void NES_mapper51_MemoryWrite(uint32 addr, uint8 data)
{
	g_NESmapper.Mapper51.bank = (data & 0x0f) << 2;
	if(0xC000 <= addr && addr <= 0xDFFF)
	{
		g_NESmapper.Mapper51.mode = (g_NESmapper.Mapper51.mode & 0x01) | ((data & 0x10) >> 3);
	}
	NES_mapper51_Sync_Prg_Banks();
}

void NES_mapper51_Sync_Prg_Banks()
{
	switch(g_NESmapper.Mapper51.mode)
	{
	case 0:
		{
			g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
			g_NESmapper.set_CPU_bank3(g_NESmapper.Mapper51.bank | 0x2c | 3);
			g_NESmapper.set_CPU_bank4(g_NESmapper.Mapper51.bank | 0x00 | 0);
			g_NESmapper.set_CPU_bank5(g_NESmapper.Mapper51.bank | 0x00 | 1);
			g_NESmapper.set_CPU_bank6(g_NESmapper.Mapper51.bank | 0x0c | 2);
			g_NESmapper.set_CPU_bank7(g_NESmapper.Mapper51.bank | 0x0c | 3);
		}
		break;

	case 1:
		{
			g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
			g_NESmapper.set_CPU_bank3(g_NESmapper.Mapper51.bank | 0x20 | 3);
			g_NESmapper.set_CPU_bank4(g_NESmapper.Mapper51.bank | 0x00 | 0);
			g_NESmapper.set_CPU_bank5(g_NESmapper.Mapper51.bank | 0x00 | 1);
			g_NESmapper.set_CPU_bank6(g_NESmapper.Mapper51.bank | 0x00 | 2);
			g_NESmapper.set_CPU_bank7(g_NESmapper.Mapper51.bank | 0x00 | 3);
		}
		break;

	case 2:
		{
			g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
			g_NESmapper.set_CPU_bank3(g_NESmapper.Mapper51.bank | 0x2e | 3);
			g_NESmapper.set_CPU_bank4(g_NESmapper.Mapper51.bank | 0x02 | 0);
			g_NESmapper.set_CPU_bank5(g_NESmapper.Mapper51.bank | 0x02 | 1);
			g_NESmapper.set_CPU_bank6(g_NESmapper.Mapper51.bank | 0x0e | 2);
			g_NESmapper.set_CPU_bank7(g_NESmapper.Mapper51.bank | 0x0e | 3);
		}
		break;

	case 3:
		{
			g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
			g_NESmapper.set_CPU_bank3(g_NESmapper.Mapper51.bank | 0x20 | 3);
			g_NESmapper.set_CPU_bank4(g_NESmapper.Mapper51.bank | 0x00 | 0);
			g_NESmapper.set_CPU_bank5(g_NESmapper.Mapper51.bank | 0x00 | 1);
			g_NESmapper.set_CPU_bank6(g_NESmapper.Mapper51.bank | 0x00 | 2);
			g_NESmapper.set_CPU_bank7(g_NESmapper.Mapper51.bank | 0x00 | 3);
		}
		break;
	}
}
/////////////////////////////////////////////////////////////////////

