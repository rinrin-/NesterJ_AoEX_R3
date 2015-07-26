
/////////////////////////////////////////////////////////////////////
// Mapper 45
void NES_mapper45_Init()
{
	g_NESmapper.Reset = NES_mapper45_Reset;
	g_NESmapper.MemoryWriteSaveRAM = NES_mapper45_MemoryWriteSaveRAM;
	g_NESmapper.MemoryWrite = NES_mapper45_MemoryWrite;
	g_NESmapper.HSync = NES_mapper45_HSync;
}

void NES_mapper45_Reset()
{
	g_NESmapper.Mapper45.patch = 0;

	g_NESmapper.Mapper45.prg0 = 0;
	g_NESmapper.Mapper45.prg1 = 1;
	g_NESmapper.Mapper45.prg2 = g_NESmapper.num_8k_ROM_banks-2;
	g_NESmapper.Mapper45.prg3 = g_NESmapper.num_8k_ROM_banks-1;

	if(NES_crc32() == 0x58bcacf6 || // Kunio 8-in-1 (Pirate Cart)
	        NES_crc32() == 0x9103cfd6 || // HIK 7-in-1 (Pirate Cart)
	        NES_crc32() == 0xc082e6d3)   // Super 8-in-1 (Pirate Cart)
	{
		g_NESmapper.Mapper45.patch = 1;
		g_NESmapper.Mapper45.prg2 = 62;
		g_NESmapper.Mapper45.prg3 = 63;
	}

	g_NESmapper.set_CPU_banks4(g_NESmapper.Mapper45.prg0,g_NESmapper.Mapper45.prg1,g_NESmapper.Mapper45.prg2,g_NESmapper.Mapper45.prg3);
	g_NESmapper.Mapper45.p[0] = g_NESmapper.Mapper45.prg0;
	g_NESmapper.Mapper45.p[1] = g_NESmapper.Mapper45.prg1;
	g_NESmapper.Mapper45.p[2] = g_NESmapper.Mapper45.prg2;
	g_NESmapper.Mapper45.p[3] = g_NESmapper.Mapper45.prg3;

	// set PPU bank pointers
	g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);
	g_NESmapper.Mapper45.chr0 = 0; g_NESmapper.Mapper45.chr1 = 1; g_NESmapper.Mapper45.chr2 = 2; g_NESmapper.Mapper45.chr3 = 3;
	g_NESmapper.Mapper45.chr4 = 4; g_NESmapper.Mapper45.chr5 = 5; g_NESmapper.Mapper45.chr6 = 6; g_NESmapper.Mapper45.chr7 = 7;
	g_NESmapper.Mapper45.c[0] = g_NESmapper.Mapper45.chr0; g_NESmapper.Mapper45.c[1] = g_NESmapper.Mapper45.chr1; g_NESmapper.Mapper45.c[2] = g_NESmapper.Mapper45.chr2; g_NESmapper.Mapper45.c[3] = g_NESmapper.Mapper45.chr3;
	g_NESmapper.Mapper45.c[4] = g_NESmapper.Mapper45.chr4; g_NESmapper.Mapper45.c[5] = g_NESmapper.Mapper45.chr5; g_NESmapper.Mapper45.c[6] = g_NESmapper.Mapper45.chr6; g_NESmapper.Mapper45.c[7] = g_NESmapper.Mapper45.chr7;

	g_NESmapper.Mapper45.irq_enabled = 0;
	g_NESmapper.Mapper45.irq_counter = 0;
	g_NESmapper.Mapper45.irq_latch = 0;

	g_NESmapper.Mapper45.regs[0] = g_NESmapper.Mapper45.regs[1] = g_NESmapper.Mapper45.regs[2] = g_NESmapper.Mapper45.regs[3] = 0;
	g_NESmapper.Mapper45.regs[4] = g_NESmapper.Mapper45.regs[5] = g_NESmapper.Mapper45.regs[6] = 0;
}

