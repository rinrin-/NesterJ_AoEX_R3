#ifdef _NES_MAPPER_CPP_

/////////////////////////////////////////////////////////////////////
// Mapper 5
void NES_mapper5_Init()
{
	g_NESmapper.Reset = NES_mapper5_Reset;
	g_NESmapper.MemoryReadLow = NES_mapper5_MemoryReadLow;
	g_NESmapper.MemoryWriteLow = NES_mapper5_MemoryWriteLow;
	g_NESmapper.MemoryWriteSaveRAM = NES_mapper5_MemoryWriteSaveRAM;
	g_NESmapper.MemoryWrite = NES_mapper5_MemoryWrite;
	g_NESmapper.HSync = NES_mapper5_HSync;
	g_NESmapper.PPU_Latch_RenderScreen = NES_mapper5_PPU_Latch_RenderScreen;
}



void NES_mapper5_Reset()
{
	uint32 i;
	g_NESmapper.Mapper5.wram_size = 1;

	if(NES_crc32() == 0x2b548d75 || // Bandit Kings of Ancient China (J)
	        NES_crc32() == 0xf4cd4998 || // Dai Koukai Jidai (J)
	        NES_crc32() == 0x8fa95456 || // Ishin no Arashi (J)
	        NES_crc32() == 0x98c8e090 || // Nobunaga no Yabou - Sengoku Gunyuu Den (J)
	        NES_crc32() == 0x57e3218b || // L'Empereur (J)
	        NES_crc32() == 0x2f50bd38 || // L'Empereur (U)
	        NES_crc32() == 0x8e9a5e2f || // L'Empereur (Alt)(U)
	        NES_crc32() == 0xb56958d1 || // Nobunaga's Ambition 2 (J)
	        NES_crc32() == 0xe6c28c5f || // Suikoden - Tenmei no Chikai (J)
	        NES_crc32() == 0xcd35e2e9)   // Uncharted Waters (J)
	{
		g_NESmapper.Mapper5.wram_size = 2;
	}

	if(NES_crc32() == 0xf4120e58 || // Aoki Ookami to Shiroki Mejika - Genchou Hishi (J)
	        NES_crc32() == 0x286613d8 || // Nobunaga no Yabou - Bushou Fuuun Roku (J)
	        NES_crc32() == 0x11eaad26 || // Romance of the 3 Kingdoms 2 (J)
	        NES_crc32() == 0x95ba5733)   // Sangokushi 2 (J)
	{
		g_NESmapper.Mapper5.wram_size = 3;
	}
	g_NESmapper.Mapper5.wram = g_NES.mapper_extram;
	g_NES.mapper_extramsize = 0x10000;

	// set SaveRAM
	for(i = 0; i < 0x10000; i++)
	{
		g_NESmapper.Mapper5.wram[i] = NES_ReadSaveRAM(i);
	}
	NES_mapper5_MMC5_set_WRAM_bank(3,0);

	// Init ExSound
	NES_APU_SelectExSound(8);

	// set CPU bank pointers
	g_NESmapper.set_CPU_bank4(g_NESmapper.num_8k_ROM_banks-1);
	g_NESmapper.set_CPU_bank5(g_NESmapper.num_8k_ROM_banks-1);
	g_NESmapper.set_CPU_bank6(g_NESmapper.num_8k_ROM_banks-1);
	g_NESmapper.set_CPU_bank7(g_NESmapper.num_8k_ROM_banks-1);

	// set PPU bank pointers
	g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);

	for(i = 0; i < 8; i++)
	{
		g_NESmapper.Mapper5.chr_reg[i][0] = i;
		g_NESmapper.Mapper5.chr_reg[i][1] = (i & 0x03) + 4;
	}
	g_NESmapper.Mapper5.wb[3] = 0;
	g_NESmapper.Mapper5.wb[4] = g_NESmapper.Mapper5.wb[5] = g_NESmapper.Mapper5.wb[6] = 8;

	g_NESmapper.Mapper5.prg_size = 3;
	g_NESmapper.Mapper5.wram_protect0 = 0x02;
	g_NESmapper.Mapper5.wram_protect1 = 0x01;
	g_NESmapper.Mapper5.chr_size = 3;
	g_NESmapper.Mapper5.gfx_mode = 0;

	g_NESmapper.Mapper5.irq_enabled = 0;
	g_NESmapper.Mapper5.irq_status = 0;
	g_NESmapper.Mapper5.irq_line = 0;

	g_NESmapper.Mapper5.split_control = 0;
	g_NESmapper.Mapper5.split_bank = 0;
}

