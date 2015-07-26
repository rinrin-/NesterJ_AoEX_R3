
/////////////////////////////////////////////////////////////////////
// Mapper 90
void NES_mapper90_Init()
{
	g_NESmapper.Reset = NES_mapper90_Reset;
	g_NESmapper.MemoryReadLow = NES_mapper90_MemoryReadLow;
	g_NESmapper.MemoryWriteLow = NES_mapper90_MemoryWriteLow;
	g_NESmapper.MemoryWrite = NES_mapper90_MemoryWrite;
	g_NESmapper.HSync = NES_mapper90_HSync;
}

uint8 NES_mapper90_MemoryReadLow(uint32 addr);
void NES_mapper90_MemoryWriteLow(uint32 addr, uint8 data);
void NES_mapper90_MemoryWrite(uint32 addr, uint8 data);
void NES_mapper90_HSync(uint32 scanline);

void NES_mapper90_SetBank_CPU();
void NES_mapper90_SetBank_PPU();
void NES_mapper90_SetBank_VRAM();



void NES_mapper90_Reset()
{
	int i;
	g_NESmapper.SetPROM_32K_Banks( g_NESmapper.num_8k_ROM_banks-4, g_NESmapper.num_8k_ROM_banks-3, g_NESmapper.num_8k_ROM_banks-2, g_NESmapper.num_8k_ROM_banks-1 );
	g_NESmapper.SetVROM_8K_Bank( 0 );

	g_NESmapper.Mapper90.patch = 0;

	if( NES_crc32() == 0x2a268152 ) {
		g_NESmapper.Mapper90.patch = 1;
	}
	if( NES_crc32() == 0x2224b882 ) {
		//nes->SetRenderMethod( NES::TILE_RENDER );
	}

	g_NESmapper.Mapper90.irq_enable = 0;	// Disable
	g_NESmapper.Mapper90.irq_counter = 0;
	g_NESmapper.Mapper90.irq_latch = 0;
	g_NESmapper.Mapper90.irq_occur = 0;
	g_NESmapper.Mapper90.irq_preset = 0;
	g_NESmapper.Mapper90.irq_offset = 0;

	g_NESmapper.Mapper90.prg_6000 = 0;
	g_NESmapper.Mapper90.prg_E000 = 0;
	g_NESmapper.Mapper90.prg_size = 0;
	g_NESmapper.Mapper90.chr_size = 0;
	g_NESmapper.Mapper90.mir_mode = 0;
	g_NESmapper.Mapper90.mir_type = 0;

	g_NESmapper.Mapper90.key_val = 0;
	g_NESmapper.Mapper90.mul_val1 = g_NESmapper.Mapper90.mul_val2 = 0;

	for( i = 0; i < 4; i++ ) {
		g_NESmapper.Mapper90.prg_reg[i] = g_NESmapper.num_8k_ROM_banks-4+i;
		g_NESmapper.Mapper90.ntl_reg[i] = 0;
		g_NESmapper.Mapper90.nth_reg[i] = 0;
		g_NESmapper.Mapper90.chl_reg[i] = i;
		g_NESmapper.Mapper90.chh_reg[i] = 0;
		g_NESmapper.Mapper90.chl_reg[i+4] = i+4;
		g_NESmapper.Mapper90.chh_reg[i+4] = 0;
	}

	if( g_NESmapper.Mapper90.sw_val )
		g_NESmapper.Mapper90.sw_val = 0x00;
	else
		g_NESmapper.Mapper90.sw_val = 0xFF;	
}

uint8 NES_mapper90_MemoryReadLow(uint32 addr)
{
	switch( addr ) {
		case	0x5000:
			return	g_NESmapper.Mapper90.sw_val?0x00:0xFF;
		case	0x5800:
			return	(uint8)(g_NESmapper.Mapper90.mul_val1*g_NESmapper.Mapper90.mul_val2);
		case	0x5801:
			return	(uint8)((g_NESmapper.Mapper90.mul_val1*g_NESmapper.Mapper90.mul_val2)>>8);
		case	0x5803:
			return	g_NESmapper.Mapper90.key_val;
	}

	if( addr >= 0x6000 ) {
	}
	return (uint8)(addr >> 8);
}

void NES_mapper90_MemoryWriteLow(uint32 addr, uint8 data)
{
	if( addr == 0x5800 ) {
		g_NESmapper.Mapper90.mul_val1 = data;
	} else
	if( addr == 0x5801 ) {
		g_NESmapper.Mapper90.mul_val2 = data;
	} else
	if( addr == 0x5803 ) {
		g_NESmapper.Mapper90.key_val = data;
	}
}

