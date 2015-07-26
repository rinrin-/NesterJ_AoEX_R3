
/////////////////////////////////////////////////////////////////////
// Mapper 12

void NES_mapper12_Init()
{
	g_NESmapper.Reset = NES_mapper12_Reset;
	g_NESmapper.MemoryWriteLow = NES_mapper12_MemoryWriteLow;
	g_NESmapper.MemoryReadLow = NES_mapper12_MemoryReadLow;
	g_NESmapper.MemoryWrite = NES_mapper12_MemoryWrite;
	g_NESmapper.HSync = NES_mapper12_HSync;
}


void	NES_mapper12_Reset()
{
	int i;
	for( i = 0; i < 8; i++ ) {
		g_NESmapper.Mapper12.reg[i] = 0x00;
	}
	
	//g_PPU.vram_write_protect = 0;

	g_NESmapper.Mapper12.prg0 = 0;
	g_NESmapper.Mapper12.prg1 = 1;
	NES_mapper12_MMC3_set_CPU_banks();

	g_NESmapper.Mapper12.vb0 = 0;
	g_NESmapper.Mapper12.vb1 = 0;
	g_NESmapper.Mapper12.chr01 = 0;
	g_NESmapper.Mapper12.chr23 = 2;
	g_NESmapper.Mapper12.chr4  = 4;
	g_NESmapper.Mapper12.chr5  = 5;
	g_NESmapper.Mapper12.chr6  = 6;
	g_NESmapper.Mapper12.chr7  = 7;
	NES_mapper12_MMC3_set_PPU_banks();

	g_NESmapper.Mapper12.we_sram  = 0;	// Disable
	g_NESmapper.Mapper12.irq_enable = 0;	// Disable
	g_NESmapper.Mapper12.irq_counter = 0;
	g_NESmapper.Mapper12.irq_latch = 0xFF;
	g_NESmapper.Mapper12.irq_request = 0;
	g_NESmapper.Mapper12.irq_preset = 0;
	g_NESmapper.Mapper12.irq_preset_vbl = 0;
}

uint8	NES_mapper12_MemoryReadLow( uint32 addr )
{
	return 0x01;
}

void NES_mapper12_MemoryWriteLow(uint32 addr, uint8 data)
{
	if( addr > 0x4100 && addr < 0x6000 ){
		g_NESmapper.Mapper12.vb0 = (data&0x01)<<8;
		g_NESmapper.Mapper12.vb1 = (data&0x10)<<4;
		NES_mapper12_MMC3_set_PPU_banks();
	}
}

