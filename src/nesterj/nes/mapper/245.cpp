/////////////////////////////////////////////////////////////////////
// Mapper 245
void NES_mapper245_Init()
{
	g_NESmapper.Reset = NES_mapper245_Reset;
	g_NESmapper.MemoryWrite = NES_mapper245_MemoryWrite;
	g_NESmapper.HSync = NES_mapper245_HSync;
}

void NES_mapper245_Reset()
{
	int i;
	for(i = 0; i < 8; i++ ) {
		g_NESmapper.Mapper245.reg[i] = 0x00;
	}

	g_NESmapper.Mapper245.prg0 = 0;
	g_NESmapper.Mapper245.prg1 = 1;

	g_NESmapper.SetPROM_32K_Banks( 0, 1, g_NESmapper.num_8k_ROM_banks-2, g_NESmapper.num_8k_ROM_banks-1 );

	if( g_NESmapper.num_1k_VROM_banks ) {
		g_NESmapper.SetVROM_8K_Bank( 0 );
	}

	g_NESmapper.Mapper245.we_sram  = 0;	// Disable
	g_NESmapper.Mapper245.irq_enable = 0;	// Disable
	g_NESmapper.Mapper245.irq_counter = 0;
	g_NESmapper.Mapper245.irq_latch = 0;
	g_NESmapper.Mapper245.irq_request = 0;	
	
	if(NES_crc32()==0x9767DC74)
	{
		g_PPU.vram_write_protect = 0;//V英烈群侠传 - 外星科技.NES
	}
	//nes->SetIrqType( NES::IRQ_CLOCK );
}

void NES_mapper245_MemoryWrite(uint32 addr, uint8 data)
{
	switch( addr&0xF7FF ) {
		case	0x8000:
			g_NESmapper.Mapper245.reg[0] = data;
			break;
		case	0x8001:
			g_NESmapper.Mapper245.reg[1] = data;
			switch( g_NESmapper.Mapper245.reg[0] ) {
				case	0x00:
					g_NESmapper.Mapper245.reg[3]=(data & 2 )<<5;
					g_NESmapper.SetPROM_8K_Bank(6,0x3E | g_NESmapper.Mapper245.reg[3]);
					g_NESmapper.SetPROM_8K_Bank(7,0x3F | g_NESmapper.Mapper245.reg[3]);
					break;
				case	0x06:
					g_NESmapper.Mapper245.prg0=data;
					break;
				case	0x07:
					g_NESmapper.Mapper245.prg1=data;
					break;
			}
			g_NESmapper.SetPROM_8K_Bank( 4, g_NESmapper.Mapper245.prg0|g_NESmapper.Mapper245.reg[3] );
			g_NESmapper.SetPROM_8K_Bank( 5, g_NESmapper.Mapper245.prg1|g_NESmapper.Mapper245.reg[3] );
			break;
		case	0xA000:
			g_NESmapper.Mapper245.reg[2] = data;
			if(NES_ROM_get_mirroring() != NES_PPU_MIRROR_FOUR_SCREEN) {
				if( data & 0x01 ) 
					g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
				else		  
					g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
			}
			break;
		case	0xA001:
		    
			break;
		case	0xC000:
			g_NESmapper.Mapper245.reg[4] = data;
			g_NESmapper.Mapper245.irq_counter = data;
			g_NESmapper.Mapper245.irq_request = 0;
			nes6502_ClrIRQ( IRQ_MAPPER );
			break;
		case	0xC001:
			g_NESmapper.Mapper245.reg[5] = data;
			g_NESmapper.Mapper245.irq_latch = data;
			g_NESmapper.Mapper245.irq_request = 0;
			nes6502_ClrIRQ( IRQ_MAPPER );
			break;
		case	0xE000:
			g_NESmapper.Mapper245.reg[6] = data;
			g_NESmapper.Mapper245.irq_enable = 0;
			g_NESmapper.Mapper245.irq_request = 0;
			nes6502_ClrIRQ( IRQ_MAPPER );
			break;
		case	0xE001:
			g_NESmapper.Mapper245.reg[7] = data;
			g_NESmapper.Mapper245.irq_enable = 1;
			g_NESmapper.Mapper245.irq_request = 0;
			nes6502_ClrIRQ( IRQ_MAPPER );
			break;
	}
}

