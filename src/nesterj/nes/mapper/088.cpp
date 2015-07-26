
/////////////////////////////////////////////////////////////////////
// Mapper 88
void NES_mapper88_Init()
{
	g_NESmapper.Reset = NES_mapper88_Reset;
	g_NESmapper.MemoryWrite = NES_mapper88_MemoryWrite;
}

void NES_mapper88_Reset()
{
	g_NESmapper.set_CPU_banks4(0,1,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);

	if(g_NESmapper.num_1k_VROM_banks >= 8)
	{
		g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);
	}
}

void NES_mapper88_MemoryWrite(uint32 addr, uint8 data)
{
	switch(addr)
	{
	case 0x8000:
		{
			g_NESmapper.Mapper88.regs[0] = data;
		}
		break;

	case 0x8001:
		{
			switch(g_NESmapper.Mapper88.regs[0] & 0x07)
			{
			case 0x00:
				{
					g_NESmapper.set_PPU_bank0(data & 0xFE);
					g_NESmapper.set_PPU_bank1((data & 0xFE)+1);
				}
				break;

			case 0x01:
				{
					g_NESmapper.set_PPU_bank2(data & 0xFE);
					g_NESmapper.set_PPU_bank3((data & 0xFE)+1);
				}
				break;

			case 0x02:
				{
					g_NESmapper.set_PPU_bank4(data|0x40);
				}
				break;

			case 0x03:
				{
					g_NESmapper.set_PPU_bank5(data|0x40);
				}
				break;

			case 0x04:
				{
					g_NESmapper.set_PPU_bank6(data|0x40);
				}
				break;

			case 0x05:
				{
					g_NESmapper.set_PPU_bank7(data|0x40);
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
		break;

	case 0xC000:
		{
			if(data&0x40)
			{
				g_NESmapper.set_mirroring(1,1,1,1);
			}
			else
			{
				g_NESmapper.set_mirroring(0,0,0,0);
			}
		}
		break;
	}
}
/////////////////////////////////////////////////////////////////////

