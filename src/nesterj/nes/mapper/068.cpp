
/////////////////////////////////////////////////////////////////////
// Mapper 68
void NES_mapper68_Init()
{
	g_NESmapper.Reset = NES_mapper68_Reset;
	g_NESmapper.MemoryWrite = NES_mapper68_MemoryWrite;
}

void NES_mapper68_Reset()
{
	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);

	g_NESmapper.Mapper68.regs[0] = 0;
	g_NESmapper.Mapper68.regs[1] = 0;
	g_NESmapper.Mapper68.regs[2] = 0;
	g_NESmapper.Mapper68.regs[3] = 0;
}

void NES_mapper68_MemoryWrite(uint32 addr, uint8 data)
{
	switch(addr & 0xF000)
	{
	case 0x8000:
		{
			g_NESmapper.set_PPU_bank0(data*2+0);
			g_NESmapper.set_PPU_bank1(data*2+1);
		}
		break;

	case 0x9000:
		{
			g_NESmapper.set_PPU_bank2(data*2+0);
			g_NESmapper.set_PPU_bank3(data*2+1);
		}
		break;

	case 0xA000:
		{
			g_NESmapper.set_PPU_bank4(data*2+0);
			g_NESmapper.set_PPU_bank5(data*2+1);
		}
		break;

	case 0xB000:
		{
			g_NESmapper.set_PPU_bank6(data*2+0);
			g_NESmapper.set_PPU_bank7(data*2+1);
		}
		break;

	case 0xC000:
		{
			g_NESmapper.Mapper68.regs[2] = data;
			NES_mapper68_SyncMirror();
		}
		break;

	case 0xD000:
		{
			g_NESmapper.Mapper68.regs[3] = data;
			NES_mapper68_SyncMirror();
		}
		break;

	case 0xE000:
		{
			g_NESmapper.Mapper68.regs[0] = (data & 0x10) >> 4;
			g_NESmapper.Mapper68.regs[1] = data & 0x03;
			NES_mapper68_SyncMirror();
		}
		break;

	case 0xF000:
		{
			g_NESmapper.set_CPU_bank4(data*2);
			g_NESmapper.set_CPU_bank5(data*2+1);
		}
		break;
	}
}

void NES_mapper68_SyncMirror()
{
	if(g_NESmapper.Mapper68.regs[0])
	{
		if (g_NESmapper.Mapper68.regs[1] == 0)
		{
			g_NESmapper.set_PPU_bank8(g_NESmapper.Mapper68.regs[2] + 0x80);   // + 0x20000
			g_NESmapper.set_PPU_bank9(g_NESmapper.Mapper68.regs[3] + 0x80);
			g_NESmapper.set_PPU_bank10(g_NESmapper.Mapper68.regs[2] + 0x80);
			g_NESmapper.set_PPU_bank11(g_NESmapper.Mapper68.regs[3] + 0x80);
		}
		else if (g_NESmapper.Mapper68.regs[1] == 1)
		{
			g_NESmapper.set_PPU_bank8(g_NESmapper.Mapper68.regs[2] + 0x80);
			g_NESmapper.set_PPU_bank9(g_NESmapper.Mapper68.regs[2] + 0x80);
			g_NESmapper.set_PPU_bank10(g_NESmapper.Mapper68.regs[3] + 0x80);
			g_NESmapper.set_PPU_bank11(g_NESmapper.Mapper68.regs[3] + 0x80);
		}
		else if (g_NESmapper.Mapper68.regs[1] == 2)
		{
			g_NESmapper.set_PPU_bank8(g_NESmapper.Mapper68.regs[2] + 0x80);
			g_NESmapper.set_PPU_bank9(g_NESmapper.Mapper68.regs[2] + 0x80);
			g_NESmapper.set_PPU_bank10(g_NESmapper.Mapper68.regs[2] + 0x80);
			g_NESmapper.set_PPU_bank11(g_NESmapper.Mapper68.regs[2] + 0x80);
		}
		else if (g_NESmapper.Mapper68.regs[1] == 3)
		{
			g_NESmapper.set_PPU_bank8(g_NESmapper.Mapper68.regs[3] + 0x80);
			g_NESmapper.set_PPU_bank9(g_NESmapper.Mapper68.regs[3] + 0x80);
			g_NESmapper.set_PPU_bank10(g_NESmapper.Mapper68.regs[3] + 0x80);
			g_NESmapper.set_PPU_bank11(g_NESmapper.Mapper68.regs[3] + 0x80);
		}
	}
	else
	{
		if (g_NESmapper.Mapper68.regs[1] == 0)
		{
			g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
		}
		else if (g_NESmapper.Mapper68.regs[1] == 1)
		{
			g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
		}
		else if (g_NESmapper.Mapper68.regs[1] == 2)
		{
			g_NESmapper.set_mirroring(0,0,0,0);
		}
		else if (g_NESmapper.Mapper68.regs[1] == 3)
		{
			g_NESmapper.set_mirroring(1,1,1,1);
		}
	}
}
/////////////////////////////////////////////////////////////////////