void NES_mapper12_MemoryWrite(uint32 addr, uint8 data)
{
	switch( addr & 0xE001 )
	{
		case	0x8000:
			g_NESmapper.Mapper12.reg[0] = data;
			NES_mapper12_MMC3_set_CPU_banks();
			NES_mapper12_MMC3_set_PPU_banks();
			break;
		case	0x8001:
			g_NESmapper.Mapper12.reg[1] = data;

			switch( g_NESmapper.Mapper12.reg[0] & 0x07 ) {
				case	0x00:
					g_NESmapper.Mapper12.chr01 = data & 0xFE;
					NES_mapper12_MMC3_set_PPU_banks();
					break;
				case	0x01:
					g_NESmapper.Mapper12.chr23 = data & 0xFE;
					NES_mapper12_MMC3_set_PPU_banks();
					break;
				case	0x02:
					g_NESmapper.Mapper12.chr4 = data;
					NES_mapper12_MMC3_set_PPU_banks();
					break;
				case	0x03:
					g_NESmapper.Mapper12.chr5 = data;
					NES_mapper12_MMC3_set_PPU_banks();
					break;
				case	0x04:
					g_NESmapper.Mapper12.chr6 = data;
					NES_mapper12_MMC3_set_PPU_banks();
					break;
				case	0x05:
					g_NESmapper.Mapper12.chr7 = data;
					NES_mapper12_MMC3_set_PPU_banks();
					break;
				case	0x06:
					g_NESmapper.Mapper12.prg0 = data;
					NES_mapper12_MMC3_set_CPU_banks();
					break;
				case	0x07:
					g_NESmapper.Mapper12.prg1 = data;
					NES_mapper12_MMC3_set_CPU_banks();
					break;
			}
			break;
		case	0xA000:
			g_NESmapper.Mapper12.reg[2] = data;
			if(NES_ROM_get_mirroring() != NES_PPU_MIRROR_FOUR_SCREEN) {
				if( data & 0x01 )
					g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
				else
					g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
			}
			break;
		case	0xA001:
			g_NESmapper.Mapper12.reg[3] = data;
			break;
		case	0xC000:
			g_NESmapper.Mapper12.reg[4] = data;
			g_NESmapper.Mapper12.irq_latch = data;
			break;
		case	0xC001:
			g_NESmapper.Mapper12.reg[5] = data;
			if( NES_GetScanline() < 240 ) {
				g_NESmapper.Mapper12.irq_counter |= 0x80;
				g_NESmapper.Mapper12.irq_preset = 0xFF;
			} else {
				g_NESmapper.Mapper12.irq_counter |= 0x80;
				g_NESmapper.Mapper12.irq_preset_vbl = 0xFF;
				g_NESmapper.Mapper12.irq_preset = 0;
			}
			break;
		case	0xE000:
			g_NESmapper.Mapper12.reg[6] = data;
			g_NESmapper.Mapper12.irq_enable = 0;
			g_NESmapper.Mapper12.irq_request = 0;

			nes6502_ClrIRQ( IRQ_MAPPER );
			break;
		case	0xE001:
			g_NESmapper.Mapper12.reg[7] = data;
			g_NESmapper.Mapper12.irq_enable = 1;
			g_NESmapper.Mapper12.irq_request = 0;
			break;
	}
}

void NES_mapper12_HSync(uint32 scanline)
{
	if( (scanline >= 0 && scanline <= 239) && (NES_PPU_spr_enabled() || NES_PPU_bg_enabled()) ) {
		if( g_NESmapper.Mapper12.irq_preset_vbl ) {
			g_NESmapper.Mapper12.irq_counter = g_NESmapper.Mapper12.irq_latch;
			g_NESmapper.Mapper12.irq_preset_vbl = 0;
		}
		if( g_NESmapper.Mapper12.irq_preset ) {
			g_NESmapper.Mapper12.irq_counter = g_NESmapper.Mapper12.irq_latch;
			g_NESmapper.Mapper12.irq_preset = 0;
		} else if( g_NESmapper.Mapper12.irq_counter > 0 ) {
			g_NESmapper.Mapper12.irq_counter--;
		}

		if( g_NESmapper.Mapper12.irq_counter == 0 ) {
			// Some game set irq_latch to zero to disable irq. So check it here.
			if( g_NESmapper.Mapper12.irq_enable && g_NESmapper.Mapper12.irq_latch ) {
				g_NESmapper.Mapper12.irq_request = 0xFF;
				nes6502_SetIRQ( IRQ_MAPPER );
			}
			g_NESmapper.Mapper12.irq_preset = 0xFF;
		}
	}
}

void	NES_mapper12_MMC3_set_CPU_banks()
{
	if( g_NESmapper.Mapper12.reg[0] & 0x40 ) {
		g_NESmapper.SetPROM_32K_Banks( g_NESmapper.num_8k_ROM_banks-2, g_NESmapper.Mapper12.prg1, g_NESmapper.Mapper12.prg0, g_NESmapper.num_8k_ROM_banks-1 );
	} else {
		g_NESmapper.SetPROM_32K_Banks( g_NESmapper.Mapper12.prg0, g_NESmapper.Mapper12.prg1, g_NESmapper.num_8k_ROM_banks-2, g_NESmapper.num_8k_ROM_banks-1 );
	}
}