void NES_mapper45_MemoryWriteSaveRAM(uint32 addr, uint8 data)
{
	if(addr == 0x6000)
	{
		g_NESmapper.Mapper45.regs[g_NESmapper.Mapper45.regs[5]] = data;
		g_NESmapper.Mapper45.regs[5]= (g_NESmapper.Mapper45.regs[5]+1) & 0x03;
		NES_mapper45_MAP45_set_CPU_bank4((uint8)g_NESmapper.Mapper45.prg0);
		NES_mapper45_MAP45_set_CPU_bank5((uint8)g_NESmapper.Mapper45.prg1);
		NES_mapper45_MAP45_set_CPU_bank6((uint8)g_NESmapper.Mapper45.prg2);
		NES_mapper45_MAP45_set_CPU_bank7((uint8)g_NESmapper.Mapper45.prg3);
		NES_mapper45_MAP45_set_PPU_banks();
	}
}

void NES_mapper45_MemoryWrite(uint32 addr, uint8 data)
{
	switch(addr & 0xE001)
	{
	case 0x8000:
		{
			uint32 swap;
			if((data & 0x40) != (g_NESmapper.Mapper45.regs[6] & 0x40))
			{
				swap = g_NESmapper.Mapper45.prg0; g_NESmapper.Mapper45.prg0 = g_NESmapper.Mapper45.prg2; g_NESmapper.Mapper45.prg2 = swap;
				swap = g_NESmapper.Mapper45.p[0]; g_NESmapper.Mapper45.p[0] = g_NESmapper.Mapper45.p[2]; g_NESmapper.Mapper45.p[2] = swap;
				g_NESmapper.set_CPU_bank4(g_NESmapper.Mapper45.p[0]);
				g_NESmapper.set_CPU_bank6(g_NESmapper.Mapper45.p[2]);
			}
			if(g_NESmapper.num_8k_ROM_banks)
			{
				if((data & 0x80) != (g_NESmapper.Mapper45.regs[6] & 0x80))
				{
					swap = g_NESmapper.Mapper45.chr4; g_NESmapper.Mapper45.chr4 = g_NESmapper.Mapper45.chr0; g_NESmapper.Mapper45.chr0 = swap;
					swap = g_NESmapper.Mapper45.chr5; g_NESmapper.Mapper45.chr5 = g_NESmapper.Mapper45.chr1; g_NESmapper.Mapper45.chr1 = swap;
					swap = g_NESmapper.Mapper45.chr6; g_NESmapper.Mapper45.chr6 = g_NESmapper.Mapper45.chr2; g_NESmapper.Mapper45.chr2 = swap;
					swap = g_NESmapper.Mapper45.chr7; g_NESmapper.Mapper45.chr7 = g_NESmapper.Mapper45.chr3; g_NESmapper.Mapper45.chr3 = swap;
					swap = g_NESmapper.Mapper45.c[4]; g_NESmapper.Mapper45.c[4] = g_NESmapper.Mapper45.c[0]; g_NESmapper.Mapper45.c[0] = swap;
					swap = g_NESmapper.Mapper45.c[5]; g_NESmapper.Mapper45.c[5] = g_NESmapper.Mapper45.c[1]; g_NESmapper.Mapper45.c[1] = swap;
					swap = g_NESmapper.Mapper45.c[6]; g_NESmapper.Mapper45.c[6] = g_NESmapper.Mapper45.c[2]; g_NESmapper.Mapper45.c[2] = swap;
					swap = g_NESmapper.Mapper45.c[7]; g_NESmapper.Mapper45.c[7] = g_NESmapper.Mapper45.c[3]; g_NESmapper.Mapper45.c[3] = swap;
					g_NESmapper.set_PPU_banks8(g_NESmapper.Mapper45.c[0],g_NESmapper.Mapper45.c[1],g_NESmapper.Mapper45.c[2],g_NESmapper.Mapper45.c[3],g_NESmapper.Mapper45.c[4],g_NESmapper.Mapper45.c[5],g_NESmapper.Mapper45.c[6],g_NESmapper.Mapper45.c[7]);
				}
			}
			g_NESmapper.Mapper45.regs[6] = data;
		}
		break;

	case 0x8001:
		{
			switch(g_NESmapper.Mapper45.regs[6] & 0x07)
			{
			case 0x00:
				{
					g_NESmapper.Mapper45.chr0 = (data & 0xFE)+0;
					g_NESmapper.Mapper45.chr1 = (data & 0xFE)+1;
					NES_mapper45_MAP45_set_PPU_banks();
				}
				break;

			case 0x01:
				{
					g_NESmapper.Mapper45.chr2 = (data & 0xFE)+0;
					g_NESmapper.Mapper45.chr3 = (data & 0xFE)+1;
					NES_mapper45_MAP45_set_PPU_banks();
				}
				break;

			case 0x02:
				{
					g_NESmapper.Mapper45.chr4 = data;
					NES_mapper45_MAP45_set_PPU_banks();
				}
				break;

			case 0x03:
				{
					g_NESmapper.Mapper45.chr5 = data;
					NES_mapper45_MAP45_set_PPU_banks();
				}
				break;

			case 0x04:
				{
					g_NESmapper.Mapper45.chr6 = data;
					NES_mapper45_MAP45_set_PPU_banks();
				}
				break;

			case 0x05:
				{
					g_NESmapper.Mapper45.chr7 = data;
					NES_mapper45_MAP45_set_PPU_banks();
				}
				break;

			case 0x06:
				{
					if(g_NESmapper.Mapper45.regs[6] & 0x40)
					{
						g_NESmapper.Mapper45.prg2 = data & 0x3F;
						NES_mapper45_MAP45_set_CPU_bank6(data);
					}
					else
					{
						g_NESmapper.Mapper45.prg0 = data & 0x3F;
						NES_mapper45_MAP45_set_CPU_bank4(data);
					}
				}
				break;

			case 0x07:
				{
					g_NESmapper.Mapper45.prg1 = data & 0x3F;
					NES_mapper45_MAP45_set_CPU_bank5(data);
				}
				break;
			}
		}
		break;

	case 0xA000:
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

	case 0xC000:
		{
			g_NESmapper.Mapper45.irq_counter = data;
		}
		break;

	case 0xC001:
		{
			g_NESmapper.Mapper45.irq_latch = data;
		}
		break;

	case 0xE000:
		{
			g_NESmapper.Mapper45.irq_enabled = 0;
		}
		break;

	case 0xE001:
		{
			g_NESmapper.Mapper45.irq_enabled = 1;
		}
		break;
	}
}

