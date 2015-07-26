
/////////////////////////////////////////////////////////////////////
// Mapper 189
void NES_mapper189_Init()
{
	g_NESmapper.Reset = NES_mapper189_Reset;
	g_NESmapper.MemoryWriteLow = NES_mapper189_MemoryWriteLow;
	g_NESmapper.MemoryWriteSaveRAM = NES_mapper189_MemoryWriteSaveRAM;
	g_NESmapper.MemoryWrite = NES_mapper189_MemoryWrite;
	g_NESmapper.HSync = NES_mapper189_HSync;
}

void NES_mapper189_Reset()
{
	g_NESmapper.SetPROM_32K_Banks( g_NESmapper.num_8k_ROM_banks-4,
								g_NESmapper.num_8k_ROM_banks-3,
								g_NESmapper.num_8k_ROM_banks-2,
								g_NESmapper.num_8k_ROM_banks-1);

	if( g_NESmapper.num_1k_VROM_banks ) {
		g_NESmapper.SetVROM_8K_Bank( 0 );
	}

	g_NESmapper.Mapper189.regs[0] = 0;
	g_NESmapper.Mapper189.regs[1] = 0;

	g_NESmapper.Mapper189.chr01 = 0;
	g_NESmapper.Mapper189.chr23 = 2;
	g_NESmapper.Mapper189.chr4  = 4;
	g_NESmapper.Mapper189.chr5  = 5;
	g_NESmapper.Mapper189.chr6  = 6;
	g_NESmapper.Mapper189.chr7  = 7;
	NES_mapper189_MMC3_set_PPU_banks();

	g_NESmapper.Mapper189.irq_enabled = 0;
	g_NESmapper.Mapper189.irq_counter = 0;
	g_NESmapper.Mapper189.irq_latch   = 0;

	int i;
	for( i = 0; i < 4; i++ ) {
		g_NESmapper.Mapper189.protect_dat[i] = 0;
	}
	g_NESmapper.Mapper189.lwd = 0xFF;

	g_NESmapper.Mapper189.patch = 0;
	if( (NES_crc32() == 0x20ca2ad3) ) {	// Street Fighter IV (GOUDER)
		g_NESmapper.Mapper189.patch = 1;
		g_NES.frame_irq_disenabled = 1;//²Î¿¼¾ª·çÌí¼Ó
		g_NESmapper.SetPROM_32K_Bank( 0 );

		g_NESmapper.Mapper189.wram = g_NES.mapper_extram;
		g_NES.mapper_extramsize = 0x2000;		
		g_NESmapper.SetPROM_Bank( 2, g_NESmapper.Mapper189.wram);
		// $4000-$5FFF
	}
	
}


void NES_mapper189_MemoryWriteLow(uint32 addr, uint8 data)
{
	if( (addr & 0xFF00) == 0x4100 ) {
	// Street Fighter 2 YOKO
		g_NESmapper.SetPROM_32K_Bank( (data&0x30)>>4 );	
	} else if( (addr & 0xFF00) == 0x6100 ) {         
	// Master Fighter 2
		g_NESmapper.SetPROM_32K_Bank( data&0x03 );
	}

	if( g_NESmapper.Mapper189.patch ) {
	// Street Fighter IV (GOUDER)
		uint8	a5000xordat[256] = {
			0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x49, 0x19, 0x09, 0x59, 0x49, 0x19, 0x09,
			0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x51, 0x41, 0x11, 0x01, 0x51, 0x41, 0x11, 0x01,
			0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x49, 0x19, 0x09, 0x59, 0x49, 0x19, 0x09,
			0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x51, 0x41, 0x11, 0x01, 0x51, 0x41, 0x11, 0x01,
			0x00, 0x10, 0x40, 0x50, 0x00, 0x10, 0x40, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x08, 0x18, 0x48, 0x58, 0x08, 0x18, 0x48, 0x58, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x10, 0x40, 0x50, 0x00, 0x10, 0x40, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x08, 0x18, 0x48, 0x58, 0x08, 0x18, 0x48, 0x58, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x58, 0x48, 0x18, 0x08, 0x58, 0x48, 0x18, 0x08,
			0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x50, 0x40, 0x10, 0x00, 0x50, 0x40, 0x10, 0x00,
			0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x58, 0x48, 0x18, 0x08, 0x58, 0x48, 0x18, 0x08,
			0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x50, 0x40, 0x10, 0x00, 0x50, 0x40, 0x10, 0x00,
			0x01, 0x11, 0x41, 0x51, 0x01, 0x11, 0x41, 0x51, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x09, 0x19, 0x49, 0x59, 0x09, 0x19, 0x49, 0x59, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x01, 0x11, 0x41, 0x51, 0x01, 0x11, 0x41, 0x51, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x09, 0x19, 0x49, 0x59, 0x09, 0x19, 0x49, 0x59, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
		};

		if( (addr >= 0x4800) && (addr <= 0x4FFF) ) {
			g_NESmapper.SetPROM_32K_Bank( ((data&0x10)>>3)+(data&0x1) );
                                                                
			if(NES_ROM_get_mirroring() != NES_PPU_MIRROR_FOUR_SCREEN)
			{
				if(data & 0x20)
				{
					g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
				}
				else
				{
					g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
				}
			}
		}
		if( (addr>=0x5000) && (addr<=0x57FF) ) {
			g_NESmapper.Mapper189.lwd = data;
		}
		if( (addr>=0x5800) && (addr<=0x5FFF) ) {
			g_NESmapper.Mapper189.wram[0x1800+(addr & 3)] = 
			g_NESmapper.Mapper189.protect_dat[ addr & 3 ] = data ^ a5000xordat[ g_NESmapper.Mapper189.lwd ];
		}
	}
}

