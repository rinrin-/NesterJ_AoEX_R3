
/////////////////////////////////////////////////////////////////////
// Mapper 160
void NES_mapper160_Init()
{
	g_NESmapper.Reset = NES_mapper160_Reset;
	g_NESmapper.MemoryWrite = NES_mapper160_MemoryWrite;
	g_NESmapper.HSync = NES_mapper160_HSync;
}

void NES_mapper160_Reset()
{
	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);

	g_NESmapper.Mapper160.irq_enabled = 0;
	g_NESmapper.Mapper160.irq_counter = 0;
	g_NESmapper.Mapper160.irq_latch = 0;
	g_NESmapper.Mapper160.refresh_type = 0;
}

void NES_mapper160_MemoryWrite(uint32 addr, uint8 data)
{
	switch(addr)
	{
	case 0x8000:
		{
			g_NESmapper.set_CPU_bank4(data);
		}
		break;

	case 0x8001:
		{
			g_NESmapper.set_CPU_bank5(data);
		}
		break;

	case 0x8002:
		{
			g_NESmapper.set_CPU_bank6(data);
		}
		break;

	case 0x9000:
		{
			if(data == 0x2B)
			{
				g_NESmapper.Mapper160.refresh_type = 1; // title
			}
			else if(data == 0xA8)
			{
				g_NESmapper.Mapper160.refresh_type = 2; // pass word
			}
			else if(data == 0x1F)
			{
				g_NESmapper.Mapper160.refresh_type = 3; // game over
			}
			else if(data == 0x7C)
			{
				g_NESmapper.Mapper160.refresh_type = 4; // continue
			}
			else if(data == 0x18)
			{
				g_NESmapper.Mapper160.refresh_type = 5; // roulette
			}
			else if(data == 0x60)
			{
				g_NESmapper.Mapper160.refresh_type = 6; // congratulation
			}
			else
			{
				g_NESmapper.Mapper160.refresh_type = 0;
			}
			g_NESmapper.set_PPU_bank0(data);
		}
		break;

	case 0x9001:
		{
			g_NESmapper.set_PPU_bank1(data);
		}
		break;

	case 0x9002:
		{
			if(g_NESmapper.Mapper160.refresh_type == 2 && data != 0xE8)
			{
				g_NESmapper.Mapper160.refresh_type = 0; // not pass word
			}
			g_NESmapper.set_PPU_bank2(data);
		}
		break;

	case 0x9003:
		{
			g_NESmapper.set_PPU_bank3(data);
		}
		break;

	case 0x9004:
		{
			g_NESmapper.set_PPU_bank4(data);
		}
		break;

	case 0x9005:
		{
			g_NESmapper.set_PPU_bank5(data);
		}
		break;

	case 0x9006:
		{
			g_NESmapper.set_PPU_bank6(data);
		}
		break;

	case 0x9007:
		{
			g_NESmapper.set_PPU_bank7(data);
		}
		break;

	case 0xC000:
		{
			g_NESmapper.Mapper160.irq_counter = g_NESmapper.Mapper160.irq_latch;
			g_NESmapper.Mapper160.irq_enabled = g_NESmapper.Mapper160.irq_latch;
		}
		break;

	case 0xC001:
		{
			g_NESmapper.Mapper160.irq_latch = data;
		}
		break;

	case 0xC002:
		{
			g_NESmapper.Mapper160.irq_enabled = 0;
		}
		break;

	case 0xC003:
		{
			g_NESmapper.Mapper160.irq_counter = data;
		}
		break;
	}
}

void NES_mapper160_HSync(uint32 scanline)
{
	if(scanline == 0 || scanline == 239)
	{
		if(g_NESmapper.Mapper160.refresh_type == 1)
		{
			g_NESmapper.set_PPU_banks8(0x58,0x59,0x5A,0x5B,0x58,0x59,0x5A,0x5B);
		}
		else if(g_NESmapper.Mapper160.refresh_type == 2)
		{
			g_NESmapper.set_PPU_banks8(0x78,0x79,0x7A,0x7B,0x78,0x79,0x7A,0x7B);
		}
		else if(g_NESmapper.Mapper160.refresh_type == 3)
		{
			g_NESmapper.set_PPU_banks8(0x7C,0x7D,0x7E,0x7F,0x7C,0x7D,0x7E,0x7F);
		}
		else if(g_NESmapper.Mapper160.refresh_type == 5)
		{
			g_NESmapper.set_PPU_banks8(0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77);
		}
		else if(g_NESmapper.Mapper160.refresh_type == 6)
		{
			g_NESmapper.set_PPU_banks8(0x5C,0x5D,0x5E,0x5F,0x7C,0x7D,0x7E,0x7F);
		}
	}
	if(scanline == 64)
	{
		if(g_NESmapper.Mapper160.refresh_type == 4)
		{
			// face of lamp's spirit
			if(g_PPU.PPU_VRAM_banks[8][32*10+16] == 0x0A)
			{
				g_NESmapper.set_PPU_bank0(0x68);
				g_NESmapper.set_PPU_bank1(0x69);
				g_NESmapper.set_PPU_bank2(0x6A);
				g_NESmapper.set_PPU_bank3(0x6B);
			}else{
				g_NESmapper.set_PPU_bank0(0x6C);
				g_NESmapper.set_PPU_bank1(0x6D);
				g_NESmapper.set_PPU_bank2(0x6E);
				g_NESmapper.set_PPU_bank3(0x6F);
			}
		}
	}
	if(scanline == 128)
	{
		if(g_NESmapper.Mapper160.refresh_type == 4)
		{
			g_NESmapper.set_PPU_bank0(0x68);
			g_NESmapper.set_PPU_bank1(0x69);
			g_NESmapper.set_PPU_bank2(0x6A);
			g_NESmapper.set_PPU_bank3(0x6B);
		}
		else if(g_NESmapper.Mapper160.refresh_type == 5)
		{
			g_NESmapper.set_PPU_banks8(0x74,0x75,0x76,0x77,0x74,0x75,0x76,0x77);
		}
	}
	if(scanline == 160)
	{
		if(g_NESmapper.Mapper160.refresh_type == 6)
		{
			g_NESmapper.set_PPU_banks8(0x60,0x61,0x5E,0x5F,0x7C,0x7D,0x7E,0x7F);
		}
	}
	if(g_NESmapper.Mapper160.irq_enabled)
	{
		if(g_NESmapper.Mapper160.irq_counter == 0xFF)
		{
			NES6502_DoIRQ();
			g_NESmapper.Mapper160.irq_counter = 0;
			g_NESmapper.Mapper160.irq_enabled = 0;
		}
		else
		{
			g_NESmapper.Mapper160.irq_counter++;
		}
	}
}
/////////////////////////////////////////////////////////////////////

