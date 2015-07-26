
/////////////////////////////////////////////////////////////////////
// Mapper 105
void NES_mapper105_Init()
{
	g_NESmapper.Reset = NES_mapper105_Reset;
	g_NESmapper.MemoryWrite = NES_mapper105_MemoryWrite;
	g_NESmapper.HSync = NES_mapper105_HSync;
}

void NES_mapper105_Reset()
{
	g_NESmapper.set_CPU_banks4(0,1,2,3);

	g_NESmapper.Mapper105.regs[0] = 0x0C;
	g_NESmapper.Mapper105.regs[1] = 0x00;
	g_NESmapper.Mapper105.regs[2] = 0x00;
	g_NESmapper.Mapper105.regs[3] = 0x10;

	g_NESmapper.Mapper105.bits = 0;
	g_NESmapper.Mapper105.write_count = 0;

	g_NESmapper.Mapper105.irq_counter = 0;
	g_NESmapper.Mapper105.irq_enabled = 0;
	g_NESmapper.Mapper105.init_state = 0;
}

void NES_mapper105_MemoryWrite(uint32 addr, uint8 data)
{
	uint32 reg_num = (addr & 0x7FFF) >> 13;

	if (data & 0x80)
	{
		g_NESmapper.Mapper105.bits = g_NESmapper.Mapper105.write_count = 0;
		if (reg_num == 0)
		{
			g_NESmapper.Mapper105.regs[reg_num] |= 0x0C;
		}
	}
	else
	{
		g_NESmapper.Mapper105.bits |= (data & 1) << g_NESmapper.Mapper105.write_count++;
		if (g_NESmapper.Mapper105.write_count == 5)
		{
			g_NESmapper.Mapper105.regs[reg_num] = g_NESmapper.Mapper105.bits & 0x1F;
			g_NESmapper.Mapper105.bits = g_NESmapper.Mapper105.write_count = 0;
		}
	}

	if(g_NESmapper.Mapper105.regs[0] & 0x02)
	{
		if(g_NESmapper.Mapper105.regs[0] & 0x01)
		{
			g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
		}
		else
		{
			g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
		}
	}
	else
	{
		if(g_NESmapper.Mapper105.regs[0] & 0x01)
		{
			g_NESmapper.set_mirroring(1,1,1,1);
		}
		else
		{
			g_NESmapper.set_mirroring(0,0,0,0);
		}
	}

	switch (g_NESmapper.Mapper105.init_state)
	{
	case 0:
	case 1:
		{
			g_NESmapper.Mapper105.init_state++;
		}
		break;

	case 2:
		{
			if(g_NESmapper.Mapper105.regs[1] & 0x08)
			{
				if (g_NESmapper.Mapper105.regs[0] & 0x08)
				{
					if (g_NESmapper.Mapper105.regs[0] & 0x04)
					{
						g_NESmapper.set_CPU_bank4((g_NESmapper.Mapper105.regs[3] & 0x07) * 2 + 16);
						g_NESmapper.set_CPU_bank5((g_NESmapper.Mapper105.regs[3] & 0x07) * 2 + 17);
						g_NESmapper.set_CPU_bank6(30);
						g_NESmapper.set_CPU_bank7(31);
					}
					else
					{
						g_NESmapper.set_CPU_bank4(16);
						g_NESmapper.set_CPU_bank5(17);
						g_NESmapper.set_CPU_bank6((g_NESmapper.Mapper105.regs[3] & 0x07) * 2 + 16);
						g_NESmapper.set_CPU_bank7((g_NESmapper.Mapper105.regs[3] & 0x07) * 2 + 17);
					}
				}
				else
				{
					g_NESmapper.set_CPU_bank4((g_NESmapper.Mapper105.regs[3] & 0x06) * 2 + 16);
					g_NESmapper.set_CPU_bank5((g_NESmapper.Mapper105.regs[3] & 0x06) * 2 + 17);
					g_NESmapper.set_CPU_bank6((g_NESmapper.Mapper105.regs[3] & 0x06) * 2 + 18);
					g_NESmapper.set_CPU_bank7((g_NESmapper.Mapper105.regs[3] & 0x06) * 2 + 19);
				}
			}
			else
			{
				g_NESmapper.set_CPU_bank4((g_NESmapper.Mapper105.regs[1] & 0x06) * 2 + 0);
				g_NESmapper.set_CPU_bank5((g_NESmapper.Mapper105.regs[1] & 0x06) * 2 + 1);
				g_NESmapper.set_CPU_bank6((g_NESmapper.Mapper105.regs[1] & 0x06) * 2 + 2);
				g_NESmapper.set_CPU_bank7((g_NESmapper.Mapper105.regs[1] & 0x06) * 2 + 3);
			}

			if(g_NESmapper.Mapper105.regs[1] & 0x10)
			{
				g_NESmapper.Mapper105.irq_counter = 0;
				g_NESmapper.Mapper105.irq_enabled = 0;
			}
			else
			{
				g_NESmapper.Mapper105.irq_enabled = 1;
			}
		}
		break;
	}
}

void NES_mapper105_HSync(uint32 scanline)
{
	if(!scanline)
	{
		if(g_NESmapper.Mapper105.irq_enabled)
		{
			g_NESmapper.Mapper105.irq_counter += 29781;
		}
		if(((g_NESmapper.Mapper105.irq_counter | 0x21FFFFFF) & 0x3E000000) == 0x3E000000)
		{
			NES6502_DoIRQ();
		}
	}
}

