
/////////////////////////////////////////////////////////////////////
// Mapper 1
void NES_mapper1_Init()
{
	if( NES_crc32()==0xaf4b5c8a)
	{
		VNES_mapper1_Init();
		return;
	}
	g_NESmapper.Reset = NES_mapper1_Reset;
	g_NESmapper.MemoryWriteSaveRAM = NES_mapper1_MemoryWriteSaveRAM;
	g_NESmapper.MemoryWrite = NES_mapper1_MemoryWrite;
}

void NES_mapper1_Reset()
{
	g_NESmapper.Mapper1.patch = 0;

	if(NES_crc32() == 0xb8747abf) // Best Play - Pro Yakyuu Special
	{
		nes6502_context context;
		uint32 i;
		g_NESmapper.Mapper1.wram = g_NES.mapper_extram;
		g_NES.mapper_extramsize = 0x4000;
		g_NESmapper.Mapper1.patch = 1;
		g_NESmapper.Mapper1.wram_bank = 0;
		g_NESmapper.Mapper1.wram_flag = 0;
		g_NESmapper.Mapper1.wram_count = 0;
		NES6502_GetContext(&context);
		context.mem_page[3] = g_NESmapper.Mapper1.wram;
		NES6502_SetContext(&context);
		for(i = 0; i < 0x4000; i++)
		{
			g_NESmapper.Mapper1.wram[i] = NES_ReadSaveRAM(i);
		}
	}

	if(NES_crc32() == 0x9b565541 || // Triathron, The
	        NES_crc32() == 0x95e4e594)   // Qix
	{
		g_NES.frame_irq_disenabled = 1;
	}

	if(NES_crc32() == 0x54b46b8e)	//Ninja Ryuukenden (C)
	{
		g_NESmapper.Mapper1.patch = 2;
	}
	if(NES_crc32() == 0x8775b7e9 || // Top Secret (C) V0.5
	   NES_crc32() == 0x74fc4122)   // Top Secret (C) V1.0
	{
		g_NESmapper.Mapper1.patch = 3;
	}
	g_NESmapper.Mapper1.write_count = 0;
	g_NESmapper.Mapper1.bits = 0x00;
	g_NESmapper.Mapper1.regs[0] = 0x0C; // reflects initial ROM state
	g_NESmapper.Mapper1.regs[1] = 0x00;
	g_NESmapper.Mapper1.regs[2] = 0x00;
	g_NESmapper.Mapper1.regs[3] = 0x00;
	{
		uint32 size_in_K = g_NESmapper.num_8k_ROM_banks * 8;

		if(size_in_K == 1024)
		{
			g_NESmapper.Mapper1.MMC1_Size = MMC1_1024K;
		}
		else if(size_in_K == 512)
		{
			g_NESmapper.Mapper1.MMC1_Size = MMC1_512K;
		}
		else
		{
			g_NESmapper.Mapper1.MMC1_Size = MMC1_SMALL;
		}
	}
	g_NESmapper.Mapper1.MMC1_256K_base = 0; // use first 256K
	g_NESmapper.Mapper1.MMC1_swap = 0;

	if(g_NESmapper.Mapper1.MMC1_Size == MMC1_SMALL)
	{
		// set two high pages to last two banks
		g_NESmapper.Mapper1.MMC1_HI1 = g_NESmapper.num_8k_ROM_banks-2;
		g_NESmapper.Mapper1.MMC1_HI2 = g_NESmapper.num_8k_ROM_banks-1;
	}
	else
	{
		// set two high pages to last two banks of current 256K region
		g_NESmapper.Mapper1.MMC1_HI1 = 30;
		g_NESmapper.Mapper1.MMC1_HI2 = 31;
	}

	// set CPU bank pointers
	g_NESmapper.Mapper1.MMC1_bank1 = 0;
	g_NESmapper.Mapper1.MMC1_bank2 = 1;
	g_NESmapper.Mapper1.MMC1_bank3 = g_NESmapper.Mapper1.MMC1_HI1;
	g_NESmapper.Mapper1.MMC1_bank4 = g_NESmapper.Mapper1.MMC1_HI2;

	NES_mapper1_MMC1_set_CPU_banks();
}

