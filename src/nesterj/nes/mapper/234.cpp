
/////////////////////////////////////////////////////////////////////
// Mapper 234
void NES_mapper234_Init()
{
	g_NESmapper.Reset = NES_mapper234_Reset;
	g_NESmapper.MemoryReadSaveRAM = NES_mapper234_MemoryReadSaveRAM;
	g_NESmapper.MemoryWrite = NES_mapper234_MemoryWrite;
}

void NES_mapper234_Reset()
{
	g_NESmapper.Mapper234.regs[0] = g_NESmapper.Mapper234.regs[1] = g_NESmapper.Mapper234.regs[2] = 0;
	NES_mapper234_Sync();
}

void NES_mapper234_MemoryReadSaveRAM(uint32 addr)
{
	nes6502_context context;
	uint8 data;
	NES6502_GetContext(&context);
	data = context.mem_page[addr >> 13][addr & 0x1fff];

	switch(addr & 0xFFF8)
	{
	case 0xFF80:
	case 0xFF88:
	case 0xFF90:
	case 0xFF98:
		{
			if(!g_NESmapper.Mapper234.regs[0])
			{
				g_NESmapper.Mapper234.regs[0] = data;
				NES_mapper234_Sync();
			}
		}
		break;

	case 0xFFC0:
	case 0xFFC8:
	case 0xFFD0:
	case 0xFFD8:
		{
			if(!g_NESmapper.Mapper234.regs[2])
			{
				g_NESmapper.Mapper234.regs[2] = data;
				NES_mapper234_Sync();
			}
		}
		break;

	case 0xFFE8:
	case 0xFFF0:
		{
			g_NESmapper.Mapper234.regs[1] = data;
		}
		break;
	}
}

void NES_mapper234_MemoryWrite(uint32 addr, uint8 data)
{
	switch(addr & 0xFFF8)
	{
	case 0xFF80:
	case 0xFF88:
	case 0xFF90:
	case 0xFF98:
		{
			if(!g_NESmapper.Mapper234.regs[0])
			{
				g_NESmapper.Mapper234.regs[0] = data;
				NES_mapper234_Sync();
			}
		}
		break;

	case 0xFFC0:
	case 0xFFC8:
	case 0xFFD0:
	case 0xFFD8:
		{
			if(!g_NESmapper.Mapper234.regs[2])
			{
				g_NESmapper.Mapper234.regs[2] = data;
				NES_mapper234_Sync();
			}
		}
		break;

	case 0xFFE8:
	case 0xFFF0:
		{
			g_NESmapper.Mapper234.regs[1] = data;
		}
		break;
	}
}

