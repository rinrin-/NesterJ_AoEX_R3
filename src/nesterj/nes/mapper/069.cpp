
/////////////////////////////////////////////////////////////////////
// Mapper 69
void NES_mapper69_Init()
{
	g_NESmapper.Reset = NES_mapper69_Reset;
	g_NESmapper.MemoryWrite = NES_mapper69_MemoryWrite;
	g_NESmapper.HSync = NES_mapper69_HSync;
}

void NES_mapper69_Reset()
{
	g_NESmapper.Mapper69.patch = 0;

	// for Dynamite Batman
	if(NES_crc32() == 0xad28aef6) // Dynamite Batman
	{
		g_NESmapper.Mapper69.patch = 1;
	}

	// Init ExSound
	NES_APU_SelectExSound(32);

	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);

	// set PPU bank pointers
	if(g_NESmapper.num_1k_VROM_banks)
	{
		g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);
	}

	g_NESmapper.Mapper69.irq_enabled = 0;
	g_NESmapper.Mapper69.irq_counter = 0;
}

void NES_mapper69_MemoryWrite(uint32 addr, uint8 data)
{
	switch(addr & 0xE000)
	{
	case 0x8000:
		{
			g_NESmapper.Mapper69.regs[0] = data;
		}
		break;

	case 0xA000:
		{
			switch(g_NESmapper.Mapper69.regs[0] & 0x0F)
			{
			case 0x00:
				{
					g_NESmapper.set_PPU_bank0(data);
				}
				break;

			case 0x01:
				{
					g_NESmapper.set_PPU_bank1(data);
				}
				break;

			case 0x02:
				{
					g_NESmapper.set_PPU_bank2(data);
				}
				break;

			case 0x03:
				{
					g_NESmapper.set_PPU_bank3(data);
				}
				break;

			case 0x04:
				{
					g_NESmapper.set_PPU_bank4(data);
				}
				break;

			case 0x05:
				{
					g_NESmapper.set_PPU_bank5(data);
				}
				break;

			case 0x06:
				{
					g_NESmapper.set_PPU_bank6(data);
				}
				break;

			case 0x07:
				{
					g_NESmapper.set_PPU_bank7(data);
				}
				break;

			case 0x08:
				{
					if(!g_NESmapper.Mapper69.patch)
					{
						if(!(data & 0x40))
						{
							g_NESmapper.set_CPU_bank3(data);
						}
					}
				}
				break;

			case 0x09:
				{
					g_NESmapper.set_CPU_bank4(data);
				}
				break;

			case 0x0A:
				{
					g_NESmapper.set_CPU_bank5(data);
				}
				break;

			case 0x0B:
				{
					g_NESmapper.set_CPU_bank6(data);
				}
				break;

			case 0x0C:
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

			case 0x0D:
				{
					g_NESmapper.Mapper69.irq_enabled = data;
				}
				break;

			case 0x0E:
				{
					g_NESmapper.Mapper69.irq_counter = (g_NESmapper.Mapper69.irq_counter & 0xFF00) | data;
				}
				break;

			case 0x0F:
				{
					g_NESmapper.Mapper69.irq_counter = (g_NESmapper.Mapper69.irq_counter & 0x00FF) | (data << 8);
				}
				break;
			}
		}
		break;

	case 0xC000:
	case 0xE000:
		{
			NES_APU_ExWrite(addr, data);
		}
		break;
	}
}

void NES_mapper69_HSync(uint32 scanline)
{
	if(g_NESmapper.Mapper69.irq_enabled)
	{
		if(g_NESmapper.Mapper69.irq_counter <= 113)
		{
			NES6502_DoIRQ();
			g_NESmapper.Mapper69.irq_counter = 0;
		}
		else
		{
			g_NESmapper.Mapper69.irq_counter -= 113;
		}
	}
}
/////////////////////////////////////////////////////////////////////

