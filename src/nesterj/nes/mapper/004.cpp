//////////////////////////////////////////////////////////////////////////
// Mapper004  Nintendo MMC3                                             //
//////////////////////////////////////////////////////////////////////////
#define	MMC3_IRQ_KLAX			1
#define	MMC3_IRQ_SHOUGIMEIKAN	2
#define	MMC3_IRQ_DAI2JISUPER	3
#define	MMC3_IRQ_DBZ2			4
#define	MMC3_IRQ_ROCKMAN3		5

void NES_mapper4_Init()
{
	g_NESmapper.Reset = NES_mapper4_Reset;
	g_NESmapper.MemoryWriteLow = NES_mapper4_MemoryWriteLow;
	g_NESmapper.MemoryReadLow = NES_mapper4_MemoryReadLow;
	g_NESmapper.MemoryWrite = NES_mapper4_MemoryWrite;
	g_NESmapper.HSync = NES_mapper4_HSync;
}


void	NES_mapper4_Reset()
{
	int i;
	for( i = 0; i < 8; i++ ) {
		g_NESmapper.Mapper4.reg[i] = 0x00;
	}

	g_NESmapper.Mapper4.prg0 = 0;
	g_NESmapper.Mapper4.prg1 = 1;
	NES_mapper4_MMC3_set_CPU_banks();

	g_NESmapper.Mapper4.chr01 = 0;
	g_NESmapper.Mapper4.chr23 = 2;
	g_NESmapper.Mapper4.chr4  = 4;
	g_NESmapper.Mapper4.chr5  = 5;
	g_NESmapper.Mapper4.chr6  = 6;
	g_NESmapper.Mapper4.chr7  = 7;
	NES_mapper4_MMC3_set_PPU_banks();

	//g_NESmapper.Mapper4.we_sram  = 0;	// Disable
	g_NESmapper.Mapper4.irq_enable = 0;	// Disable
	g_NESmapper.Mapper4.irq_counter = 0;
	g_NESmapper.Mapper4.irq_latch = 0xFF;
	g_NESmapper.Mapper4.irq_request = 0;
	g_NESmapper.Mapper4.irq_preset = 0;
	g_NESmapper.Mapper4.irq_preset_vbl = 0;

	g_NESmapper.Mapper4.rom_type = 0;
	g_NESmapper.Mapper4.security = 0;

	// IRQƒ^ƒCƒvÝ’è
	//nes->SetIrqType( NES::IRQ_CLOCK );
	g_NESmapper.Mapper4.irq_type = 0;

	if( NES_crc32() == 0x8bc9ec42 ) {	// "Contra Fighter (Unl) [U]"
		g_NESmapper.Mapper4.rom_type = 1;
	}
	if( NES_crc32() == 0xA9B36A7D ) {	// Feng Yun (C)
		g_NESmapper.Mapper4.rom_type = 2;
	}
	if( NES_crc32() == 0x5c707ac4 ) {	// Mother(J)
		//nes->SetIrqType( NES::IRQ_HSYNC );
		//nes->SetRenderMethod( NES::TILE_RENDER );
	}
	if( NES_crc32() == 0xcb106f49 ) {	// F-1 Sensation(J)
		//nes->SetRenderMethod( NES::TILE_RENDER );
	}
	if( NES_crc32() == 0x1170392a ) {	// Karakuri Kengou Den - Musashi Road - Karakuri Nin Hashiru!(J)
		//nes->SetIrqType( NES::IRQ_HSYNC );
	}
	if( NES_crc32() == 0x14a01c70 ) {	// Gun-Dec(J)
		//nes->SetRenderMethod( NES::TILE_RENDER );
	}
	if( NES_crc32() == 0xeffeea40 ) {	// For Klax(J)
		g_NESmapper.Mapper4.irq_type = MMC3_IRQ_KLAX;
		//nes->SetIrqType( NES::IRQ_HSYNC );
		//nes->SetRenderMethod( NES::TILE_RENDER );
	}
	if( NES_crc32() == 0xc17ae2dc ) {	// God Slayer - Haruka Tenkuu no Sonata(J)
		//nes->SetRenderMethod( NES::TILE_RENDER );
	}
	if( NES_crc32() == 0x126ea4a0 ) {	// Summer Carnival '92 - Recca(J)
		//nes->SetIrqType( NES::IRQ_HSYNC );
	}
	if( NES_crc32() == 0x1f2f4861 ) {	// J League Fighting Soccer - The King of Ace Strikers(J)
		//nes->SetIrqType( NES::IRQ_HSYNC );
	}
	if( NES_crc32() == 0x5a6860f1 ) {	// Shougi Meikan '92(J)
		g_NESmapper.Mapper4.irq_type = MMC3_IRQ_SHOUGIMEIKAN;
		//nes->SetIrqType( NES::IRQ_HSYNC );
	}
	if( NES_crc32() == 0xae280e20 ) {	// Shougi Meikan '93(J)
		g_NESmapper.Mapper4.irq_type = MMC3_IRQ_SHOUGIMEIKAN;
		//nes->SetIrqType( NES::IRQ_HSYNC );
	}
	if( NES_crc32() == 0xe19a2473 ) {	// Sugoro Quest - Dice no Senshi Tachi(J)
		//nes->SetIrqType( NES::IRQ_HSYNC );
		//nes->SetRenderMethod( NES::TILE_RENDER );
	}
	if( NES_crc32() == 0x702d9b33 ) {	// Star Wars - The Empire Strikes Back(Victor)(J)
		//nes->SetIrqType( NES::IRQ_HSYNC );
	}
	if( NES_crc32() == 0xa9a0d729 ) {	// Dai Kaijuu - Deburas(J)
		//nes->SetRenderMethod( NES::TILE_RENDER );
	}
	if( NES_crc32() == 0xc5fea9f2 ) {	// Dai 2 Ji - Super Robot Taisen(J)
		g_NESmapper.Mapper4.irq_type = MMC3_IRQ_DAI2JISUPER;
	}
	if( NES_crc32() == 0xd852c2f7 ) {	// Time Zone(J)
		//nes->SetRenderMethod( NES::TILE_RENDER );
	}
	if( NES_crc32() == 0xecfd3c69 ) {	// Taito Chase H.Q.(J)
		//nes->SetRenderMethod( NES::TILE_RENDER );
	}
	if( NES_crc32() == 0x7a748058 ) {	// Tom & Jerry (and Tuffy)(J)
		//nes->SetIrqType( NES::IRQ_HSYNC );
	}
	if( NES_crc32() == 0xaafe699c ) {	// Ninja Ryukenden 3 - Yomi no Hakobune(J)
		//nes->SetRenderMethod( NES::TILE_RENDER );
	}
	if( NES_crc32() == 0x6cc62c06 ) {	// Hoshi no Kirby - Yume no Izumi no Monogatari(J)
		//nes->SetRenderMethod( NES::TILE_RENDER );
	}
	if( NES_crc32() == 0x877dba77 ) {	// My Life My Love - Boku no Yume - Watashi no Negai(J)
		//nes->SetIrqType( NES::IRQ_HSYNC );
	}
	if( NES_crc32() == 0x6f96ed15 ) {	// Max Warrior - Wakusei Kaigenrei(J)
		//nes->SetIrqType( NES::IRQ_HSYNC );
	}
	if( NES_crc32() == 0x8685f366 ) {	// Matendouji(J)
		//nes->SetRenderMethod( NES::TILE_RENDER );
	}
	if( NES_crc32() == 0x8635fed1 ) {	// Mickey Mouse 3 - Yume Fuusen(J)
		//nes->SetRenderMethod( NES::TILE_RENDER );
	}
	if( NES_crc32() == 0x26ff3ea2 ) {	// Yume Penguin Monogatari(J)
		//nes->SetIrqType( NES::IRQ_HSYNC );
	}
	if( NES_crc32() == 0x7671bc51 ) {	// Red Ariimaa 2(J)
		//nes->SetIrqType( NES::IRQ_HSYNC );
	}
	if( NES_crc32() == 0xade11141 ) {	// Wanpaku Kokkun no Gourmet World(J)
		//nes->SetIrqType( NES::IRQ_HSYNC );
	}
	if( NES_crc32() == 0x7c7ab58e ) {	// Walkuere no Bouken - Toki no Kagi Densetsu(J)
		//nes->SetRenderMethod( NES::POST_RENDER );
	}
	if( NES_crc32() == 0x26ff3ea2 ) {	// Yume Penguin Monogatari(J)
		//nes->SetRenderMethod( NES::TILE_RENDER );
	}
	if( NES_crc32() == 0x126ea4a0 ) {	// Summer Carnival '92 Recca(J)
		//nes->SetRenderMethod( NES::TILE_RENDER );
	}

	if( NES_crc32() == 0x1d2e5018		// Rockman 3(J)
	 || NES_crc32() == 0x6b999aaf ) {	// Megaman 3(U)
		g_NESmapper.Mapper4.irq_type = MMC3_IRQ_ROCKMAN3;
	}

	if( NES_crc32() == 0xd88d48d7 ) {	// Kick Master(U)
		g_NESmapper.Mapper4.irq_type = MMC3_IRQ_ROCKMAN3;
	}

	if( NES_crc32() == 0xe763891b ) {	// DBZ2
		g_NESmapper.Mapper4.irq_type = MMC3_IRQ_DBZ2;
	}

	// VS-Unisystem
	g_NESmapper.Mapper4.vs_patch = 0;
	g_NESmapper.Mapper4.vs_index = 0;

	if( NES_crc32() == 0xeb2dba63		// VS TKO Boxing
	 || NES_crc32() == 0x98cfe016 ) {	// VS TKO Boxing (Alt)
		g_NESmapper.Mapper4.vs_patch = 1;
	}
	if( NES_crc32() == 0x135adf7c ) {	// VS Atari RBI Baseball
		g_NESmapper.Mapper4.vs_patch = 2;
	}
	if( NES_crc32() == 0xf9d3b0a3		// VS Super Xevious
	 || NES_crc32() == 0x9924980a		// VS Super Xevious (b1)
	 || NES_crc32() == 0x66bb838f ) {	// VS Super Xevious (b2)
		g_NESmapper.Mapper4.vs_patch = 3;
	}
	
	g_NESmapper.Mapper4.wram = g_NES.mapper_extram;
	g_NES.mapper_extramsize = 0x2000;		
	g_NESmapper.SetPROM_Bank( 2, g_NESmapper.Mapper4.wram);
}

