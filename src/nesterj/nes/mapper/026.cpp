
/////////////////////////////////////////////////////////////////////
// Mapper 26
void NES_mapper26_Init()
{
	g_NESmapper.Reset = NES_mapper26_Reset;
	g_NESmapper.MemoryWrite = NES_mapper26_MemoryWrite;
	g_NESmapper.HSync = NES_mapper26_HSync;
}

void NES_mapper26_Reset()
{
	// Init ExSound
	NES_APU_SelectExSound(1);


	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);

	// set PPU bank pointers
	if(g_NESmapper.num_1k_VROM_banks)
	{
		g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);
	}

	g_NESmapper.Mapper26.irq_enabled = 0;
	g_NESmapper.Mapper26.irq_counter = 0;
}

void NES_mapper26_MemoryWrite(uint32 addr, uint8 data)
{
	switch(addr)
	{
	case 0x8000:
		{
			g_NESmapper.set_CPU_bank4(data*2+0);
			g_NESmapper.set_CPU_bank5(data*2+1);
		}
		break;

	case 0xB003:
		{
			data = data & 0x7F;
			if(data == 0x08 || data == 0x2C)
			{
				g_NESmapper.set_mirroring(1,1,1,1);
			}
			else if(data == 0x20)
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
			}
			else if(data == 0x24)
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
			}
			else if(data == 0x28)
			{
				g_NESmapper.set_mirroring(0,0,0,0);
			}
		}
		break;

	case 0xC000:
		{
			g_NESmapper.set_CPU_bank6(data);
		}
		break;

	case 0xD000:
		{
			g_NESmapper.set_PPU_bank0(data);
		}
		break;

	case 0xD001:
		{
			g_NESmapper.set_PPU_bank2(data);
		}
		break;

	case 0xD002:
		{
			g_NESmapper.set_PPU_bank1(data);
		}
		break;

	case 0xD003:
		{
			g_NESmapper.set_PPU_bank3(data);
		}
		break;

	case 0xE000:
		{
			g_NESmapper.set_PPU_bank4(data);
		}
		break;

	case 0xE001:
		{
			g_NESmapper.set_PPU_bank6(data);
		}
		break;

	case 0xE002:
		{
			g_NESmapper.set_PPU_bank5(data);
		}
		break;

	case 0xE003:
		{
			g_NESmapper.set_PPU_bank7(data);
		}
		break;

	case 0xF000:
		{
			g_NESmapper.Mapper26.irq_latch = data;
		}
		break;

	case 0xF001:
		{
			g_NESmapper.Mapper26.irq_enabled = data & 0x01;
		}
		break;

	case 0xF002:
		{
			g_NESmapper.Mapper26.irq_enabled = data & 0x03;
			if(g_NESmapper.Mapper26.irq_enabled & 0x02)
			{
				g_NESmapper.Mapper26.irq_counter = g_NESmapper.Mapper26.irq_latch;
			}
		}
		break;
	}

	addr = (addr & 0xfffc) | ((addr & 1) << 1) | ((addr & 2) >> 1);
	NES_APU_ExWrite(addr, data);
}

void NES_mapper26_HSync(uint32 scanline)
{
	if(g_NESmapper.Mapper26.irq_enabled & 0x03)
	{
		if(g_NESmapper.Mapper26.irq_counter >= 0xFE)
		{
			NES6502_DoIRQ();
			g_NESmapper.Mapper26.irq_counter = g_NESmapper.Mapper26.irq_latch;
			g_NESmapper.Mapper26.irq_enabled = 0;
		}
		else
		{
			g_NESmapper.Mapper26.irq_counter++;
		}
	}
}
/////////////////////////////////////////////////////////////////////