void NES_mapper189_MemoryWriteSaveRAM(uint32 addr, uint8 data)
{
	if( (addr & 0xFF00) == 0x6100 ) {
	// Master Fighter 2
		uint8 tempdata2;
		tempdata2 = data & 0x03;
		g_NESmapper.set_CPU_banks4(tempdata2*4,tempdata2*4+1,tempdata2*4+2,tempdata2*4+3);
	}
}
void NES_mapper189_MemoryWrite(uint32 addr, uint8 data)
{
	switch( addr&0xE001 ) {
		case	0x8000:
			g_NESmapper.Mapper189.regs[0] = data;
			NES_mapper189_MMC3_set_PPU_banks();
			break;

		case	0x8001:
			g_NESmapper.Mapper189.regs[1] = data;
			NES_mapper189_MMC3_set_PPU_banks();
			switch( g_NESmapper.Mapper189.regs[0] & 0x07 ) {
				case	0x00:
					g_NESmapper.Mapper189.chr01 = data & 0xFE;
					NES_mapper189_MMC3_set_PPU_banks();
					break;
				case	0x01:
					g_NESmapper.Mapper189.chr23 = data & 0xFE;
					NES_mapper189_MMC3_set_PPU_banks();
					break;
				case	0x02:
					g_NESmapper.Mapper189.chr4 = data;
					NES_mapper189_MMC3_set_PPU_banks();
					break;
				case	0x03:
					g_NESmapper.Mapper189.chr5 = data;
					NES_mapper189_MMC3_set_PPU_banks();
					break;
				case	0x04:
					g_NESmapper.Mapper189.chr6 = data;
					NES_mapper189_MMC3_set_PPU_banks();
					break;
				case	0x05:
					g_NESmapper.Mapper189.chr7 = data;
					NES_mapper189_MMC3_set_PPU_banks();
					break;
			}
			break;

		case	0xA000:
			if( data&0x01 ) //SetVRAM_Mirror( VRAM_HMIRROR );			
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
			else
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
			break;

		case	0xC000:
			g_NESmapper.Mapper189.irq_counter = data;
			break;
		case	0xC001:
			g_NESmapper.Mapper189.irq_latch = data;
			break;
		case	0xE000:
			g_NESmapper.Mapper189.irq_enabled = 0;
			nes6502_ClrIRQ( IRQ_MAPPER );	
			break;
		case	0xE001:
			g_NESmapper.Mapper189.irq_enabled = 0xFF;
			break;
	}	
}