uint8 NES_mapper5_MemoryReadLow(uint32 addr)
{
	uint8 ret = (uint8)(addr >> 8);
	if(addr == 0x5204)
	{
		ret = g_NESmapper.Mapper5.irq_status;
		g_NESmapper.Mapper5.irq_status &= ~0x80;
	}
	else if(addr == 0x5205)
	{
		ret = (uint8)((g_NESmapper.Mapper5.value0*g_NESmapper.Mapper5.value1) & 0x00FF);
	}
	else if(addr == 0x5206)
	{
		ret = (uint8)(((g_NESmapper.Mapper5.value0*g_NESmapper.Mapper5.value1) & 0xFF00) >> 8);
	}
	else if(addr >= 0x5C00 && addr <=0x5FFF)
	{
		if(g_NESmapper.Mapper5.gfx_mode == 2 || g_NESmapper.Mapper5.gfx_mode == 3)
		{
			uint8* nametable2 = NES_PPU_get_namt() + 0x800;
			ret = nametable2[addr & 0x3FF];
		}
	}
	return ret;
}

void NES_mapper5_MemoryWriteLow(uint32 addr, uint8 data)
{
	uint32 i;

	switch(addr)
	{
	case 0x5100:
		{
			g_NESmapper.Mapper5.prg_size = data & 0x03;
		}
		break;

	case 0x5101:
		{
			g_NESmapper.Mapper5.chr_size = data & 0x03;
		}
		break;

	case 0x5102:
		{
			g_NESmapper.Mapper5.wram_protect0 = data & 0x03;
		}
		break;

	case 0x5103:
		{
			g_NESmapper.Mapper5.wram_protect1 = data & 0x03;
		}
		break;

	case 0x5104:
		{
			g_NESmapper.Mapper5.gfx_mode = data & 0x03;
		}
		break;

	case 0x5105:
		{
			for(i = 0; i < 4; i++)
			{
				g_NESmapper.set_VRAM_bank((uint8)(8+i), (data & 0x03));
				data >>= 2;
			}
		}
		break;

	case 0x5106:
		{
			uint8* nametable3 = NES_PPU_get_namt() + 0xC00;
			for(i = 0; i < 0x3C0; i++)
			{
				nametable3[i] = data;
			}
		}
		break;

	case 0x5107:
		{
			uint8* nametable3 = NES_PPU_get_namt() + 0xC00;
			data &= 0x03;
			data = data | (data<<2) | (data<<4) | (data<<6);
			for(i = 0x3C0; i < 0x400; i++)
			{
				nametable3[i] = data;
			}
		}
		break;

	case 0x5113:
		{
			NES_mapper5_MMC5_set_WRAM_bank(3, (uint8)(data & 0x07));
		}
		break;

	case 0x5114:
	case 0x5115:
	case 0x5116:
	case 0x5117:
		{
			NES_mapper5_MMC5_set_CPU_bank((uint8)(addr & 0x07), (uint8)data);
		}
		break;

	case 0x5120:
	case 0x5121:
	case 0x5122:
	case 0x5123:
	case 0x5124:
	case 0x5125:
	case 0x5126:
	case 0x5127:
		{
			g_NESmapper.Mapper5.chr_reg[addr & 0x07][0] = data;
			NES_mapper5_sync_Chr_banks(0);
		}
		break;

	case 0x5128:
	case 0x5129:
	case 0x512A:
	case 0x512B:
		{
			g_NESmapper.Mapper5.chr_reg[(addr & 0x03) + 0][1] = data;
			g_NESmapper.Mapper5.chr_reg[(addr & 0x03) + 4][1] = data;
		}
		break;

	case 0x5200:
		{
			g_NESmapper.Mapper5.split_control = data;
		}
		break;

	case 0x5201:
		{
			//split_scroll = data;
		}
		break;

	case 0x5202:
		{
			g_NESmapper.Mapper5.split_bank = data & 0x3F;
		}
		break;

	case 0x5203:
		{
			g_NESmapper.Mapper5.irq_line = data;
		}
		break;

	case 0x5204:
		{
			g_NESmapper.Mapper5.irq_enabled = data;
		}
		break;

	case 0x5205:
		{
			g_NESmapper.Mapper5.value0 = data;
		}
		break;

	case 0x5206:
		{
			g_NESmapper.Mapper5.value1 = data;
		}
		break;

	default:
		{
			if(addr >= 0x5000 && addr <= 0x5015)
			{
				NES_APU_ExWrite(addr, data);
			}
			else if(addr >= 0x5C00 && addr <= 0x5FFF)
			{
				if(g_NESmapper.Mapper5.gfx_mode != 3)
				{
					uint8* nametable2 = NES_PPU_get_namt() + 0x800;
					nametable2[addr & 0x3FF] = data; //(irq_status & 0) ? data : 0x40;
				}
			}
		}
		break;
	}
}

