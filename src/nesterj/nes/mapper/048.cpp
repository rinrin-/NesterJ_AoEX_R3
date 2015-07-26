
/////////////////////////////////////////////////////////////////////
// Mapper 48
void NES_mapper48_Init()
{
	g_NESmapper.Reset = NES_mapper48_Reset;
	g_NESmapper.MemoryWrite = NES_mapper48_MemoryWrite;
	g_NESmapper.HSync = NES_mapper48_HSync;
}

void NES_mapper48_Reset()
{
	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);

	// set PPU bank pointers
	g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);

	g_NESmapper.Mapper48.regs[0] = 0;
	g_NESmapper.Mapper48.irq_enabled = 0;
	g_NESmapper.Mapper48.irq_counter = 0;
}

void NES_mapper48_MemoryWrite(uint32 addr, uint8 data)
{
	switch(addr)
	{
	case 0x8000:
		{
			if(!g_NESmapper.Mapper48.regs[0])
			{
				if(data & 0x40)
				{
					g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
				}
				else
				{
					g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
				}
			}
			g_NESmapper.set_CPU_bank4(data);
		}
		break;

	case 0x8001:
		{
			g_NESmapper.set_CPU_bank5(data);
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
			g_NESmapper.Mapper48.irq_counter = data;
		}
		break;

	case 0xC001:
		{
			g_NESmapper.Mapper48.irq_enabled = data & 0x01;
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
			g_NESmapper.Mapper48.regs[0] = 1;
		}
		break;
	}
}

void NES_mapper48_HSync(uint32 scanline)
{
	if(g_NESmapper.Mapper48.irq_enabled)
	{
		if((scanline >= 0) && (scanline <= 239))
		{
			if(NES_PPU_spr_enabled() || NES_PPU_bg_enabled())
			{
				if(g_NESmapper.Mapper48.irq_counter == 0xFF)
				{
					NES6502_DoIRQ();
					g_NESmapper.Mapper48.irq_enabled = 0;
				}
				else
				{
					g_NESmapper.Mapper48.irq_counter++;
				}
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////

