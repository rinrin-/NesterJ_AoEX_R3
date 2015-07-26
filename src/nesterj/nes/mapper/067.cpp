
/////////////////////////////////////////////////////////////////////
// Mapper 67
void NES_mapper67_Init()
{
	g_NESmapper.Reset = NES_mapper67_Reset;
	g_NESmapper.MemoryWrite = NES_mapper67_MemoryWrite;
	g_NESmapper.HSync = NES_mapper67_HSync;
}

void NES_mapper67_Reset()
{
	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);

	// set PPU bank pointers
	g_NESmapper.set_PPU_bank0(0);
	g_NESmapper.set_PPU_bank1(1);
	g_NESmapper.set_PPU_bank2(2);
	g_NESmapper.set_PPU_bank3(3);
	g_NESmapper.set_PPU_bank4(g_NESmapper.num_8k_ROM_banks-4);
	g_NESmapper.set_PPU_bank5(g_NESmapper.num_8k_ROM_banks-3);
	g_NESmapper.set_PPU_bank6(g_NESmapper.num_8k_ROM_banks-2);
	g_NESmapper.set_PPU_bank7(g_NESmapper.num_8k_ROM_banks-1);

	g_NESmapper.Mapper67.irq_enabled = 0;
	g_NESmapper.Mapper67.irq_counter = 0;
	g_NESmapper.Mapper67.irq_latch = 0;
}

void NES_mapper67_MemoryWrite(uint32 addr, uint8 data)
{
	switch(addr & 0xF800)
	{
	case 0x8800:
		{
			g_NESmapper.set_PPU_bank0(data*2+0);
			g_NESmapper.set_PPU_bank1(data*2+1);
		}
		break;

	case 0x9800:
		{
			g_NESmapper.set_PPU_bank2(data*2+0);
			g_NESmapper.set_PPU_bank3(data*2+1);
		}
		break;

	case 0xA800:
		{
			g_NESmapper.set_PPU_bank4(data*2+0);
			g_NESmapper.set_PPU_bank5(data*2+1);
		}
		break;

	case 0xB800:
		{
			g_NESmapper.set_PPU_bank6(data*2+0);
			g_NESmapper.set_PPU_bank7(data*2+1);
		}
		break;

	case 0xC800:
		{
			g_NESmapper.Mapper67.irq_counter = g_NESmapper.Mapper67.irq_latch;
			g_NESmapper.Mapper67.irq_latch = data;
		}
		break;

	case 0xD800:
		{
			g_NESmapper.Mapper67.irq_enabled = data & 0x10;
		}
		break;

	case 0xE800:
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
				g_NESmapper.set_mirroring(0,0,0,0);
			}
			else
			{
				g_NESmapper.set_mirroring(1,1,1,1);
			}
		}
		break;

	case 0xF800:
		{
			g_NESmapper.set_CPU_bank4(data*2+0);
			g_NESmapper.set_CPU_bank5(data*2+1);
		}
		break;
	}
}

void NES_mapper67_HSync(uint32 scanline)
{
	if(g_NESmapper.Mapper67.irq_enabled)
	{
		if((scanline >= 0) && (scanline <= 239))
		{
			if(NES_PPU_spr_enabled() || NES_PPU_bg_enabled())
			{
				if(--g_NESmapper.Mapper67.irq_counter == 0xF6)
				{
					g_NESmapper.Mapper67.irq_counter = g_NESmapper.Mapper67.irq_latch;
					NES6502_DoIRQ();
				}
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////

