#ifdef _NES_MAPPER_CPP_

/////////////////////////////////////////////////////////////////////
// Mapper 64
void NES_mapper64_Init()
{
	g_NESmapper.Reset = NES_mapper64_Reset;
	g_NESmapper.MemoryWrite = NES_mapper64_MemoryWrite;
	g_NESmapper.HSync = NES_mapper64_HSync;
}

void NES_mapper64_Reset()
{
	// set CPU bank pointers
	g_NESmapper.set_CPU_bank4(g_NESmapper.num_8k_ROM_banks-1);
	g_NESmapper.set_CPU_bank5(g_NESmapper.num_8k_ROM_banks-1);
	g_NESmapper.set_CPU_bank6(g_NESmapper.num_8k_ROM_banks-1);
	g_NESmapper.set_CPU_bank7(g_NESmapper.num_8k_ROM_banks-1);

	// set PPU bank pointers
	if(g_NESmapper.num_1k_VROM_banks)
	{
		g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);
	}

	g_NESmapper.Mapper64.irq_latch = 0;
	g_NESmapper.Mapper64.irq_counter = 0;
	g_NESmapper.Mapper64.irq_enabled = 0;

	g_NESmapper.Mapper64.regs[0] = 0;
	g_NESmapper.Mapper64.regs[1] = 0;
	g_NESmapper.Mapper64.regs[2] = 0;
}

void NES_mapper64_MemoryWrite(uint32 addr, uint8 data)
{
	switch(addr & 0xF003)
	{
	case 0x8000:
		{
			g_NESmapper.Mapper64.regs[0] = data & 0x0F;
			g_NESmapper.Mapper64.regs[1] = data & 0x40;
			g_NESmapper.Mapper64.regs[2] = data & 0x80;
		}
		break;

	case 0x8001:
		{
			switch(g_NESmapper.Mapper64.regs[0])
			{
			case 0x00:
				{
					if(g_NESmapper.Mapper64.regs[2])
					{
						g_NESmapper.set_PPU_bank4(data+0);
						g_NESmapper.set_PPU_bank5(data+1);
					}
					else
					{
						g_NESmapper.set_PPU_bank0(data+0);
						g_NESmapper.set_PPU_bank1(data+1);
					}
				}
				break;

			case 0x01:
				{
					if(g_NESmapper.Mapper64.regs[2])
					{
						g_NESmapper.set_PPU_bank6(data+0);
						g_NESmapper.set_PPU_bank7(data+1);
					}
					else
					{
						g_NESmapper.set_PPU_bank2(data+0);
						g_NESmapper.set_PPU_bank3(data+1);
					}
				}
				break;

			case 0x02:
				{
					if(g_NESmapper.Mapper64.regs[2])
					{
						g_NESmapper.set_PPU_bank0(data);
					}
					else
					{
						g_NESmapper.set_PPU_bank4(data);
					}
				}
				break;

			case 0x03:
				{
					if(g_NESmapper.Mapper64.regs[2])
					{
						g_NESmapper.set_PPU_bank1(data);
					}
					else
					{
						g_NESmapper.set_PPU_bank5(data);
					}
				}
				break;

			case 0x04:
				{
					if(g_NESmapper.Mapper64.regs[2])
					{
						g_NESmapper.set_PPU_bank2(data);
					}
					else
					{
						g_NESmapper.set_PPU_bank6(data);
					}
				}
				break;

			case 0x05:
				{
					if(g_NESmapper.Mapper64.regs[2])
					{
						g_NESmapper.set_PPU_bank3(data);
					}
					else
					{
						g_NESmapper.set_PPU_bank7(data);
					}
				}
				break;

			case 0x06:
				{
					if(g_NESmapper.Mapper64.regs[1])
					{
						g_NESmapper.set_CPU_bank5(data);
					}
					else
					{
						g_NESmapper.set_CPU_bank4(data);
					}
				}
				break;

			case 0x07:
				{
					if(g_NESmapper.Mapper64.regs[1])
					{
						g_NESmapper.set_CPU_bank6(data);
					}
					else
					{
						g_NESmapper.set_CPU_bank5(data);
					}
				}
				break;

			case 0x08:
				{
					//if(g_NESmapper.Mapper64.regs[2])
					//{
					//  set_PPU_bank5(data);
					//}
					//else
					{
						g_NESmapper.set_PPU_bank1(data);
					}
				}
				break;

			case 0x09:
				{
					//if(g_NESmapper.Mapper64.regs[2])
					//{
					//  set_PPU_bank7(data);
					//}
					//else
					{
						g_NESmapper.set_PPU_bank3(data);
					}
				}
				break;

			case 0x0F:
				{
					if(g_NESmapper.Mapper64.regs[1])
					{
						g_NESmapper.set_CPU_bank4(data);
					}
					else
					{
						g_NESmapper.set_CPU_bank6(data);
					}
				}
				break;
			}
		}
		break;

	case 0xA000:
		{
			if(!(data & 0x01))
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
			}
			else
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
			}
		}
		break;

	case 0xC000:
		{
			g_NESmapper.Mapper64.irq_latch = data;
			g_NESmapper.Mapper64.irq_counter = g_NESmapper.Mapper64.irq_latch;
		}
		break;

	case 0xC001:
		{
			g_NESmapper.Mapper64.irq_counter = g_NESmapper.Mapper64.irq_latch;
		}
		break;

	case 0xE000:
		{
			g_NESmapper.Mapper64.irq_enabled = 0;
			g_NESmapper.Mapper64.irq_counter = g_NESmapper.Mapper64.irq_latch;
		}
		break;

	case 0xE001:
		{
			g_NESmapper.Mapper64.irq_enabled = 1;
			g_NESmapper.Mapper64.irq_counter = g_NESmapper.Mapper64.irq_latch;
		}
		break;
	}
}

void NES_mapper64_HSync(uint32 scanline)
{
	if(g_NESmapper.Mapper64.irq_enabled)
	{
		if((scanline >= 0) && (scanline <= 239))
		{
			if(NES_PPU_spr_enabled() || NES_PPU_bg_enabled())
			{
				if(!(--g_NESmapper.Mapper64.irq_counter))
				{
					g_NESmapper.Mapper64.irq_counter = g_NESmapper.Mapper64.irq_latch;
					NES6502_DoIRQ();
				}
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////

#endif