void NES_mapper5_MemoryWriteSaveRAM(uint32 addr, uint8 data)
{
	if(g_NESmapper.Mapper5.wram_protect0 == 0x02 && g_NESmapper.Mapper5.wram_protect1 == 0x01)
	{
		if(g_NESmapper.Mapper5.wb[3] != 8)
		{
			g_NESmapper.Mapper5.wram[g_NESmapper.Mapper5.wb[3]*0x2000+(addr&0x1FFF)] = data;
			NES_WriteSaveRAM(g_NESmapper.Mapper5.wb[3]*0x2000+(addr&0x1FFF), data);
		}
	}
	NES_WriteSaveRAM(addr&0x1FFF, g_NESmapper.Mapper5.wram[addr&0x1FFF]);
}

void NES_mapper5_MemoryWrite(uint32 addr, uint8 data)
{
	if(g_NESmapper.Mapper5.wram_protect0 == 0x02 && g_NESmapper.Mapper5.wram_protect1 == 0x01)
	{
		if(addr >= 0x8000 && addr <= 0x9FFF)
		{
			if(g_NESmapper.Mapper5.wb[4] != 8)
			{
				g_NESmapper.Mapper5.wram[g_NESmapper.Mapper5.wb[4]*0x2000+(addr&0x1FFF)] = data;
				NES_WriteSaveRAM(g_NESmapper.Mapper5.wb[4]*0x2000+(addr&0x1FFF), data);
			}
		}
		else if(addr >= 0xA000 && addr <= 0xBFFF)
		{
			if(g_NESmapper.Mapper5.wb[5] != 8)
			{
				g_NESmapper.Mapper5.wram[g_NESmapper.Mapper5.wb[5]*0x2000+(addr&0x1FFF)] = data;
				NES_WriteSaveRAM(g_NESmapper.Mapper5.wb[5]*0x2000+(addr&0x1FFF), data);
			}
		}
		else if(addr >= 0xC000 && addr <= 0xDFFF)
		{
			if(g_NESmapper.Mapper5.wb[6] != 8)
			{
				g_NESmapper.Mapper5.wram[g_NESmapper.Mapper5.wb[6]*0x2000+(addr&0x1FFF)] = data;
				NES_WriteSaveRAM(g_NESmapper.Mapper5.wb[6]*0x2000+(addr&0x1FFF), data);
			}
		}
	}
}

void NES_mapper5_HSync(uint32 scanline)
{
	if(scanline <= 240)
	{
		if(scanline == g_NESmapper.Mapper5.irq_line)
		{
			if(NES_PPU_spr_enabled() && NES_PPU_bg_enabled())
			{
				g_NESmapper.Mapper5.irq_status |= 0x80;
			}
		}
		if((g_NESmapper.Mapper5.irq_status & 0x80) && (g_NESmapper.Mapper5.irq_enabled & 0x80))
		{
			NES6502_DoIRQ();
		}
	}
	else
	{
		g_NESmapper.Mapper5.irq_status |= 0x40;
	}
}