uint8	NES_mapper4_MemoryReadLow( uint32 addr )
{
	if( !g_NESmapper.Mapper4.vs_patch ) {
		if(g_NESmapper.Mapper4.rom_type == 1){
			if( addr >= 0x4020 && addr <= 0x7FFF ) {
				return g_NESmapper.Mapper4.security;
			}
		} else if( addr >= 0x5000 && addr <= 0x5FFF ) {
				return	g_NESmapper.Mapper4.wram[addr-0x4000];
		}
	} else if( g_NESmapper.Mapper4.vs_patch == 1 ) {
		// VS TKO Boxing g_NESmapper.Mapper4.security
		if( addr == 0x5E00 ) {
			g_NESmapper.Mapper4.vs_index = 0;
			return	0x00;
		} else if( addr == 0x5E01 ) {
			uint8	VS_TKO_Security[32] = {
				0xff, 0xbf, 0xb7, 0x97, 0x97, 0x17, 0x57, 0x4f,
				0x6f, 0x6b, 0xeb, 0xa9, 0xb1, 0x90, 0x94, 0x14,
				0x56, 0x4e, 0x6f, 0x6b, 0xeb, 0xa9, 0xb1, 0x90,
				0xd4, 0x5c, 0x3e, 0x26, 0x87, 0x83, 0x13, 0x00 };
			return	VS_TKO_Security[(g_NESmapper.Mapper4.vs_index++) & 0x1F];
		}
	} else if( g_NESmapper.Mapper4.vs_patch == 2 ) {
		// VS Atari RBI Baseball g_NESmapper.Mapper4.security
		if( addr == 0x5E00 ) {
			g_NESmapper.Mapper4.vs_index = 0;
			return	0x00;
		} else if( addr == 0x5E01 ) {
			if( g_NESmapper.Mapper4.vs_index++ == 9 )
				return	0x6F;
			else
				return	0xB4;
		}
	} else if( g_NESmapper.Mapper4.vs_patch == 3 ) {
		// VS Super Xevious
		switch( addr ) {
			case	0x54FF:
				return	0x05;
			case	0x5678:
				if( g_NESmapper.Mapper4.vs_index )
					return	0x00;
				else
					return	0x01;
				break;
			case	0x578f:
				if( g_NESmapper.Mapper4.vs_index )
					return	0xD1;
				else
					return	0x89;
				break;
			case	0x5567:
				if( g_NESmapper.Mapper4.vs_index ) {
					g_NESmapper.Mapper4.vs_index = 0;
					return	0x3E;
				} else {
					g_NESmapper.Mapper4.vs_index = 1;
					return	0x37;
				}
				break;
			default:
				break;
		}
	}

	//return	Mapper::ReadLow( addr );
	return  (uint8)(addr >> 8);
}