void NES_mapper234_Sync()
{
	if(g_NESmapper.Mapper234.regs[0] & 0x80)
	{
		g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
	}
	else
	{
		g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
	}
	if (g_NESmapper.Mapper234.regs[0] & 0x40)
	{
		g_NESmapper.set_CPU_bank4(((g_NESmapper.Mapper234.regs[0] & 0x0E)|(g_NESmapper.Mapper234.regs[1] & 0x01))*4+0);
		g_NESmapper.set_CPU_bank5(((g_NESmapper.Mapper234.regs[0] & 0x0E)|(g_NESmapper.Mapper234.regs[1] & 0x01))*4+1);
		g_NESmapper.set_CPU_bank6(((g_NESmapper.Mapper234.regs[0] & 0x0E)|(g_NESmapper.Mapper234.regs[1] & 0x01))*4+2);
		g_NESmapper.set_CPU_bank7(((g_NESmapper.Mapper234.regs[0] & 0x0E)|(g_NESmapper.Mapper234.regs[1] & 0x01))*4+3);
		g_NESmapper.set_PPU_bank0((((g_NESmapper.Mapper234.regs[0] & 0x0E)<<2)|((g_NESmapper.Mapper234.regs[1] & 0x70)>>4))*8+0);
		g_NESmapper.set_PPU_bank1((((g_NESmapper.Mapper234.regs[0] & 0x0E)<<2)|((g_NESmapper.Mapper234.regs[1] & 0x70)>>4))*8+1);
		g_NESmapper.set_PPU_bank2((((g_NESmapper.Mapper234.regs[0] & 0x0E)<<2)|((g_NESmapper.Mapper234.regs[1] & 0x70)>>4))*8+2);
		g_NESmapper.set_PPU_bank3((((g_NESmapper.Mapper234.regs[0] & 0x0E)<<2)|((g_NESmapper.Mapper234.regs[1] & 0x70)>>4))*8+3);
		g_NESmapper.set_PPU_bank4((((g_NESmapper.Mapper234.regs[0] & 0x0E)<<2)|((g_NESmapper.Mapper234.regs[1] & 0x70)>>4))*8+4);
		g_NESmapper.set_PPU_bank5((((g_NESmapper.Mapper234.regs[0] & 0x0E)<<2)|((g_NESmapper.Mapper234.regs[1] & 0x70)>>4))*8+5);
		g_NESmapper.set_PPU_bank6((((g_NESmapper.Mapper234.regs[0] & 0x0E)<<2)|((g_NESmapper.Mapper234.regs[1] & 0x70)>>4))*8+6);
		g_NESmapper.set_PPU_bank7((((g_NESmapper.Mapper234.regs[0] & 0x0E)<<2)|((g_NESmapper.Mapper234.regs[1] & 0x70)>>4))*8+7);
	}
	else
	{
		g_NESmapper.set_CPU_bank4((g_NESmapper.Mapper234.regs[0] & 0x0F)*4+0);
		g_NESmapper.set_CPU_bank5((g_NESmapper.Mapper234.regs[0] & 0x0F)*4+1);
		g_NESmapper.set_CPU_bank6((g_NESmapper.Mapper234.regs[0] & 0x0F)*4+2);
		g_NESmapper.set_CPU_bank7((g_NESmapper.Mapper234.regs[0] & 0x0F)*4+3);
		g_NESmapper.set_PPU_bank0((((g_NESmapper.Mapper234.regs[0] & 0x0F)<<2)|((g_NESmapper.Mapper234.regs[1] & 0x30)>>4))*8+0);
		g_NESmapper.set_PPU_bank1((((g_NESmapper.Mapper234.regs[0] & 0x0F)<<2)|((g_NESmapper.Mapper234.regs[1] & 0x30)>>4))*8+1);
		g_NESmapper.set_PPU_bank2((((g_NESmapper.Mapper234.regs[0] & 0x0F)<<2)|((g_NESmapper.Mapper234.regs[1] & 0x30)>>4))*8+2);
		g_NESmapper.set_PPU_bank3((((g_NESmapper.Mapper234.regs[0] & 0x0F)<<2)|((g_NESmapper.Mapper234.regs[1] & 0x30)>>4))*8+3);
		g_NESmapper.set_PPU_bank4((((g_NESmapper.Mapper234.regs[0] & 0x0F)<<2)|((g_NESmapper.Mapper234.regs[1] & 0x30)>>4))*8+4);
		g_NESmapper.set_PPU_bank5((((g_NESmapper.Mapper234.regs[0] & 0x0F)<<2)|((g_NESmapper.Mapper234.regs[1] & 0x30)>>4))*8+5);
		g_NESmapper.set_PPU_bank6((((g_NESmapper.Mapper234.regs[0] & 0x0F)<<2)|((g_NESmapper.Mapper234.regs[1] & 0x30)>>4))*8+6);
		g_NESmapper.set_PPU_bank7((((g_NESmapper.Mapper234.regs[0] & 0x0F)<<2)|((g_NESmapper.Mapper234.regs[1] & 0x30)>>4))*8+7);
	}
}
/////////////////////////////////////////////////////////////////////

