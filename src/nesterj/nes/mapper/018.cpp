
/////////////////////////////////////////////////////////////////////
// Mapper 18
void NES_mapper18_Init()
{
	g_NESmapper.Reset = NES_mapper18_Reset;
	g_NESmapper.MemoryWrite = NES_mapper18_MemoryWrite;
	g_NESmapper.HSync = NES_mapper18_HSync;
}

void NES_mapper18_Reset()
{
	if(NES_crc32() == 0x424ec0a6)   // Jajamaru Gekimaden - Maboroshi no Kinmajou (J)
	{
		g_NESmapper.Mapper18.patch = 1;
	}

	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);

	g_NESmapper.Mapper18.regs[0] = 0;
	g_NESmapper.Mapper18.regs[1] = 1;
	g_NESmapper.Mapper18.regs[2] = g_NESmapper.num_8k_ROM_banks-2;
	g_NESmapper.Mapper18.regs[3] = g_NESmapper.num_8k_ROM_banks-1;
	g_NESmapper.Mapper18.regs[4] = 0;
	g_NESmapper.Mapper18.regs[5] = 0;
	g_NESmapper.Mapper18.regs[6] = 0;
	g_NESmapper.Mapper18.regs[7] = 0;
	g_NESmapper.Mapper18.regs[8] = 0;
	g_NESmapper.Mapper18.regs[9] = 0;
	g_NESmapper.Mapper18.regs[10] = 0;

	g_NESmapper.Mapper18.irq_enabled = 0;
	g_NESmapper.Mapper18.irq_latch = 0;
	g_NESmapper.Mapper18.irq_counter = 0;
}