void NES_mapper5_MMC5_set_CPU_bank(uint8 page, uint8 bank)
{
	if(bank & 0x80)
	{
		if(g_NESmapper.Mapper5.prg_size == 0)
		{
			if(page == 7)
			{
				g_NESmapper.set_CPU_bank4((bank & 0x7C)+0);
				g_NESmapper.set_CPU_bank5((bank & 0x7C)+1);
				g_NESmapper.set_CPU_bank6((bank & 0x7C)+2);
				g_NESmapper.set_CPU_bank7((bank & 0x7C)+3);
				g_NESmapper.Mapper5.wb[4] = g_NESmapper.Mapper5.wb[5] = g_NESmapper.Mapper5.wb[6] = 8;
			}
		}
		if(g_NESmapper.Mapper5.prg_size == 1)
		{
			if(page == 5)
			{
				g_NESmapper.set_CPU_bank4((bank & 0x7E)+0);
				g_NESmapper.set_CPU_bank5((bank & 0x7E)+1);
				g_NESmapper.Mapper5.wb[4] = g_NESmapper.Mapper5.wb[5] = 8;
			}
			if(page == 7)
			{
				g_NESmapper.set_CPU_bank6((bank & 0x7E)+0);
				g_NESmapper.set_CPU_bank7((bank & 0x7E)+1);
				g_NESmapper.Mapper5.wb[6] = 8;
			}
		}
		if(g_NESmapper.Mapper5.prg_size == 2)
		{
			if(page == 5)
			{
				g_NESmapper.set_CPU_bank4((bank & 0x7E)+0);
				g_NESmapper.set_CPU_bank5((bank & 0x7E)+1);
				g_NESmapper.Mapper5.wb[4] = g_NESmapper.Mapper5.wb[5] = 8;
			}
			if(page == 6)
			{
				g_NESmapper.set_CPU_bank6(bank & 0x7F);
				g_NESmapper.Mapper5.wb[6] = 8;
			}
			if(page == 7)
			{
				g_NESmapper.set_CPU_bank7(bank & 0x7F);
			}
		}
		if(g_NESmapper.Mapper5.prg_size == 3)
		{
			if(page == 4)
			{
				g_NESmapper.set_CPU_bank4(bank & 0x7F);
				g_NESmapper.Mapper5.wb[4] = 8;
			}
			if(page == 5)
			{
				g_NESmapper.set_CPU_bank5(bank & 0x7F);
				g_NESmapper.Mapper5.wb[5] = 8;
			}
			if(page == 6)
			{
				g_NESmapper.set_CPU_bank6(bank & 0x7F);
				g_NESmapper.Mapper5.wb[6] = 8;
			}
			if(page == 7)
			{
				g_NESmapper.set_CPU_bank7(bank & 0x7F);
			}
		}
	}
	else
	{
		if(g_NESmapper.Mapper5.prg_size == 1)
		{
			if(page == 5)
			{
				NES_mapper5_MMC5_set_WRAM_bank((uint8)4, (uint8)((bank & 0x06)+0));
				NES_mapper5_MMC5_set_WRAM_bank((uint8)5, (uint8)((bank & 0x06)+1));
			}
		}
		if(g_NESmapper.Mapper5.prg_size == 2)
		{
			if(page == 5)
			{
				NES_mapper5_MMC5_set_WRAM_bank((uint8)4, (uint8)((bank & 0x06)+0));
				NES_mapper5_MMC5_set_WRAM_bank((uint8)5, (uint8)((bank & 0x06)+1));
			}
			if(page == 6)
			{
				NES_mapper5_MMC5_set_WRAM_bank((uint8)6, (uint8)(bank & 0x07));
			}
		}
		if(g_NESmapper.Mapper5.prg_size == 3)
		{
			if(page == 4)
			{
				NES_mapper5_MMC5_set_WRAM_bank((uint8)4, (uint8)(bank & 0x07));
			}
			if(page == 5)
			{
				NES_mapper5_MMC5_set_WRAM_bank((uint8)5, (uint8)(bank & 0x07));
			}
			if(page == 6)
			{
				NES_mapper5_MMC5_set_WRAM_bank((uint8)6, (uint8)(bank & 0x07));
			}
		}
	}
}

void NES_mapper5_MMC5_set_WRAM_bank(uint8 page, uint8 bank)
{
	if(bank != 8)
	{
		if(g_NESmapper.Mapper5.wram_size == 1) bank = (bank > 3) ? 8 : 0;
		if(g_NESmapper.Mapper5.wram_size == 2) bank = (bank > 3) ? 1 : 0;
		if(g_NESmapper.Mapper5.wram_size == 3) bank = (bank > 3) ? 8 : bank;
		if(g_NESmapper.Mapper5.wram_size == 4) bank = (bank > 3) ? 4 : bank;
	}
	g_NESmapper.Mapper5.wb[page] = bank;

	if(bank != 8)
	{
		nes6502_context context;
		NES6502_GetContext(&context);
		context.mem_page[page] = g_NESmapper.Mapper5.wram + bank*0x2000;
		NES6502_SetContext(&context);
	}
}

uint8 NES_mapper5_PPU_Latch_RenderScreen(uint8 mode, uint32 addr)
{
	uint8 ex_pal = 0;

	if(g_NESmapper.Mapper5.gfx_mode == 1 && mode == 1)
	{
		// ex gfx mode
		uint8* nametable2 = NES_PPU_get_namt() + 0x800;
		uint32 bank = (nametable2[addr] & 0x3F) << 2;
		g_NESmapper.set_PPU_banks8(bank,bank+1,bank+2,bank+3,bank,bank+1,bank+2,bank+3);
		ex_pal = ((nametable2[addr] & 0xC0) >> 4) | 0x01;
	}
	else
	{
		// normal
		NES_mapper5_sync_Chr_banks(mode);
	}
	return ex_pal;
}