void NES_mapper189_HSync(uint32 scanline)
{
	if( (scanline >= 0 && scanline <= 239) ) {
		if(NES_PPU_spr_enabled() || NES_PPU_bg_enabled()){
			if( g_NESmapper.Mapper189.irq_enabled ) {
				if( !(--g_NESmapper.Mapper189.irq_counter) ) {
					g_NESmapper.Mapper189.irq_counter = g_NESmapper.Mapper189.irq_latch;
					nes6502_SetIRQ( IRQ_MAPPER );
				}
			}
		}
	}
}

void NES_mapper189_MMC3_set_PPU_banks()
{
	if( g_NESmapper.Mapper189.patch )
	{
		g_NESmapper.SetVROM_8K_Banks( g_NESmapper.Mapper189.chr01,
								g_NESmapper.Mapper189.chr01+1,
								g_NESmapper.Mapper189.chr23,
								g_NESmapper.Mapper189.chr23+1,
								g_NESmapper.Mapper189.chr4,
								g_NESmapper.Mapper189.chr5,
								g_NESmapper.Mapper189.chr6,
								g_NESmapper.Mapper189.chr7 );
	} else {
		if( g_NESmapper.num_1k_VROM_banks ) {
			if( g_NESmapper.Mapper189.regs[0] & 0x80 ) {
				g_NESmapper.SetVROM_8K_Banks( g_NESmapper.Mapper189.chr4, g_NESmapper.Mapper189.chr5, g_NESmapper.Mapper189.chr6, g_NESmapper.Mapper189.chr7,
						 g_NESmapper.Mapper189.chr01, g_NESmapper.Mapper189.chr01+1, g_NESmapper.Mapper189.chr23, g_NESmapper.Mapper189.chr23+1 );
			} else {
				g_NESmapper.SetVROM_8K_Banks( g_NESmapper.Mapper189.chr01, g_NESmapper.Mapper189.chr01+1, g_NESmapper.Mapper189.chr23, g_NESmapper.Mapper189.chr23+1,
						 g_NESmapper.Mapper189.chr4, g_NESmapper.Mapper189.chr5, g_NESmapper.Mapper189.chr6, g_NESmapper.Mapper189.chr7 );
			}
		} else {
			if( g_NESmapper.Mapper189.regs[0] & 0x80 ) {
				g_NESmapper.SetCRAM_1K_Bank( 4, (g_NESmapper.Mapper189.chr01+0)&0x07 );
				g_NESmapper.SetCRAM_1K_Bank( 5, (g_NESmapper.Mapper189.chr01+1)&0x07 );
				g_NESmapper.SetCRAM_1K_Bank( 6, (g_NESmapper.Mapper189.chr23+0)&0x07 );
				g_NESmapper.SetCRAM_1K_Bank( 7, (g_NESmapper.Mapper189.chr23+1)&0x07 );
				g_NESmapper.SetCRAM_1K_Bank( 0, g_NESmapper.Mapper189.chr4&0x07 );
				g_NESmapper.SetCRAM_1K_Bank( 1, g_NESmapper.Mapper189.chr5&0x07 );
				g_NESmapper.SetCRAM_1K_Bank( 2, g_NESmapper.Mapper189.chr6&0x07 );
				g_NESmapper.SetCRAM_1K_Bank( 3, g_NESmapper.Mapper189.chr7&0x07 );
			} else {
				g_NESmapper.SetCRAM_1K_Bank( 0, (g_NESmapper.Mapper189.chr01+0)&0x07 );
				g_NESmapper.SetCRAM_1K_Bank( 1, (g_NESmapper.Mapper189.chr01+1)&0x07 );
				g_NESmapper.SetCRAM_1K_Bank( 2, (g_NESmapper.Mapper189.chr23+0)&0x07 );
				g_NESmapper.SetCRAM_1K_Bank( 3, (g_NESmapper.Mapper189.chr23+1)&0x07 );
				g_NESmapper.SetCRAM_1K_Bank( 4, g_NESmapper.Mapper189.chr4&0x07 );
				g_NESmapper.SetCRAM_1K_Bank( 5, g_NESmapper.Mapper189.chr5&0x07 );
				g_NESmapper.SetCRAM_1K_Bank( 6, g_NESmapper.Mapper189.chr6&0x07 );
				g_NESmapper.SetCRAM_1K_Bank( 7, g_NESmapper.Mapper189.chr7&0x07 );
			}   
		}
	}
}


/////////////////////////////////////////////////////////////////////