void NES_mapper1_MMC1_set_CPU_banks()
{
	g_NESmapper.set_CPU_banks4((g_NESmapper.Mapper1.MMC1_256K_base << 5) + (g_NESmapper.Mapper1.MMC1_bank1 & ((g_NESmapper.Mapper1.patch == 3) ? 255 : 31)),
	              (g_NESmapper.Mapper1.MMC1_256K_base << 5) + (g_NESmapper.Mapper1.MMC1_bank2 & ((g_NESmapper.Mapper1.patch == 3) ? 255 : 31)),
	              (g_NESmapper.Mapper1.MMC1_256K_base << 5) + (g_NESmapper.Mapper1.MMC1_bank3 & ((g_NESmapper.Mapper1.patch == 3) ? 255 : 31)),
	              (g_NESmapper.Mapper1.MMC1_256K_base << 5) + (g_NESmapper.Mapper1.MMC1_bank4 & ((g_NESmapper.Mapper1.patch == 3) ? 255 : 31)));
}

void NES_mapper1_MemoryWriteSaveRAM(uint32 addr, uint8 data)
{
	// Best Play - Pro Yakyuu Special
	if(g_NESmapper.Mapper1.patch==1)
	{
		if(g_NESmapper.Mapper1.wram_bank)
		{
			g_NESmapper.Mapper1.wram[addr-0x4000] = data;
		}
		else
		{
			g_NESmapper.Mapper1.wram[addr-0x6000] = data;
		}
		NES_WriteSaveRAM(addr-0x6000, g_NESmapper.Mapper1.wram[addr-0x6000]);
		NES_WriteSaveRAM(addr-0x4000, g_NESmapper.Mapper1.wram[addr-0x4000]);
	}
}

