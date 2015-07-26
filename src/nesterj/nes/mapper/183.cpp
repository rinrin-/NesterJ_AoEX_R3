
/////////////////////////////////////////////////////////////////////
// Mapper 183
void NES_mapper183_Init()
{
	g_NESmapper.Reset = NES_mapper183_Reset;
	g_NESmapper.MemoryWrite = NES_mapper183_MemoryWrite;
	g_NESmapper.HSync = NES_mapper183_HSync;
}

void NES_mapper183_Reset()
{
	uint8 i;
	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);

	// set PPU bank pointers
	g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);

	for(i = 0; i < 8; i++)
	{
		g_NESmapper.Mapper183.regs[i] = i;
	}

	g_NESmapper.Mapper183.irq_enabled = 0;
	g_NESmapper.Mapper183.irq_counter = 0;
}

void NES_mapper183_MemoryWrite(uint32 addr, uint8 data)
{
	switch(addr)
	{
	case 0x8800:
		{
			g_NESmapper.set_CPU_bank4(data);
		}
		break;

	case 0xA800:
		{
			g_NESmapper.set_CPU_bank5(data);
		}
		break;

	case 0xA000:
		{
			g_NESmapper.set_CPU_bank6(data);
		}
		break;

	case 0xB000:
		{
			g_NESmapper.Mapper183.regs[0] = (g_NESmapper.Mapper183.regs[0] & 0xf0) | (data & 0x0f);
			g_NESmapper.set_PPU_bank0(g_NESmapper.Mapper183.regs[0]);
		}
		break;

	case 0xB004:
		{
			g_NESmapper.Mapper183.regs[0] = (g_NESmapper.Mapper183.regs[0] & 0x0f) | ((data & 0x0f) << 4);
			g_NESmapper.set_PPU_bank0(g_NESmapper.Mapper183.regs[0]);
		}
		break;

	case 0xB008:
		{
			g_NESmapper.Mapper183.regs[1] = (g_NESmapper.Mapper183.regs[1] & 0xf0) | (data & 0x0f);
			g_NESmapper.set_PPU_bank1(g_NESmapper.Mapper183.regs[1]);
		}
		break;

	case 0xB00C:
		{
			g_NESmapper.Mapper183.regs[1] = (g_NESmapper.Mapper183.regs[1] & 0x0f) | ((data & 0x0f) << 4);
			g_NESmapper.set_PPU_bank1(g_NESmapper.Mapper183.regs[1]);
		}
		break;

	case 0xC000:
		{
			g_NESmapper.Mapper183.regs[2] = (g_NESmapper.Mapper183.regs[2] & 0xf0) | (data & 0x0f);
			g_NESmapper.set_PPU_bank2(g_NESmapper.Mapper183.regs[2]);
		}
		break;

	case 0xC004:
		{
			g_NESmapper.Mapper183.regs[2] = (g_NESmapper.Mapper183.regs[2] & 0x0f) | ((data & 0x0f) << 4);
			g_NESmapper.set_PPU_bank2(g_NESmapper.Mapper183.regs[2]);
		}
		break;

	case 0xC008:
		{
			g_NESmapper.Mapper183.regs[3] = (g_NESmapper.Mapper183.regs[3] & 0xf0) | (data & 0x0f);
			g_NESmapper.set_PPU_bank3(g_NESmapper.Mapper183.regs[3]);
		}
		break;

	case 0xC00C:
		{
			g_NESmapper.Mapper183.regs[3] = (g_NESmapper.Mapper183.regs[3] & 0x0f) | ((data & 0x0f) << 4);
			g_NESmapper.set_PPU_bank3(g_NESmapper.Mapper183.regs[3]);
		}
		break;

	case 0xD000:
		{
			g_NESmapper.Mapper183.regs[4] = (g_NESmapper.Mapper183.regs[4] & 0xf0) | (data & 0x0f);
			g_NESmapper.set_PPU_bank4(g_NESmapper.Mapper183.regs[4]);
		}
		break;

	case 0xD004:
		{
			g_NESmapper.Mapper183.regs[4] = (g_NESmapper.Mapper183.regs[4] & 0x0f) | ((data & 0x0f) << 4);
			g_NESmapper.set_PPU_bank4(g_NESmapper.Mapper183.regs[4]);
		}
		break;

	case 0xD008:
		{
			g_NESmapper.Mapper183.regs[5] = (g_NESmapper.Mapper183.regs[5] & 0xf0) | (data & 0x0f);
			g_NESmapper.set_PPU_bank5(g_NESmapper.Mapper183.regs[5]);
		}
		break;

	case 0xD00C:
		{
			g_NESmapper.Mapper183.regs[5] = (g_NESmapper.Mapper183.regs[5] & 0x0f) | ((data & 0x0f) << 4);
			g_NESmapper.set_PPU_bank5(g_NESmapper.Mapper183.regs[5]);
		}
		break;

	case 0xE000:
		{
			g_NESmapper.Mapper183.regs[6] = (g_NESmapper.Mapper183.regs[6] & 0xf0) | (data & 0x0f);
			g_NESmapper.set_PPU_bank6(g_NESmapper.Mapper183.regs[6]);
		}
		break;

	case 0xE004:
		{
			g_NESmapper.Mapper183.regs[6] = (g_NESmapper.Mapper183.regs[6] & 0x0f) | ((data & 0x0f) << 4);
			g_NESmapper.set_PPU_bank6(g_NESmapper.Mapper183.regs[6]);
		}
		break;

	case 0xE008:
		{
			g_NESmapper.Mapper183.regs[7] = (g_NESmapper.Mapper183.regs[7] & 0xf0) | (data & 0x0f);
			g_NESmapper.set_PPU_bank7(g_NESmapper.Mapper183.regs[7]);
		}
		break;

	case 0xE00C:
		{
			g_NESmapper.Mapper183.regs[7] = (g_NESmapper.Mapper183.regs[7] & 0x0f) | ((data & 0x0f) << 4);
			g_NESmapper.set_PPU_bank7(g_NESmapper.Mapper183.regs[7]);
		}
		break;

	case 0x9008:
		{
			if(data == 1)
			{
				uint8 i;
				for(i = 0; i < 8; i++)
				{
					g_NESmapper.Mapper183.regs[i] = i;
				}
				g_NESmapper.set_CPU_banks4(0,1,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);
				g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);
			}
		}
		break;

	case 0x9800:
		{
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
			else if(data == 3)
			{
				g_NESmapper.set_mirroring(1,1,1,1);
			}
		}
		break;

	case 0xF000:
		{
			g_NESmapper.Mapper183.irq_counter = (g_NESmapper.Mapper183.irq_counter & 0xFF00) | data;
		}
		break;

	case 0xF004:
		{
			g_NESmapper.Mapper183.irq_counter = (g_NESmapper.Mapper183.irq_counter & 0x00FF) | (data << 8);
		}
		break;

	case 0xF008:
		{
			g_NESmapper.Mapper183.irq_enabled = data;
		}
		break;
	}
}

void NES_mapper183_HSync(uint32 scanline)
{
	if(g_NESmapper.Mapper183.irq_enabled & 0x02)
	{
		if(g_NESmapper.Mapper183.irq_counter <= 113)
		{
			NES6502_DoIRQ();
			g_NESmapper.Mapper183.irq_counter = 0;
		}
		else
		{
			g_NESmapper.Mapper183.irq_counter -= 113;
		}
	}
}
/////////////////////////////////////////////////////////////////////