void	NES_mapper12_MMC3_set_PPU_banks()
{
	if( g_NESmapper.num_1k_VROM_banks ) {
		if( g_NESmapper.Mapper12.reg[0] & 0x80 ) {
			g_NESmapper.SetVROM_8K_Banks( g_NESmapper.Mapper12.vb0+g_NESmapper.Mapper12.chr4,
							 g_NESmapper.Mapper12.vb0+g_NESmapper.Mapper12.chr5,
							 g_NESmapper.Mapper12.vb0+g_NESmapper.Mapper12.chr6,
							 g_NESmapper.Mapper12.vb0+g_NESmapper.Mapper12.chr7,
							 g_NESmapper.Mapper12.vb1+g_NESmapper.Mapper12.chr01,
							 g_NESmapper.Mapper12.vb1+g_NESmapper.Mapper12.chr01+1,
							 g_NESmapper.Mapper12.vb1+g_NESmapper.Mapper12.chr23,
							 g_NESmapper.Mapper12.vb1+g_NESmapper.Mapper12.chr23+1 );
		} else {
			g_NESmapper.SetVROM_8K_Banks( g_NESmapper.Mapper12.vb0+g_NESmapper.Mapper12.chr01,
							 g_NESmapper.Mapper12.vb0+g_NESmapper.Mapper12.chr01+1,
							 g_NESmapper.Mapper12.vb0+g_NESmapper.Mapper12.chr23,
							 g_NESmapper.Mapper12.vb0+g_NESmapper.Mapper12.chr23+1,
							 g_NESmapper.Mapper12.vb1+g_NESmapper.Mapper12.chr4,
							 g_NESmapper.Mapper12.vb1+g_NESmapper.Mapper12.chr5,
							 g_NESmapper.Mapper12.vb1+g_NESmapper.Mapper12.chr6,
							 g_NESmapper.Mapper12.vb1+g_NESmapper.Mapper12.chr7 );
		}
	} else {
		if( g_NESmapper.Mapper12.reg[0] & 0x80 ) {
			g_NESmapper.SetCRAM_1K_Bank( 4, (g_NESmapper.Mapper12.chr01+0)&0x07 );
			g_NESmapper.SetCRAM_1K_Bank( 5, (g_NESmapper.Mapper12.chr01+1)&0x07 );
			g_NESmapper.SetCRAM_1K_Bank( 6, (g_NESmapper.Mapper12.chr23+0)&0x07 );
			g_NESmapper.SetCRAM_1K_Bank( 7, (g_NESmapper.Mapper12.chr23+1)&0x07 );
			g_NESmapper.SetCRAM_1K_Bank( 0, g_NESmapper.Mapper12.chr4&0x07 );
			g_NESmapper.SetCRAM_1K_Bank( 1, g_NESmapper.Mapper12.chr5&0x07 );
			g_NESmapper.SetCRAM_1K_Bank( 2, g_NESmapper.Mapper12.chr6&0x07 );
			g_NESmapper.SetCRAM_1K_Bank( 3, g_NESmapper.Mapper12.chr7&0x07 );
		} else {
			g_NESmapper.SetCRAM_1K_Bank( 0, (g_NESmapper.Mapper12.chr01+0)&0x07 );
			g_NESmapper.SetCRAM_1K_Bank( 1, (g_NESmapper.Mapper12.chr01+1)&0x07 );
			g_NESmapper.SetCRAM_1K_Bank( 2, (g_NESmapper.Mapper12.chr23+0)&0x07 );
			g_NESmapper.SetCRAM_1K_Bank( 3, (g_NESmapper.Mapper12.chr23+1)&0x07 );
			g_NESmapper.SetCRAM_1K_Bank( 4, g_NESmapper.Mapper12.chr4&0x07 );
			g_NESmapper.SetCRAM_1K_Bank( 5, g_NESmapper.Mapper12.chr5&0x07 );
			g_NESmapper.SetCRAM_1K_Bank( 6, g_NESmapper.Mapper12.chr6&0x07 );
			g_NESmapper.SetCRAM_1K_Bank( 7, g_NESmapper.Mapper12.chr7&0x07 );
		}
	}
}
/////////////////////////////////////////////////////////////////////

