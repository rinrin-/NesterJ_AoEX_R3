
/////////////////////////////////////////////////////////////////////
// Mapper 65
void NES_mapper65_Init()
{
	g_NESmapper.Reset = NES_mapper65_Reset;
	g_NESmapper.MemoryWrite = NES_mapper65_MemoryWrite;
	g_NESmapper.HSync = NES_mapper65_HSync;
}

void NES_mapper65_Reset()
{
	g_NESmapper.Mapper65.patch = g_NESmapper.Mapper65.patch2 = 0;

	// for Kaiketsu Yanchamaru 3
	if(	(NES_crc32() == 0X5F17F55E)||//Kaiketsu Yanchamaru 3 - Taiketsu! ZouringenÖÐÎÄ°æ
		(NES_crc32() == 0xe30b7f64) )// Kaiketsu Yanchamaru 3 - Taiketsu! Zouringen
	{
		g_NESmapper.Mapper65.patch = 1;
	}
	if(NES_crc32() == 0xd202612b) // X 2
	{
		g_NESmapper.Mapper65.patch2 = 1;
	}

	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);

	// set PPU bank pointers
	if(g_NESmapper.num_1k_VROM_banks)
	{
		g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);
	}

	g_NESmapper.Mapper65.irq_enabled = 0;
	g_NESmapper.Mapper65.irq_counter = 0;
}

void NES_mapper65_MemoryWrite(uint32 addr, uint8 data)
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
			if(!g_NESmapper.Mapper65.patch)
			{
				if(data & 0x40)
				{
					g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
				}
				else
				{
					g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
				}
			}
		}
		break;

	case 0x9001:
		{
			if(g_NESmapper.Mapper65.patch)
			{
				if(data & 0x80)
				{
					g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
				}
				else
				{
					g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
				}
			}
		}
		break;

	case 0x9003:
		{
			if(!g_NESmapper.Mapper65.patch)
			{
				g_NESmapper.Mapper65.irq_enabled = data & 0x80;
			}
		}
		break;

	case 0x9004:
		{
			if(!g_NESmapper.Mapper65.patch)
			{
				g_NESmapper.Mapper65.irq_counter = g_NESmapper.Mapper65.irq_latch;
			}
		}
		break;

	case 0x9005:
		{
			if(g_NESmapper.Mapper65.patch)
			{
				g_NESmapper.Mapper65.irq_counter = (uint8)(data << 1);
				g_NESmapper.Mapper65.irq_enabled = data;
			}
			else
			{
				g_NESmapper.Mapper65.irq_latch = (g_NESmapper.Mapper65.irq_latch & 0x00FF) | ((uint32)data << 8);
			}
		}
		break;

	case 0x9006:
		{
			if(g_NESmapper.Mapper65.patch)
			{
				g_NESmapper.Mapper65.irq_enabled = 1;
			}
			else
			{
				g_NESmapper.Mapper65.irq_latch = (g_NESmapper.Mapper65.irq_latch & 0xFF00) | data;
			}
		}
		break;

	case 0xB000:
		{
			g_NESmapper.set_PPU_bank0(data);
		}
		break;

	case 0xB001:
		{
			g_NESmapper.set_PPU_bank1(data);
		}
		break;

	case 0xB002:
		{
			g_NESmapper.set_PPU_bank2(data);
		}
		break;

	case 0xB003:
		{
			g_NESmapper.set_PPU_bank3(data);
		}
		break;

	case 0xB004:
		{
			g_NESmapper.set_PPU_bank4(data);
		}
		break;

	case 0xB005:
		{
			g_NESmapper.set_PPU_bank5(data);
		}
		break;

	case 0xB006:
		{
			g_NESmapper.set_PPU_bank6(data);
		}
		break;

	case 0xB007:
		{
			g_NESmapper.set_PPU_bank7(data);
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
	}
}

void NES_mapper65_HSync(uint32 scanline)
{
	if(g_NESmapper.Mapper65.irq_enabled)
	{
		if(g_NESmapper.Mapper65.patch)
		{
			if(g_NESmapper.Mapper65.irq_counter == 0)
			{
				NES6502_DoIRQ();
				g_NESmapper.Mapper65.irq_enabled = 0;
			}
			else
			{
				g_NESmapper.Mapper65.irq_counter--;
			}
		}
		else
		{
			if(g_NESmapper.Mapper65.irq_counter <= (unsigned int)((g_NESmapper.Mapper65.patch2)?111:113))
			{
				NES6502_DoIRQ();
				g_NESmapper.Mapper65.irq_enabled = 0;
				g_NESmapper.Mapper65.irq_counter = 0xFFFF;
			}
			else
			{
				g_NESmapper.Mapper65.irq_counter -= ((g_NESmapper.Mapper65.patch2)?111:113);
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////