void NES_mapper18_MemoryWrite(uint32 addr, uint8 data)
{
	// g_NESmapper.Mapper18.regs[ 0] ... 8K PROM bank at CPU $8000
	// g_NESmapper.Mapper18.regs[ 1] ... 8K PROM bank at CPU $A000
	// g_NESmapper.Mapper18.regs[ 2] ... 8K PROM bank at CPU $C000
	// g_NESmapper.Mapper18.regs[ 3] ... 1K VROM bank at PPU $0000
	// g_NESmapper.Mapper18.regs[ 4] ... 1K VROM bank at PPU $0400
	// g_NESmapper.Mapper18.regs[ 5] ... 1K VROM bank at PPU $0800
	// g_NESmapper.Mapper18.regs[ 6] ... 1K VROM bank at PPU $0C00
	// g_NESmapper.Mapper18.regs[ 7] ... 1K VROM bank at PPU $1000
	// g_NESmapper.Mapper18.regs[ 8] ... 1K VROM bank at PPU $1400
	// g_NESmapper.Mapper18.regs[ 9] ... 1K VROM bank at PPU $1800
	// g_NESmapper.Mapper18.regs[10] ... 1K VROM bank at PPU $1C00

	switch(addr)
	{
	case 0x8000:
		{
			g_NESmapper.Mapper18.regs[0] = (g_NESmapper.Mapper18.regs[0] & 0xF0) | (data & 0x0F);
			g_NESmapper.set_CPU_bank4(g_NESmapper.Mapper18.regs[0]);
		}
		break;

	case 0x8001:
		{
			g_NESmapper.Mapper18.regs[0] = (g_NESmapper.Mapper18.regs[0] & 0x0F) | ((data & 0x0F) << 4);
			g_NESmapper.set_CPU_bank4(g_NESmapper.Mapper18.regs[0]);
		}
		break;

	case 0x8002:
		{
			g_NESmapper.Mapper18.regs[1] = (g_NESmapper.Mapper18.regs[1] & 0xF0) | (data & 0x0F);
			g_NESmapper.set_CPU_bank5(g_NESmapper.Mapper18.regs[1]);
		}
		break;

	case 0x8003:
		{
			g_NESmapper.Mapper18.regs[1] = (g_NESmapper.Mapper18.regs[1] & 0x0F) | ((data & 0x0F) << 4);
			g_NESmapper.set_CPU_bank5(g_NESmapper.Mapper18.regs[1]);
		}
		break;

	case 0x9000:
		{
			g_NESmapper.Mapper18.regs[2] = (g_NESmapper.Mapper18.regs[2] & 0xF0) | (data & 0x0F);
			g_NESmapper.set_CPU_bank6(g_NESmapper.Mapper18.regs[2]);
		}
		break;

	case 0x9001:
		{
			g_NESmapper.Mapper18.regs[2] = (g_NESmapper.Mapper18.regs[2] & 0x0F) | ((data & 0x0F) << 4);
			g_NESmapper.set_CPU_bank6(g_NESmapper.Mapper18.regs[2]);
		}
		break;

	case 0xA000:
		{
			g_NESmapper.Mapper18.regs[3] = (g_NESmapper.Mapper18.regs[3] & 0xF0) | (data & 0x0F);
			g_NESmapper.set_PPU_bank0(g_NESmapper.Mapper18.regs[3]);
		}
		break;

	case 0xA001:
		{
			g_NESmapper.Mapper18.regs[3] = (g_NESmapper.Mapper18.regs[3] & 0x0F) | ((data & 0x0F) << 4);
			g_NESmapper.set_PPU_bank0(g_NESmapper.Mapper18.regs[3]);
		}
		break;

	case 0xA002:
		{
			g_NESmapper.Mapper18.regs[4] = (g_NESmapper.Mapper18.regs[4] & 0xF0) | (data & 0x0F);
			g_NESmapper.set_PPU_bank1(g_NESmapper.Mapper18.regs[4]);
		}
		break;

	case 0xA003:
		{
			g_NESmapper.Mapper18.regs[4] = (g_NESmapper.Mapper18.regs[4] & 0x0F) | ((data & 0x0F) << 4);
			g_NESmapper.set_PPU_bank1(g_NESmapper.Mapper18.regs[4]);
		}
		break;

	case 0xB000:
		{
			g_NESmapper.Mapper18.regs[5] = (g_NESmapper.Mapper18.regs[5] & 0xF0) | (data & 0x0F);
			g_NESmapper.set_PPU_bank2(g_NESmapper.Mapper18.regs[5]);
		}
		break;

	case 0xB001:
		{
			g_NESmapper.Mapper18.regs[5] = (g_NESmapper.Mapper18.regs[5] & 0x0F) | ((data & 0x0F) << 4);
			g_NESmapper.set_PPU_bank2(g_NESmapper.Mapper18.regs[5]);
		}
		break;

	case 0xB002:
		{
			g_NESmapper.Mapper18.regs[6] = (g_NESmapper.Mapper18.regs[6] & 0xF0) | (data & 0x0F);
			g_NESmapper.set_PPU_bank3(g_NESmapper.Mapper18.regs[6]);
		}
		break;

	case 0xB003:
		{
			g_NESmapper.Mapper18.regs[6] = (g_NESmapper.Mapper18.regs[6] & 0x0F) | ((data & 0x0F) << 4);
			g_NESmapper.set_PPU_bank3(g_NESmapper.Mapper18.regs[6]);
		}
		break;

	case 0xC000:
		{
			g_NESmapper.Mapper18.regs[7] = (g_NESmapper.Mapper18.regs[7] & 0xF0) | (data & 0x0F);
			g_NESmapper.set_PPU_bank4(g_NESmapper.Mapper18.regs[7]);
		}
		break;

	case 0xC001:
		{
			g_NESmapper.Mapper18.regs[7] = (g_NESmapper.Mapper18.regs[7] & 0x0F) | ((data & 0x0F) << 4);
			g_NESmapper.set_PPU_bank4(g_NESmapper.Mapper18.regs[7]);
		}
		break;

	case 0xC002:
		{
			g_NESmapper.Mapper18.regs[8] = (g_NESmapper.Mapper18.regs[8] & 0xF0) | (data & 0x0F);
			g_NESmapper.set_PPU_bank5(g_NESmapper.Mapper18.regs[8]);
		}
		break;

	case 0xC003:
		{
			g_NESmapper.Mapper18.regs[8] = (g_NESmapper.Mapper18.regs[8] & 0x0F) | ((data & 0x0F) << 4);
			g_NESmapper.set_PPU_bank5(g_NESmapper.Mapper18.regs[8]);
		}
		break;

	case 0xD000:
		{
			g_NESmapper.Mapper18.regs[9] = (g_NESmapper.Mapper18.regs[9] & 0xF0) | (data & 0x0F);
			g_NESmapper.set_PPU_bank6(g_NESmapper.Mapper18.regs[9]);
		}
		break;

	case 0xD001:
		{
			g_NESmapper.Mapper18.regs[9] = (g_NESmapper.Mapper18.regs[9] & 0x0F) | ((data & 0x0F) << 4);
			g_NESmapper.set_PPU_bank6(g_NESmapper.Mapper18.regs[9]);
		}
		break;

	case 0xD002:
		{
			g_NESmapper.Mapper18.regs[10] = (g_NESmapper.Mapper18.regs[10] & 0xF0) | (data & 0x0F);
			g_NESmapper.set_PPU_bank7(g_NESmapper.Mapper18.regs[10]);
		}
		break;

	case 0xD003:
		{
			g_NESmapper.Mapper18.regs[10] = (g_NESmapper.Mapper18.regs[10] & 0x0F) | ((data & 0x0F) << 4);
			g_NESmapper.set_PPU_bank7(g_NESmapper.Mapper18.regs[10]);
		}
		break;

	case 0xE000:
		{
			g_NESmapper.Mapper18.irq_latch = (g_NESmapper.Mapper18.irq_latch & 0xFFF0) | (data & 0x0F);
		}
		break;

	case 0xE001:
		{
			g_NESmapper.Mapper18.irq_latch = (g_NESmapper.Mapper18.irq_latch & 0xFF0F) | ((data & 0x0F) << 4);
		}
		break;

	case 0xE002:
		{
			g_NESmapper.Mapper18.irq_latch = (g_NESmapper.Mapper18.irq_latch & 0xF0FF) | ((data & 0x0F) << 8);
		}
		break;

	case 0xE003:
		{
			g_NESmapper.Mapper18.irq_latch = (g_NESmapper.Mapper18.irq_latch & 0x0FFF) | ((data & 0x0F) << 12);
		}
		break;

	case 0xF000:
		{
			g_NESmapper.Mapper18.irq_counter = g_NESmapper.Mapper18.irq_latch;
		}
		break;

	case 0xF001:
		{
			g_NESmapper.Mapper18.irq_enabled = data & 0x01;
		}
		break;

	case 0xF002:
		{
			data &= 0x03;
			if(data == 0)
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
			}
			else if(data == 1)
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
			}
			else
			{
				g_NESmapper.set_mirroring(0,0,0,0);
			}
		}
		break;
	}
}

