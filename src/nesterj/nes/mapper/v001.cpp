//////////////////////////////////////////////////////////////////////////
// Mapper001  Nintendo MMC1                                             //
//////////////////////////////////////////////////////////////////////////

void VNES_mapper1_Init()
{
	g_NESmapper.Reset = VNES_mapper1_Reset;
	//g_NESmapper.MemoryWriteSaveRAM = VNES_mapper1_MemoryWriteSaveRAM;
	g_NESmapper.MemoryWrite = VNES_mapper1_MemoryWrite;
}


void	VNES_mapper1_Reset()
{
	g_NESmapper.vnes01.reg[0] = 0x0C;		// D3=1,D2=1
	g_NESmapper.vnes01.reg[1] = 
	g_NESmapper.vnes01.reg[2] = 
	g_NESmapper.vnes01.reg[3] = 0;
	g_NESmapper.vnes01.shift = 
	g_NESmapper.vnes01.regbuf = 0;

	g_NESmapper.vnes01.patch = 0;
	g_NESmapper.vnes01.wram_patch = 0;

	if( NES_ROM_get_num_16k_ROM_banks() < 32 ) {
		g_NESmapper.SetPROM_32K_Banks( 0, 1, g_NESmapper.num_8k_ROM_banks-2, g_NESmapper.num_8k_ROM_banks-1 );
	} else {
		g_NESmapper.SetPROM_16K_Bank( 4, 0 );
		g_NESmapper.SetPROM_16K_Bank( 6, 16-1 );

		g_NESmapper.vnes01.patch = 1;
	}
}