void NES_mapper4_MemoryWriteLow(uint32 addr, uint8 data)
{
	if(g_NESmapper.Mapper4.rom_type == 1){
		if( addr >= 0x4020 && addr <= 0x7FFF ) {
			g_NESmapper.Mapper4.security = data & 3;
			if (g_NESmapper.Mapper4.security == 1){
				g_NESmapper.Mapper4.security = 2;
			}
		}
	} else if( addr >= 0x5000 && addr <= 0x5FFF ) {
		g_NESmapper.Mapper4.wram[addr-0x4000] = data;
	} else {
		;//Mapper::WriteLow( addr, data );
	}
}

void NES_mapper4_MemoryWrite(uint32 addr, uint8 data)
{
	switch( addr & 0xE001 ) {
		case	0x8000:
			g_NESmapper.Mapper4.reg[0] = data;
			NES_mapper4_MMC3_set_CPU_banks();
			NES_mapper4_MMC3_set_PPU_banks();
			break;
		case	0x8001:
			g_NESmapper.Mapper4.reg[1] = data;

			switch( g_NESmapper.Mapper4.reg[0] & 0x07 ) {
				case	0x00:
					g_NESmapper.Mapper4.chr01 = data & 0xFE;
					NES_mapper4_MMC3_set_PPU_banks();
					break;
				case	0x01:
					g_NESmapper.Mapper4.chr23 = data & 0xFE;
					NES_mapper4_MMC3_set_PPU_banks();
					break;
				case	0x02:
					g_NESmapper.Mapper4.chr4 = data;
					NES_mapper4_MMC3_set_PPU_banks();
					break;
				case	0x03:
					g_NESmapper.Mapper4.chr5 = data;
					NES_mapper4_MMC3_set_PPU_banks();
					break;
				case	0x04:
					g_NESmapper.Mapper4.chr6 = data;
					NES_mapper4_MMC3_set_PPU_banks();
					break;
				case	0x05:
					g_NESmapper.Mapper4.chr7 = data;
					NES_mapper4_MMC3_set_PPU_banks();
					break;
				case	0x06:
					g_NESmapper.Mapper4.prg0 = data;
					NES_mapper4_MMC3_set_CPU_banks();
					break;
				case	0x07:
					g_NESmapper.Mapper4.prg1 = data;
					NES_mapper4_MMC3_set_CPU_banks();
					break;
			}
			break;
		case	0xA000:
			g_NESmapper.Mapper4.reg[2] = data;
			//if( !nes->rom->Is4SCREEN() ) 
			if(NES_ROM_get_mirroring() != NES_PPU_MIRROR_FOUR_SCREEN)
			{
				if( data & 0x01 ) //SetVRAM_Mirror( VRAM_HMIRROR );
					g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
				else		  //SetVRAM_Mirror( VRAM_VMIRROR );
					g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
			}
			break;
		case	0xA001:
			g_NESmapper.Mapper4.reg[3] = data;
			break;
		case	0xC000:
			g_NESmapper.Mapper4.reg[4] = data;
			if( g_NESmapper.Mapper4.irq_type == MMC3_IRQ_KLAX || g_NESmapper.Mapper4.irq_type == MMC3_IRQ_ROCKMAN3 ) {
				g_NESmapper.Mapper4.irq_counter = data;
			} else {
				g_NESmapper.Mapper4.irq_latch = data;
			}
			if( g_NESmapper.Mapper4.irq_type == MMC3_IRQ_DBZ2 ) {
				g_NESmapper.Mapper4.irq_latch = 0x07;
			}
			break;
		case	0xC001:
			g_NESmapper.Mapper4.reg[5] = data;
			if( g_NESmapper.Mapper4.irq_type == MMC3_IRQ_KLAX || g_NESmapper.Mapper4.irq_type == MMC3_IRQ_ROCKMAN3 ) {
				g_NESmapper.Mapper4.irq_latch = data;
			} else {
				if( (NES_GetScanline() < 240) || (g_NESmapper.Mapper4.irq_type == MMC3_IRQ_SHOUGIMEIKAN) ) {
					g_NESmapper.Mapper4.irq_counter |= 0x80;
					g_NESmapper.Mapper4.irq_preset = 0xFF;
				} else {
					g_NESmapper.Mapper4.irq_counter |= 0x80;
					g_NESmapper.Mapper4.irq_preset_vbl = 0xFF;
					g_NESmapper.Mapper4.irq_preset = 0;
				}
			}
			break;
		case	0xE000:
			g_NESmapper.Mapper4.reg[6] = data;
			g_NESmapper.Mapper4.irq_enable = 0;
			g_NESmapper.Mapper4.irq_request = 0;

			//nes->cpu->ClrIRQ( IRQ_MAPPER );
			nes6502_ClrIRQ( IRQ_MAPPER );
			break;
		case	0xE001:
			g_NESmapper.Mapper4.reg[7] = data;
			g_NESmapper.Mapper4.irq_enable = 1;
			g_NESmapper.Mapper4.irq_request = 0;
			break;
	}
}