void NES_mapper18_HSync(uint32 scanline)
{
	if(g_NESmapper.Mapper18.irq_enabled)
	{
		if(g_NESmapper.Mapper18.irq_counter <= 113)
		{
			NES6502_DoIRQ();
			g_NESmapper.Mapper18.irq_counter = (g_NESmapper.Mapper18.patch == 1) ? 114 : 0;
			g_NESmapper.Mapper18.irq_enabled = 0;
		}
		else
		{
			g_NESmapper.Mapper18.irq_counter -= 113;
		}
	}
}

#define MAP18_ROM(ptr)  (((ptr)-NES_ROM_get_ROM_banks())  >> 13)
#define MAP18_VROM(ptr) (((ptr)-NES_ROM_get_VROM_banks()) >> 10)

void NES_mapper18_SNSS_fixup()
{
	nes6502_context context;
	NES6502_GetContext(&context);
	g_NESmapper.Mapper18.regs[0] = MAP18_ROM(context.mem_page[4]);
	g_NESmapper.Mapper18.regs[1] = MAP18_ROM(context.mem_page[5]);
	g_NESmapper.Mapper18.regs[2] = MAP18_ROM(context.mem_page[6]);
	g_NESmapper.Mapper18.regs[3] = MAP18_VROM(g_PPU.PPU_VRAM_banks[0]);
	g_NESmapper.Mapper18.regs[4] = MAP18_VROM(g_PPU.PPU_VRAM_banks[1]);
	g_NESmapper.Mapper18.regs[5] = MAP18_VROM(g_PPU.PPU_VRAM_banks[2]);
	g_NESmapper.Mapper18.regs[6] = MAP18_VROM(g_PPU.PPU_VRAM_banks[3]);
	g_NESmapper.Mapper18.regs[7] = MAP18_VROM(g_PPU.PPU_VRAM_banks[4]);
	g_NESmapper.Mapper18.regs[8] = MAP18_VROM(g_PPU.PPU_VRAM_banks[5]);
	g_NESmapper.Mapper18.regs[9] = MAP18_VROM(g_PPU.PPU_VRAM_banks[6]);
	g_NESmapper.Mapper18.regs[10] = MAP18_VROM(g_PPU.PPU_VRAM_banks[7]);
}
/////////////////////////////////////////////////////////////////////

