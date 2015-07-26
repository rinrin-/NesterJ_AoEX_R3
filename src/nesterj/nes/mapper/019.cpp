
/////////////////////////////////////////////////////////////////////
// Mapper 19
void NES_mapper19_Init()
{
	g_NESmapper.Reset = NES_mapper19_Reset;
	g_NESmapper.MemoryReadLow = NES_mapper19_MemoryReadLow;
	g_NESmapper.MemoryWriteLow = NES_mapper19_MemoryWriteLow;
	g_NESmapper.MemoryWrite = NES_mapper19_MemoryWrite;
	g_NESmapper.HSync = NES_mapper19_HSync;
}

void NES_mapper19_Reset()
{
	g_NESmapper.Mapper19.patch = 0;
	g_NESmapper.Mapper19.irq_sn=113;

	if(NES_crc32() == 0x761ccfb5 )//|| // Digital Devil Story - Megami Tensei 2
		//	  NES_crc32() == 0x3deac303)  // RollingThunder(J)
	{
		g_NESmapper.Mapper19.patch = 1;
		g_NESmapper.Mapper19.irq_sn = 112;
	}
	else if(NES_crc32() == 0x96533999 || // Dokuganryuu Masamune
	        NES_crc32() == 0x429fd177 || // Famista '90
	        NES_crc32() == 0xdd454208 || // Hydlide 3 - Yami Kara no Houmonsha (J).nes
	        NES_crc32() == 0xb1b9e187 || // Kaijuu Monogatari
	        NES_crc32() == 0xaf15338f)   // Mindseeker
	{
		g_NESmapper.Mapper19.patch = 2;
	}
	if(NES_crc32() == 0xb62a7b71) // Family Circuit '91(J)
	{
		g_NESmapper.Mapper19.patch = 3;
		g_NESmapper.Mapper19.irq_sn=100;
	}


	// Init ExSound
	NES_APU_SelectExSound(16);

	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);

	// set PPU bank pointers
	if(g_NESmapper.num_1k_VROM_banks >= 8)
	{
		g_NESmapper.set_PPU_bank0(g_NESmapper.num_1k_VROM_banks-8);
		g_NESmapper.set_PPU_bank1(g_NESmapper.num_1k_VROM_banks-7);
		g_NESmapper.set_PPU_bank2(g_NESmapper.num_1k_VROM_banks-6);
		g_NESmapper.set_PPU_bank3(g_NESmapper.num_1k_VROM_banks-5);
		g_NESmapper.set_PPU_bank4(g_NESmapper.num_1k_VROM_banks-4);
		g_NESmapper.set_PPU_bank5(g_NESmapper.num_1k_VROM_banks-3);
		g_NESmapper.set_PPU_bank6(g_NESmapper.num_1k_VROM_banks-2);
		g_NESmapper.set_PPU_bank7(g_NESmapper.num_1k_VROM_banks-1);
	}

	g_NESmapper.Mapper19.regs[0] = 0;
	g_NESmapper.Mapper19.regs[1] = 0;
	g_NESmapper.Mapper19.regs[2] = 0;
}

uint8 NES_mapper19_MemoryReadLow(uint32 addr)
{
	if(addr == 0x4800)
	{
		if(g_NESmapper.Mapper19.patch == 2)
		{
			uint8 retval =  NES_ReadSaveRAM(g_NESmapper.Mapper19.regs[2] & 0x7F);
			if(g_NESmapper.Mapper19.regs[2] & 0x80) g_NESmapper.Mapper19.regs[2] = ((g_NESmapper.Mapper19.regs[2] & 0x7F)+1) | 0x80;
			return retval;
		}
		else
		{
			return NES_APU_ExRead(addr);
		}
	}
	else if((addr & 0xF800) == 0x5000) // addr $5000-$57FF
	{
		return (uint8)(g_NESmapper.Mapper19.irq_counter & 0x00FF);
	}
	else if ((addr & 0xF800) == 0x5800) // addr $5800-$5FFF
	{
		return (uint8)((g_NESmapper.Mapper19.irq_counter & 0x7F00) >> 8);
	}
	else
	{
		return (uint8)(addr >> 8);
	}
}

void NES_mapper19_MemoryWriteLow(uint32 addr, uint8 data)
{
	switch(addr & 0xF800)
	{
	case 0x4800:
		{
			if(addr == 0x4800)
			{
				if(g_NESmapper.Mapper19.patch == 2)
				{
					NES_WriteSaveRAM(g_NESmapper.Mapper19.regs[2] & 0x7F, data);
					if(g_NESmapper.Mapper19.regs[2] & 0x80) g_NESmapper.Mapper19.regs[2] = ((g_NESmapper.Mapper19.regs[2] & 0x7F)+1) | 0x80;
				}
				else
				{
					NES_APU_ExWrite(addr, data);
				}
			}
		}
		break;

	case 0x5000: // addr $5000-$57FF
		{
			g_NESmapper.Mapper19.irq_counter = (g_NESmapper.Mapper19.irq_counter & 0xFF00) | data;
		}
		break;

	case 0x5800: // addr $5000-$57FF
		{
			g_NESmapper.Mapper19.irq_counter = (g_NESmapper.Mapper19.irq_counter & 0x00FF) | ((uint32)(data & 0x7F) << 8);
			g_NESmapper.Mapper19.irq_enabled = (data & 0x80) >> 7;
			if(g_NESmapper.Mapper19.patch == 1 || g_NESmapper.Mapper19.patch == 2)
			{
				g_NESmapper.Mapper19.irq_counter ++;
			}
		}
		break;
	}
}

