
/////////////////////////////////////////////////////////////////////
// Mapper 182
void NES_mapper182_Init()
{
	g_NESmapper.Reset = NES_mapper182_Reset;
	g_NESmapper.MemoryWrite = NES_mapper182_MemoryWrite;
	g_NESmapper.HSync = NES_mapper182_HSync;
}

void NES_mapper182_Reset()
{
	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);

	// set PPU bank pointers
	g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);

	g_NESmapper.Mapper182.regs[0] = 0;

	g_NESmapper.Mapper182.irq_enabled = 0;
	g_NESmapper.Mapper182.irq_counter = 0;
}

void NES_mapper182_MemoryWrite(uint32 addr, uint8 data)
{
	switch(addr & 0xF003)
	{
	case 0x8001:
		{
			if(data & 0x01)
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
			}
			else
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
			}
		}
		break;

	case 0xA000:
		{
			g_NESmapper.Mapper182.regs[0] = data & 0x07;
		}
		break;

	case 0xC000:
		{
			switch(g_NESmapper.Mapper182.regs[0])
			{
			case 0x00:
				{
					g_NESmapper.set_PPU_bank0((data & 0xFE)+0);
					g_NESmapper.set_PPU_bank1((data & 0xFE)+1);
				}
				break;

			case 0x01:
				{
					g_NESmapper.set_PPU_bank5(data);
				}
				break;

			case 0x02:
				{
					g_NESmapper.set_PPU_bank2((data & 0xFE)+0);
					g_NESmapper.set_PPU_bank3((data & 0xFE)+1);
				}
				break;

			case 0x03:
				{
					g_NESmapper.set_PPU_bank7(data);
				}
				break;

			case 0x04:
				{
					g_NESmapper.set_CPU_bank4(data);
				}
				break;

			case 0x05:
				{
					g_NESmapper.set_CPU_bank5(data);
				}
				break;

			case 0x06:
				{
					g_NESmapper.set_PPU_bank4(data);
				}
				break;

			case 0x07:
				{
					g_NESmapper.set_PPU_bank6(data);
				}
				break;
			}
		}
		break;

	case 0xE003:
		{
			g_NESmapper.Mapper182.irq_counter = data;
			g_NESmapper.Mapper182.irq_enabled = data;
		}
		break;
	}
}

void NES_mapper182_HSync(uint32 scanline)
{
	if(g_NESmapper.Mapper182.irq_enabled)
	{
		if((scanline >= 0) && (scanline <= 240))
		{
			if(NES_PPU_spr_enabled() || NES_PPU_bg_enabled())
			{
				if(!(--g_NESmapper.Mapper182.irq_counter))
				{
					g_NESmapper.Mapper182.irq_counter = 0;
					g_NESmapper.Mapper182.irq_enabled = 0;
					NES6502_DoIRQ();
				}
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////

