
/////////////////////////////////////////////////////////////////////
// Mapper 24
void NES_mapper24_Init()
{
	g_NESmapper.Reset = NES_mapper24_Reset;
	g_NESmapper.MemoryWrite = NES_mapper24_MemoryWrite;
	g_NESmapper.HSync = NES_mapper24_HSync;
}

void NES_mapper24_Reset()
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

	g_NESmapper.Mapper24.patch=0;
	if(NES_crc32() == 0xba6bdd6a)   // Akumajou Densetsu (J)
		g_NESmapper.Mapper24.patch=1;

	g_NESmapper.Mapper24.irq_enabled = 0;
	g_NESmapper.Mapper24.irq_counter = 0;
	g_NESmapper.Mapper24.irq_latch = 0;
}

void NES_mapper24_MemoryWrite(uint32 addr, uint8 data)
{
	switch(addr & 0xF003)
	{
	case 0x8000:
		{
			g_NESmapper.set_CPU_bank4(data*2+0);
			g_NESmapper.set_CPU_bank5(data*2+1);
		}
		break;

    //DCR
    case 0x9000:
    case 0x9001:
    case 0x9002:
    case 0xA000:
    case 0xA001:
    case 0xA002:
    case 0xB000:
    case 0xB001:
    case 0xB002:
      NES_APU_Write(addr, data);
      break;

	case 0xB003:
		{
			data = data & 0x0C;
			if(data == 0x00)
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
			}
			else if(data == 0x04)
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
			}
			else if(data == 0x08)
			{
				g_NESmapper.set_mirroring(0,0,0,0);
			}
			else if(data == 0x0C)
			{
				g_NESmapper.set_mirroring(1,1,1,1);
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
			g_NESmapper.set_PPU_bank1(data);
		}
		break;

	case 0xD002:
		{
			g_NESmapper.set_PPU_bank2(data);
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
			g_NESmapper.set_PPU_bank5(data);
		}
		break;

	case 0xE002:
		{
			g_NESmapper.set_PPU_bank6(data);
		}
		break;

	case 0xE003:
		{
			g_NESmapper.set_PPU_bank7(data);
		}
		break;

	case 0xF000:
		{
			g_NESmapper.Mapper24.irq_latch = data;
		}
		break;

	case 0xF001:
		{
			g_NESmapper.Mapper24.irq_enabled = data & 0x03;
			if(g_NESmapper.Mapper24.irq_enabled & 0x02)
			{
				g_NESmapper.Mapper24.irq_counter = (uint32)g_NESmapper.Mapper24.irq_latch;
			}
		}
		break;

	case 0xF002:
		{
			if(g_NESmapper.Mapper24.irq_enabled & 0x01)
			{
				g_NESmapper.Mapper24.irq_enabled |= 0x02;
			}
			else
			{
				g_NESmapper.Mapper24.irq_enabled &= 0x01;
			}
		}
		break;
	}
//	NES_APU_ExWrite(addr, data);
}

void NES_mapper24_HSync(uint32 scanline)
{
	if(g_NESmapper.Mapper24.irq_enabled & 0x02)
	{
		if(g_NESmapper.Mapper24.patch){
			if(g_NESmapper.Mapper24.irq_counter == 0xFF)
			{
				NES6502_DoPendingIRQ();
				g_NESmapper.Mapper24.irq_counter = g_NESmapper.Mapper24.irq_latch;
			}
			else
			{
				g_NESmapper.Mapper24.irq_counter++;
			}
		}
		else{
			if(g_NESmapper.Mapper24.irq_counter == 0xFF)
			{
				NES6502_DoIRQ();
				g_NESmapper.Mapper24.irq_counter = g_NESmapper.Mapper24.irq_latch;
			}
			else
			{
				g_NESmapper.Mapper24.irq_counter++;
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////