void NES_mapper45_HSync(uint32 scanline)
{
	if(g_NESmapper.Mapper45.irq_enabled)
	{
		if((scanline >= 0) && (scanline <= 239))
		{
			if(NES_PPU_spr_enabled() || NES_PPU_bg_enabled())
			{
				if(!(g_NESmapper.Mapper45.irq_counter--))
				{
					g_NESmapper.Mapper45.irq_counter = g_NESmapper.Mapper45.irq_latch;
					NES6502_DoIRQ();
				}
			}
		}
	}
}

void NES_mapper45_MAP45_set_CPU_bank4(uint8 data)
{
	data &= (g_NESmapper.Mapper45.regs[3] & 0x3F) ^ 0xFF;
	data &= 0x3F;
	data |= g_NESmapper.Mapper45.regs[1];
	g_NESmapper.set_CPU_bank4(data);
	g_NESmapper.Mapper45.p[0] = data;
}

void NES_mapper45_MAP45_set_CPU_bank5(uint8 data)
{
	data &= (g_NESmapper.Mapper45.regs[3] & 0x3F) ^ 0xFF;
	data &= 0x3F;
	data |= g_NESmapper.Mapper45.regs[1];
	g_NESmapper.set_CPU_bank5(data);
	g_NESmapper.Mapper45.p[1] = data;
}
void NES_mapper45_MAP45_set_CPU_bank6(uint8 data)
{
	data &= (g_NESmapper.Mapper45.regs[3] & 0x3F) ^ 0xFF;
	data &= 0x3F;
	data |= g_NESmapper.Mapper45.regs[1];
	g_NESmapper.set_CPU_bank6(data);
	g_NESmapper.Mapper45.p[2] = data;
}
void NES_mapper45_MAP45_set_CPU_bank7(uint8 data)
{
	data &= (g_NESmapper.Mapper45.regs[3] & 0x3F) ^ 0xFF;
	data &= 0x3F;
	data |= g_NESmapper.Mapper45.regs[1];
	g_NESmapper.set_CPU_bank7(data);
	g_NESmapper.Mapper45.p[3] = data;
}