void NES_mapper5_sync_Chr_banks(uint8 mode)
{
	if(g_NESmapper.Mapper5.chr_size == 0)
	{
		g_NESmapper.set_PPU_bank0(g_NESmapper.Mapper5.chr_reg[7][mode]*8+0);
		g_NESmapper.set_PPU_bank1(g_NESmapper.Mapper5.chr_reg[7][mode]*8+1);
		g_NESmapper.set_PPU_bank2(g_NESmapper.Mapper5.chr_reg[7][mode]*8+2);
		g_NESmapper.set_PPU_bank3(g_NESmapper.Mapper5.chr_reg[7][mode]*8+3);
		g_NESmapper.set_PPU_bank4(g_NESmapper.Mapper5.chr_reg[7][mode]*8+4);
		g_NESmapper.set_PPU_bank5(g_NESmapper.Mapper5.chr_reg[7][mode]*8+5);
		g_NESmapper.set_PPU_bank6(g_NESmapper.Mapper5.chr_reg[7][mode]*8+6);
		g_NESmapper.set_PPU_bank7(g_NESmapper.Mapper5.chr_reg[7][mode]*8+7);
	}
	else if(g_NESmapper.Mapper5.chr_size == 1)
	{
		g_NESmapper.set_PPU_bank0(g_NESmapper.Mapper5.chr_reg[3][mode]*4+0);
		g_NESmapper.set_PPU_bank1(g_NESmapper.Mapper5.chr_reg[3][mode]*4+1);
		g_NESmapper.set_PPU_bank2(g_NESmapper.Mapper5.chr_reg[3][mode]*4+2);
		g_NESmapper.set_PPU_bank3(g_NESmapper.Mapper5.chr_reg[3][mode]*4+3);
		g_NESmapper.set_PPU_bank4(g_NESmapper.Mapper5.chr_reg[7][mode]*4+0);
		g_NESmapper.set_PPU_bank5(g_NESmapper.Mapper5.chr_reg[7][mode]*4+1);
		g_NESmapper.set_PPU_bank6(g_NESmapper.Mapper5.chr_reg[7][mode]*4+2);
		g_NESmapper.set_PPU_bank7(g_NESmapper.Mapper5.chr_reg[7][mode]*4+3);
	}
	else if(g_NESmapper.Mapper5.chr_size == 2)
	{
		g_NESmapper.set_PPU_bank0(g_NESmapper.Mapper5.chr_reg[1][mode]*2+0);
		g_NESmapper.set_PPU_bank1(g_NESmapper.Mapper5.chr_reg[1][mode]*2+1);
		g_NESmapper.set_PPU_bank2(g_NESmapper.Mapper5.chr_reg[3][mode]*2+0);
		g_NESmapper.set_PPU_bank3(g_NESmapper.Mapper5.chr_reg[3][mode]*2+1);
		g_NESmapper.set_PPU_bank4(g_NESmapper.Mapper5.chr_reg[5][mode]*2+0);
		g_NESmapper.set_PPU_bank5(g_NESmapper.Mapper5.chr_reg[5][mode]*2+1);
		g_NESmapper.set_PPU_bank6(g_NESmapper.Mapper5.chr_reg[7][mode]*2+0);
		g_NESmapper.set_PPU_bank7(g_NESmapper.Mapper5.chr_reg[7][mode]*2+1);
	}
	else
	{
		g_NESmapper.set_PPU_bank0(g_NESmapper.Mapper5.chr_reg[0][mode]);
		g_NESmapper.set_PPU_bank1(g_NESmapper.Mapper5.chr_reg[1][mode]);
		g_NESmapper.set_PPU_bank2(g_NESmapper.Mapper5.chr_reg[2][mode]);
		g_NESmapper.set_PPU_bank3(g_NESmapper.Mapper5.chr_reg[3][mode]);
		g_NESmapper.set_PPU_bank4(g_NESmapper.Mapper5.chr_reg[4][mode]);
		g_NESmapper.set_PPU_bank5(g_NESmapper.Mapper5.chr_reg[5][mode]);
		g_NESmapper.set_PPU_bank6(g_NESmapper.Mapper5.chr_reg[6][mode]);
		g_NESmapper.set_PPU_bank7(g_NESmapper.Mapper5.chr_reg[7][mode]);
	}
}
/////////////////////////////////////////////////////////////////////

#endif