void	VNES_mapper1_MemoryWrite( uint32 addr, uint8 data )
{
	if((addr & 0x6000) != (g_NESmapper.vnes01.last_addr & 0x6000))
	{
			g_NESmapper.vnes01.shift = g_NESmapper.vnes01.regbuf = 0;
	}
	g_NESmapper.vnes01.last_addr = addr;
	

	if( data & 0x80 ) 
	{
		g_NESmapper.vnes01.shift = g_NESmapper.vnes01.regbuf = 0;
		g_NESmapper.vnes01.reg[0] |= 0x0C;		// D3=1,D2=1 残りはリセットされない
		return;
	}

	if( data&0x01 ) g_NESmapper.vnes01.regbuf |= 1<<g_NESmapper.vnes01.shift;
	if( ++g_NESmapper.vnes01.shift < 5 )
		return;
	addr = (addr&0x7FFF)>>13;
	g_NESmapper.vnes01.reg[addr] = g_NESmapper.vnes01.regbuf;

	g_NESmapper.vnes01.regbuf = 0;
	g_NESmapper.vnes01.shift = 0;

	if( g_NESmapper.vnes01.patch != 1 ) 
	{
		switch( addr ) 
		{
			case	0:
				if( g_NESmapper.vnes01.reg[0] & 0x02 ) {
					if( g_NESmapper.vnes01.reg[0] & 0x01 ) 
					g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
					else		    
					g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
				} else {
					if( g_NESmapper.vnes01.reg[0] & 0x01 ) 
					g_NESmapper.set_mirroring(1,1,1,1);
					else
					g_NESmapper.set_mirroring(0,0,0,0);
				}
				break;
			case	1:
				// Register #1
				if( g_NESmapper.num_1k_VROM_banks ) {
					if( g_NESmapper.vnes01.reg[0] & 0x10 ) {
						g_NESmapper.SetVROM_4K_Bank( 0, g_NESmapper.vnes01.reg[1] );
						g_NESmapper.SetVROM_4K_Bank( 4, g_NESmapper.vnes01.reg[2] );
					} else {
						// CHR 8K bank($0000-$1FFF)
						g_NESmapper.SetVROM_8K_Bank( g_NESmapper.vnes01.reg[1]>>1 );
					}
				} else {
					// For Romancia
					if( g_NESmapper.vnes01.reg[0] & 0x10 ) {
						g_NESmapper.SetCRAM_4K_Bank( 0, g_NESmapper.vnes01.reg[1] );
					}
				}
				break;
			case	2:
				// Register #2
				if( g_NESmapper.num_1k_VROM_banks ) {
					if(  g_NESmapper.vnes01.reg[0] & 0x10 ) {
						// CHR 4K bank lower($0000-$0FFF)
						g_NESmapper.SetVROM_4K_Bank( 0, g_NESmapper.vnes01.reg[1] );
						// CHR 4K bank higher($1000-$1FFF)
						g_NESmapper.SetVROM_4K_Bank( 4, g_NESmapper.vnes01.reg[2] );
					} else {
						// CHR 8K bank($0000-$1FFF)
						g_NESmapper.SetVROM_8K_Bank( g_NESmapper.vnes01.reg[1]>>1 );
					}
				} else {
					// For Romancia
					if( g_NESmapper.vnes01.reg[0] & 0x10 ) {
						g_NESmapper.SetCRAM_4K_Bank( 4, g_NESmapper.vnes01.reg[2] );
					}
				}
				break;
			case	3:
				if( !(g_NESmapper.vnes01.reg[0] & 0x08) ) {
				// PRG 32K bank ($8000-$FFFF)
					g_NESmapper.SetPROM_32K_Bank( g_NESmapper.vnes01.reg[3]>>1 );
				} else {
					if( g_NESmapper.vnes01.reg[0] & 0x04 ) {
					// PRG 16K bank ($8000-$BFFF)
						g_NESmapper.SetPROM_16K_Bank( 4, g_NESmapper.vnes01.reg[3] );
						g_NESmapper.SetPROM_16K_Bank( 6, NES_ROM_get_num_16k_ROM_banks()-1 );
					} else {
					// PRG 16K bank ($C000-$FFFF)
						g_NESmapper.SetPROM_16K_Bank( 6, g_NESmapper.vnes01.reg[3] );
						g_NESmapper.SetPROM_16K_Bank( 4, 0 );
					}
				}
				break;
		}
	} else {
		// For 512K/1M byte Cartridge
		int	PROM_BASE = 0;
		if( NES_ROM_get_num_16k_ROM_banks() >= 32 ) {
			PROM_BASE = g_NESmapper.vnes01.reg[1] & 0x10;
		}
		
		// Register #0
		if( addr == 0 ) {
			if(  g_NESmapper.vnes01.reg[0] & 0x02 ) {
				if(  g_NESmapper.vnes01.reg[0] & 0x01 ) 
					g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
				else		    
					g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
			} else {
				if(  g_NESmapper.vnes01.reg[0] & 0x01 ) 
					g_NESmapper.set_mirroring(1,1,1,1);
				else		    
					g_NESmapper.set_mirroring(0,0,0,0);
			}
		}
		// Register #1
		if( g_NESmapper.num_1k_VROM_banks ) {
			if( g_NESmapper.vnes01.reg[0] & 0x10 ) {
				g_NESmapper.SetVROM_4K_Bank( 0, g_NESmapper.vnes01.reg[1] );
			} else {
				g_NESmapper.SetVROM_8K_Bank( g_NESmapper.vnes01.reg[1]>>1 );
			}
		} else {
			if( g_NESmapper.vnes01.reg[0] & 0x10 ) {
				g_NESmapper.SetCRAM_4K_Bank( 0, g_NESmapper.vnes01.reg[1] );
			}
		}
		// Register #2
		if( g_NESmapper.num_1k_VROM_banks ) {
			if( g_NESmapper.vnes01.reg[0] & 0x10 ) {
			// CHR 4K bank higher($1000-$1FFF)
				g_NESmapper.SetVROM_4K_Bank( 4, g_NESmapper.vnes01.reg[2] );
			}
		} else {
			// For Romancia
			if( g_NESmapper.vnes01.reg[0] & 0x10 ) {
				g_NESmapper.SetCRAM_4K_Bank( 4, g_NESmapper.vnes01.reg[2] );
			}
		}
		// Register #3
		if( !( g_NESmapper.vnes01.reg[0] & 0x08) ) {
		// PRG 32K bank ($8000-$FFFF)
			g_NESmapper.SetPROM_32K_Bank( (g_NESmapper.vnes01.reg[3]&(0xF+PROM_BASE))>>1 );
		} else {
			if(  g_NESmapper.vnes01.reg[0] & 0x04 ) {
			// PRG 16K bank ($8000-$BFFF)
				g_NESmapper.SetPROM_16K_Bank( 4, PROM_BASE+(g_NESmapper.vnes01.reg[3]&0x0F) );
				if( NES_ROM_get_num_16k_ROM_banks() >= 32 )
					g_NESmapper.SetPROM_16K_Bank( 6, PROM_BASE+16-1 );
			} else {
			// PRG 16K bank ($C000-$FFFF)
				g_NESmapper.SetPROM_16K_Bank( 6, PROM_BASE+(g_NESmapper.vnes01.reg[3]&0x0F) );
				if( NES_ROM_get_num_16k_ROM_banks() >= 32 )
					g_NESmapper.SetPROM_16K_Bank( 4, PROM_BASE );
			}
		}
	}
}