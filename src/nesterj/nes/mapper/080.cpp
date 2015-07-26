
/////////////////////////////////////////////////////////////////////
// Mapper 80
void NES_mapper80_Init()
{
	g_NESmapper.Reset = NES_mapper80_Reset;
	g_NESmapper.MemoryWriteSaveRAM = NES_mapper80_MemoryWriteSaveRAM;
	//g_NESmapper.MemoryWriteLow =  NES_mapper80_MemoryWriteSaveRAM;
}

void NES_mapper80_Reset()
{
	g_NESmapper.Mapper80.patch = 0;

	if(NES_crc32() == 0x9832d15a)   // Fudou Myouou Den (J).nes
	{
		// for Hudoumyouou Den
		g_NESmapper.Mapper80.patch = 1;
	}

	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);

	// set PPU bank pointers
	if(g_NESmapper.num_1k_VROM_banks)
	{
		g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);
	}
}

void NES_mapper80_MemoryWriteSaveRAM(uint32 addr, uint8 data)
{
	switch(addr)
	{
	case 0x7EF0:
		{
			//g_NESmapper.set_PPU_bank0(data & 0x7F);
			//g_NESmapper.set_PPU_bank1((data & 0x7F)+1);
			g_NESmapper.SetVROM_2K_Bank( 0, (data>>1)&0x3F );

			if(NES_ROM_get_num_16k_ROM_banks()==16)
			{
				if (data & 0x80)
				{
					g_NESmapper.set_VRAM_bank(0x8, 1);
					g_NESmapper.set_VRAM_bank(0x9, 1);
				}
				else
				{
					g_NESmapper.set_VRAM_bank(0x8, 0);
					g_NESmapper.set_VRAM_bank(0x9, 0);
				}
			}
		}
		break;

	case 0x7EF1:
		{
			//g_NESmapper.set_PPU_bank2(data & 0x7F);
			//g_NESmapper.set_PPU_bank3((data & 0x7F)+1);
			g_NESmapper.SetVROM_2K_Bank( 2, (data>>1)&0x3F );

			if(g_NESmapper.Mapper80.patch)
			{
				if (data & 0x80)
				{
					g_NESmapper.set_VRAM_bank(0xA, 1);
					g_NESmapper.set_VRAM_bank(0xB, 1);
				}
				else
				{
					g_NESmapper.set_VRAM_bank(0xA, 0);
					g_NESmapper.set_VRAM_bank(0xB, 0);
				}
			}
		}
		break;

	case 0x7EF2:
		{
			g_NESmapper.set_PPU_bank4(data);
		}
		break;

	case 0x7EF3:
		{
			g_NESmapper.set_PPU_bank5(data);
		}
		break;

	case 0x7EF4:
		{
			g_NESmapper.set_PPU_bank6(data);
		}
		break;

	case 0x7EF5:
		{
			g_NESmapper.set_PPU_bank7(data);
		}
		break;

	case 0x7EF6:
		{
			if(data & 0x01)
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
			}
			else
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
			}
		}
		break;

	case 0x7EFA:
	case 0x7EFB:
		{
			g_NESmapper.set_CPU_bank4(data);
		}
		break;

	case 0x7EFC:
	case 0x7EFD:
		{
			g_NESmapper.set_CPU_bank5(data);
		}
		break;

	case 0x7EFE:
	case 0x7EFF:
		{
			g_NESmapper.set_CPU_bank6(data);
		}
		break;
	}
}
/////////////////////////////////////////////////////////////////////

