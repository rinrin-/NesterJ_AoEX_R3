
/////////////////////////////////////////////////////////////////////
// Mapper 9
void NES_mapper9_Init()
{
	g_NESmapper.Reset = NES_mapper9_Reset;
	g_NESmapper.MemoryWrite = NES_mapper9_MemoryWrite;
	g_NESmapper.PPU_Latch_FDFE = NES_mapper9_PPU_Latch_FDFE;
}

void NES_mapper9_Reset()
{
	int i;
	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,g_NESmapper.num_8k_ROM_banks-3,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);

	for(i = 0; i < sizeof(g_NESmapper.Mapper9.regs)/sizeof(g_NESmapper.Mapper9.regs[0]); i++)
		g_NESmapper.Mapper9.regs[i] = 0;

	g_NESmapper.Mapper9.regs[2] = 4;

	g_NESmapper.Mapper9.latch_0000 = 0xFE;
	g_NESmapper.Mapper9.latch_1000 = 0xFE;

	NES_mapper9_set_VROM_0000();
	NES_mapper9_set_VROM_1000();
}

void NES_mapper9_PPU_Latch_FDFE(uint32 addr)
{
	if(addr & 0x1000)
	{
		g_NESmapper.Mapper9.latch_1000 = (addr & 0x0FF0) >> 4;
		NES_mapper9_set_VROM_1000();
	}
	else
	{
		g_NESmapper.Mapper9.latch_0000 = (addr & 0x0FF0) >> 4;
		NES_mapper9_set_VROM_0000();
	}
}

void NES_mapper9_set_VROM_0000()
{
	int bank_num = (g_NESmapper.Mapper9.latch_0000 == 0xFD) ? g_NESmapper.Mapper9.regs[1] : g_NESmapper.Mapper9.regs[2];

	bank_num <<= 2;

	g_NESmapper.set_PPU_bank0(bank_num+0);
	g_NESmapper.set_PPU_bank1(bank_num+1);
	g_NESmapper.set_PPU_bank2(bank_num+2);
	g_NESmapper.set_PPU_bank3(bank_num+3);
}

void NES_mapper9_set_VROM_1000()
{
	int bank_num = (g_NESmapper.Mapper9.latch_1000 == 0xFD) ? g_NESmapper.Mapper9.regs[3] : g_NESmapper.Mapper9.regs[4];

	bank_num <<= 2;

	g_NESmapper.set_PPU_bank4(bank_num+0);
	g_NESmapper.set_PPU_bank5(bank_num+1);
	g_NESmapper.set_PPU_bank6(bank_num+2);
	g_NESmapper.set_PPU_bank7(bank_num+3);
}

void NES_mapper9_MemoryWrite(uint32 addr, uint8 data)
{
	switch(addr & 0xF000)
	{
	case 0xA000:
		{
			uint8 bank_num;
			g_NESmapper.Mapper9.regs[0] = data;

			// 8K ROM bank at $8000
			bank_num = g_NESmapper.Mapper9.regs[0];
			g_NESmapper.set_CPU_bank4(bank_num);
		}
		break;

	case 0xB000:
		{
			// B000-BFFF: select 4k VROM for (0000) $FD latch
			g_NESmapper.Mapper9.regs[1] = data;
			NES_mapper9_set_VROM_0000();
		}
		break;

	case 0xC000:
		{
			// C000-CFFF: select 4k VROM for (0000) $FE latch
			g_NESmapper.Mapper9.regs[2] = data;
			NES_mapper9_set_VROM_0000();
		}
		break;

	case 0xD000:
		{
			// D000-DFFF: select 4k VROM for (1000) $FD latch
			g_NESmapper.Mapper9.regs[3] = data;
			NES_mapper9_set_VROM_1000();
		}
		break;

	case 0xE000:
		{
			// E000-EFFF: select 4k VROM for (1000) $FE latch
			g_NESmapper.Mapper9.regs[4] = data;
			NES_mapper9_set_VROM_1000();
		}
		break;

	case 0xF000:
		{
			g_NESmapper.Mapper9.regs[5] = data;

			if(g_NESmapper.Mapper9.regs[5] & 0x01)
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
			}
			else
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
			}
		}
		break;
	}
}

void NES_mapper9_SNSS_fixup()
{
	NES_mapper9_set_VROM_0000();
	NES_mapper9_set_VROM_1000();
}
/////////////////////////////////////////////////////////////////////