void NES_mapper245_HSync(uint32 scanline)
{
	if( (scanline >= 0 && scanline <= 239) ) {
		if( NES_PPU_spr_enabled() || NES_PPU_bg_enabled() ) {
			if( g_NESmapper.Mapper245.irq_enable && !g_NESmapper.Mapper245.irq_request ) {
				if( scanline == 0 ) {
					if( g_NESmapper.Mapper245.irq_counter ) {
						g_NESmapper.Mapper245.irq_counter--;
					}
				}
				if( !(g_NESmapper.Mapper245.irq_counter--) ) {
					g_NESmapper.Mapper245.irq_request = 0xFF;
					g_NESmapper.Mapper245.irq_counter = g_NESmapper.Mapper245.irq_latch;
					nes6502_SetIRQ( IRQ_MAPPER );
				}
			}
		}
	}
}

void NES_mapper245_SetBank_CPU()
{
	g_NESmapper.SetPROM_32K_Banks( g_NESmapper.Mapper245.prg0, g_NESmapper.Mapper245.prg1, g_NESmapper.num_8k_ROM_banks-2, g_NESmapper.num_8k_ROM_banks-1 );
}

void NES_mapper245_SetBank_PPU()
{
	if( g_NESmapper.num_1k_VROM_banks ) {
		if( g_NESmapper.Mapper245.reg[0] & 0x80 ) {
			g_NESmapper.SetVROM_8K_Banks( g_NESmapper.Mapper245.chr4, g_NESmapper.Mapper245.chr5, g_NESmapper.Mapper245.chr6, g_NESmapper.Mapper245.chr7,
					 g_NESmapper.Mapper245.chr23+1, g_NESmapper.Mapper245.chr23, g_NESmapper.Mapper245.chr01+1, g_NESmapper.Mapper245.chr01 );
		} else {
			g_NESmapper.SetVROM_8K_Banks( g_NESmapper.Mapper245.chr01, g_NESmapper.Mapper245.chr01+1, g_NESmapper.Mapper245.chr23, g_NESmapper.Mapper245.chr23+1,
					 g_NESmapper.Mapper245.chr4, g_NESmapper.Mapper245.chr5, g_NESmapper.Mapper245.chr6, g_NESmapper.Mapper245.chr7 );
		}
	} else {
		if( g_NESmapper.Mapper245.reg[0] & 0x80 ) {
			g_NESmapper.SetCRAM_1K_Bank( 4, (g_NESmapper.Mapper245.chr01+0)&0x07 );
			g_NESmapper.SetCRAM_1K_Bank( 5, (g_NESmapper.Mapper245.chr01+1)&0x07 );
			g_NESmapper.SetCRAM_1K_Bank( 6, (g_NESmapper.Mapper245.chr23+0)&0x07 );
			g_NESmapper.SetCRAM_1K_Bank( 7, (g_NESmapper.Mapper245.chr23+1)&0x07 );
			g_NESmapper.SetCRAM_1K_Bank( 0, g_NESmapper.Mapper245.chr4&0x07 );
			g_NESmapper.SetCRAM_1K_Bank( 1, g_NESmapper.Mapper245.chr5&0x07 );
			g_NESmapper.SetCRAM_1K_Bank( 2, g_NESmapper.Mapper245.chr6&0x07 );
			g_NESmapper.SetCRAM_1K_Bank( 3, g_NESmapper.Mapper245.chr7&0x07 );
		} else {              
			g_NESmapper.SetCRAM_1K_Bank( 0, (g_NESmapper.Mapper245.chr01+0)&0x07 );
			g_NESmapper.SetCRAM_1K_Bank( 1, (g_NESmapper.Mapper245.chr01+1)&0x07 );
			g_NESmapper.SetCRAM_1K_Bank( 2, (g_NESmapper.Mapper245.chr23+0)&0x07 );
			g_NESmapper.SetCRAM_1K_Bank( 3, (g_NESmapper.Mapper245.chr23+1)&0x07 );
			g_NESmapper.SetCRAM_1K_Bank( 4, g_NESmapper.Mapper245.chr4&0x07 );
			g_NESmapper.SetCRAM_1K_Bank( 5, g_NESmapper.Mapper245.chr5&0x07 );
			g_NESmapper.SetCRAM_1K_Bank( 6, g_NESmapper.Mapper245.chr6&0x07 );
			g_NESmapper.SetCRAM_1K_Bank( 7, g_NESmapper.Mapper245.chr7&0x07 );
		}                     
	}
}


/////////////////////////////////////////////////////////////////////

