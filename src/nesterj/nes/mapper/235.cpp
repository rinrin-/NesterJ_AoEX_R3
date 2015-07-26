
/////////////////////////////////////////////////////////////////////
// Mapper 235
void NES_mapper235_Init()
{
	g_NESmapper.Reset = NES_mapper235_Reset;
	g_NESmapper.MemoryWrite = NES_mapper235_MemoryWrite;
}

void NES_mapper235_Reset()
{
	// set dummy data for open bus
	uint32 i;
	for(i = 0; i < 0x2000; i++)
	{
		g_NESmapper.Mapper235.dummy[i] = 0xFF;
	}

	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,2,3);
}

void NES_mapper235_MemoryWrite(uint32 addr, uint8 data)
{
	uint8 prg_bank = ((addr & 0x0300) >> 3) | (addr & 0x001F);
	uint8 open_bus = 0;

	if (g_NESmapper.num_8k_ROM_banks == 64*2)
	{
		// 100-in-1
		switch (addr & 0x0300)
		{
		case 0x0000: break;
		case 0x0100: open_bus = 1; break;
		case 0x0200: open_bus = 1; break;
		case 0x0300: open_bus = 1; break;
		}
	}
	else if (g_NESmapper.num_8k_ROM_banks == 128*2)
	{
		// 150-in-1
		switch (addr & 0x0300)
		{
		case 0x0000: break;
		case 0x0100: open_bus = 1; break;
		case 0x0200: prg_bank = (prg_bank & 0x1F) | 0x20; break;
		case 0x0300: open_bus = 1; break;
		}
	}
	else if (g_NESmapper.num_8k_ROM_banks == 192*2)
	{
		// 210-in-1
		switch (addr & 0x0300)
		{
		case 0x0000: break;
		case 0x0100: open_bus = 1; break;
		case 0x0200: prg_bank = (prg_bank & 0x1F) | 0x20; break;
		case 0x0300: prg_bank = (prg_bank & 0x1F) | 0x40; break;
		}
	}
	else if (g_NESmapper.num_8k_ROM_banks == 256*2)
	{
		// 260-in-1
	}
	if(addr & 0x0800)
	{
		if(addr & 0x1000)
		{
			// upper 16KB Banks
			g_NESmapper.set_CPU_bank4(prg_bank*4+2);
			g_NESmapper.set_CPU_bank5(prg_bank*4+3);
			g_NESmapper.set_CPU_bank6(prg_bank*4+2);
			g_NESmapper.set_CPU_bank7(prg_bank*4+3);
		}
		else
		{
			// lower 16KB Banks
			g_NESmapper.set_CPU_bank4(prg_bank*4+0);
			g_NESmapper.set_CPU_bank5(prg_bank*4+1);
			g_NESmapper.set_CPU_bank6(prg_bank*4+0);
			g_NESmapper.set_CPU_bank7(prg_bank*4+1);
		}
	}
	else
	{
		// 32KB Banks
		g_NESmapper.set_CPU_bank4(prg_bank*4+0);
		g_NESmapper.set_CPU_bank5(prg_bank*4+1);
		g_NESmapper.set_CPU_bank6(prg_bank*4+2);
		g_NESmapper.set_CPU_bank7(prg_bank*4+3);
	}
	if(open_bus)
	{
		nes6502_context context;
		NES6502_GetContext(&context);
		context.mem_page[4] = g_NESmapper.Mapper235.dummy;
		context.mem_page[5] = g_NESmapper.Mapper235.dummy;
		context.mem_page[6] = g_NESmapper.Mapper235.dummy;
		context.mem_page[7] = g_NESmapper.Mapper235.dummy;
		NES6502_SetContext(&context);
	}
	if(addr & 0x0400)
	{
		g_NESmapper.set_mirroring(0,0,0,0);
	}
	else if(addr & 0x2000)
	{
		g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
	}
	else
	{
		g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
	}
}
/////////////////////////////////////////////////////////////////////

