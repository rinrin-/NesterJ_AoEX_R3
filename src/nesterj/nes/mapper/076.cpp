
/////////////////////////////////////////////////////////////////////
// Mapper 76
void NES_mapper76_Init()
{
	g_NESmapper.Reset = NES_mapper76_Reset;
	g_NESmapper.MemoryWrite = NES_mapper76_MemoryWrite;
}

void NES_mapper76_Reset()
{
	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);

	// set PPU bank pointers
	if(g_NESmapper.num_1k_VROM_banks >= 8)
	{
		g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);
	}
}

void NES_mapper76_MemoryWrite(uint32 addr, uint8 data)
{
	if(addr == 0x8000)
	{
		g_NESmapper.Mapper76.regs[0] = data;
	}
	else if(addr == 0x8001)
	{
		switch(g_NESmapper.Mapper76.regs[0] & 0x07)
		{
		case 0x02:
			{
				g_NESmapper.set_PPU_bank0(data*2+0);
				g_NESmapper.set_PPU_bank1(data*2+1);
			}
			break;

		case 0x03:
			{
				g_NESmapper.set_PPU_bank2(data*2+0);
				g_NESmapper.set_PPU_bank3(data*2+1);
			}
			break;

		case 0x04:
			{
				g_NESmapper.set_PPU_bank4(data*2+0);
				g_NESmapper.set_PPU_bank5(data*2+1);
			}
			break;

		case 0x05:
			{
				g_NESmapper.set_PPU_bank6(data*2+0);
				g_NESmapper.set_PPU_bank7(data*2+1);
			}
			break;

		case 0x06:
			{
				g_NESmapper.set_CPU_bank4(data);
			}
			break;

		case 0x07:
			{
				g_NESmapper.set_CPU_bank5(data);
			}
			break;
		}
	}
}
/////////////////////////////////////////////////////////////////////