void NES_mapper90_MemoryWrite(uint32 addr, uint8 data)
{
	switch( addr & 0xF007 ) {
		case	0x8000:
		case	0x8001:
		case	0x8002:
		case	0x8003:
			g_NESmapper.Mapper90.prg_reg[addr&3] = data;
			NES_mapper90_SetBank_CPU();
			break;

		case	0x9000:
		case	0x9001:
		case	0x9002:
		case	0x9003:
		case	0x9004:
		case	0x9005:
		case	0x9006:
		case	0x9007:
			g_NESmapper.Mapper90.chl_reg[addr&7] = data;
			NES_mapper90_SetBank_PPU();
			break;

		case	0xA000:
		case	0xA001:
		case	0xA002:
		case	0xA003:
		case	0xA004:
		case	0xA005:
		case	0xA006:
		case	0xA007:
			g_NESmapper.Mapper90.chh_reg[addr&7] = data;
			NES_mapper90_SetBank_PPU();
			break;

		case	0xB000:
		case	0xB001:
		case	0xB002:
		case	0xB003:
			g_NESmapper.Mapper90.ntl_reg[addr&3] = data;
			NES_mapper90_SetBank_VRAM();
			break;

		case	0xB004:
		case	0xB005:
		case	0xB006:
		case	0xB007:
			g_NESmapper.Mapper90.nth_reg[addr&3] = data;
			NES_mapper90_SetBank_VRAM();
			break;

		case	0xC002:
			g_NESmapper.Mapper90.irq_enable = 0;
			g_NESmapper.Mapper90.irq_occur = 0;
			nes6502_ClrIRQ( IRQ_MAPPER );
			break;
		case	0xC003:
			g_NESmapper.Mapper90.irq_enable = 0xFF;
			g_NESmapper.Mapper90.irq_preset = 0xFF;
			break;
		case	0xC004:
			break;
		case	0xC005:
			if( g_NESmapper.Mapper90.irq_offset & 0x80 ) {
				g_NESmapper.Mapper90.irq_latch = data ^ (g_NESmapper.Mapper90.irq_offset | 1);
			} else {
				g_NESmapper.Mapper90.irq_latch = data | (g_NESmapper.Mapper90.irq_offset&0x27);
			}
			g_NESmapper.Mapper90.irq_preset = 0xFF;
			break;
		case	0xC006:
			if( g_NESmapper.Mapper90.patch ) {
				g_NESmapper.Mapper90.irq_offset = data;
			}
			break;

		case	0xD000:
			g_NESmapper.Mapper90.prg_6000 = data & 0x80;
			g_NESmapper.Mapper90.prg_E000 = data & 0x04;
			g_NESmapper.Mapper90.prg_size = data & 0x03;
			g_NESmapper.Mapper90.chr_size = (data & 0x18)>>3;
			g_NESmapper.Mapper90.mir_mode = data & 0x20;
			NES_mapper90_SetBank_CPU();
			NES_mapper90_SetBank_PPU();
			NES_mapper90_SetBank_VRAM();
			break;

		case	0xD001:
			g_NESmapper.Mapper90.mir_type = data & 0x03;
			NES_mapper90_SetBank_VRAM();
			break;

		case	0xD003:
			break;
	}
}

void NES_mapper90_HSync(uint32 scanline)
{
	if( (scanline >= 0 && scanline <= 239) ) {
		if(NES_PPU_spr_enabled() || NES_PPU_bg_enabled()) {
			if( g_NESmapper.Mapper90.irq_preset ) {
				g_NESmapper.Mapper90.irq_counter = g_NESmapper.Mapper90.irq_latch;
				g_NESmapper.Mapper90.irq_preset = 0;
			}
			if( g_NESmapper.Mapper90.irq_counter ) {
				g_NESmapper.Mapper90.irq_counter--;
			}
			if( !g_NESmapper.Mapper90.irq_counter ) {
				if( g_NESmapper.Mapper90.irq_enable ) {
					nes6502_SetIRQ( IRQ_MAPPER );
				}
			}
		}
	}
}