void NES_mapper19_MemoryWrite(uint32 addr, uint8 data)
{
	switch(addr & 0xF800)
	{
	case 0x8000: // addr $8000-$87FF
		{
			if(data < 0xE0 || g_NESmapper.Mapper19.regs[0] == 1)
			{
				g_NESmapper.set_PPU_bank0(data);
			}
			else
			{
				g_NESmapper.set_VRAM_bank(0, 0);
			}
		}
		break;

	case 0x8800: // addr $8800-$8FFF
		{
			if(data < 0xE0 || g_NESmapper.Mapper19.regs[0] == 1)
			{
				g_NESmapper.set_PPU_bank1(data);
			}
			else
			{
				g_NESmapper.set_VRAM_bank(1, 1);
			}
		}
		break;

	case 0x9000: // addr $9000-$97FF
		{
			if(data < 0xE0 || g_NESmapper.Mapper19.regs[0] == 1)
			{
				g_NESmapper.set_PPU_bank2(data);
			}
			else
			{
				g_NESmapper.set_VRAM_bank(2, 2);
			}
		}
		break;

	case 0x9800: // addr $9800-$9FFF
		{
			if(data < 0xE0 || g_NESmapper.Mapper19.regs[0] == 1)
			{
				g_NESmapper.set_PPU_bank3(data);
			}
			else
			{
				g_NESmapper.set_VRAM_bank(3, 3);
			}
		}
		break;

	case 0xA000: // addr $A000-$A7FF
		{
			if(data < 0xE0 || g_NESmapper.Mapper19.regs[1] == 1)
			{
				g_NESmapper.set_PPU_bank4(data);
			}
			else
			{
				g_NESmapper.set_VRAM_bank(4, 4);
			}
		}
		break;

	case 0xA800: // addr $A800-$AFFF
		{
			if(data < 0xE0 || g_NESmapper.Mapper19.regs[1] == 1)
			{
				g_NESmapper.set_PPU_bank5(data);
			}
			else
			{
				g_NESmapper.set_VRAM_bank(5, 5);
			}
		}
		break;

	case 0xB000: // addr $B000-$B7FF
		{
			if(data < 0xE0 || g_NESmapper.Mapper19.regs[1] == 1)
			{
				g_NESmapper.set_PPU_bank6(data);
			}
			else
			{
				g_NESmapper.set_VRAM_bank(6, 6);
			}
		}
		break;

	case 0xB800: // addr $B800-$BFFF
		{
			if(data < 0xE0 || g_NESmapper.Mapper19.regs[1] == 1)
			{
				g_NESmapper.set_PPU_bank7(data);
			}
			else
			{
				g_NESmapper.set_VRAM_bank(7, 7);
			}
		}
		break;

	case 0xC000: // addr $C000-$C7FF
		{
			if (g_NESmapper.Mapper19.patch != 3) {
				if(data <= 0xDF)
				{
					g_NESmapper.set_PPU_bank8(data);
				}
				else
				{
					g_NESmapper.set_VRAM_bank(8, data & 0x01);
				}
			}
		}
		break;

	case 0xC800: // addr $C800-$CFFF
		{
			if (g_NESmapper.Mapper19.patch != 3) {
				if(data <= 0xDF)
				{
					g_NESmapper.set_PPU_bank9(data);
				}
				else
				{
					g_NESmapper.set_VRAM_bank(9, data & 0x01);
				}
			}
		}
		break;

	case 0xD000: // addr $D000-$D7FF
		{
			if (g_NESmapper.Mapper19.patch != 3) {
				if(data <= 0xDF)
				{
					g_NESmapper.set_PPU_bank10(data);
				}
				else
				{
					g_NESmapper.set_VRAM_bank(10, data & 0x01);
				}
			}
		}
		break;

	case 0xD800: // addr $D800-$DFFF
		{
			if (g_NESmapper.Mapper19.patch != 3) {
				if(data <= 0xDF)
				{
					g_NESmapper.set_PPU_bank11(data);
				}
				else
				{
					g_NESmapper.set_VRAM_bank(11, data & 0x01);
				}
			}
		}
		break;

	case 0xE000: // addr $E000-$E7FF
		{
			g_NESmapper.set_CPU_bank4(data & 0x3F);
		}
		break;

	case 0xE800: // addr $E800-$EFFF
		{
			g_NESmapper.set_CPU_bank5(data & 0x3F);
			g_NESmapper.Mapper19.regs[0] = (data & 0x40) >> 6;
			g_NESmapper.Mapper19.regs[1] = (data & 0x80) >> 7;
		}
		break;

	case 0xF000: // addr $F000-$F7FF
		{
			g_NESmapper.set_CPU_bank6(data & 0x3F);
		}
		break;

	case 0xF800:
		{
			if(addr == 0xF800)
			{
				if(g_NESmapper.Mapper19.patch == 2)
				{
					g_NESmapper.Mapper19.regs[2] = data;
				}
				else
				{
					NES_APU_ExWrite(addr, data);
				}
			}
		}
		break;
	}
}

void NES_mapper19_HSync(uint32 scanline)
{
	if(g_NESmapper.Mapper19.irq_enabled)
	{
		if(g_NESmapper.Mapper19.irq_counter >= (uint32)(0x7FFF - g_NESmapper.Mapper19.irq_sn))
		{
			g_NESmapper.Mapper19.irq_counter = 0x7FFF;
			NES6502_DoIRQ();
		}
		else
		{
			g_NESmapper.Mapper19.irq_counter += g_NESmapper.Mapper19.irq_sn;
		}
	}
}
/////////////////////////////////////////////////////////////////////