void NES_mapper4_HSync(uint32 scanline)
{
	if( g_NESmapper.Mapper4.irq_type == MMC3_IRQ_KLAX ) 
	{
		if( (scanline >= 0 && scanline <= 239) && (NES_PPU_spr_enabled() || NES_PPU_bg_enabled()) ) {
			if( g_NESmapper.Mapper4.irq_enable ) {
				if( g_NESmapper.Mapper4.irq_counter == 0 ) {
					g_NESmapper.Mapper4.irq_counter = g_NESmapper.Mapper4.irq_latch;
					g_NESmapper.Mapper4.irq_request = 0xFF;
				}
				if( g_NESmapper.Mapper4.irq_counter > 0 ) {
					g_NESmapper.Mapper4.irq_counter--;
				}
			}
		}
		if( g_NESmapper.Mapper4.irq_request ) {
			//nes->cpu->SetIRQ( IRQ_MAPPER );
			nes6502_SetIRQ( IRQ_MAPPER );
		}
	}
	else if( g_NESmapper.Mapper4.irq_type == MMC3_IRQ_ROCKMAN3 ) 
	{
		if( (scanline >= 0 && scanline <= 239) && (NES_PPU_spr_enabled() || NES_PPU_bg_enabled()) ) {
			if( g_NESmapper.Mapper4.irq_enable ) {
				if( !(g_NESmapper.Mapper4.irq_counter--) ) {
					g_NESmapper.Mapper4.irq_request = 0xFF;
					g_NESmapper.Mapper4.irq_counter = g_NESmapper.Mapper4.irq_latch;
				}
			}
		}
		if( g_NESmapper.Mapper4.irq_request ) {
			//nes->cpu->SetIRQ( IRQ_MAPPER );
			nes6502_SetIRQ( IRQ_MAPPER );
		}
	}
	else 
	{
		if( (scanline >=0) && (scanline <= 239) )
		if(  NES_PPU_spr_enabled() || NES_PPU_bg_enabled() ) 
		{
				if( g_NESmapper.Mapper4.irq_preset_vbl ) 
				{
					g_NESmapper.Mapper4.irq_counter = g_NESmapper.Mapper4.irq_latch;
					g_NESmapper.Mapper4.irq_preset_vbl = 0;
				}
				if( g_NESmapper.Mapper4.irq_preset ) 
				{
					g_NESmapper.Mapper4.irq_counter = g_NESmapper.Mapper4.irq_latch;
					g_NESmapper.Mapper4.irq_preset = 0;
					if( g_NESmapper.Mapper4.irq_type == MMC3_IRQ_DAI2JISUPER && scanline == 0 ) {
						g_NESmapper.Mapper4.irq_counter--;
					}
				}
				else if( g_NESmapper.Mapper4.irq_counter > 0 ) 
				{
					g_NESmapper.Mapper4.irq_counter--;
				}

				if( g_NESmapper.Mapper4.irq_counter == 0 ) 
				{
					if( g_NESmapper.Mapper4.irq_enable ) 
					{
						g_NESmapper.Mapper4.irq_request = 0xFF;
						//nes->cpu->SetIRQ( IRQ_MAPPER );
						nes6502_SetIRQ( IRQ_MAPPER );
					}
					g_NESmapper.Mapper4.irq_preset = 0xFF;
				}			
		}
	}
}

