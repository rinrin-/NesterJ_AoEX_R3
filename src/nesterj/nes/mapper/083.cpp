
/////////////////////////////////////////////////////////////////////
// Mapper 83
void NES_mapper83_Init()
{
	g_NESmapper.Reset = NES_mapper83_Reset;
	g_NESmapper.MemoryWrite = NES_mapper83_MemoryWrite;
	g_NESmapper.HSync = NES_mapper83_HSync;
}

void NES_mapper83_Reset()
{
	g_NESmapper.Mapper83.regs[0] = g_NESmapper.Mapper83.regs[1] = g_NESmapper.Mapper83.regs[2] = 0;

	// set CPU bank pointers
	if(g_NESmapper.num_8k_ROM_banks >= 32)
	{
		g_NESmapper.set_CPU_banks4(0,1,30,31);
		g_NESmapper.Mapper83.regs[1] = 0x30;
	}
	else
	{
		g_NESmapper.set_CPU_banks4(0,1,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);
	}

	// set PPU bank pointers
	if(g_NESmapper.num_1k_VROM_banks)
	{
		g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);
	}

	g_NESmapper.Mapper83.irq_enabled = 0;
	g_NESmapper.Mapper83.irq_counter = 0;
}

uint8 NES_mapper83_MemoryReadLow(uint32 addr)
{
	if((addr & 0x5100) == 0x5100)
	{
		return g_NESmapper.Mapper83.regs[2];
	}
	else
	{
		return addr >> 8;
	}
}

void NES_mapper83_MemoryWriteLow(uint32 addr, uint8 data)
{
	switch(addr)
	{
	case 0x5101:
	case 0x5102:
	case 0x5103:
		{
			g_NESmapper.Mapper83.regs[2] = data;
		}
		break;
	}
}

