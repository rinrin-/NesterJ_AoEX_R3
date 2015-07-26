
/////////////////////////////////////////////////////////////////////
// Mapper 41
void NES_mapper41_Init()
{
	g_NESmapper.Reset = NES_mapper41_Reset;
	g_NESmapper.MemoryWriteSaveRAM = NES_mapper41_MemoryWriteSaveRAM;
	g_NESmapper.MemoryWrite = NES_mapper41_MemoryWrite;
}

void NES_mapper41_Reset()
{
	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,2,3);

	// set PPU bank pointers
	if(g_NESmapper.num_1k_VROM_banks)
	{
		g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);
	}
}

void NES_mapper41_MemoryWriteSaveRAM(uint32 addr, uint8 data)
{
	if(addr < 0x6800)
	{
		g_NESmapper.Mapper41.regs[0] = (uint8)(addr & 0xFF);

		g_NESmapper.set_CPU_bank4((g_NESmapper.Mapper41.regs[0] & 0x07)*4+0);
		g_NESmapper.set_CPU_bank5((g_NESmapper.Mapper41.regs[0] & 0x07)*4+1);
		g_NESmapper.set_CPU_bank6((g_NESmapper.Mapper41.regs[0] & 0x07)*4+2);
		g_NESmapper.set_CPU_bank7((g_NESmapper.Mapper41.regs[0] & 0x07)*4+3);
		if(g_NESmapper.Mapper41.regs[0] & 0x20)
		{
			g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
		}
		else
		{
			g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
		}
	}
}

void NES_mapper41_MemoryWrite(uint32 addr, uint8 data)
{
	if(g_NESmapper.Mapper41.regs[0] & 0x04)
	{
		uint8 chr_bank = ((g_NESmapper.Mapper41.regs[0] & 0x18) >> 1) | (data & 0x03);

		g_NESmapper.set_PPU_bank0(chr_bank*8+0);
		g_NESmapper.set_PPU_bank1(chr_bank*8+1);
		g_NESmapper.set_PPU_bank2(chr_bank*8+2);
		g_NESmapper.set_PPU_bank3(chr_bank*8+3);
		g_NESmapper.set_PPU_bank4(chr_bank*8+4);
		g_NESmapper.set_PPU_bank5(chr_bank*8+5);
		g_NESmapper.set_PPU_bank6(chr_bank*8+6);
		g_NESmapper.set_PPU_bank7(chr_bank*8+7);
	}
}
/////////////////////////////////////////////////////////////////////