void NES_mapper90_SetBank_CPU()
{
	if( g_NESmapper.Mapper90.prg_size == 0 ) {
		g_NESmapper.SetPROM_32K_Banks( g_NESmapper.num_8k_ROM_banks-4, g_NESmapper.num_8k_ROM_banks-3, g_NESmapper.num_8k_ROM_banks-2, g_NESmapper.num_8k_ROM_banks-1 );
	} else
	if( g_NESmapper.Mapper90.prg_size == 1 ) {
		g_NESmapper.SetPROM_32K_Banks( g_NESmapper.Mapper90.prg_reg[1]*2, g_NESmapper.Mapper90.prg_reg[1]*2+1, g_NESmapper.num_8k_ROM_banks-2, g_NESmapper.num_8k_ROM_banks-1 );
	} else
	if( g_NESmapper.Mapper90.prg_size == 2 ) {
		if( g_NESmapper.Mapper90.prg_E000 ) {
			g_NESmapper.SetPROM_32K_Banks( g_NESmapper.Mapper90.prg_reg[0], g_NESmapper.Mapper90.prg_reg[1], g_NESmapper.Mapper90.prg_reg[2], g_NESmapper.Mapper90.prg_reg[3] );
		} else {
			if( g_NESmapper.Mapper90.prg_6000 ) {
				g_NESmapper.SetPROM_8K_Bank( 3, g_NESmapper.Mapper90.prg_reg[3] );
			}
			g_NESmapper.SetPROM_32K_Banks( g_NESmapper.Mapper90.prg_reg[0], g_NESmapper.Mapper90.prg_reg[1], g_NESmapper.Mapper90.prg_reg[2], g_NESmapper.num_8k_ROM_banks-1 );
		}
	} else {
		g_NESmapper.SetPROM_32K_Banks( g_NESmapper.Mapper90.prg_reg[3], g_NESmapper.Mapper90.prg_reg[2], g_NESmapper.Mapper90.prg_reg[1], g_NESmapper.Mapper90.prg_reg[0] );
	}
}

void NES_mapper90_SetBank_PPU()
{
	int	bank[8];
	int i;

	for( i = 0; i < 8; i++ ) {
		bank[i] = ((int)g_NESmapper.Mapper90.chh_reg[i]<<8)|((int)g_NESmapper.Mapper90.chl_reg[i]);
	}

	if( g_NESmapper.Mapper90.chr_size == 0 ) {
		g_NESmapper.SetVROM_8K_Bank( bank[0] );
	} else
	if( g_NESmapper.Mapper90.chr_size == 1 ) {
		g_NESmapper.SetVROM_4K_Bank( 0, bank[0] );
		g_NESmapper.SetVROM_4K_Bank( 4, bank[4] );
	} else
	if( g_NESmapper.Mapper90.chr_size == 2 ) {
		g_NESmapper.SetVROM_2K_Bank( 0, bank[0] );
		g_NESmapper.SetVROM_2K_Bank( 2, bank[2] );
		g_NESmapper.SetVROM_2K_Bank( 4, bank[4] );
		g_NESmapper.SetVROM_2K_Bank( 6, bank[6] );
	} else {
		g_NESmapper.SetVROM_8K_Banks( bank[0], bank[1], bank[2], bank[3], bank[4], bank[5], bank[6], bank[7] );
	}
}

void NES_mapper90_SetBank_VRAM()
{
	int	bank[4];
	int i;

	for(  i = 0; i < 4; i++ ) {
		bank[i] = ((int)g_NESmapper.Mapper90.nth_reg[i]<<8)|((int)g_NESmapper.Mapper90.ntl_reg[i]);
	}

	if( !g_NESmapper.Mapper90.patch && g_NESmapper.Mapper90.mir_mode ) {
		for( i = 0; i < 4; i++ ) {
			if( !g_NESmapper.Mapper90.nth_reg[i] && (g_NESmapper.Mapper90.ntl_reg[i] == (uint8)i) ) {
				g_NESmapper.Mapper90.mir_mode = 0;
			}
		}

		if( g_NESmapper.Mapper90.mir_mode ) {
			g_NESmapper.SetVROM_1K_Bank(  8, bank[0] );
			g_NESmapper.SetVROM_1K_Bank(  9, bank[1] );
			g_NESmapper.SetVROM_1K_Bank( 10, bank[2] );
			g_NESmapper.SetVROM_1K_Bank( 11, bank[3] );
		}
	} else {
		if( g_NESmapper.Mapper90.mir_type == 0 ) {
			g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
		} else
		if( g_NESmapper.Mapper90.mir_type == 1 ) {
			g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
		} else {
			g_NESmapper.set_mirroring(0,0,0,0);
		}
	}
}
/////////////////////////////////////////////////////////////////////