void NES_mapper83_MemoryWrite(uint32 addr, uint8 data)
{
	switch(addr)
	{
	case 0x8000:
	case 0xB000:
	case 0xB0FF:
	case 0xB1FF:
		{
			g_NESmapper.Mapper83.regs[0] = data;
			g_NESmapper.set_CPU_bank4((data&0x3f)*2+0);
			g_NESmapper.set_CPU_bank5((data&0x3f)*2+1);
			g_NESmapper.set_CPU_bank6(((data&0x30)|0x0F)*2+0);
			g_NESmapper.set_CPU_bank7(((data&0x30)|0x0F)*2+1);

		}
		break;

	case 0x8100:
		{
			/*        if(g_NESmapper.num_1k_VROM_banks <= 32*8)
			        {
			          g_NESmapper.Mapper83.regs[1] = data;
			        }
			*/
			if((data & 0x03) == 0x00)
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
			}
			else if((data & 0x03) == 0x01)
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
			}
			else if((data & 0x03) == 0x02)
			{
				g_NESmapper.set_mirroring(0,0,0,0);
			}
			else
			{
				g_NESmapper.set_mirroring(1,1,1,1);
			}
		}
		break;

	case 0x8200:
		{
			g_NESmapper.Mapper83.irq_counter = (g_NESmapper.Mapper83.irq_counter & 0xFF00) | (uint32)data;
		}
		break;

	case 0x8201:
		{
			g_NESmapper.Mapper83.irq_counter = (g_NESmapper.Mapper83.irq_counter & 0x00FF) | ((uint32)data << 8);
			g_NESmapper.Mapper83.irq_enabled = data;
		}
		break;

	case 0x8300:
		{
			g_NESmapper.set_CPU_bank4(data);
		}
		break;

	case 0x8301:
		{
			g_NESmapper.set_CPU_bank5(data);
		}
		break;

	case 0x8302:
		{
			g_NESmapper.set_CPU_bank6(data);
		}
		break;

	case 0x8310:
		{
			g_NESmapper.set_PPU_bank0((uint32)((g_NESmapper.Mapper83.regs[0]&0x30)<<4)|data);
			/*
			        if((g_NESmapper.Mapper83.regs[1] & 0x30) == 0x30)
			        {
			          set_PPU_bank0((uint32)((data & 0x30) << 4)^data);
			        }
			        else if((g_NESmapper.Mapper83.regs[1] & 0x30) == 0x10 || (g_NESmapper.Mapper83.regs[1] & 0x30) == 0x20)
			        {
			          set_PPU_bank0((((data & 0x30) << 4)^data)*2+0);
			          set_PPU_bank1((((data & 0x30) << 4)^data)*2+1);
			        }
				*/
		}
		break;

	case 0x8311:
		{
			g_NESmapper.set_PPU_bank1((uint32)((g_NESmapper.Mapper83.regs[0]&0x30)<<4)|data);
			/*
			        if((g_NESmapper.Mapper83.regs[1] & 0x30) == 0x30)
			        {
			          set_PPU_bank1((uint32)((data & 0x30) << 4)^data);
			        }
			        else if((g_NESmapper.Mapper83.regs[1] & 0x30) == 0x10 || (g_NESmapper.Mapper83.regs[1] & 0x30) == 0x20)
			        {
			          set_PPU_bank2((((data & 0x30) << 4)^data)*2+0);
			          set_PPU_bank3((((data & 0x30) << 4)^data)*2+1);
			        }
				*/
		}
		break;

	case 0x8312:
		{
			g_NESmapper.set_PPU_bank2((uint32)((g_NESmapper.Mapper83.regs[0]&0x30)<<4)|data);
			/*
			        if((g_NESmapper.Mapper83.regs[1] & 0x30) == 0x30)
			        {
			          set_PPU_bank2((uint32)((data & 0x30) << 4)^data);
			        }
			        else if((g_NESmapper.Mapper83.regs[1] & 0x30) == 0x10 || (g_NESmapper.Mapper83.regs[1] & 0x30) == 0x20)
			        {
			          set_PPU_bank4((((data & 0x30) << 4)^data)*2+0);
			          set_PPU_bank5((((data & 0x30) << 4)^data)*2+1);
			        }
				*/
		}
		break;

	case 0x8313:
		{
			g_NESmapper.set_PPU_bank3((uint32)((g_NESmapper.Mapper83.regs[0]&0x30)<<4)|data);
			/*
					if((g_NESmapper.Mapper83.regs[1] & 0x30) == 0x30)
			        {
			          set_PPU_bank3((uint32)((data & 0x30) << 4)^data);
			        }
			        else if((g_NESmapper.Mapper83.regs[1] & 0x30) == 0x10 || (g_NESmapper.Mapper83.regs[1] & 0x30) == 0x20)
			        {
			          set_PPU_bank6((((data & 0x30) << 4)^data)*2+0);
			          set_PPU_bank7((((data & 0x30) << 4)^data)*2+1);
			        }
				*/
		}
		break;

	case 0x8314:
		{
			g_NESmapper.set_PPU_bank4((uint32)((g_NESmapper.Mapper83.regs[0]&0x30)<<4)|data);
			/*
			        if((g_NESmapper.Mapper83.regs[1] & 0x30) == 0x30)
			        {
			          set_PPU_bank4((uint32)((data & 0x30) << 4)^data);
			        }
			        else if((g_NESmapper.Mapper83.regs[1] & 0x30) == 0x10 || (g_NESmapper.Mapper83.regs[1] & 0x30) == 0x20)
			        {
			          set_PPU_bank4((((data & 0x30) << 4)^data)*2+0);
			          set_PPU_bank5((((data & 0x30) << 4)^data)*2+1);
			        }
				*/
		}
		break;

	case 0x8315:
		{
			g_NESmapper.set_PPU_bank5((uint32)((g_NESmapper.Mapper83.regs[0]&0x30)<<4)|data);
			/*
					if((g_NESmapper.Mapper83.regs[1] & 0x30) == 0x30)
			        {
			          set_PPU_bank5((uint32)((data & 0x30) << 4)^data);
			        }
			        else if((g_NESmapper.Mapper83.regs[1] & 0x30) == 0x10 || (g_NESmapper.Mapper83.regs[1] & 0x30) == 0x20)
			        {
			          set_PPU_bank6((((data & 0x30) << 4)^data)*2+0);
			          set_PPU_bank7((((data & 0x30) << 4)^data)*2+1);
			        }
				*/
		}
		break;

	case 0x8316:
		{
			g_NESmapper.set_PPU_bank6((uint32)((g_NESmapper.Mapper83.regs[0]&0x30)<<4)|data);
			/*
					if((g_NESmapper.Mapper83.regs[1] & 0x30) == 0x30)
			        {
			          set_PPU_bank6(((data & 0x30) << 4)^data);
			        }
			        else if((g_NESmapper.Mapper83.regs[1] & 0x30) == 0x10 || (g_NESmapper.Mapper83.regs[1] & 0x30) == 0x20)
			        {
			          set_PPU_bank4((((data & 0x30) << 4)^data)*2+0);
			          set_PPU_bank5((((data & 0x30) << 4)^data)*2+1);
			        }
					*/
		}
		break;

	case 0x8317:
		{
			g_NESmapper.set_PPU_bank7((uint32)((g_NESmapper.Mapper83.regs[0]&0x30)<<4)|data);
			/*
			        if((g_NESmapper.Mapper83.regs[1] & 0x30) == 0x30)
			        {
			          set_PPU_bank7(((data & 0x30) << 4)^data);
			        }
			        else if((g_NESmapper.Mapper83.regs[1] & 0x30) == 0x10 || (g_NESmapper.Mapper83.regs[1] & 0x30) == 0x20)
			        {
			          set_PPU_bank6((((data & 0x30) << 4)^data)*2+0);
			          set_PPU_bank7((((data & 0x30) << 4)^data)*2+1);
			        }
			*/
		}
		break;

	case 0x8318:
		{
			g_NESmapper.set_CPU_bank4(((g_NESmapper.Mapper83.regs[0]&0x30)|data)*2+0);
			g_NESmapper.set_CPU_bank5(((g_NESmapper.Mapper83.regs[0]&0x30)|data)*2+1);
		}
		break;
	}
	LOG("W " << HEX(addr,4) << "  " << HEX(data,2) << " REG "<< HEX(g_NESmapper.Mapper83.regs[0],2) <<endl);
}

void NES_mapper83_HSync(uint32 scanline)
{
	if(g_NESmapper.Mapper83.irq_enabled)
	{
		if(g_NESmapper.Mapper83.irq_counter <= 113)
		{
			NES6502_DoIRQ();
			g_NESmapper.Mapper83.irq_enabled = 0;
		}
		else
		{
			g_NESmapper.Mapper83.irq_counter -= 113;
		}
	}
}
/////////////////////////////////////////////////////////////////////

