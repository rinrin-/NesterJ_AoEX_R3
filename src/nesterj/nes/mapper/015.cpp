
/////////////////////////////////////////////////////////////////////
// Mapper 15
void NES_mapper15_Init()
{
	g_NESmapper.Reset = NES_mapper15_Reset;
	g_NESmapper.MemoryWrite = NES_mapper15_MemoryWrite;
}

void NES_mapper15_Reset()
{
	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,2,3);
}

void NES_mapper15_MemoryWrite(uint32 addr, uint8 data)
{
	switch(addr)
	{
	case 0x8000:
		{
			if(data & 0x80)
			{
				g_NESmapper.set_CPU_bank4((data & 0x3F)*2+1);
				g_NESmapper.set_CPU_bank5((data & 0x3F)*2+0);
				g_NESmapper.set_CPU_bank6((data & 0x3F)*2+3);
				g_NESmapper.set_CPU_bank7((data & 0x3F)*2+2);
			}
			else
			{
				g_NESmapper.set_CPU_bank4((data & 0x3F)*2+0);
				g_NESmapper.set_CPU_bank5((data & 0x3F)*2+1);
				g_NESmapper.set_CPU_bank6((data & 0x3F)*2+2);
				g_NESmapper.set_CPU_bank7((data & 0x3F)*2+3);
			}
			if(data & 0x40)
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
			}
			else
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
			}
		}
		break;

	case 0x8001:
		{
			if(data & 0x80)
			{
				g_NESmapper.set_CPU_bank6((data & 0x3F)*2+1);
				g_NESmapper.set_CPU_bank7((data & 0x3F)*2+0);
			}
			else
			{
				g_NESmapper.set_CPU_bank6((data & 0x3F)*2+0);
				g_NESmapper.set_CPU_bank7((data & 0x3F)*2+1);
			}
		}
		break;

	case 0x8002:
		{
			if(data & 0x80)
			{
				g_NESmapper.set_CPU_bank4((data & 0x3F)*2+1);
				g_NESmapper.set_CPU_bank5((data & 0x3F)*2+1);
				g_NESmapper.set_CPU_bank6((data & 0x3F)*2+1);
				g_NESmapper.set_CPU_bank7((data & 0x3F)*2+1);
			}
			else
			{
				g_NESmapper.set_CPU_bank4((data & 0x3F)*2);
				g_NESmapper.set_CPU_bank5((data & 0x3F)*2);
				g_NESmapper.set_CPU_bank6((data & 0x3F)*2);
				g_NESmapper.set_CPU_bank7((data & 0x3F)*2);
			}
		}
		break;

	case 0x8003:
		{
			if(data & 0x80)
			{
				g_NESmapper.set_CPU_bank6((data & 0x3F)*2+1);
				g_NESmapper.set_CPU_bank7((data & 0x3F)*2+0);
			}
			else
			{
				g_NESmapper.set_CPU_bank6((data & 0x3F)*2+0);
				g_NESmapper.set_CPU_bank7((data & 0x3F)*2+1);
			}
			if(data & 0x40)
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
			}
			else
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
			}
		}
		break;
	}
}
/////////////////////////////////////////////////////////////////////

