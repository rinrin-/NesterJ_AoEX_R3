
/////////////////////////////////////////////////////////////////////
// Mapper 33
void NES_mapper33_Init()
{
	g_NESmapper.Reset = NES_mapper33_Reset;
	g_NESmapper.MemoryWrite = NES_mapper33_MemoryWrite;
	g_NESmapper.HSync = NES_mapper33_HSync;
}

void NES_mapper33_Reset()
{
	g_NESmapper.Mapper33.patch = g_NESmapper.Mapper33.patch2 = 0;

	if(NES_crc32() == 0x5e9bc161 || // Akira
	        NES_crc32() == 0xecdbafa4 || // Bakushou!! Jinsei Gekijou
	        NES_crc32() == 0x59cd0c31 || // Don Doko Don
	        NES_crc32() == 0x837c1342 || // Golf Ko Open
	        NES_crc32() == 0x42d893e4 || // Operation Wolf
	        NES_crc32() == 0x07ee6d8f || // Power Blazer
	        NES_crc32() == 0x5193fb54 )  // Takeshi no Sengoku Fuuunji
	{
		g_NESmapper.Mapper33.patch = 1;
	}
	if(NES_crc32() == 0x63bb86b5)   // Jetsons-Cogswell'sCaper!,The(J)
	{
		g_NESmapper.Mapper33.patch2=1;
	}

	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);

	// set PPU bank pointers
	if(g_NESmapper.num_1k_VROM_banks)
	{
		g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);
	}

	g_NESmapper.Mapper33.irq_enabled = 0;
	g_NESmapper.Mapper33.irq_counter = 0;
}

void NES_mapper33_MemoryWrite(uint32 addr, uint8 data)
{
	switch(addr)
	{
	case 0x8000:
		{
			if(g_NESmapper.Mapper33.patch) // old #33 games
			{
				if(data & 0x40)
				{
					g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
				}
				else
				{
					g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
				}
				g_NESmapper.set_CPU_bank4(data & 0x1F);
			}
			else
			{
				g_NESmapper.set_CPU_bank4(data);
			}
		}
		break;

	case 0x8001:
		{
			if(g_NESmapper.Mapper33.patch) // old #33 games
			{
				g_NESmapper.set_CPU_bank5(data & 0x1F);
			}
			else
			{
				g_NESmapper.set_CPU_bank5(data);
			}
		}
		break;

	case 0x8002:
		{
			g_NESmapper.set_PPU_bank0(data*2+0);
			g_NESmapper.set_PPU_bank1(data*2+1);
		}
		break;

	case 0x8003:
		{
			g_NESmapper.set_PPU_bank2(data*2+0);
			g_NESmapper.set_PPU_bank3(data*2+1);
		}
		break;

	case 0xA000:
		{
			g_NESmapper.set_PPU_bank4(data);
		}
		break;

	case 0xA001:
		{
			g_NESmapper.set_PPU_bank5(data);
		}
		break;

	case 0xA002:
		{
			g_NESmapper.set_PPU_bank6(data);
		}
		break;

	case 0xA003:
		{
			g_NESmapper.set_PPU_bank7(data);
		}
		break;

	case 0xC000:
		{
			g_NESmapper.Mapper33.irq_counter = data;
		}
		break;

	case 0xC001:
	case 0xC002:
	case 0xE001:
	case 0xE002:
		{
			g_NESmapper.Mapper33.irq_enabled = data;
		}
		break;

	case 0xE000:
		{
			if (data & 0x40)
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
	//  LOG("MAP33: "<< HEX(addr,4) << "  " << HEX(data,2) << endl);
}

void NES_mapper33_HSync(uint32 scanline)
{
	if(g_NESmapper.Mapper33.irq_enabled)
	{
		if(NES_PPU_spr_enabled() || NES_PPU_bg_enabled())
		{
			if (scanline >= 0 && scanline <= 239)
			{
				if(g_NESmapper.Mapper33.irq_counter == 0)
				{
					if(g_NESmapper.Mapper33.patch2)
						NES6502_DoIRQ();
					else
						NES6502_DoPendingIRQ(); //DoIRQ()
					g_NESmapper.Mapper33.irq_counter = 0;
					g_NESmapper.Mapper33.irq_enabled = 0;
				}
				else
				{
					g_NESmapper.Mapper33.irq_counter++;
				}
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////

