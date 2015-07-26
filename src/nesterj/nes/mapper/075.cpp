
/////////////////////////////////////////////////////////////////////
// Mapper 75
void NES_mapper75_Init()
{
	g_NESmapper.Reset = NES_mapper75_Reset;
	g_NESmapper.MemoryWrite = NES_mapper75_MemoryWrite;
}

void NES_mapper75_Reset()
{
	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);

	// set PPU bank pointers
	if(g_NESmapper.num_1k_VROM_banks)
	{
		g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);
	}

	g_NESmapper.Mapper75.regs[0] = 0;
	g_NESmapper.Mapper75.regs[1] = 1;
}

void NES_mapper75_MemoryWrite(uint32 addr, uint8 data)
{
	switch(addr & 0xF000)
	{
	case 0x8000:
		{
			g_NESmapper.set_CPU_bank4(data);
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
			g_NESmapper.Mapper75.regs[0] = (g_NESmapper.Mapper75.regs[0] & 0x0F) | ((data & 0x02) << 3);
			g_NESmapper.set_PPU_bank0(g_NESmapper.Mapper75.regs[0]*4+0);
			g_NESmapper.set_PPU_bank1(g_NESmapper.Mapper75.regs[0]*4+1);
			g_NESmapper.set_PPU_bank2(g_NESmapper.Mapper75.regs[0]*4+2);
			g_NESmapper.set_PPU_bank3(g_NESmapper.Mapper75.regs[0]*4+3);
			g_NESmapper.Mapper75.regs[1] = (g_NESmapper.Mapper75.regs[1] & 0x0F) | ((data & 0x04) << 2);
			g_NESmapper.set_PPU_bank4(g_NESmapper.Mapper75.regs[1]*4+0);
			g_NESmapper.set_PPU_bank5(g_NESmapper.Mapper75.regs[1]*4+1);
			g_NESmapper.set_PPU_bank6(g_NESmapper.Mapper75.regs[1]*4+2);
			g_NESmapper.set_PPU_bank7(g_NESmapper.Mapper75.regs[1]*4+3);
		}
		break;

	case 0xA000:
		{
			g_NESmapper.set_CPU_bank5(data);
		}
		break;

	case 0xC000:
		{
			g_NESmapper.set_CPU_bank6(data);
		}
		break;

	case 0xE000:
		{
			g_NESmapper.Mapper75.regs[0] = (g_NESmapper.Mapper75.regs[0] & 0x10) | (data & 0x0F);
			g_NESmapper.set_PPU_bank0(g_NESmapper.Mapper75.regs[0]*4+0);
			g_NESmapper.set_PPU_bank1(g_NESmapper.Mapper75.regs[0]*4+1);
			g_NESmapper.set_PPU_bank2(g_NESmapper.Mapper75.regs[0]*4+2);
			g_NESmapper.set_PPU_bank3(g_NESmapper.Mapper75.regs[0]*4+3);
		}
		break;

	case 0xF000:
		{
			g_NESmapper.Mapper75.regs[1] = (g_NESmapper.Mapper75.regs[1] & 0x10) | (data & 0x0F);
			g_NESmapper.set_PPU_bank4(g_NESmapper.Mapper75.regs[1]*4+0);
			g_NESmapper.set_PPU_bank5(g_NESmapper.Mapper75.regs[1]*4+1);
			g_NESmapper.set_PPU_bank6(g_NESmapper.Mapper75.regs[1]*4+2);
			g_NESmapper.set_PPU_bank7(g_NESmapper.Mapper75.regs[1]*4+3);
		}
		break;
	}
}
/////////////////////////////////////////////////////////////////////