void NES_mapper1_MemoryWrite(uint32 addr, uint8 data)
{
	uint32 reg_num;
	if((g_NESmapper.Mapper1.patch==1) && (addr == 0xBFFF))
	{
		g_NESmapper.Mapper1.wram_count++;
		g_NESmapper.Mapper1.wram_flag += data & 0x01;
		if(g_NESmapper.Mapper1.wram_count == 5)
		{
			if(g_NESmapper.Mapper1.wram_flag)
			{
				nes6502_context context;
				g_NESmapper.Mapper1.wram_bank = 1;
				NES6502_GetContext(&context);
				context.mem_page[3] = g_NESmapper.Mapper1.wram + 0x2000;
				NES6502_SetContext(&context);
			}
			else
			{
				nes6502_context context;
				g_NESmapper.Mapper1.wram_bank = 0;
				NES6502_GetContext(&context);
				context.mem_page[3] = g_NESmapper.Mapper1.wram;
				NES6502_SetContext(&context);
			}
			g_NESmapper.Mapper1.wram_flag = 0;
			g_NESmapper.Mapper1.wram_count = 0;
		}
	}

	if(g_NESmapper.Mapper1.patch==2)
	{
		if(data & 0x80)
		{
			g_NESmapper.Mapper1.write_count = 0;
			g_NESmapper.Mapper1.bits = 0x00;
			return;
		}
		reg_num = (addr & 0x7FFF) >> 13;
		g_NESmapper.Mapper1.regs[reg_num] = data;
	}else{
	// if write is to a different reg, reset
	if((addr & 0x6000) != (g_NESmapper.Mapper1.last_write_addr & 0x6000))
	{
		g_NESmapper.Mapper1.write_count = 0;
		g_NESmapper.Mapper1.bits = 0x00;
	}
	g_NESmapper.Mapper1.last_write_addr = addr;

	// if bit 7 set, reset and return
	if(data & 0x80)
	{
		g_NESmapper.Mapper1.write_count = 0;
		g_NESmapper.Mapper1.bits = 0x00;
		return;
	}

	if(data & 0x01) g_NESmapper.Mapper1.bits |= (1 << g_NESmapper.Mapper1.write_count);
	g_NESmapper.Mapper1.write_count++;
	if(g_NESmapper.Mapper1.write_count < 5) return;

	reg_num = (addr & 0x7FFF) >> 13;
	g_NESmapper.Mapper1.regs[reg_num] = g_NESmapper.Mapper1.bits;
	}

	g_NESmapper.Mapper1.write_count = 0;
	g_NESmapper.Mapper1.bits = 0x00;

	//  LOG("MAP1 REG" << reg_num << ": " << HEX(regs[reg_num],2) << endl);

	switch(reg_num)
	{
	case 0:
		{
			//        LOG("REG0: " << HEX(regs[0],2) << endl);

			// set mirroring
			if(g_NESmapper.Mapper1.regs[0] & 0x02)
			{
				if(g_NESmapper.Mapper1.regs[0] & 0x01)
				{
					g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
				}
				else
				{
					g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
				}
			}
			else
			{
				// one-screen mirroring
				if(g_NESmapper.Mapper1.regs[0] & 0x01)
				{
					g_NESmapper.set_mirroring(1,1,1,1);
				}
				else
				{
					g_NESmapper.set_mirroring(0,0,0,0);
				}
			}
		}
		break;

	case 1:
		{
			uint8 bank_num = g_NESmapper.Mapper1.regs[1];

			//        LOG("REG1: " << HEX(regs[1],2) <<"REG2: " << HEX(regs[2],2) << endl);

			if(g_NESmapper.Mapper1.MMC1_Size == MMC1_1024K)
			{
				if(g_NESmapper.Mapper1.regs[0] & 0x10)
				{
					if(g_NESmapper.Mapper1.MMC1_swap)
					{
						g_NESmapper.Mapper1.MMC1_256K_base = (g_NESmapper.Mapper1.regs[1] & 0x10) >> 4;
						if(g_NESmapper.Mapper1.regs[0] & 0x08)
						{
							g_NESmapper.Mapper1.MMC1_256K_base |= ((g_NESmapper.Mapper1.regs[2] & 0x10) >> 3);
						}
						NES_mapper1_MMC1_set_CPU_banks();
						g_NESmapper.Mapper1.MMC1_swap = 0;
					}
					else
					{
						g_NESmapper.Mapper1.MMC1_swap = 1;
					}
				}
				else
				{
					// use 1st or 4th 256K banks
					g_NESmapper.Mapper1.MMC1_256K_base = (g_NESmapper.Mapper1.regs[1] & 0x10) ? 3 : 0;
					NES_mapper1_MMC1_set_CPU_banks();
				}
			}
			else if((g_NESmapper.Mapper1.MMC1_Size == MMC1_512K) && (!g_NESmapper.num_1k_VROM_banks))
			{
				g_NESmapper.Mapper1.MMC1_256K_base = (g_NESmapper.Mapper1.regs[1] & 0x10) >> 4;
				NES_mapper1_MMC1_set_CPU_banks();
			}
			else if(g_NESmapper.num_1k_VROM_banks)
			{
				// set VROM bank at $0000
				if(g_NESmapper.Mapper1.regs[0] & 0x10)
				{
					// swap 4K
					bank_num <<= 2;
					g_NESmapper.set_PPU_bank0(bank_num+0);
					g_NESmapper.set_PPU_bank1(bank_num+1);
					g_NESmapper.set_PPU_bank2(bank_num+2);
					g_NESmapper.set_PPU_bank3(bank_num+3);
				}
				else
				{
					// swap 8K
					bank_num <<= 2;
					g_NESmapper.set_PPU_banks8(bank_num+0,bank_num+1,bank_num+2,bank_num+3,
					              bank_num+4,bank_num+5,bank_num+6,bank_num+7);
				}
			}
			else
			{
				if(g_NESmapper.Mapper1.regs[0] & 0x10)
				{
					bank_num <<= 2;
					g_NESmapper.set_VRAM_bank(0, bank_num+0);
					g_NESmapper.set_VRAM_bank(1, bank_num+1);
					g_NESmapper.set_VRAM_bank(2, bank_num+2);
					g_NESmapper.set_VRAM_bank(3, bank_num+3);
				}
			}
		}
		break;

	case 2:
		{
			uint8 bank_num = g_NESmapper.Mapper1.regs[2];

			//        LOG("REG2: " << HEX(regs[2],2) << endl);

			if((g_NESmapper.Mapper1.MMC1_Size == MMC1_1024K) && (g_NESmapper.Mapper1.regs[0] & 0x08))
			{
				if(g_NESmapper.Mapper1.MMC1_swap)
				{
					g_NESmapper.Mapper1.MMC1_256K_base =  (g_NESmapper.Mapper1.regs[1] & 0x10) >> 4;
					g_NESmapper.Mapper1.MMC1_256K_base |= ((g_NESmapper.Mapper1.regs[2] & 0x10) >> 3);
					NES_mapper1_MMC1_set_CPU_banks();
					g_NESmapper.Mapper1.MMC1_swap = 0;
				}
				else
				{
					g_NESmapper.Mapper1.MMC1_swap = 1;
				}
			}

			if(!g_NESmapper.num_1k_VROM_banks)
			{
				if(g_NESmapper.Mapper1.regs[0] & 0x10)
				{
					bank_num <<= 2;
					g_NESmapper.set_VRAM_bank(4, bank_num+0);
					g_NESmapper.set_VRAM_bank(5, bank_num+1);
					g_NESmapper.set_VRAM_bank(6, bank_num+2);
					g_NESmapper.set_VRAM_bank(7, bank_num+3);
					break;
				}
			}

			// set 4K VROM bank at $1000
			if(g_NESmapper.Mapper1.regs[0] & 0x10)
			{
				// swap 4K
				bank_num <<= 2;
				g_NESmapper.set_PPU_bank4(bank_num+0);
				g_NESmapper.set_PPU_bank5(bank_num+1);
				g_NESmapper.set_PPU_bank6(bank_num+2);
				g_NESmapper.set_PPU_bank7(bank_num+3);
			}
		}
		break;

	case 3:
		{
			uint8 bank_num = g_NESmapper.Mapper1.regs[3];

			//        LOG("REG3: " << HEX(regs[3],2) << endl);

			// set ROM bank
			if(g_NESmapper.Mapper1.regs[0] & 0x08)
			{
				// 16K of ROM
				bank_num <<= 1;

				if(g_NESmapper.Mapper1.regs[0] & 0x04)
				{
					// 16K of ROM at $8000
					g_NESmapper.Mapper1.MMC1_bank1 = bank_num;
					g_NESmapper.Mapper1.MMC1_bank2 = bank_num+1;
					g_NESmapper.Mapper1.MMC1_bank3 = g_NESmapper.Mapper1.MMC1_HI1;
					g_NESmapper.Mapper1.MMC1_bank4 = g_NESmapper.Mapper1.MMC1_HI2;
				}
				else
				{
					// 16K of ROM at $C000
					if(g_NESmapper.Mapper1.MMC1_Size == MMC1_SMALL)
					{
						g_NESmapper.Mapper1.MMC1_bank1 = 0;
						g_NESmapper.Mapper1.MMC1_bank2 = 1;
						g_NESmapper.Mapper1.MMC1_bank3 = bank_num;
						g_NESmapper.Mapper1.MMC1_bank4 = bank_num+1;
					}
				}
			}
			else
			{
				// 32K of ROM at $8000
				bank_num <<= 1; // thanx rinao

				g_NESmapper.Mapper1.MMC1_bank1 = bank_num;
				g_NESmapper.Mapper1.MMC1_bank2 = bank_num+1;
				if(g_NESmapper.Mapper1.MMC1_Size == MMC1_SMALL)
				{
					g_NESmapper.Mapper1.MMC1_bank3 = bank_num+2;
					g_NESmapper.Mapper1.MMC1_bank4 = bank_num+3;
				}
			}

			NES_mapper1_MMC1_set_CPU_banks();
		}
		break;
	}
}
/////////////////////////////////////////////////////////////////////

