/////////////////////////////////////////////////////////////////////
// Mapper 255
void NES_mapper255_Init()
{
	g_NESmapper.Reset = NES_mapper255_Reset;
	g_NESmapper.MemoryWrite = NES_mapper255_MemoryWrite;
	g_NESmapper.MemoryReadLow = NES_mapper255_MemoryReadLow;
	g_NESmapper.MemoryWriteLow = NES_mapper255_MemoryWriteLow;
}

void NES_mapper255_Reset()
{
	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,2,3);

	// set PPU bank pointers
	g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);

	g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);

	//g_NESmapper.Mapper255.regs[0] = g_NESmapper.Mapper255.regs[2] = 0x0F;
	//g_NESmapper.Mapper255.regs[1] = g_NESmapper.Mapper255.regs[3] = 0x00;
}

uint8 NES_mapper255_MemoryReadLow(uint32 addr)
{
	if(addr >= 0x5800)
	{
		return g_NESmapper.Mapper255.regs[addr & 0x0003] & 0x0F;
	}
	else
	{
		return (uint8)(addr >> 8);
	}
}

void NES_mapper255_MemoryWriteLow(uint32 addr, uint8 data)
{
	if(addr >= 0x5800)
	{
		g_NESmapper.Mapper255.regs[addr & 0x0003] = data & 0x0F;
	}
}

void NES_mapper255_MemoryWrite(uint32 addr, uint8 data)
{
	uint8 prg_bank = (uint8)((addr & 0x0F80) >> 7);
	uint8 chr_bank = (uint8)(addr & 0x003F);
	uint8 rom_bank = (uint8)((addr & 0x4000) >> 14);

	if(addr & 0x2000)
	{
		g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
	}
	else
	{
		g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
	}

	if(addr & 0x1000)
	{//16K PRG_ROM
		if(addr & 0x0040)
		{//upper half
			g_NESmapper.set_CPU_bank4(0x80*rom_bank+prg_bank*4+2);
			g_NESmapper.set_CPU_bank5(0x80*rom_bank+prg_bank*4+3);
			g_NESmapper.set_CPU_bank6(0x80*rom_bank+prg_bank*4+2);
			g_NESmapper.set_CPU_bank7(0x80*rom_bank+prg_bank*4+3);
		}
		else
		{//lower half
			g_NESmapper.set_CPU_bank4(0x80*rom_bank+prg_bank*4+0);
			g_NESmapper.set_CPU_bank5(0x80*rom_bank+prg_bank*4+1);
			g_NESmapper.set_CPU_bank6(0x80*rom_bank+prg_bank*4+0);
			g_NESmapper.set_CPU_bank7(0x80*rom_bank+prg_bank*4+1);
		}
	}
	else
	{//32K PRG_ROM
		g_NESmapper.set_CPU_bank4(0x80*rom_bank+prg_bank*4+0);
		g_NESmapper.set_CPU_bank5(0x80*rom_bank+prg_bank*4+1);
		g_NESmapper.set_CPU_bank6(0x80*rom_bank+prg_bank*4+2);
		g_NESmapper.set_CPU_bank7(0x80*rom_bank+prg_bank*4+3);
	}

	g_NESmapper.set_PPU_bank0(0x200*rom_bank+chr_bank*8+0);
	g_NESmapper.set_PPU_bank1(0x200*rom_bank+chr_bank*8+1);
	g_NESmapper.set_PPU_bank2(0x200*rom_bank+chr_bank*8+2);
	g_NESmapper.set_PPU_bank3(0x200*rom_bank+chr_bank*8+3);
	g_NESmapper.set_PPU_bank4(0x200*rom_bank+chr_bank*8+4);
	g_NESmapper.set_PPU_bank5(0x200*rom_bank+chr_bank*8+5);
	g_NESmapper.set_PPU_bank6(0x200*rom_bank+chr_bank*8+6);
	g_NESmapper.set_PPU_bank7(0x200*rom_bank+chr_bank*8+7);
}
/////////////////////////////////////////////////////////////////////