void NES_mapper4_MMC3_set_CPU_banks()
{
	if( g_NESmapper.Mapper4.reg[0] & 0x40 ) {
		g_NESmapper.SetPROM_32K_Banks( g_NESmapper.num_8k_ROM_banks-2, g_NESmapper.Mapper4.prg1, g_NESmapper.Mapper4.prg0, g_NESmapper.num_8k_ROM_banks-1 );
	} else {
		g_NESmapper.SetPROM_32K_Banks( g_NESmapper.Mapper4.prg0, g_NESmapper.Mapper4.prg1, g_NESmapper.num_8k_ROM_banks-2, g_NESmapper.num_8k_ROM_banks-1 );
	}
}

void NES_mapper4_MMC3_set_PPU_banks()
{
	if( g_NESmapper.num_1k_VROM_banks ) {
		if( g_NESmapper.Mapper4.reg[0] & 0x80 ) {
			if(g_NESmapper.Mapper4.rom_type == 2){
				NES_mapper4_MMC3_set_PPU_banksSUB( 4, g_NESmapper.Mapper4.chr01+0 );
				NES_mapper4_MMC3_set_PPU_banksSUB( 5, g_NESmapper.Mapper4.chr01+1 );
				NES_mapper4_MMC3_set_PPU_banksSUB( 6, g_NESmapper.Mapper4.chr23+0 );
				NES_mapper4_MMC3_set_PPU_banksSUB( 7, g_NESmapper.Mapper4.chr23+1 );
				NES_mapper4_MMC3_set_PPU_banksSUB( 0, g_NESmapper.Mapper4.chr4 );
				NES_mapper4_MMC3_set_PPU_banksSUB( 1, g_NESmapper.Mapper4.chr5 );
				NES_mapper4_MMC3_set_PPU_banksSUB( 2, g_NESmapper.Mapper4.chr6 );
				NES_mapper4_MMC3_set_PPU_banksSUB( 3, g_NESmapper.Mapper4.chr7 );
			} else {
				g_NESmapper.SetVROM_8K_Banks( g_NESmapper.Mapper4.chr4, g_NESmapper.Mapper4.chr5, g_NESmapper.Mapper4.chr6, g_NESmapper.Mapper4.chr7,
						 g_NESmapper.Mapper4.chr01, g_NESmapper.Mapper4.chr01+1, g_NESmapper.Mapper4.chr23, g_NESmapper.Mapper4.chr23+1 );
			}
		} else {
			if(g_NESmapper.Mapper4.rom_type == 2){
				NES_mapper4_MMC3_set_PPU_banksSUB( 0, g_NESmapper.Mapper4.chr01+0 );
				NES_mapper4_MMC3_set_PPU_banksSUB( 1, g_NESmapper.Mapper4.chr01+1 );
				NES_mapper4_MMC3_set_PPU_banksSUB( 2, g_NESmapper.Mapper4.chr23+0 );
				NES_mapper4_MMC3_set_PPU_banksSUB( 3, g_NESmapper.Mapper4.chr23+1 );
				NES_mapper4_MMC3_set_PPU_banksSUB( 4, g_NESmapper.Mapper4.chr4 );
				NES_mapper4_MMC3_set_PPU_banksSUB( 5, g_NESmapper.Mapper4.chr5 );
				NES_mapper4_MMC3_set_PPU_banksSUB( 6, g_NESmapper.Mapper4.chr6 );
				NES_mapper4_MMC3_set_PPU_banksSUB( 7, g_NESmapper.Mapper4.chr7 );
			} else {
				g_NESmapper.SetVROM_8K_Banks( g_NESmapper.Mapper4.chr01, g_NESmapper.Mapper4.chr01+1, g_NESmapper.Mapper4.chr23, g_NESmapper.Mapper4.chr23+1,
						 g_NESmapper.Mapper4.chr4, g_NESmapper.Mapper4.chr5, g_NESmapper.Mapper4.chr6, g_NESmapper.Mapper4.chr7 );
			}
		}
	} else {
		if( g_NESmapper.Mapper4.reg[0] & 0x80 ) {
			g_NESmapper.SetCRAM_1K_Bank( 4, (g_NESmapper.Mapper4.chr01+0)&0x07 );
			g_NESmapper.SetCRAM_1K_Bank( 5, (g_NESmapper.Mapper4.chr01+1)&0x07 );
			g_NESmapper.SetCRAM_1K_Bank( 6, (g_NESmapper.Mapper4.chr23+0)&0x07 );
			g_NESmapper.SetCRAM_1K_Bank( 7, (g_NESmapper.Mapper4.chr23+1)&0x07 );
			g_NESmapper.SetCRAM_1K_Bank( 0, g_NESmapper.Mapper4.chr4&0x07 );
			g_NESmapper.SetCRAM_1K_Bank( 1, g_NESmapper.Mapper4.chr5&0x07 );
			g_NESmapper.SetCRAM_1K_Bank( 2, g_NESmapper.Mapper4.chr6&0x07 );
			g_NESmapper.SetCRAM_1K_Bank( 3, g_NESmapper.Mapper4.chr7&0x07 );
		} else {
			g_NESmapper.SetCRAM_1K_Bank( 0, (g_NESmapper.Mapper4.chr01+0)&0x07 );
			g_NESmapper.SetCRAM_1K_Bank( 1, (g_NESmapper.Mapper4.chr01+1)&0x07 );
			g_NESmapper.SetCRAM_1K_Bank( 2, (g_NESmapper.Mapper4.chr23+0)&0x07 );
			g_NESmapper.SetCRAM_1K_Bank( 3, (g_NESmapper.Mapper4.chr23+1)&0x07 );
			g_NESmapper.SetCRAM_1K_Bank( 4, g_NESmapper.Mapper4.chr4&0x07 );
			g_NESmapper.SetCRAM_1K_Bank( 5, g_NESmapper.Mapper4.chr5&0x07 );
			g_NESmapper.SetCRAM_1K_Bank( 6, g_NESmapper.Mapper4.chr6&0x07 );
			g_NESmapper.SetCRAM_1K_Bank( 7, g_NESmapper.Mapper4.chr7&0x07 );
		}
	}
}

void NES_mapper4_MMC3_set_PPU_banksSUB( int bank, int page )
{
	if(  page < 8 ) {
		g_NESmapper.SetCRAM_1K_Bank( bank, page );
	} else {
		g_NESmapper.SetVROM_1K_Bank( bank, page );
	}
}