void NES_mapper45_MAP45_set_PPU_banks()
{
	uint8 table[16] =
	    {
	        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	        0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7F,0xFF
	    };
	uint8 i;

	g_NESmapper.Mapper45.c[0] = g_NESmapper.Mapper45.chr0;
	g_NESmapper.Mapper45.c[1] = g_NESmapper.Mapper45.chr1;
	g_NESmapper.Mapper45.c[2] = g_NESmapper.Mapper45.chr2;
	g_NESmapper.Mapper45.c[3] = g_NESmapper.Mapper45.chr3;
	g_NESmapper.Mapper45.c[4] = g_NESmapper.Mapper45.chr4;
	g_NESmapper.Mapper45.c[5] = g_NESmapper.Mapper45.chr5;
	g_NESmapper.Mapper45.c[6] = g_NESmapper.Mapper45.chr6;
	g_NESmapper.Mapper45.c[7] = g_NESmapper.Mapper45.chr7;
	for(i = 0; i < 8; i++)
	{
		g_NESmapper.Mapper45.c[i] &= table[g_NESmapper.Mapper45.regs[2] & 0x0F];
		g_NESmapper.Mapper45.c[i] |= g_NESmapper.Mapper45.regs[0] & ((g_NESmapper.Mapper45.patch == 0) ? 0xff : 0xC0);
		g_NESmapper.Mapper45.c[i] += (uint32)(g_NESmapper.Mapper45.regs[2] & ((g_NESmapper.Mapper45.patch == 0) ? 0x10 : 0x30)) << 4;
	}
	if(g_NESmapper.Mapper45.regs[6] & 0x80)
	{
//LOG("W4 " << HEX(g_NESmapper.Mapper45.c[4],2)<<" " << HEX(g_NESmapper.Mapper45.c[5],2)<<" " << HEX(g_NESmapper.Mapper45.c[6],2)<<" " << HEX(g_NESmapper.Mapper45.c[7],2)<<" " << HEX(g_NESmapper.Mapper45.c[0],2)<<" " << HEX(g_NESmapper.Mapper45.c[1],2)<<" " << HEX(g_NESmapper.Mapper45.c[2],2)<<" " << HEX(g_NESmapper.Mapper45.c[3],2) << endl);
		g_NESmapper.set_PPU_banks8(g_NESmapper.Mapper45.c[4],g_NESmapper.Mapper45.c[5],g_NESmapper.Mapper45.c[6],g_NESmapper.Mapper45.c[7],g_NESmapper.Mapper45.c[0],g_NESmapper.Mapper45.c[1],g_NESmapper.Mapper45.c[2],g_NESmapper.Mapper45.c[3]);
	}
	else
	{
//LOG("W4 " << " " << HEX(g_NESmapper.Mapper45.c[0],2)<<" " << HEX(g_NESmapper.Mapper45.c[1],2)<<" " << HEX(g_NESmapper.Mapper45.c[2],2)<<" " << HEX(g_NESmapper.Mapper45.c[3],2) << HEX(g_NESmapper.Mapper45.c[4],2)<<" " << HEX(g_NESmapper.Mapper45.c[5],2)<<" " << HEX(g_NESmapper.Mapper45.c[6],2)<<" " << HEX(g_NESmapper.Mapper45.c[7],2)<< endl);
		g_NESmapper.set_PPU_banks8(g_NESmapper.Mapper45.c[0],g_NESmapper.Mapper45.c[1],g_NESmapper.Mapper45.c[2],g_NESmapper.Mapper45.c[3],g_NESmapper.Mapper45.c[4],g_NESmapper.Mapper45.c[5],g_NESmapper.Mapper45.c[6],g_NESmapper.Mapper45.c[7]);
	}
}
/////////////////////////////////////////////////////////////////////

