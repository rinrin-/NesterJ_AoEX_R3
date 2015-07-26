
/////////////////////////////////////////////////////////////////////
// Mapper 237
void NES_mapper237_Init()
{
	g_NESmapper.Reset = NES_mapper237_Reset;
	g_NESmapper.MemoryWrite = NES_mapper237_MemoryWrite;
}

void NES_mapper237_Reset()
{
	g_NESmapper.set_CPU_banks4(0,1,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);
	g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);
	g_NESmapper.Mapper237.wram = g_NES.mapper_extram;
	g_NES.mapper_extramsize = 0x8000;
}

void NES_mapper237_MemoryWrite(uint32 addr, uint8 data)
{
	if(addr & 0x4000)
	{
		if((addr & 0x0030) == 0x00)
		{
			g_NESmapper.set_CPU_bank4((addr&0x07)*2+0);
			g_NESmapper.set_CPU_bank5((addr&0x07)*2+1);
			g_NESmapper.set_CPU_bank6(g_NESmapper.num_8k_ROM_banks-2);
			g_NESmapper.set_CPU_bank7(g_NESmapper.num_8k_ROM_banks-1);
		}
		else if((addr & 0x0030) == 0x10)
		{
			uint32 i;
			nes6502_context context;
			for(i = 0; i < 0x2000; i++)
			{
				g_NESmapper.Mapper237.wram[i+0x0000] = g_NESmapper.ROM_banks[((addr&0x07)*2+0)*0x2000+(i&0x1ff0)+0x0d]; // 0x0b?
				g_NESmapper.Mapper237.wram[i+0x2000] = g_NESmapper.ROM_banks[((addr&0x07)*2+1)*0x2000+(i&0x1ff0)+0x0d]; // 0x0b?
				g_NESmapper.Mapper237.wram[i+0x4000] = g_NESmapper.ROM_banks[(g_NESmapper.num_8k_ROM_banks-2)*0x2000+(i&0x1ff0)+0x0d]; // 0x0b?
				g_NESmapper.Mapper237.wram[i+0x6000] = g_NESmapper.ROM_banks[(g_NESmapper.num_8k_ROM_banks-1)*0x2000+(i&0x1ff0)+0x0d]; // 0x0b?
			}
			NES6502_GetContext(&context);
			context.mem_page[4] = g_NESmapper.Mapper237.wram + 0x0000;
			context.mem_page[5] = g_NESmapper.Mapper237.wram + 0x2000;
			context.mem_page[6] = g_NESmapper.Mapper237.wram + 0x4000;
			context.mem_page[7] = g_NESmapper.Mapper237.wram + 0x6000;
			NES6502_SetContext(&context);
		}
		else if((addr & 0x0030) == 0x20)
		{
			g_NESmapper.set_CPU_bank4((addr&0x06)*2+0);
			g_NESmapper.set_CPU_bank5((addr&0x06)*2+1);
			g_NESmapper.set_CPU_bank6((addr&0x06)*2+2);
			g_NESmapper.set_CPU_bank7((addr&0x06)*2+3);
		}
		else if((addr & 0x0030) == 0x30)
		{
			g_NESmapper.set_CPU_bank4((addr&0x07)*2+0);
			g_NESmapper.set_CPU_bank5((addr&0x07)*2+1);
			g_NESmapper.set_CPU_bank6((addr&0x07)*2+0);
			g_NESmapper.set_CPU_bank7((addr&0x07)*2+1);
		}
	}
	else
	{
		if(addr & 0x0020)
		{
			g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
		}
		else
		{
			g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
		}
		g_NESmapper.set_PPU_bank0((addr&0x07)*8+0);
		g_NESmapper.set_PPU_bank1((addr&0x07)*8+1);
		g_NESmapper.set_PPU_bank2((addr&0x07)*8+2);
		g_NESmapper.set_PPU_bank3((addr&0x07)*8+3);
		g_NESmapper.set_PPU_bank4((addr&0x07)*8+4);
		g_NESmapper.set_PPU_bank5((addr&0x07)*8+5);
		g_NESmapper.set_PPU_bank6((addr&0x07)*8+6);
		g_NESmapper.set_PPU_bank7((addr&0x07)*8+7);
	}
}
/////////////////////////////////////////////////////////////////////

