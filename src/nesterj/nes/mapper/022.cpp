
/////////////////////////////////////////////////////////////////////
// Mapper 22
void NES_mapper22_Init()
{
	g_NESmapper.Reset = NES_mapper22_Reset;
	g_NESmapper.MemoryWrite = NES_mapper22_MemoryWrite;
}

void NES_mapper22_Reset()
{
	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);
	g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);
}

void NES_mapper22_MemoryWrite(uint32 addr, uint8 data)
{
	switch(addr)
	{
	case 0x8000:
		{
			g_NESmapper.set_CPU_bank4(data);
		}
		break;

	case 0x9000:
		{
			data &= 0x03;
			if(data == 0)
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
			}
			else if(data == 1)
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
			}
			else if(data == 2)
			{
				g_NESmapper.set_mirroring(1,1,1,1);
			}
			else
			{
				g_NESmapper.set_mirroring(0,0,0,0);
			}
		}
		break;

	case 0xA000:
		{
			g_NESmapper.set_CPU_bank5(data);
		}
		break;

	case 0xB000:
		{
			g_NESmapper.set_PPU_bank0(data >> 1);
		}
		break;

	case 0xB001:
		{
			g_NESmapper.set_PPU_bank1(data >> 1);
		}
		break;

	case 0xC000:
		{
			g_NESmapper.set_PPU_bank2(data >> 1);
		}
		break;

	case 0xC001:
		{
			g_NESmapper.set_PPU_bank3(data >> 1);
		}
		break;

	case 0xD000:
		{
			g_NESmapper.set_PPU_bank4(data >> 1);
		}
		break;

	case 0xD001:
		{
			g_NESmapper.set_PPU_bank5(data >> 1);
		}
		break;

	case 0xE000:
		{
			g_NESmapper.set_PPU_bank6(data >> 1);
		}
		break;

	case 0xE001:
		{
			g_NESmapper.set_PPU_bank7(data >> 1);
		}
		break;
	}
}
/////////////////////////////////////////////////////////////////////

