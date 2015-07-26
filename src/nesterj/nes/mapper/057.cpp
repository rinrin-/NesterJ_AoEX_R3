/////////////////////////////////////////////////////////////////////
// Mapper 57
void NES_mapper57_Init()
{
	g_NESmapper.Reset = NES_mapper57_Reset;
	g_NESmapper.MemoryWrite = NES_mapper57_MemoryWrite;
}

void NES_mapper57_Reset()
{
	g_NESmapper.set_CPU_banks4(0,1,0,1);
	g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);

	g_NESmapper.Mapper57.regs[0] = 0;
}

void NES_mapper57_MemoryWrite(uint32 addr, uint8 data)
{
	switch(addr)
	{
	case 0x8000:
	case 0x8001:
	case 0x8002:
	case 0x8003:
		{
			if(data & 0x40)
			{
				g_NESmapper.set_PPU_bank0(8 * ((data & 0x03) + ((g_NESmapper.Mapper57.regs[0] & 0x10) >> 1) + (g_NESmapper.Mapper57.regs[0] & 0x07)) + 0);
				g_NESmapper.set_PPU_bank1(8 * ((data & 0x03) + ((g_NESmapper.Mapper57.regs[0] & 0x10) >> 1) + (g_NESmapper.Mapper57.regs[0] & 0x07)) + 1);
				g_NESmapper.set_PPU_bank2(8 * ((data & 0x03) + ((g_NESmapper.Mapper57.regs[0] & 0x10) >> 1) + (g_NESmapper.Mapper57.regs[0] & 0x07)) + 2);
				g_NESmapper.set_PPU_bank3(8 * ((data & 0x03) + ((g_NESmapper.Mapper57.regs[0] & 0x10) >> 1) + (g_NESmapper.Mapper57.regs[0] & 0x07)) + 3);
				g_NESmapper.set_PPU_bank4(8 * ((data & 0x03) + ((g_NESmapper.Mapper57.regs[0] & 0x10) >> 1) + (g_NESmapper.Mapper57.regs[0] & 0x07)) + 4);
				g_NESmapper.set_PPU_bank5(8 * ((data & 0x03) + ((g_NESmapper.Mapper57.regs[0] & 0x10) >> 1) + (g_NESmapper.Mapper57.regs[0] & 0x07)) + 5);
				g_NESmapper.set_PPU_bank6(8 * ((data & 0x03) + ((g_NESmapper.Mapper57.regs[0] & 0x10) >> 1) + (g_NESmapper.Mapper57.regs[0] & 0x07)) + 6);
				g_NESmapper.set_PPU_bank7(8 * ((data & 0x03) + ((g_NESmapper.Mapper57.regs[0] & 0x10) >> 1) + (g_NESmapper.Mapper57.regs[0] & 0x07)) + 7);
			}
		}
		break;

	case 0x8800:
		{
			g_NESmapper.Mapper57.regs[0] = data;

			if(data & 0x80)
			{
				g_NESmapper.set_CPU_bank4(4 * ((data & 0x40) >> 6) + 8 + 0);
				g_NESmapper.set_CPU_bank5(4 * ((data & 0x40) >> 6) + 8 + 1);
				g_NESmapper.set_CPU_bank6(4 * ((data & 0x40) >> 6) + 8 + 2);
				g_NESmapper.set_CPU_bank7(4 * ((data & 0x40) >> 6) + 8 + 3);
			}
			else
			{
				g_NESmapper.set_CPU_bank4(2 * ((data & 0x60) >> 5) + 0);
				g_NESmapper.set_CPU_bank5(2 * ((data & 0x60) >> 5) + 1);
				g_NESmapper.set_CPU_bank6(2 * ((data & 0x60) >> 5) + 0);
				g_NESmapper.set_CPU_bank7(2 * ((data & 0x60) >> 5) + 1);
			}

			g_NESmapper.set_PPU_bank0(8 * ((data & 0x07) + ((data & 0x10) >> 1)) + 0);
			g_NESmapper.set_PPU_bank1(8 * ((data & 0x07) + ((data & 0x10) >> 1)) + 1);
			g_NESmapper.set_PPU_bank2(8 * ((data & 0x07) + ((data & 0x10) >> 1)) + 2);
			g_NESmapper.set_PPU_bank3(8 * ((data & 0x07) + ((data & 0x10) >> 1)) + 3);
			g_NESmapper.set_PPU_bank4(8 * ((data & 0x07) + ((data & 0x10) >> 1)) + 4);
			g_NESmapper.set_PPU_bank5(8 * ((data & 0x07) + ((data & 0x10) >> 1)) + 5);
			g_NESmapper.set_PPU_bank6(8 * ((data & 0x07) + ((data & 0x10) >> 1)) + 6);
			g_NESmapper.set_PPU_bank7(8 * ((data & 0x07) + ((data & 0x10) >> 1)) + 7);

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
	}
}
/////////////////////////////////////////////////////////////////////
